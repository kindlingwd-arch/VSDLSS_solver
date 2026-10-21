#!/usr/bin/env bash
# Load balance of the tail replay (window schedule), and the core's thread scaling:
#   1. build bench_phase (METIS, MKL as tools/ab_test.sh)
#   2. bench_phase on the largest component, chunks in index order
#      (VSDLSS_TAIL_LPT=0) and largest first (=1), VSDLSS_TAIL_STATS=1:
#      per tail replay the wall time, the threads' time inside chunks, the
#      utilization busy / (threads x wall), the busiest thread and the longest
#      chunk -- medians per direction and thread count
#   3. the core alone at 1 2 4 8 16 32 threads (how much of it runs in parallel)
#
#   cd <data directory with diag.txt data.txt>
#   bash <source>/tools/tail_balance_test.sh <source directory> ["threads" = "16 32"] [reps = 15]
#
# Results are bitwise the same with and without VSDLSS_TAIL_LPT (bench_phase
# compares across thread counts; tests/test_m3 across the switch).  For the
# per-step effect in production run tools/ab_test.sh with "VSDLSS_TAIL_LPT=1".
# Output: tail_balance_<time>/  (logs, summary.txt = the result to send back)
set -u
SRC=$(cd "${1:?usage: bash tail_balance_test.sh <source directory> [threads] [reps]}" && pwd)
TH=${2:-16 32}; REPS=${3:-15}
export OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores} MKL_THREADING_LAYER=SEQUENTIAL
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
OUT=tail_balance_$(date +%m%d_%H%M); mkdir -p "$OUT" || exit 1
S=$OUT/summary.txt
say() { echo "$*" | tee -a "$S"; }
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)"
  echo "source: $SRC ($(git -C "$SRC" log -1 --format='%h %s' 2>/dev/null || echo 'no git'))  threads: $TH  reps: $REPS  load: $(cut -d' ' -f1-3 /proc/loadavg)"
} | tee "$S"
grep -q VSDLSS_TAIL_LPT "$SRC/src/vsdlss_reduction.c" || { say "this source has no VSDLSS_TAIL_LPT: update src/vsdlss_reduction.c, src/vsdlss_m3_internal.h"; exit 1; }

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
say "    ok"

summ() {   # summ <stats file>: medians per direction and thread count
    awk '/^vsdlss tail stats:/ {
            k=$4" "$6; n[k]++; w[k,n[k]]=$10; b[k,n[k]]=$13; u[k,n[k]]=$16; m[k,n[k]]=$18; l[k,n[k]]=$22 }
         function med(a,k,c,  i,j,t,v) { for(i=1;i<=c;i++) v[i]=a[k,i]+0;
            for(i=2;i<=c;i++) { t=v[i]; for(j=i-1;j>=1 && v[j]>t;j--) v[j+1]=v[j]; v[j+1]=t } return v[int((c+1)/2)] }
         END { for(k in n) { split(k,p," ");
            printf "    %-8s threads %-3s wall %7.2f ms | busy %8.2f ms | utilization %5.1f%% | busiest thread %7.2f ms | longest chunk %6.2f ms  (%d replays)\n",
                   p[1], p[2], med(w,k,n[k]), med(b,k,n[k]), med(u,k,n[k]), med(m,k,n[k]), med(l,k,n[k]), n[k] } }' "$1" | sort
}

for lpt in 0 1; do
    echo; say "=== 2.$((lpt+1)). tail replay, chunks $([ $lpt = 1 ] && echo 'largest first' || echo 'in index order') (VSDLSS_TAIL_LPT=$lpt), threads $TH"
    t0=$(date +%s)
    env VSDLSS_INTERLEAVE=1 VSDLSS_TAIL_STATS=1 VSDLSS_TAIL_LPT=$lpt "$SRC/bench_phase" text:. 6 "$TH" "$REPS" > "$OUT/phase_lpt$lpt.log" 2> "$OUT/stats_lpt$lpt.log"
    say "    ($(($(date +%s) - t0)) s)"
    grep -aE "^T=|^# results" "$OUT/phase_lpt$lpt.log" | sed 's/^/    /' | tee -a "$S"
    if grep -q "^vsdlss tail stats:" "$OUT/stats_lpt$lpt.log"; then summ "$OUT/stats_lpt$lpt.log" | tee -a "$S"
    else say "    no tail stats: this case's tail is not window-scheduled (too small), nothing to balance"; fi
done
say "    (wall: one tail replay; busy: the threads' time inside chunks, summed; utilization = busy / (threads x wall);"
say "     the longest chunk bounds the wall time from below)"

echo; say "=== 3. the core alone at 1 2 4 8 16 32 threads (bench_phase, chunks in index order)"
env VSDLSS_INTERLEAVE=1 "$SRC/bench_phase" text:. 6 "1 2 4 8 16 32" "$REPS" > "$OUT/core_scaling.log" 2>&1
grep -aE "^T=" "$OUT/core_scaling.log" | sed -E 's/.*(T=[0-9]+).*(core fwd\+bwd.*)/    \1  \2/' | tee -a "$S"
say "    (a core that barely gets faster from 1 to 32 threads runs mostly on one thread: the tree top)"

echo; say "Done. Please send: $S"
