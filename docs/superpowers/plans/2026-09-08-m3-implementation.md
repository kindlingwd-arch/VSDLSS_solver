# M3 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 独立实现可复用 RHS 的低度预消元、分量求解和真实超节点块 Cholesky，并保留 M1 作为对照。

**Architecture:** M3 facade 负责事务和所有权；components、reduction、supernodal 三个模块分别负责映射、Schur 记录和块数值计算。先独立验证模块，再接入 facade，M1 数值代码不修改。

**Tech Stack:** C11、int64_t/csi、double、现有 CSC 与符号例程、Make、ASan/UBSan；无新增第三方依赖。

**Spec:** `docs/superpowers/specs/2026-09-08-m3-design.md`

## Global Constraints

- M1 不变，独立 M3 入口；order=0..4 仅选择核心排序。
- 不依赖截断指针、原版偏移或私有 ABI。
- 因子化不依赖 RHS，solve 使用独立工作区，失败不改 solution。
- 构造失败 out=NULL，释放部分构造对象；尺寸运算先检查再分配。
- 不近似丢弃，不扰动主元，不放宽超节点，不分离仍有接口耦合的子图。
- 禁止全局 n*n 稠密工作区，禁止把 M1 数值因子打包当作块分解。
- 每个行为先写失败测试、观察失败、最小实现、回归、提交。

## 文件布局与接口契约

新增 `src/vsdlss_m3_internal.h` 统一内部类型；`src/vsdlss_components.c`、`src/vsdlss_reduction.c`、`src/vsdlss_supernodal_symbolic.c`、`src/vsdlss_supernodal_numeric.c`、`src/vsdlss_m3.c` 单一职责。新增 `test/test_m3.c` 与 `test/m3_test_helpers.h`。公开声明只放 `include/vsdlss.h`。所有索引均零基。

```c
typedef struct vsdlss_components {
    csi n, count;
    csi *offset; /* count+1 */
    csi *vertices; /* local grouped position -> original, n */
    csi *component_of, *local_of; /* original -> component/local */
} vsdlss_components;
typedef struct vsdlss_elim_record {
    csi vertex, degree, neighbor[3];
    double pivot, multiplier[3];
} vsdlss_elim_record;
typedef struct vsdlss_reduction {
    csi n, count, core_n;
    vsdlss_elim_record *records;
    csi *core_vertices;
    vsdlss *core;
} vsdlss_reduction;
typedef struct vsdlss_sn_symbolic vsdlss_sn_symbolic;
typedef struct vsdlss_sn_factor vsdlss_sn_factor;

vsdlss_status vsdlss_components_build(const vsdlss *, vsdlss_components **);
vsdlss_status vsdlss_component_extract(const vsdlss *, const vsdlss_components *, csi, vsdlss **);
void vsdlss_components_free(vsdlss_components *);
vsdlss_status vsdlss_reduce(const vsdlss *, vsdlss_reduction **);
vsdlss_status vsdlss_reduce_rhs(const vsdlss_reduction *, const double *, double *, double *);
vsdlss_status vsdlss_reduce_recover(const vsdlss_reduction *, const double *, const double *, double *);
void vsdlss_reduction_free(vsdlss_reduction *);
vsdlss_status vsdlss_sn_analyze(const vsdlss *permuted, vsdlss_sn_symbolic **);
void vsdlss_sn_symbolic_free(vsdlss_sn_symbolic *);
vsdlss_status vsdlss_sn_factorize(const vsdlss *permuted, const vsdlss_sn_symbolic *, vsdlss_sn_factor **);
vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *, const double *, double *);
vsdlss_status vsdlss_sn_export_L(const vsdlss_sn_factor *, vsdlss **);
void vsdlss_sn_factor_free(vsdlss_sn_factor *);
```

reduce_rhs 输出 core_rhs[core_n]、saved[count]；recover 输入这次求解的 saved 和 core_solution，输出原分量顺序 x[n]。空核心的指针允许 NULL。sn_factor 拥有自己的布局副本，不悬挂借用 symbolic。export_L 返回调用者拥有的新 CSC，用于对照和诊断，不是因子构造路径。

## Task 1：证据闭环与安全字段表

**Files:** 创建 `docs/reconstruction/05-m3-field-evidence.md`；读取 reference/funcs 下 newFactorBlock、deleteFactorBlock、setFactorBlockPtrs、newBlockInfo、deleteBlockInfo、readBlockInfo、writeBlockInfo、factorWithinBlock、subBlockContribFromRowRange、forwardSolveBlocked、backwardSolveBlocked、factorDeg1Deg2Pf、factorDeg3Pf，以及它们调用的分配/释放函数。

**Produces:** 逐字段证据表及新结构所有权契约；不产生运行时依赖。

- [ ] 完整读取每个选中函数；用 rg 追踪每个指针字段读写，不能从名称推定类型。
- [ ] 每行填写 owner type、offset、access width、allocator、writer、reader、deallocator、alias/owned、confidence。无法确认的字段明确列为 unresolved，不进入新结构。
- [ ] 核查 `newFactorBlock` 的 0x18/0x20 连续数据区与 0x28/0x30 指针表，区分指针表自身所有权和所指对象所有权；核查 0x00..0x14 标量。
- [ ] 明确原版低度平方根因子系数与新实现 multiplier=a/d 的转换关系，不宣称存储逐项等价。
- [ ] 自审每个新字段的创建、使用和释放路径，提交文档。证据不完整不阻止数学上独立实现，但禁止把猜测写成已恢复字段。

## Task 2：不相干分量与局部 CSC

**Files:** components 模块、内部头、测试文件、Makefile。
**Consumes/Produces:** components_build/extract/free，签名见统一契约。

- [ ] 添加手工上 CSC：p={0,1,2,4,6}, i={0,1,0,2,1,3}, x={4,5,-1,4,-2,5}。测试代码核心：

```c
vsdlss_components *c=NULL;
CHECK(vsdlss_components_build(&A,&c)==VSDLSS_OK);
CHECK(c->count==2 && c->offset[1]==2);
CHECK(c->vertices[0]==0 && c->vertices[1]==2);
CHECK(c->vertices[2]==1 && c->vertices[3]==3);
```

- [ ] 添加 test-m3 Make target；运行 `make test-m3`，确认尚缺组件实现导致失败。
- [ ] 实现迭代 BFS、最小原编号组件顺序、组件内部原编号升序；extract 通过 local_of 构建局部 CSC，禁止跨组件边。

```c
/* 原图邻居 w 只有 component_of[w]==component 才可进入局部矩阵。 */
local_row = components->local_of[original_row];
```

- [ ] 检查两个局部矩阵分别为 [[4,-1],[-1,4]] 和 [[5,-2],[-2,5]]；追加孤立点、单组件、交错编号、非法 CSC 测试。
- [ ] `make test-m3 && make test` 全通过后提交组件模块。

## Task 3：动态低度矩阵消元

**Files:** reduction 模块、内部头、test/test_m3.c。
**Consumes/Produces:** reduce/free；内部可变稀疏数值邻接及 records/core，不复用只有结构的 graph 来存数值。

- [ ] 手算 3x3 fixture：[[4,2,0],[2,5,1],[0,1,3]]。首步 v=0 后 A11=4；次步 v=1 后 A22=2.75：

```c
vsdlss_reduction *r=NULL;
CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK);
CHECK(r->count==3 && r->core_n==0);
CHECK(r->records[0].pivot==4 && r->records[0].multiplier[0]==0.5);
CHECK(r->records[1].pivot==4);
CHECK(r->records[2].pivot==2.75);
```

- [ ] 运行 `make test-m3` 观察缺少 reduce 的失败。
- [ ] 实现对称稀疏更新、带失效条目检查的度 0..3 队列；每次弹出重新核实活跃状态与当前度，同度按原编号排序。对邻居对只更新一次：

```c
multiplier[i] = a[i] / pivot;
updated = old_value - multiplier[i] * a[j];
/* 先 reserve 两端邻接，再同时插入；写入前检查 isfinite(updated)。 */
```

- [ ] 追加度2环、度3立方图、附着低度枝条的 K5 核心、纯 K5 无消元测试。K5 的每节点度4，确保真正覆盖非空核心；链和普通网格不能替代此验收。
- [ ] 测试填充触发度更新、保留数值抵消的结构边、非正主元、非有限更新；每次失败 out=NULL。全部通过再提交。

## Task 4：RHS 约化与逆序恢复

**Files:** reduction 模块、test/test_m3.c。
**Consumes/Produces:** reduce_rhs/recover；records 不可变。

- [ ] 使用 Task 3 矩阵和 x={1,-2,3}，手工 RHS={0,-5,7}：

```c
double saved[3], x[3], b[]={0,-5,7};
CHECK(vsdlss_reduce_rhs(r,b,NULL,saved)==VSDLSS_OK);
CHECK(saved[0]==0 && saved[1]==-5 && saved[2]==8.25);
CHECK(vsdlss_reduce_recover(r,saved,NULL,x)==VSDLSS_OK);
CHECK(fabs(x[0]-1)<1e-12 && fabs(x[1]+2)<1e-12 && fabs(x[2]-3)<1e-12);
```

- [ ] 运行 test-m3 并观察缺失接口失败。
- [ ] 实现副本工作向量、正序 saved 和反序恢复：

```c
saved[k]=work[record.vertex];
work[record.neighbor[i]]-=record.multiplier[i]*saved[k];
/* 逆序时邻居是后消元节点或已求解核心节点。 */
x[record.vertex]=saved[k]/record.pivot;
x[record.vertex]-=record.multiplier[i]*x[record.neighbor[i]];
```

- [ ] 追加第二个全非零 RHS、非空核心手算映射、因子记录前后不变、NULL 空核心、输入输出别名和 NONFINITE 的事务测试。
- [ ] 通过 test-m3 与全回归后提交。

## Task 5：严格超节点符号分析

**Files:** supernodal_symbolic 模块、内部头、测试 helper。
**Consumes/Produces:** sn_analyze/free，输入已排列矩阵；symbolic 保存列界、R 行表、面板偏移和更新散射映射。

- [ ] 对稠密 SPD 4x4 图要求一个宽度4超节点；对三对角4x4 图要求前三个节点宽度为1、1、2：

```c
/* 通过内部只读布局访问验证：列边界分别 {0,4} 与 {0,1,2,4}。 */
CHECK(symbolic->column_start[0]==0);
CHECK(symbolic->column_start[symbolic->count]==4);
```

- [ ] RED 后实现符号 L 模式，通过现有 etree/ereach 生成，不运行 numeric_factorize。
- [ ] 合并条件：parent[j]==j+1，列 j 的行表去掉 j 后与列 j+1 行表完全相同；构建 J×J 与 R×J 连续列主序存储和 checked size_t 前缀和。

```c
merge = parent[j] == j+1 && len[j] == len[j+1]+1;
/* merge 为真还必须逐项比较 row_j[1:] 与 row_next[:]。 */
```

- [ ] 补充不能合并的同长度异行表、单节点、多个超节点共享后续块；每个符号非零恰好映射到一个面板位置。
- [ ] 全部通过后提交。symbolic struct 的具体字段声明与本任务一起加入内部头，后续只通过该定义访问。

## Task 6：真实块数值分解与块求解

**Files:** supernodal_numeric 模块、内部头、test/test_m3.c。
**Consumes/Produces:** sn_factorize/solve/export_L/free；只借用 analyze 的布局直至复制完成。

- [ ] 添加稠密3x3 手算 A=[[4,2,0],[2,5,2],[0,2,5]]，自然序 L=[[2,0,0],[1,2,0],[0,1,2]]；添加至少两个超节点相互更新的稀疏 fixture。

```c
CHECK(vsdlss_sn_factorize(&A,symbolic,&factor)==VSDLSS_OK);
CHECK(vsdlss_sn_export_L(factor,&L)==VSDLSS_OK);
/* 将 L 展开到测试专用小型 dense 后逐项比较上述手算值。 */
```

- [ ] RED 后实现按超节点顺序装配面板、块内 chol、三角右求解、外部 rank-k 更新散射。只向符号结构允许的位置写入；缺失映射报 INVALID。

```c
/* 某源块给目标面板 (r,c) 的贡献 */
for (csi t=0;t<source_width;t++)
    target_value -= source_row_r[t] * source_row_c[t];
/* 对角块逐主元检查正性，随后 sqrt；面板解与更新独立测试。 */
```

- [ ] 实现面板前代和反代；固定排列自然序输入同一核心，调用 M1 order=2 比较 export_L 和 PA P^T-LL^T。
- [ ] 测试 RHS 重用、异构面板宽度、单超节点、跨多个后续块更新、非正定错误；测试不得仅比较最终解。
- [ ] `make test-m3 && make test` 通过后提交。

## Task 7：独立 M3 facade 与全局映射

**Files:** src/vsdlss_m3.c、include/vsdlss.h、内部头、test/test_m3.c。
**Produces:** 已确认的三个公开 API，具体签名：

```c
typedef struct vsdlss_m3_factor vsdlss_m3_factor;
vsdlss_status vsdlss_factorize_m3(const vsdlss *A,int order,vsdlss_m3_factor **out);
vsdlss_status vsdlss_m3_solve(const vsdlss_m3_factor *,const double *rhs,double *solution);
void vsdlss_m3_factor_free(vsdlss_m3_factor *);
```

- [ ] RED：交错编号两个非相干分量，非恒定解生成 RHS；调用上述入口，检查恢复原顺序、两次 RHS 和 rhs==solution。
- [ ] 因子化串联 normalize、components、reduce、order、symperm、sn_analyze/factorize。core_n==0 跳过核心流水线；映射方向固定 q[new]=old。
- [ ] 求解时先 gather 分量 RHS、reduce_rhs，置换核心 RHS、sn_solve、逆置换、recover、scatter 到临时全局结果，全部成功才 memcpy 给调用者。

```c
permuted_rhs[k] = core_rhs[q[k]];
core_x[q[k]] = permuted_x[k];
global_x[components->vertices[begin+local]] = component_x[local];
```

- [ ] 测试不同分量混合全消元/非空核心；独立对角分量；所有 order 值；非法 order；后一个分量失败时释放前面因子；M1 API 和行为不变。
- [ ] 全部通过后提交。

## Task 8：安全、对照基准与交付

**Files:** Makefile、test/test_m3.c、test/bench_m3.c、README.md、docs/reconstruction/CONTINUATION.md、字段证据表。

- [x] 将 test-m3 放入 test 和 sanitizers；新增文件依赖内部头，确保头文件变化触发重编译。测试专用分配失败注入通过内部 allocator 包装，不加入公开 API。
- [x] 逐个失败分配序号测试构造事务与释放；增加大尺寸溢出路径的内部单元测试，不实际申请巨大内存。
- [x] 固定种子小 SPD 图：A=加权图 Laplacian+I；M1/M3 两路比较解、原始残差。度3立方图检查低度消元；K5 检查单一稠密超节点，三维网格检查多超节点、外部行和更新映射。

```c
CHECK(vsdlss_backward_error(A,x_m3,b,&eta)==VSDLSS_OK);
CHECK(eta<=1e-12);
/* 小型 dense oracle 在 test helper 内独立实现，禁止调用被测块核。 */
```

- [x] 基准报告分量数、度0/1/2/3消元数、core_n/core_nnz、超节点数/最大宽度、面板存储、阶段时间、峰值工作区估计、M1/M3 残差。时间不作 pass/fail；无真实大规模测试时不宣称百万节点可用。
- [x] 执行 `make clean && make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test`；再 `make sanitizers`。支持时开启 leak detection；若环境限制则明确记录，不能写成已做泄漏验证。
- [x] 执行同核心同排列因子对照与完整流程基准；核对所有实际执行分支，不能以“用了 M1 fallback”冒充 M3 通过。
- [ ] 更新文档中的实际完成状态和证据置信度；实现方严格验证和本地检查点已完成，独立代码审查及审查后的再次全量验证留给根任务。

## 自审与执行边界

任务1覆盖逐字段证据；任务2覆盖独立分量；任务3/4覆盖动态低度与恢复；任务5/6覆盖严格超节点和真实块更新；任务7覆盖API/映射/事务；任务8覆盖对照和安全。规格不要求逐位原版兼容，不把块宽度等同节点度。任务 8 复选框依据 2026-09-10 的严格测试、sanitizer 和基准结果更新。

已发表的 MLD 版本仍需另外核查其与既定 FM/Hopcroft-Karp 设计的差距：当前增益细化和增广路匹配不应称为完全相同算法。M3 仅依赖排序是合法双射及其符号一致性；不要在本计划中静默扩大范围重写 MLD，也不要延续“完全复现原版”的结论。
