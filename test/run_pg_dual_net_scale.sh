#!/usr/bin/env bash
# Reproducible single-RHS VDD/GND power-grid profiles with exact degrees 1..6.
set -euo pipefail
cd "$(dirname "$0")/.."
make bench_pg_profile
scale=${1:-3m}
case "$scale" in
    61m) counts=(61000000 2500000 15260000 43180000 59988 10 2) ;;
    30m) counts=(30000000 1229508 7504918 21236066 29496 10 2) ;;
    8m)  counts=(8000000 327868 2001312 5662950 7858 10 2) ;;
    3m)  counts=(3000000 122950 750492 2123606 2940 10 2) ;;
    500k) counts=(500000 20492 125082 353934 480 10 2) ;;
    *) echo "usage: $0 {61m|30m|8m|3m|500k}" >&2; exit 2 ;;
esac
# PG_GENERATE_ONLY=1 validates the large graph without allocating a factor.
# PG_SOLVE_SWEEP=1 benchmarks 1, 2, 4, 8 solve threads with the same factor.
PG_NETS=2 PG_SPLIT=0.52 ./bench_pg_profile 5 "${PG_THREADS:-8}" 1 1 "${counts[@]}"
