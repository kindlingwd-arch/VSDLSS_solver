# 内存压缩：4 字节索引、紧凑面板、分解峰值（2026-09-25）

环境同 `solve-simd-perm2-20260925.md`（i7-1260P，WSL2 13 GB，GCC 9.4，METIS + MKL，8 线程）。
所有改动均无损：解与 `590190b` 逐位一致（16M、30M 哈希相同），`make test`（并行、串行、BLAS=1）通过。

## 改动

| 项 | 内容 | 位置 |
|---|---|---|
| ① 4 字节索引 | 超节点因子的行下标、列起点、父节点、源块（`vsdlss_sni` = int32）；偏移量（`row_ptr`、`panel_offset`、`blk_ptr`）仍为 64 位；核心超过 2^31 返回 `VSDLSS_ERR_UNSUPPORTED`。局部→全局映射 `map32`、核心映射 `core_map32`（全局下标超过 32 位时保留 64 位路径） | `vsdlss_m3_internal.h`，`vsdlss_supernodal_numeric.c`，`vsdlss_m3.c` |
| ② 去掉重复表 | 分解后释放 `components->vertices`、BFS `gather`（由 `map32` 取代）和 `core_vertices`（由 `core_map` 取代；M3 回写核心解时已检查有限性，`vsdlss_reduce_backward_inplace` 在 `core_vertices` 为 NULL 时跳过重复扫描）；求解树的子树数组按实际数目收缩 | `vsdlss_m3.c`，`vsdlss_reduction.c` |
| ③ 紧凑面板 | 对角块只存下三角（按列紧凑），外部行列主序、主维 = 外部行数；存储量正好等于 `l_nnz`。分解时目标面板在每线程的完整临时面板中组装、更新、分解（运算不变），再压缩写入；求解内核由模板 `vsdlss_panel_solve.inc` 同时生成完整布局（M4 磁盘面板仍用）和紧凑布局两套，运算顺序相同；BLAS 求解路径用 `dtpsv` | `vsdlss_panel.c`，`vsdlss_panel_solve.inc`，`tools/generate_small_solve.py`，`vsdlss_simd.c` |
| ④ 分解峰值 | `vsdlss_sn_factorize_consume`：复制完布局后立即释放符号分析和置换后的核心矩阵、归还空闲内存，再分配 L；每线程 n 长的 `relmap`/`stamp` 改为对有序行表归并查找（`target_row`），每线程工作区只剩 `max_rows` 大小 | `vsdlss_supernodal_numeric.c` |

另：`VSDLSS_TRACE=1` 现在打印各阶段常驻/峰值内存和因子内存明细。

## 实测

16M（与 `590190b` 同时段交替，12 次求解中位数，两轮）：

| 用例 | 进程峰值 | 单次求解（原始编号接口） | 分解 |
|---|---|---|---|
| 16M 平均度数 2.5 | 5044 → 4315 MB（−14%） | 0.441/0.423/0.524/0.509 → 0.392/0.408/0.453/0.489 s | 不变 |
| 16M 61M 分支拓扑 | 6075 → 5070 MB（−17%） | 0.585/0.571/0.582/0.638 → 0.463/0.521/0.530/0.537 s | 不变 |

因子常驻（16M 平均度数 2.5）：3408 → 2803 MB（L 2198 → 1927，行下标 108 → 54，元数据 145 → 99，映射与顶点表 373 → 139）。

30M（`codex/powergrid-61m-solve-experiment` 分支生成器，双网，8 线程）：

| 指标 | 590190b | 本版本 |
|---|---|---|
| 分解峰值 | 10919 MB | 9621 MB（−12%） |
| 分解后常驻 | 9318 MB | 8300 MB |
| 因子内存 | 8277 MB | 6854 MB（−17%） |
| 数值分解（两网并行） | — | 11.5/12.6 → 9.8/10.9 s（去掉随机查表） |
| 单次求解（热） | 1.24 s（旧版有换页） | 1.08–1.13 s |

其中进程基线（测试程序自己的矩阵等）约 1.3 GB；求解器自身的分解峰值约 9.6 → 8.3 GB。
在服务器上（32 线程），旧版每线程两张 n 长的表在 61M 时约 2.8 GB，这部分已完全去掉。
