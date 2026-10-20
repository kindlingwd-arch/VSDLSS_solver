#!/usr/bin/env bash
# Where the solve time goes, with hardware evidence:
#   1. build bench_stream and bench_phase (METIS, MKL as tools/ab_test.sh)
#   2. machine bandwidth: bench_stream, 16 / 32 / 64 / 128 threads
#   3. solve phases of the largest component: bench_phase (replay fwd / bwd,
#      blocks, tail, core fwd+bwd, each with the GB/s it streams), 16 / 32 / 64
#      threads -- compare those GB/s with step 2
#   4. perf (when usable): bench_phase at the production thread count, perf
#      attached after the factorization: perf stat (IPC, cache and memory
#      events this CPU supports) and perf record (functions by share of
#      cycles; libgomp spin / barrier functions = threads waiting)
#
#   cd <data directory with diag.txt data.txt>
#   bash <source>/tools/perf_profile.sh <source directory> [threads=32] [seconds=20]
#
# Optional environment: STREAM_MB (MiB per array, default 1024), PHASE_THREADS
# (default "16 32 64"), PHASE_REPS (default 15), BUILD (make variables, as
# tools/ab_test.sh), SKIP_STREAM=1, SKIP_PHASE=1, SKIP_PERF=1.
# Threads bound as in production (OMP_PROC_BIND=spread, OMP_PLACES=cores),
# memory interleaved (VSDLSS_INTERLEAVE=1: the benchmark calls set_mempolicy
# itself).  bench_phase measures one component with all the threads, the
# production solve runs VDD and GND at once with half each.
# Output: perf_profile_<time>/  (logs, perf.data, summary.txt = the result to send back)
set -u
SRC=$(cd "${1:?usage: bash perf_profile.sh <source directory> [threads] [seconds]}" && pwd)
T=${2:-32}; SECS=${3:-20}
export OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores} MKL_THREADING_LAYER=SEQUENTIAL
for f in diag.txt data.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
[ -f "$SRC/Makefile" ] && [ -f "$SRC/test/bench_phase.c" ] || { echo "not a source directory: $SRC"; exit 1; }
OUT=perf_profile_$(date +%m%d_%H%M); mkdir -p "$OUT" || exit 1
S=$OUT/summary.txt
say() { echo "$*" | tee -a "$S"; }
{
  echo "### $(date)  $(hostname)  $(uname -r)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)|L2 cache|L3 cache|CPU max MHz"
  echo "source: $SRC ($(git -C "$SRC" log -1 --format='%h %s' 2>/dev/null || echo 'no git'))  threads: $T  perf seconds: $SECS"
  echo "env: OMP_PROC_BIND=$OMP_PROC_BIND OMP_PLACES=$OMP_PLACES  load: $(cut -d' ' -f1-3 /proc/loadavg)"
} | tee "$S"

# ---- 1. build ----
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
echo; say "=== 1. build: make -C $SRC bench_phase $BUILD; ${CC:-gcc} bench_stream"
rm -f "$SRC/bench_phase" "$SRC/bench_stream"
# bench_stream is built directly (no Makefile target needed on older trees)
if ! { eval make -C "\"$SRC\"" -j"$(nproc)" bench_phase $BUILD &&
       ${CC:-gcc} -O2 -fopenmp -o "$SRC/bench_stream" "$SRC/test/bench_stream.c" -lm; } > "$OUT/build.log" 2>&1; then
    say "    BUILD FAILED"; tail -n 15 "$OUT/build.log" | sed 's/^/    | /' | tee -a "$S"; exit 1
fi
case "$BUILD" in *BLAS=1*) ;; *) say "    WARNING: built without BLAS/MKL (production uses MKL)";; esac
say "    ok"
PH=$SRC/bench_phase

# ---- 2. machine bandwidth ----
echo; say "=== 2. machine memory bandwidth (bench_stream, ${STREAM_MB:-1024} MiB per array)"
if [ "${SKIP_STREAM:-0}" = 1 ]; then say "    skipped"
else
    "$SRC/bench_stream" "16 32 64 128" "${STREAM_MB:-1024}" 10 > "$OUT/stream.log" 2>&1
    sed 's/^/    /' "$OUT/stream.log" | tee -a "$S"
fi

# ---- 3. solve phases, thread sweep ----
echo; say "=== 3. solve phases of the largest component (bench_phase, order 6, threads ${PHASE_THREADS:-16 32 64}, median of ${PHASE_REPS:-15})"
if [ "${SKIP_PHASE:-0}" = 1 ]; then say "    skipped"
else
    t0=$(date +%s)
    env VSDLSS_INTERLEAVE=1 "$PH" text:. 6 "${PHASE_THREADS:-16 32 64}" "${PHASE_REPS:-15}" > "$OUT/phase.log" 2>&1
    say "    ($(($(date +%s) - t0)) s, log phase.log)"
    grep -aE "^# (numa|text|\.|tail|component)|^T=|^# results|^# [^ ]+: n " "$OUT/phase.log" | sed 's/^/    /' | tee -a "$S"
    say "    (GB/s = bytes of the records (replay) or of L (core) streamed per second: compare with step 2)"
fi

# ---- 4. perf ----
echo; say "=== 4. perf: bench_phase at $T threads, attached after the factorization ($SECS s stat, $SECS s record)"
PERF=$(command -v perf 2>/dev/null)
PAR=$(cat /proc/sys/kernel/perf_event_paranoid 2>/dev/null || echo '?')
if [ "${SKIP_PERF:-0}" = 1 ]; then say "    skipped"
elif [ -z "$PERF" ]; then say "    perf not found (install linux-tools / perf, or ask an administrator)"
elif ! "$PERF" stat -e cycles -- true > /dev/null 2>&1; then
    say "    perf cannot count here (perf_event_paranoid = $PAR; it must be <= 1 for a user, or run as root)"
else
    say "    perf: $("$PERF" --version 2>&1)  perf_event_paranoid = $PAR"
    EV=""
    for e in cycles instructions cache-references cache-misses L1-dcache-loads L1-dcache-load-misses \
             LLC-loads LLC-load-misses dTLB-load-misses stalled-cycles-frontend stalled-cycles-backend \
             l2_cache_req_stat.ls_rd_blk_c ls_refills_from_sys.ls_mabresp_lcl_cache \
             ls_refills_from_sys.ls_mabresp_lcl_dram ls_refills_from_sys.ls_mabresp_rmt_dram \
             ls_refills_from_sys.ls_mabresp_rmt_cache; do
        "$PERF" stat -e "$e" -- true > /dev/null 2>&1 && EV="${EV:+$EV,}$e"
    done
    say "    events: $EV"
    # enough repetitions to outlast both attachments (one thread count repeated)
    reps=""; for i in $(seq 1 16); do reps="$reps $T"; done
    env VSDLSS_INTERLEAVE=1 "$PH" text:. 6 "$reps" 64 > "$OUT/perf_phase.log" 2>&1 &
    PID=$!
    for i in $(seq 1 3600); do grep -aq "^# tail" "$OUT/perf_phase.log" 2>/dev/null && break; kill -0 $PID 2>/dev/null || break; sleep 1; done
    if ! kill -0 $PID 2>/dev/null; then say "    bench_phase ended before the solves:"; tail -n 5 "$OUT/perf_phase.log" | sed 's/^/    | /' | tee -a "$S"
    else
        sleep 2
        "$PERF" stat -p $PID -e "$EV" -- sleep "$SECS" > "$OUT/perf_stat.txt" 2>&1
        "$PERF" record -F 499 -g -p $PID -o "$OUT/perf.data" -- sleep "$SECS" > "$OUT/perf_record.log" 2>&1
        kill $PID 2>/dev/null; wait $PID 2>/dev/null
        say "    perf stat (all threads, $SECS s of solves):"
        grep -E "^ +[0-9,.]+ +[A-Za-z_]|insn per cycle|seconds time elapsed" "$OUT/perf_stat.txt" | sed 's/^/    /' | tee -a "$S"
        # --no-children only exists from perf 3.16 (CentOS 7 ships 3.10): fall back without it
        for opt in "--no-children" ""; do
            "$PERF" report -i "$OUT/perf.data" --stdio $opt -g none --sort symbol 2>/dev/null \
                | grep -E "^ +[0-9]+\.[0-9]+%" | head -n 40 > "$OUT/perf_top.txt"
            [ -s "$OUT/perf_top.txt" ] && break
        done
        for opt in "--no-children" ""; do
            "$PERF" report -i "$OUT/perf.data" --stdio $opt -g none --sort dso 2>/dev/null \
                | grep -E "^ +[0-9]+\.[0-9]+%" | head -n 10 > "$OUT/perf_dso.txt"
            [ -s "$OUT/perf_dso.txt" ] && break
        done
        [ -s "$OUT/perf_top.txt" ] || "$PERF" report -i "$OUT/perf.data" --stdio -g none --sort symbol 2>&1 | head -n 5 | sed 's/^/    | /' | tee -a "$S"
        say "    functions by share of cycles (no children; libgomp / gomp_* / do_spin = waiting at barriers):"
        sed 's/^/    /' "$OUT/perf_top.txt" | tee -a "$S"
        say "    by library:"
        sed 's/^/    /' "$OUT/perf_dso.txt" | tee -a "$S"
    fi
fi

echo; say "Done. Please send: $S"
