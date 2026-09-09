# M4 多列磁盘路径与 M5 验收准备

日期：2026-09-09。基线 `7b11f95`；分支 `codex/m4-panels-m5-validation`。

## 本次实现

1. `src/vsdlss_panel.c` 提取 M3 与 M4 共用的面板 Cholesky、更新内积、前代和回代内核。M4 不调用 M3 全内存分解后再落盘。
2. `vsdlss_sn_analyze_compact` 复用严格超节点识别，但不分配右看式全局更新散射表 `update_target`。准备阶段得到结构后，根据预算及 `max_columns` 拆分超节点。块的行索引保持升序，首 width 行是本块连续列。
3. `src/vsdlss_m4_panel.c` 在磁盘初始化全部面板；随后释放准备阶段的矩阵与符号布局。数值阶段只保留源块和目标块两个有界缓冲、置换、块目录、列所属块索引。一个源块分解后，按外部行所属目标块分组更新，目标块一次读入/更新/写回，不再逐列扫描整个输入和所有历史列。
4. 块载荷按连续缓冲读写，包含共享行索引和列主序数值。求解按块顺序/逆序流式读取并调用同一套 M3 内核。
5. v2 因子可原子保存、释放、重新打开并复用；头/目录及每个块分别校验 FNV-1a 64 位完整性摘要。对同维矩阵使用错误因子仍需调用方检查残差，格式不包含输入矩阵身份签名。
6. CLI 支持 `--block-cols`、`--save-factor`、`--load-factor`、`--m3`，并报告块数、多列块数、实际最大列宽、工作区和块 I/O 次数。
7. `tools/validate_m5.py` 提供独立残差、重建路径交叉对照、持久因子跨进程复用、输入/执行文件 SHA-256 证据和可选原版命令适配器验收。

## 内存口径

设 n 为维数。数值工作区的保守基数为 `sizeof(vsdlss_m4_factor)+256+64*n`，包含置换、最多 n 个 40 字节块描述、列所属块索引及求解向量余量。块 payload 为 `8*rows*(width+1)`。数值阶段的预算为基数加两个最大 payload，不持有完整数值 L 或符号 L。

宽度上限根据最坏 rows=n 先选定，再按严格超节点边界拆块；实际峰值通过 `vsdlss_m4_workspace_bytes`/stats 返回，不超过传入预算。不足以容纳两个单列块时，显式 `vsdlss_factorize_m4_ex` 返回 OOM。原 `vsdlss_factorize_m4` 自动优先使用 v2（默认最大宽度 8），预算过小时保留 v1 逐列回退。v1 不支持持久化；CLI 指定保存或块宽时强制使用 v2。

该预算仍然**不是整个进程的硬内存上限**。规范化、排序、完整符号准备阶段在内存中进行，可能消耗 O(nnz(L)) 的结构空间；准备阶段结束后释放。调用方 A/RHS/输出、libc、堆分配器和栈也不计入。全流程外存排序/符号分析不属于本次已实现能力，不能称为固定 RSS 的全外存求解器。

## v2 磁盘格式

全部 8 字节单元均为显式 little-endian。double 要求 IEEE-754 binary64。使用 POSIX 64 位文件定位。

| 字节偏移 | 内容 |
|---|---|
| 0 | 魔数 `56 53 44 4c 53 53 34 00` |
| 8 | 版本 2 |
| 16 | n |
| 24 | 块数 |
| 32 | 预算限制后的列宽上限 |
| 40 | 载荷起始偏移：64+8n+40×块数 |
| 48 | 文件精确总字节数 |
| 56 | 元数据 FNV-1a 摘要，覆盖前 7 个头字段、置换和块目录 |
| 64 | q[new]=old，n 个 uint64 |
| 64+8n | 每块 5 个 uint64：begin、width、rows、offset、checksum |
| 载荷区 | 每块先写 rows 个全局行号，再写 rows×width 个 binary64；上三角槽固定为零 |

目录必须连续覆盖所有列，载荷偏移连续且无尾随字节。加载先检查形状/乘法/预算/精确长度，再检查排列双射、元数据摘要、行号顺序、连续对角行、有限数值及正对角；随后才交付因子。每次 solve/save 重新核对头/目录，每块读取再次核对摘要。FNV 用于意外损坏检测，不是密码学认证，不能保证检测一切恶意伪造。

临时工作文件仍由 mkstemp 独占创建后立即 unlink。保存使用目标同目录的临时文件，完整复制后 fflush、fsync、关闭，最后 rename；失败保留原目标并移除临时文件。未承诺断电后的目录项持久性（未 fsync 父目录），也没有半分解断点恢复。保存的完整因子可跨进程重用。并发修改同一因子或外部改写其文件不受支持。

## 算法证据映射

| 原反编译流程 | 重建实现 | 验证 |
|---|---|---|
| `factorWithinBlock` / `factorWithinBlockExt1To3`：块内数值分解 | M3/M4 共用 `vsdlss_panel_factor` | M3 既有回归、独立稠密重构 |
| `subBlockContribFromRowRange`：块贡献更新 | `vsdlss_panel_dot` + 根据目标块局部行索引的散射更新 | 网格五种排序、稠密宽度 1/2/4/8 |
| `writeFactorChunk/readFactorChunk`：块级 I/O、索引与数值 | v2 连续载荷 I/O、目录与摘要 | 截断、位翻转、目录/版本损坏、真实短写 |
| `forwardSolveBlocked/backwardSolveBlocked` | 共用 `vsdlss_panel_solve` | 多 RHS、原地失败保护、跨进程复用 |
| `saveJobToDiskFreeMemoryJrt/loadJobFromDiskJrt`：保存与恢复生命周期 | 重建因子 save/open；不沿用原版私有布局 | 释放后重新打开、独立进程 replay |

映射是流程原理，不是原符号 ABI 或文件字节兼容承诺。

## M5 的实际边界

路线图原文要求“有原始可执行程序和真实样本后，分别验收文件、API 和数值行为”。当前仓库提供的是反编译源码及重编译产物，**没有确认可运行的原始 asim_3d/原版库及真实输入输出样本**。`reference/PLAN_C_RESULT.md` 已指出重编译数值内核损坏；不能将它当原版 oracle。

因此本次交付 M5 的可执行验收准备，不能签署 M5 原版兼容通过：

| 项目 | 当前状态 |
|---|---|
| 重建公共 API、文件适配、M1/M3/M4 数值一致性 | 自动回归覆盖 |
| v2 因子保存/加载、损坏拒绝 | 自动回归覆盖 |
| 原版文件/数值对照 | 工具已就绪；缺原程序和真实样本 |
| 原版 37 参数 ABI | 未验证，未新增伪 ABI wrapper |
| 部分分解、一般 LU/不定系统、线程并行 | 路线图要求在原版证据后另定范围；本次未实现 |

### 对照工具用法

先用生成数据运行重建回归（不代表真实原版样本）：

```sh
python3 test/gen_sparse.py test_sparse 24
# 将下面 JSON 存为仓库根目录 manifest.json：
# {"cases":[{"job":"test_sparse"}]}
python3 tools/validate_m5.py manifest.json --output report.json
python3 tools/validate_m5.py manifest.json --require-original --output report.json
```

无原版时第一条返回 0 表示重建回归通过，第二条返回 3，报告明确 M5 未验收。错误或差异返回 1，不留下伪通过结果。`test/test_m5.py` 验证这些状态。

真实样本 manifest 每个 case 提供 `job`、必要时 `extra_files`，以及 `sha256` 映射（文件名到 SHA-256，必须覆盖全部输入）。可选顶层 `oracle` 为：

```json
{
  "kind": "original",
  "argv": ["/absolute/path/to/original-adapter", "{job}"],
  "sha256": "实际执行文件的SHA-256"
}
```

命令参数必须明确指定 `{job}`，不经过 shell，逐 case 在独立临时目录执行；适配器须将原版结果转换成 `<job>.rsl` little-endian binary64。适配器背后的原版依赖、版本、许可证和原版输入映射还需随样本提供；工具不会凭适配器声明证明二进制来源，也不会把文件协议验收升级成 37 参数 ABI 验收。

## 验证结果

- `test_m4_panels`：24 阶稠密 SPD，在相同矩阵/预算下数值块读取数从列宽 1 的 300 次降到列宽 8 的 6 次；不是通用耗时或生产规模性能承诺。
- 64 阶网格，order 0–4：独立 `PAP^T=LL^T` 每元素误差低于 1e-12，后向误差低于 1e-12；预算触发宽度 2 的自动拆块。
- 损坏元数据/数值、短头、错误版本、尾随字节、低预算、非 SPD/NaN RHS、强制短写、保存失败保留旧目标均有测试。
- M5 工具覆盖 M1/M3/两种 M4 宽度和跨进程 replay；真实原版兼容明确未验收。

完整严格构建与 sanitizer 命令、最终结果见 CONTINUATION.md。
