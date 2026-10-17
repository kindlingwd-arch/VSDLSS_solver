#!/usr/bin/env bash
# Pipelined-solve analysis on the data of transient_driver: exports the
# structure once (export_pipeline, factorization as in production), then
# runs tools/pipeline_analysis.py (pure Python 3, no numpy) on it.
#
#   make export_pipeline METIS=1 [BLAS=1 BLAS_LIBS=...]    (as the library is built)
#   cd <data directory with diag.txt data.txt>
#   bash pipeline_test.sh <path to export_pipeline> <threads> <fwd ms> <core ms> <bwd ms> [core ms at 8 threads per net]
#
# threads: all solver threads (two nets: each net gets half).  fwd / core /
# bwd: one net's "reduce forward", "core", "reduce backward" from a
# VSDLSS_TRACE=1 solve at that thread count (e.g. 32 40 28 32 23).
# The export stays in the output directory; rerun the analysis with other
# times without factoring again:
#   python3 pipeline_analysis.py <out>/pipeline_export.txt --threads 16 --fwd 40 --core 28 --bwd 32
# Output: pipeline_test_<time>/  (export, logs, summary.txt)
set -u
BIN=${1:?usage: bash pipeline_test.sh <export_pipeline> <threads> <fwd ms> <core ms> <bwd ms> [core ms at 8 threads]}
T=${2:?threads}; FWD=${3:?fwd ms}; CORE=${4:?core ms}; BWD=${5:?bwd ms}; CAP=${6:-0}
ORDER=${ORDER:-6}; NETS=${NETS:-2}
HERE=$(cd "$(dirname "$0")" && pwd)
[ -x "$BIN" ] || { echo "not executable: $BIN"; exit 1; }
[ -f "$HERE/pipeline_analysis.py" ] || { echo "missing $HERE/pipeline_analysis.py"; exit 1; }
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
OUT=pipeline_test_$(date +%m%d_%H%M); mkdir -p "$OUT"
PER=$(( T / NETS )); [ "$PER" -lt 1 ] && PER=1
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)"
  echo "export: $BIN order $ORDER, $T threads | per net: $PER threads, fwd $FWD ms, core $CORE ms, bwd $BWD ms, core at 8 threads ${CAP} ms"
} | tee "$OUT/summary.txt"
env MKL_THREADING_LAYER=SEQUENTIAL "$BIN" text:. "$ORDER" "$T" "$OUT/pipeline_export.txt" > "$OUT/export.log" 2>&1
rc=$?
grep -a "^#" "$OUT/export.log" | grep -v "^# numa" | sed 's/^/    /' | tee -a "$OUT/summary.txt"
if [ $rc -ne 0 ]; then
  { echo "    EXPORT FAILED ($rc)"; tail -n 3 "$OUT/export.log" | sed 's/^/    | /'; } | tee -a "$OUT/summary.txt"; exit 1
fi
echo | tee -a "$OUT/summary.txt"
python3 "$HERE/pipeline_analysis.py" "$OUT/pipeline_export.txt" --threads "$PER" \
    --fwd "$FWD" --core "$CORE" --bwd "$BWD" --core-cap "$CAP" 2>&1 | tee -a "$OUT/summary.txt"
echo; echo "done: $OUT/summary.txt"
