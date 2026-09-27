#!/usr/bin/env python3
"""Summarize tools/ci_solve_scaling.sh logs into markdown tables.

usage: ci_solve_summary.py <out_dir>

Per order: SOLVE / INTERNAL wall time (min over reps and rounds) for each
thread count and variant (base, new, pull), and the tree-solve phases from
VSDLSS_SOLVE_PROFILE (min per component, summed over components).  Checks:
bitwise_same flags, and new vs pull xhash (push and pull forms must agree
bit for bit)."""
import glob, os, re, sys
from collections import defaultdict

out = sys.argv[1]
VAR = ["base", "new", "pull"]
NAME = {"base": "原始", "new": "本版", "pull": "本版-拉式"}
ORD = {"5": "AMD", "6": "METIS"}

wall = defaultdict(lambda: 1e30)          # (v, ord, kind, nt) -> s
phase = defaultdict(lambda: [1e30] * 4)   # (v, ord, nt, comp_n) -> [fs, ft, bt, bs]
xhash = defaultdict(set)                  # (v, ord) -> hashes
flags = []                                # bitwise failures
factor = {}

for path in sorted(glob.glob(os.path.join(out, "run_*_o*_r*.log"))):
    v, o = re.match(r".*run_(\w+?)_o(\d+)_r\d+\.log", path).groups()
    section = None
    for line in open(path, errors="replace"):
        m = re.match(r"@@ threads=(\d+)", line)
        if m:
            section = int(m.group(1)); continue
        if line.startswith("# xhash"):
            xhash[(v, o)].add(line.split("=", 1)[1].strip()); section = None; continue
        if line.startswith("# factor") and o not in factor:
            factor[o] = re.sub(r" page_faults.*", "", line.strip()[2:])
        m = re.match(r"(SOLVE|INTERNAL) threads=(\d+) min=([\d.]+).*?(bitwise_same(?:_as_solve)?=(\d))", line)
        if m:
            k = (v, o, m.group(1), int(m.group(2)))
            wall[k] = min(wall[k], float(m.group(3)))
            if m.group(5) != "1": flags.append(f"{os.path.basename(path)}: {line.strip()}")
            continue
        m = re.search(r"solve profile: n=(\d+) nt=\d+ .*fwd sub ([\d.]+) top ([\d.]+) \| bwd top ([\d.]+) sub ([\d.]+)", line)
        if m and section is not None:
            k = (v, o, section, int(m.group(1)))
            vals = [float(x) for x in m.groups()[1:]]
            phase[k] = [min(a, b) for a, b in zip(phase[k], vals)]

meta = open(os.path.join(out, "meta.txt")).read().strip() if os.path.exists(os.path.join(out, "meta.txt")) else ""
print("# 多线程求解扩展性 A/B（单 RHS）\n")
print("```\n" + meta + "\n```\n")
print("原始 = 稠密求解优化前；本版 = 当前提交；本版-拉式 = 当前提交 + `VSDLSS_FWD_SUB_PUSH=0`。")
print("时间为 SOLVE_REPS 次与各轮中的最小值（ms）。SOLVE 含排列/收集/写回，INTERNAL 为内部求解。\n")

def ms(x): return "-" if x >= 1e29 else f"{x*1e3:.1f}"

orders = sorted({k[1] for k in wall}, key=lambda o: {"6": 0, "5": 1}.get(o, 2))
for o in orders:
    print(f"## {ORD.get(o, 'order ' + o)}\n")
    if o in factor: print(f"`{factor[o]}`\n")
    nts = sorted({k[3] for k in wall if k[1] == o})
    for kind in ("SOLVE", "INTERNAL"):
        print(f"### {kind}\n")
        print("| 线程 | 原始 | 本版 | 本版-拉式 | 本版/原始 提速 | 推式/拉式 | 本版扩展（相对 1 线程） |")
        print("|---:|---:|---:|---:|---:|---:|---:|")
        one = wall[("new", o, kind, 1)]
        for t in nts:
            b, n, p = (wall[(v, o, kind, t)] for v in VAR)
            sp = f"{b/n:.2f}×" if n < 1e29 and b < 1e29 else "-"
            pp = f"{p/n:.2f}×" if n < 1e29 and p < 1e29 else "-"
            sc = f"{one/n:.2f}×" if n < 1e29 and one < 1e29 else "-"
            print(f"| {t} | {ms(b)} | {ms(n)} | {ms(p)} | {sp} | {pp} | {sc} |")
        print()
    rows = []
    for t in nts:
        if t < 2: continue
        for v in VAR:
            comps = [k for k in phase if k[0] == v and k[1] == o and k[2] == t]
            if not comps: continue
            s = [sum(phase[k][i] for k in comps) for i in range(4)]
            rows.append((t, v, len(comps), s))
    if not rows:
        print("（没有树求解分阶段数据：线程数不超过分量数时，各分量单线程串行求解，不走树路径。）\n")
    if rows:
        print("### 树求解分阶段（VSDLSS_SOLVE_PROFILE，各分量最小值求和，ms）\n")
        print("| 线程 | 版本 | 分量 | 前代子树 | 前代树顶 | 回代树顶 | 回代子树 | 合计 |")
        print("|---:|---|---:|---:|---:|---:|---:|---:|")
        for t, v, nc, s in rows:
            print(f"| {t} | {NAME[v]} | {nc} | " + " | ".join(f"{x*1e3:.1f}" for x in s) + f" | {sum(s)*1e3:.1f} |")
        print()
    hn, hp, hb = xhash[("new", o)], xhash[("pull", o)], xhash[("base", o)]
    same = "一致" if hn and hn == hp else "**不一致**"
    print(f"结果哈希：本版 {sorted(hn)}，本版-拉式 {sorted(hp)}（推式与拉式应逐位一致：{same}）；"
          f"原始 {sorted(hb)}（宽度 >32 的分块回代改变舍入，预期不同）。\n")

print("## 逐位检查\n")
print("所有线程数 bitwise_same=1。" if not flags else "**存在失败：**\n\n" + "\n".join(f"- `{f}`" for f in flags))
