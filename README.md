# VSDLSS Solver Reconstruction

这是一个依据 VSDLSS 反编译证据和稀疏直接法数学原理重新实现的求解器。`reference/` 保存原始 Ghidra 输出和调用证据；它不参与构建。`src/` 是独立、可维护、经过测试的新实现，不声称与原程序逐行一致或 ABI 兼容。

## 当前能力（M2）

- 实数 double、对称正定矩阵（SPD）。
- 0-based、上三角 CSC 输入，索引类型为 `int64_t`。
- 输入校验、列内排序、重复坐标求和，且不修改调用方矩阵。
- 自然序、Reverse Cuthill–McKee、动态最小度和多层嵌套剖分排序。
- 按实际消元图插入填充边；报告预测 `nnz(L)`、新增填充边和消元树高度。
- MLD 迭代发现连通分量，以伪外围 BFS 层递归分割，并在失衡时构造边界分隔集。
- 消元树、符号列计数、稀疏 Cholesky `A = LL^T`。
- 一个因子顺序求解多个 RHS；失败时不覆盖输出。
- NaN/Inf、非法 CSC、非正定矩阵、I/O 错误和不支持操作的明确状态。
- RedHawk 风格 `.hdr/.matd/.matf/.matt/.mato/.rhs` 文件适配。

暂不支持一般 LU、不定 LDLT、原版私有数据布局、超节点、磁盘分块、GPU、并行和原版 37 参数 ABI。

## 与反编译原理的对应

| 反编译中确认的阶段 | 新实现 |
|---|---|
| 排序 `MLDOrder/minDegreeOrder` | 动态最小度与递归 MLD；恢复算法阶段和不变量，不声称相同内部容器或逐项 tie-break |
| 符号消元 `symbolicElimination` | 消元树、reach 和精确的 L 列容量预测 |
| 数值分解 `factorA/factorAOneChunkNew` | 内存内稀疏 Cholesky；保留相同的“排序→符号→数值”分层原理 |
| 前代与回代 `solveLoadCase` | `L y = Pb`、`L^T z = y`、`x = P^T z` |
| 多 load case | `vsdlss_factor_solve` 复用同一只读因子 |

原版包含全内存和磁盘分块两条路径，并对小块使用专门内核。M1 先恢复可信的数学闭环；后续超节点和分块实现将与 M1 的因子及残差逐项对照。

## 构建与测试

```bash
make
make test
make sanitizers
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
| MLD | 11913 | 87 | `7.438e-15` |

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

置换约定固定为 `q[new]=old`、`pinv[old]=new`，并由性质测试验证双射。

更详细的证据边界、审计和路线见 `docs/reconstruction/`。
