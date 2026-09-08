# 开发续接记录

最后更新：2026-09-08。

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

单元测试还覆盖：2x2 精确解、非恒定正负交替三对角解、自然序/RCM、一个因子三个 RHS、`PAP^T` 与 `LL^T` 重构、非正定/奇异/NaN/Inf、非法 CSC、重复坐标、文件短读和输出失败。

LeakSanitizer 在当前受 ptrace 管理的环境无法读取 `/proc/<pid>/task`，因此 `make sanitizers` 设置 `ASAN_OPTIONS=detect_leaks=0`，仍运行 AddressSanitizer 与 UndefinedBehaviorSanitizer。

## 下一阶段

M2 应从反编译证据恢复真正的动态最小度或嵌套剖分排序，并以 `nnz(L)`、运行时间和内存为指标与 M1 RCM 比较。不要恢复已删除的同名空壳。M3 再依据块结构的生产者/消费者证据实现超节点或小块专门内核；M4 才进入磁盘分块。

所有新增算法必须先建立独立失败测试，并与 M1 解、因子重构及后向误差交叉验证。
