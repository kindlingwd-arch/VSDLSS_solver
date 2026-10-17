#define _GNU_SOURCE
#include "vsdlss_parallel.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#if defined(__linux__)
#include <sched.h>
#endif
#if defined(__linux__)
#include <sys/mman.h>
#endif
#define BIG_HINT_BYTES ((size_t)8<<20)
static void big_hint(void *p,size_t bytes)
{
#if defined(__linux__) && defined(MADV_HUGEPAGE)
    static int enabled=-1;         /* VSDLSS_NO_HUGEPAGES=1 turns the hint off */
    int on=__atomic_load_n(&enabled,__ATOMIC_RELAXED);
    if(on<0){const char *e=getenv("VSDLSS_NO_HUGEPAGES");on=!(e&&*e&&*e!='0');
        __atomic_store_n(&enabled,on,__ATOMIC_RELAXED);}
    if(!on||!p||bytes<BIG_HINT_BYTES)return;
    uintptr_t a=((uintptr_t)p+4095)&~(uintptr_t)4095,e=((uintptr_t)p+bytes)&~(uintptr_t)4095;
    if(e>a)(void)madvise((void*)a,e-a,MADV_HUGEPAGE);
#else
    (void)p;(void)bytes;
#endif
}
void *vsdlss_big_malloc(size_t bytes){void *p=malloc(bytes);big_hint(p,bytes);return p;}
void *vsdlss_big_malloc_aligned(size_t align,size_t bytes)
{void *p=NULL;if(posix_memalign(&p,align,bytes?bytes:align))return NULL;big_hint(p,bytes);return p;}
void *vsdlss_big_calloc(size_t count,size_t size)
{
    void *p=calloc(count,size);
    if(p&&size&&count<=SIZE_MAX/size)big_hint(p,count*size);
    return p;
}
/* Thread count: per calling thread; 0 until vsdlss_set_num_threads is called
 * on it, then the default below applies (VSDLSS_NUM_THREADS, else 1). */
static _Thread_local int requested=0;
static _Thread_local int observed=1;
int vsdlss_parallel_enabled(void)
{
#ifdef _OPENMP
    return 1;
#else
    return 0;
#endif
}
#if defined(__linux__) && defined(_OPENMP)
/* Physical cores among the CPUs this process may run on (sysfs topology):
 * a CPU counts unless the lowest of its hardware-thread siblings is also
 * allowed.  0 when unknown. */
static int physical_cores(void)
{
    cpu_set_t set;
    if(sched_getaffinity(0,sizeof set,&set)) return 0;
    int n=0;
    for(int c=0;c<CPU_SETSIZE;c++){
        if(!CPU_ISSET(c,&set)) continue;
        char path[96]; int first=-1;
        snprintf(path,sizeof path,"/sys/devices/system/cpu/cpu%d/topology/thread_siblings_list",c);
        FILE *f=fopen(path,"r");
        if(!f) return 0;
        if(fscanf(f,"%d",&first)!=1) first=-1;
        fclose(f);
        if(first<0) return 0;
        if(first==c||first>=CPU_SETSIZE||!CPU_ISSET(first,&set)) n++;
    }
    return n;
}
#endif
/* "Automatic": the OpenMP default team size, which follows OMP_NUM_THREADS
 * and the process's CPU affinity (taskset, cgroup cpusets), capped on Linux
 * at the physical cores it may run on: the solve is bandwidth bound and
 * hardware threads only compete for the same memory traffic (laptop, 4M
 * nodes: 16 logical CPUs 94-99 ms per solve, 4 threads 64-66 ms).
 * Computed once; 1 without OpenMP. */
static int auto_threads(void)
{
#ifdef _OPENMP
    static int cached=0;
    int t=__atomic_load_n(&cached,__ATOMIC_RELAXED);
    if(t<1){
        t=omp_get_max_threads();
#if defined(__linux__)
        int pc=physical_cores();
        if(pc>0&&pc<t) t=pc;
#endif
        t=t<1?1:t>1024?1024:t;
        __atomic_store_n(&cached,t,__ATOMIC_RELAXED);
    }
    return t;
#else
    return 1;
#endif
}
/* VSDLSS_NUM_THREADS = n or "auto" (also 0): the count of threads that never
 * called vsdlss_set_num_threads.  Unset or invalid: 1.  Read once. */
static int default_threads(void)
{
    static int cached=0;
    int v=__atomic_load_n(&cached,__ATOMIC_RELAXED);
    if(v<1){
        const char *e=getenv("VSDLSS_NUM_THREADS");
        char *end=NULL; long n;
        if(!e||!*e) v=1;
        else if(!strcmp(e,"auto")) v=auto_threads();
        else if((n=strtol(e,&end,10)),end!=e&&*end==0&&n==0) v=auto_threads();
        else v=end!=e&&*end==0&&n>=1&&n<=1024?(int)n:1;
        if(v>1&&!vsdlss_parallel_enabled()) v=1;
        __atomic_store_n(&cached,v,__ATOMIC_RELAXED);
    }
    return v;
}
static inline int threads_now(void){return requested>0?requested:default_threads();}
vsdlss_status vsdlss_set_num_threads(int threads)
{
    if(threads==0) threads=auto_threads();
    if(threads<1||threads>1024)return VSDLSS_ERR_INVALID;
    if(threads>1&&!vsdlss_parallel_enabled())return VSDLSS_ERR_UNSUPPORTED;
    requested=threads;observed=1;return VSDLSS_OK;
}
int vsdlss_get_num_threads(void){return threads_now();}
int vsdlss_parallel_last_team_size(void){return observed;}
static _Thread_local int budget=0;
int vsdlss_parallel_set_budget(int threads){int old=budget;budget=threads;return old;}
int vsdlss_parallel_width(double work)
{
#ifdef _OPENMP
    if(work<100000.0)return 1;
    if(!omp_in_parallel())return threads_now();
    if(budget>1&&omp_get_active_level()<omp_get_max_active_levels())return budget;
#else
    (void)work;
#endif
    return 1;
}
void vsdlss_parallel_observe(void)
{
#ifdef _OPENMP
    int team=omp_get_num_threads();if(team>observed)observed=team;
#endif
}

static _Thread_local int dag_enabled=0;
void vsdlss_set_dag_enabled(int enabled){dag_enabled=enabled!=0;}
int vsdlss_get_dag_enabled(void){return dag_enabled;}
