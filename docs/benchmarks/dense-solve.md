# 单 RHS 内部稠密求解对照（2026-09-26）

独立测试分支：`test/dense-solve-kernels-20260926`，基于 main `590190b`。
仅增加基准、构建目标和实测记录；没有修改求解器算法。问题是分解完成后
`Lx=b` / `Lᵀx=b` 谁更快，不包含 POTRF、SYRK、GEMM 分解更新。

## 已实测结论

很小的1–6阶三角块，VSDLSS专用内核有优势；宽块前代较接近，宽块回代
CHOLMOD明显更快。512/1024阶回代，两轮均约4.4–4.8倍；2048阶约3.8–4.4倍。
这些都是相同L、相同RHS的单块求解，不能用此前完整稀疏求解仅差5%–7%的
结果替代，也不能把这些倍数直接外推到低度power-grid总求解时间。

下表从第一轮CSV的`actual_api`行生成，单位微秒。VSDLSS直接调用
`vsdlss_panel_solve`；CHOLMOD直接调用`cholmod_l_super_lsolve/ltsolve`。
CHOLMOD输入是完整的单超节点三角因子，先通过`cholmod_l_check_factor`验证。
不包含外部行、排列、收集、写回或堆分配。双方入口自身的检查/调度保留，
因此很小尺寸包含API开销，不能解释成纯浮点吞吐差异。

| 宽度 | V前代 | C前代 | V回代 | C回代 | 回代V/C |
|---:|---:|---:|---:|---:|---:|
| 1 | 0.008 | 0.065 | 0.009 | 0.062 | 0.14 |
| 4 | 0.020 | 0.092 | 0.030 | 0.091 | 0.33 |
| 16 | 0.667 | 0.228 | 0.277 | 0.399 | 0.69 |
| 64 | 1.577 | 1.575 | 3.956 | 2.636 | 1.50 |
| 128 | 3.887 | 3.151 | 14.565 | 6.161 | 2.36 |
| 256 | 12.048 | 10.563 | 59.927 | 15.562 | 3.85 |
| 512 | 42.257 | 42.107 | 225.643 | 47.446 | 4.76 |
| 787 | 116.567 | 112.736 | 514.986 | 118.977 | 4.33 |
| 1024 | 180.068 | 190.018 | 1004.543 | 225.728 | 4.45 |
| 2048 | 747.977 | 776.569 | 3683.401 | 834.243 | 4.42 |

## 测试口径

- 单线程、单RHS、FP64；宽度1–2048，额外行0–4096，共30个形状，分别前代和回代。
- 因子预先构造：正对角下三角L、稠密非零下三角/外部行，数值适度缩放。
  不做任何分解。用long double生成已知解对应的输入，检查两边最大绝对误差。
- 预热两轮，正式九轮，报告每次调用的中位数，以及轮间最小/最大值。
  左右交替先后顺序；按形状批处理多个独立向量，提高小块计时分辨率。
  每个向量仍为单RHS调用，未合并成多RHS BLAS-3。
- 输入恢复、分配、因子验证和已知解检查均在计时外。每个向量每轮只解一次，
  避免反复对已修改向量求解导致下溢。计时后消费输出，防止无用计算消除。
- 这是复用同一因子的热缓存/稳态形状扫描，不是多GB因子流式读取测试。
- 两个完整运行均通过；最大绝对误差约1.288e-14，门槛1e-11。
- 50个纯计算路径检查与现有VSDLSS入口逐位相同。小于8列的外部面板只测
  真实API，不用通用循环冒充专用小阶内核。
- 没有要求BLAS与VSDLSS逐位相同。BLAS可能采用FMA/不同求和顺序。

CSV scope：

| scope | VSDLSS侧 | 对照侧 | 含义 |
|---|---|---|---|
| diag_vs_blas | 真实panel入口，ext=0 | 直接DTRSV | 排除CHOLMOD入口本身；V侧仍保留自身检查和调度 |
| actual_api | 真实panel入口，ext=0 | 真实CHOLMOD super solve入口 | 完整有效的单个稠密三角块；最直接的正式结论依据 |
| panel_compute_adapter | 依据当前VSDLSS源码抽取的算术路径 | CHOLMOD单RHS使用的DTRSV+DGEMV序列 | ext向量已连续，排除稀疏收集和堆分配；是基准适配器而非真实库入口 |
| panel_total | 真实VSDLSS panel入口 | CHOLMOD单面板工作序列适配器 | 加入连续index的收集/写回；CHOLMOD侧不含公开入口检查和整个超节点遍历 |

`panel_compute_adapter`保持128行前代块、SIMD AXPY、四列dot4及对角回代顺序，
但抽取后编译器生成指令可能与原函数不同，因此不能把它的倍数当作原函数
精确加速比。它的逐位检查验证的是运算结果。`panel_total`中ext>1024时
VSDLSS回代原函数的内部malloc/free仍在计时内，CHOLMOD E缓冲预分配；这是
生产路径分配策略差异，不属于纯稠密算术差距，必须与core行分开看。
本测试没有测试随机/离散index，也不通过相减两个计时来估算“精确搬运成本”。

## 当前源码对应的原因

`src/vsdlss_panel.c`前代对角部分调用`vsdlss_simd_axpy_neg`，单个已求出的
x[j]更新多个独立输出，容易向量化。宽块回代的对角部分（generic和
simd_back_narrow中均有）仍为：

```c
for (j = width; j-- > 0;) {
    v = x[j];
    for (r = j + 1; r < width; r++) v -= a[j * rows + r] * x[r];
    x[j] = v / a[j * rows + j];
}
```

每个v有连续的数据依赖，且保持严格求和顺序限制了普通归约向量化。
CHOLMOD该段调用优化DTRSV。源码与实测共同指向宽块对角回代是一个值得
优先优化的位置，但没有硬件计数器或逐指令消融，本报告不把所有差距都
定量归于单一原因，也不承诺逐位模式一定能完全追平BLAS。

CHOLMOD对单RHS外部行用DGEMV；只有多RHS才切换TRSM/GEMM。故这里不能引用
之前GEMM慢3–11倍来推导单RHS性能。真实数据上还需统计不同(w,e)的调用数、
字节数及时间权重；节点平均度数2.5本身不能确定超节点宽度分布。

## 环境

- VSDLSS base 590190b7e7909a560af541106ca3ca9efeade8a4。
- Intel Xeon Platinum 8370C @2.80GHz，虚拟机，AVX-512；8 CPU配额、8GiB内存。
  单线程运行，未固定CPU亲和性，噪声范围保留在CSV；不外推绝对时延。
- GCC13.3，`-O2 -std=c11 -fopenmp`，最终构建启用`-Wall -Wextra -Werror`。
- SuiteSparse v7.8.3 / CHOLMOD5.3.0，Release `-O3 -DNDEBUG`，CPU路径。
- OpenBLAS0.3.30 DYNAMIC_ARCH，SkylakeX，来自SciPy，以符号跳转桩链接。
- `OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1`，VSDLSS API threads=1。
- 当前Make目标强制不启用VSDLSS外部BLAS，源码也有宏检查，防止双方实际
  都走同一个BLAS而得到误导性结果。

## 复现

需要支持`nthreads_max`的CHOLMOD（本次为5.3），以及优化BLAS。系统安装时：

```sh
make bench_dense_solve BLAS_LIBS='-lopenblas'
OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 ./bench_dense_solve 9 > result.csv
```

非标准安装位置通过现有Make变量指定；例如源码构建SuiteSparse：

```sh
make bench_dense_solve TMPDIR=/tmp \
  CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' \
  CHOLMOD_CFLAGS='-I/path/SuiteSparse/CHOLMOD/Include -I/path/SuiteSparse/SuiteSparse_config' \
  CHOLMOD_LIBS='-L/path/ssbuild/CHOLMOD -lcholmod -Wl,-rpath,/path/ssbuild/CHOLMOD' \
  BLAS_LIBS='-L/path/blas -lopenblas -Wl,-rpath,/path/blas'
OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 ./bench_dense_solve 9 > result.csv
```

SciPy的OpenBLAS需要仓库已有`tools/blas_symbol_shim.s`（含dtrsv_/dgemv_）
将Fortran名字转发到scipy_前缀，再将BLAS_LIBS指向该跳转库。需保证动态库
及其依赖可被加载。使用通用CORE2-only BLAS会严重低估对照性能。

源码：`test/bench_dense_solve.c`。原始数据：
- `dense-solve-20260926.csv`及`dense-solve-20260926-repeat.csv`。
- 同名`-checks.txt`记录每次运行检查通过。

本轮验证：已有`test_kernels`通过；新基准两轮完整运行通过；无警告构建；
`git diff --check`通过。复现命令中的可选第三方依赖不接入默认make/test。
