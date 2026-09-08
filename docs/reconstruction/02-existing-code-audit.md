# 现有代码审计（静态证据）

日期：2026-09-08。以下由源码检查得出；尚无本轮编译或动态复现。P0 表示应阻止将有关路径作为可信实现，P1 表示首阶段需要修复，P2 表示后续完善。

> 处置更新：用户确认交付单一的新求解器实现。下表涉及的旧 `vsdlss_catalog`、`vsdlss_api`、`vsdlss_ds` 和旧 harness 已从活动源码删除；`reference/` 仍保留。其余 M1 问题由新矩阵、排序、因子和 I/O 模块修复并由测试覆盖。下表保留为历史审计证据。

## 可继续利用的基础

`src/vsdlss.c` 已有上三角 CSC、消元树、reach、符号列计数、稀疏 Cholesky 和三角代入。可以作为审计和加固的起点。`src/main.c` 有二维 Poisson 演示；`test/gen_sparse.py` 有文件格式样例生成器。它们尚不足以覆盖健壮性、ABI 和原版行为一致性。

## 发现清单

| 编号 | 级别 | 位置与直接证据 | 影响与下一项验证 |
|---|---|---|---|
| A01 | P0 | `src/vsdlss.c:vsdlss_rcm` 使用 `nb[512]`，超限邻居先标 visited 却不入队 | 可遗漏节点并留下未初始化置换；用超过 512 度的星形图，中心设最高编号以确保遍历触发，验证双射 |
| A02 | P0 | `src/vsdlss_catalog.c:newFactorBlockHeader_vsdlss` 将 malloc 指针转为 uint32_t；`newFactorChunk_vsdlss` 只分配 12 个 uint32_t | 指针截断；原版块为 0x70 字节且使用 8 字节字段；必须采用有类型结构，不可发布为 ABI 兼容 |
| A03 | P0 | `src/vsdlss_ds.c:mld_rec` 递归时 BFS 仍访问全局邻接，不限制当前 nodes；队列只分配 cnt 项 | 子问题可能访问子集外节点、队列越界或不收敛；没有真正分隔集；该路径需隔离直到重建并验证 |
| A04 | P1 | `vsdlss_rcm` 返回 old→new，两个高层函数再 `vsdlss_pinv`；`vsdlss_cholsolve(0)` 自然序但 `vsdlss_chol_factor(0)` RCM | 排序语义不一致；统一约定并测试实际置换，不能用残差替代 |
| A05 | P1 | `src/vsdlss_catalog.c:mainMat_vsdlss/solverVsdlss` 为重复坐标分别生成条目；`vsdlss_chol` 用 `x[Ci[p]]=Cx[p]` | 重复坐标覆盖而非累加，矩阵意义改变；在输入层合并，比较等价规范化输入 |
| A06 | P1 | `vsdlss_chol` 只判断 `d<=0`，未拒绝 NaN；公共入口未完整验证 CSC 和方阵条件 | 非法输入可能越界或返回含 NaN 的成功结果；检查单调列指针、范围、值、方阵、容量 |
| A07 | P1 | `vsdlss_chol_factor` 的分配失败分支清理不完整；malloc/calloc 直接乘尺寸 | 失败泄漏、负长度及溢出风险；检查尺寸边界并集中清理 |
| A08 | P1 | 文件入口仅检查 diag，未确认 r/c/v 成功就访问；缺失与截断 rhs 都退回 A·ones；输出打开失败也可能返回成功 | 错解或崩溃；分别测试缺文件、短文件、非法坐标、输出失败；不得把损坏 RHS 当缺省输入 |
| A09 | P1 | `vsdlss_api.h` 的 factor 类型为 `vsdlss_num`；`vsdlss_catalog.h` 为持有 N 的包装类型；存在同名不同接口实现 | 使用错误头文件可造成类型/链接不匹配；Makefile 实际 solver 使用 catalog，`src/vsdlss_api.c` 不在 APISRCS |
| A10 | P1 | `src/vsdlss_catalog.c:denseCholesky` 写下三角，而 `solveDenseUpper` 读取上三角 | 若组合用于 LLᵀ 求解会读取原始上三角；独立小矩阵验证后统一转置访问语义 |
| A11 | P1 | `Makefile:test` 依赖只有 SOLVER，却直接运行 HARNESS；`src/main.c` 最后无条件 return 0 | 新目录 `make test` 不保证构建被调用程序；演示失败不导致测试失败 |
| A12 | P2 | `src/vsdlss_catalog.c:minDegreeOrder` 只递减旧邻居度，不加入消元填充边 | 不是完整最小度排序，不能按函数名算作实现完成 |
| A13 | P2 | `MLDCoarsen/MLDFindPartition/MLDUncoarsenRefine` 等为空；`readMatrixFromFile` 为空 | 当前是占位接口；需显式能力状态和失败返回，不能静默成功 |
| A14 | P1 | `checkResults_vsdlss` 计算 max|x|，没有 A 和 b | 不能称为残差校验；增加独立 SpMV 与后向误差 |
| A15 | P2 | `typedef long longlong`；catalog 依赖 `sys/times.h`、`unistd.h` | 原生 Windows 的类型和平台构建不成立；核心与平台 I/O/计时分离 |

## 旧文档的使用边界

- README 的“faithful/correct/verified”是历史描述，本轮不认可为验收证据。
- README 对剩余不可编译函数同时出现 23 和 24，需重新运行编译清单才可更新统计。
- README 的“原版一般 LU 支持”和“真正的 MLD 实现”需要重新核验；后者当前代码并不满足嵌套剖分要求。
- `PIPELINE.md` 将某段矩阵表示写成 CSR，不能据此把磁盘格式或所有内部结构统一认定为 CSR。
- 现有源码自述来自 CSparse，后续需核对原始版本、版权声明及许可证文件，记录来源；本轮未确认实际版本。

## 当前环境验证

项目目录没有 `.git`，`git status` 返回非仓库；因此未提交、更未创建开发分支。

当前 PATH 中未发现 gcc、clang、make、cmake，找到 Python 3.14 和 wsl.exe。`wsl --list --quiet` 返回 `Wsl/EnumerateDistros/Service/E_ACCESSDENIED`。这仅证明当前调用无法访问 WSL 服务，不证明机器没有发行版或其他目录没有编译器。

本轮没有修改现有 C 文件，没有声称测试通过。正式实现前需确认可用工具链，再建立可重复的构建/测试命令并记录完整结果。
