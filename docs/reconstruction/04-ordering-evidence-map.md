# M2 排序证据映射

| 反编译责任 | 重建实现 | 验证 | 状态 |
|---|---|---|---|
| `minDegreeOrder_vsdlss` 的最小度入口 | `vsdlss_min_degree_order` | 六节点布尔消元 oracle | 算法责任已验证；私有队列布局未恢复 |
| `minDegreeOrderArr_vsdlss` 的低度/通用分派 | 当前统一动态最小度内核 | 顺序、填充边、`nnz(L)` | 数学行为已验证；专门低度内核留待 M3 |
| `MLDOrder_vsdlss` 的顶层排列与合法性检查 | `vsdlss_mld_order` + `vsdlss_validate_permutation` | 9×9 网格与不变量测试 | 阶段职责已验证 |
| `MLDOrderInternal_vsdlss` 的递归分区 | 迭代组件发现、多层分区、节点分隔器、最小度叶子 | 网格数值求解、星形图无跨边平衡分区、2048 孤立点 | 阶段职责与不变量已验证 |
| `MLDSHEMMatching` / `MLDCreateCoarseGraph` | `vsdlss_mld_coarsen_one` 的确定性强边匹配、权重聚合和重复粗边合并 | `test_shem_and_coarse_aggregation` | 算法原理已重建；随机顺序改为稳定代表编号 |
| `MLDInitPartition` / `MLDGrowRegionBalanceRefine` | 多起点区域增长、7/8 平衡约束和确定性增益细化 | `test_partition_refines_without_increasing_cut` | 分区职责与割权不增已验证；原版随机种子流不复刻 |
| `MLDUncoarsenRefineOneLevel` | 细粗映射投影并逐层重新细化 | 多层 30×30 网格测试 | 投影与逐层细化已验证 |
| `MLDConvertEdgeSepToNodeSep` / `MLDMinCover` | 二分边界最大匹配、Kőnig 最小顶点覆盖转节点分隔器 | `test_node_separator_removes_every_cross_edge` | 最小覆盖结构及无跨边不变量已验证 |
| 排序进入符号与数值分解 | `vsdlss_order_analyze` → `vsdlss_numeric_factorize` | 预测与实际 `nnz(L)` 相等 | 已验证 |

置换统一采用 `q[new]=old`、`pinv[old]=new`。`reference/` 不参与编译，以上映射只说明可观察责任和数学不变量，不说明 ABI、内存布局或精确 tie-break 兼容。
