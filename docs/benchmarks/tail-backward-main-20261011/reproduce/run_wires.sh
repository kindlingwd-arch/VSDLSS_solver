#!/usr/bin/env bash
set -euo pipefail
script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
repo_dir=$(cd -- "$script_dir/../../../.." && pwd)
out_dir=${1:-$(mktemp -d /tmp/vsdlss-tail-research.XXXXXX)}
mkdir -p -- "$out_dir"
out_dir=$(cd -- "$out_dir" && pwd)
# GNU make treats spaces in target paths as separators.
if [[ "$out_dir" == *[[:space:]]* ]]; then
    printf 'Output path must not contain whitespace: %s\n' "$out_dir" >&2
    exit 2
fi
baseline=85792d29b4fe91c87d8cb16e919666b0d8f578e4
if ! git -C "$repo_dir" rev-parse --verify "$baseline^{commit}" >/dev/null; then
    printf 'Cannot read the baseline Git commit; check clone history and Git worktree paths.\n' >&2
    exit 2
fi
# Normalize checkout line endings without changing the user's Git settings.
if ! git -c core.autocrlf=true -C "$repo_dir" diff --quiet "$baseline" -- src include test/bench_phase.c test/bench_tail.c Makefile third_party/metis-5.1.0; then
    printf 'Source differs from the recorded baseline %s; use that production snapshot to reproduce these results.\n' "$baseline" >&2
    exit 2
fi
export OMP_DYNAMIC=FALSE OMP_PROC_BIND=close OMP_PLACES=cores
export OMP_WAIT_POLICY=ACTIVE GOMP_SPINCOUNT=30000 TMPDIR=/tmp
unset VSDLSS_TAIL_GRAIN
{
    printf 'baseline=%s\nhead=%s\n' "$baseline" "$(git -C "$repo_dir" rev-parse HEAD)"
    uname -a
    gcc --version | head -1
    lscpu
    env | LC_ALL=C sort | sed -n '/^OMP_/p; /^GOMP_/p; /^VSDLSS_/p'
} > "$out_dir/environment.log"
make -C "$repo_dir" -f Makefile -f "$script_dir/Makefile" METIS=1 \
    CFLAGS='-O2 -Wall -Wextra -Werror -Iinclude -std=c11' \
    TAIL_OUT="$out_dir" tail-research-build > "$out_dir/build.log" 2>&1
cd -- "$out_dir"
./generate_wires
printf '%s  wires-1m.bin\n' be612edbc86d656b69608836a8e9abdd8cac9951ae6ebb5052cfd58a0f1611f6 | sha256sum -c -
for spec in default-1:0 chain-2:1 chain-3:1 default-4:0; do
    label=${spec%:*}
    chain=${spec#*:}
    VSDLSS_CHAIN_ORDER=$chain ./analysis_probe wires-1m.bin > "wires-$label.log" 2>&1
    printf 'completed %s exit=0\n' "$label"
    sed -n '/^component=/p; /^GRAIN /p; /^SOLVE /p; /^LEDGER /p; /^backward_error=/p' "wires-$label.log"
done
printf 'Results: %s\n' "$out_dir"
