# 参考文档 PR 验证记录

日期：2026-10-11；生产基线：`85792d29b4fe91c87d8cb16e919666b0d8f578e4`。

## 已完成检查

| 检查 | 结果 | 证据 |
|---|---|---|
| 可移植复现入口 | 四个进程全部退出 0；每次与单线程参考逐位一致，残差检查通过 | [复现运行日志](validation-reproduction.log) |
| 严格编译 | 诊断及生成器 `-O2 -Wall -Wextra -Werror -std=c11` 编译通过 | 复现脚本在编译非零退出时立即停止；四个进程均完成 |
| 生成输入 | 与原始 100 万 wires SHA256 一致 | 复现日志首行 `wires-1m.bin: OK` |
| 全套测试 | `TMPDIR=/tmp make METIS=1 test` 退出 0 | [完整测试日志](validation-full-suite.log) |
| 原始日志 | 5 个 SHA256 全部一致；4 个 wires 日志各有 60 个尾部、60 个粒度、60 个完整 solve 有效样本 | [数据清单](manifest.json)、[82 行摘要](summary.csv) |
| 文档 | 本地 Markdown 链接及空白检查通过 | `git diff --check`；逐链接解析本地目标 |
| 改动范围 | `src/`、`include/`、`test/`、生产 Makefile 与基线无差异 | `git diff --quiet origin/main -- src include Makefile test` 退出 0 |

复现是在 Windows 工作树的 WSL 环境中进行，使用进程级 `GIT_DIR` / `GIT_WORK_TREE` 路径映射，没有改写共享 Git 元数据或 Git 配置。附加 Makefile 只提供文档诊断构建目标。

## 首轮测试失败与复测

首次未显式设定临时目录的全套运行，在 `test_m4.c` 的文件截断故障注入检查 `ftruncate` 处失败，测试停止：[首轮日志](validation-initial-full-suite.log)。随后显式 `TMPDIR=/tmp ./test_m4` 通过，并用显式 Linux 临时目录重新运行完整测试，退出 0。没有修改测试或生产实现来绕过失败，也不把首轮失败日志称为通过证据。

## 统计边界

本次新复现证明文档入口可运行、输入指纹和数值约束一致；其时间没有合并到原始 ABBA 样本，也没有替换分析报告中的中位数。原始 `default-a.log` 性能时间因与编译重叠而仍然全部排除。文献和真实依赖深度不是新调度器的加速验收。
