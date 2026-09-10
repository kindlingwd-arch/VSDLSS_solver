# 并行设计、证据与验证

日期：2026-09-10。基线 `f3a53af`；分支 `codex/parallel-supernodal`。

## 参考证据

| 来源 | 可确认的行为 | 本项目采用方式 |
|---|---|---|
| `reference/funcs/computeNodeToThread_vsdlss.c` | 初始化按度 1/2/3 加权的工作量，选剩余容量较大的线程，将节点/块映射到线程后扣除估算成本 | 连通分量和变长更新列采用动态调度，工作量阈值决定是否启用并行；没有复制固定 67 元素栈数组 |
| `getMyThreadIndex_vsdlss.c`、`nrerror_vsdlss.c` | 根据线程标识查询注册槽；错误处理使用 pthread mutex | 每个任务返回局部状态，join 后按分量/RHS 编号选第一个错误；输出只在全部成功后提交 |
| `encapsulatedMalloc_vsdlss.c`、`encapsulatedFree_vsdlss.c` | 对原全局内存管理器加锁 | 保持本项目独立 malloc/free 所有权，无原版全局内存管理器共享状态 |
| `reference/funcs/vsdlss.c` | 入口调用 `setNumThreadsInBlasLib(1)` | 有外层并行时禁用内层并行，避免线程过量；不修改进程全局 OpenMP 或 BLAS 设置 |

反编译证据不完整：上述函数能支持工作量划分、线程登记和同步原理，但没有据此证明恢复了原版完整任务图、队列或 pthread 生命周期。

Intel oneMKL PARDISO 文档将 `iparm[23]` 定义为因子分解并行控制（包含两层算法），`iparm[24]` 定义为求解并行控制，可按矩阵分区或 RHS 并行。本项目借鉴层次选择，使用自己的 SPD 数值内核；没有链接 PARDISO，也没有对 PARDISO 做性能对比。

来源：[Intel oneMKL PARDISO iparm 参数（C 接口，2023.2）](https://www.intel.com/content/www/us/en/docs/onemkl/developer-reference-c/2023-2/pardiso-iparm-parameter.html)。这里使用 C 的 0-based 下标，避免与 Fortran 的 1-based iparm 混淆。

## 实际实现的两层并行

### 外层：独立分量或多个 RHS

M3 规范化和发现连通分量后，各分量分别执行抽取、低度缩减、排序、符号分析和数值分解。各任务只写自己的因子结构；求解阶段只写私有工作向量中互不重叠的分量位置。OpenMP `schedule(dynamic,1)` 平衡大小不等的分量，所有任务结束后才返回或统一清理。

新增 `vsdlss_m3_solve_many`：RHS 和输出为列主序，支持独立 leading dimension。多个 RHS 之间并行，工作区汇总全部结果，任何一个 RHS 失败均不覆盖调用方输出，包括原地调用。相较逐 RHS 调用会增加 O(n×nrhs) 暂存及各工作线程的求解暂存；此 API 不受 M4 数值内存预算约束。

多 RHS 的外层并行激活后，分量/面板内层自动串行。只有一个分量或一个 RHS 时可继续使用更内层的并行机会。

### 内层：共享面板内核和块贡献更新

- 面板 Cholesky：主元列依赖保持顺序。一个持久 OpenMP team 跨越该面板各列；每列主元由 single 区计算，随后行更新静态分配，barrier 完成后再进入下一列。每个输出元素的内积顺序固定，无浮点并行 reduction。
- M3 外部更新：一个源超节点处理完成后，将不同目标列分配给线程。同一个源产生的三角散射位置互不重叠；不同源之间保持既有顺序，避免对同一目标累加产生竞争。
- M4：源块、目标块读取由调用线程执行；目标块已加载后，不同目标列的数值更新并行。完成 barrier 后由调用线程写回。两个显式块缓冲不随线程数增加，文件游标和摘要没有多线程共享写操作。
- 前代：先顺序解块内三角部分，再并行处理不同外部行。
- 回代：不同块内列对已完成的外部行贡献可并行计算，然后顺序解块内转置三角部分。此次调整了相较旧实现的加减顺序，但线程数改变不会改变单个元素的运算顺序。用残差和 M1 对照覆盖舍入变化。

## 线程配置及串行路径

```c
vsdlss_set_num_threads(4);
/* 接着调用 M3/M4 分解、求解或 M3 solve_many */
int requested = vsdlss_get_num_threads();
int used = vsdlss_parallel_last_team_size();
```

配置为调用线程局部状态，默认 1，合法范围 1–1024；不调用 `omp_set_num_threads` 等全局设置。`used` 是自上次成功 setter 以来、由该调用线程观察到的最大实际 team 大小，不能当作所有阶段都使用了该线程数。OpenMP runtime 可限制实际线程数。

估计工作量小于 100000 时走单线程；分量/RHS 调度还受可用任务数限制。阈值是当前启发式参数，不宣称对所有机器最优。处于已有活跃 OpenMP region 内时不再创建活跃内层 team。

```sh
make clean
make OPENMP=1
./vsdlss_solver --m3 --threads 4 jobname
./vsdlss_solver --disk-budget 4194304 --block-cols 96 --threads 4 jobname

make clean
make OPENMP=0 test
```

默认构建 `OPENMP=1`，要求支持 `-fopenmp` 的编译器和运行库。`OPENMP=0` 不依赖 OpenMP，多线程请求返回 `VSDLSS_ERR_UNSUPPORTED`，不会静默冒充并行。切换构建选项需 `make clean`。临时目录默认尊重 `TMPDIR`，否则使用 `/tmp`；测试通过 Makefile 使用可写的工作目录，显式目录仍优先。

## 正确性与内存约束

每个并行阶段只写互斥的元素/分量/RHS；源数据在该阶段只读。状态通过整数 reduction 或任务状态数组归并，失败时在所有线程结束后释放对象。M4 solve 对同一因子对象仍不得由外部多个调用线程同时调用，因为 FILE 游标和统计具有可变状态；本次并行的是该调用内部的数值计算。

M4 显式堆数值预算仍为两个块缓冲与元数据，1/2/4 线程测试验证其不变。线程栈和 OpenMP runtime 内存不计入；它仍不是整个进程 RSS 上限。M3 的并发准备及多 RHS 会提高峰值内存，不承诺固定预算。

M1 的标量稀疏 Cholesky、排序/符号阶段的单连通任务、M4 块 I/O、M4 源块推进顺序仍串行。没有实现任意消元树任务 DAG 调度、并行磁盘预取、GPU、MPI 或一般不定矩阵求解。本实现是已验证的首版共享内存并行加速，不是完整复刻 PARDISO 的所有并行策略。

## 测试与性能

`test/test_parallel.c` 检查：

- 多线程 team 实际大于 1，面板 factor 在 1/4 线程下逐位一致；非正定主元正确失败。
- M3 四个交错编号的独立分量：1/2/4 线程，四个带步长 RHS，数值结果逐位一致且后向误差 <1e-12。
- 多 RHS 中含 NaN 时，所有输出及 padding 均保留；其中一个分量非 SPD 时因子输出为 NULL。
- M4 1/2/4 线程：与已知解对照、逐位一致、显式数值工作区不随线程数变化。
- 大面板前/回代实际并行且与串行逐位一致。
- 无 OpenMP 构建拒绝多线程请求。

`test/bench_parallel.c` 对每种线程数取 5 次运行中位数，面板阶段另有预热。计时结果见 [parallel-benchmark.tsv](parallel-benchmark.tsv)。当前环境 `getconf _NPROCESSORS_ONLN` 返回 9；编译 `-O2 -Wall -Wextra -Werror -std=c11 -fopenmp`，不同硬件、负载与矩阵会改变结果。

| 测试 | 1 线程 | 4 线程 | 加速比 |
|---|---:|---:|---:|
| 1024×256 面板 factor | 0.042075 s | 0.018776 s | 2.24× |
| 384 阶稠密 M4，块宽 96，分解+求解 | 0.029512 s | 0.024391 s | 1.21× |

第二行包含排序/符号/文件处理的串行开销；微基准加速不代表端到端等比例加速，更不代表相对 PARDISO 的加速。两者在每次计时后均检查数值结果。运行 `make bench-parallel` 可复现测量。

严格构建、串行构建和 sanitizer 的最终结果记录在 CONTINUATION.md。

### ThreadSanitizer 限制

尝试了 GCC 13.3 `-fsanitize=thread -fopenmp`。完整并行回归进程返回 66 并报告竞争；定位包括 single 写入主元后、隐式 barrier 之后的读取。独立 `test/omp_tsan_probe.c` 只做 master 写、显式 `omp barrier`、各线程读取，也报告同类竞争并返回 66。这说明当前 libgomp/TSan 组合不能可靠建模该 OpenMP 同步；不能把这些运行登记为 TSan 通过，也不能据此排除所有应用层竞争。

复现探针：

```sh
cc -O1 -g -fopenmp -fsanitize=thread test/omp_tsan_probe.c -o test_omp_tsan_probe
TSAN_OPTIONS=halt_on_error=1 ./test_omp_tsan_probe
```

未添加 sanitizer 抑制规则或伪造同步注解。当前依据互斥写入设计审查、实际 1/2/4 线程结果/失败测试以及 ASan/UBSan 验证。后续应在支持 OpenMP 同步分析的运行库/工具组合上完成有效 race gate。

ASan/UBSan 沿用项目既有的不检查泄漏策略。当前运行环境未能通过 `ASAN_OPTIONS` 环境变量关闭不支持的 LeakSanitizer，因此仅在 sanitizer 测试链接 `test/sanitizer_options.c` 提供同一默认设置；生产构建不包含该文件。
