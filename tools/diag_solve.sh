#!/usr/bin/env bash
# VSDLSS solve diagnostics for a slow large case.
#
#   ./diag_solve.sh "<command that factors and solves, with {T} for the thread count>"
#   e.g. tools/diag_solve.sh "./vsdlss_solver --m3 --threads {T} job64M"
#        tools/diag_solve.sh "./transient_driver diag.txt data.txt"   (threads via VSDLSS_THREADS)
#
# Writes everything to diag_<host>_<time>/ and prints a summary.  It only
# reads system state and runs the given command; it changes no settings.
set -u
CMD=${1:?usage: $0 "<command with {T}>"}
OUT=diag_$(hostname)_$(date +%m%d_%H%M); mkdir -p "$OUT"
THREADS=${THREADS:-"32 16 8 4 1"}   # STEPS (default 3) limits transient_driver steps per run
numa_counters() { grep -E "^(numa_hint_faults|numa_pages_migrated|numa_pte_updates) " /proc/vmstat 2>/dev/null; }
run() {  # run <tag> <threads> [env...]
    local tag=$1 t=$2; shift 2
    local c=${CMD//\{T\}/$t}
    echo "== $tag T=$t: env $* $c" >> "$OUT/runs.txt"
    local n0; n0=$(numa_counters)
    /usr/bin/time -v env "$@" VSDLSS_TRACE=1 STEPS=${STEPS:-3} OMP_NUM_THREADS=$t VSDLSS_THREADS=$t $c > "$OUT/$tag.T$t.out" 2> "$OUT/$tag.T$t.err"
    # kernel NUMA balancing during the run (system-wide counters)
    paste <(echo "$n0") <(numa_counters) | awk '{printf "   kernel %s +%d\n", $1, $4-$2}' >> "$OUT/runs.txt"
    # wall phases of the solve(s) as printed by VSDLSS_TRACE, one line per component and phase
    grep -a "trace: solve:" "$OUT/$tag.T$t.err" | sed -E 's/.*solve: +(.*[^ ]) +([0-9.]+) s .*/\1|\2/' |
        awk -F'|' '{s[$1]+=$2; c[$1]++} END{for(k in s) printf "   solve phase %-18s %8.3f s summed over %d lines (components x solves)\n", k, s[k], c[k]}' | sort >> "$OUT/runs.txt"
    grep -aE "Maximum resident|Elapsed|Major .*page faults|Percent of CPU" "$OUT/$tag.T$t.err" | sed 's/^/   /' >> "$OUT/runs.txt"
    grep -aiE "solve|time|numa:|^step" "$OUT/$tag.T$t.out" | head -8 | sed 's/^/   out: /' >> "$OUT/runs.txt"
}

echo "### 1. machine" | tee "$OUT/system.txt"
{ lscpu | grep -E "Model name|^CPU\(s\)|Thread|Socket|NUMA node";
  echo "avx2 cpus: $(grep -c avx2 /proc/cpuinfo), avx512f cpus: $(grep -c avx512f /proc/cpuinfo)"
  echo "nproc (usable by this process): $(nproc)"; cat /sys/fs/cgroup/cpu.max 2>/dev/null | sed 's/^/cgroup cpu.max: /'
  numactl -H 2>/dev/null | head -12; free -g; swapon --show 2>/dev/null
  echo "THP: $(cat /sys/kernel/mm/transparent_hugepage/enabled 2>/dev/null)"
  echo "NUMA nodes online: $(cat /sys/devices/system/node/online 2>/dev/null), kernel.numa_balancing: $(cat /proc/sys/kernel/numa_balancing 2>/dev/null)"
  echo "governor: $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 2>/dev/null)"
  uptime; } >> "$OUT/system.txt" 2>&1
echo "### 2. environment" >> "$OUT/system.txt"
env | grep -E "^(OMP_|KMP_|MKL_|GOMP_|VSDLSS_|OPENBLAS|LD_PRELOAD)" >> "$OUT/system.txt"
echo "### 3. binary" >> "$OUT/system.txt"
BIN=$(echo "$CMD" | awk '{print $1}')
ldd "$BIN" 2>/dev/null | grep -E "gomp|iomp|mkl|openblas|blas" >> "$OUT/system.txt"

# A. as you run it now (inherited environment)
for t in $THREADS; do run asis $t; done
# B. recommended environment
for t in $THREADS; do run rec $t MKL_THREADING_LAYER=SEQUENTIAL OMP_PROC_BIND=spread OMP_PLACES=cores; done
# C. recommended + NUMA interleave: numactl if installed, else VSDLSS_INTERLEAVE=1
#    (examples/transient_driver interleaves itself; other programs ignore it)
if command -v numactl >/dev/null; then
    CMD0=$CMD; CMD="numactl --interleave=all $CMD0"
    for t in 32 16; do run rec_numa $t MKL_THREADING_LAYER=SEQUENTIAL OMP_PROC_BIND=spread OMP_PLACES=cores; done
    CMD=$CMD0
else
    for t in 32 16; do run rec_interleave $t VSDLSS_INTERLEAVE=1 MKL_THREADING_LAYER=SEQUENTIAL OMP_PROC_BIND=spread OMP_PLACES=cores; done
fi
# D. which OpenMP runtimes are loaded at run time (libiomp5 next to libgomp = two thread pools)
#    (inherited environment; LD_DEBUG lists every library as it is initialised)
env LD_DEBUG=libs STEPS=1 OMP_NUM_THREADS=2 VSDLSS_THREADS=2 ${CMD//\{T\}/2} 2>&1 >/dev/null |
    grep -aE "calling init: .*(gomp|iomp|mkl_)" | sed -E 's/.*calling init: //' | sort -u > "$OUT/runtime_libs.txt"

echo; echo "Done: $OUT/  (system.txt, runs.txt, runtime_libs.txt, per-run .out/.err)"
cat "$OUT/system.txt"; echo; cat "$OUT/runs.txt"; echo; echo "runtime libraries:"; cat "$OUT/runtime_libs.txt"
