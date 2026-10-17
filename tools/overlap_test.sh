#!/usr/bin/env bash
# Feasibility of decoupling the low-degree replays from the core solve
# (test/bench_overlap.c): structure of the interface response H, core solve
# and replays alone and at the same time, cost of the two extra products.
#
#   make bench_overlap METIS=1 [BLAS=1 BLAS_LIBS=...]    (as the library is built)
#   cd <data directory with diag.txt data.txt>
#   bash overlap_test.sh <path to bench_overlap> [threads list="16 32"] [reps=9]
#
# 16 threads is the share of one net in the two-net solve on 32 threads.
# The two thread groups of part B are started by the program itself, so
# OMP_PROC_BIND is set to false here.
# Output: overlap_test_<time>/  (one .log per thread count + summary.txt)
set -u
BIN=${1:?usage: bash overlap_test.sh <path to bench_overlap> [threads list] [reps]}
THREADS=${2:-"16 32"}
REPS=${3:-9}
ORDER=${ORDER:-6}
[ -x "$BIN" ] || { echo "not executable: $BIN"; exit 1; }
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
OUT=overlap_test_$(date +%m%d_%H%M); mkdir -p "$OUT"
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Thread|Core|Socket|NUMA"
  echo "bench: $BIN  order: $ORDER  threads: $THREADS  reps: $REPS"
} | tee "$OUT/summary.txt"
for t in $THREADS; do
  echo; echo "=== $t threads   (load before: $(cut -d' ' -f1-3 /proc/loadavg))" | tee -a "$OUT/summary.txt"
  env MKL_THREADING_LAYER=SEQUENTIAL OMP_PROC_BIND=false "$BIN" text:. "$ORDER" "$t" "$REPS" > "$OUT/T$t.log" 2>&1
  rc=$?
  if [ $rc -ne 0 ]; then
    { echo "    EXIT CODE $rc (see $OUT/T$t.log)"; tail -n 3 "$OUT/T$t.log" | sed 's/^/    | /'; } | tee -a "$OUT/summary.txt"
  fi
  grep -a "^#" "$OUT/T$t.log" | grep -v "^# numa" | sed 's/^/    /' | tee -a "$OUT/summary.txt"
done
echo; echo "done: $OUT/summary.txt"
