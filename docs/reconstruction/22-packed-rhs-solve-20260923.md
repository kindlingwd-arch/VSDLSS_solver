# Repeated power-grid solves in factor-local order (2026-09-23)

Continuation of [21](21-powergrid-direct-experiments-20260923.md). The
direct solver and exact reduction are unchanged. For callers that can keep
RHS and solutions in factor-local order, M3 now exports a
`packed_to_global` permutation and solves a single packed RHS. The packed
sequence concatenates all components' local solve orders. The ordinary
single-RHS API retains global numbering, aliasing, and transactional output.

## Diagnosis

The 2M-node, two-thread synthetic power-grid benchmark previously reported
about 5-8 ms in gather and about 4 ms in scatter per solve (after correcting
the cumulative trace timer). The core took about 19-21 ms. The supernodal
solve itself benefits from multiple threads: on the one-net graph, warm
end-to-end times in three runs were 54.90-57.36 ms at one thread,
35.72-36.21 ms at two threads, and 26.87-29.92 ms at four threads.
Building the core task tree took only 0.52-0.64 ms in a temporary probe;
we discarded a prototype that merely removed the core's extra vector
allocation, because its five-run times overlapped the baseline.

The packed API removes the two global conversions **only if the caller
produces and consumes packed vectors**. An application that repacks its
global vector before every solve pays the conversions elsewhere.

## Implementation and safety

- `vsdlss_m3_export_packed_permutation` copies the mapping once. Its
  elements are global vertex IDs. The mapping is immutable for the life
  of the factor.
- `vsdlss_m3_solve_packed` shares the existing component scheduler,
  workspaces, low-degree replay, core solver, and two-phase commit with
  `vsdlss_m3_solve`. It reads each component's consecutive packed slice
  and writes the corresponding slice only after *all* components succeed.
- rhs and solution may alias. Nonfinite input leaves the entire output
  unchanged. Concurrent solves still acquire cached workspaces or fall
  back to private workspaces. The ordinary API takes its original
  global-map branch.
- `test_parallel` compares packed and ordinary solutions bitwise at
  1/2/4 threads, including four components, aliasing, invalid map length,
  nonfinite input, and the separate case where all components have empty
  reduced cores.

## A/B benchmark

Existing `bench_pg_profile` with `PG_NETS=1` or `2`, order 5 (AMD),
2 threads, one RHS, shuffled numbering, and the 2M-node degree histogram
specified in [21](21-powergrid-direct-experiments-20260923.md).
`make bench_pg_profile` now prints warm ordinary and packed times to
five decimal places and asserts that the packed solution maps back
bitwise to the ordinary solution. Each process computes the permutation
and packs its RHS *before* timing packed solves; the warm number is the
minimum of three repetitions for each API, with ordinary timed first.
Five independent process runs were measured. The benchmark now additionally
alternates call order for six ordinary and six packed calls per process,
reporting the median of each set. Four fresh processes were used for this
paired check.

| Graph | Ordinary warm, five runs (ms) | Packed warm, five runs (ms) | Median paired improvement |
| --- | --- | --- | ---: |
| One net | 38.70, 39.18, 41.77, 34.42, 35.22 | 28.15, 28.38, 27.60, 27.60, 29.10 | about 27% |
| VDD + GND | 31.23, 30.96, 28.28, 32.43, 31.85 | 23.03, 18.16, 20.45, 19.07, 18.70 | about 41% |

These are synthetic, same-factor warm times on a shared virtual machine,
not a claim about a customer's grid. Factorization is unchanged. The
original-numbering call on this branch had comparable timings to the
prior benchmark (roughly 35-43 ms single net and 28-32 ms dual net
across the measurements). Next validate the API at the customer's actual
node count and wire-current checks, and verify that upstream current
assembly and downstream voltage/current evaluation can truly stay in
the factor-local numbering.

The alternating-order check gave these per-process medians (ordinary /
packed, in ms):

| Graph | Four paired process runs | Median of per-process savings |
| --- | --- | ---: |
| One net | 37.19/34.90; 38.78/27.97; 40.99/29.04; 42.63/29.11 | about 29% |
| VDD + GND | 30.85/18.17; 30.21/21.02; 28.40/20.25; 32.42/21.10 | about 33% |

The first single-net run saved only 6% while the other three saved
28-32%; the range is relevant when interpreting these VM timings.
