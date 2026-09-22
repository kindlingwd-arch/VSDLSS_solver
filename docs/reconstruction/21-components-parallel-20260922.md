# 2026-09-22 连通分量阶段并行化（第一步：可测量的并集查找路径）

接 [19](19-single-solve-efficiency-20260922.md) 的"仍可继续"第一条。全量算例（2287.5 万结点，2 线程）中连通分量阶段为 5.64 s，是预处理里最大的一项，且完全串行。

## 输入结构

用户确认目标网表的实际结构：**VDD 与 GND 是两个互不相连的子图，结点数相同**。因此矩阵稳定地分成 2 个等规模连通分量。这决定了并行粒度的上界：按分量并行最多 2 倍。

## 本次改动

### 1. 阶段内计时

`TRACE` 宏从 `vsdlss_m3.c` 提到 `vsdlss_m3_internal.h`（`vsdlss_trace_on` / `vsdlss_trace_now` 改为外部链接），连通分量阶段内部新增三个打点：

- `components: adjacency`——度数统计、前缀和、邻接表填充
- `components: label+bfs`——分量标号与 BFS
- `components: finalize`——`vertices` / `local_of` 填充与 `order` 重映射

**这是本次最重要的产出。** 第 19 篇只记录了该阶段的总耗时 5.64 s，没有拆分；在不知道 BFS 与建邻接表各占多少之前，无法判断并行 BFS 是否值得做。

### 2. 分量级并行 BFS（默认关闭）

`vsdlss_components.c` 新增一条可选路径：在填充邻接表的同一遍扫描里维护并集查找（union-find），随后各分量的 BFS 并行执行。

关键是**输出与串行扫描逐位相同**，论证如下：

- `uf_union` 总是把较大的根链接到较小的根，因此每个集合的根就是该分量的最小结点编号。
- 按升序给根编号，得到的分量 id 顺序等同于串行"升序扫描首个未访问结点"的顺序。
- 每个分量的 BFS 以其根（= 最小结点）为种子，邻接表遍历顺序不变，因此队列内容与串行完全一致。
- 各分量写入 `order` 中互不重叠的区间，只标记自己的结点，线程间无共享写。

所以 `component_of`、`order`、`offset` 和各分量大小在任意线程数下都与串行结果逐位相同。这一点由 `test_supernodal.c` 的 `two_supply_nets` 逐数组 `memcmp` 检查，包括 `wgraph` 的 `ptr`/`idx`/`val`/`diag`。

并集查找的父数组借用 `components->local_of`（该数组到最后一遍才写入），分量级数组只有 count 长度，**没有新增 n 长度分配**。

### 3. 测试

`two_supply_nets`：`powergrid(40,2)` 构造两个等规模分量（VDD/GND 形状，结点编号随机打乱），先以串行路径建一次，再在 2 / 4 线程下走并集查找路径，逐数组比对。同时检查分量数为 2 且两个分量大小相等。

已有的 `large_powergrid`（`powergrid(85,2)`，n≈28.6 万）本来就检查 1/2/4 线程解逐位一致，因此一旦默认开启该路径，它自动成为回归护栏。

## 为什么默认关闭

**并集查找的成本可能超过它换来的收益，这一点尚未实测。**

- 该路径为每个非对角元做两次 `find`，即对一个 n 长度数组（全量算例约 183 MB）的两次随机访问。全量上三角 4701 万非零元，合计约 9400 万次随机访问。
- 它替换掉的串行 BFS，访问模式与之相近（随机访问 `component_of` 与 `adjacent`）。
- 分量只有 2 个，并行 BFS 最多把 BFS 部分减半。

即：新增约"一遍随机访问扫描"的成本，换取"BFS 减半"的收益。若两者量级相当，则净收益为负。因此默认 `vsdlss_components_uf_min = INT64_MAX`（关闭），由 `VSDLSS_COMPONENTS_UF=1` 在同一个二进制上开启，便于 A/B 对比。测试直接调低阈值以覆盖该路径。

无论开关如何，输出不变，所以这只是时间上的取舍，不影响数值结果。

## 需要实测的内容

在目标机器（非 2 核沙箱 VM）上：

```sh
make bench-pg-profile
VSDLSS_TRACE=1 ./bench_pg_profile                      # 串行路径，看阶段拆分
VSDLSS_COMPONENTS_UF=1 VSDLSS_TRACE=1 ./bench_pg_profile   # 并集查找路径
```

要回答三个问题：

1. `components: adjacency` 与 `components: label+bfs` 各占多少？若 adjacency 占大头，则并行 BFS 方向本身收益有限，应转向减少该遍散射的缺页与内存带宽压力。
2. 开启并集查找后总耗时是升是降？
3. 线程数超过 2 时有无变化？分量只有 2 个，`label+bfs` 不会再快，但可以确认没有负面影响。

## 若实测显示 BFS 占大头

则下一步是**确定性的层同步并行 BFS**，并行度不再受分量数限制：

- 逐层展开，下一层的顺序只由（父结点在当前层中的位置，父结点邻接表内的位置）决定。
- 同层被多个父结点发现的结点，按"父位置最小者"归属（原子取最小，与线程调度无关），再用前缀和压缩。

其结果与线程数无关，但**不再与当前串行 BFS 的顺序相同**。这一点是可接受的：`order` 只是重编号的局部性启发式，第 18 篇引入它时已经明确"改变消元顺序和核心编号，结果只保证数学上等价"。但它会改变填充与浮点运算顺序，所有数值基准需要重测。复杂度和验证成本都明显更高，**因此应当在第 1 问有答案之后再决定是否做**。

## 验证状态

**本改动未在本地构建或测试。** 开发环境为 Windows，无 gcc/make/WSL。以上仅为静态设计与代码审查。

合并前需要在 Linux 上完成：

```sh
make clean
make -j4 CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test smoke test-small test-reduced-dag
make -B -j4 OPENMP=0 CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test smoke
make sanitizers
```

`two_supply_nets` 是新增测试，`test-supernodal` 目标即可单独运行。
