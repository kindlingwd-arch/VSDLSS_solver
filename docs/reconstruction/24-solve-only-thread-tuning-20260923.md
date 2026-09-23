# Post-factorization solve throughput (2026-09-23)

Continuation of [23](23-full-scale-packed-solve-20260923.md). This experiment
changes **only the solve thread setting after building one factor**. It does
not change factorization, numerical kernels, ordering or the direct method.
The existing `vsdlss_set_num_threads()` can be called between solve batches;
the factor is reusable across these settings. In these runs factorization
always used two threads, order 5 (AMD), shuffled node IDs and one RHS. The
benchmark switched the thread setting before each group of seven packed
solves, discarded the first solve, and reported the median of the remaining
six. Every result was mapped back and checked bitwise against the ordinary
solution obtained from the same factor. Each process scanned both ascending
and descending thread counts to expose warm-cache and virtual-machine noise.

| Synthetic graph | Nodes | Full nonzeros | Sweep direction | 1 thread | 2 threads | 4 threads | 8 threads |
| --- | ---: | ---: | --- | ---: | ---: | ---: | ---: |
| Original powergrid | 6,396,932 | 19,989,422 | ascending | 114.62 ms | 80.91 ms | 69.06 ms | 64.07 ms |
| Original powergrid | 6,396,932 | 19,989,422 | descending | 110.07 ms | 84.38 ms | 58.96 ms | 54.83 ms |
| Degree-profile single net | 22,875,397 | 71,151,237 | ascending | 578.30 ms | 402.31 ms | 309.77 ms | 303.33 ms |
| Degree-profile single net | 22,875,397 | 71,151,237 | descending | 579.89 ms | 433.28 ms | 307.77 ms | 262.89 ms |
| Degree-profile VDD + GND | 22,875,397 | 71,151,237 | ascending | 562.75 ms | 339.53 ms | 270.57 ms | 202.11 ms |
| Degree-profile VDD + GND | 22,875,397 | 71,151,237 | descending | 562.76 ms | 307.36 ms | 238.98 ms | 195.64 ms |

On this host (8-CPU cgroup quota), four solve threads improve the single-net
case by 23–29% relative to two solve threads; eight threads improve VDD +
GND by 36–40%. Eight threads in the single-net case varied more than four,
so four is a conservative setting for that topology. The original
20-million-nonzero topology improved by 21% at eight versus two threads in
the ascending scan and 35% in the descending scan. These are repeated **packed** solve times, independent
of RHS packing, factorization and postprocessing. The best count depends on
available physical CPUs, memory bandwidth and other running workloads.

An attempted immutable supernodal-tree cache gave no reliable improvement:
6.4M-node packed solve 77.35 ms baseline versus 77.50 and 72.92 ms with the
cache (two runs), and full-size single-net 404.11 versus 404.82 ms. An
experimental destructive in-place packed solve saved at most a few percent
consistently at full scale and regressed on the smaller grid. Keeping an
original RHS required a 15 ms copy for the 22.9M-node test, offsetting this
benefit. An additional temporary vector copy was removed in a prototype
but comparisons against the original implementation were mixed. These
prototypes were discarded; there is no new numerical solve path in this
change.

Reproduce the sweep using the benchmark built with `make bench_pg_profile
bench_powergrid`:

```sh
PG_SOLVE_SWEEP=1 ./bench_pg_profile 5 2 1 1
PG_NETS=2 PG_SOLVE_SWEEP=1 ./bench_pg_profile 5 2 1 1
PG_SOLVE_SWEEP=1 ./bench_powergrid 2e7 5 2 1 1
PG_NETS=2 PG_SOLVE_SWEEP=1 PG_SOLVE_SWEEP_REVERSE=1 ./bench_pg_profile 5 2 1 1
```

`PG_SOLVE_SWEEP_REVERSE=1` also works for the other two commands. For an
application, call `vsdlss_set_num_threads(solve_threads)` **after**
`vsdlss_factorize_m3` and before repeated `vsdlss_m3_solve_packed` calls.
Change this global thread setting between batches, not while other solves
are running.
