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
#if defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#endif

/* NUMA interleave of the large arrays (VSDLSS_NUMA_INTERLEAVE, default on
 * when the machine has more than one NUMA node with memory; 0 turns it
 * off).  mbind(MPOL_INTERLEAVE) spreads the pages over the nodes so both
 * sockets' memory channels serve a solve, and gives the range an explicit
 * policy, which the kernel's automatic NUMA balancing leaves alone: on the
 * 61M-node case (dual EPYC, numa_balancing=1) a solve step took 563 ms with
 * first-touch pages, about 610K minor faults per step from the balancing,
 * and 151 ms interleaved.  Only where the memory goes changes, never a
 * result.  Linux only; elsewhere and on one node a no-op. */
int vsdlss_numa_interleave=-1;
long vsdlss_numa_spread_bytes;
#if defined(__linux__) && defined(SYS_mbind)
#define NUMA_MASK_WORDS 16
static unsigned long numa_mask[NUMA_MASK_WORDS];
static int numa_nodes=-1;
static int numa_parse(const char *path)
{
    FILE *fp=fopen(path,"r"); char line[1024]; int count=0;
    if(!fp) return -1;
    if(!fgets(line,sizeof line,fp)) { fclose(fp); return -1; }
    fclose(fp);
    for(char *t=line;*t && *t!='\n';) {
        char *e; long a=strtol(t,&e,10), b=a;
        if(e==t) break;
        if(*e=='-') { t=e+1; b=strtol(t,&e,10); }
        for(long k=a;k<=b && k<NUMA_MASK_WORDS*64;k++)
            if(k>=0) { numa_mask[k/64]|=1UL<<(k%64); count++; }
        t=*e==','?e+1:e;
    }
    return count;
}
static int numa_count(void)
{
    int c=__atomic_load_n(&numa_nodes,__ATOMIC_ACQUIRE);
    if(c>=0) return c;
    memset(numa_mask,0,sizeof numa_mask);
    c=numa_parse("/sys/devices/system/node/has_memory");
    if(c<0) { memset(numa_mask,0,sizeof numa_mask); c=numa_parse("/sys/devices/system/node/online"); }
    if(c<0) c=0;
    __atomic_store_n(&numa_nodes,c,__ATOMIC_RELEASE);
    return c;
}
#endif
int vsdlss_numa_interleave_active(void)
{
#if defined(__linux__) && defined(SYS_mbind)
    int on=__atomic_load_n(&vsdlss_numa_interleave,__ATOMIC_RELAXED);
    if(on<0) {
        const char *e=getenv("VSDLSS_NUMA_INTERLEAVE");
        on=!(e && *e=='0');
        __atomic_store_n(&vsdlss_numa_interleave,on,__ATOMIC_RELAXED);
    }
    return on==2 ? numa_count()>0 : on && numa_count()>1;      /* 2: also on one node (tests) */
#else
    return 0;
#endif
}
/* Interleave the whole pages of [p, p+bytes) (at least 1 MiB) over the
 * memory nodes.  move: also migrate pages already placed (one-time cost,
 * for arrays filled before the call); otherwise only pages touched later
 * follow the policy. */
void vsdlss_numa_spread(void *p,size_t bytes,int move)
{
#if defined(__linux__) && defined(SYS_mbind)
    if(!p || bytes<((size_t)1<<20) || !vsdlss_numa_interleave_active()) return;
    const uintptr_t pg=(uintptr_t)sysconf(_SC_PAGESIZE);
    uintptr_t a=((uintptr_t)p+pg-1)&~(pg-1), e=((uintptr_t)p+bytes)&~(pg-1);
    if(e<=a) return;
    /* MPOL_INTERLEAVE = 3, MPOL_MF_MOVE = 2 (linux/mempolicy.h) */
    if(syscall(SYS_mbind,(void*)a,(unsigned long)(e-a),3,numa_mask,
               (unsigned long)(NUMA_MASK_WORDS*64),move?2u:0u)==0)
        __atomic_fetch_add(&vsdlss_numa_spread_bytes,(long)(e-a),__ATOMIC_RELAXED);
#else
    (void)p;(void)bytes;(void)move;
#endif
}

#define BIG_HINT_BYTES ((size_t)8<<20)
static void big_hint(void *p,size_t bytes)
{
    vsdlss_numa_spread(p,bytes,0);     /* before the pages are touched */
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
