# Shared panel kernel optimization — 2026-09-10

Base: integrated tree 454bb8eb711b546da29970c3e5de5968699a88bc (remote c313d8a).

## Design and implementation

The previous left-looking panel kernel synchronized its team for every pivot column and traversed prior columns with a large stride for each output row. The replacement first factors the diagonal block, then computes independent external row tiles (32 rows each). Each tile completes all columns without cross-tile synchronization. Both diagonal updates and tile updates traverse contiguous rows in the inner loop. Each element retains increasing-k subtraction order, with no fast-math, pivot modification, or external BLAS dependency.

There is one external-row parallel region per panel, static scheduling, and team size is capped by the number of tiles. Existing outer component/RHS parallelism and nested-team suppression remain in effect. The diagonal block is serial; square panels gain locality but no within-panel factorization parallelism in this revision. Workspace adds a 32-double automatic array per active worker (256 bytes); M4's numerical heap budget is unchanged and, as before, excludes thread stacks/runtime.

The public API, sparse ordering, symbolic structure, factor layout, and disk format are unchanged. Nonfinite external values return NONFINITE; nonpositive diagonal pivots return NOT_POSDEF. Public factor/solve failure remains transactional; intermediate internal panels can be modified before an error, as before.

## Reference and scope

[Intel oneMKL PARDISO iparm documentation](https://www.intel.com/content/www/us/en/docs/onemkl/developer-reference-c/2023-2/pardiso-iparm-parameter.html) describes two-level factorization (iparm[23]) and matrix-versus-RHS solve parallelism (iparm[24]). These support choosing task granularity by phase. The row-tile kernel is our implementation decision, not a reconstruction of PARDISO internals. PARDISO was not linked or benchmarked.

Alternatives for later work: BLAS-3 blocked diagonal/update kernels; dependency-aware supernode task scheduling. Both require broader changes and should follow full-stage profiling on representative inputs.

## Measurement

`make bench-parallel`, GCC -O2, OpenMP, same environment, warm-up plus five repetitions (median), 1/2/4 requested threads. See panel-speed-before.tsv and panel-speed-after.tsv. Final after measurement ran without concurrent build/test jobs.

1024x256 panel: 1 thread 132.066 -> 10.586 ms (12.48x); 4 threads 48.518 -> 5.687 ms (8.53x). New 4-vs-1 speedup is 1.86x, versus old 2.72x: absolute runtime improves substantially, but this case's relative parallel efficiency decreases because the serial diagonal is now a larger share. This is not a claim of universally improved scaling.

384-order M4 factor+solve: 4 threads 20.739 -> 21.389 ms. This small end-to-end case shows no demonstrated improvement; timings are affected by serial symbolic work, disk work and noise. Do not extrapolate panel microbenchmark speedup to whole solver speed or large production EDA matrices.

## Validation

Full strict regression includes M1/M3 comparison, factor reconstruction, residuals, M4 I/O failures, and allocation failure injection (550 factor / 15 solve allocations). Additional known-L reconstruction covers 0/1/31/32/33/65 external rows, 1/2/4 threads and NaN external entries; serial build also executes these boundary cases. Existing tests check same-build serial/parallel bitwise equality.

Next priorities: separately measure M3 symbolic/update-map construction, M4 I/O and source scans, then add BLAS-3 blocking or supernode task scheduling where the dominant measured cost warrants it. Real EDA matrices are needed to establish production benefit.

Verification completed 2026-09-11: strict full regression and OPENMP=0 panel boundary tests passed. ASan/UBSan solver/io/ordering/MLD/M3/M4 tests passed on the initial run; linking test_m4_panels then failed with an unresolved vsdlss_mld_order symbol despite the definition/source being present. Rebuilding test_m4_panels and test_parallel with identical sanitizer flags succeeded and both passed. The intermittent linker cause was not reproduced. LeakSanitizer and ThreadSanitizer are not claimed.
