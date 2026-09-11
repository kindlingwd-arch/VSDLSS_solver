# 快速测试、接口接入与源码发包

## 1. 环境与首次运行

已验证环境：Linux、GCC、GNU Make、Python 3；C11、POSIX 文件接口，默认链接 OpenMP 和 libm。不依赖原版库、BLAS 或 PARDISO。Windows 请使用 Linux/WSL 或另行验证 POSIX 兼容环境；不是原生 MSVC 发布包。

在仓库根目录或解压后的源码根目录执行：

```sh
make clean
make -j4 smoke
```

四条路径都应输出 `x=[1 2 3]` 和 `PASS`：M1、M3、M4、M4 reduced。后向误差应小于 1e-12。示例源码为 `examples/quickstart.c`，矩阵与 RHS 已写在文件中，不需要准备输入文件。M4 reduced 的这个极小示例会完全消元；保留磁盘核心的混合案例由 test-reduced-dag 覆盖。

无 OpenMP 环境：先 `make clean`，再 `make OPENMP=0 smoke`。切换 OPENMP、编译器或 CFLAGS 必须 clean；Make 不自动追踪参数变化。默认线程数为 1，线程数大于 1 在串行构建中返回 UNSUPPORTED。

## 2. 测试命令

| 命令 | 内容 |
|---|---|
| `make smoke` | 四种公开求解入口的最小已知解测试 |
| `make test` | 全部回归：输入、排序、低度处理、因子/残差、微内核、M4、DAG、M5 gate |
| `make test-m3` | M3 因子与恢复、分配失败注入 |
| `make test-m4 test-m4-panels` | 磁盘分块、保存/重开与 I/O 故障 |
| `make test-reduced-dag` | 混合空/非空核心、预算、恢复、单分量 DAG 一致性 |
| `make test-small` | 1–6 阶微内核与通用核逐位对照 |
| `make sanitizers` | ASan/UBSan；清理重编译，不含已通过的 LSan/TSan 声明 |
| `make bench-m3 bench-parallel` | 性能观察，不等于生产性能验收 |

严格构建：

```sh
make clean
make -j4 CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test smoke
```

## 3. 使用文件输入

```sh
make
python3 test/gen_sparse.py demo 10
./vsdlss_solver demo
./vsdlss_solver --m3 --threads 4 demo
./vsdlss_solver --disk-budget 1048576 demo
./vsdlss_solver --m4-reduced --disk-budget 1048576 demo
```

生成器同时生成 hdr/matd/matf/matt/mato/rhs；该案例已知解为全 1。实际文件规范和读取实现见 `src/vsdlss_io.c`、`test/gen_sparse.py`。缺失 RHS 默认报错，只有显式 `--demo-rhs` 才生成演示 RHS。CLI 成功返回 0，失败返回非零并打印错误。每次运行会写同一 job 对应的解文件，保留比较结果时请复制或使用不同 job 名称。

保存与重开完整 M4 因子：

```sh
./vsdlss_solver --disk-budget 1048576 --save-factor demo.factor demo
./vsdlss_solver --disk-budget 1048576 --load-factor demo.factor demo
```

load-factor 的 job 必须对应原因子矩阵；维度相同不能证明矩阵相同。文件为本项目 v2 格式，不兼容原版私有格式。组合 M4 reduced 不支持 save/load。DAG 是实验性内存分解路径，用 `--m3 --dag --threads 4` 显式启用；默认关闭，小矩阵上可能显著变慢。

## 4. C 接口契约

公开头：`include/vsdlss.h`，内部头不属于稳定接口。示例使用 0-based 上三角 CSC：`p` 长度 n+1，`i/x` 长度至少 p[n]，`p[0]=0`，每列行号不大于列号，`nz=-1`。索引 `csi=int64_t`，数值 `double`。只接收实对称正定矩阵；输入只存一侧，不要同时填入上下三角。输入验证/规范化处理排序与重复项，调用者仍必须保证数组内存与声明长度有效。

三阶示例：

```
A = [[4,-1,0],[-1,4,-1],[0,-1,3]]
p = [0,1,3,5]
i = [0,0,1,1,2]
values = [4,-1,4,-1,3]
b = [2,4,7]
x = [1,2,3]
```

| 路径 | 创建 | 求解 | 释放 |
|---|---|---|---|
| M1 | vsdlss_factorize | vsdlss_factor_solve | vsdlss_factor_free |
| M3 | vsdlss_factorize_m3 | vsdlss_m3_solve / vsdlss_m3_solve_many | vsdlss_m3_factor_free |
| M4 | vsdlss_factorize_m4 / vsdlss_factorize_m4_ex | vsdlss_m4_solve | vsdlss_m4_factor_free |
| M4 reduced | vsdlss_factorize_m4_reduced | vsdlss_m4_reduced_solve | vsdlss_m4_reduced_free |

order：0 默认 MLD，1 RCM，2 自然序，3 最小度，4 MLD。因子创建内部保留求解所需数据，不获取用户输入数组所有权；可对同一因子顺序求解多个 RHS。创建失败 out 为 NULL；公开求解失败保持输出不变；支持 rhs 与 solution 为同一数组。释放函数接受 NULL，释放后指针不得复用。

多 RHS：`vsdlss_m3_solve_many(f,nrhs,rhs,ldrhs,out,ldout)`，按列存放，每列为一个长度 n 的 RHS，两个 leading dimension 均至少 n，nrhs 至少 1。输出整批成功后提交。M4 reduced 对象不得传入此入口；M4 和 M4 reduced 同一因子不支持外部并发调用。

`vsdlss_set_num_threads(n)` 是调用线程局部设置，默认 1；先检查返回状态。`vsdlss_parallel_last_team_size()` 是观察到的最大线程组，不是利用率。`vsdlss_set_dag_enabled(1)` 只打开内存数值分解 DAG。

M4 budget 限制磁盘数值工作区及其约定元数据，不是进程总内存上限。M4 reduced 的 disk_budget 是所有子因子的该工作区之和，预处理、核心矩阵和恢复向量额外驻留。budget 太小会失败；不要将磁盘模式理解为任意矩阵都能装入该内存。临时目录必须存在且可写；可通过 CLI --temp-dir 或 API directory 指定。

| 状态 | 含义及排查 |
|---|---|
| OK | 成功 |
| INVALID | 维度、CSC、参数或内部布局不合法 |
| OOM | 内存分配失败或尺寸溢出 |
| NOT_POSDEF | 非正主元，检查是否确为 SPD、是否缺少参考节点 |
| NONFINITE | NaN/Inf 或计算溢出 |
| IO | 文件操作、完整性或预算相关路径失败，检查日志与输入 |
| UNSUPPORTED | 不支持的排序、线程或功能组合 |

使用 `vsdlss_status_string` 打印返回状态。`vsdlss_backward_error(A,x,b,&eta)` 可用于独立检查。不得只以“返回成功”代替数值验收。

## 5. 链接与打包

```sh
make libvsdlss.a
cc -O2 -std=c11 -Iinclude examples/quickstart.c libvsdlss.a -fopenmp -lm -o my_test
./my_test m3
make dist
```

输出 `dist/vsdlss-source.tar.gz` 及 SHA256 文件。包内包含本项目源码、头文件、示例、测试、工具和文档；不含 reference、旧二进制、工作区临时文件、Git 历史或凭据。源码包可在没有 Git 的目录中构建。

静态库不意味着所有运行时静态链接：默认仍需平台的 libm/OpenMP 运行时。这里交付源码发行包和构建方法，不声明跨平台二进制 ABI 稳定，也不擅自增加许可证授权。

发布验收：解压包 → make clean → make smoke → make test；在接收方目标平台重复验证。DAG 保持默认关闭。M5 测试通过只表示适配器/gate 自检通过，不表示原版兼容已验收。
