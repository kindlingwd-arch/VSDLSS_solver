# 开发续接记录

最后更新：2026-09-10。


## 合并状态

新分支 `codex/integrate-solver-20260910` 整合最新 M3 安全修复与下述 M4、M5 验证及并行开发历史。保留分配失败注入、退化 MLD 分隔回退和 M3 基准。下文验证记录属于各开发里程碑。

### 整合验证（2026-09-10）

- `make -j4 CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test bench-m3` 通过。
- `make -B -j4 OPENMP=0 CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test` 通过。
- `make sanitizers` 通过：ASan/UBSan 全部 C 测试；不含 LeakSanitizer 或有效 TSan 验收。
- 保留 M3 的 OOM 与退化分隔修复；整合后分配失败扫描为分解 550、求解 15 个分配点。
- M3 基准补计并行实现新增的分量状态数组，单分量求解临时空间估算为 63876 字节，排除 OpenMP runtime；M1/M3 后向误差分别为 5.954e-16 / 6.291e-16。
- M5 gate/replay 测试通过仍不代表原版兼容已验证。

## 共享内存并行里程碑

分支 `codex/parallel-supernodal`，基于 `f3a53af`；后续章节保留此前里程碑记录。

- 已审读 `computeNodeToThread`、线程索引/错误/分配器 mutex、`vsdlss` 入口的单线程 BLAS 设置，并参考 Intel oneMKL PARDISO 的两层因子/矩阵或 RHS 求解并行说明。证据及边界见 `08-parallel-design.md`。
- 新增调用线程局部线程配置（默认 1）；OpenMP 构建默认启用，`OPENMP=0` 可完全串行编译。禁止活跃嵌套并行，小任务按估算工作量回退单线程。
- M3 独立分量并行分解/求解；新增事务式 `vsdlss_m3_solve_many`，可按多个 RHS 并行。
- M3/M4 共用面板分解的行更新并行；外部贡献按互斥目标列并行；前/回代的外部行贡献并行。主元列及源块依赖顺序保留。
- M4 I/O 仍由调用线程串行调度，两个块缓冲及显式数值预算不随线程增加；线程栈/runtime 不属于预算。同一个 M4 因子仍不得外部并发调用。
- CLI `--threads n` 报告 requested/max_team_used；M1 标量路径保持串行。
- 当前测试环境没有可写 `/tmp`，生产临时路径现在尊重 `TMPDIR`，测试用工作目录。

### 并行验证与性能

`OPENMP=0` 与 `OPENMP=1` 严格全回归均通过；ASan/UBSan 全部 C 测试通过（不含 LeakSanitizer）。1/2/4 线程专项覆盖真实 team、多分量、多 RHS、完整 M4、前/回代、非正定/NaN 失败保护；同构建内串并行结果逐位一致且后向误差 <1e-12。M4 显式工作区不随线程变化。

`parallel-benchmark.tsv` 保存五次计时中位数：4 线程面板 factor 约 2.24×，384 阶 M4 分解+求解约 1.21×。GCC 13.3，`getconf _NPROCESSORS_ONLN`=9。不是生产算例或相对 PARDISO 的性能结论。

ThreadSanitizer 尝试返回 66。已用独立显式 barrier 探针重现当前 libgomp/TSan 的同步识别问题，未登记 race gate 通过。没有添加竞争抑制规则；仍需在支持 OpenMP 的工具组合上完成有效竞态检查。

ASan/UBSan 测试维持仓库既有 `detect_leaks=0` 口径；当前环境不能通过环境变量可靠设置该默认值，新增仅测试链接的 `test/sanitizer_options.c`。生产构建不链接此文件，LeakSanitizer 未验收。

复现命令：

```sh
make clean
make OPENMP=0 CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
make sanitizers
make clean
make OPENMP=1 CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
make bench-parallel
```

下一步仍是拿到原版程序/真实样本完成 M5 文件与 ABI 验收，以及在目标机器进行大规模矩阵扩展性和有效 race gate 验证。任意消元树 DAG、并行磁盘预取、MPI/GPU、一般不定系统未实现。

## 当前最新状态：M4 多列与 M5 验收准备

最新分支 `codex/m4-panels-m5-validation`，基于已发布的 `7b11f95`。
下方“本次交付”保留初版单列阶段的历史记录；当前能力以本节和 `07-m4-panels-m5.md` 为准。

- M3/M4 复用 `src/vsdlss_panel.c` 的面板分解、更新内积和前/回代内核。
- M4 新增 `src/vsdlss_m4_panel.c`：严格超节点按预算拆分；源/目标两个缓冲；外部行按目标块分组更新；连续载荷 I/O。默认优先多列，极小预算回退 v1 单列。
- 准备阶段复用 compact M3 符号分析，不生成全局更新散射表；矩阵和符号布局在数值阶段前释放。准备阶段仍不受数值预算限制。
- v2 支持完整因子保存、跨进程打开、头/目录/块 FNV 完整性检查；原子保存失败保留旧文件。格式不兼容原版，未实现中途断点续算。
- CLI 新增块宽、保存、加载和 M3 选项；stats 显示多列块与块 I/O 数量。
- `tools/validate_m5.py` + `test/test_m5.py` 提供文件/数值对照 gate、SHA-256 输入和程序记录、独立残差与跨进程复用。原版缺失时 `--require-original` 返回 3，不得把重建内部对比当 M5 原版兼容。

### 最新验证

Linux x86-64 / GCC / GNU Make：

```sh
make clean
make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
make sanitizers
```

C 回归与 ASan/UBSan 均通过（沿用 `detect_leaks=0`；未声称验证 LeakSanitizer）。
M5 工具自测通过，明确显示 original compatibility NOT verified。
24 阶稠密矩阵数值块读取：宽度 1 为 300 次，宽度 8 为 6 次。
64 阶稀疏网格五种排序：重构误差 <1e-12、后向误差 <1e-12；预算自动拆为宽度 2 通过。
短写、保存失败保留目标、截断/损坏拒绝和失败输出保护均通过。

### 仍需证据的 M5 项目

仓库没有确认可用的原始 asim_3d/原版库及真实输入输出样本。`reference/libvsdlss_rebuilt.so` 是有已知数值损坏的反编译重编译产物，不作为 oracle。37 参数 ABI 仍未验证。部分分解、一般矩阵/不定系统、并行范围仍需原版证据和独立需求确认。

下一步最小动作：提供原程序及依赖、版本/调用约定、真实输入输出；用文档中的显式适配器 manifest 运行 M5 gate，再逐条恢复原版文件/API 契约。全流程内存硬上限仍需外存化排序与符号准备，不能据本次数值预算宣称已完成。

## 当前决定

用户确认不再追踪或修补旧的 catalog、伪 ABI、占位 MLD 和旧 harness。`reference/` 原样保留作为反编译证据，不参与构建；交付物只包含依据反编译原理重新实现的新求解器。

## M1 已实现

- 单一公共头 `include/vsdlss.h`。
- 上三角 CSC 验证、排序与重复项合并、对称 SpMV、后向误差。
- 统一置换 `q[new]=old`、`pinv[old]=new`。
- 无 512 邻居上限的 RCM，覆盖不连通图、孤立点和 1026 节点星形图。
- 稀疏 Cholesky 的符号分析、数值分解和三角求解。
- 拥有内部规范化矩阵与因子的 `vsdlss_factor`；支持多个 RHS，失败保持输出。
- 严格文件适配与新 CLI；缺失 RHS 仅在显式 demo 模式下降级。
- 已删除旧 `vsdlss_catalog`、`vsdlss_api`、`vsdlss_ds`、旧 harness 和对应头文件/测试。
- 独立审查后移除了未校验的一次性 `vsdlss_cholsolve`、`vsdlss_chol_factor` 和裸 RCM 公共入口；演示程序也只通过 `vsdlss_factor` API 求解。
- 排序、符号分析和数值分解现在逐层传播 OOM、非有限数与非正定状态，不再把内部 NULL 一律解释为非正定。
- `.hdr` 逐行严格校验，拒绝未知键、格式错误和尾随文本；零 offdiag 时也验证三个边文件存在且长度为零。

## 最新验证

环境：Linux x86-64，GCC，GNU Make。

```text
make clean
make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
exit 0
test_solver: ALL OK
test_io: ALL OK
generated n=10 file case: backward_error=8.882e-17
```

M2 的 30×30 Poisson 基准中，实际与预测 `nnz(L)` 对四种排序均完全相等：自然序 27029、RCM 19315、动态最小度 10351、MLD 11913；对应消元树高度为 900、900、109、87，最大解误差均低于 `1e-14`。

完整多层流程接入后的确定性 MLD 基准为 `nnz(L)=14837`、消元树高度 95、累计分隔节点 283、最大解误差 `5.440e-15`，预测 `nnz(L)` 与实际值一致。该版本优先恢复原版的匹配—粗化—分区—反粗化—节点分隔结构；分区质量仍可在不改变接口的前提下继续优化。

单元测试还覆盖：2x2 精确解、非恒定正负交替三对角解、自然序/RCM、一个因子三个 RHS、`PAP^T` 与 `LL^T` 重构、非正定/奇异/NaN/Inf、非法 CSC、重复坐标、文件短读和输出失败。

LeakSanitizer 在当前受 ptrace 管理的环境无法读取 `/proc/<pid>/task`，因此 `make sanitizers` 设置 `ASAN_OPTIONS=detect_leaks=0`，仍运行 AddressSanitizer 与 UndefinedBehaviorSanitizer。

## M2 与完整多层 MLD 已实现

- 可变简单消元图对称去重，并在消元时将活动邻居补成团。
- 动态最小度每步使用当前消元图度数，确定性地以原节点编号打破平局。
- `order=3` 为动态最小度；`order=4` 为递归 MLD；`order=0` 默认使用 MLD。
- MLD 先迭代发现全部连通分量，再执行确定性 SHEM 匹配、加权粗图构造、最粗层多起点区域增长、平衡/增益细化和逐层反粗化。跨分区边界通过最大匹配与 Kőnig 最小顶点覆盖转成节点分隔器；伪外围 BFS 只处理退化情形。左右递归后最后排列分隔集，32 节点及以下叶子使用动态最小度。
- 新增 `test/test_mld.c`，独立验证粗化权重、确定性分区、割权不增、节点分隔无跨边，以及 30×30 网格确实建立多层层级。
- 排序统计通过按最终排列重放消元得到，预测 `nnz(L)` 与实际因子交叉验证。

## M3 已实现

- 独立 M3 facade 拥有规范化、分量映射、0/1/2/3 度精确预消元记录、核心排列和严格超节点数值因子；同一因子可求解多个 RHS，失败不覆盖输出。
- 固定权重 Laplacian+I 的立方体、K5 和 4x4x4 网格测试同时比较独立稠密 oracle、M1、M3 和原矩阵后向误差；立方体覆盖度 3 消元，K5 明确检查单一稠密超节点，三维网格检查多个超节点、外部行和非空更新映射。
- 测试分配器对 4x4x4 网格逐一注入因子化和求解的所有分配失败，并在每次迭代核对事务结果和实时分配计数。该检查曾发现 MLD 分区错误被误当作退化分隔回退；现在 OOM 原样传播，仅成功但退化的分隔结果进入 BFS 回退。
- `bench-m3` 报告缩减度数、核心、超节点、面板、临时符号元数据、保留因子元数据、明确范围的求解峰值工作区估算、各阶段时间及 M1/M3 后向误差。它是 10x10x10 网格观测，不支持百万节点规模结论。

M4 才进入磁盘分块。当前 MLD 的增益细化和匹配实现不宣称与已发表版本逐步等价。

## M3 基线与 M4 本次交付

拉取基线为 `codex/m1-spd-reimplementation` 的 `d7b2db5`。该提交已经实现独立 M3 API、分量求解、低度缩减和超节点符号/数值内核；原续接记录的“下一步实现 M3”已过时。本次新分支为 `codex/m4-disk-blocks`。

- 新增 `src/vsdlss_m4.c` 和内部头，提供直接写盘的稀疏左看式 Cholesky、逐列磁盘前代/回代、重复 RHS 和失败输出保护。
- 公共接口位于 `include/vsdlss.h`；CLI 新增 `--disk-budget bytes` 和 `--temp-dir path`，严格校验参数。
- 明确重建格式 v1 的头、版本、端序、64 位置换/偏移/计数和 binary64 数值，不宣称原版格式兼容。
- 独占临时文件立即 unlink，因子拥有句柄；数值/写入失败和释放均关闭，不留下命名文件。
- `test/test_m4.c` 在 8 KiB 工作区下与 M1、稠密因子重构和已知解交叉验证；覆盖短读、内核强制短写、元数据损坏、尾随数据、清理、输入及预算边界。
- Makefile 默认测试补入此前遗漏的 M3，并加入 M4；sanitizers 同样包含两者。README 和 `06-m4-disk-evidence.md` 已更新。

### 本次验证

环境：Linux x86-64，GCC，GNU Make，POSIX 文件 API。

```text
make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
exit 0; solver/io/ordering/mld/m3/m4 均通过
文件样例：内存及 M4 backward_error=8.882e-17

make sanitizers
exit 0; AddressSanitizer + UndefinedBehaviorSanitizer 所有测试通过
ASAN_OPTIONS=detect_leaks=0（沿用仓库既有设置，未宣称 LeakSanitizer 验证）
```

100 阶网格的五种 order 值在 8192 字节预算下均通过：文件大于预算，稠密重构每元素误差低于 `1e-12`，三个非恒定 RHS 的解误差低于 `1e-11`、后向误差低于 `1e-12`。

### 明确边界与下一步

M4 提供最小逐列磁盘块数值闭环，并非原版块文件或 M3 多列超节点磁盘化。
预算为 `sizeof(factor)+128+40*n`，约束数值堆工作区与因子元数据；不含驻留的规范化输入、排序、路径、运行库和调用栈，不是全进程内存上限。当前逐列扫描输入/历史列且逐记录 I/O，尚未优化大规模性能。格式无校验和，不支持持久导出、重新打开或崩溃恢复。

后续最小动作：在不改变失败/预算契约下，建立有界多列缓存和列依赖索引的独立性能/数值测试，再考虑接入 M3 超节点更新。若要求整个流程的硬内存上限，需要另行将输入规范化与排序外存化。M5 原版兼容仍需真实原版程序与样本证据。

所有新增算法应建立独立失败测试，并与 M1 解、因子重构及后向误差交叉验证。
