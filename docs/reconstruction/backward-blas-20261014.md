# 按矩阵块尺寸选择 M3 反向 BLAS 内核

## 改动和启用

`VSDLSS_BLAS_BACKWARD=1` 在 BLAS 构建中启用 M3 packed 面板的反向内核选择：
面板宽度 `width >= 16`，或 `width >= 7 && ext >= 64` 时，调用已有的
`DGEMV(transpose)` 外部行更新和 `DTPSV(transpose)` 三角求解。
其余尺寸继续使用原内核。因子存储和所有数值计算均为 FP64，不复制或重新分解因子。
原编号的 `vsdlss_m3_solve` 接口、正向求解、树调度和 M4 full-layout 内核均不变。

选项默认关闭，仅字符串 `1` 启用；无 BLAS 构建忽略此选项。环境变量在首次使用时缓存，
须在求解前配置，不能在同一进程内切换。已有 `VSDLSS_BLAS_SOLVE_MIN` 优先：
若要仅启用新反向策略，必须将该阈值设为 `0`；其非零值仍可以在两方向选择 BLAS。
现有 BLAS 的整数尺寸限制、pivot 检查、非有限结果检查以及 gather 分配失败后的内置回退继续生效。

例如使用线程安全、单线程 OpenBLAS：

```sh
make BLAS=1 METIS=1 BLAS_LIBS='-lopenblas'
export OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1
export VSDLSS_BLAS_SOLVE_MIN=0 VSDLSS_BLAS_BACKWARD=1
# 应用仍用 vsdlss_set_num_threads(8) 和 METIS ordering=6。
```

MKL 使用 `MKL_NUM_THREADS=1 MKL_THREADING_LAYER=SEQUENTIAL`。这些线程约束与既有 BLAS
路径相同。与内置路径的浮点累加次序不同，不承诺逐位一致；需验证实际数据残差。

## 本地 16M 实测（2026-10-14）

环境：WSL Ubuntu、GCC 9.4、MKL 2026.1 sequential；METIS ordering=6，求解器 8 线程。
分解设置 `VSDLSS_BLAS_MIN=16`，基线回代设置 `VSDLSS_BLAS_SOLVE_MIN=0`。
比较完整原编号的单 RHS `vsdlss_m3_solve`，复用相同已分解因子；不把分解和残差计算计入求解时间。

下表来自整理为环境变量选项之前的等价候选：仅在计时调用外切换内部实验选择器，
每个 case 共 3 组，每组 21 对交替执行，共每种实现 63 次。生产代码不包含该实验选择器。

| case | 基线中位数 | 候选中位数 | 时间减少 | 三组各自的候选/基线中位数比 |
| --- | ---: | ---: | ---: | --- |
| pgb_16M | 401.585 ms | 387.544 ms | 3.50% | 0.9825 / 0.9971 / 0.9758 |
| emir16M | 322.010 ms | 302.104 ms | 6.18% | 0.9333 / 0.9380 / 0.9757 |

合并样本的中位数之比不等于各组比值的均值。逐对候选更快的次数分别为 36/63、48/63，
pgb 的一组几乎持平。因此这只是本机的有限小幅收益，不保证其他机器、BLAS 或矩阵会加速，
也没有达到两份 case 耗时降至 50% 的原目标。两秒一次的监控未发现 `ab_packed` 干扰，
两份 case 的 swap-in/out 增量均为零；未排除宿主机全部后台活动和温度变化。
另一轮独立二进制比较受 `ab_packed` CPU 争用干扰，已排除，不作为性能证据。

原 RHS、确定性人工 RHS 和交替符号 RHS 均检查了向后误差，验收阈值 `berr <= 1e-12`。
候选相对基线解的最大相对无穷范数差分别为 `8.59e-15`、`4.44e-15`；
两个 case 中基线及候选的最大向后误差分别不超过 `8.09e-17`、`3.51e-17`。

整理为正式环境变量选项后，两个 case 各重新分解一次，检查 3 种 RHS、4 次预热和
31 次计时回代。六个解哈希均与先前候选一致；pgb_16M 的最大向后误差为 `3.36e-17`，
emir16M 为 `1.92e-17`。这是正式代码的数值复验，不是新一轮基线/候选性能比较。

## 回归与复测方法

`make test-backward-blas` 在未设置、`0` 和 `1` 三种环境分别启动测试进程。
每个进程覆盖 1080 个尺寸/方向/状态组合：阈值两侧、gather 栈/堆边界、非零起始编号、
散布外部索引、无外部行、零/NaN pivot 和 NaN RHS。用未改动的 full-layout 内核作为数值参考；
BLAS 构建还包装真实 `dtpsv` 调用计数，防止选项实际未生效却通过数值检查。
`make test-supernodal` 的 BLAS 构建增加独立反向选项运行，检查树求解及线程数一致性。
既有 `test-small` 专门检查内置 packed/full-layout 内核逐位相等，故该目标显式关闭两种
BLAS 回代选项；新 BLAS 路径由上述独立测试验证数值误差和状态，不放宽原逐位断言。

WSL 下测试临时文件应放在 Linux 文件系统，例如 `TMPDIR=/tmp make ... test`。
本机首次完整回归使用 Windows 挂载目录时，既有 M4 文件损坏测试的 `ftruncate` 失败；
同一测试二进制改用 Linux 临时目录，在新选项关闭和开启时均通过。
最终 BLAS/METIS 构建在 `VSDLSS_BLAS_BACKWARD=1 VSDLSS_BLAS_SOLVE_MIN=0` 下的完整
`make test` 通过（`test-small` 按上述约定使用内置内核）；无 BLAS/METIS 构建的
`test-backward-blas` 三种配置也全部通过。

完整数据的独立进程复测可使用现有 `bench_history`（共享库的构建方法见
[performance-history-20261013](performance-history-20261013.md)）：

```sh
make bench_history
export VSDLSS_BLAS_MIN=16 VSDLSS_BLAS_SOLVE_MIN=0
export MKL_NUM_THREADS=1 MKL_THREADING_LAYER=SEQUENTIAL
# lib.so 必须由本版本源码以 BLAS=1、METIS=1 的配置构建。
# 每个 case 选择一个尚不存在的 REF 文件，第一轮写入基准解。
REF=pgb-reference.bin VSDLSS_BLAS_BACKWARD=0 ./bench_history ./lib.so /path/pgb_16M.bin 6 8 31
REF=pgb-reference.bin VSDLSS_BLAS_BACKWARD=1 ./bench_history ./lib.so /path/pgb_16M.bin 6 8 31
# 对 emir16M.bin 使用另一个 REF 文件，并按 AB/BA 顺序重复两份数据。
```

独立进程会重新分解，需多轮交替控制漂移；以上命令不能复现历史实验的同一份因子内交替。
必须检查进程状态、每次输出的残差与相对解差、swap 和竞争负载，不能仅比较一次耗时。
