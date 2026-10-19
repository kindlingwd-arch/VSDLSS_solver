# 2026-10-19 求解器接管时间步进：P1 与 P3/P4 实现

设计见 [transient-stepping-design-20261018](transient-stepping-design-20261018.md)（前提和接口按 PR #61 修订后的版本）。本文记录实现、正确性验证和本机实测。

## 结论

- 新接口 `vsdlss_m3_transient_*`：求解器自己按后向欧拉推进 A x_{k+1} = Ch x_k + s_{k+1}，全程使用打包顺序，每步没有收集和写回。
- 能融合的分量，把第 k 步回代和第 k+1 步前代按单元（尾部连通块、块并行段）交错执行，消元记录每步从内存读一遍。
- **每一步的解和"按同一公式算 b、逐步调用 `vsdlss_m3_solve_packed`"逐位一致**。融合和不融合、批量大小、线程数都不影响结果。
- 本机（WSL2 笔记本，4 线程，252 万结点级合成网格，每步 30% 结点有电流源）每步耗时：

| 网格 | 现在的用法（调用者顺序 + `vsdlss_m3_solve`） | 新接口，不融合 | 新接口，融合 |
|---|---|---|---|
| grid:600:3 | 53.2 ms | 41.4 ms | 38.8 ms |
| grid:300:20 | 52.9 ms | 37.8 ms | 33.4 ms |
| grid:150:80 | 52.8 ms | 38.5 ms | **30.1 ms（−43%）** |
| grid:600:3，块 4096（尾部 76 万条） | 58.4 ms | 44.6 ms | 42.5 ms |
| grid:150:80，块 4096（尾部 35 万条） | 48.4 ms | 34.8 ms | **25.5 ms（−47%）** |
| grid:600:3，块 1024（尾部 200 万条） | 55.7 ms | 47.8 ms | 41.4 ms |
| grid:150:80，块 1024（尾部 139 万条） | 56.8 ms | 41.3 ms | 33.9 ms |

  上表所有行的结果都和"现在的用法"逐位一致（`bench_tr` 检查）。

- 线上用例需要在服务器上实测，见第 5 节。

## 1. 接口

```c
vsdlss_status vsdlss_m3_transient_create(const vsdlss_m3_factor *F, const vsdlss *Ch,
        csi nsrc, const csi *src_node, csi ndep, const csi *dep_node, vsdlss_m3_transient **T);
vsdlss_status vsdlss_m3_transient_start(vsdlss_m3_transient *T, const double *x0);
vsdlss_status vsdlss_m3_transient_run(vsdlss_m3_transient *T, csi nsteps, const double *src,
        void (*cb)(void *ctx, csi k, const double *x_packed, double *dep_next), void *ctx);
vsdlss_status vsdlss_m3_transient_get(const vsdlss_m3_transient *T, csi nidx, const csi *node, double *out);
vsdlss_status vsdlss_m3_transient_info(const vsdlss_m3_transient *T, csi *fused, csi *units, csi *late);
void vsdlss_m3_transient_free(vsdlss_m3_transient *T);
```

- `Ch` = C/h，上三角 CSC，调用者编号。对角是接地电容；非对角是两端电容（例如 PwlCap 的 n–out），两端必须在同一个连通分量里。
- `src_node`：每步有电流源的结点，一次登记，可以重复，重复的数值相加。`run` 的 `src` 按行给出每一步的数值。电流源很多时，按打包位置排序登记，每步写入 s 的访存更顺序。
- `dep_node`：右端项依赖调用者自己维护的状态的结点，例如封装 RL、SRC2 的伴随模型。每一步完成后由回调给出下一步的数值。
- 回调：每一步完成后调用一次（第一次 `run` 前还会为起始状态调用一次）。参数 `x_packed` 是打包顺序的完整解，调用期间有效，结点 i 在 `vsdlss_m3_export_packed_slots` 给出的 slot[i] 处。
- 右端项公式（固定，用于逐位回归）：
  - b[v] = Ch[v][v]·x[v]，再加上 Ch[v][u]·x[u]（u 按打包位置升序），再加上 s[v]；
  - s[v] 从 0.0 开始，先按登记顺序加这一步的电流源，再按 dep 顺序加回调给出的值。
- dt 变化、电源域切换导致 A 改变时：重新分解（或少量电导变化用 `vsdlss_m3_modify`），再重新 create、start。

## 2. 执行流程

每一次迭代完成第 k 步、准备第 k+1 步（每个分量）：

| 阶段 | 能融合的分量 | 其他分量 |
|---|---|---|
| A | ① core 求解（w[core] 拷到 x[core]，原地求解）；非特殊 core 结点 w = d·x + s<br>② 尾部连通块并行：回代 k → 本块结点 w = d·x + s → 非晚做的立即前代 k+1（对 core 的延后更新进缓冲区）<br>③ 块并行段并行：同上 | —（这一步已经在上一次的 B 里解完） |
| 回调 | 拿到第 k 步的完整解，给出第 k+1 步的 dep 值 | |
| B | 特殊结点的 w；晚做的块、晚做的连通块前代 k+1；按消元顺序补减延后的 core 更新 | 单独解第 k+1 步（b → 前代 → core → 回代，与 `vsdlss_m3_solve_packed_inplace` 相同的算术） |

- 分量之间用 `run_components` 并发：VDD 和 GND 各分一半线程，和求解时一样。
- 向量：x（解）、w（工作向量）、s（电流源）、d（Ch 的对角），都是打包顺序，共 4 个 n 维 double，每个结点 32 字节。6100 万结点约 2 GB。
- 设计里的 P2（b 在回代后顺手算出）在融合路径里已经做了：每个单元回代完，马上在缓存里算本单元的 b。

### 2.1 什么情况下单元要晚做

一个单元的前代要等所有回代完成（放到 B 阶段），满足以下任一条件即可：

- 单元里有特殊结点：两端电容的端点或 dep 结点，它们的 b 需要别的结点的解；
- 块更新了某个特殊结点；
- 对连通块而言，它的某个结点被块更新过（块的更新必须先于连通块的前代，这是数据依赖）；
- 对连通块而言，它直接更新的某个 core 结点也被块更新过（要保持"先块、后尾部"的更新顺序，才能逐位一致）。

`VSDLSS_TRACE=1` 时，`create` 会打印每个分量的晚做统计和原因分项。例如：

```
vsdlss trace: transient fusion: 98 chunks, late 98 (special 0, updated by a block 98, direct core update 0);
blocks late 0 of 3515; blocks updating tail vertices of one chunk 1247, of several 2268
```

本机合成网格的块很多、连通块少而大，几乎所有连通块都被块碰到，因此全部晚做，融合的收益主要来自块并行段。线上用例只有 504 个块，却有 6209 个连通块（见 [solve-overlap-pipeline-20261017](solve-overlap-pipeline-20261017.md)），预计大部分连通块不会晚做。需要用上面的 trace 在服务器上确认。

### 2.2 能融合的条件

- 打包的低度消元，且做了求解序重编号；
- 有块并行段，尾部是单窗口调度（默认配置下就是这样）；
- core 是连续的。

不满足的分量（小分量、`VSDLSS_RELABEL=0`、按层调度的尾部、多窗口）走不融合的路径，结果一样。`VSDLSS_TR_FUSE=0` 可以关闭融合，用来做 A/B 对比。

## 3. 正确性

逐位一致的依据：

- 每条记录的算术和 `vsdlss_reduce_forward_inplace` / `backward_inplace` 完全相同。回代从 w 读主元值、写入 x，和原地求解读写的是同一个数。
- 每个向量元素受到的运算顺序不变：先赋值为 b，再受块的更新，再受尾部的更新；延后的 core 更新仍按消元顺序补减。2.1 节的晚做条件就是为了保证这个顺序。
- core 求解用的是同一个函数、同样的右端项。

`test/test_transient.c`（已加入 `make test` 和 `make sanitizers`）：

- A = G + Ch：接地电阻网络，两个网格加导线链和悬挂结点，再加一个小分量，调用者编号随机打乱。Ch 包括每个结点的接地电容，以及 12 个两端电容。30% 的结点有电流源（部分重复登记），另有 9 个 dep 结点，回调按解算出它们的值。
- 参照：按文档公式算 b，用 `vsdlss_m3_solve_packed` 逐步求解。新接口每一步的解（包括起始状态）都要求和参照逐位一致，`get` 的结果也要一致。
- 覆盖：一批 12 步、逐步 12 次、3+9 两批；融合与不融合；重编号开/关；窗口调度/按层调度；1/4 线程。融合场景还要求确实走了融合，而且有晚做的单元。
- 非法输入：跨分量的两端电容、越界结点、NULL 因子、start 之前 run、有 dep 结点却没有回调、NaN 电流源、非有限的 x0。
- `create` 的分配失败扫描（38 个点）：每个点都返回 OOM，没有泄漏。
- 对照实验：
  - 关掉晚做判定后，第 2 步开始和参照不一致；
  - 计算 b 时漏掉非对角项后，有 48 处不一致。

## 4. 基准：`test/bench_tr.c`

```bash
make bench_tr
./bench_tr grid:600:3 5 4 20                     # 输入、排序、线程、步数
TR_BLOCK=1024 ./bench_tr grid:150:80 5 4 20      # 小块：尾部占主导
VSDLSS_TRACE=1 ./bench_tr text:/path/to/case 6 32 20   # 真实用例，同时打印晚做统计
```

程序依次比较四种做法，并检查结果是否逐位一致：

1. `orig`：调用者顺序算 b，再调用 `vsdlss_m3_solve`；
2. `packed`：打包顺序手工算 b，再调用 `solve_packed_inplace`；
3. `tr`：新接口，不融合；
4. `tr+fuse`：新接口，融合。

参数：Ch = TR_CH × diag(A)（默认 0.1），电流源占 TR_SRC（默认 30%）的结点。

## 5. 公开基准：IBM 瞬态电源网格 ibmpg1t–6t

`test/bench_ibmpg_tr.c`（`make bench_ibmpg_tr`）读取 IBM 瞬态网表（Nassif，ASP-DAC 2008，托管在 <https://web.ece.ucsb.edu/~lip/PGBenchmarks/ibmpgbench.html>，不随仓库分发），用新接口做后向欧拉，并做三层检查：

1. 三种做法逐位一致：手工逐步（按接口公式算 b，再调用 `solve_packed_inplace`）、新接口不融合、新接口融合；
2. 和官方波形比较（`.print` 列出的 20 个结点、每个时间点）；
3. 每步耗时。

```bash
make bench_ibmpg_tr
./bench_ibmpg_tr ibmpg1t.spice.bz2 ibmpg1t.output.bz2
PG_BLOCK=2048 PG_TAILMIN=16 VSDLSS_TRACE=1 ./bench_ibmpg_tr ibmpg1t.spice.bz2 ibmpg1t.output.bz2   # 小用例也走融合
```

网表结构（ibmpg1t）和建模方式：

| 元件 | 内容 | 处理 |
|---|---|---|
| R（大写） | 栅格导线 | G |
| r（小写） | 封装电阻 0.25 Ω，decap 的串联电阻 | G |
| c | decap：VDD 侧"n1 —r— _Z_ —c— 地"（接地）；**GND 侧"地 —r— _Z_ —c— n0"（两端电容，3381 个）** | C/h 同时进 A 和 Ch；两端电容是 Ch 的非对角项 |
| l | 封装电感 1 nH，"栅格 —r— _X_ —l— _Y_ —v— 地"，277 个 | 伴随模型：h/L 进 A；历史电流作为 dep 结点，由回调维护 |
| v | 焊盘 0 V / 1.8 V（Dirichlet）；大写 V 是 0 V 过孔短路（合并结点） | 焊盘对应的项作为常数电流源 |
| i | `pulse(i1, i2, td, tr, tf, pw, per)` 负载电流 | 每步按 t_{k+1} 求值 |
| `.tran 10p 10n` | 1000 步 | |

x₀ 是直流解（电容开路、电感短路，电流源取 t=0 时刻的值），电感的初始电流在直流解上用 KCL 求出。

ibmpg1t 的结果（本机，1 线程）：

- 2.5 万个未知数，Ch 有 3381 个非对角项，277 个 dep 结点。
- **三种做法逐位一致**。在 `PG_BLOCK=2048 PG_TAILMIN=16` 下，一个分量走了融合（另一个分量的尾部是 3 个窗口，不满足融合条件），结果同样逐位一致。
- **和官方波形比较**：t=0（直流）最大误差 0.0004 mV；全程最大误差 1.08 mV，出现在 t=0.25 ns 电流快速跳变处。官方没有说明积分方法，这个量级符合"官方用梯形法、这里用后向欧拉"带来的 O(h) 差别。
- 每步约 0.5 ms，规模太小，耗时没有参考价值。
- 这个用例的 GND 侧 decap 是两端电容，加上封装电感，几乎所有单元都有特殊结点、都要晚做。它适合用来压力测试非对角项和 dep 结点的处理路径。你们网表的 decap 都接地，融合条件更好。

ibmpg2t–6t 的网表压缩后分别为 1.6 / 10.3 / 12.2 / 15.2 / 25.4 MB，可以用同一个程序测试。

## 6. 待服务器确认

1. 在线上用例上跑 `VSDLSS_TRACE=1 ./bench_tr text:... 6 32 20`，记录：
   - 每步耗时（现在约 165 ms，设计估计 P1 约 130 ms，P4 约 90–100 ms）；
   - 晚做连通块的比例和原因。
2. 如果晚做比例高、原因是"被块更新过"，可以考虑把只更新一个连通块的块并进该连通块的任务（trace 里有这类块的数量）。
3. 如果仿真器的电流源本来就按打包位置组装，可以再加一个"直接写打包向量"的输入形式，省掉 s 的一次散布。
