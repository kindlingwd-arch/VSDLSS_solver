# M2 排序证据映射

| 反编译责任 | 重建实现 | 验证 | 状态 |
|---|---|---|---|
| `minDegreeOrder_vsdlss` 的最小度入口 | `vsdlss_min_degree_order` | 六节点布尔消元 oracle | 算法责任已验证；私有队列布局未恢复 |
| `minDegreeOrderArr_vsdlss` 的低度/通用分派 | 当前统一动态最小度内核 | 顺序、填充边、`nnz(L)` | 数学行为已验证；专门低度内核留待 M3 |
| `MLDOrder_vsdlss` 的顶层排列与合法性检查 | `vsdlss_mld_order` + `vsdlss_validate_permutation` | 9×9 网格与不变量测试 | 阶段职责已验证 |
| `MLDOrderInternal_vsdlss` 的递归分区 | 迭代组件发现、伪外围 BFS 层分隔、7/8 平衡回退、最小度叶子 | 网格数值求解、星形图无跨边平衡分区、2048 孤立点 | 原理已重建；原版匹配/随机细化未声称等价 |
| 排序进入符号与数值分解 | `vsdlss_order_analyze` → `vsdlss_numeric_factorize` | 预测与实际 `nnz(L)` 相等 | 已验证 |

置换统一采用 `q[new]=old`、`pinv[old]=new`。`reference/` 不参与编译，以上映射只说明可观察责任和数学不变量，不说明 ABI、内存布局或精确 tie-break 兼容。
