# VSDLSS Solver Reconstruction

这是一个依据 VSDLSS 反编译证据和稀疏直接法数学原理重新实现的求解器。`reference/` 保存原始 Ghidra 输出和调用证据；它不参与构建。`src/` 是独立、可维护、经过测试的新实现，不声称与原程序逐行一致或 ABI 兼容。

## 当前能力（完整 MLD）

- 实数 double、对称正定矩阵（SPD）。
- 0-based、上三角 CSC 输入，索引类型为 `int64_t`。
- 输入校验、列内排序、重复坐标求和，且不修改调用方矩阵。
- 自然序、Reverse Cuthill–McKee、动态最小度和多层嵌套剖分排序。
- 按实际消元图插入填充边；报告预测 `nnz(L)`、新增填充边和消元树高度。
- MLD 使用确定性的 SHEM 强边匹配和加权粗图，最粗层多起点区域增长分区，并在反粗化的每一层执行平衡与增益细化。
- 跨分区边通过最大匹配和 Kőnig 最小顶点覆盖转成节点分隔器；伪外围 BFS 仅保留为退化分区的安全回退。
- 消元树、符号列计数、稀疏 Cholesky `A = LL^T`。
- 一个因子顺序求解多个 RHS；失败时不覆盖输出。
- M3 独立入口提供连通分量拆分、0/1/2/3 度精确预消元、严格超节点块 Cholesky 和可复用 RHS 求解；M1 保留为独立对照。
- NaN/Inf、非法 CSC、非正定矩阵、I/O 错误和不支持操作的明确状态。
- RedHawk 风格 `.hdr/.matd/.matf/.matt/.mato/.rhs` 文件适配。

暂不支持一般 LU、不定 LDLT、原版私有数据布局、磁盘分块、GPU、并行和原版 37 参数 ABI。当前 MLD 的增益细化和匹配实现也不宣称与已发表版本逐步等价。

## 与反编译原理的对应

| 反编译中确认的阶段 | 新实现 |
|---|---|
| 排序 `MLDOrder/minDegreeOrder` | 动态最小度与递归 MLD；恢复算法阶段和不变量，不声称相同内部容器或逐项 tie-break |
| 符号消元 `symbolicElimination` | 消元树、reach 和精确的 L 列容量预测 |
| 数值分解 `factorA/factorAOneChunkNew` | 内存内稀疏 Cholesky；保留相同的“排序→符号→数值”分层原理 |
| 前代与回代 `solveLoadCase` | `L y = Pb`、`L^T z = y`、`x = P^T z` |
| 多 load case | `vsdlss_factor_solve` 复用同一只读因子 |

原版包含全内存和磁盘分块两条路径，并对小块使用专门内核。M3 已在内存内实现严格超节点分解，并与 M1 的解及残差逐项对照；磁盘分块仍待实现。

## 构建与测试

```bash
make
make test
make sanitizers
make bench-m3
```

严格编译：

```bash
make clean
make CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' test
```

演示二维 Poisson：

```bash
./vsdlss_solve 30
```

当前 30×30（900 阶）Poisson 基准：

| 排序 | `nnz(L)` | 消元树高度 | 最大解误差 |
|---|---:|---:|---:|
| 自然序 | 27029 | 900 | `5.773e-15` |
| RCM | 19315 | 900 | `2.887e-15` |
| 动态最小度 | 10351 | 109 | `5.107e-15` |
| 完整多层 MLD | 14837 | 95 | `5.440e-15` |

这些数值用于回归当前实现，不代表某种排序对所有矩阵都更优。

文件求解：

```bash
./vsdlss_solver [-p 0|1|2|3|4] [--demo-rhs] jobname
```

`-p 0`（默认）使用 MLD，`-p 1` 使用 RCM，`-p 2` 使用自然序，`-p 3` 使用动态最小度，`-p 4` 显式使用 MLD。默认要求 `.rhs` 存在且长度准确；只有显式指定 `--demo-rhs` 才会在 RHS 缺失时构造 `b=A·1`。成功后写出 `<jobname>.rsl`。

## 公共 API

入口集中在 `include/vsdlss.h`：

1. `vsdlss_factorize(A, order, &factor)`：验证、规范化并分解。
2. `vsdlss_factor_solve(factor, rhs, solution)`：复用因子求解。
3. `vsdlss_backward_error(A, solution, rhs, &eta)`：独立计算后向误差。
4. `vsdlss_factor_free(factor)`：释放因子拥有的全部资源。

M3 对应入口为 `vsdlss_factorize_m3`、`vsdlss_m3_solve` 和 `vsdlss_m3_factor_free`。测试包含固定权重图 Laplacian+I 的独立稠密 oracle、M1/M3 对照、逐分配序号失败注入和事务检查；K5 固定为单一稠密超节点，三维网格还直接检查多个超节点、外部行和更新映射。测试分配器不属于公共 API。`bench-m3` 输出阶段时间、缩减度数、核心、超节点、面板、临时符号元数据、保留因子元数据、求解临时区峰值估算和两路后向误差。峰值覆盖 facade 数组与 `reduce_rhs`/`reduce_recover`/超节点求解的嵌套临时数组，明确排除保留因子、输入输出和分配器开销；它不是进程内存高水位，也不表示已经验证百万节点规模。

置换约定固定为 `q[new]=old`、`pinv[old]=new`，并由性质测试验证双射。

更详细的证据边界、审计和路线见 `docs/reconstruction/`。
