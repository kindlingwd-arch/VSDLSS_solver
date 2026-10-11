# 最新 main 尾部回代分析：证据与复现

绑定生产提交：`85792d29b4fe91c87d8cb16e919666b0d8f578e4`。报告见 [算法与测量分析](../../reconstruction/tail-backward-main-analysis-20261011.md)，研究方向见 [调度参考](../../reconstruction/tail-backward-scheduling-references-20261011.md)。

## 文件与有效范围

| 文件 | 来源 | 可支持的结论 |
|---|---|---|
| `wires-default-1.log`、`wires-default-4.log` | 默认编号；ABBA 的 A | 100 万 wires 的结构、逐次尾部 / 粒度 / solve 时间和数值检查 |
| `wires-chain-2.log`、`wires-chain-3.log` | 链连续编号；ABBA 的 B | 同一输入链编号后的结构、逐次时间和数值检查 |
| `default-a.log` | 已有 400 万 far 输入，首版诊断 | **只采用结构和数值结果**；计时与一次生成器编译重叠，全部性能时间排除 |
| `summary.csv` | 从以上日志抽取的原始摘要行 | 便于比较进程中位数；不包含重新拟合或混合统计 |
| `manifest.json` | 文件指纹和使用状态 | 核对原始日志、输入和分析提交 |
| `reproduce/` | 独立诊断 C、生成器、附加 Makefile、Bash 入口 | 重新测量相同生产代码；不是新的回代调度器 |
| `validation.md` | 本文档 PR 的新验证记录 | 核对可移植复现入口及测试结果；不替换原始基准 |

没有提交生成的矩阵二进制、诊断二进制、归档的完整求解器源码或第三方论文 PDF。原始日志原样保存，具有路径的首轮分析仅作数据来源；复现入口已移除特定 Windows 用户路径。

## 复现

Linux / WSL，需要 Bash、Git、GNU make 和 GCC；生产 Makefile 会构建 bundled METIS。输出路径应不包含空白，输出约包含 40 MB 生成输入和日志。可从仓库任意路径启动：

```sh
bash docs/benchmarks/tail-backward-main-20261011/reproduce/run_wires.sh /tmp/vsdlss-tail-research
```

脚本检查 `src/`、`include/`、使用的两个基准源文件、根 Makefile 和 METIS 源码与绑定提交一致，然后严格编译、生成输入并核对 SHA256，运行 A–B–B–A。后续生产代码改变时，先恢复绑定生产快照或重新建立新版本基准，不能把新版本数字写成原版本的复现结果。

检查仅在该次 Git 命令中规范化 LF / CRLF，不修改用户 Git 配置。若从 WSL 访问 Windows Git 创建的绝对路径 worktree，需为调用进程设置 Linux 格式的 `GIT_DIR` / `GIT_WORK_TREE` 映射；普通 Linux clone 无需这一步。

C 诊断在同一分解上交替 grain=2048 与 256；入口清除环境中的 grain 覆盖，调用当前 main 的现有全局粒度设置。线程数由诊断显式设置为 1/2/4/8；其他继承的 `VSDLSS_*` 环境会写入 `environment.log`。要复现默认设置，应使用未配置额外 `VSDLSS_*` 的环境。

每种模式预热 2 次、15 个有效样本；每次结果与本进程 1 线程参考逐位比较，失败即非零退出。完整 solve 另外检查后向误差。默认编号与链编号的消元顺序不同，跨模式不宣称逐位一致。

100 万输入可重新生成，SHA256 为 `be612edbc86d656b69608836a8e9abdd8cac9951ae6ebb5052cfd58a0f1611f6`。400 万 far 的 SHA256 为 `c457b9b56fb64822593ee5f4f21d7713833a395518742b55b24baf22f786e9d0`；该已有输入未随 PR 提交，不能仅凭这份 PR 重新生成它。

## 诊断边界

尾部测试通过 main 的 packed reduction 视图执行，将块段置空，仍保留生产入口和空块的线程区域。每次先前消元、紧接回代，中间不进行全向量验证；重置、比较和残差均不计入回代时间。短尾部入口成本不能被误读为全部 solve 成本。

依赖深度统计利用消元次序和当前 packed layout 的不变量：尾部邻居是后续尾部主元或核心。它不是执行新调度计划的测试，也不是供任意损坏输入使用的通用图验证器。新计划仍需单独证明所有依赖、记录覆盖及数值行为。

这里记录的是 WSL 笔记本上的证据；用户的 3300 万 / 2809 万生产网和目标服务器尚未在本轮验收。实际线程数、原始样本和统计含义优先于“理论线性加速”的估计。
