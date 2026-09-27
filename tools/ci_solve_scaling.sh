#!/usr/bin/env bash
# Multi-thread solve scaling A/B: original (BASE_REF) vs this checkout.
#
#   tools/ci_solve_scaling.sh [scale] [orders] [rounds]
#     scale  : 8m (default) | 3m     VDD/GND dual-net power grid (PG_NETS=2)
#     orders : "6 5" (default)       6 = METIS, 5 = AMD
#     rounds : 2 (default)           interleaved repetitions (min is reported)
#   env: BASE_REF (default 1dbf969), THREADS (default: 1 2 4 ... <= nproc),
#        SOLVE_REPS (default 7), OUT (default solve-scaling-out),
#        METIS_PREFIX (default $HOME/metis64; built here if missing)
#
# Variants, run interleaved on the same machine and the same matrix:
#   base : BASE_REF                     (before the dense-solve work)
#   new  : this checkout
#   pull : this checkout, VSDLSS_FWD_SUB_PUSH=0 (subtree phase in pull form)
# Each run factors once, then times SOLVE_REPS single-RHS solves per thread
# count (bench_pg_solve), with VSDLSS_SOLVE_PROFILE=1 for the tree phases.
# Results: $OUT/summary.md (+ raw logs).  Also usable on any Linux box.
set -euo pipefail
cd "$(dirname "$0")/.."
ROOT=$PWD
SCALE=${1:-8m}; ORDERS=${2:-"6 5"}; ROUNDS=${3:-2}
BASE_REF=${BASE_REF:-1dbf969}
OUT=${OUT:-$ROOT/solve-scaling-out}; mkdir -p "$OUT"
METIS_PREFIX=${METIS_PREFIX:-$HOME/metis64}
NPROC=$(nproc)
if [ -z "${THREADS:-}" ]; then THREADS=""; t=1; while [ $t -le "$NPROC" ]; do THREADS="$THREADS $t"; t=$((t*2)); done; fi
THREADS=$(echo $THREADS)
export SOLVE_REPS=${SOLVE_REPS:-7}

case "$SCALE" in   # run_pg_dual_net_scale.sh profiles, d4 odd / d6+=3 for this generator
    8m) COUNTS="8000000 327868 2001310 5662950 7859 10 3" ;;
    3m) COUNTS="3000000 122950 750491 2123606 2941 10 3" ;;
    *) echo "scale must be 8m or 3m" >&2; exit 2 ;;
esac

# --- METIS 5 with 64-bit idx_t (the library only; its CLI tools may fail to link) ---
if [ ! -f "$METIS_PREFIX/lib/libmetis.a" ]; then
    W=$(mktemp -d)
    git clone -q --depth 1 https://github.com/KarypisLab/GKlib.git "$W/GKlib"
    git clone -q --depth 1 https://github.com/KarypisLab/METIS.git "$W/METIS"
    (cd "$W/GKlib" && make config prefix="$METIS_PREFIX" >/dev/null && make -j"$NPROC" install >/dev/null)
    (cd "$W/METIS" && make config i64=1 prefix="$METIS_PREFIX" gklib_path="$METIS_PREFIX" >/dev/null \
        && { make -j"$NPROC" install >/dev/null 2>&1 || true; })
    mkdir -p "$METIS_PREFIX/include" "$METIS_PREFIX/lib"
    cp "$W/METIS/build/libmetis/libmetis.a" "$METIS_PREFIX/lib/"
    sed 's|^//#define IDXTYPEWIDTH 32|#define IDXTYPEWIDTH 64|; s|^//#define REALTYPEWIDTH 32|#define REALTYPEWIDTH 32|' \
        "$W/METIS/include/metis.h" > "$METIS_PREFIX/include/metis.h"
fi
printf '#include <metis.h>\nint main(void){return sizeof(idx_t)==8?0:1;}\n' > "$OUT/idx.c"
cc -I"$METIS_PREFIX/include" "$OUT/idx.c" -o "$OUT/idx" && "$OUT/idx" || { echo "METIS idx_t is not 64-bit" >&2; exit 1; }

# --- two builds ---
MK=(METIS=1 METIS_CFLAGS="-I$METIS_PREFIX/include" METIS_LIBS="-L$METIS_PREFIX/lib -lmetis -lGKlib")
git worktree remove --force "$OUT/base-src" 2>/dev/null || true
rm -rf "$OUT/base-src"; git worktree prune
git worktree add -q --detach "$OUT/base-src" "$BASE_REF"
(cd "$OUT/base-src" && make -s bench_pg_solve "${MK[@]}")
make -s bench_pg_solve "${MK[@]}"
cp "$OUT/base-src/bench_pg_solve" "$OUT/bench_base"; cp bench_pg_solve "$OUT/bench_new"
git worktree remove --force "$OUT/base-src"

{ echo "commit_new=$(git rev-parse --short HEAD) base=$BASE_REF scale=$SCALE orders=\"$ORDERS\" rounds=$ROUNDS";
  echo "threads=\"$THREADS\" solve_reps=$SOLVE_REPS nproc=$NPROC";
  echo "cpu=$(grep -m1 'model name' /proc/cpuinfo | cut -d: -f2- | sed 's/^ *//')";
  echo "mem_gb=$(awk '/MemTotal/{printf "%.1f",$2/1048576}' /proc/meminfo)"; } > "$OUT/meta.txt"
cat "$OUT/meta.txt"

# --- interleaved runs ---
for r in $(seq 1 "$ROUNDS"); do
  for ord in $ORDERS; do
    for v in base new pull; do
      bin=$OUT/bench_new; push=1
      [ $v = base ] && bin=$OUT/bench_base
      [ $v = pull ] && push=0
      log=$OUT/run_${v}_o${ord}_r${r}.log
      echo "== round $r order $ord $v"
      PG_NETS=2 SOLVE_THREADS="$THREADS" VSDLSS_SOLVE_PROFILE=1 VSDLSS_FWD_SUB_PUSH=$push \
        stdbuf -oL -eL "$bin" "$ord" "$NPROC" 1 1 $COUNTS > "$log" 2>&1
      grep -E "^# factor|^SOLVE|^INTERNAL|xhash" "$log" | sed 's/page_faults.*//'
    done
  done
done

python3 "$ROOT/tools/ci_solve_summary.py" "$OUT" > "$OUT/summary.md"
cat "$OUT/summary.md"
