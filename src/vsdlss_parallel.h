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
/* Task teams.  The threads of a team that runs OpenMP tasks (the tree top of
 * a tree-parallel solve) point at the team's count of threads that are busy
 * with work of their own; a kernel called there may then hand part of a
 * large step to the others as child tasks instead of opening a team.
 * Thread-local; set returns the previous pointer (NULL: not a task team). */
#include <stdatomic.h>
atomic_int *vsdlss_parallel_set_tasks(atomic_int *busy);
/* 1 when the calling thread is in a task team. */
int vsdlss_parallel_tasks(void);
/* Threads of the caller's task team that are free to take a child task now
 * (0 outside a task team): handing work to a busy team only makes the
 * caller wait for it. */
int vsdlss_parallel_helpers(void);
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
