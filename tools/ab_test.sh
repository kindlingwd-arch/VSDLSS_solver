#!/usr/bin/env bash
# A/B(/C/...) of solver settings (environment variables) in the transient solve.
# Builds transient_driver itself (as tools/pipe_bwd_test.sh), then for every
# configuration: timing runs (configurations alternated round by round, no
# trace), the solution hash of every step (X_HASH=1), and one traced run
# (median of every "solve:" line).
#
#   cd <data directory with diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt ...>
#   bash <source>/tools/ab_test.sh <source directory | transient_driver> [threads=32] [steps=5] [rounds=3] ["ENV=v ..." ...]
#
# A configuration is a string of VAR=value words ("" = defaults).  Without
# configurations: the core tree top (MKL build):
#   c1  ""                                          BLAS panel solves, top by branches (default)
#   c2  "VSDLSS_TOP_BRANCHES=0"                     BLAS, top in order (one panel at a time)
#   c3  "VSDLSS_BLAS_SOLVE_MIN=0"                   built-in kernels, top in order, a team per large panel
#   c4  "VSDLSS_BLAS_SOLVE_MIN=0 VSDLSS_TOP_BRANCHES=3"   built-in kernels, top by branches
# The top schedule never changes the bits; BLAS vs built-in panel solves do
# (both are exact to rounding), so c1/c2 and c3/c4 are compared separately.
# BUILD, thread binding: as in pipe_bwd_test.sh.
# Output: ab_test_<time>/  (build.log, one .log per run, summary.txt)
set -u
ARG=${1:?usage: bash ab_test.sh <source directory | transient_driver> [threads] [steps] [rounds] [configurations...]}
T=${2:-32}; STEPS=${3:-5}; ROUNDS=${4:-3}
shift; [ $# -gt 0 ] && shift; [ $# -gt 0 ] && shift; [ $# -gt 0 ] && shift
if [ $# -gt 0 ]; then CFG=("$@")
else CFG=("" "VSDLSS_TOP_BRANCHES=0" "VSDLSS_BLAS_SOLVE_MIN=0" "VSDLSS_BLAS_SOLVE_MIN=0 VSDLSS_TOP_BRANCHES=3"); fi
N=${#CFG[@]}
export OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores}
for f in diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
OUT=ab_test_$(date +%m%d_%H%M); mkdir -p "$OUT"
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)"
  echo "input: $ARG  threads: $T  steps: $STEPS  rounds: $ROUNDS"
  echo "env: OMP_PROC_BIND=${OMP_PROC_BIND:-} OMP_PLACES=${OMP_PLACES:-} VSDLSS_INTERLEAVE=${VSDLSS_INTERLEAVE:-}"
  for i in $(seq 1 $N); do echo "  c$i: ${CFG[$((i-1))]:-(defaults)}"; done
} | tee "$OUT/summary.txt"

if [ -d "$ARG" ]; then
    SRC=$(cd "$ARG" && pwd)
    [ -f "$SRC/Makefile" ] && [ -f "$SRC/src/vsdlss_reduction.c" ] || { echo "not a source directory: $SRC"; exit 1; }
    if [ -z "${BUILD+x}" ]; then
        BUILD="METIS=1"
        # libmkl_rt: $MKLROOT, the loader cache, the usual oneAPI / conda / pip
        # places, LD_LIBRARY_PATH, then a bounded search of /opt /usr /home
        MKLRT=""
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

run() {   # run <name> <configuration index> <trace 0/1> <steps>
    local name=$1 i=$2 trace=$3 steps=$4
    env MKL_THREADING_LAYER=SEQUENTIAL VSDLSS_THREADS=$T STEPS=$steps CHECK_RESIDUAL=0 X_HASH=1 \
        VSDLSS_TRACE=$trace ${CFG[$((i-1))]} "$DRV" diag.txt data.txt > "$OUT/$name.log" 2>&1
    local rc=$?
    local med; med=$(grep -a "warm solves" "$OUT/$name.log" | sed -E 's/.*median ([0-9.]+) ms, min ([0-9.]+) ms.*/median \1 ms  min \2 ms/')
    printf "    %-10s %s%s\n" "$name" "${med:-no warm solves}" "$([ $rc -ne 0 ] && echo "  EXIT CODE $rc")" | tee -a "$OUT/summary.txt"
    [ $rc -ne 0 ] && tail -n 3 "$OUT/$name.log" | sed 's/^/      | /' | tee -a "$OUT/summary.txt"
}

echo; echo "=== 1. timing (no trace; order rotated each round)" | tee -a "$OUT/summary.txt"
for r in $(seq 1 "$ROUNDS"); do
    for k in $(seq 0 $((N-1))); do i=$(( (k + r - 1) % N + 1 )); run c${i}_$r $i 0 "$STEPS"; done
done
med_of() { for f in "$OUT"/$1_[0-9]*.log; do grep -a "warm solves" "$f" | sed -E 's/.*median ([0-9.]+) ms.*/\1/'; done |
    sort -g | awk '{a[NR]=$1} END {if(NR) print a[int((NR+1)/2)]; else print "nan"}'; }
min_of() { for f in "$OUT"/$1_[0-9]*.log; do grep -a "warm solves" "$f" | sed -E 's/.*min ([0-9.]+) ms.*/\1/'; done | sort -g | head -n 1; }
echo "    summary (median of the runs' warm medians | best warm solve):" | tee -a "$OUT/summary.txt"
BASE=$(med_of c1)
for i in $(seq 1 $N); do
    awk -v n="c$i" -v m="$(med_of c$i)" -v b="$(min_of c$i)" -v a="$BASE" -v d="${CFG[$((i-1))]:-(defaults)}" \
        'BEGIN {printf "      %-4s %8.2f ms (%+6.1f%% vs c1) | %8.2f ms   %s\n", n, m, (a>0?100*(m-a)/a:0), b, d}' | tee -a "$OUT/summary.txt"
done

echo; echo "=== 2. bitwise (x_hash of the first solve and every step)" | tee -a "$OUT/summary.txt"
declare -a H
for i in $(seq 1 $N); do
    ref=""; same=1
    for f in "$OUT"/c${i}_[0-9]*.log; do
        h=$(grep -ao "x_hash=[0-9a-f]*" "$f" | tr '\n' ' ')
        [ -z "$h" ] && { same=0; continue; }
        if [ -z "$ref" ]; then ref=$h; elif [ "$h" != "$ref" ]; then same=0; fi
    done
    H[$i]=$ref
    echo "    c$i: $([ $same -eq 1 ] && echo "same in all its runs" || echo "RUNS DIFFER")  ${ref%% *}" | tee -a "$OUT/summary.txt"
done
for i in $(seq 2 $N); do
    [ "${H[$i]}" = "${H[1]}" ] && s="identical to c1" || s="differs from c1"
    echo "    c$i $s" | tee -a "$OUT/summary.txt"
done

echo; echo "=== 3. phases (VSDLSS_TRACE=1, $STEPS steps, median ms per line, all components)" | tee -a "$OUT/summary.txt"
for i in $(seq 1 $N); do run trace_c$i $i 1 "$STEPS"; done
phases() {
    grep -a "vsdlss trace: solve" "$1" | sed -E 's/^vsdlss trace: (.*[^ ]) +([0-9.]+) s  rss.*/\1|\2/' |
        awk -F'|' '{v[$1]=v[$1] " " $2*1000; if(!($1 in o)) o[$1]=++n} END {for(k in o) print o[k] "|" k "|" v[k]}' | sort -n |
        while IFS='|' read -r _ label vals; do
            m=$(echo $vals | tr ' ' '\n' | sort -g | awk '{a[NR]=$1} END {print a[int((NR+1)/2)]}')
            printf "      %-34s %8.1f  (%d lines)\n" "$label" "$m" "$(echo $vals | wc -w)"
        done
}
for i in $(seq 1 $N); do echo "    c$i: ${CFG[$((i-1))]:-(defaults)}"; phases "$OUT/trace_c$i.log"; done | tee -a "$OUT/summary.txt"
echo; echo "Done. Please send: $OUT/summary.txt"
