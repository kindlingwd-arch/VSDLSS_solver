# Power-grid direct-solver experiments (2026-09-23)

Scope: measure ordering and single-RHS movement on the current `main` direct
solver. No iterative solver was tested or introduced. Baseline:
`463dce05f8c5ae35e7da785a4aba6b7c84afff43`. The only code change on
this experiment branch resets the timer immediately before the scatter phase;
previous `VSDLSS_TRACE` output labeled the entire solve as `solve: scatter`.

## Workload and method

The existing `bench_pg_profile` generator produced exactly 2,000,000 nodes
with degree histogram 224,000 / 1,335,200 / 437,000 / 3,787 / 10 / 3 for
degrees 1 / 2 / 3 / 4 / 5 / >=6. Its shuffled numbering and grounded
brick-wall grid are synthetic, not a customer's extracted netlist. Both
ordering choices use the same graph, conductances, random seed, two threads,
one RHS, and exact 0-3 degree pre-elimination. `PG_NETS=2` partitions the
same histogram between independent VDD and GND graphs. Three alternated
runs were made for each configuration on an AMD EPYC virtual host; other
processes and memory/page cache can affect wall time. Report medians for
factorization and ranges for ordering.

```sh
make bench_pg_profile
VSDLSS_TRACE=1 PG_NETS=1 ./bench_pg_profile 5 2 1 1 2000000 224000 1335200 437000 3787 10 3
VSDLSS_TRACE=1 PG_NETS=1 ./bench_pg_profile 4 2 1 1 2000000 224000 1335200 437000 3787 10 3
VSDLSS_TRACE=1 PG_NETS=2 ./bench_pg_profile 5 2 1 1 2000000 224000 1335200 437000 3787 10 3
VSDLSS_TRACE=1 PG_NETS=2 ./bench_pg_profile 4 2 1 1 2000000 224000 1335200 437000 3787 10 3
```

Repeat the four commands in that order three times. `order=5` is AMD and
`order=4` is the repository's existing MLD. The `core ordering` traces
for two nets are emitted by concurrent workers and must not be added to
estimate wall time.

## Results

| Graph | Ordering | Core nodes | nnz(L), rounded | Core ordering | Factor median (three runs) | Warm solve |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| One net | AMD | 126,024 | 6.3 M | 0.049–0.058 s | 1.04 s (0.86–1.36) | about 0.04 s |
| One net | MLD | 126,024 | 6.5 M | 1.484–1.525 s | 2.44 s (2.33–2.44) | about 0.04 s |
| VDD + GND | AMD | 124,458 | 5.5 M | two workers, each 0.021–0.030 s | 0.69 s (0.66–0.69) | about 0.03 s |
| VDD + GND | MLD | 124,458 | 6.1 M | two workers, each 0.651–0.750 s | 1.37 s (1.37–1.51) | about 0.03 s |

The corresponding backward errors in the benchmark were 1.19e-17,
3.26e-17, 1.48e-17, and 1.57e-17 respectively, in table order.
AMD is preferable to this MLD implementation for these cases; this
comparison does **not** test METIS, nested dissection from another library,
or real extracted grids. The two-net experiment additionally confirms that
splitting changes the core and fill, so it cannot be read as a speedup
attributable solely to thread scheduling.

After resetting the phase timer, a representative 2M-node one-net AMD
solve traced gather at 5–8 ms, core at 19–21 ms, reduction forward/backward
at about 4–6 ms each, and scatter at about 4 ms (one repeat was 10 ms).
The benchmark's 0.04 s warm solve is printed at centisecond resolution.
Previously reported `solve: scatter` traces from this code were cumulative;
do not use their values to estimate a packed-RHS API speedup. An internal
numbering interface might avoid roughly 9–18 ms of the measured work in
this synthetic case, but it also needs a precise mapping and caller contract;
the end-to-end saving must be measured after implementation.

## Decision and next experiments

1. Keep AMD as the default; there is no evidence here for switching to the
   present MLD on this family of grids.
2. Benchmark an independent separator-based ordering on the **reduced
   core**, with total factor/solve time and fill as the criteria. Time its
   ordering construction too. Do not substitute the existing MLD timing
   for a nested-dissection experiment.
3. Prototype a caller-owned, internal-order single-RHS interface only for
   repeated solves, after writing down permutation lifetime and concurrent
   use semantics. Compare against the ordinary API at identical accuracy.
4. Validate both on at least one real extracted VDD/GND net, including
   voltage-drop and branch-current errors, before selecting a default.

For any future gain claim, rerun alternated baselines on the target machine,
record medians with more precise solve timings, and compare total work rather
than a microkernel in isolation.
