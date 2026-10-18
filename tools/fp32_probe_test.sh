#!/usr/bin/env bash
# Accuracy of a single-precision factor on the data of transient_driver
# (test/fp32_probe.c): factor once in double, round the stored factor values
# to float, solve the same right-hand side, compare with the double solution;
# then one and two steps of iterative refinement.
#
#   cd <data directory with diag.txt data.txt>
#   bash <source>/tools/fp32_probe_test.sh <source directory> [threads=32]
#
# Builds fp32_probe into the output directory (bundled METIS, ordering 6 as
# in production; the source tree's own objects are not touched).
# Output: fp32_probe_<time>/  (build.log, summary.txt)
set -u
SRC=$(cd "${1:?usage: bash fp32_probe_test.sh <source directory> [threads]}" && pwd)
T=${2:-32}
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
for f in test/fp32_probe.c test/bench_input.h src/vsdlss_m3_internal.h; do
    [ -f "$SRC/$f" ] || { echo "missing $SRC/$f (copy the package first)"; exit 1; }
done
OUT=fp32_probe_$(date +%m%d_%H%M); mkdir -p "$OUT"
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)"
  echo "source: $SRC  threads: $T"
} | tee "$OUT/summary.txt"
echo "building fp32_probe (log: $OUT/build.log) ..." | tee -a "$OUT/summary.txt"
{
    make -C "$SRC" -j"$(nproc)" METIS=1 build/metis/libmetis.a &&
    (cd "$SRC" && ${CC:-gcc} -O2 -Iinclude -std=c11 -fopenmp -DVSDLSS_METIS -DVSDLSS_METIS_THREADSAFE \
        -Ithird_party/metis-5.1.0/include -o "$OLDPWD/$OUT/fp32_probe" test/fp32_probe.c \
        $(ls src/vsdlss*.c) build/metis/libmetis.a -lm)
} > "$OUT/build.log" 2>&1 || { echo "BUILD FAILED"; tail -n 15 "$OUT/build.log"; exit 1; }
env OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores} \
    "$OUT/fp32_probe" text:. 6 "$T" 2 2>&1 | grep -v "^# numa" | tee -a "$OUT/summary.txt"
{
  echo
  echo "How to read it:"
  echo "  float off    multipliers and core off-diagonal blocks in float (pivots, diagonal triangles double)"
  echo "  float all    every stored factor value in float"
  echo "  |x - x_double|_inf / |x|_inf   relative solution error of the float factor"
  echo "  + k refinement   after k steps of r = b - A x, x += solve(r) with the same float factor"
} | tee -a "$OUT/summary.txt"
echo; echo "Done. Please send: $OUT/summary.txt"
