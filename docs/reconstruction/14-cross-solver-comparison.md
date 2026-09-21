> 历史实验记录：以下基准来自含宽松超节点原型的分支，不代表当前 main。2026-09-21 整合恢复严格超节点，移除 VSDLSS_SN_STRICT 环境开关；当前行为与重新实测见 [整合记录](16-main-integration-20260921.md)。

# 第三方求解器横向对比（2026-09-13）

目的：把"我们慢在哪"从推测变成可分解的测量。对照组选 **CHOLMOD 5.3.0**
（SuiteSparse，稀疏 SPD Cholesky 的事实标准，超节点 + BLAS-3），
外加 **SuperLU**（scipy 1.17.1 自带）作为非对称 LU 的参照。

**Intel oneMKL PARDISO 未能测到**：`pypi.org`（`mkl` wheel）与
`archive.ubuntu.com`（`libmkl-rt`）都不在本会话的网络出口白名单内，返回 403。
CHOLMOD 与 PARDISO 属同一类实现（填充缩减排序 → 超节点符号 → BLAS-3 数值），
下面的分解结论对两者都适用，但绝对数字只代表 CHOLMOD。

## 环境

Intel Xeon @ 2.10 GHz，2 核（无超线程），AVX-512 + FMA，GCC 13.3.0。

- VSDLSS：`-O2 -Wall -Wextra -std=c11 -fopenmp`，分支
  `codex/relaxed-supernodes-20260913`（含松弛超节点合并原型）。
- CHOLMOD 5.3.0：`-O3 -DNDEBUG`，链接 scipy 附带的 OpenBLAS 0.3.31
  （AVX-512 内核）。
- 该机器的 BLAS 天花板：DGEMM 61.3 GF/s（1 线程）/ 110.4 GF/s（2 线程），
  稠密 DPOTRF 25.1 / 37.7 GF/s。

矩阵：k³ 网格上的 7 点 Laplacian，对角 6.5，上三角 CSC，与
`test/bench_sn.c` 完全相同。三次运行取最小值，另有一次预热。

## 结果（1 线程）

"分解"对 VSDLSS 是 `vsdlss_factorize_m3`（含排序、符号、数值），
对 CHOLMOD 是 `cholmod_l_analyze` + `cholmod_l_factorize`，两者口径一致。

### n = 5832（18³）

| 求解器 | 分解 | 求解 | 因子非零元 | 数值速率 |
|---|---:|---:|---:|---:|
| VSDLSS-M3 | 0.2020 s | 0.00248 s | 706,770 | 2.22 GF/s |
| **CHOLMOD 超节点** | **0.0175 s** | **0.00094 s** | **481,432** | **9.91 GF/s** |
| CHOLMOD 单列（关掉 BLAS-3） | 0.0663 s | — | 481,432 | 1.94 GF/s |
| SuperLU（非对称 LU） | 0.1270 s | 0.00143 s | 2,062,860 (L+U) | — |

### n = 10648（22³）

| 求解器 | 分解 | 求解 | 因子非零元 | 数值速率 |
|---|---:|---:|---:|---:|
| VSDLSS-M3 | 0.5461 s | 0.00581 s | 1,650,715 | 2.60 GF/s |
| **CHOLMOD 超节点** | **0.0447 s** | **0.00223 s** | **1,315,142** | **16.08 GF/s** |
| CHOLMOD 单列（关掉 BLAS-3） | 0.3830 s | — | 1,315,142 | 1.54 GF/s |
| SuperLU（非对称 LU） | 0.5830 s | 0.00861 s | 6,006,672 (L+U) | — |

### n = 17576（26³）

| 求解器 | 分解 | 求解 | 因子非零元 | 数值速率 |
|---|---:|---:|---:|---:|
| VSDLSS-M3 | 1.4480 s | 0.01149 s | 3,526,673 | 2.99 GF/s |
| **CHOLMOD 超节点** | **0.1745 s** | **0.00463 s** | **2,184,062** | **14.83 GF/s** |
| CHOLMOD 单列（关掉 BLAS-3） | 0.6950 s | — | 2,184,062 | 1.49 GF/s |

分解总时间比：**11.5× / 12.2× / 8.3×**。求解比：2.6× / 2.6× / 2.5×。
两者后向误差同量级（1e-15 ～ 1e-16），精度不是差异来源。

## 差距的分解

关键对照是 **CHOLMOD 超节点 vs CHOLMOD 单列**：同一个库、同一个排序、
同一套符号分析，唯一区别是数值内核走不走 BLAS-3。

| 22³，1 线程 | 时间 | 速率 |
|---|---:|---:|
| CHOLMOD 超节点（BLAS-3） | 0.0367 s | 16.08 GF/s |
| CHOLMOD 单列（无 BLAS-3） | 0.3830 s | 1.54 GF/s |
| **BLAS-3 的净贡献** | **10.4×** | |
| VSDLSS 数值阶段（松弛超节点） | 0.1984 s | 2.60 GF/s |

**我们的数值内核已经不是最主要的问题了。** 经过前两轮的分块内核改造，
它在 2.60 GF/s，比 CHOLMOD 自己的非 BLAS-3 路径（1.54 GF/s）还快 1.7×，
而且这还是在多算 1.26 倍填充的前提下。

真正的大头在准备阶段。26³ 的分段计时：

| 阶段 | VSDLSS，1 线程 | 占比 | CHOLMOD |
|---|---:|---:|---:|
| MLD 排序 | 0.635 s | 44% | — |
| 符号分析 `sn_analyze` | 0.286 s | 20% | — |
| 数值分解 | 0.574 s | 40% | 0.070 s |
| **排序 + 符号合计** | **0.921 s** | **64%** | **0.105 s** |

CHOLMOD 的**整个** analyze（排序 + 消元树 + 符号 + 超节点划分）是 0.105 s；
我们**仅 MLD 排序一项**就是 0.635 s，慢 **8.8×**，而且结果更差：
填充多 **1.61×**（3,526,673 vs 2,184,062）。填充的代价是复利的——
它同时抬高符号阶段的规模、数值阶段的浮点数和求解阶段的访存。

CHOLMOD 在 18³/22³ 选了 AMD（`ordering=2`），到 26³ 自动切到
METIS/NESDIS（`ordering=3`，analyze 涨到 0.105 s 但填充仍然最优）。

## 由此得到的优先级

按"消除后端到端能省多少"排序（以 26³、1 线程为基准）：

1. **排序阶段**（当前 44%）。目标不只是更快，还要更少填充。
   把 MLD 做到 CHOLMOD analyze 的量级并拿到 AMD 级别的填充，
   单这一项就能把总时间压到约 0.53 s（2.7×），因为它同时削掉
   数值和符号两端的规模。
2. **BLAS-3 数值内核**（当前 40%，约 5× 头寸）。单独做只能把
   1.448 s 压到约 1.0 s（**1.45×**）——Amdahl 决定了它不能单独走。
3. 两项都做：约 0.33 s（4.4×），进入 CHOLMOD 的 2× 以内。

也就是说，**"把浮点运算压进 BLAS-3"是对的方向，但它不是当前最大的那块**。
先修排序，BLAS-3 的收益才兑现得出来。

## 复现

CHOLMOD 不是本项目的依赖，`test/bench_cross.c` 因此不接入默认 `make`。

```sh
# CHOLMOD（只需这几个模块）
git clone --depth 1 -b v7.8.3 https://github.com/DrTimothyAldenDavis/SuiteSparse.git
cmake -S SuiteSparse -B ssbuild -DCMAKE_BUILD_TYPE=Release \
  -DSUITESPARSE_ENABLE_PROJECTS="suitesparse_config;amd;camd;colamd;ccolamd;cholmod" \
  -DBLAS_LIBRARIES=$BLAS -DLAPACK_LIBRARIES=$BLAS \
  -DCHOLMOD_GPL=ON -DCHOLMOD_PARTITION=ON -DCMAKE_INSTALL_PREFIX=$PWD/ssinstall
cmake --build ssbuild -j2 ; cmake --install ssbuild

# scipy 自带的 OpenBLAS 把符号前缀成了 scipy_*；tools/blas_symbol_shim.s
# 生成一组跳转桩把标准 Fortran 名转发过去。用系统 BLAS 时不需要它。
gcc -shared -fPIC -o libblasshim.so tools/blas_symbol_shim.s $BLAS

gcc -O2 -Iinclude -Isrc -I ssinstall/include/suitesparse -std=c11 -fopenmp \
  -o bench_cross test/bench_cross.c $(ls src/vsdlss*.c | grep -v main) \
  -L ssinstall/lib -lcholmod -L. -lblasshim -Wl,-rpath,ssinstall/lib -lm
OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 ./bench_cross 22 1 3
```

## 一个关于并行的旁证

在这台 2 核机器、这些规模上，**CHOLMOD 从第二个线程拿到的加速也几乎为零**
（18³：9.91 → 10.32 GF/s；22³：16.08 → 14.17 GF/s，在噪声内甚至倒退）。
我们自己 1.13–1.21× 的分解加速比放在这个背景下并不算差。
结论是：在这个硬件和规模区间，差距**全部**来自单线程效率，不来自并行度；
线程级并行要到更大规模和更多核心上才是主要矛盾。
