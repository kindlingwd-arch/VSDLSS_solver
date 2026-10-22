#!/usr/bin/env bash
# The core solve's critical path and thread scaling, in one go:
#   1. build bench_phase (METIS, MKL as tools/ab_test.sh)
#   2. bench_phase on the largest component: the heaviest root-to-leaf chain
#      of the core's supernodal tree (its size, diagonal vs external bytes, the
#      largest panels on it), and the solve phases at 1 / 8 / 16 / 32 threads
#
#   cd <data directory with diag.txt data.txt>
#   bash <source>/tools/core_path_test.sh <source directory> ["threads" = "1 8 16 32"] [reps = 5]
#
# Threads bound as in production (OMP_PROC_BIND=spread, OMP_PLACES=cores),
# memory interleaved (VSDLSS_INTERLEAVE=1), MKL sequential.
# Output: core_path_<time>/  (build.log, phase.log, summary.txt = the result to send back)
set -u
SRC=$(cd "${1:?usage: bash core_path_test.sh <source directory> [threads] [reps]}" && pwd)
TH=${2:-1 8 16 32}; REPS=${3:-5}
export OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores} MKL_THREADING_LAYER=SEQUENTIAL
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
grep -q core_path "$SRC/test/bench_phase.c" || { echo "this source's test/bench_phase.c has no core critical path: update it"; exit 1; }
OUT=core_path_$(date +%m%d_%H%M); mkdir -p "$OUT" || exit 1
S=$OUT/summary.txt
say() { echo "$*" | tee -a "$S"; }
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)"
  echo "source: $SRC ($(git -C "$SRC" log -1 --format='%h %s' 2>/dev/null || echo 'no git'))  threads: $TH  reps: $REPS  load: $(cut -d' ' -f1-3 /proc/loadavg)"
} | tee "$S"

if [ -z "${BUILD+x}" ]; then        # as tools/ab_test.sh: METIS, and MKL when found
    BUILD="METIS=1"; MKLRT=""
    [ -n "${MKLROOT:-}" ] && MKLRT=$(find "$MKLROOT" -name 'libmkl_rt.so*' 2>/dev/null | head -n 1)
    [ -z "$MKLRT" ] && MKLRT=$(ldconfig -p 2>/dev/null | awk '/libmkl_rt\.so/ {print $NF; exit}')
    if [ -z "$MKLRT" ]; then
        for d in ${LD_LIBRARY_PATH:+${LD_LIBRARY_PATH//:/ }} /opt/intel/oneapi/mkl/latest/lib /opt/intel/oneapi/mkl/latest/lib/intel64 \
                 /opt/intel/mkl/lib/intel64 ${CONDA_PREFIX:+$CONDA_PREFIX/lib} "$HOME"/.local/lib; do
            for f in "$d"/libmkl_rt.so*; do [ -e "$f" ] && { MKLRT=$f; break 2; }; done
        done
    fi
    [ -z "$MKLRT" ] && MKLRT=$(timeout 60 find /opt /usr /home -maxdepth 8 -name 'libmkl_rt.so*' 2>/dev/null | head -n 1)
    if [ -n "$MKLRT" ]; then D=$(dirname "$MKLRT"); BUILD="METIS=1 BLAS=1 BLAS_LIBS='-L$D -lmkl_rt -Wl,-rpath,$D'"
    elif ldconfig -p 2>/dev/null | grep -q libopenblas; then BUILD="METIS=1 BLAS=1 BLAS_LIBS=-lopenblas"; fi
fi
echo; say "=== 1. build: make -C $SRC bench_phase $BUILD"
rm -f "$SRC/bench_phase"
if ! eval make -C "\"$SRC\"" -j"$(nproc)" bench_phase $BUILD > "$OUT/build.log" 2>&1; then
    say "    BUILD FAILED"; tail -n 15 "$OUT/build.log" | sed 's/^/    | /' | tee -a "$S"; exit 1
fi
case "$BUILD" in *BLAS=1*) ;; *) say "    WARNING: built without BLAS/MKL (production uses MKL)";; esac
say "    ok"

echo; say "=== 2. core critical path and solve phases (bench_phase, order 6, threads $TH, median of $REPS)"
t0=$(date +%s)
env VSDLSS_INTERLEAVE=1 "$SRC/bench_phase" text:. 6 "$TH" "$REPS" > "$OUT/phase.log" 2>&1
say "    ($(($(date +%s) - t0)) s, log phase.log)"
grep -aE "^# [^ ]+: n |^# core|^T=|^# results" "$OUT/phase.log" | sed 's/^/    /' | tee -a "$S"
say "    (heaviest chain: one solve direction reads it panel after panel at any thread count;"
say "     diagonal = triangular solves (dtpsv), external = products (dgemv))"

echo; say "Done. Please send: $S"
