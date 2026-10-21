#!/usr/bin/env bash
# Where the core solve's time goes, per phase (VSDLSS_SOLVE_PROFILE=1):
#   1. build bench_phase (METIS, MKL as tools/ab_test.sh)
#   2. bench_phase on the largest component at 1 / 8 / 16 threads; per core
#      solve the times of its four phases -- forward subtrees (independent
#      subtrees in parallel), forward top, backward top, backward subtrees
#      (by branches, the backward subtrees run inside "bwd top") -- medians
#      per thread count, with the top's share of L, the largest split subtree
#      and the core's critical path
#
#   cd <data directory with diag.txt data.txt>
#   bash <source>/tools/core_profile_test.sh <source directory> ["threads" = "1 8 16"] [reps = 9]
#
# Threads bound as in production, memory interleaved, MKL sequential.
# Output: core_profile_<time>/  (build.log, phase.log, profile.log, summary.txt = the result to send back)
set -u
SRC=$(cd "${1:?usage: bash core_profile_test.sh <source directory> [threads] [reps]}" && pwd)
TH=${2:-1 8 16}; REPS=${3:-9}
export OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores} MKL_THREADING_LAYER=SEQUENTIAL
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
grep -q "largest subtree" "$SRC/src/vsdlss_supernodal_numeric.c" || { echo "this source has the old solve profile: update src/vsdlss_supernodal_numeric.c"; exit 1; }
OUT=core_profile_$(date +%m%d_%H%M); mkdir -p "$OUT" || exit 1
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
rm -f "$SRC/bench_phase" "$SRC/libvsdlss.a" "$SRC"/src/*.o
if ! eval make -C "\"$SRC\"" -j"$(nproc)" bench_phase $BUILD > "$OUT/build.log" 2>&1; then
    say "    BUILD FAILED"; tail -n 15 "$OUT/build.log" | sed 's/^/    | /' | tee -a "$S"; exit 1
fi
case "$BUILD" in *BLAS=1*) ;; *) say "    WARNING: built without BLAS/MKL (production uses MKL)";; esac
say "    ok"

echo; say "=== 2. core phases (bench_phase, order 6, threads $TH, median of $REPS, VSDLSS_SOLVE_PROFILE=1)"
t0=$(date +%s)
env VSDLSS_INTERLEAVE=1 VSDLSS_SOLVE_PROFILE=1 "$SRC/bench_phase" text:. 6 "$TH" "$REPS" > "$OUT/phase.log" 2> "$OUT/profile.log"
say "    ($(($(date +%s) - t0)) s)"
grep -aE "^# [^ ]+: n |^# core|^T=|^# results" "$OUT/phase.log" | sed 's/^/    /' | tee -a "$S"
# the core of the measured component only (the largest n), each thread count
N=$(grep -a "vsdlss solve profile" "$OUT/profile.log" | sed -E 's/.*n=([0-9]+).*/\1/' | sort -n | tail -n 1)
grep -a "vsdlss solve profile: n=$N " "$OUT/profile.log" | head -n 1 | sed -E 's/.*(top (in order|by branches)[^|]*\|.*)/    structure: \1/' | tee -a "$S"
grep -a "vsdlss solve profile: n=$N " "$OUT/profile.log" | awk '{
    gf = gb = 0
    for (i = 1; i <= NF; i++) {        # the first "fwd sub" / "bwd top" only ("top by branches: fwd bwd" follows)
        if ($i ~ /^nt_top=/) { split($i, kv, "="); k = kv[2] }
        if ($i == "fwd" && $(i+1) == "sub" && !gf) { fs = $(i+2); ft = $(i+4); gf = 1 }
        if ($i == "bwd" && $(i+1) == "top" && !gb) { bt = $(i+2); bs = $(i+4); gb = 1 }
    }
    n[k]++; FS_[k, n[k]] = fs; FT[k, n[k]] = ft; BT[k, n[k]] = bt; BS[k, n[k]] = bs }
  function med(a, k, c,   i, j, t, v) { for (i = 1; i <= c; i++) v[i] = a[k, i] + 0
      for (i = 2; i <= c; i++) { t = v[i]; for (j = i - 1; j >= 1 && v[j] > t; j--) v[j+1] = v[j]; v[j+1] = t } return v[int((c+1)/2)] }
  END { for (k in n) { p1 = med(FS_, k, n[k]); p2 = med(FT, k, n[k]); p3 = med(BT, k, n[k]); p4 = med(BS, k, n[k])
      printf "    threads %-3s fwd subtrees %6.2f ms | fwd top %6.2f ms | bwd top %6.2f ms | bwd subtrees %6.2f ms | total %6.2f ms  (%d solves)\n",
             k, 1e3*p1, 1e3*p2, 1e3*p3, 1e3*p4, 1e3*(p1+p2+p3+p4), n[k] } }' | sort -k2,2n | tee -a "$S"
say "    (1 thread takes the serial path: no profile line; subtrees: independent subtrees in parallel; top: the panels above them, by branches with MKL;"
say "     by branches the backward subtrees run inside bwd top)"

echo; say "Done. Please send: $S"
