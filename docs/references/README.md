# 参考文献

求解策略讨论中用到的三份资料。仓库只收录允许再分发的那一份，其余两份给出官方链接。

## 1. Scott & Tůma，Algorithms for Sparse Linear Systems（2023）

- 文件：[Scott-Tuma-2023-Algorithms-for-Sparse-Linear-Systems.pdf](Scott-Tuma-2023-Algorithms-for-Sparse-Linear-Systems.pdf)（原样收录，未修改）
- Jennifer Scott, Miroslav Tůma. *Algorithms for Sparse Linear Systems*. Nečas Center Series, Birkhäuser, 2023. ISBN 978-3-031-25820-6，[doi:10.1007/978-3-031-25820-6](https://doi.org/10.1007/978-3-031-25820-6)
- 许可：开放获取，[CC BY 4.0](http://creativecommons.org/licenses/by/4.0/)

与本项目相关的部分：

- 第 1.2.1 节、第 4 章：分析阶段只依赖稀疏模式，可跨多次数值分解复用（M3 目前把排序、符号、数值放在同一个调用里）。
- 第 4.6 节：电源网格填充少、超节点小，需要宽松合并；超节点内部列重排（本项目已试，求解快 0–5%）。
- 第 7.4 节：混合精度迭代精化、对角缩放。
- 第 8.1 节：最小填充（MF/AMF）准则，介于 AMD 与嵌套剖分之间。
- 第 8.5 节、9.5 节：加边界块形式、Schur 补、区域分解（按图划分分块、NUMA 本地化）。
- 第 9–11 章：以已有因子作预条件的 Krylov 方法；M 矩阵上不完全 Cholesky 不崩溃，保持行和的 MIC 适合拉普拉斯型矩阵。

## 2. Li & Liu，Parallel Sparse and Data-Sparse Factorization-Based Linear Solvers（2026）

- Xiaoye Sherry Li, Yang Liu. arXiv:2602.14289, 2026. <https://arxiv.org/abs/2602.14289>
- 许可：arXiv 非独占分发许可（只授权 arXiv 分发），未收录 PDF。

与本项目相关的部分：

- 第 2.1 节：三角求解的点对点同步（Park 等）。本项目的树顶计数器调度和尾部窗口调度已覆盖并超过。
- 第 2.2–2.3 节：3D 通信规避框架、subtree-to-subcube 映射。核心思想是子树静态绑定到处理器组，对应本项目的 NUMA 静态映射（`tools/numa_test.sh`）。
- 第 3 节：按块形状选内核的决策树（PanguLU）、不规则批量内核（MAGMA irrLU/irrTRSM/irrGEMM）、常驻 GPU 的求解器（Caracal）。
- 第 4–5 节：BLR/HSS/HODBF 低秩压缩与 ZFP 有损压缩；压缩后的分解作 GMRES 预条件子（算法 1）。

## 3. Davis, Rajamanickam & Sid-Lakhdar，A survey of direct methods for sparse linear systems（2016）

- Timothy A. Davis, Sivasankaran Rajamanickam, Wissam M. Sid-Lakhdar. *Acta Numerica* 25 (2016) 383–566，[doi:10.1017/S0962492916000076](https://doi.org/10.1017/S0962492916000076)
- 作者自存的技术报告版：<http://faculty.cse.tamu.edu/davis/publications.html>
- 许可：剑桥大学出版社版权，未收录 PDF。

与本项目相关的部分：

- 第 12.1 节：Cholesky 秩 k 更新/降秩（Davis–Hager），耗时与 L 中改动的元素数成正比；改动的列是消元树上从 w 的首个非零到根的路径。适用于 ECO、短路/开路分析等小改动后重解。CHOLMOD 的 Modify 模块为 GPL，需按论文自行实现。
- 第 12.2 节：对超节点稠密对角块显式求逆（Anderson–Saad、Raghavan），即 `25-solve-ledger-kernels-20260927.md` 中的 P1-2。
- 第 8.4 节：AMMF（比 AMD 少约 25% 浮点运算，排序时间多约 25%）、MMDF；嵌套剖分与 halo AMD / CAMD 的混合排序。
- 第 9.1 节：CHOLMOD 按“浮点运算量 / |L|”是否大于 40 选择超节点或按行（up-looking）方法。
