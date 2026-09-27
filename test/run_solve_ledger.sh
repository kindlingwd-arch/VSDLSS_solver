#!/bin/bash
# Reproduces docs/reconstruction/25-solve-ledger-kernels-20260927.md.
#   make bench_step   (and, for the BLAS rows, bench_step_blas: see the doc)
#   test/run_solve_ledger.sh <1m|2m|4m|8m> <experiment> [out.log]
# experiments: flows | kv1 | kv3 | blas0 | blas32 | blas128 | perm2on | perm2off | buckets | acc_kv3
# Runs are single threaded; alternate A/B runs yourself (A B A B) when comparing processes.
set -e
case $1 in
 1m) A="1000000 100000 302038 595948 2001 10 3";;
 2m) A="2000000 200000 604038 1191948 4001 10 3";;
 4m) A="4000000 400000 1208038 2383948 8001 10 3";;
 8m) A="8000000 800000 2416038 4767948 16001 10 3";;
 *) echo "size: 1m|2m|4m|8m"; exit 2;;
esac
export PG_NETS=2 OMP_NUM_THREADS=1 OPENBLAS_NUM_THREADS=1
BIN=./bench_step; E=()
case $2 in
 flows)    E=(TR_STEPS=41 TR_CHECK=10 TR_MODES=GNPI VSDLSS_SOLVE_LEDGER=1);;
 kv1)      E=(TR_STEPS=41 TR_CHECK=10 TR_MODES=IK TR_KKV=1 VSDLSS_SOLVE_LEDGER=1);;
 kv3)      E=(TR_STEPS=41 TR_CHECK=20 TR_MODES=IK TR_KKV=3 VSDLSS_SOLVE_LEDGER=1);;
 blas0|blas32|blas128) BIN=./bench_step_blas; E=(VSDLSS_BLAS_SOLVE_MIN=${2#blas} TR_STEPS=31 TR_CHECK=0 TR_MODES=I VSDLSS_SOLVE_LEDGER=1);;
 perm2on)  E=(VSDLSS_PERM2=1 TR_STEPS=31 TR_CHECK=0 TR_MODES=G VSDLSS_SOLVE_LEDGER=1);;
 perm2off) E=(VSDLSS_PERM2=0 TR_STEPS=31 TR_CHECK=0 TR_MODES=G VSDLSS_SOLVE_LEDGER=1);;
 buckets)  E=(TR_STEPS=31 TR_CHECK=0 TR_MODES=P VSDLSS_SOLVE_LEDGER=2);;
 acc_kv3)  E=(TR_STEPS=21 TR_CHECK=5 TR_MODES=G TR_IKV=3);;
 *) echo "unknown experiment"; exit 2;;
esac
out=${3:-/dev/stdout}
env "${E[@]}" $BIN 5 1 1 1 $A > $out 2>&1
