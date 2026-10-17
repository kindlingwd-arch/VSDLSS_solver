#!/usr/bin/env bash
# NUMA test for the transient solve: runs A / B / C / C2 and summarises them.
#
#   cd <data directory with diag.txt data.txt b_vector.txt ... 1b_vector.txt ...>
#   bash numa_test.sh <path to transient_driver> [steps=3]
#
#   A   threads spread over both sockets, memory placed by first touch (as now)
#   B   same threads, memory interleaved over both NUMA nodes (VSDLSS_INTERLEAVE=1)
#   C   node0 only: 32 physical cores (CPUs 0-31), memory local to node0
#   C2  node0 only: all 64 physical cores (CPUs 0-63)
#
# Every run records the kernel NUMA-balancing counters before and after
# (system wide: other jobs on the machine add to them too) and the load.
# Output: numa_test_<time>/  (one .log per run + summary.txt)
set -u
DRV=${1:?usage: bash numa_test.sh <path to transient_driver> [steps]}
STEPS=${2:-3}
[ -x "$DRV" ] || { echo "not executable: $DRV"; exit 1; }
for f in diag.txt data.txt b_vector.txt x_vector.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
OUT=numa_test_$(date +%m%d_%H%M); mkdir -p "$OUT"

counters() { grep -E "^(numa_hint_faults|numa_pages_migrated|numa_pte_updates) " /proc/vmstat 2>/dev/null; }

{
  echo "### machine"
  lscpu | grep -E "Model name|^CPU\(s\)|Thread|Core|Socket|NUMA"
  echo "kernel.numa_balancing = $(cat /proc/sys/kernel/numa_balancing 2>/dev/null)"
  echo "THP: $(cat /sys/kernel/mm/transparent_hugepage/enabled 2>/dev/null)"
  for n in /sys/devices/system/node/node*; do
      echo "$(basename $n): $(grep -E 'MemTotal|MemFree' $n/meminfo | awk '{printf "%s %.1f GB  ", $3, $4/1048576}')"
  done
  echo "driver: $DRV   steps: $STEPS"
} | tee "$OUT/summary.txt"

run() {   # run <name> <description> <env and command...>
    local name=$1 desc=$2; shift 2
    echo; echo "=== $name: $desc" | tee -a "$OUT/summary.txt"
    echo "    load before: $(cut -d' ' -f1-3 /proc/loadavg)" | tee -a "$OUT/summary.txt"
    local c0; c0=$(counters)
    env STEPS=$STEPS CHECK_RESIDUAL=0 "$@" diag.txt data.txt > "$OUT/$name.log" 2>&1
    local rc=$?
    local c1; c1=$(counters)
    {
      [ $rc -ne 0 ] && echo "    EXIT CODE $rc (see $OUT/$name.log)"
      grep -aE "threads=|numa:|first:|peak_rss" "$OUT/$name.log" | grep -v "warm solves" | sed 's/^/    /'
      grep -a "^step" "$OUT/$name.log" | sed -E 's/ residual=.*//; s/^/    /'
      grep -a "warm solves" "$OUT/$name.log" | sed 's/^/    /'
      paste <(echo "$c0") <(echo "$c1") | awk '{printf "    kernel %-20s +%d\n", $1, $4-$2}'
      echo "    load after:  $(cut -d' ' -f1-3 /proc/loadavg)"
    } | tee -a "$OUT/summary.txt"
}

COMMON="MKL_THREADING_LAYER=SEQUENTIAL"
run A  "32 threads spread over both sockets, first-touch memory (current way)" \
    $COMMON VSDLSS_THREADS=32 OMP_PROC_BIND=spread OMP_PLACES=cores "$DRV"
run B  "32 threads spread over both sockets, memory interleaved over both nodes" \
    $COMMON VSDLSS_INTERLEAVE=1 VSDLSS_THREADS=32 OMP_PROC_BIND=spread OMP_PLACES=cores "$DRV"
run C  "node0 only: 32 physical cores (CPUs 0-31), memory local" \
    $COMMON VSDLSS_THREADS=32 OMP_PROC_BIND=close OMP_PLACES=cores taskset -c 0-31 "$DRV"
run C2 "node0 only: 64 physical cores (CPUs 0-63), memory local" \
    $COMMON VSDLSS_THREADS=64 OMP_PROC_BIND=close OMP_PLACES=cores taskset -c 0-63 "$DRV"

echo; echo "### one line per run: warm solve median | minor faults per step | kernel hint faults / pages migrated during the run" | tee -a "$OUT/summary.txt"
for name in A B C C2; do
    awk -v n=$name '
        /warm solves/ {for(i=1;i<=NF;i++) if($i=="median") med=$(i+1)}
        /^step/ {for(i=1;i<=NF;i++) if($i ~ /^minor_faults=/){split($i,a,"="); s+=a[2]; k++}}
        END {printf "%-3s median %8s ms | minor faults/step %10.0f |", n, med, k?s/k:0}' "$OUT/$name.log" | tee -a "$OUT/summary.txt"
    awk -v n="=== $name:" '$0 ~ n {f=1} f && /kernel numa_hint_faults/ {h=$3} f && /kernel numa_pages_migrated/ {m=$3; exit} END {print " hint " (h==""?"n/a":h) " / migrated " (m==""?"n/a":m)}' "$OUT/summary.txt" | tee -a "$OUT/summary.txt"
done
echo; echo "Done. Please send: $OUT/summary.txt"
