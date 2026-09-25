#define _DEFAULT_SOURCE
#include "vsdlss_parallel.h"
#include <stdint.h>
#include <stdlib.h>
#if defined(__linux__)
#include <sys/mman.h>
#endif
#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
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
size_t vsdlss_llc_bytes(void)
{
#if defined(_SC_LEVEL3_CACHE_SIZE)
    long l3=sysconf(_SC_LEVEL3_CACHE_SIZE);
    if(l3>0) return (size_t)l3;
#endif
    return 0;
}
void *vsdlss_big_malloc(size_t bytes){void *p=malloc(bytes);big_hint(p,bytes);return p;}
void *vsdlss_big_calloc(size_t count,size_t size)
{
    void *p=calloc(count,size);
    if(p&&size&&count<=SIZE_MAX/size)big_hint(p,count*size);
    return p;
}
static _Thread_local int requested=1;
static _Thread_local int observed=1;
int vsdlss_parallel_enabled(void)
{
#ifdef _OPENMP
    return 1;
#else
    return 0;
#endif
}
vsdlss_status vsdlss_set_num_threads(int threads)
{
    if(threads<1||threads>1024)return VSDLSS_ERR_INVALID;
    if(threads>1&&!vsdlss_parallel_enabled())return VSDLSS_ERR_UNSUPPORTED;
    requested=threads;observed=1;return VSDLSS_OK;
}
int vsdlss_get_num_threads(void){return requested;}
int vsdlss_parallel_last_team_size(void){return observed;}
static _Thread_local int budget=0;
int vsdlss_parallel_set_budget(int threads){int old=budget;budget=threads;return old;}
int vsdlss_parallel_width(double work)
{
#ifdef _OPENMP
    if(work<100000.0)return 1;
    if(!omp_in_parallel())return requested;
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
