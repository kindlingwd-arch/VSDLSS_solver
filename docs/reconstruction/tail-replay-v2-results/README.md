# Tail replay v2 evidence

See [the report](../tail-replay-v2-report-20261011.md) and [pre-development analysis](../tail-replay-v2-analysis-20261011.md).

Performance data compare production code at `0234642` with the candidate developed from that commit, using identical updated measurement programs. Each ABBA round is a separate process: main, v2, v2, main. The PR is subsequently based on `85792d2`; these numbers do not measure its opt-in chain-order mode.

- `final-idle-2m/8m.csv` and corresponding logs: replay-only; 21 samples, two warmups, every output checked, actual tail teams recorded.
- `active-fullsolve-*.log`: complete M3 solve on the wires graph, 21 samples. Requesting 2/4/8 threads results in an actual tail team of 2; core and mapping time are included. GNU ld wrappers observe replay teams independently of core teams.
- `fullsolve-*.log`: complete solve of the degree-histogram PG graph, 15 samples. Its tail is below the parallel grain; correctness and negative control, not a measurement of the new tail kernel.
- `ablation-*.log`: candidate with SIMD 0, 1, 1, 0; 21 samples on the 2M wires graph.
- `coverage-*.log`: homogeneous-chunk classification counts; diagnostic timing is not performance evidence.
- `pr-*.log`: fresh validation after updating the branch to `85792d2`.

Default performance build: GCC 9.4, `-O2 -Wall -Wextra -Iinclude -std=c11`, OpenMP, METIS=1. Runtime: `OMP_PROC_BIND=close OMP_PLACES=cores OMP_DYNAMIC=FALSE OMP_WAIT_POLICY=ACTIVE GOMP_SPINCOUNT=30000`, default tail grain 2048. WSL2 on i7-1260P, 8 reported cores/16 logical CPUs, 12 GiB, one NUMA node. No other CPU-intensive WSL process was running during the final comparisons. Min/median/max expose remaining host noise.

Build `make METIS=1 bench_tail bench_pg_solve bench_tail_solve`. Replay invocations:

```sh
./bench_tail 2000000 "1 2 4 8" 21 20 200
./bench_tail 8000000 "1 2 4 8" 21 20 1000
SOLVE_THREADS="1 2 4 8" SOLVE_REPS=15 ./bench_pg_solve 6 8 1 1 2000000 224000 1335000 437236 3751 10 3
VSDLSS_TAIL_GRAIN=2048 ./bench_tail_solve
```

For comparisons, copy the candidate's benchmark programs into a clean `0234642` snapshot without changing its production sources; compile both with identical options. `bench_tail_solve` additionally requires the two GNU ld `--wrap` flags shown in its Makefile target. Run versions sequentially in ABBA order and retain raw logs. An 8x replay speedup and production-data/server validation are **not achieved** by these results.
