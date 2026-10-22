#!/usr/bin/env bash
# Medians of the core phase times in a VSDLSS_SOLVE_PROFILE=1 log (as written
# by tools/core_profile_test.sh: core_profile_<time>/profile.log), for the
# largest core in it, per thread count.
#   bash core_profile_summary.sh <profile.log>
set -u
L=${1:?usage: bash core_profile_summary.sh <profile.log>}
N=$(grep -a "vsdlss solve profile" "$L" | sed -E 's/.*n=([0-9]+).*/\1/' | sort -n | tail -n 1)
grep -a "vsdlss solve profile: n=$N " "$L" | awk '{
    gf = gb = 0
    for (i = 1; i <= NF; i++) {
        if ($i ~ /^nt_top=/) { split($i, kv, "="); k = kv[2] }
        if ($i == "fwd" && $(i+1) == "sub" && !gf) { fs = $(i+2); ft = $(i+4); gf = 1 }
        if ($i == "bwd" && $(i+1) == "top" && !gb) { bt = $(i+2); bs = $(i+4); gb = 1 }
    }
    n[k]++; FS_[k, n[k]] = fs; FT[k, n[k]] = ft; BT[k, n[k]] = bt; BS[k, n[k]] = bs }
  function med(a, k, c,   i, j, t, v) { for (i = 1; i <= c; i++) v[i] = a[k, i] + 0
      for (i = 2; i <= c; i++) { t = v[i]; for (j = i - 1; j >= 1 && v[j] > t; j--) v[j+1] = v[j]; v[j+1] = t } return v[int((c+1)/2)] }
  END { for (k in n) { p1 = med(FS_, k, n[k]); p2 = med(FT, k, n[k]); p3 = med(BT, k, n[k]); p4 = med(BS, k, n[k])
      printf "threads %-3s fwd subtrees %6.2f ms | fwd top %6.2f ms | bwd top %6.2f ms | bwd subtrees %6.2f ms | total %6.2f ms  (%d solves)\n",
             k, 1e3*p1, 1e3*p2, 1e3*p3, 1e3*p4, 1e3*(p1+p2+p3+p4), n[k] } }' | sort -k2,2n
