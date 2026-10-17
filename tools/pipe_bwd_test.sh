#!/usr/bin/env bash
# A/B of the backward pipeline (VSDLSS_PIPE_BWD=1) in the transient solve.
# Builds transient_driver itself, then runs the comparison.
#
#   cd <data directory with diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt ...>
#   bash <source>/tools/pipe_bwd_test.sh <source directory> [threads=32] [steps=5] [rounds=3]
#
# Build: in <source directory>: make transient_driver $BUILD after removing
# the old objects (flags may differ).  BUILD defaults to "METIS=1", plus
# "BLAS=1 BLAS_LIBS=..." for MKL when libmkl_rt is found ($MKLROOT, ldconfig,
# oneAPI / conda places, LD_LIBRARY_PATH, /opt /usr /home), else OpenBLAS
# when installed; set BUILD to
# override, e.g. BUILD="METIS=1 BLAS=1 BLAS_LIBS='-L/opt/mkl/lib -lmkl_rt'".
# A path to an already built transient_driver instead of the directory skips
# the build.
#
# 1. Timing: `rounds` pairs of runs, pipeline off / on (order alternated per
#    round against drift), without the trace (it reads /proc on every line).
# 2. Bitwise: every run prints the FNV hash of each step's solution
#    (X_HASH=1); all off and on runs must give the same hashes.
# 3. Phases: one run each with VSDLSS_TRACE=1 (median of every "solve:"
#    line); with the pipeline, "core" + "reduce backward" become
#    "core + tail backward" + "reduce backward (blocks)".
# Threads are bound as in production (OMP_PROC_BIND=spread OMP_PLACES=cores)
# unless the caller sets OMP_PROC_BIND / OMP_PLACES: unbound threads on a
# two-socket machine move between NUMA nodes and make runs differ by +-50%.
# Output: pipe_bwd_test_<time>/  (build.log, one .log per run, summary.txt)
set -u
ARG=${1:?usage: bash pipe_bwd_test.sh <source directory | transient_driver> [threads] [steps] [rounds]}
T=${2:-32}; STEPS=${3:-5}; ROUNDS=${4:-3}
export OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores}
for f in diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
OUT=pipe_bwd_test_$(date +%m%d_%H%M); mkdir -p "$OUT"
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)"
  echo "input: $ARG  threads: $T  steps: $STEPS  rounds: $ROUNDS"
  echo "env: OMP_PROC_BIND=${OMP_PROC_BIND:-} OMP_PLACES=${OMP_PLACES:-} VSDLSS_INTERLEAVE=${VSDLSS_INTERLEAVE:-}"
} | tee "$OUT/summary.txt"

if [ -d "$ARG" ]; then
    SRC=$(cd "$ARG" && pwd)
    [ -f "$SRC/Makefile" ] && [ -f "$SRC/src/vsdlss_reduction.c" ] || { echo "not a source directory: $SRC"; exit 1; }
    grep -q vsdlss_reduce_core_bwd_pipelined "$SRC/src/vsdlss_reduction.c" ||
        { echo "the sources in $SRC have no backward pipeline: copy the overlay package first"; exit 1; }
    if [ -z "${BUILD+x}" ]; then
        BUILD="METIS=1"
        # libmkl_rt: $MKLROOT, the loader cache, the usual oneAPI / conda / pip
        # places, LD_LIBRARY_PATH, then a bounded search of /opt /usr /home
        MKLRT=""
        [ -n "${MKLROOT:-}" ] && MKLRT=$(find "$MKLROOT" -name 'libmkl_rt.so*' 2>/dev/null | head -n 1)
        [ -z "$MKLRT" ] && MKLRT=$(ldconfig -p 2>/dev/null | awk '/libmkl_rt\.so/ {print $NF; exit}')
        if [ -z "$MKLRT" ]; then
            for d in ${LD_LIBRARY_PATH//:/ } /opt/intel/oneapi/mkl/latest/lib /opt/intel/oneapi/mkl/latest/lib/intel64 \
                     /opt/intel/mkl/lib/intel64 ${CONDA_PREFIX:+$CONDA_PREFIX/lib} "$HOME"/.local/lib; do
                for f in "$d"/libmkl_rt.so*; do [ -e "$f" ] && { MKLRT=$f; break 2; }; done
            done
        fi
        [ -z "$MKLRT" ] && MKLRT=$(timeout 60 find /opt /usr /home -maxdepth 8 -name 'libmkl_rt.so*' 2>/dev/null | head -n 1)
        if [ -n "$MKLRT" ]; then D=$(dirname "$MKLRT"); BUILD="METIS=1 BLAS=1 BLAS_LIBS='-L$D -lmkl_rt -Wl,-rpath,$D'"
        elif ldconfig -p 2>/dev/null | grep -q libopenblas; then BUILD="METIS=1 BLAS=1 BLAS_LIBS=-lopenblas"; fi
    fi
    echo "build: make -C $SRC transient_driver $BUILD   (log: $OUT/build.log)" | tee -a "$OUT/summary.txt"
    rm -f "$SRC/libvsdlss.a" "$SRC"/src/*.o "$SRC/transient_driver"
    if ! eval make -C "\"$SRC\"" -j"$(nproc)" transient_driver $BUILD > "$OUT/build.log" 2>&1; then
        { echo "    BUILD FAILED"; tail -n 15 "$OUT/build.log" | sed 's/^/    | /'; } | tee -a "$OUT/summary.txt"; exit 1
    fi
    DRV=$SRC/transient_driver
    case "$BUILD" in *BLAS=1*) ;; *) echo "    WARNING: built without BLAS/MKL (production uses MKL): set BUILD=\"METIS=1 BLAS=1 BLAS_LIBS='-L<mkl>/lib -lmkl_rt -Wl,-rpath,<mkl>/lib'\" or MKLROOT" | tee -a "$OUT/summary.txt";; esac
    grep -E "^(cc|gcc|g\+\+|c\+\+).*transient_driver" "$OUT/build.log" | tail -n 1 | cut -c1-300 | sed 's/^/    /' | tee -a "$OUT/summary.txt"
else
    DRV=$ARG
fi
[ -x "$DRV" ] || { echo "not executable: $DRV"; exit 1; }
grep -aq X_HASH "$DRV" || echo "    note: $DRV has no X_HASH (old driver): the bitwise check will fail" | tee -a "$OUT/summary.txt"

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
min_of() {   # best warm solve over all runs
    for f in "$OUT"/$1_[0-9]*.log; do grep -a "warm solves" "$f" | sed -E 's/.*min ([0-9.]+) ms.*/\1/'; done |
        sort -g | head -n 1
}
OFF=$(med_of off); ON=$(med_of on)
awk -v a="$OFF" -v b="$ON" 'BEGIN {printf "    warm median over rounds: off %.2f ms, on %.2f ms, change %+.2f ms (%+.1f%%)\n", a, b, b-a, (a>0?100*(b-a)/a:0)}' |
    tee -a "$OUT/summary.txt"
awk -v a="$(min_of off)" -v b="$(min_of on)" 'BEGIN {printf "    best warm solve:         off %.2f ms, on %.2f ms, change %+.2f ms (%+.1f%%)\n", a, b, b-a, (a>0?100*(b-a)/a:0)}' |
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
