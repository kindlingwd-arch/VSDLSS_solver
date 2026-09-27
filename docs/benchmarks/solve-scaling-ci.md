# 多线程求解扩展性 A/B（GitHub Actions / 任意 Linux 机器）

开发沙箱只有 1 个 CPU，测不出多线程墙钟。`.github/workflows/solve-scaling.yml`
在 GitHub 托管 runner（公开仓库：4 vCPU、16 GB）上运行
`tools/ci_solve_scaling.sh`，比较三个变体：

- **原始**：`1dbf969`（稠密求解优化前）
- **本版**：当前提交
- **本版-拉式**：当前提交 + `VSDLSS_FWD_SUB_PUSH=0`（单独看推式子树的贡献）

用例：8M VDD/GND 双网 power grid（`run_pg_dual_net_scale.sh 8m` 度数分布），
METIS 与 AMD；线程 1、2、4；每个线程数 7 次单 RHS 求解，多轮交替取最小，
并开启 `VSDLSS_SOLVE_PROFILE=1` 拆分树求解四个阶段。

## 触发

- 向 `test/dense-solve-kernels-*` 分支推送（改动 `src/`、脚本或 workflow 时）自动运行。
- 手动：Actions 页 → solve-scaling → Run workflow（可选 scale 8m/3m、排序、轮数）。
  注意 GitHub 只在 workflow 文件已存在于**默认分支**时显示手动按钮；未合入默认分支前
  用推送触发即可。

结果：运行页的 Summary 为对比表；原始日志在 artifact `solve-scaling-<sha>`。
共享虚拟机有噪声、只有 4 核，只看趋势；正式数据请在目标机器上运行同一脚本：

```sh
tools/ci_solve_scaling.sh 8m "6 5" 3            # THREADS="1 2 4 8 16" 可指定
```

脚本会自建 64 位 METIS（`METIS_PREFIX`，默认 `~/metis64`）。双网有两个分量，
线程数不超过 2 时各分量单线程串行、不走树路径，树阶段表从 4 线程起才有数据。
