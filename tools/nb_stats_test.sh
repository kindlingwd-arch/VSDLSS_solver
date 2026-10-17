#!/usr/bin/env bash
# Neighbour-index distances of the packed low-degree records on the data of
# transient_driver (test/nb_stats.c): does a record's neighbour tend to be the
# next record (k+1) or a nearby one after the solve-order relabel?
#
#   cd <data directory with diag.txt data.txt>
#   bash <source>/tools/nb_stats_test.sh <source directory> [threads=32]
#
# Builds nb_stats into the output directory (bundled METIS, ordering 6 as in
# production; the source tree's own objects are not touched), factors once,
# prints the statistics.  Output: nb_stats_<time>/  (build.log, summary.txt)
set -u
SRC=$(cd "${1:?usage: bash nb_stats_test.sh <source directory> [threads]}" && pwd)
T=${2:-32}
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
for f in test/nb_stats.c test/bench_input.h src/vsdlss_m3_internal.h; do
    [ -f "$SRC/$f" ] || { echo "missing $SRC/$f (copy the package first)"; exit 1; }
done
OUT=nb_stats_$(date +%m%d_%H%M); mkdir -p "$OUT"
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)"
  echo "source: $SRC  threads: $T"
} | tee "$OUT/summary.txt"
echo "building nb_stats (log: $OUT/build.log) ..." | tee -a "$OUT/summary.txt"
{
    make -C "$SRC" -j"$(nproc)" METIS=1 build/metis/libmetis.a &&
    (cd "$SRC" && ${CC:-gcc} -O2 -Iinclude -std=c11 -fopenmp -DVSDLSS_METIS -DVSDLSS_METIS_THREADSAFE \
        -Ithird_party/metis-5.1.0/include -o "$OLDPWD/$OUT/nb_stats" test/nb_stats.c \
        $(ls src/vsdlss*.c) build/metis/libmetis.a -lm)
} > "$OUT/build.log" 2>&1 || { echo "BUILD FAILED"; tail -n 15 "$OUT/build.log"; exit 1; }
env OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores} \
    "$OUT/nb_stats" text:. 6 "$T" 2>&1 | grep -v "^# numa" | tee -a "$OUT/summary.txt"
cat <<'EOF' | tee -a "$OUT/summary.txt"

How to read it:
  "next record (d = 1)"  share of neighbour indices that are exactly the next record (k+1)
  "d < 2^16"             share that is a later record within 65535 (fits a 16-bit distance)
  "neighbour in the core" share pointing into the core (far away, would stay 4 bytes)
  "bytes per record"     what the replays stream now, and with 16-bit distances
EOF
echo; echo "Done. Please send: $OUT/summary.txt"
