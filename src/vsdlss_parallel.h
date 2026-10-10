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
/* Set by the threads of a team that runs OpenMP tasks and has members
 * waiting for them (the tree top of a tree-parallel solve): a kernel called
 * there may split a large step into child tasks instead of opening a team.
 * Thread-local; returns the previous value. */
int vsdlss_parallel_set_tasks(int on);
int vsdlss_parallel_tasks(void);
/* malloc / calloc for large arrays (>= 8 MiB): the fresh pages are marked
 * for transparent huge pages, which cuts first-touch page faults by up to
 * 512x and TLB misses in random access.  A hint only (no-op elsewhere;
 * VSDLSS_NO_HUGEPAGES=1 disables it); free() as usual. */
#include <stddef.h>
void *vsdlss_big_malloc(size_t bytes);
void *vsdlss_big_calloc(size_t count, size_t size);
void *vsdlss_big_malloc_aligned(size_t align, size_t bytes);  /* align: power of 2, >= sizeof(void*) */
void vsdlss_parallel_observe(void); /* master thread only */
#endif
