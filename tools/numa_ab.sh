#!/usr/bin/env bash
# NUMA placement A/B: tools/ab_test.sh with five fixed settings.
#
#   cd <data directory with diag.txt data.txt b_vector.txt x_vector.txt 1b_vector.txt ...>
#   bash <source>/tools/numa_ab.sh <source directory | transient_driver> [threads=32] [steps=5] [rounds=3]
#
#   c1  VSDLSS_NUMA_INTERLEAVE=0                 old behaviour: first-touch pages
#   c2  (defaults)                               the library interleaves its large arrays
#   c3  VSDLSS_INTERLEAVE=1                      the driver interleaves the whole process
#   c4  VSDLSS_NUMA_PLACE=1                      library interleave + each net's arrays on its socket
#   c5  VSDLSS_INTERLEAVE=1 VSDLSS_NUMA_PLACE=1  process interleave + each net's arrays on its socket
# Build, thread binding, output: as tools/ab_test.sh (ab_test_<time>/summary.txt).
HERE=$(cd "$(dirname "$0")" && pwd)
exec bash "$HERE/ab_test.sh" "${1:?usage: bash numa_ab.sh <source directory | transient_driver> [threads] [steps] [rounds]}" \
    "${2:-32}" "${3:-5}" "${4:-3}" \
    "VSDLSS_NUMA_INTERLEAVE=0" "" "VSDLSS_INTERLEAVE=1" "VSDLSS_NUMA_PLACE=1" "VSDLSS_INTERLEAVE=1 VSDLSS_NUMA_PLACE=1"
