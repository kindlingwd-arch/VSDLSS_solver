#ifndef VSDLSS_PARALLEL_H
#define VSDLSS_PARALLEL_H
#include "vsdlss.h"
#ifdef _OPENMP
#include <omp.h>
#define VSDLSS_OMP(x) _Pragma(#x)
#else
#define VSDLSS_OMP(x)
#endif
int vsdlss_parallel_width(double work);
/* Threads the calling thread may use inside an enclosing parallel region
 * (nested teams for concurrently processed components; 0 or 1 = serial).
 * Thread-local; returns the previous value. */
int vsdlss_parallel_set_budget(int threads);
/* malloc / calloc for large arrays (>= 8 MiB): the fresh pages are marked
 * for transparent huge pages, which cuts first-touch page faults by up to
 * 512x and TLB misses in random access.  A hint only (no-op elsewhere;
 * VSDLSS_NO_HUGEPAGES=1 disables it); free() as usual. */
#include <stddef.h>
void *vsdlss_big_malloc(size_t bytes);
void *vsdlss_big_calloc(size_t count, size_t size);
void *vsdlss_big_malloc_aligned(size_t align, size_t bytes);  /* align: power of 2, >= sizeof(void*) */
void vsdlss_parallel_observe(void); /* master thread only */
/* NUMA interleave of large arrays (see vsdlss_parallel.c): active when
 * VSDLSS_NUMA_INTERLEAVE is not 0 (default on) and the machine has more
 * than one memory node.  vsdlss_numa_spread interleaves the whole pages of
 * a range of at least 1 MiB; move also migrates pages already placed.  The
 * vsdlss_big_* allocators apply it.  Tests: vsdlss_numa_interleave (-1 =
 * environment, 0, 1, 2 = also on a one-node machine) and the byte count
 * vsdlss_numa_spread_bytes. */
int vsdlss_numa_interleave_active(void);
void vsdlss_numa_spread(void *p, size_t bytes, int move);
extern int vsdlss_numa_interleave;
extern long vsdlss_numa_spread_bytes;
/* Placement by user (VSDLSS_NUMA_PLACE=1, default off; tests:
 * vsdlss_numa_place -1/0/1, with vsdlss_numa_interleave = 2 also on one
 * node): vsdlss_numa_bind prefers node for the whole pages of a range of at
 * least 1 MiB and moves them; vsdlss_numa_node_here is the calling thread's
 * node (-1 unknown). */
int vsdlss_numa_place_active(void);
int vsdlss_numa_node_here(void);
void vsdlss_numa_bind(void *p, size_t bytes, int node);
extern int vsdlss_numa_place;
extern long vsdlss_numa_place_bytes;
#endif
