# Parallel replay of the elimination tail

M3 builds an immutable conflict schedule after reducing a component. It tracks
the pivot and every neighbour touched by a record. Records in one level touch
disjoint vertices; levels run forward for RHS reduction and backward for recovery.
All updates to a vertex retain their original arithmetic order. No floating-point
atomics are used. This includes shared scatter targets, not just pivot dependencies.

With solve-order relabelling enabled, M3 can also group these commuting records
into contiguous levels before relabelling. Each level has static tiles of up to
256 records, using the existing sequential replay kernels inside each tile. The
schedule is built once during factorization and reused for every RHS. Factorization
may take longer; schedule construction and packing are outside solve timing.
Homogeneous tiles use fixed-degree SIMD loops for recovery after relabelling;
mixed-degree tiles use the existing general kernel. Both preserve each record's
division and subtraction order.

`VSDLSS_REPLAY_LEVELS=0` disables construction and parallel replay. The default is
enabled. Changing it between solves on a retained factor disables concurrency but
does not restore its original record layout. `VSDLSS_REPLAY_PACK=0` retains the
indirect schedule and original record order. Set environment variables before
factorization; do not change them concurrently with a solve.

Small tails (less than 32768 records), narrow schedules (mean width below 256),
unpacked records, disk factors, and builds without OpenMP keep the existing path.
Optional schedule/packing allocation failures retain a valid factor and fall back
to the available path. Packed array indices are bounded before constructing a
schedule. Existing nonfinite checks and external input formats are retained.

The indirect schedule adds 8 bytes per record plus level pointers. Contiguous
tiles add about 20 bytes per 256 records plus level pointers; partially filled
tiles increase this. Packing temporarily duplicates the segment's head, pivot,
neighbour and multiplier arrays. It does not duplicate the numerical core factor.
Use `VSDLSS_TRACE=1` to report retained schedule size and factorization phases.
Trace output should be disabled for timed comparisons.

## Reproduce

Build with the same optimization flags and ordering as the production driver:

```sh
TMPDIR=/tmp make -j8 METIS=1 test bench_phase bench_replay_levels
```

Use the actual matrix on the actual solve machine. The benchmark uses two warmups,
alternates variants, reports median/min/max wall time, checks every sample for
bitwise equality, and checks the complete solution's backward error. It times
copies and validation outside replay intervals. Full solve time includes the
solver's gathers, scatters and other normal solve work. A second factor with all
replay optimizations disabled supplies the full-solve baseline, outside its timer.
The backward A/B diagnostic instead uses the same factor/layout to isolate
concurrency. This distinction matters when packing changes the record order.

```sh
OMP_PROC_BIND=spread OMP_PLACES=cores VSDLSS_REPLAY_LEVELS=1 \
  VSDLSS_REQUIRE_REPLAY_SPEEDUP=8 \
  ./bench_phase text:/path/to/net 6 "1 8 16 32 64" 31 > replay-results.log
```

`text:DIR` reads `diag.txt` and strict upper `data.txt` in the existing driver
format. A binary PG_DUMP matrix is also accepted. Order `6` requires METIS; use
the production order for a fair comparison. Run with the same BLAS settings,
thread affinity, NUMA policy and CPU load as the original driver. The acceptance
gate fails if the measured largest-component backward speedup is below 8x or no
records received a schedule. Complete solve timings and residuals are reported
separately. A passing backward gate does not imply an 8x complete-solve gain.

Synthetic cases are useful diagnostics; they do not establish a user-case result:

```sh
# About 32 million elimination records in 666 levels.
OMP_PROC_BIND=spread OMP_PLACES=cores ./bench_replay_levels 48000 666 0 7 1
# Same records using the indirect schedule.
OMP_PROC_BIND=spread OMP_PLACES=cores ./bench_replay_levels 48000 666 0 7 0
# Shared update targets; normal factorization and complete solve.
OMP_PROC_BIND=spread OMP_PLACES=cores ./bench_phase leaf:4096:16 6 "1 2 4 8" 31
```

The microbenchmark compares the optimized tail with a separately retained original
layout. Its final argument selects contiguous packing (1) or indirect scheduling
(0). The leaf case is a synthetic grounded graph with a four-regular core and
many leaves. Large leaf/grid cases often mostly reduce in the existing parallel
blocks; always inspect the printed tail percentage and scheduled-record count.

## Verification

`test-replay-levels` covers both packed formats, original parallel blocks and a
nonzero tail offset, degrees 0..3, shared targets, saved and in-place replay,
narrow levels, nonfinite recovery, allocation failure and retry, and actual M3
factorization followed by multiple RHS solves. It compares serial and parallel
results bitwise and checks the production solution residual. It also compares
a factor built with optimizations disabled against the reordered factor.

The performance target is an acceptance condition, not a theoretical consequence
of 666 wide layers. Memory bandwidth, record layout, synchronization and available
CPU execution resources still affect speedup. Representative server measurements
are required before claiming an 8x result or merging as a performance release.
