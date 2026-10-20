#!/usr/bin/env bash
# The transient interface (vsdlss_m3_transient) on a simulator dump, in one go:
#   1. build bench_tr_replay, gen_tr_dump, test_transient (METIS, MKL as tools/ab_test.sh)
#   2. unit test (test_transient: bitwise against the packed solve)
#   3. synthetic dump (gen_tr_dump) replayed: the whole chain on known data
#   4. the real dump replayed: Ch and sources reconstructed and saved (TR_SAVE),
#      every step checked against <k>x_vector.txt, orig / manual / tr / tr+fuse
#      timed (fusion automatic, traced); then once more with forced fusion
#      (PG_FUSE=2), timed too: the bits and the speed of the fused path.  When the cross-check says
#      the diagonal excess is not reliable (few digits in the text), step 4 is
#      repeated with TR_CH=ratio.
#   6. fusion upper bound (PG_FUSE=3, timing only): every unit fused right
#      away, the late rules ignored (results not exact), against tr unfused;
#      the trace lines "transient fusion records / potential / unit footprint"
#      give the late records per reason and the unit sizes.
#
#   cd <dump directory: diag.txt data.txt x_vector.txt 1b_vector.txt 1x_vector.txt 2b_vector.txt ...>
#   bash <source>/tools/tr_replay_test.sh <source directory> [threads=32] [steps=10] [rounds=3]
#   (csh: the same line; variables before it as "env DT=1e-12 bash ...")
#
# Optional environment: DT (time step in seconds: capacitance statistics),
# TR_CH / TR_RTOL / TR_TOL (see test/bench_tr_replay.c), SAVE_FMT (bin
# (default) | txt: the saved sources), SKIP_UNIT=1, SKIP_SYNTH=1, SKIP_FUSE2=1, SKIP_BOUND=1, SKIP_SAVE=1 (do not save the
# reconstructed inputs in step 4), TR_PACKED=0
# (leave out the packed-source way: it holds steps x n doubles), BUILD (make
# variables, as tools/ab_test.sh).  Threads bound (OMP_PROC_BIND=spread,
# OMP_PLACES=cores), memory as the NUMA runs chose: numactl --interleave=all
# (when installed, else VSDLSS_INTERLEAVE=1: bench_tr_replay calls set_mempolicy
# itself) and VSDLSS_NUMA_PLACE=1.
# Output: tr_replay_<time>/  (build.log, one .log per run, inputs/ = the saved
# Ch, x0, sources; summary.txt = the result to send back)
set -u
ARG=${1:?usage: bash tr_replay_test.sh <source directory> [threads] [steps] [rounds]}
T=${2:-32}; STEPS=${3:-10}; ROUNDS=${4:-3}
export OMP_PROC_BIND=${OMP_PROC_BIND:-spread} OMP_PLACES=${OMP_PLACES:-cores} VSDLSS_NUMA_PLACE=${VSDLSS_NUMA_PLACE:-1}
export MKL_THREADING_LAYER=SEQUENTIAL TR_STEPS=$STEPS
for f in diag.txt data.txt x_vector.txt 1b_vector.txt 1x_vector.txt; do [ -f "$f" ] || { echo "missing $f in $(pwd)"; exit 1; }; done
DUMP=$(pwd)
OUT=$DUMP/tr_replay_$(date +%m%d_%H%M)
mkdir -p "$OUT" 2>/dev/null || { OUT=$HOME/tr_replay_$(date +%m%d_%H%M); mkdir -p "$OUT"; }
S=$OUT/summary.txt
say() { echo "$@" | tee -a "$S"; }
NUMA=""; if command -v numactl >/dev/null 2>&1; then NUMA="numactl --interleave=all"; else export VSDLSS_INTERLEAVE=1; fi
[ -w "$DUMP" ] || export TR_NOCACHE=1
nsteps=0; while [ $nsteps -lt "$STEPS" ] && [ -f "$((nsteps + 1))b_vector.txt" ] && [ -f "$((nsteps + 1))x_vector.txt" ]; do nsteps=$((nsteps + 1)); done
{
  echo "### $(date)  $(hostname)"
  lscpu | grep -E "Model name|^CPU\(s\)|Socket|NUMA node\(s\)"
  echo "source: $ARG  dump: $DUMP  threads: $T  steps: $nsteps of $STEPS requested  rounds: $ROUNDS"
  echo "env: OMP_PROC_BIND=$OMP_PROC_BIND OMP_PLACES=$OMP_PLACES VSDLSS_NUMA_PLACE=$VSDLSS_NUMA_PLACE memory: ${NUMA:-VSDLSS_INTERLEAVE=1 (numactl not found: bench_tr_replay interleaves itself)}"
  echo "     DT=${DT:-} TR_CH=${TR_CH:-} TR_RTOL=${TR_RTOL:-} TR_TOL=${TR_TOL:-} SAVE_FMT=${SAVE_FMT:-bin}${TR_NOCACHE:+ TR_NOCACHE=1 (dump directory not writable)}"
  echo "disk: $(df -h "$DUMP" | awk 'NR==2 {print $4 " free in " $6}')  (the binary cache .trcache needs about (2*steps+1) x n x 8 bytes plus the matrix)"
} | tee "$S"

# ---- 1. build ----
SRC=$(cd "$ARG" && pwd)
[ -f "$SRC/Makefile" ] && [ -f "$SRC/src/vsdlss_m3_transient.c" ] && [ -f "$SRC/test/bench_tr_replay.c" ] ||
    { say "not a source directory with the transient interface (branch feat/transient-stepping): $SRC"; exit 1; }
if [ -z "${BUILD+x}" ]; then
    BUILD="METIS=1"
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
echo; say "=== 1. build: make -C $SRC bench_tr_replay gen_tr_dump test_transient $BUILD"
rm -f "$SRC/bench_tr_replay" "$SRC/gen_tr_dump" "$SRC/test_transient"
if ! eval make -C "\"$SRC\"" -j"$(nproc)" bench_tr_replay gen_tr_dump test_transient $BUILD > "$OUT/build.log" 2>&1; then
    { echo "    BUILD FAILED"; tail -n 15 "$OUT/build.log" | sed 's/^/    | /'; } | tee -a "$S"; exit 1
fi
say "    ok (git: $(git -C "$SRC" log --oneline -1 2>/dev/null || echo 'not a git checkout'))"
case "$BUILD" in *BLAS=1*) ;; *) say "    WARNING: built without BLAS/MKL (production uses MKL)";; esac
R=$SRC/bench_tr_replay

# ---- 2. unit test ----
echo; say "=== 2. unit test (test_transient)"
if [ "${SKIP_UNIT:-0}" = 1 ]; then say "    skipped"
else
    (cd "$SRC" && ./test_transient) > "$OUT/unit.log" 2>&1; rc=$?
    if [ $rc -eq 0 ]; then say "    PASS  ($(grep -ac . "$OUT/unit.log") lines in unit.log)"
    else say "    FAIL (exit code $rc)"; tail -n 10 "$OUT/unit.log" | sed 's/^/    | /' | tee -a "$S"; fi
fi

# extract the result lines of a replay log: check <log>
check() {
    local log=$1
    grep -aE "^# (numa|Ch from|cross-check|warning|capacitance|step 1:|sources|saved|factorize)|fused components|transient fusion" "$log" | awk '!seen[$0]++' | sed 's/^/    /' | tee -a "$S"
    sed -n '/^validation/,/^timing/p' "$log" | grep -av '^timing' | sed 's/^/    /' | tee -a "$S"
    sed -n '/^timing/,$p' "$log" | grep -aE "^timing|^  (orig|manual|tr|tr\+fuse|packed-s) " | sed 's/^/    /' | tee -a "$S"
}
verdict() {   # verdict <log> <label>: tr, tr+fuse (and packed-s when present) bitwise = manual
    local log=$1 label=$2 nb nd want=2
    grep -aqE "^  packed-s " "$log" && want=3
    nb=$(grep -ac "bitwise = manual" "$log"); nd=$(grep -ac "DIFFERS from manual" "$log")
    if grep -aq "^validation" "$log" && [ "$nb" -eq "$want" ] && [ "$nd" -eq 0 ]; then say "    $label: PASS ($nb ways bitwise = manual)"
    else say "    $label: FAIL ($nb bitwise, $nd differ$(grep -aq '^validation' "$log" || echo ', run did not finish'))"; tail -n 3 "$log" | sed 's/^/    | /' | tee -a "$S"; fi
}
fused_note() {   # fused_note <log>: did tr+fuse take the fused path at all
    grep -aqE "tr\+fuse +fused components [1-9]" "$1" ||
        say "    note: tr+fuse fused no component (no scheduled tail, or every unit late): it ran the unfused path, so this checks nothing extra"
}
speedup() {   # speedup <log>
    sed -n '/^timing/,$p' "$1" | awk '/^  (orig|manual|tr|tr\+fuse|packed-s) / {t[$1]=$2}
        END {if (t["orig"] > 0) { printf "    per step: orig %.1f ms, manual %.1f, tr %.1f (%+.1f%% vs orig), tr+fuse %.1f (%+.1f%% vs orig, %+.1f%% vs tr)\n",
             t["orig"], t["manual"], t["tr"], 100*(t["tr"]-t["orig"])/t["orig"], t["tr+fuse"], 100*(t["tr+fuse"]-t["orig"])/t["orig"], (t["tr"]>0?100*(t["tr+fuse"]-t["tr"])/t["tr"]:0)
             if ("packed-s" in t) printf "    sources in packed order (packed-s): %.1f ms per step (%+.1f%% vs orig)\n", t["packed-s"], 100*(t["packed-s"]-t["orig"])/t["orig"] }}' | tee -a "$S"
    say "    (orig is handed the dumped b and does not build it; manual / tr / tr+fuse / packed-s also build b = Ch x + s every step;"
    say "     packed-s gets its sources already in packed order, as if the simulator stamped them into b[slot[node]])"
}

# ---- 3. synthetic dump ----
echo; say "=== 3. synthetic dump (gen_tr_dump: 278800 nodes, 8 steps, known C/dt)"
if [ "${SKIP_SYNTH:-0}" = 1 ]; then say "    skipped"
else
    "$SRC/gen_tr_dump" "$OUT/synth" > "$OUT/synth_gen.log" 2>&1 || { say "    gen_tr_dump FAILED"; tail -n 3 "$OUT/synth_gen.log" | sed 's/^/    | /' | tee -a "$S"; }
    # small case: smaller reduction blocks and a scheduled tail, fusion forced, so the fused path really runs
    env -u TR_CH -u TR_RTOL -u TR_TOL -u DT TR_STEPS=8 TR_NOCACHE=1 PG_ROUNDS=1 PG_BLOCK=4096 PG_TAILMIN=16 PG_FUSE=2 \
        TR_CHECK_CDIAG="$OUT/synth/cdiag_true.txt" $NUMA "$R" "$OUT/synth" 6 "$T" > "$OUT/synth.log" 2>&1
    grep -aE "^# (Ch against|cross-check)|tr\+fuse +fused components" "$OUT/synth.log" | sed 's/^/    /' | tee -a "$S"
    sed -n '/^validation/,/^timing/p' "$OUT/synth.log" | grep -av '^timing' | sed 's/^/    /' | tee -a "$S"
    verdict "$OUT/synth.log" "synthetic"; fused_note "$OUT/synth.log"
    rm -rf "$OUT/synth"
fi

# ---- 4. the real dump ----
echo; say "=== 4. real dump: $nsteps steps, $ROUNDS timing rounds, fusion automatic, traced; inputs saved to $OUT/inputs"
t0=$(date +%s)
SAVE_ARGS=(TR_SAVE="$OUT/inputs" TR_SAVE_FMT="${SAVE_FMT:-bin}")
[ "${SKIP_SAVE:-0}" = 1 ] && SAVE_ARGS=()
env ${SAVE_ARGS[@]+"${SAVE_ARGS[@]}"} PG_ROUNDS=$ROUNDS PG_FUSE=1 VSDLSS_TRACE=1 \
    $NUMA "$R" "$DUMP" 6 "$T" > "$OUT/real.log" 2>&1
say "    ($(($(date +%s) - t0)) s, log real.log)"
check "$OUT/real.log"; verdict "$OUT/real.log" "real dump"; speedup "$OUT/real.log"

if [ -z "${TR_CH:-}" ] && grep -aqE "excess is not reliable|warning: no node has a step-independent" "$OUT/real.log"; then
    echo; say "=== 4b. the excess is not reliable: again with TR_CH=ratio TR_RTOL=${TR_RTOL:-1e-5} (inputs saved to $OUT/inputs_ratio)"
    env TR_CH=ratio TR_RTOL="${TR_RTOL:-1e-5}" TR_SAVE="$OUT/inputs_ratio" TR_SAVE_FMT="${SAVE_FMT:-bin}" PG_ROUNDS=1 PG_FUSE=1 \
        $NUMA "$R" "$DUMP" 6 "$T" > "$OUT/real_ratio.log" 2>&1
    check "$OUT/real_ratio.log"; verdict "$OUT/real_ratio.log" "real dump, TR_CH=ratio"
fi

echo; say "=== 5. real dump, forced fusion (PG_FUSE=2, $ROUNDS timing rounds: the bits and the speed of the fused path)"
if [ "${SKIP_FUSE2:-0}" = 1 ]; then say "    skipped"
else
    env PG_ROUNDS=$ROUNDS PG_FUSE=2 VSDLSS_TRACE=1 TR_PACKED=0 $NUMA "$R" "$DUMP" 6 "$T" > "$OUT/real_fuse2.log" 2>&1
    grep -aE "fused components|transient fusion" "$OUT/real_fuse2.log" | awk '!seen[$0]++' | sed 's/^/    /' | tee -a "$S"
    sed -n '/^validation/,/^timing/p' "$OUT/real_fuse2.log" | grep -av '^timing' | sed 's/^/    /' | tee -a "$S"
    verdict "$OUT/real_fuse2.log" "forced fusion"; fused_note "$OUT/real_fuse2.log"; speedup "$OUT/real_fuse2.log"
fi

echo; say "=== 6. real dump, fusion upper bound (PG_FUSE=3: no unit late -- timing only, results not exact)"
if [ "${SKIP_BOUND:-0}" = 1 ]; then say "    skipped"
else
    env PG_ROUNDS=$ROUNDS PG_FUSE=3 VSDLSS_TRACE=1 TR_PACKED=0 $NUMA "$R" "$DUMP" 6 "$T" > "$OUT/real_bound.log" 2>&1
    grep -aE "fused components|transient fusion" "$OUT/real_bound.log" | awk '!seen[$0]++' | sed 's/^/    /' | tee -a "$S"
    sed -n '/^validation/,/^timing/p' "$OUT/real_bound.log" | grep -av '^timing' | sed 's/^/    /' | tee -a "$S"
    sed -n '/^timing/,$p' "$OUT/real_bound.log" | grep -aE "^timing|^  (orig|manual|tr|tr\+fuse) " | sed 's/^/    /' | tee -a "$S"
    sed -n '/^timing/,$p' "$OUT/real_bound.log" | awk '/^  (tr|tr\+fuse) / {t[$1]=$2}
        END {if (t["tr"] > 0) printf "    upper bound of fusion: tr %.1f ms -> every unit fused %.1f ms per step (%+.1f%%)\n", t["tr"], t["tr+fuse"], 100*(t["tr+fuse"]-t["tr"])/t["tr"]}' | tee -a "$S"
    fused_note "$OUT/real_bound.log"
fi

echo; say "Done. Please send: $S"
[ "${SKIP_SAVE:-0}" = 1 ] || say "(inputs for the simulator: $OUT/inputs -- cdiag.txt, x0.txt, src_nodes, <k>s, meta.txt; see test/bench_tr_replay.c save_inputs)"
