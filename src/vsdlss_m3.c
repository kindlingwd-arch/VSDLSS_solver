#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

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

/* Components smaller than this keep the ascending numbering. */
csi vsdlss_reorder_min = 4096;
#define VSDLSS_REORDER_MIN vsdlss_reorder_min
/* Tests only: use 64-bit inverse-map codes even when 32 bits would do. */
int vsdlss_m3_inverse_force64 = 0;

static int count_fits(csi n, size_t width)
{
    return n >= 0 && (uint64_t)n <= SIZE_MAX / width;
}

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
        status=vsdlss_sn_analyze_relaxed(permuted,&symbolic);
        if(status!=VSDLSS_OK) goto done;
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
#ifdef VSDLSS_METIS
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
 * solution in its local buffer: gather (checking the RHS is finite), forward
 * elimination of the low-degree vertices, core solve, recovery.  Only when
 * every component succeeded does phase 2 scatter the local solutions into
 * the caller's array, so a failed solve leaves it untouched, and rhs may be
 * the same array as solution (every component has read its RHS by then). */
typedef struct { double *local, *saved, *core, *core_x; int own; } solve_ws;

/* internal: rhs and out are in the factor's internal order (component c's
 * local vector at components->offset[c]), so gather and write-back are
 * sequential copies.  loc: each component's local buffer, read by the
 * sequential write-back through the inverse map. */
typedef struct {
    const vsdlss_m3_factor *f; const double *rhs; double *out; solve_ws *ws;
    int internal; double **loc;
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
    memset(w,0,sizeof(*w));
    if(!count_fits(cf->n,sizeof(double)) || !count_fits(core,sizeof(double)) ||
       !count_fits(r->count,sizeof(double))) return VSDLSS_ERR_OOM;
    if(cf->ws_local && atomic_exchange(&cf->ws_busy,1)==0) {
        w->own=1; w->local=cf->ws_local; w->saved=cf->ws_saved; w->core=cf->ws_core;
    } else {
        w->local=(double*)malloc((size_t)(cf->n?cf->n:1)*sizeof(double));
        if(r->count) w->saved=(double*)malloc((size_t)r->count*sizeof(double));
        if(core) w->core=(double*)malloc((size_t)core*sizeof(double));
    }
    if(core&&cf->disk) w->core_x=(double*)malloc((size_t)core*sizeof(double));
    if(!w->local||(r->count&&!w->saved)||(core&&!w->core)||(core&&cf->disk&&!w->core_x)) {
        ws_release(f,c,w); return VSDLSS_ERR_OOM;
    }
    return VSDLSS_OK;
}

/* Phase 1 for one component (all buffers private). */
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
    } else {
        const csi *map=component_map(x->f,c); const double *rhs=x->rhs;
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static) reduction(|:bad))
        for(csi i=0;i<cn;i++) { double v=rhs[map[i]]; local[i]=v; bad|=!isfinite(v); }
    }
    if(bad) return VSDLSS_ERR_NONFINITE;
    TRACE("solve: gather",t0);
    status=vsdlss_reduce_forward_inplace(r,local,w->saved);
    if(status!=VSDLSS_OK) return status;
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
    status=vsdlss_reduce_backward_inplace(r,w->saved,local);
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

static vsdlss_status solve_common(const vsdlss_m3_factor *factor,
                                  const double *rhs, double *solution, int internal)
{
    vsdlss_status status=VSDLSS_OK, *results=NULL; solve_ws *ws=NULL; double **loc=NULL;
    csi c, taken=0;
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
    solve_ctx ctx={factor,rhs,solution,ws,internal,loc};
    run_components(factor,factor->disk_mode,solve_local,&ctx,results);
    for(c=0;c<factor->count;c++) if(results[c]!=VSDLSS_OK) { status=results[c]; goto done; }
    double t0=trace_now();
    if(inverse) write_back_inverse(&ctx);
    else run_components(factor,factor->disk_mode,scatter_local,&ctx,results);
    TRACE("solve: scatter",t0);
done:
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
