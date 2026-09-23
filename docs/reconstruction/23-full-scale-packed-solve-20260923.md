# Full-scale direct power-grid solve validation (2026-09-23)

Follow-up to [22](22-packed-rhs-solve-20260923.md). The earlier 2M-node
experiment was only a screening run. This experiment exercises the original
`bench_powergrid` topology and the full 22,875,397-node degree histogram,
both as one connected grid and as two VDD/GND components. All runs use AMD
(order 5), two threads, shuffled vertex IDs, one RHS, and the same direct
factorization for the ordinary and factor-local packed solve. The packed
permutation and RHS are prepared before timed calls, so timings represent
reuse of an already packed data flow rather than conversion of global RHS
on every solve. Every comparison alternates ordinary and packed calls,
discards the first pair and takes the median of six calls per API.

| Generator | Nodes | Full nonzeros | Ordinary / packed median, run 1 | Ordinary / packed median, run 2 | Bitwise equality | Backward error |
| --- | ---: | ---: | ---: | ---: | --- | ---: |
| Original `bench_powergrid` | 6,396,932 | 19,989,422 | 126.72 / 77.02 ms | — | yes | 2.11e-16 |
| Degree-profile single net | 22,875,397 | 71,151,237 | 663.34 / 414.16 ms | 635.14 / 400.07 ms | yes in both | 5.08e-17 |
| Degree-profile VDD + GND | 22,875,397 | 71,151,237 | 536.17 / 307.31 ms | 533.23 / 305.78 ms | yes in both | 2.76e-17 |

The original `bench_powergrid` target is **20 million full nonzeros**, not
20 million nodes. Its default topology produces about 6.4 million nodes.
The degree-profile example actually exceeds 20 million nodes and preserves
the exact 22,875,397-node histogram in both single- and two-net modes.

Full-size factorization took 22.24 and 22.83 s (single net) and 17.93 and
16.70 s (VDD + GND); the largest observed resident high-water mark was
5,224 MB. The two single-net paired runs saved 37.6% and 37.0% in solve
time; the two VDD/GND paired runs saved 42.7% in each run. The original
20-million-nonzero topology saved 39.2% in its paired run.
This compares **repeated solve** time; the packed interface does not speed
up factorization. The ordinary backward errors are computed against the
original sparse matrix, and packed solutions are mapped back to the global
IDs for exact bitwise comparison. Synthetic topology and load values remain
proxies for an actual extracted EMIR network. If each new load is generated
in global order and the final voltages must be restored to global order,
the cost of these conversions must be added to the packed timing.

Reproduce with `make bench_pg_profile bench_powergrid`, then:

```sh
./bench_powergrid 2e7 5 2 1 1
./bench_pg_profile 5 2 1 1
PG_NETS=2 ./bench_pg_profile 5 2 1 1
```
