/* Standalone gather/write-back benchmark; no matrix, factor or solve.
 * ./bench_perm2_only [nodes=32000000] [threads=4] [repeats=8]
 * The random global<->packed bijection models shuffled VDD/GND IDs.
 * Bucket size and pass order match the M3 two-pass permutation path.
 * Plan construction, allocation and correctness checks are outside timings.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#define SHIFT 15
#define MASK ((1u << SHIFT) - 1u)
typedef struct {
    uint32_t n, buckets;
    int threads;
    uint32_t *p2g, *g2p;
    uint16_t *offset_g, *offset_w;
    size_t *pos_g, *pos_w, *start_g, *start_w, *cursor;
} plan;

static double seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}
static uint64_t state=0x9e3779b97f4a7c15ULL;
static uint64_t random64(void) {
    state ^= state << 13; state ^= state >> 7; state ^= state << 17;
    return state;
}
static void *allocate(size_t count, size_t width) {
    if (count && width > SIZE_MAX/count) return NULL;
    return malloc(count*width);
}
static int make_plan(plan *p) {
    const uint32_t n=p->n, b=p->buckets;
    const int T=p->threads;
    const size_t slots=(size_t)b*(size_t)T;
    p->p2g=allocate(n,sizeof(uint32_t)); p->g2p=allocate(n,sizeof(uint32_t));
    p->offset_g=allocate(n,sizeof(uint16_t)); p->offset_w=allocate(n,sizeof(uint16_t));
    p->pos_g=calloc(slots,sizeof(size_t)); p->pos_w=calloc(slots,sizeof(size_t));
    p->start_g=allocate(b+1,sizeof(size_t)); p->start_w=allocate(b+1,sizeof(size_t));
    p->cursor=allocate(slots,sizeof(size_t));
    if (!p->p2g||!p->g2p||!p->offset_g||!p->offset_w||!p->pos_g||
        !p->pos_w||!p->start_g||!p->start_w||!p->cursor) return 0;
    for (uint32_t i=0;i<n;i++) p->p2g[i]=i;
    for (uint32_t i=n-1;i>0;i--) {
        uint32_t j=(uint32_t)(random64()%((uint64_t)i+1));
        uint32_t x=p->p2g[i];p->p2g[i]=p->p2g[j];p->p2g[j]=x;
    }
    for (uint32_t i=0;i<n;i++) p->g2p[p->p2g[i]]=i;
    const uint32_t chunk=(n+(uint32_t)T-1)/(uint32_t)T;
    for (int t=0;t<T;t++) {
        uint32_t lo=(uint64_t)t*chunk<n?t*chunk:n;
        uint32_t hi=(uint64_t)lo+chunk<n?lo+chunk:n;
        size_t *cg=p->pos_g+(size_t)t*b,*cw=p->pos_w+(size_t)t*b;
        for (uint32_t g=lo;g<hi;g++) cg[p->g2p[g]>>SHIFT]++;
        for (uint32_t i=lo;i<hi;i++) cw[p->p2g[i]>>SHIFT]++;
    }
    size_t at_g=0,at_w=0;
    for (uint32_t k=0;k<b;k++) {
        p->start_g[k]=at_g;p->start_w[k]=at_w;
        for (int t=0;t<T;t++) {
            size_t at=(size_t)t*b+k, ng=p->pos_g[at],nw=p->pos_w[at];
            p->pos_g[at]=at_g;at_g+=ng;
            p->pos_w[at]=at_w;at_w+=nw;
        }
    }
    p->start_g[b]=at_g;p->start_w[b]=at_w;
    if (at_g!=n||at_w!=n) return 0;
    memcpy(p->cursor,p->pos_g,slots*sizeof(size_t));
    for (int t=0;t<T;t++) {
        uint32_t lo=(uint64_t)t*chunk<n?t*chunk:n;
        uint32_t hi=(uint64_t)lo+chunk<n?lo+chunk:n;
        size_t *q=p->cursor+(size_t)t*b;
        for (uint32_t g=lo;g<hi;g++) {
            uint32_t i=p->g2p[g];p->offset_g[q[i>>SHIFT]++]=i&MASK;
        }
    }
    memcpy(p->cursor,p->pos_w,slots*sizeof(size_t));
    for (int t=0;t<T;t++) {
        uint32_t lo=(uint64_t)t*chunk<n?t*chunk:n;
        uint32_t hi=(uint64_t)lo+chunk<n?lo+chunk:n;
        size_t *q=p->cursor+(size_t)t*b;
        for (uint32_t i=lo;i<hi;i++) {
            uint32_t g=p->p2g[i];p->offset_w[q[g>>SHIFT]++]=g&MASK;
        }
    }
    return 1;
}
static void direct_gather(const plan *p,const double *global,double *packed) {
    const uint32_t n=p->n;
    #ifdef _OPENMP
    #pragma omp parallel for num_threads(p->threads) schedule(static)
    #endif
    for (uint32_t i=0;i<n;i++) packed[i]=global[p->p2g[i]];
}
static void direct_write(const plan *p,const double *packed,double *global) {
    const uint32_t n=p->n;
    #ifdef _OPENMP
    #pragma omp parallel for num_threads(p->threads) schedule(static)
    #endif
    for (uint32_t g=0;g<n;g++) global[g]=packed[p->g2p[g]];
}
static int blocked_gather(plan *p,const double *global,double *packed,double *scratch) {
    const uint32_t n=p->n,b=p->buckets,chunk=(n+(uint32_t)p->threads-1)/(uint32_t)p->threads;
    int bad=0;
    #ifdef _OPENMP
    #pragma omp parallel num_threads(p->threads) reduction(|:bad)
    #endif
    {
        int t=0;
        #ifdef _OPENMP
        t=omp_get_thread_num();
        #endif
        uint32_t lo=(uint64_t)t*chunk<n?t*chunk:n;
        uint32_t hi=(uint64_t)lo+chunk<n?lo+chunk:n;
        size_t *q=p->cursor+(size_t)t*b;
        memcpy(q,p->pos_g+(size_t)t*b,(size_t)b*sizeof(size_t));
        for (uint32_t g=lo;g<hi;g++) scratch[q[p->g2p[g]>>SHIFT]++]=global[g];
        #ifdef _OPENMP
        #pragma omp barrier
        #pragma omp for schedule(dynamic,4)
        #endif
        for (uint32_t k=0;k<b;k++) {
            double *dst=packed+((size_t)k<<SHIFT);
            for (size_t j=p->start_g[k];j<p->start_g[k+1];j++) {
                double value=scratch[j];dst[p->offset_g[j]]=value;bad|=!isfinite(value);
            }
        }
    }
    return bad;
}
static void blocked_write(plan *p,const double *packed,double *global,double *scratch) {
    const uint32_t n=p->n,b=p->buckets,chunk=(n+(uint32_t)p->threads-1)/(uint32_t)p->threads;
    #ifdef _OPENMP
    #pragma omp parallel num_threads(p->threads)
    #endif
    {
        int t=0;
        #ifdef _OPENMP
        t=omp_get_thread_num();
        #endif
        uint32_t lo=(uint64_t)t*chunk<n?t*chunk:n;
        uint32_t hi=(uint64_t)lo+chunk<n?lo+chunk:n;
        size_t *q=p->cursor+(size_t)t*b;
        memcpy(q,p->pos_w+(size_t)t*b,(size_t)b*sizeof(size_t));
        for (uint32_t i=lo;i<hi;i++) scratch[q[p->p2g[i]>>SHIFT]++]=packed[i];
        #ifdef _OPENMP
        #pragma omp barrier
        #pragma omp for schedule(dynamic,4)
        #endif
        for (uint32_t k=0;k<b;k++) {
            double *dst=global+((size_t)k<<SHIFT);
            for (size_t j=p->start_w[k];j<p->start_w[k+1];j++)
                dst[p->offset_w[j]]=scratch[j];
        }
    }
}
static int compare(const double *a,const double *b,uint32_t n) {
    return memcmp(a,b,(size_t)n*sizeof(double))==0;
}
static double median(double *values,int n) {
    for (int i=1;i<n;i++) for (int j=i;j>0&&values[j]<values[j-1];j--) {
        double t=values[j];values[j]=values[j-1];values[j-1]=t;
    }
    return n&1?values[n/2]:0.5*(values[n/2-1]+values[n/2]);
}
int main(int argc,char **argv) {
    unsigned long long count=argc>1?strtoull(argv[1],NULL,10):32000000ULL;
    int threads=argc>2?atoi(argv[2]):4,repeats=argc>3?atoi(argv[3]):8;
    if (count<2||count>UINT32_MAX-1||threads<1||threads>64||repeats<2||repeats>50) {
        fprintf(stderr,"usage: %s [nodes] [threads=4] [repeats=8]\n",argv[0]);return 2;
    }
    #ifdef _OPENMP
    omp_set_dynamic(0);
    #else
    if (threads!=1) { fprintf(stderr,"build without OpenMP requires threads=1\n");return 2; }
    #endif
    plan p={.n=(uint32_t)count,.buckets=((uint32_t)count>>SHIFT)+1,.threads=threads};
    double *global=allocate(p.n,sizeof(double)),*packed=allocate(p.n,sizeof(double));
    double *direct_p=allocate(p.n,sizeof(double)),*direct_g=allocate(p.n,sizeof(double));
    double *written=allocate(p.n,sizeof(double)),*scratch=allocate(p.n,sizeof(double));
    double *tg=allocate(repeats,sizeof(double)),*tw=allocate(repeats,sizeof(double));
    double *bg=allocate(repeats,sizeof(double)),*bw=allocate(repeats,sizeof(double));
    if (!global||!packed||!direct_p||!direct_g||!written||!scratch||!tg||!tw||!bg||!bw||!make_plan(&p)) {
        fprintf(stderr,"allocation/plan failed\n");return 1;
    }
    for (uint32_t i=0;i<p.n;i++) global[i]=(double)i+0.125;
    direct_gather(&p,global,direct_p);
    if (blocked_gather(&p,global,packed,scratch)||!compare(direct_p,packed,p.n)) {
        fprintf(stderr,"gather mismatch\n");return 1;
    }
    direct_write(&p,packed,direct_g);
    blocked_write(&p,packed,written,scratch);
    if (!compare(global,direct_g,p.n)||!compare(direct_g,written,p.n)) {
        fprintf(stderr,"write-back mismatch\n");return 1;
    }
    for (int i=0;i<repeats;i++) {
        double t=seconds();
        if (i&1) { if(blocked_gather(&p,global,packed,scratch)) return 1; }
        else direct_gather(&p,global,direct_p);
        double elapsed=seconds()-t;
        if (i&1) bg[i]=elapsed;else tg[i]=elapsed;
        t=seconds();
        if (i&1) direct_gather(&p,global,direct_p);
        else if(blocked_gather(&p,global,packed,scratch)) return 1;
        elapsed=seconds()-t;
        if (i&1) tg[i]=elapsed;else bg[i]=elapsed;
        t=seconds();
        if (i&1) { blocked_write(&p,packed,written,scratch); }
        else direct_write(&p,direct_p,direct_g);
        elapsed=seconds()-t;
        if (i&1) bw[i]=elapsed;else tw[i]=elapsed;
        t=seconds();
        if (i&1) direct_write(&p,direct_p,direct_g);
        else blocked_write(&p,packed,written,scratch);
        elapsed=seconds()-t;
        if (i&1) tw[i]=elapsed;else bw[i]=elapsed;
    }
    if (!compare(direct_p,packed,p.n)||!compare(global,direct_g,p.n)||!compare(global,written,p.n)) {
        fprintf(stderr,"final mismatch\n");return 1;
    }
    printf("n=%u threads=%d repeats=%d permutation=random bucket=%u checked=bitwise\n",
           p.n,threads,repeats,1u<<SHIFT);
    printf("gather_direct_ms=%.3f gather_two_pass_ms=%.3f write_direct_ms=%.3f write_two_pass_ms=%.3f\n",
           1000*median(tg,repeats),1000*median(bg,repeats),1000*median(tw,repeats),1000*median(bw,repeats));
    free(global);free(packed);free(direct_p);free(direct_g);free(written);free(scratch);
    free(tg);free(tw);free(bg);free(bw);
    free(p.p2g);free(p.g2p);free(p.offset_g);free(p.offset_w);
    free(p.pos_g);free(p.pos_w);free(p.start_g);free(p.start_w);free(p.cursor);
    return 0;
}
