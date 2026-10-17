#!/usr/bin/env bash
# A/B of the backward pipeline (VSDLSS_PIPE_BWD=1) in the transient solve.
#
#   make transient_driver METIS=1 [BLAS=1 BLAS_LIBS=...]    (as before)
#   cd <data directory with diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt ...>
#   bash pipe_bwd_test.sh <path to transient_driver> [threads=32] [steps=5] [rounds=3]
#
# 1. Timing: `rounds` pairs of runs, pipeline off / on (order alternated per
#    round against drift), without the trace (it reads /proc on every line).
# 2. Bitwise: every run prints the FNV hash of each step's solution
#    (X_HASH=1); all off and on runs must give the same hashes.
# 3. Phases: one run each with VSDLSS_TRACE=1 (median of every "solve:"
#    line); with the pipeline, "core" + "reduce backward" become
#    "core + tail backward" + "reduce backward (blocks)".
# OMP_PROC_BIND / OMP_PLACES etc. are taken from the caller's environment.
# Output: pipe_bwd_test_<time>/  (one .log per run + summary.txt)
set -u
DRV=${1:?usage: bash pipe_bwd_test.sh <transient_driver> [threads] [steps] [rounds]}
T=${2:-32}; STEPS=${3:-5}; ROUNDS=${4:-3}
[ -x "$DRV" ] || { echo "not executable: $DRV"; exit 1; }
for f in diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
OUT=pipe_bwd_test_$(date +%m%d_%H%M); mkdir -p "$OUT"
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)"
  echo "driver: $DRV  threads: $T  steps: $STEPS  rounds: $ROUNDS"
  echo "env: OMP_PROC_BIND=${OMP_PROC_BIND:-} OMP_PLACES=${OMP_PLACES:-} VSDLSS_INTERLEAVE=${VSDLSS_INTERLEAVE:-}"
} | tee "$OUT/summary.txt"

run() {   # run <name> <pipe 0/1> <trace 0/1> <steps>
    local name=$1 pipe=$2 trace=$3 steps=$4
    env MKL_THREADING_LAYER=SEQUENTIAL VSDLSS_THREADS=$T STEPS=$steps CHECK_RESIDUAL=0 X_HASH=1 \
        VSDLSS_PIPE_BWD=$pipe VSDLSS_TRACE=$trace "$DRV" diag.txt data.txt > "$OUT/$name.log" 2>&1
    local rc=$?
    local med; med=$(grep -a "warm solves" "$OUT/$name.log" | sed -E 's/.*median ([0-9.]+) ms, min ([0-9.]+) ms.*/median \1 ms  min \2 ms/')
    printf "    %-8s pipe=%d  %s%s\n" "$name" "$pipe" "${med:-no warm solves}" "$([ $rc -ne 0 ] && echo "  EXIT CODE $rc")" | tee -a "$OUT/summary.txt"
    [ $rc -ne 0 ] && tail -n 3 "$OUT/$name.log" | sed 's/^/      | /' | tee -a "$OUT/summary.txt"
}

echo; echo "=== 1. timing (no trace)" | tee -a "$OUT/summary.txt"
for r in $(seq 1 "$ROUNDS"); do
    if [ $((r % 2)) -eq 1 ]; then run off_$r 0 0 "$STEPS"; run on_$r 1 0 "$STEPS"
    else run on_$r 1 0 "$STEPS"; run off_$r 0 0 "$STEPS"; fi
done
med_of() {   # median of the runs' warm medians
    for f in "$OUT"/$1_[0-9]*.log; do grep -a "warm solves" "$f" | sed -E 's/.*median ([0-9.]+) ms.*/\1/'; done |
        sort -g | awk '{a[NR]=$1} END {if(NR) print a[int((NR+1)/2)]; else print "nan"}'
}
OFF=$(med_of off); ON=$(med_of on)
awk -v a="$OFF" -v b="$ON" 'BEGIN {printf "    warm median over rounds: off %.2f ms, on %.2f ms, change %+.2f ms (%+.1f%%)\n", a, b, b-a, a>0?100*(b-a)/a:0}' |
    tee -a "$OUT/summary.txt"

echo; echo "=== 2. bitwise (x_hash of the first solve and every step)" | tee -a "$OUT/summary.txt"
REF=""; SAME=1
for f in "$OUT"/off_[0-9]*.log "$OUT"/on_[0-9]*.log; do
    h=$(grep -ao "x_hash=[0-9a-f]*" "$f" | tr '\n' ' ')
    [ -z "$h" ] && { echo "    $(basename "$f"): no hashes (driver without X_HASH?)" | tee -a "$OUT/summary.txt"; SAME=0; continue; }
    if [ -z "$REF" ]; then REF=$h; elif [ "$h" != "$REF" ]; then SAME=0; echo "    $(basename "$f") differs: $h" | tee -a "$OUT/summary.txt"; fi
done
[ $SAME -eq 1 ] && echo "    identical in all runs: ${REF%% *} ..." | tee -a "$OUT/summary.txt" || echo "    NOT IDENTICAL" | tee -a "$OUT/summary.txt"

echo; echo "=== 3. phases (VSDLSS_TRACE=1, median ms per line, all components)" | tee -a "$OUT/summary.txt"
run trace_off 0 1 3; run trace_on 1 1 3
USED=$(grep -ac "solve: core + tail backward" "$OUT/trace_on.log")
echo "    pipeline used in $USED component solves (0: the reduction is not single-window / relabelled, normal path ran)" | tee -a "$OUT/summary.txt"
phases() {
    grep -a "vsdlss trace: solve" "$1" | sed -E 's/^vsdlss trace: (.*[^ ]) +([0-9.]+) s  rss.*/\1|\2/' |
        awk -F'|' '{v[$1]=v[$1] " " $2*1000; if(!($1 in o)) o[$1]=++n} END {for(k in o) print o[k] "|" k "|" v[k]}' | sort -n |
        while IFS='|' read -r _ label vals; do
            m=$(echo $vals | tr ' ' '\n' | sort -g | awk '{a[NR]=$1} END {print a[int((NR+1)/2)]}')
            printf "      %-34s %8.1f  (%d lines)\n" "$label" "$m" "$(echo $vals | wc -w)"
        done
}
for k in off on; do echo "    pipe $k:"; phases "$OUT/trace_$k.log"; done | tee -a "$OUT/summary.txt"
echo; echo "Done. Please send: $OUT/summary.txt"
