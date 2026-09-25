# 2026-09-24 可选 METIS 核心排序（ordering 6）与 MKL PARDISO 对比

分支 `main_backup`。本记录不占用编号序列。环境同 [perf-solve-blas-20260923](perf-solve-blas-20260923.md)；
MKL 2026.1（PyPI wheel），METIS 5.1.0（SuiteSparse 5.10.1 自带，本地编译，64 位 idx_t）。

## 与 MKL PARDISO 对比（先于 METIS 接入）

PARDISO：SPD（mtype 2），默认参数，AMD 或 METIS 排序；`test/bench_pardiso.c` 读 `PG_DUMP` 导出的同一系统。
我们：`main_backup`（含嵌套并行修复），分解用 MKL sequential 作 BLAS，求解用内置树并行。8 线程：

| 用例 | 分解总时间 我们 / PARDISO 最好 | 单次求解 我们 / PARDISO 最好 | 峰值内存 |
|---|---:|---:|---|
| 2288 万客户分布，双网 | 14.4 s / 未完成 | 0.58 s / 未完成 | 我们约 4.7 GB；PARDISO 超过 7.5 GB 被 OOM 终止 |
| 平均度数 2.5，800 万，双网 | 6.9 s / 35.8 s | 0.32 s / 2.1 s | 约 3.3 GB / 5.7 GB |
| 平均度数 2.5，400 万，双网 | 2.85 s / 15.9 s | 0.15 s / 0.85 s | — |
| ibmpg4 | 2.3 s / 4.1 s | 0.135 s / 0.139 s | 约 1.4 GB / 1.1–1.4 GB |

PARDISO 的分析阶段（全矩阵排序 + 符号）占其大部分时间；其数值分解本身很快。PARDISO 的求解未调
`iparm[7]`（迭代精化）与 `iparm[24]`（并行求解），可能偏慢。其 METIS 排序在 ibmpg4 上 nnz(L) 只有我们
AMD 的 55%，这是接入 METIS 的动机。

## 实现

- `vsdlss_order` 新增 `order == 6`（`-DVSDLSS_METIS`）：由核心的上三角 CSC 建无对角对称邻接，调用
  `METIS_NodeND`（`perm[新] = 旧`，即 `q`）。`vsdlss_factorize_m3` 在 METIS 构建下接受 6。
- 固定 `METIS_OPTION_SEED = 1`。`VSDLSS_METIS_NITER/CTYPE/RTYPE` 环境变量仅供实验；实测
  NITER 对时间几乎无影响，RM 粗化与单侧分隔细化都使填充和求解变差，保留默认值。
- **线程安全**：METIS 5.1 的 GKlib 随机数默认调用 libc `rand()`（全局状态），定义 `USE_GKRAND` 时用
  全局 Mersenne Twister 数组。两个大分量（VDD/GND）并发调用时顺序依赖线程时序，nnz(L) 随线程数变化
  （ibmpg4：43.53M 对 42.77M），`mti` 下标在竞争下还可能越界。处理：
  - 默认对 `METIS_NodeND` 加 `omp critical`（任意 METIS 都安全，但两个网的排序串行，800 万双网分解
    13.7 s → 23.2 s）；
  - `METIS_THREADSAFE=1`（`-DVSDLSS_METIS_THREADSAFE`）用于以 `-DUSE_GKRAND` 编译、并把
    `GKlib/random.c` 中 `mt[]`、`mti` 改为 `static __thread` 的 METIS：调用可重叠，排序也更快
    （libc `rand()` 有锁）。每次调用都重新播种，结果与线程数无关。
- Makefile：`make METIS=1 METIS_CFLAGS=... METIS_LIBS=... [METIS_THREADSAFE=1]`，默认关闭。

## 验证

默认、`OPENMP=0`、`OPENMP=0`+METIS、METIS（线程安全版）、METIS（串行调用）、MKL BLAS + METIS
各构建的回归测试通过；METIS 构建下 `test_supernodal` 遍历排序 0–6，含 1/2/4 线程逐位一致检查，
`test_amd` 改用 7 作为未知排序。线程安全版 METIS 下，ibmpg4 在 1/4/8 线程、两个进程中 nnz(L) 均为
43.01M；800 万双网两次运行解哈希相同。与 IBM 参考解误差不变（5.0e-6 V）。

## 性能：AMD（order 5）对 METIS（order 6）

8 线程，分解用 MKL；METIS 为线程安全版（800 万）或早期未加锁版（其余，nnz(L) 差异在 1% 内）。

| 用例 | nnz(L) | 数值分解（每网） | 普通 / 打包求解 | 峰值内存 | 排序（每网） | 分解总时间 |
|---|---:|---:|---:|---:|---:|---:|
| ibmpg4 | 82.7M → 43.0M（−48%） | — | 0.114 → 0.061 s | — | 0.4 → 3.0 s | 1.8 → 2.9–3.2 s |
| 平均度数 2.5，400 万 | 70.6M → 55.0M（−22%） | 0.8 → 0.34 s | −29% / −28% | −12% | 0.5 → 3.8 s | 3.5 → 6.1 s |
| 平均度数 2.5，800 万 | 161.6M → 117.7M（−27%） | 2.5 → 0.9 s | 0.357 → 0.28 s / 0.248 → 0.199 s | −11% | 1.0 → 6.2–6.8 s | 7.5 → 11.5–11.9 s |
| 2288 万客户分布 | 95.3M → 73.7M（−23%） | 4.0 → 2.1 s | −9% / 持平 | 持平 | 1.0 → 7.6 s | 14.7 → 20.5 s |

结论：

- METIS 使求解快约 20–30%（网格型数据近 2 倍），数值分解快 2–3 倍，L 与内存少 11–48%。
- 代价是排序：每网多 2.5–7 s，单次分解总时间变长。按 800 万双网计，约 60–85 次求解收回；ibmpg4 约 25 次。
- 适用：同一矩阵要求解很多次（多工况、瞬态、ECO 迭代），或内存紧张（6000 万规模估计从约 30 GB 降到
  约 22–24 GB）。只分解一次、求解少量次数时 AMD（默认）更快。
- 仍可改进：METIS 排序串行、占分解大头；可只对大分量用 METIS，或并行嵌套剖分（mt-metis、ParMETIS）。

> **2026-09-25 更正**：上表"平均度数 2.5，800 万/400 万"两行中 PARDISO 的求解时间（2.1 s、0.85 s）在同一矩阵上无法复现：
> 同一矩阵、PARDISO 默认配置为 0.37–0.59 s，最快配置 0.25 s，与本求解器持平。详见
> [solve-simd-perm2-20260925.md](solve-simd-perm2-20260925.md)。
