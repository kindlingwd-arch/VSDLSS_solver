# 2026-09-23 求解路径优化、IBM 电源网格实测、可选 BLAS

分支 `perf/solve-path-20260923`，基于 main `463dce0`。本记录不占用编号序列。

## 环境

i7-1260P（4 性能核 + 8 能效核，16 线程）笔记本，WSL2 Ubuntu 20.04，GCC 9.4，
`-O2 -fopenmp`。WSL 分到 16 个 vCPU、7.7 GB 内存。单线程流式带宽约 14 GB/s，
4 线程约 24 GB/s 后饱和；随机读 4 线程约 1.6 亿项/秒后下降。纯计算 16 线程吞吐约为单线程的 9.7 倍。

## 一、合并后的 main 首次在 Linux 上验证

`OPENMP=1` 与 `OPENMP=0` 完整回归、smoke、ASan/UBSan、LSan（test_m3、test_parallel、test_supernodal）全部通过。
唯一问题：按第三批合并记录的命令加 `-Werror` 时，GCC 9 在 `test/test_io.c:24` 报 `-Wformat-truncation` 误报；
加 `-Wno-error=format-truncation` 后通过。

## 二、求解路径（结果逐位不变）

1. **按输出顺序写回**：分解时建立逆映射“全局结点 → (分量, 本地下标)”（32 位编码可用时每结点 4 字节，
   否则 8 字节），求解最后一步顺序写调用者数组、随机读分量缓冲。随机写需先读入整条缓存行，比随机读贵。
2. **`vsdlss_m3_solve_internal` / `vsdlss_m3_internal_order`**：按因子内部编号传入右端项、取回解，
   gather 与写回退化为顺序拷贝。结果与 `vsdlss_m3_solve` 逐位相同，失败语义相同。
3. **`vsdlss_sn_solve_inplace`**：不再每次分配 n 长数组、拷贝两次、扫描两遍；有限性检查并入调用方已有的拷贝循环。
4. **`forward_pull` 列外层循环**：面板连续读取，每个元素仍按 j 递增依次相减；逐元素 `isfinite` 移到最后统一扫描。
5. **求解树缓存**：后序在分解时建立，子树切分按线程数懒建并以 CAS 发布，并发求解安全。
6. `solve: scatter` 的计时起点改为写回开始处（此前包含了整个第一阶段）。

新增的分配都算作因子的一部分，失败返回 OOM；`test_m3` 的分解分配点从 127 增至 136，逐点注入均通过。
`test_parallel` 新增 `internal_order_solve`：402 个分量，1–4 线程，32 位与强制 64 位编码。

2288 万结点双网合成用例（随机编号），单次求解中位数：

| 线程 | 旧 | 新 `vsdlss_m3_solve` | 新接口 |
|---:|---:|---:|---:|
| 1 | 0.99 s | 0.89 s | 0.49 s |
| 2 | 0.68 s | 0.575 s | 0.32 s |
| 4 | 0.68 s | 0.57 s | 0.34 s |

同一用例不打乱编号时整次求解为 0.39 s，说明随机编号下近一半时间花在 gather/写回的随机访存上。

## 三、IBM 电源网格基准（ibmpg1–6）

来源：<https://web.ece.ucsb.edu/~lip/PGBenchmarks/ibmpgbench.html>（Nassif, ASP-DAC 2008），数据不入库。
`test/bench_ibmpg.c` 把 SPICE 网表转成 SPD 系统：电阻→电导，电流源→右端项，接地电压源→已知电压移到右端项，
0 V 结点间电压源（过孔）→合并结点。与参考解逐结点比较，六个用例最大误差 5–7e-6 V（参考解只有 6 位有效数字）。
`PG_STATS=1` 输出结构与数值特征：

| | pg1 | pg2 | pg3 | pg4 | pg5 | pg6 |
|---|---:|---:|---:|---:|---:|---:|
| 未知量 | 1.6 万 | 12.7 万 | 85.1 万 | 95.3 万 | 54.0 万 | 83.4 万 |
| 平均度数 | 3.64 | 3.28 | 3.29 | 3.26 | 3.98 | 3.95 |
| 核心占比 | 72% | 64% | 67% | 67% | 92% | 97% |
| 条件数 | 7.5e4 | 3.3e5 | 2.4e12 | 1.9e6 | 2.1e6 | 2.7e6 |

与客户度数分布（平均度数 2.11，核心 7%）差别很大：IBM 网格以度数 3–4 为主，耗时集中在核心超节点分解。
ibmpg3 的极小过孔电阻（6.5e-10 Ω）使条件数升至 1e12，合并近零电阻可以消除。
六个用例均为 VDD/GND 多分量；求解 2 线程最快，4/8 线程慢约 20%。
（更正：这是 GCC 9 下嵌套并行未生效所致，多出的线程没有进入分量内部；修复见
[merge-20260923-d](merge-20260923-d.md) 末节。本文其余多线程数字也是修复前测得的。）

## 四、可选 BLAS（`make BLAS=1`，默认关闭）

- 分解：源块宽度 ≥ `VSDLSS_BLAS_MIN` 的更新用 `dgemm`，面板宽度达到该值时用 `dpotrf` + `dtrsm`。
- 求解：面板宽度 ≥ `VSDLSS_BLAS_SOLVE_MIN` 时用 `dtrsv` + `dgemv`；开启后每个分量内部走串行路径
  （树调度的拉取式前代没有与之同舍入的 BLAS 形式）。
- BLAS 块总按固定的行块边界切分，与线程数无关；在 BLAS 单线程的前提下，1/2/4 线程结果逐位相同，
  `test_supernodal`、`test_parallel`、`test_reduced_dag` 在 BLAS 构建下全部通过。与内置内核结果不逐位相同（差 1e-14 量级）。
- **BLAS 必须线程安全且在求解器内单线程运行。**Ubuntu 20.04 的串行 OpenBLAS 0.3.8 并发调用会算错；
  OpenMP 版 0.3.8 会用 `omp_get_max_threads()` 覆盖 `OPENBLAS_NUM_THREADS`，需同时设 `OMP_NUM_THREADS=1`
  （求解器自身用显式 `num_threads`，不受影响）。0.3.8 不识别 12 代酷睿，需 `OPENBLAS_CORETYPE=Haswell`。

稠密内核（`test/bench_dense.c`）：内置 DGEMM 3.9–6.0 GF/s，OpenBLAS（AVX2）32–49 GF/s；
面板 Cholesky 3.7–4.6 对 21–42 GF/s。

## 五、与 CHOLMOD 5.10.1 对比

CHOLMOD 超节点、AMD、同一 OpenBLAS；`test/bench_cholmod.c` 读 `PG_DUMP` 导出的同一系统。
我们为 BLAS 阈值 16（分解与求解）。分解含排序与符号分析。

| 用例 | CHOLMOD 分解 | 我们 内置 1T / 2T | 我们 BLAS 1T / 2T | CHOLMOD 求解 | 我们 BLAS 求解 1T / 2T |
|---|---:|---:|---:|---:|---:|
| ibmpg3 | 3.43 s | 8.84 / 5.01 s | 2.88 / 1.65 s | 0.105 s | 0.087 / 0.056 s |
| ibmpg4 | 4.31 s | 12.84 / 7.15 s | 3.62 / 2.06 s | 0.118 s | 0.105 / 0.068 s |
| ibmpg6 | 1.41 s | 2.04 / 1.25 s | 1.51 / 0.91 s | 0.062 s | 0.064 / 0.043 s |
| 2288 万双网 | 49.1 s | — / 17.4 s | — / 13.6 s | 1.50 s | 0.82 / 0.56 s |

- 稠密内核曾是与 CHOLMOD 的主要差距，接入 BLAS 后反超。CHOLMOD 的 BLAS 多线程（4 线程）收益很小，窄超节点时更慢。
- 2288 万用例：CHOLMOD 对全矩阵 AMD 耗时 34 s，nnz(L) 1.47 亿；我们先做低度消元，只对 7% 的核心排序，nnz(L) 9530 万。
  内部顺序接口加 BLAS 求解为 0.28 s。
- 网格型 IBM 数据上我们的填充多 9–16%（pg3、pg6），排序还有改进空间。

## 六、仍可继续

- 多右端项求解改用 BLAS-3（`dtrsm`/`dgemm`）；
- 客户类型数据上，预处理（连通分量、重编号、低度消元）已是分解的主要耗时；
- 预处理合并 0 V 源与近零电阻。

## 复现

```sh
make bench_pg_solve && PG_NETS=2 SOLVE_THREADS="1 2 4" ./bench_pg_solve 5 2 1 1
make bench_ibmpg && PG_THREADS="1 2 4" ./bench_ibmpg ibmpg3.spice.bz2 ibmpg3.solution.bz2
PG_STATS=1 ./bench_ibmpg ibmpg3.spice.bz2 ibmpg3.solution.bz2
make clean && make BLAS=1 BLAS_LIBS='-lopenblas' test-supernodal test-parallel bench_ibmpg bench_dense
OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1 VSDLSS_BLAS_MIN=16 VSDLSS_BLAS_SOLVE_MIN=16 ./bench_ibmpg ...
PG_DUMP=pg3.bin ./bench_ibmpg ... && make bench_cholmod CHOLMOD_CFLAGS=... CHOLMOD_LIBS=... && ./bench_cholmod pg3.bin
```
