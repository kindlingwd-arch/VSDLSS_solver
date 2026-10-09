# 2026-10-09 与 MKL PARDISO 的对比（16M 双网，main 37014b2）

本文档记录当前 main 与 Intel oneMKL PARDISO 在同一组 16M 系统上的分解、求解、内存与精度对比，
并更正同一天早些时候一次错误测量（见末节）。结论与
[solve-simd-perm2-20260925.md](solve-simd-perm2-20260925.md) 一致：求解略快，分解约快 2 倍，内存约为 PARDISO 的 55–60%。

## 环境与方法

- 机器：笔记本，Intel Core i7-1260P（16 逻辑核），WSL2 Ubuntu 20.04，12 GB 内存，GCC 9.4。
  **噪声大**：同一程序单次运行之间求解时间可相差 30–50%，分解时间可相差约 2 倍；因此下面采用两边**交替、各自独立进程**、多轮的结果，并确认运行时没有使用 swap。
- 我们：main 37014b2，`-DVSDLSS_BLAS -DVSDLSS_METIS`，ordering 6（METIS），MKL sequential 作 BLAS，
  8 线程；`test/bench_dump_solve.c`（只用公开接口，原始编号接口 `vsdlss_m3_solve`）。
- PARDISO：MKL 2026.1（PyPI wheel），SPD（mtype 2），`test/bench_pardiso.c`，`MKL_THREADING_LAYER=GNU`，8 线程。
  最快配置：并行 METIS（`iparm[1]=3`）、两级分解（`iparm[23]=1`）、按矩阵划分并行求解（`iparm[24]=2`）、
  **迭代精化 0 步（`iparm[7]=0`）**。
- 两边读同一个 `PG_DUMP` 文件（同一矩阵、同一右端项）。求解时间为热求解中位数（15–21 次），括号内为最快一次。
- 用例：
  - **emir16M**：16M 双网，平均度数 2.5；低度消元后核心 4.43M。
  - **pgb_16M**：16M 双网，61M 用例的分支拓扑，平均度数 2.67；核心 5.79M。

## 结果（8 线程）

### 求解

| 用例 | 我们 | PARDISO 最快配置 | 我们的耗时 / PARDISO |
|---|---|---|---|
| emir16M | **0.348 s**（0.320） | 0.510 s（0.467） | 0.68 |
| pgb_16M 第 1 轮 | **0.466 s**（0.381） | 0.702 s（0.597） | 0.66 |
| pgb_16M 第 2 轮 | **0.425 s**（0.395） | 0.619 s（0.570） | 0.69 |

### 分解、内存、精度

| 指标 | emir16M 我们 / PARDISO | pgb_16M 我们 / PARDISO |
|---|---|---|
| 分解总时间（含排序/分析） | **41.6 s** / 106.1 s | **48.4–55.2 s** / 97.1–107.5 s |
| 其中排序/分析 | 约 28–34 s（核心 METIS，每网单线程，两网并发） / 96.7 s | 约 34–40 s / 85.2–95.0 s |
| 其中纯数值分解 | 约 5 s/网（两网并发） / 9.4 s | 约 6–8 s/网 / 10.6–12.5 s |
| 峰值内存（进程 VmHWM） | **4.4 GB** / 7.8 GB | **5.2 GB** / 8.4–8.5 GB |
| L 非零元 | 2.53 亿（核心，含约 11% 合并补零）+ 低度消元记录 / 3.13 亿 | 3.45 亿 + 记录 / 3.98 亿 |
| 后向误差 | 3.5e-17 / 3.8e-17 | 8.1e-17 / 4.0e-17 |

PARDISO 其他配置（同一时段，8 线程，精化 0 步）：普通 METIS（`iparm[1]=2`）+ 经典分解，求解 0.82 s（emir）/ 1.62 s（pgb），峰值 9.1–11.1 GB；`iparm[24]=0` 与 `=2` 在 emir 上相近，在 pgb 上 `=2` 更快。

## 分析

- **求解基本受内存带宽限制。** 两边都是每次把 L 前代、回代各读一遍。我们的 L 小 13–19%，实际快约 1.5 倍，
  说明除因子更小外，打包面板、32 位索引、树并行前代/回代与顺序化的消元回放、收集/写回也更省带宽。
- **分解的优势几乎全在分析阶段。** PARDISO 对完整的 1600 万节点做排序和符号分析；我们先把 1000–1160 万个
  度数 ≤3 的节点精确消去，只对核心做 METIS。只看纯数值分解，PARDISO 的 BLAS-3 内核比我们快。
- **我们分解的瓶颈是核心 METIS 排序**（占分解时间约 60–70%，每个网单线程）。后续若要缩短分解，优先考虑
  并行嵌套剖分或复用排序/符号结果，而不是数值内核。
- **内存**：我们约为 PARDISO 的 55–60%。按比例外推，64M 时 PARDISO 峰值预计超过 30 GB，在 20 GB 内存的
  服务器上大概率无法运行；我们估计约 17–18 GB（只算求解器）。

## 更正：迭代精化导致的错误测量

同一天早些时候用一份旧的 PARDISO 测试程序得到"我们求解快 5–14 倍"，**该结论作废**。旧程序没有设置
`iparm[7]`，`pardisoinit` 对 mtype 2 的默认值会做最多 2 步迭代精化，每次"求解"实际包含多次前代/回代和残差计算
（其后向误差 1e-17 也因此比我们低）。同一配置下的对照：

| PARDISO（普通 METIS，经典分解，8 线程） | 精化 0 步 | 精化 2 步（默认） |
|---|---|---|
| emir16M 求解 | 0.82 s | 5.63 s |
| pgb_16M 求解 | 1.62 s | 5.51 s |

此外，那一轮我们的 pgb_16M 求解测得 0.65 s，是在几轮 PARDISO（峰值约 11 GB）之后、已使用 swap 的状态下测的；
交替复测为 0.43–0.47 s。以后对比 PARDISO 统一用仓库中的 `test/bench_pardiso.c`（显式设置 `iparm[7]`，
默认 0），并交替运行、检查 swap。

## 复现

```bash
# PARDISO（MKL 目录按实际修改）
make bench_pardiso PARDISO_LIBS="-L<mkl>/lib -l:libmkl_rt.so.3 -Wl,-rpath,<mkl>/lib -Wl,--no-as-needed -lgomp -lm -ldl -lpthread"
MKL_THREADING_LAYER=GNU PARDISO_FACT_PAR=1 PARDISO_SOLVE_PAR=2 PARDISO_REFINE=0 ./bench_pardiso pgb_16M.bin 3 8 21

# 我们（先 make METIS=1 构建 build/metis/libmetis.a）
gcc -O2 -Iinclude -std=c11 -fopenmp -DVSDLSS_BLAS -DVSDLSS_METIS -DVSDLSS_METIS_THREADSAFE \
    -Ithird_party/metis-5.1.0/include -o bench_dump_solve test/bench_dump_solve.c src/vsdlss*.c \
    build/metis/libmetis.a -lm -L<mkl>/lib -l:libmkl_rt.so.3 -Wl,-rpath,<mkl>/lib
MKL_THREADING_LAYER=SEQUENTIAL VSDLSS_BLAS_MIN=16 VSDLSS_BLAS_SOLVE_MIN=0 ./bench_dump_solve pgb_16M.bin 6 8 21
```

`PG_DUMP` 文件格式：int64 的 n、nnz、上三角 CSC 的 p[n+1]、i[nnz]、x[nnz]，以及右端项 b[n]；
可由 `test/bench_pg_solve.c` 或 `test/bench_ibmpg.c` 设置 `PG_DUMP=文件名` 导出。
