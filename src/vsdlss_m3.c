#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#if defined(__GLIBC__)
#include <malloc.h>
#endif
#if defined(__SSE2__)
#include <emmintrin.h>
#endif

/* VSDLSS_TRACE=1 prints the facade's phase times to stderr. */
static double trace_now(void)
{
#ifdef _OPENMP
    return omp_get_wtime();
#else
    return (double)clock()/CLOCKS_PER_SEC;
#endif
}
static int trace_on(void)
{
    static int cached=-1;
    if(cached<0){const char *e=getenv("VSDLSS_TRACE");cached=e&&*e&&*e!='0';}
    return cached;
}
#define TRACE(label,t0) do{ if(trace_on()){ double t1_=trace_now(); \
    fprintf(stderr,"vsdlss trace: %-22s %8.3f s\n",label,t1_-(t0)); (t0)=t1_; } }while(0)

/* Memory freed by the factorization's temporary structures (adjacency
 * lists, ordering workspaces, symbolic analysis) stays in glibc's
 * per-thread arenas unless handed back: measured 26% of the process after
 * factoring an 8M-node grid, 2.5 GB on a 30M-node one, still resident
 * during every later solve.  Level (VSDLSS_TRIM): 0 never, 1 when the
 * factorization ends, 2 (default) also between phases, before each
 * component's factor L is allocated, which lowers the peak.  Other C
 * libraries: no-op. */
static int trim_level(void)
{
    static int level=-1;
    if(level<0) { const char *e=getenv("VSDLSS_TRIM"); level=e&&*e?atoi(e):2; }
    return level;
}
static void release_free_memory(int level)
{
#if defined(__GLIBC__)
    if(trim_level()>=level) {
        double t0=trace_now();
        malloc_trim(0);
        if(trace_on()) fprintf(stderr,"vsdlss trace: malloc_trim (level %d)   %8.3f s\n",level,trace_now()-t0);
    }
#else
    (void)level;
#endif
}

/* Components smaller than this keep the ascending numbering. */
csi vsdlss_reorder_min = 4096;
#define VSDLSS_REORDER_MIN vsdlss_reorder_min
/* Tests only: use 64-bit inverse-map codes even when 32 bits would do. */
int vsdlss_m3_inverse_force64 = 0;

static int count_fits(csi n, size_t width)
{
    return n >= 0 && (uint64_t)n <= SIZE_MAX / width;
}

static void perm_plan_free(struct vsdlss_perm_plan *p);

void vsdlss_m3_factor_free(vsdlss_m3_factor *factor)
{
    csi k;
    if(!factor) return;
    if(factor->component) for(k=0;k<factor->count;k++) {
        vsdlss_m4_factor_free(factor->component[k].disk);
        vsdlss_reduction_free(factor->component[k].reduction);
        free(factor->component[k].gather);
        free(factor->component[k].ws_local);
        free(factor->component[k].ws_saved);
        free(factor->component[k].ws_core);
        free(factor->component[k].core_map);
        vsdlss_sn_factor_free(factor->component[k].numeric);
    }
    free(factor->component);
    vsdlss_components_free(factor->components);
    free(factor->inv32); free(factor->inv64);
    perm_plan_free(atomic_load(&factor->pplan));
    free(factor->pbuf); free(factor->pscr_block);
    free(factor);
}

/* ---- Scheduling of independent components --------------------------------
 * Components with at least 1/(4T) of the vertices ("large": e.g. separate
 * VDD and GND nets) run concurrently, each on a share of the T threads
 * proportional to its size, as a nested team (a single large component gets
 * all T threads).  The other components run in one flat loop, one thread
 * each.  A component's arithmetic never depends on its thread count, so
 * results are the same for every schedule. */
typedef vsdlss_status (*component_fn)(void *ctx, csi component);

static csi comp_size(const vsdlss_m3_factor *f, csi c)
{ return f->components->offset[c+1]-f->components->offset[c]; }

#define MAX_TEAM 1024
static void run_components(const vsdlss_m3_factor *f, int serial, component_fn fn,
                           void *ctx, vsdlss_status *res)
{
    const csi count=f->count, n=f->n;
    int T=serial?1:vsdlss_parallel_width((double)n*256);
    csi big[MAX_TEAM+1], nbig=0; int share[MAX_TEAM]; double frac[MAX_TEAM];
    if(count==1 || T<=1) {                  /* inner code uses all T threads */
        for(csi c=0;c<count;c++) res[c]=fn(ctx,c);
        return;
    }
    if(T>MAX_TEAM) T=MAX_TEAM;
    csi limit=n/(4*(csi)T); if(limit<1) limit=1;
    for(csi c=0;c<count && nbig<=T;c++) if(comp_size(f,c)>=limit) {
        csi at=nbig++;             /* insertion by size, descending; ties by index */
        while(at>0 && comp_size(f,big[at-1])<comp_size(f,c)) { big[at]=big[at-1]; at--; }
        big[at]=c;
    }
    if(nbig>T) nbig=0;                        /* many similar pieces: flat loop */
    if(nbig==1) {
        res[big[0]]=fn(ctx,big[0]);
    } else if(nbig>1) {
        /* Largest-remainder shares of T, at least one thread each. */
        csi total=0; int given=0;
        for(csi i=0;i<nbig;i++) total+=comp_size(f,big[i]);
        for(csi i=0;i<nbig;i++) {
            double exact=(double)T*(double)comp_size(f,big[i])/(double)total;
            share[i]=(int)exact; frac[i]=exact-share[i]; given+=share[i];
        }
        while(given<T) {
            csi best=0;
            for(csi i=1;i<nbig;i++) if(frac[i]>frac[best]) best=i;
            share[best]++; given++; frac[best]=-1.0;
        }
        for(csi i=0;i<nbig;i++) while(share[i]<1) {
            csi most=0; for(csi j=1;j<nbig;j++) if(share[j]>share[most]) most=j;
            share[most]--; share[i]++;
        }
#ifdef _OPENMP
        int levels=omp_get_max_active_levels();
        if(levels<2) omp_set_max_active_levels(2);
#if _OPENMP < 201811
        /* Before OpenMP 5.0 (e.g. GCC <= 10) nested teams also need
         * nest-var; without it every component team silently runs on one
         * thread.  Newer runtimes derive it from max-active-levels. */
        int nested=omp_get_nested();
        if(!nested) omp_set_nested(1);
#endif
#endif
        VSDLSS_OMP(omp parallel for num_threads((int)nbig) schedule(static,1))
        for(csi i=0;i<nbig;i++) {
            int prev=vsdlss_parallel_set_budget(share[i]);
            if(i==0) vsdlss_parallel_observe();
            res[big[i]]=fn(ctx,big[i]);
            vsdlss_parallel_set_budget(prev);
        }
#ifdef _OPENMP
        if(levels<2) omp_set_max_active_levels(levels);
#if _OPENMP < 201811
        if(!nested) omp_set_nested(0);
#endif
#endif
    }
    if(nbig==count) return;
    /* Everything not run above: exactly the components below the limit when
     * a large set was run, otherwise all of them. */
    VSDLSS_OMP(omp parallel num_threads(T))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(dynamic,1))
        for(csi c=0;c<count;c++) if(!nbig || comp_size(f,c)<limit) res[c]=fn(ctx,c);
    }
}

/* Phase 1: this component's reduction input, built straight from the
 * weighted graph (no CSC copy of the component).  Large components are
 * renumbered in BFS order; small ones keep the ascending numbering.  (The
 * CSC extraction path remains for callers without a graph.) */
typedef struct {
    vsdlss_m3_factor *factor; const vsdlss *src; const vsdlss_wgraph *graph;
    csi *newidx; vsdlss_reduce_input **inputs; int order;
} factor_ctx;

static vsdlss_status prepare_component(void *vctx, csi component)
{
    factor_ctx *x=(factor_ctx*)vctx;
    vsdlss_m3_factor *factor=x->factor;
    vsdlss_m3_component_factor *cf=factor->component+component;
    const csi begin=factor->components->offset[component];
    vsdlss *local=NULL; vsdlss_status st;
    vsdlss_reduce_input **input=x->inputs+component;
    cf->n=factor->components->offset[component+1]-begin;
    if(x->graph && factor->components->order && cf->n>=VSDLSS_REORDER_MIN) {
        const csi *perm=factor->components->order+begin;
        cf->gather=(csi*)vsdlss_big_malloc((size_t)cf->n*sizeof(csi));
        if(!cf->gather) return VSDLSS_ERR_OOM;
        for(csi k=0;k<cf->n;k++){
            csi g=factor->components->vertices[begin+perm[k]];
            cf->gather[k]=g; x->newidx[g]=k;     /* this component's entries only */
        }
        return vsdlss_reduce_prepare_graph(x->graph->ptr,x->graph->idx,x->graph->val,x->graph->diag,
                                           cf->gather,cf->n,x->newidx,input);
    }
    if(x->graph)       /* ascending numbering: local_of is already the new index */
        return vsdlss_reduce_prepare_graph(x->graph->ptr,x->graph->idx,x->graph->val,x->graph->diag,
                                           factor->components->vertices+begin,cf->n,
                                           factor->components->local_of,input);
    st=vsdlss_component_extract_normalized(x->src,factor->components,component,&local);
    if(st!=VSDLSS_OK) return st;
    st=vsdlss_reduce_prepare_csc(local,input);
    vsdlss_spfree(local);
    return st;
}

/* VSDLSS_SN_REORDER=1: reorder columns inside supernodes before the numeric
 * factorization (contiguous descendant rows for the solves; changes the
 * rounding, not the fill).  Off by default while it is being evaluated. */
static int sn_reorder_on(void)
{
    static int on=-1;
    if(on<0) { const char *e=getenv("VSDLSS_SN_REORDER"); on=e && e[0]=='1'; }
    return on;
}

/* Phase 2: reduce (consuming the prepared input), order and factor the core. */
static vsdlss_status factor_component(void *vctx, csi component)
{
    factor_ctx *x=(factor_ctx*)vctx;
    vsdlss_m3_factor *factor=x->factor;
    vsdlss_m3_component_factor *cf=factor->component+component;
    vsdlss *permuted=NULL; csi *pinv=NULL; vsdlss_sn_symbolic *symbolic=NULL;
    double t0=trace_now();
    vsdlss_reduce_ws ws;
    vsdlss_status status=vsdlss_reduce_run_packed(x->inputs[component],&cf->reduction,&ws);
    x->inputs[component]=NULL;
    if(status!=VSDLSS_OK) goto done;
    atomic_init(&cf->ws_busy,0);
    cf->ws_local=ws.local; cf->ws_saved=ws.saved; cf->ws_core=ws.core;
    TRACE("low-degree reduction",t0);
    if(cf->reduction->core_n && !factor->disk_mode) {
        vsdlss_reduction *r=cf->reduction; csi *q=NULL;
        status=vsdlss_order(r->core,x->order,&q,&pinv);
        cf->core_map=q;
        if(status!=VSDLSS_OK) goto done;
        TRACE("core ordering",t0);
        status=vsdlss_postorder_permutation(r->core,q,pinv);
        if(status!=VSDLSS_OK) goto done;
        permuted=vsdlss_symperm(r->core,pinv,1);
        if(!permuted) {status=VSDLSS_ERR_OOM;goto done;}
        /* The solve reads core unknown k at local vertex core_vertices[q[k]]. */
        for(csi k=0;k<r->core_n;k++) q[k]=r->core_vertices[q[k]];
        vsdlss_spfree(r->core); r->core=NULL;   /* only the disk mode reads it later */
        release_free_memory(2);                  /* ordering workspace, before L */
        /* Strict supernodes (no amalgamation zeros) were measured on the
         * 16M-node EMIR case: 11% fewer stored entries but no faster solve
         * and a slower factorization, so the relaxed layout stays. */
        status=vsdlss_sn_analyze_relaxed(permuted,&symbolic);
        if(status!=VSDLSS_OK) goto done;
        if(sn_reorder_on() && symbolic->count>1) {
            /* Columns reordered inside supernodes (vsdlss_sn_reorder.c):
             * same fill and operations, contiguous descendant rows. */
            const csi cn=r->core_n; csi *p=NULL;
            if(trace_on()) fprintf(stderr,"vsdlss trace: supernodes before reordering %lld\n",(long long)symbolic->count);
            status=vsdlss_sn_reorder_within(symbolic,&p);
            if(status==VSDLSS_ERR_UNSUPPORTED) status=VSDLSS_OK;   /* too large: keep the order */
            else if(status!=VSDLSS_OK) goto done;
            if(p) {
                csi *pi=(csi*)malloc((size_t)cn*sizeof(csi)), *q2=(csi*)malloc((size_t)cn*sizeof(csi));
                vsdlss *p2=NULL;
                if(pi && q2) {
                    for(csi k=0;k<cn;k++) { pi[p[k]]=k; q2[k]=q[p[k]]; }
                    p2=vsdlss_symperm(permuted,pi,1);
                }
                free(pi); free(p);
                if(!p2) { free(q2); status=VSDLSS_ERR_OOM; goto done; }
                memcpy(q,q2,(size_t)cn*sizeof(csi)); free(q2);
                vsdlss_spfree(permuted); permuted=p2;
                vsdlss_sn_symbolic_free(symbolic); symbolic=NULL;
                status=vsdlss_sn_analyze_relaxed(permuted,&symbolic);
                if(status!=VSDLSS_OK) goto done;
                TRACE("supernode reordering",t0);
            }
        }
        if(trace_on()) fprintf(stderr,"vsdlss trace: core L stored %lld, amalgamation zeros %lld (%.1f%%), supernodes %lld\n",
                               (long long)symbolic->l_nnz,(long long)symbolic->relaxed_zeros,
                               100.0*(double)symbolic->relaxed_zeros/(double)(symbolic->l_nnz?symbolic->l_nnz:1),
                               (long long)symbolic->count);
        TRACE("core symbolic",t0);
        status=vsdlss_sn_factorize(permuted,symbolic,&cf->numeric);
        TRACE("core numeric",t0);
    }
done:
    free(pinv); vsdlss_sn_symbolic_free(symbolic); vsdlss_spfree(permuted);
    return status;
}

static vsdlss_status build_inverse(vsdlss_m3_factor *f);

static vsdlss_status factorize_shared(const vsdlss *A, int order,
                                  vsdlss_m3_factor **out,int disk_mode,size_t budget,const char *directory)
{
    vsdlss_m3_factor *factor=NULL; vsdlss *normalized=NULL;
    vsdlss_status status; csi component;
    vsdlss_status *results=NULL; vsdlss_reduce_input **inputs=NULL;
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
#if defined(VSDLSS_KAHIP)
    if(order<0 || order>7) return VSDLSS_ERR_UNSUPPORTED;   /* 7: KaHIP ND on the core */
#elif defined(VSDLSS_METIS)
    if(order<0 || order>6) return VSDLSS_ERR_UNSUPPORTED;   /* 6: METIS on the core */
#else
    if(order<0 || order>5) return VSDLSS_ERR_UNSUPPORTED;
#endif
    if(!A) return VSDLSS_ERR_INVALID;
    if(A->n==INT64_MAX || A->n<1 || !count_fits(A->n+1,sizeof(csi)) ||
       !count_fits(A->n,sizeof(double))) return A->n<1?VSDLSS_ERR_INVALID:VSDLSS_ERR_OOM;
    double t0=trace_now();
    const vsdlss *src=A; vsdlss_wgraph *graph=NULL;
    /* Already-normalized input (sorted, no duplicates: the usual case) is
     * used in place; only otherwise is a normalized copy made. */
    status=vsdlss_validate_upper_csc(A);
    if(status!=VSDLSS_OK) return status;
    if(!vsdlss_is_normalized_upper(A)) {
        status=vsdlss_normalize_upper(A,&normalized);
        if(status!=VSDLSS_OK) return status;
        src=normalized;
    }
    TRACE("normalize",t0);
    factor=(vsdlss_m3_factor*)calloc(1,sizeof(*factor));
    if(!factor) {status=VSDLSS_ERR_OOM;goto fail;}
    factor->n=src->n; factor->disk_mode=disk_mode;
    status=vsdlss_components_build_graph(src,&factor->components,&graph);
    if(status!=VSDLSS_OK) goto fail;
    /* The weighted graph holds everything the components need from here
     * on, so a normalized copy of A is dropped now (peak memory). */
    vsdlss_spfree(normalized); normalized=NULL; src=NULL;
    TRACE("components",t0);
    factor->count=factor->components->count;
    if(!count_fits(factor->count,sizeof(*factor->component))) {status=VSDLSS_ERR_OOM;goto fail;}
    factor->component=(vsdlss_m3_component_factor*)calloc((size_t)factor->count,sizeof(*factor->component));
    if(!factor->component) {status=VSDLSS_ERR_OOM;goto fail;}
    results=calloc((size_t)factor->count,sizeof(*results));
    inputs=calloc((size_t)factor->count,sizeof(*inputs));
    /* newidx (graph vertex -> new index) is written only for vertices of
     * renumbered components, whose local_of entries no later step reads (small
     * components read local_of only for their own vertices), so it shares that
     * array instead of allocating another n-length one. */
    factor_ctx ctx={factor,src,graph,factor->components->local_of,inputs,order};
    if(!results||!inputs||!ctx.newidx){status=VSDLSS_ERR_OOM;goto fail;}
    /* Prepare every component, then drop the graph, the normalized copy and
     * the vertex maps no later phase reads, so the reduction's working set
     * does not coexist with them (peak memory on large grids). */
    run_components(factor,disk_mode,prepare_component,&ctx,results);
    for(component=0;component<factor->count;component++)if(results[component]!=VSDLSS_OK){
        status=results[component];goto fail;
    }
    TRACE("renumber+adjacency",t0);
    vsdlss_wgraph_free(graph); graph=NULL; ctx.graph=NULL; ctx.newidx=NULL;
    free(factor->components->order); factor->components->order=NULL;
    free(factor->components->component_of); factor->components->component_of=NULL;
    free(factor->components->local_of); factor->components->local_of=NULL;
    release_free_memory(2);                      /* adjacency graph */
    run_components(factor,disk_mode,factor_component,&ctx,results);
    for(component=0;component<factor->count;component++)if(results[component]!=VSDLSS_OK){
        status=results[component];goto fail;
    }
    free(results); results=NULL; free(inputs); inputs=NULL;
    if(disk_mode) {
        csi cores=0;
        for(component=0;component<factor->count;component++)
            if(factor->component[component].reduction->core_n)cores++;
        size_t remaining=budget;
        for(component=0;component<factor->count;component++) {
            vsdlss_m3_component_factor *cf=&factor->component[component];
            if(!cf->reduction->core_n)continue;
            status=vsdlss_factorize_m4(cf->reduction->core,order,remaining/(size_t)cores,directory,&cf->disk);
            if(status!=VSDLSS_OK)goto fail;
            size_t used=vsdlss_m4_workspace_bytes(cf->disk);
            if(used>remaining){status=VSDLSS_ERR_OOM;goto fail;}
            remaining-=used;cores--;
        }
    }
    status=build_inverse(factor);
    if(status!=VSDLSS_OK) goto fail;
    release_free_memory(1);
    TRACE("release free memory",t0);
    *out=factor; return VSDLSS_OK;
fail:
    if(inputs) for(component=0;component<factor->count;component++) vsdlss_reduce_input_free(inputs[component]);
    free(results); free(inputs);
    vsdlss_wgraph_free(graph);
    vsdlss_spfree(normalized); vsdlss_m3_factor_free(factor); return status;
}

/* ---- Solve ------------------------------------------------------------------
 * Every component's buffers are taken first (the factor's cached workspace,
 * or private buffers when a concurrent solve holds it), so nothing can fail
 * for lack of memory once work starts.  Phase 1 computes each component's
 * solution in its local buffer: phase 1a gathers (checking the RHS is
 * finite), eliminates the low-degree vertices forward and solves the core
 * for every component, then phase 1b recovers the eliminated vertices.  Only when
 * every component succeeded does phase 2 scatter the local solutions into
 * the caller's array, so a failed solve leaves it untouched, and rhs may be
 * the same array as solution (every component has read its RHS by then). */
typedef struct { double *local, *saved, *core, *core_x; int own; } solve_ws;

/* Solve-time state of the fused passes (see solve_common). */
typedef struct {
    const struct vsdlss_perm_plan *pl; double *buf, *scr;
    int P; atomic_int *busy;       /* scratch slots and their flags */
    int streamed;
} fused_ctx;
/* Fused parts of the two-pass gather / write-back (defined with them). */
static int fused_forward(const vsdlss_m3_factor *f, const fused_ctx *z, csi c, double *local);
static int fused_backward(const vsdlss_m3_factor *f, const fused_ctx *z, csi c, double *local);
static int fused_component(const fused_ctx *z, csi c);

/* internal: rhs and out are in the factor's internal order (component c's
 * local vector at components->offset[c]), so gather and write-back are
 * sequential copies.  loc: each component's local buffer, read by the
 * sequential write-back through the inverse map. */
typedef struct {
    const vsdlss_m3_factor *f; const double *rhs; double *out; solve_ws *ws;
    int internal; double **loc;
    int pregathered;            /* local buffers already filled (two-pass gather) */
    const fused_ctx *z;         /* fused passes, or NULL */
} solve_ctx;

static const csi *component_map(const vsdlss_m3_factor *f, csi c)
{
    const vsdlss_m3_component_factor *cf=f->component+c;
    return cf->gather?cf->gather:f->components->vertices+f->components->offset[c];
}

/* inv[g] = (c << shift) | i for global vertex g = component_map(c)[i]. */
static vsdlss_status build_inverse(vsdlss_m3_factor *f)
{
    const csi n=f->n, count=f->count; csi largest=1; int bi=0, bc=0;
    for(csi c=0;c<count;c++) if(f->component[c].n>largest) largest=f->component[c].n;
    while(bi<62 && ((csi)1<<bi)<largest) bi++;
    while(bc<62 && ((csi)1<<bc)<count) bc++;
    if(bi+bc>63) return VSDLSS_OK;            /* not encodable: keep the scatter */
    f->inv_shift=bi;
    if(bi+bc<=32 && !vsdlss_m3_inverse_force64) f->inv32=(uint32_t*)vsdlss_big_malloc((size_t)n*sizeof(uint32_t));
    else f->inv64=(uint64_t*)vsdlss_big_malloc((size_t)n*sizeof(uint64_t));
    if(!f->inv32 && !f->inv64) return VSDLSS_ERR_OOM;
    int T=vsdlss_parallel_width((double)n*4); (void)T;
    for(csi c=0;c<count;c++) {
        const csi cn=f->component[c].n, *map=component_map(f,c);
        const uint64_t hi=(uint64_t)c<<bi;
        int t=cn>=65536?T:1; (void)t;
        if(f->inv32) {
            uint32_t *inv=f->inv32;
            VSDLSS_OMP(omp parallel for num_threads(t) if(t>1) schedule(static))
            for(csi i=0;i<cn;i++) inv[map[i]]=(uint32_t)(hi|(uint64_t)i);
        } else {
            uint64_t *inv=f->inv64;
            VSDLSS_OMP(omp parallel for num_threads(t) if(t>1) schedule(static))
            for(csi i=0;i<cn;i++) inv[map[i]]=hi|(uint64_t)i;
        }
    }
    return VSDLSS_OK;
}

static void ws_release(const vsdlss_m3_factor *f, csi c, solve_ws *w)
{
    vsdlss_m3_component_factor *cf=(vsdlss_m3_component_factor *)(f->component+c);
    if(w->own) atomic_store(&cf->ws_busy,0);
    else { free(w->local); free(w->saved); free(w->core); }
    free(w->core_x);
    memset(w,0,sizeof(*w));
}

static vsdlss_status ws_acquire(const vsdlss_m3_factor *f, csi c, solve_ws *w)
{
    vsdlss_m3_component_factor *cf=(vsdlss_m3_component_factor *)(f->component+c);
    const vsdlss_reduction *r=cf->reduction; const csi core=r->core_n;
    const int need_saved=r->count && r->records;     /* packed replays keep it in place */
    memset(w,0,sizeof(*w));
    if(!count_fits(cf->n,sizeof(double)) || !count_fits(core,sizeof(double)) ||
       !count_fits(r->count,sizeof(double))) return VSDLSS_ERR_OOM;
    if(cf->ws_local && atomic_exchange(&cf->ws_busy,1)==0) {
        w->own=1; w->local=cf->ws_local; w->saved=cf->ws_saved; w->core=cf->ws_core;
    } else {
        w->local=(double*)malloc((size_t)(cf->n?cf->n:1)*sizeof(double));
        if(need_saved) w->saved=(double*)malloc((size_t)r->count*sizeof(double));
        if(core) w->core=(double*)malloc((size_t)core*sizeof(double));
    }
    if(core&&cf->disk) w->core_x=(double*)malloc((size_t)core*sizeof(double));
    if(!w->local||(need_saved&&!w->saved)||(core&&!w->core)||(core&&cf->disk&&!w->core_x)) {
        ws_release(f,c,w); return VSDLSS_ERR_OOM;
    }
    return VSDLSS_OK;
}

/* Phase 1a for one component (all buffers private): gather, forward
 * replay, core solve. */
static vsdlss_status solve_local(void *vctx, csi c)
{
    const solve_ctx *x=(const solve_ctx*)vctx;
    const vsdlss_m3_component_factor *cf=x->f->component+c;
    const vsdlss_reduction *r=cf->reduction;
    const csi core=r->core_n, cn=cf->n;
    solve_ws *w=x->ws+c;
    double *local=w->local, *core_b=w->core;
    vsdlss_status status; int bad=0;
    int gt=vsdlss_parallel_width((double)cn*4); (void)gt;
    double t0=trace_now();
    if(x->internal) {
        const double *src=x->rhs+x->f->components->offset[c];
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static) reduction(|:bad))
        for(csi i=0;i<cn;i++) { double v=src[i]; local[i]=v; bad|=!isfinite(v); }
    } else if(!x->pregathered) {
        const csi *map=component_map(x->f,c); const double *rhs=x->rhs;
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static) reduction(|:bad))
        for(csi i=0;i<cn;i++) { double v=rhs[map[i]]; local[i]=v; bad|=!isfinite(v); }
    }
    if(bad) return VSDLSS_ERR_NONFINITE;
    TRACE("solve: gather",t0);
    const int fused=fused_component(x->z,c);
    if(fused) {               /* this component's gather pass 2 with the replay */
        if(fused_forward(x->f,x->z,c,local)) return VSDLSS_ERR_NONFINITE;
    } else {
        status=vsdlss_reduce_forward_inplace(r,local,w->saved);   /* NULL when packed */
        if(status!=VSDLSS_OK) return status;
    }
    TRACE("solve: reduce forward",t0);
    if(core) {
        if(cf->disk) {
            for(csi k=0;k<core;k++) core_b[k]=local[r->core_vertices[k]];
            status=vsdlss_m4_solve(cf->disk,core_b,w->core_x);
            if(status!=VSDLSS_OK) return status;
            for(csi k=0;k<core;k++) local[r->core_vertices[k]]=w->core_x[k];
        } else {
            /* In place on the private core buffer.  A non-finite core RHS or
             * intermediate leaves a non-finite entry in the result, so the
             * write-back's scan reports it as vsdlss_sn_solve's scans did. */
            const csi *cm=cf->core_map;
            int ct=vsdlss_parallel_width((double)core*4); (void)ct;
            VSDLSS_OMP(omp parallel for num_threads(ct) if(ct>1) schedule(static))
            for(csi k=0;k<core;k++) core_b[k]=local[cm[k]];
            status=vsdlss_sn_solve_inplace(cf->numeric,core_b);
            if(status!=VSDLSS_OK) return status;
            VSDLSS_OMP(omp parallel for num_threads(ct) if(ct>1) schedule(static) reduction(|:bad))
            for(csi k=0;k<core;k++) { double v=core_b[k]; local[cm[k]]=v; bad|=!isfinite(v); }
            if(bad) return VSDLSS_ERR_NONFINITE;
        }
    }
    TRACE("solve: core",t0);
    return VSDLSS_OK;
}

/* Phase 1b for one component: backward replay.  It runs after every
 * component's phase 1a because a fused write-back pass 1 fills the same
 * bucket buffer that the fused gathers read. */
static vsdlss_status solve_local_back(void *vctx, csi c)
{
    const solve_ctx *x=(const solve_ctx*)vctx;
    const vsdlss_m3_component_factor *cf=x->f->component+c;
    const vsdlss_reduction *r=cf->reduction;
    solve_ws *w=x->ws+c;
    vsdlss_status status;
    double t0=trace_now();
    /* The in-memory core write-back of phase 1a checked every core value. */
    if(fused_component(x->z,c)) status=fused_backward(x->f,x->z,c,w->local)?VSDLSS_OK:VSDLSS_ERR_NONFINITE;
    else status=r->core_n&&!cf->disk?vsdlss_reduce_backward_core_checked(r,w->saved,w->local)
                                    :vsdlss_reduce_backward_inplace(r,w->saved,w->local);
    TRACE("solve: reduce backward",t0);
    return status;
}

/* Phase 2 for one component: scatter to the caller's numbering, or a
 * sequential copy in internal order. */
static vsdlss_status scatter_local(void *vctx, csi c)
{
    const solve_ctx *x=(const solve_ctx*)vctx;
    const csi cn=x->f->component[c].n;
    const double *local=x->ws[c].local;
    int gt=vsdlss_parallel_width((double)cn*4); (void)gt;
    if(x->internal) {
        double *out=x->out+x->f->components->offset[c];
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static))
        for(csi i=0;i<cn;i++) out[i]=local[i];
    } else {
        const csi *map=component_map(x->f,c); double *out=x->out;
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static))
        for(csi i=0;i<cn;i++) out[map[i]]=local[i];
    }
    return VSDLSS_OK;
}

/* Phase 2 through the inverse map: the caller's array is written in order
 * (streaming stores), the component buffers are read at random.  Random
 * writes cost a read of the whole cache line first, random reads do not. */
static void write_back_inverse(const solve_ctx *x)
{
    const vsdlss_m3_factor *f=x->f; const csi n=f->n;
    double *out=x->out; double *const *loc=x->loc;
    const int sh=f->inv_shift;
    int T=vsdlss_parallel_width((double)n*4); (void)T;
    if(f->inv32) {
        const uint32_t *inv=f->inv32, mask=(uint32_t)(((uint64_t)1<<sh)-1);
        if(f->count==1) {
            const double *l0=loc[0];
            VSDLSS_OMP(omp parallel for num_threads(T) if(T>1) schedule(static))
            for(csi g=0;g<n;g++) out[g]=l0[inv[g]&mask];
        } else {
            VSDLSS_OMP(omp parallel for num_threads(T) if(T>1) schedule(static))
            for(csi g=0;g<n;g++) { uint32_t u=inv[g]; out[g]=loc[u>>sh][u&mask]; }
        }
    } else {
        const uint64_t *inv=f->inv64, mask=((uint64_t)1<<sh)-1;
        VSDLSS_OMP(omp parallel for num_threads(T) if(T>1) schedule(static))
        for(csi g=0;g<n;g++) { uint64_t u=inv[g]; out[g]=loc[u>>sh][u&mask]; }
    }
}

/* ---- Blocked two-pass permutations -------------------------------------------
 * The original-order solve moves n doubles through a random permutation
 * twice: gather (caller order -> the components' local buffers) and
 * write-back (local buffers -> caller order).  Done directly, each element
 * is one random memory access and the loops are latency bound.  Two passes
 * make both sides sequential: pass 1 streams the source and appends each
 * value to the bucket of its destination block (2^sh doubles, 256 KB, cache
 * resident); pass 2 fills one destination block per bucket.  Pure copies:
 * results are bitwise those of the direct loops.  VSDLSS_PERM2=0 disables.
 *
 * Internal side: the local buffers are laid end to end in a padded order
 * (component c at poff[c]).  A component whose low-degree reduction is
 * packed and blocked is "fused": it starts on a chunk boundary (so does the
 * component after it), so each of its chunks and buckets holds only its own
 * entries.  Its gather pass 2 runs inside its solve, block by block, right
 * before the forward replay of the block (the block is then still in cache),
 * and its write-back pass 1 right after the backward replay of the block.
 * Everything else takes both passes globally.
 *
 * The plan (built on first use, kept with the factor) depends only on the
 * permutation: sources are cut into chunks of CI entries (whole reduction
 * blocks), and slots are laid out bucket-major, each (chunk, bucket) pair
 * a contiguous range, so chunks can be scheduled dynamically by any team.
 * For each source element it keeps its bucket (16 bits), and for each slot
 * the destination offset in its bucket (16 bits).
 *
 * Memory traffic (bandwidth bound): pass 1 stages each bucket's current
 * 64-byte line and writes full lines with non-temporal stores; global pass
 * 2 assembles a block in a cache-resident scratch and streams it out, so
 * neither the bucket buffer nor the destination is read before it is
 * written.  Streaming is for vectors larger than the cache only
 * (stream_min_bytes).  Measurements: docs/reconstruction/
 * solve-nonkernel-20260925.md. */
#define PERM_SH 15
/* Smallest n using the two-pass path (tests lower it). */
csi vsdlss_perm2_min = (csi)1<<20;
#define PERM_MIN vsdlss_perm2_min
typedef struct vsdlss_perm_plan {
    int sh; csi G, CI;           /* bucket 2^sh = G entries; source chunk CI (a multiple of G) */
    csi np;                      /* padded internal length */
    csi *poff;                   /* count+1: padded start of each component */
    csi *frank;                  /* count: rank among fused components, -1 if not fused */
    csi nf;                      /* fused components */
    csi Sg, Bp, Sp, Bg, maxB;    /* gather: Sg global chunks -> Bp padded buckets; write-back: Sp -> Bg */
    csi *posG, *posW;            /* Sg*Bp, Sp*Bg: first slot of (chunk, bucket) */
    csi *bstartG, *bstartW;      /* Bp+1, Bg+1: first slot of each bucket */
    uint16_t *bktG, *offG;       /* n: padded bucket of each global entry; offset of each slot */
    uint16_t *bktW, *offW;       /* np: global bucket of each padded entry; n: offset of each slot */
} vsdlss_perm_plan;

static void perm_plan_free(struct vsdlss_perm_plan *p)
{
    if(!p) return;
    free(p->poff); free(p->frank); free(p->posG); free(p->posW); free(p->bstartG); free(p->bstartW);
    free(p->bktG); free(p->offG); free(p->bktW); free(p->offW); free(p);
}

static int perm2_on(void)
{
    static int on=-1;
    if(on<0) { const char *e=getenv("VSDLSS_PERM2"); on=!(e && e[0]=='0'); }
    return on;
}

/* Component and local index of global vertex g. */
static inline void inv_of(const vsdlss_m3_factor *f, csi g, csi *c, csi *i)
{
    const int sh=f->inv_shift;
    if(f->inv32) { uint32_t u=f->inv32[g]; *c=(csi)(u>>sh); *i=(csi)(u&(uint32_t)(((uint64_t)1<<sh)-1)); }
    else { uint64_t u=f->inv64[g]; *c=(csi)(u>>sh); *i=(csi)(u&(((uint64_t)1<<sh)-1)); }
}
/* Last component starting at or before padded position p. */
static csi comp_at(const vsdlss_perm_plan *pl, csi count, csi p)
{
    const csi *off=pl->poff; csi lo=0, hi=count-1;
    while(lo<hi) { csi mid=lo+(hi-lo+1)/2; if(off[mid]<=p) lo=mid; else hi=mid-1; }
    return lo;
}

static vsdlss_perm_plan *perm_plan_build(const vsdlss_m3_factor *f)
{
    const csi n=f->n, count=f->count;
    vsdlss_perm_plan *pl=(vsdlss_perm_plan*)calloc(1,sizeof(*pl));
    if(!pl) return NULL;
    pl->poff=(csi*)malloc((size_t)(count+1)*sizeof(csi));
    pl->frank=(csi*)malloc((size_t)count*sizeof(csi));
    if(!pl->poff||!pl->frank) { perm_plan_free(pl); return NULL; }
    /* Bucket size: 2^15, or the reduction block when that is a smaller
     * power of two (tests); fused components need blocks of whole buckets. */
    csi G=(csi)1<<PERM_SH, bsf=0;
    for(csi c=0;c<count;c++) {
        csi bs=0;
        if(!f->disk_mode && vsdlss_reduce_fused_blocks(f->component[c].reduction,&bs) && !(bs&(bs-1)) && bs<G) G=bs;
    }
    for(csi c=0;c<count;c++) {
        csi bs=0; pl->frank[c]=-1;
        if(f->disk_mode || !vsdlss_reduce_fused_blocks(f->component[c].reduction,&bs) || bs%G) continue;
        if(bsf && bs!=bsf) continue;             /* one chunk size for all fused components */
        bsf=bs; pl->frank[c]=pl->nf++;
    }
    int sh=0; while(((csi)1<<sh)<G) sh++;
    pl->sh=sh; pl->G=G;
    /* Chunks of k reduction blocks, k doubling while the slot tables would
     * exceed n/4 entries (they are Sg*Bp and Sp*Bg). */
    csi CI=bsf?bsf:G;
    for(;;) {
        csi at=0;
        for(csi c=0;c<count;c++) {
            if(pl->frank[c]>=0 || (c>0 && pl->frank[c-1]>=0)) at=(at+CI-1)/CI*CI;
            pl->poff[c]=at; at+=f->component[c].n;
        }
        pl->poff[count]=at; pl->np=at;
        pl->Sg=(n+CI-1)/CI; pl->Bg=(n+G-1)/G; pl->Sp=(at+CI-1)/CI; pl->Bp=(at+G-1)/G;
        const double cells=(double)pl->Sg*(double)pl->Bp+(double)pl->Sp*(double)pl->Bg;
        if(cells<=(double)(n/2>(csi)1<<16?n/2:(csi)1<<16) || CI>=((csi)1<<40)) break;
        CI*=2;
    }
    pl->CI=CI;
    if(pl->Bp>65536 || pl->Bg>65536) { perm_plan_free(pl); return NULL; }   /* 16-bit buckets */
    pl->maxB=pl->Bp>pl->Bg?pl->Bp:pl->Bg;
    const csi Sg=pl->Sg, Bp=pl->Bp, Sp=pl->Sp, Bg=pl->Bg, np=pl->np, *poff=pl->poff;
    pl->posG=(csi*)calloc((size_t)Sg*(size_t)Bp,sizeof(csi)); pl->posW=(csi*)calloc((size_t)Sp*(size_t)Bg,sizeof(csi));
    pl->bstartG=(csi*)malloc((size_t)(Bp+1)*sizeof(csi)); pl->bstartW=(csi*)malloc((size_t)(Bg+1)*sizeof(csi));
    pl->bktG=(uint16_t*)vsdlss_big_malloc((size_t)n*sizeof(uint16_t)); pl->offG=(uint16_t*)vsdlss_big_malloc((size_t)n*sizeof(uint16_t));
    pl->bktW=(uint16_t*)vsdlss_big_malloc((size_t)(np?np:1)*sizeof(uint16_t)); pl->offW=(uint16_t*)vsdlss_big_malloc((size_t)n*sizeof(uint16_t));
    if(!pl->posG||!pl->posW||!pl->bstartG||!pl->bstartW||!pl->bktG||!pl->offG||!pl->bktW||!pl->offW) {
        perm_plan_free(pl); return NULL;
    }
    int T=vsdlss_parallel_width((double)n*4); (void)T;
    /* Counts per (chunk, bucket); every chunk is walked in ascending source
     * order, here and in the passes. */
    VSDLSS_OMP(omp parallel for num_threads(T) if(T>1) schedule(dynamic,1))
    for(csi s=0;s<Sg;s++) {
        const csi lo=s*CI, hi=lo+CI<n?lo+CI:n; csi *row=pl->posG+(size_t)s*Bp;
        for(csi g=lo;g<hi;g++) { csi c,i; inv_of(f,g,&c,&i); csi b=(poff[c]+i)>>sh; pl->bktG[g]=(uint16_t)b; row[b]++; }
    }
    VSDLSS_OMP(omp parallel for num_threads(T) if(T>1) schedule(dynamic,1))
    for(csi s=0;s<Sp;s++) {
        const csi lo=s*CI, hi=lo+CI<np?lo+CI:np; csi *row=pl->posW+(size_t)s*Bg;
        for(csi c=comp_at(pl,count,lo);c<count && poff[c]<hi;c++) {
            const csi a=poff[c]>lo?poff[c]:lo, e=poff[c]+f->component[c].n<hi?poff[c]+f->component[c].n:hi;
            const csi *map=component_map(f,c);
            for(csi p=a;p<e;p++) { csi b=map[p-poff[c]]>>sh; pl->bktW[p]=(uint16_t)b; row[b]++; }
        }
    }
    csi at=0;
    for(csi b=0;b<Bp;b++) { pl->bstartG[b]=at; for(csi s=0;s<Sg;s++) { csi *v=pl->posG+(size_t)s*Bp+b, k=*v; *v=at; at+=k; } }
    pl->bstartG[Bp]=at;
    at=0;
    for(csi b=0;b<Bg;b++) { pl->bstartW[b]=at; for(csi s=0;s<Sp;s++) { csi *v=pl->posW+(size_t)s*Bg+b, k=*v; *v=at; at+=k; } }
    pl->bstartW[Bg]=at;
    int failed=0;
    VSDLSS_OMP(omp parallel num_threads(T) if(T>1) reduction(|:failed))
    {
        csi *q=(csi*)malloc((size_t)pl->maxB*sizeof(csi));
        if(!q) failed=1;
        VSDLSS_OMP(omp for schedule(dynamic,1))
        for(csi s=0;s<Sg;s++) {
            if(!q) continue;
            const csi lo=s*CI, hi=lo+CI<n?lo+CI:n;
            memcpy(q,pl->posG+(size_t)s*Bp,(size_t)Bp*sizeof(csi));
            for(csi g=lo;g<hi;g++) { csi c,i; inv_of(f,g,&c,&i); pl->offG[q[pl->bktG[g]]++]=(uint16_t)((poff[c]+i)&(G-1)); }
        }
        VSDLSS_OMP(omp for schedule(dynamic,1))
        for(csi s=0;s<Sp;s++) {
            if(!q) continue;
            const csi lo=s*CI, hi=lo+CI<np?lo+CI:np;
            memcpy(q,pl->posW+(size_t)s*Bg,(size_t)Bg*sizeof(csi));
            for(csi c=comp_at(pl,count,lo);c<count && poff[c]<hi;c++) {
                const csi a=poff[c]>lo?poff[c]:lo, e=poff[c]+f->component[c].n<hi?poff[c]+f->component[c].n:hi;
                const csi *map=component_map(f,c);
                for(csi p=a;p<e;p++) { csi g=map[p-poff[c]]; pl->offW[q[pl->bktW[p]]++]=(uint16_t)(g&(G-1)); }
            }
        }
        free(q);
    }
    if(failed) { perm_plan_free(pl); return NULL; }
    return pl;
}

static const vsdlss_perm_plan *perm_plan_get(const vsdlss_m3_factor *fc)
{
    vsdlss_m3_factor *f=(vsdlss_m3_factor*)fc;
    vsdlss_perm_plan *pl=atomic_load(&f->pplan);
    if(pl) return pl;
    pl=perm_plan_build(f);
    if(!pl) return NULL;
    vsdlss_perm_plan *expect=NULL;
    if(!atomic_compare_exchange_strong(&f->pplan,&expect,pl)) { perm_plan_free(pl); pl=expect; }
    return pl;
}

#define PERM_LINE 8                                 /* doubles per 64-byte line */
/* Non-temporal stores (SSE2); plain stores elsewhere.  stream_fence orders
 * a thread's streamed stores before the barrier that publishes them. */
static inline void stream_line(double *dst, const double *src)   /* dst 64-byte aligned */
{
#if defined(__SSE2__)
    _mm_stream_pd(dst,_mm_load_pd(src)); _mm_stream_pd(dst+2,_mm_load_pd(src+2));
    _mm_stream_pd(dst+4,_mm_load_pd(src+4)); _mm_stream_pd(dst+6,_mm_load_pd(src+6));
#else
    memcpy(dst,src,PERM_LINE*sizeof(double));
#endif
}
static void stream_copy(double *dst, const double *src, csi len)
{
#if defined(__SSE2__)
    csi i=0;
    if(((uintptr_t)dst&15) && len>0) { dst[0]=src[0]; i=1; }
    if(((uintptr_t)(dst+i)&15)==0) {
        for(;i+2<=len;i+=2) _mm_stream_pd(dst+i,_mm_loadu_pd(src+i));
    }
    for(;i<len;i++) dst[i]=src[i];
#else
    memcpy(dst,src,(size_t)len*sizeof(double));
#endif
}
static inline void stream_fence(void)
{
#if defined(__SSE2__)
    _mm_sfence();
#endif
}

/* Vectors of at least this many bytes are moved with non-temporal stores
 * (default: twice the last-level cache, 64 MB when unknown; the environment
 * variable VSDLSS_STREAM_MIN overrides it).  Smaller ones stay in cache
 * for the loop that reads them next, so plain stores are better there. */
double vsdlss_perm2_stream_min = -1;         /* < 0: as above (tests set it) */
static double stream_min_bytes(void)
{
    static double v=-1;
    if(vsdlss_perm2_stream_min>=0) return vsdlss_perm2_stream_min;
    if(v<0) {
        const char *e=getenv("VSDLSS_STREAM_MIN");
        const size_t l3=vsdlss_llc_bytes();
        double m=l3?2.0*(double)l3:64.0*1024*1024;
        if(e && *e) m=atof(e);
        v=m;
    }
    return v;
}

/* Scratch slots (P of them): thread t of the global passes uses slot t,
 * the fused write-back takes slots from the pool.  Each: maxB staging
 * lines, one block, maxB queue counters. */
static size_t slot_stride(const vsdlss_perm_plan *pl)
{
    size_t d=(size_t)pl->maxB*PERM_LINE+(size_t)pl->G+(size_t)pl->maxB;
    return (d+PERM_LINE-1)/PERM_LINE*PERM_LINE;
}
typedef struct { double *stage, *blk; csi *q; } perm_slot;
static perm_slot slot_at(const vsdlss_perm_plan *pl, double *scr, size_t k)
{
    perm_slot s;
    s.stage=scr+k*slot_stride(pl); s.blk=s.stage+(size_t)pl->maxB*PERM_LINE; s.q=(csi*)(s.blk+pl->G);
    return s;
}

/* Scratch vector of np doubles and P scratch slots (64-byte aligned); NULL
 * when a concurrent solve holds them. */
static double *pbuf_acquire(const vsdlss_m3_factor *fc, const vsdlss_perm_plan *pl, int P)
{
    vsdlss_m3_factor *f=(vsdlss_m3_factor*)fc;
    const size_t need=(size_t)P*slot_stride(pl);
    if(atomic_exchange(&f->pbuf_busy,1)) return NULL;
    if(!f->pbuf) f->pbuf=(double*)vsdlss_big_malloc((size_t)(pl->np?pl->np:1)*sizeof(double));
    if(f->pbuf && f->pscr_len<need) {
        free(f->pscr_block); f->pscr_block=NULL; f->pscr=NULL; f->pscr_len=0;
        f->pscr_block=malloc(need*sizeof(double)+64);
        if(f->pscr_block) {
            f->pscr=(double*)(((uintptr_t)f->pscr_block+63)&~(uintptr_t)63);
            f->pscr_len=need;
        }
    }
    if(!f->pbuf || !f->pscr) { atomic_store(&f->pbuf_busy,0); return NULL; }
    return f->pbuf;
}
static void pbuf_release(const vsdlss_m3_factor *fc)
{ atomic_store(&((vsdlss_m3_factor*)fc)->pbuf_busy,0); }

/* Pass 1 of both directions, over a run of consecutive source chunks:
 * p1_feed appends src[i-soff] for i in [lo, hi) to the buckets,
 * buf[q[bkt[i]]++], through one staging line per bucket.  The slots of one
 * bucket are contiguous across consecutive chunks, so a run carries its
 * lines from chunk to chunk.  first[b] is the run's first slot of bucket b:
 * a line that starts before it is shared with another run, so only this
 * run's slots of it are stored (plainly); p1_flush stores the lines left
 * partial at the end of the run (plainly) and fences the streamed ones. */
static void p1_feed(const double *restrict src, csi soff, const uint16_t *restrict bkt, csi lo, csi hi,
                    double *restrict buf, csi *restrict q, const csi *restrict first,
                    double *restrict stage, int streamed)
{
    /* a0: slot index of the first 64-byte boundary in buf, modulo 8. */
    const csi a0=(csi)(((64-((uintptr_t)buf&63))&63)/sizeof(double));
    if(!streamed) {                                   /* plain stores */
        for(csi i=lo;i<hi;i++) buf[q[bkt[i]]++]=src[i-soff];
        return;
    }
    for(csi i=lo;i<hi;i++) {
        const csi b=bkt[i], pos=q[b]++, sl=(pos-a0)&(PERM_LINE-1);
        double *st=stage+(size_t)b*PERM_LINE;
        st[sl]=src[i-soff];
        if(sl==PERM_LINE-1) {
            const csi ls=pos-(PERM_LINE-1);
            if(ls>=first[b]) stream_line(buf+ls,st);
            else for(csi k=first[b];k<=pos;k++) buf[k]=st[(k-a0)&(PERM_LINE-1)];
        }
    }
}
static void p1_flush(double *restrict buf, const csi *restrict q, const csi *restrict first, csi B,
                     const double *restrict stage, int streamed)
{
    if(!streamed) return;
    const csi a0=(csi)(((64-((uintptr_t)buf&63))&63)/sizeof(double));
    for(csi b=0;b<B;b++) {
        const csi end=q[b], part=(end-a0)&(PERM_LINE-1);
        csi k=end-part; if(k<first[b]) k=first[b];
        const double *st=stage+(size_t)b*PERM_LINE;
        for(;k<end;k++) buf[k]=st[(k-a0)&(PERM_LINE-1)];
    }
    stream_fence();
}

/* This thread's share [*lo, *hi) of m items split evenly among T. */
static void share_of(csi m, int t, int T, csi *lo, csi *hi)
{ *lo=m*(csi)t/(csi)T; *hi=m*(csi)(t+1)/(csi)T; }

/* Feeds internal chunk s of the write-back to pass 1 (the components
 * overlapping it; a fused component's chunk holds only its own entries). */
static void writeback_chunk(const vsdlss_m3_factor *f, const vsdlss_perm_plan *pl, double *buf,
                            double *const *loc, csi s, csi *q, const csi *first, double *stage, int streamed)
{
    const csi lo=s*pl->CI, hi=lo+pl->CI<pl->np?lo+pl->CI:pl->np, *poff=pl->poff;
    for(csi c=comp_at(pl,f->count,lo);c<f->count && poff[c]<hi;c++) {
        const csi a=poff[c]>lo?poff[c]:lo, e=poff[c]+f->component[c].n<hi?poff[c]+f->component[c].n:hi;
        if(a<e) p1_feed(loc[c],poff[c],pl->bktW,a,e,buf,q,first,stage,streamed);
    }
}

/* Gather, global part: pass 1 for all entries, pass 2 for the buckets of
 * components that are not fused.  loc[c][i] = rhs[component_map(c)[i]];
 * returns 1 when a gathered value is not finite (the direct gather's check). */
static int perm2_gather(const vsdlss_m3_factor *f, const vsdlss_perm_plan *pl, const double *rhs,
                        double *buf, double *const *loc, double *scr, int T, int streamed)
{
    const csi n=f->n, Bp=pl->Bp, CI=pl->CI, G=pl->G, *poff=pl->poff;
    int bad=0;
    (void)T;
    VSDLSS_OMP(omp parallel num_threads(T) reduction(|:bad))
    {
#ifdef _OPENMP
        const int t=omp_get_thread_num();
#else
        const int t=0;
#endif
#ifdef _OPENMP
        const int nth=omp_get_num_threads();
#else
        const int nth=1;
#endif
        perm_slot sl=slot_at(pl,scr,(size_t)t);
        csi s0, s1; share_of(pl->Sg,t,nth,&s0,&s1);
        if(s0<s1) {                                     /* one run per thread */
            const csi *first=pl->posG+(size_t)s0*Bp, hi=s1*CI<n?s1*CI:n;
            memcpy(sl.q,first,(size_t)Bp*sizeof(csi));
            p1_feed(rhs,0,pl->bktG,s0*CI,hi,buf,sl.q,first,sl.stage,streamed);
            p1_flush(buf,sl.q,first,Bp,sl.stage,streamed);
        }
        VSDLSS_OMP(omp barrier)
        VSDLSS_OMP(omp for schedule(dynamic,4))
        for(csi b=0;b<Bp;b++) {
            const csi p0=b*G, k0=pl->bstartG[b], k1=pl->bstartG[b+1];
            if(k0==k1) continue;
            const csi c=comp_at(pl,f->count,p0), cend=poff[c]+f->component[c].n;
            if(pl->frank[c]>=0) continue;                /* in that component's solve */
            const csi pend=p0+G<cend?p0+G:cend;
            if(p0>=poff[c] && k1-k0==pend-p0) {          /* block inside one component */
                /* Every slot of the block is written once (a permutation). */
                double *d=streamed?sl.blk:loc[c]+(p0-poff[c]);
                for(csi k=k0;k<k1;k++) { double v=buf[k]; d[pl->offG[k]]=v; bad|=!isfinite(v); }
                if(streamed) stream_copy(loc[c]+(p0-poff[c]),sl.blk,pend-p0);
            } else {
                for(csi k=k0;k<k1;k++) {
                    const csi p=p0+pl->offG[k], cc=comp_at(pl,f->count,p); double v=buf[k];
                    loc[cc][p-poff[cc]]=v; bad|=!isfinite(v);
                }
            }
        }
        stream_fence();
    }
    return bad;
}

/* Write-back, global part: pass 1 for the chunks of components that are not
 * fused (fused ones took it in their solve), then pass 2 for all entries:
 * out[component_map(c)[i]] = loc[c][i]. */
static void perm2_writeback(const vsdlss_m3_factor *f, const vsdlss_perm_plan *pl, double *buf,
                            double *const *loc, double *out, double *scr, int T, int streamed)
{
    const csi n=f->n, G=pl->G;
    (void)T;
    VSDLSS_OMP(omp parallel num_threads(T))
    {
#ifdef _OPENMP
        const int t=omp_get_thread_num();
#else
        const int t=0;
#endif
#ifdef _OPENMP
        const int nth=omp_get_num_threads();
#else
        const int nth=1;
#endif
        perm_slot sl=slot_at(pl,scr,(size_t)t);
        csi s0, s1; const csi *first=NULL;
        share_of(pl->Sp,t,nth,&s0,&s1);
        for(csi s=s0;s<s1;s++) {                        /* runs of chunks not fused */
            if(pl->frank[comp_at(pl,f->count,s*pl->CI)]>=0) {
                if(first) { p1_flush(buf,sl.q,first,pl->Bg,sl.stage,streamed); first=NULL; }
                continue;
            }
            if(!first) { first=pl->posW+(size_t)s*pl->Bg; memcpy(sl.q,first,(size_t)pl->Bg*sizeof(csi)); }
            writeback_chunk(f,pl,buf,loc,s,sl.q,first,sl.stage,streamed);
        }
        if(first) p1_flush(buf,sl.q,first,pl->Bg,sl.stage,streamed);
        VSDLSS_OMP(omp barrier)
        VSDLSS_OMP(omp for schedule(dynamic,4))
        for(csi b=0;b<pl->Bg;b++) {
            const csi g0=b*G, len=g0+G<n?G:n-g0;
            /* Every slot of the block is written once (a permutation). */
            double *d=streamed?sl.blk:out+g0;
            for(csi k=pl->bstartW[b];k<pl->bstartW[b+1];k++) d[pl->offW[k]]=buf[k];
            if(streamed) stream_copy(out+g0,sl.blk,len);
        }
        stream_fence();
    }
}

/* A fused component's forward replay with its gather pass 2: per chunk,
 * fill the chunk from its buckets, then replay the chunk's blocks while it
 * is in cache; then the sequential tail.  Same operations on every entry as
 * vsdlss_reduce_forward_inplace.  Returns 1 when a value is not finite. */
static int fused_forward(const vsdlss_m3_factor *f, const fused_ctx *z, csi c, double *local)
{
    const vsdlss_perm_plan *pl=z->pl;
    const vsdlss_reduction *r=f->component[c].reduction;
    const csi cn=f->component[c].n, CI=pl->CI, sh=pl->sh, b0=pl->poff[c]>>sh;
    const csi nch=(cn+CI-1)/CI; csi bs=0;
    vsdlss_reduce_fused_blocks(r,&bs);
    int bad=0, gt=vsdlss_parallel_width((double)cn*4); (void)gt;
    VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(dynamic,1) reduction(|:bad))
    for(csi j=0;j<nch;j++) {
        const csi lo=j*CI, hi=lo+CI<cn?lo+CI:cn;
        for(csi bb=lo>>sh;bb<=(hi-1)>>sh;bb++) {
            double *d=local+(bb<<sh);
            for(csi k=pl->bstartG[b0+bb];k<pl->bstartG[b0+bb+1];k++) { double v=z->buf[k]; d[pl->offG[k]]=v; bad|=!isfinite(v); }
        }
        for(csi b=lo/bs;b<=(hi-1)/bs;b++) vsdlss_reduce_forward_block(r,b,local);
    }
    if(!bad) vsdlss_reduce_forward_tail(r,local);
    return bad;
}

/* Scratch slot from the pool (z->P slots): a holder never waits while
 * holding one, so spinning always makes progress. */
static size_t slot_acquire(const fused_ctx *z)
{
    for(;;) for(int k=0;k<z->P;k++)
        if(!atomic_load_explicit(z->busy+k,memory_order_relaxed) && !atomic_exchange(z->busy+k,1)) return (size_t)k;
}
static void slot_release(const fused_ctx *z, size_t k)
{ atomic_store(z->busy+k,0); }

/* A fused component's backward replay with its write-back pass 1: the tail,
 * then per chunk its blocks and the chunk's pass 1.  Same operations as
 * vsdlss_reduce_backward_core_checked.  Returns 0 when a recovered value is
 * not finite. */
static int fused_backward(const vsdlss_m3_factor *f, const fused_ctx *z, csi c, double *local)
{
    const vsdlss_perm_plan *pl=z->pl;
    const vsdlss_reduction *r=f->component[c].reduction;
    const csi cn=f->component[c].n, CI=pl->CI, s0=pl->poff[c]/CI, pc=pl->poff[c];
    const csi nch=(cn+CI-1)/CI; csi bs=0;
    vsdlss_reduce_fused_blocks(r,&bs);
    int ok=vsdlss_reduce_backward_tail(r,local);
    if(!ok) return 0;
    int gt=vsdlss_parallel_width((double)cn*4); (void)gt;
    /* Contiguous chunks per thread (one pass-1 run each).  A thread holds
     * a scratch slot for its run; it waits for nothing while holding it. */
    VSDLSS_OMP(omp parallel num_threads(gt) if(gt>1) reduction(&:ok))
    {
#ifdef _OPENMP
        const int t=omp_get_thread_num(), nth=omp_get_num_threads();
#else
        const int t=0, nth=1;
#endif
        csi j0, j1; share_of(nch,t,nth,&j0,&j1);
        if(j0<j1) {
            const size_t k=slot_acquire(z);
            perm_slot sl=slot_at(pl,z->scr,k);
            const csi *first=pl->posW+(size_t)(s0+j0)*pl->Bg;
            memcpy(sl.q,first,(size_t)pl->Bg*sizeof(csi));
            for(csi j=j0;j<j1;j++) {
                const csi lo=j*CI, hi=lo+CI<cn?lo+CI:cn;
                for(csi b=lo/bs;b<=(hi-1)/bs;b++) ok&=vsdlss_reduce_backward_block(r,b,local);
                p1_feed(local,pc,pl->bktW,pc+lo,pc+hi,z->buf,sl.q,first,sl.stage,z->streamed);
            }
            p1_flush(z->buf,sl.q,first,pl->Bg,sl.stage,z->streamed);
            slot_release(z,k);
        }
    }
    return ok;
}

static int fused_component(const fused_ctx *z, csi c)
{ return z && z->pl->frank[c]>=0; }

static vsdlss_status solve_common(const vsdlss_m3_factor *factor,
                                  const double *rhs, double *solution, int internal)
{
    vsdlss_status status=VSDLSS_OK, *results=NULL; solve_ws *ws=NULL; double **loc=NULL;
    const vsdlss_perm_plan *pl=NULL; double *pbuf=NULL; atomic_int *busy=NULL;
    csi c, taken=0; int T=1, streamed=0;
    fused_ctx z;
    if(!factor || !factor->components || !factor->component || !rhs || !solution)
        return VSDLSS_ERR_INVALID;
    if(!count_fits(factor->count,sizeof(solve_ws))) return VSDLSS_ERR_OOM;
    const int inverse=!internal && (factor->inv32 || factor->inv64);
    ws=(solve_ws*)calloc((size_t)factor->count,sizeof(*ws));
    results=(vsdlss_status*)calloc((size_t)factor->count,sizeof(*results));
    if(inverse) loc=(double**)malloc((size_t)factor->count*sizeof(*loc));
    if(!ws||!results||(inverse&&!loc)) { status=VSDLSS_ERR_OOM; goto done; }
    for(taken=0;taken<factor->count;taken++) {
        status=ws_acquire(factor,taken,ws+taken);
        if(status!=VSDLSS_OK) goto done;
        if(loc) loc[taken]=ws[taken].local;
    }
    solve_ctx ctx={factor,rhs,solution,ws,internal,loc,0,NULL};
    if(inverse && !factor->disk_mode && factor->n>=PERM_MIN && perm2_on()) {
        /* Two-pass gather here, two-pass write-back below, the fused parts
         * inside the components' solves (plan and scratch permitting;
         * otherwise the direct loops).  P bounds the threads that can hold
         * a scratch slot at once (the components' schedule uses at most
         * vsdlss_parallel_width(n*256) threads). */
        T=vsdlss_parallel_width((double)factor->n*4);
        int P=vsdlss_parallel_width((double)factor->n*256); if(P<T) P=T;
        pl=perm_plan_get(factor);
        if(pl) busy=(atomic_int*)calloc((size_t)P,sizeof(*busy));
        if(pl && busy) pbuf=pbuf_acquire(factor,pl,P);
        if(pbuf) {
            double tg=trace_now();
            streamed=(double)pl->np*sizeof(double)>=stream_min_bytes();
            z.pl=pl; z.buf=pbuf; z.scr=factor->pscr; z.P=P; z.busy=busy; z.streamed=streamed;
            int bad=perm2_gather(factor,pl,rhs,pbuf,loc,factor->pscr,T,streamed);
            TRACE("solve: two-pass gather",tg);
            if(bad) { status=VSDLSS_ERR_NONFINITE; goto done; }
            ctx.pregathered=1; ctx.z=&z;
        }
    }
    run_components(factor,factor->disk_mode,solve_local,&ctx,results);
    for(c=0;c<factor->count;c++) if(results[c]!=VSDLSS_OK) { status=results[c]; goto done; }
    run_components(factor,factor->disk_mode,solve_local_back,&ctx,results);
    for(c=0;c<factor->count;c++) if(results[c]!=VSDLSS_OK) { status=results[c]; goto done; }
    double t0=trace_now();
    if(ctx.pregathered) perm2_writeback(factor,pl,pbuf,loc,solution,factor->pscr,T,streamed);
    else if(inverse) write_back_inverse(&ctx);
    else run_components(factor,factor->disk_mode,scatter_local,&ctx,results);
    TRACE("solve: scatter",t0);
done:
    if(pbuf) pbuf_release(factor);
    free(busy);
    if(ws) for(c=0;c<taken;c++) ws_release(factor,c,ws+c);
    free(ws); free(results); free(loc);
    return status;
}

vsdlss_status vsdlss_m3_solve(const vsdlss_m3_factor *factor,
                              const double *rhs, double *solution)
{ return solve_common(factor,rhs,solution,0); }

vsdlss_status vsdlss_m3_solve_internal(const vsdlss_m3_factor *factor,
                                       const double *rhs, double *solution)
{ return solve_common(factor,rhs,solution,1); }

vsdlss_status vsdlss_m3_internal_order(const vsdlss_m3_factor *factor, csi *perm)
{
    if(!factor || !factor->components || !factor->component || !perm) return VSDLSS_ERR_INVALID;
    for(csi c=0;c<factor->count;c++) {
        const csi cn=factor->component[c].n, *map=component_map(factor,c);
        memcpy(perm+factor->components->offset[c],map,(size_t)cn*sizeof(csi));
    }
    return VSDLSS_OK;
}

/* The packed API is the internal order under its other name. */
vsdlss_status vsdlss_m3_solve_packed(const vsdlss_m3_factor *factor,
                                     const double *rhs, double *solution)
{ return solve_common(factor,rhs,solution,1); }

vsdlss_status vsdlss_m3_export_packed_permutation(const vsdlss_m3_factor *factor,
                                                   csi *packed_to_global, csi length)
{
    if(!factor || length<factor->n) return VSDLSS_ERR_INVALID;
    return vsdlss_m3_internal_order(factor,packed_to_global);
}

/* A batch of right-hand sides through every component: per RHS gather and
 * in-place forward elimination, one batched supernodal solve per component,
 * then per RHS in-place recovery.  Per-RHS arithmetic equals solve_local. */
static vsdlss_status solve_batch(const vsdlss_m3_factor *f,csi nrhs,const double *rhs,
                                 csi ldrhs,double *dest,csi lddest)
{
    vsdlss_status st=VSDLSS_OK;
    for(csi c=0;c<f->count&&st==VSDLSS_OK;c++){
        const vsdlss_m3_component_factor *cf=f->component+c;
        const vsdlss_reduction *r=cf->reduction;
        const csi cn=cf->n,core=r->core_n,cnt=r->count;
        const csi *map=component_map(f,c), *cm=cf->core_map;
        double *local=malloc((size_t)(cn?cn:1)*sizeof(double));
        double *perm=core?malloc((size_t)core*(size_t)nrhs*sizeof(double)):NULL;
        double *saved=cnt?malloc((size_t)cnt*(size_t)nrhs*sizeof(double)):NULL;
        if(!local||(core&&!perm)||(cnt&&!saved)){st=VSDLSS_ERR_OOM;goto next;}
        for(csi r0=0;r0<nrhs&&st==VSDLSS_OK;r0++){
            const double *b=rhs+(size_t)r0*ldrhs; int bad=0;
            for(csi k=0;k<cn;k++){local[k]=b[map[k]];bad|=!isfinite(local[k]);}
            if(bad){st=VSDLSS_ERR_NONFINITE;break;}
            st=vsdlss_reduce_forward_inplace(r,local,cnt?saved+(size_t)r0*cnt:NULL);
            if(st==VSDLSS_OK)for(csi k=0;k<core;k++)perm[(size_t)r0*core+k]=local[cm[k]];
        }
        if(st==VSDLSS_OK&&core)st=vsdlss_sn_solve_batch(cf->numeric,nrhs,perm,core);
        for(csi r0=0;r0<nrhs&&st==VSDLSS_OK;r0++){
            double *xo=dest+(size_t)r0*lddest;
            for(csi k=0;k<core;k++)local[cm[k]]=perm[(size_t)r0*core+k];
            st=vsdlss_reduce_backward_inplace(r,cnt?saved+(size_t)r0*cnt:NULL,local);
            if(st==VSDLSS_OK)for(csi k=0;k<cn;k++)xo[map[k]]=local[k];
        }
next:
        free(local);free(perm);free(saved);
    }
    return st;
}

vsdlss_status vsdlss_m3_solve_many(const vsdlss_m3_factor *f,csi nrhs,
    const double *rhs,csi ldrhs,double *solution,csi ldsolution)
{
    if(f && f->disk_mode)return VSDLSS_ERR_UNSUPPORTED;
    if(!f||!rhs||!solution||nrhs<1||ldrhs<f->n||ldsolution<f->n)return VSDLSS_ERR_INVALID;
    if(f->n<1||(uint64_t)nrhs>SIZE_MAX/sizeof(double)/(uint64_t)f->n||
        (uint64_t)nrhs>SIZE_MAX/sizeof(vsdlss_status)||
        (uint64_t)(nrhs-1)>(SIZE_MAX/sizeof(double)-(uint64_t)f->n)/(uint64_t)ldrhs||
        (uint64_t)(nrhs-1)>(SIZE_MAX/sizeof(double)-(uint64_t)f->n)/(uint64_t)ldsolution)
        return VSDLSS_ERR_OOM;
    /* One RHS: the single-solve path (cached workspace, parallel gather,
     * block-parallel reduction, tree-parallel core) gives identical bits. */
    if(nrhs==1)return vsdlss_m3_solve(f,rhs,solution);
    double *work=malloc((size_t)nrhs*(size_t)f->n*sizeof(double));
    vsdlss_status *results=calloc((size_t)nrhs,sizeof(*results)),status=VSDLSS_OK;
    if(!work||!results){free(work);free(results);return VSDLSS_ERR_OOM;}
    int nt=vsdlss_parallel_width((double)f->n*nrhs*256);
    if(nt>nrhs)nt=(int)nrhs;
    (void)nt;
    /* Contiguous groups of right-hand sides, one batched solve per group;
     * `work` is private per RHS, so no extra staging is needed. */
    csi groups=nt>1?nt:1;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(static,1))
        for(csi g=0;g<groups;g++){
            csi r0=nrhs*g/groups,r1=nrhs*(g+1)/groups;
            if(r1>r0)results[r0]=solve_batch(f,r1-r0,rhs+(size_t)r0*ldrhs,ldrhs,
                                             work+(size_t)r0*f->n,f->n);
        }
    }
    for(csi r=0;r<nrhs;r++)if(results[r]!=VSDLSS_OK){status=results[r];break;}
    if(status==VSDLSS_OK)for(csi r=0;r<nrhs;r++)
        memcpy(solution+(size_t)r*ldsolution,work+(size_t)r*f->n,(size_t)f->n*sizeof(double));
    free(work);free(results);return status;
}

vsdlss_status vsdlss_factorize_m3(const vsdlss *a,int order,vsdlss_m3_factor **out)
{ return factorize_shared(a,order,out,0,0,NULL); }
vsdlss_status vsdlss_factorize_m4_reduced(const vsdlss *a,int order,size_t budget,const char *directory,vsdlss_m4_reduced_factor **out)
{ return factorize_shared(a,order,out,1,budget,directory); }
vsdlss_status vsdlss_m4_reduced_solve(vsdlss_m4_reduced_factor *f,const double *b,double *x)
{ if(!f||!f->disk_mode)return VSDLSS_ERR_INVALID;return vsdlss_m3_solve(f,b,x); }
void vsdlss_m4_reduced_free(vsdlss_m4_reduced_factor *f)
{ vsdlss_m3_factor_free(f); }
