# VSDLSS Solver Reconstruction

这是一个依据 VSDLSS 反编译证据和稀疏直接法数学原理重新实现的求解器。`reference/` 保存原始 Ghidra 输出和调用证据；它不参与构建。`src/` 是独立、可维护、经过测试的新实现，不声称与原程序逐行一致或 ABI 兼容。

## 当前能力（M1–M4）

- 实数 double、对称正定矩阵（SPD）。
- 0-based、上三角 CSC 输入，索引类型为 `int64_t`。
- 输入校验、列内排序、重复坐标求和，且不修改调用方矩阵。
- 自然序、Reverse Cuthill–McKee、动态最小度和多层嵌套剖分排序。
- 按实际消元图插入填充边；报告预测 `nnz(L)`、新增填充边和消元树高度。
- MLD 使用确定性的 SHEM 强边匹配和加权粗图，最粗层多起点区域增长分区，并在反粗化的每一层执行平衡与增益细化。
- 跨分区边通过最大匹配和 Kőnig 最小顶点覆盖转成节点分隔器；伪外围 BFS 仅保留为退化分区的安全回退。
- 消元树、符号列计数、稀疏 Cholesky `A = LL^T`。
- 一个因子顺序求解多个 RHS；失败时不覆盖输出。
- NaN/Inf、非法 CSC、非正定矩阵、I/O 错误和不支持操作的明确状态。
- RedHawk 风格 `.hdr/.matd/.matf/.matt/.mato/.rhs` 文件适配。

- M3：按连通分量分解、低度预消元与超节点数值分解，独立可重用 API。
- M4：与 M3 共用多列面板内核，预算驱动的超节点拆块、块级磁盘更新与求解、v2 因子保存/打开、完整性校验。
- 共享内存并行：M3 独立分量/多个 RHS、M3/M4 面板分解与更新、面板外部行求解；默认线程数 1。

暂不支持一般 LU、不定 LDLT、原版私有数据布局/块文件、GPU、MPI 和原版 37 参数 ABI。M5 原版文件/API 兼容尚需原始程序和真实样本验收。

## 与反编译原理的对应

| 反编译中确认的阶段 | 新实现 |
|---|---|
| 排序 `MLDOrder/minDegreeOrder` | 动态最小度与递归 MLD；恢复算法阶段和不变量，不声称相同内部容器或逐项 tie-break |
| 符号消元 `symbolicElimination` | 消元树、reach 和精确的 L 列容量预测 |
| 数值分解 `factorA/factorAOneChunkNew` | 内存内稀疏 Cholesky；保留相同的“排序→符号→数值”分层原理 |
| 前代与回代 `solveLoadCase` | `L y = Pb`、`L^T z = y`、`x = P^T z` |
| 多 load case | `vsdlss_factor_solve` 复用同一只读因子 |

原版包含全内存和磁盘分块两条路径，并对小块使用专门内核。M3 与 M4 分别提供独立超节点和磁盘路径，测试与 M1 及稠密参考交叉验证。

## 构建与测试

```bash
make
make test
make sanitizers
```

严格编译：

```bash
make clean
make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
```

演示二维 Poisson：

```bash
./vsdlss_solve 30
```

当前 30×30（900 阶）Poisson 基准：

| 排序 | `nnz(L)` | 消元树高度 | 最大解误差 |
|---|---:|---:|---:|
| 自然序 | 27029 | 900 | `5.773e-15` |
| RCM | 19315 | 900 | `2.887e-15` |
| 动态最小度 | 10351 | 109 | `5.107e-15` |
| 完整多层 MLD | 14837 | 95 | `5.440e-15` |

这些数值用于回归当前实现，不代表某种排序对所有矩阵都更优。

文件求解：

```bash
./vsdlss_solver [-p 0|1|2|3|4] [--demo-rhs] jobname
```

`-p 0`（默认）使用 MLD，`-p 1` 使用 RCM，`-p 2` 使用自然序，`-p 3` 使用动态最小度，`-p 4` 显式使用 MLD。默认要求 `.rhs` 存在且长度准确；只有显式指定 `--demo-rhs` 才会在 RHS 缺失时构造 `b=A·1`。成功后写出 `<jobname>.rsl`。

## 公共 API

入口集中在 `include/vsdlss.h`：

1. `vsdlss_factorize(A, order, &factor)`：验证、规范化并分解。
2. `vsdlss_factor_solve(factor, rhs, solution)`：复用因子求解。
3. `vsdlss_backward_error(A, solution, rhs, &eta)`：独立计算后向误差。
4. `vsdlss_factor_free(factor)`：释放因子拥有的全部资源。

置换约定固定为 `q[new]=old`、`pinv[old]=new`，并由性质测试验证双射。

更详细的证据边界、审计和路线见 `docs/reconstruction/`。

## M4 多列磁盘求解

```bash
./vsdlss_solver --disk-budget 1048576 --block-cols 8 --save-factor job.factor jobname
./vsdlss_solver --disk-budget 1048576 --load-factor job.factor jobname
./vsdlss_solver --m3 jobname
```

`--block-cols` 限制面板宽度，实际根据超节点边界及预算进一步拆分。数值阶段只保留源块和目标块两个缓冲，按目标块分组执行 M3 共用更新内核。省略块宽时优先使用最大宽度 8 的面板路径；极低预算保留 v1 单列回退。保存和显式块宽强制 v2，预算不足明确报错。

`--disk-budget` 为数值阶段显式堆工作区及因子元数据的字节上限。输入规范化、排序和符号准备仍在内存进行，不受此预算约束；它不是全进程 RSS 上限。准备阶段矩阵/符号布局在磁盘数值更新前释放。

v2 文件具有块及元数据完整性摘要，保存失败保留原目标文件；成功保存后可跨进程重新打开。格式是本项目重建格式，不兼容原版私有块文件。默认临时因子仍在创建后立即 unlink，释放时关闭。

API：`vsdlss_factorize_m4_ex(A, order, budget, temp_directory, max_columns, &factor)`、
`vsdlss_m4_solve`、`vsdlss_m4_save`、`vsdlss_m4_open`、`vsdlss_m4_get_stats`、
`vsdlss_m4_dimension`、`vsdlss_m4_workspace_bytes`、`vsdlss_m4_factor_free`。

在 24 阶稠密回归中，列宽 1 与 8 的数值块读取次数分别为 300 和 6；该结果不是通用运行时间或生产规模性能承诺。

## M5 对照验收

`tools/validate_m5.py` 根据 manifest 在隔离目录运行 M1/M3/M4，并用独立残差和跨进程因子复用检查数值行为。原版对照需提供哈希锁定的可执行程序/适配器及真实样本；缺少原版时 `--require-original` 返回 3，不能登记为 M5 通过。原版 37 参数 ABI、一般 LU/不定系统、部分分解尚未验收或实现。共享内存并行的实现及证据见下节。

实现细节、格式及 manifest 说明见
[07-m4-panels-m5.md](docs/reconstruction/07-m4-panels-m5.md)。
旧 v1 证据及格式保留于
[06-m4-disk-evidence.md](docs/reconstruction/06-m4-disk-evidence.md)。

## 并行求解

默认构建启用 OpenMP（需要支持 `-fopenmp` 的编译器/运行库）：

```bash
make clean
make
./vsdlss_solver --m3 --threads 4 jobname
./vsdlss_solver --disk-budget 4194304 --block-cols 96 --threads 4 jobname
make test-parallel
make bench-parallel
```

`vsdlss_set_num_threads(4)` 设置调用线程的并行策略；小任务和嵌套阶段自动串行。
`vsdlss_m3_solve_many` 按列主序批量求解 RHS，支持 leading dimension，整个批次失败时保留输出。
CLI 报告请求线程数和实际观察到的最大 team 大小。

M1 标量路径、M4 块读写及源块推进顺序仍串行；同一个 M4 因子仍不支持外部并发调用。
M4 显式数值工作区不随线程数增加，但 OpenMP 运行库/线程栈不计入该预算。

不使用 OpenMP 的构建：`make clean && make OPENMP=0 test`；该构建拒绝大于 1 的线程数。
实测 4 线程面板 factor 约 2.24×、M4 小型端到端约 1.21×；不是所有矩阵的性能保证。
设计、反编译/PARDISO 证据、基准和 TSan 工具限制见
[08-parallel-design.md](docs/reconstruction/08-parallel-design.md)。
