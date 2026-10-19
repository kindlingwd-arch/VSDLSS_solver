#!/usr/bin/env bash
# Replay software prefetch A/B: tools/ab_test.sh with four fixed settings,
# all on the production NUMA setting (process interleave + placement by user).
#
#   cd <data directory with diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt ...>
#   bash <source>/tools/prefetch_ab.sh <source directory | transient_driver> [threads=32] [steps=5] [rounds=5]
#
#   c1  no prefetch (VSDLSS_PREFETCH unset = 0)
#   c2  VSDLSS_PREFETCH=8     prefetch 8 neighbour slots ahead
#   c3  VSDLSS_PREFETCH=24    24 slots ahead (the distance test_m3 checks)
#   c4  VSDLSS_PREFETCH=64    64 slots ahead
# The prefetch acts on both replays; compare "reduce forward" and "reduce
# backward" in section 3 of summary.txt to see each side.  Prefetch hints
# never change the bits: all hashes must be identical.
# Build, thread binding, output: as tools/ab_test.sh (ab_test_<time>/summary.txt).
HERE=$(cd "$(dirname "$0")" && pwd)
BASE="VSDLSS_INTERLEAVE=1 VSDLSS_NUMA_PLACE=1"
exec bash "$HERE/ab_test.sh" "${1:?usage: bash prefetch_ab.sh <source directory | transient_driver> [threads] [steps] [rounds]}" \
    "${2:-32}" "${3:-5}" "${4:-5}" \
    "$BASE" "$BASE VSDLSS_PREFETCH=8" "$BASE VSDLSS_PREFETCH=24" "$BASE VSDLSS_PREFETCH=64"
