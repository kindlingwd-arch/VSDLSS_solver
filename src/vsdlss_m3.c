#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"
#include "vsdlss_ledger.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#if defined(__GLIBC__)
#include <malloc.h>
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
/* Resident and peak resident set in MB (Linux /proc; -1 elsewhere). */
static void proc_mem(long *rss, long *hwm)
{
    *rss=*hwm=-1;
#if defined(__linux__)
    FILE *fp=fopen("/proc/self/status","r"); char line[128];
    if(!fp) return;
    while(fgets(line,sizeof line,fp)) {
        if(!strncmp(line,"VmRSS:",6)) *rss=atol(line+6)/1024;
        else if(!strncmp(line,"VmHWM:",6)) *hwm=atol(line+6)/1024;
    }
    fclose(fp);
#endif
}
static void trace_line(const char *label, double dt)
{
    long rss,hwm; proc_mem(&rss,&hwm);
    fprintf(stderr,"vsdlss trace: %-22s %8.3f s  rss %6ld MB  peak %6ld MB\n",label,dt,rss,hwm);
}
#define TRACE(label,t0) do{ if(trace_on()){ double t1_=trace_now(); \
    trace_line(label,t1_-(t0)); (t0)=t1_; } }while(0)

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
void vsdlss_release_free_memory(int level)
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
        free(factor->component[k].map32);
        free(factor->component[k].core_map32);
        free(factor->component[k].ws_local);
        free(factor->component[k].ws_saved);
        free(factor->component[k].ws_core);
        free(factor->component[k].core_map);
        vsdlss_sn_factor_free(factor->component[k].numeric);
    }
    free(factor->component);
    vsdlss_components_free(factor->components);
    free(factor->inv32); free(factor->inv64);
    for(k=0;k<VSDLSS_PERM_PLAN_SLOTS;k++) perm_plan_free(atomic_load(&factor->pplan[k]));
    free(factor->pbuf);
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
        cf->renumbered=1;
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

/* Solve-order relabel (VSDLSS_RELABEL, default on): renumber a component's
 * local vertices as [eliminated vertices in record order, core unknowns in
 * supernodal order].  The forward/backward replays then walk their pivot
 * vertices sequentially, and the core right-hand side is the contiguous
 * tail local[count..n) -- the core solve runs on it in place, with no
 * gather or scatter.  Only the numbering changes: every entry sees the same
 * operations in the same order, so solutions are bitwise identical.  The
 * packed/internal order (vsdlss_m3_export_packed_permutation) changes with
 * it.  Applies to packed reductions of renumbered (BFS) components. */
int vsdlss_m3_relabel = -1;
void (*vsdlss_m3_core_hook)(csi, const vsdlss *) = NULL;
static int relabel_on(void)
{
    if(vsdlss_m3_relabel<0){ const char *e=getenv("VSDLSS_RELABEL"); vsdlss_m3_relabel=e?atoi(e):1; }
    return vsdlss_m3_relabel;
}
static vsdlss_status relabel_component(vsdlss_m3_component_factor *cf)
{
    vsdlss_reduction *r=cf->reduction;
    const csi n=cf->n, cnt=r->count, core=r->core_n;
    if(!cf->gather || r->records || n!=r->n || cnt+core!=n || (core && !cf->core_map)) return VSDLSS_OK;
    if(n>(csi)0x3fffffff) return VSDLSS_OK;
    csi *id=(csi*)malloc((size_t)n*sizeof(csi)), *g2=(csi*)vsdlss_big_malloc((size_t)n*sizeof(csi));
    if(!id||!g2){ free(id); free(g2); return VSDLSS_ERR_OOM; }
    for(csi v=0;v<n;v++) id[v]=-1;
    csi pos=0;
    for(csi q=0;q<r->pk_count;q++){ const vsdlss_pk_seg *g=r->pk+q; if(!g->head && g->count) goto skip;
        for(csi i=0;i<g->count;i++){ csi v=(csi)(g->head[i]&0x3fffffffu); if(v<0||v>=n||id[v]>=0) goto skip; id[v]=pos++; } }
    if(pos!=cnt) goto skip;
    for(csi k=0;k<core;k++){ csi v=cf->core_map[k]; if(v<0||v>=n||id[v]>=0) goto skip; id[v]=cnt+k; }
    for(csi q=0;q<r->pk_count;q++){ vsdlss_pk_seg *g=r->pk+q;
        for(csi i=0;i<g->count;i++){ uint32_t h=g->head[i]; g->head[i]=(h&~0x3fffffffu)|(uint32_t)id[h&0x3fffffffu]; }
        for(csi j=0;j<g->nbn;j++) g->nb[j]=(uint32_t)id[g->nb[j]]; }
    for(csi k=0;k<core;k++){ if(r->core_vertices) r->core_vertices[k]=id[r->core_vertices[k]]; cf->core_map[k]=cnt+k; }
    /* Record k now pivots on vertex k: keep only the degrees (1 byte instead
     * of 4 per record, read by both replays). */
    for(csi q=0;q<r->pk_count;q++){ vsdlss_pk_seg *g=r->pk+q; int seq=g->count>0;
        for(csi i=0;i<g->count&&seq;i++) seq=(csi)(g->head[i]&0x3fffffffu)==g->k0+i;
        if(!seq) continue;
        uint8_t *deg=(uint8_t*)malloc((size_t)g->count);
        if(!deg){ free(id); free(g2); return VSDLSS_ERR_OOM; }
        for(csi i=0;i<g->count;i++) deg[i]=(uint8_t)(g->head[i]>>30);
        free(g->head); g->head=NULL; g->deg=deg; }
    for(csi v=0;v<n;v++) g2[id[v]]=cf->gather[v];
    free(cf->gather);
    cf->gather=g2; cf->core_contig=1;
    free(id);
    return VSDLSS_OK;
skip:
    free(id); free(g2);
    return VSDLSS_OK;
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
        if(vsdlss_m3_core_hook) vsdlss_m3_core_hook(component,permuted);
        /* The solve reads core unknown k at local vertex core_vertices[q[k]]. */
        for(csi k=0;k<r->core_n;k++) q[k]=r->core_vertices[q[k]];
        vsdlss_spfree(r->core); r->core=NULL;   /* only the disk mode reads it later */
        /* core_map replaces core_vertices from here on (the solve checks the
         * core values itself when writing them back). */
        free(r->core_vertices); r->core_vertices=NULL;
        vsdlss_release_free_memory(2);                  /* ordering workspace, before L */
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
        status=vsdlss_sn_factorize_consume(&permuted,&symbolic,&cf->numeric);
        TRACE("core numeric",t0);
    }
    if(status==VSDLSS_OK && !factor->disk_mode && relabel_on()) {
        status=relabel_component(cf);
        TRACE("solve-order relabel",t0);
    }
    /* The tail's forward plan needs the final vertex numbering. */
    if(status==VSDLSS_OK && !factor->disk_mode && cf->reduction) {
        status=vsdlss_reduce_tail_plan(cf->reduction);
        if(cf->reduction->tail_levels) TRACE("reduction tail plan",t0);
    }
    /* 32-bit core map once the numbering is final (in memory only). */
    if(status==VSDLSS_OK && !factor->disk_mode && cf->core_map && cf->n<=(csi)UINT32_MAX) {
        const csi cn=cf->reduction->core_n;
        cf->core_map32=(uint32_t*)vsdlss_big_malloc((size_t)(cn?cn:1)*sizeof(uint32_t));
        if(!cf->core_map32) {status=VSDLSS_ERR_OOM;goto done;}
        for(csi k=0;k<cn;k++) cf->core_map32[k]=(uint32_t)cf->core_map[k];
        free(cf->core_map); cf->core_map=NULL;
    }
done:
    free(pinv); vsdlss_sn_symbolic_free(symbolic); vsdlss_spfree(permuted);
    return status;
}

static vsdlss_status build_inverse(vsdlss_m3_factor *f);

/* 32-bit local -> global maps for every component when the global indices
 * fit, replacing the BFS gather arrays and components->vertices (which only
 * the maps read once the factorization is done). */
static vsdlss_status compact_maps(vsdlss_m3_factor *f)
{
    if(f->n>(csi)UINT32_MAX+1) return VSDLSS_OK;
    const csi *off=f->components->offset;
    for(csi c=0;c<f->count;c++) {
        vsdlss_m3_component_factor *cf=f->component+c;
        const csi cn=cf->n, *src=cf->gather?cf->gather:f->components->vertices+off[c];
        cf->map32=(uint32_t*)vsdlss_big_malloc((size_t)(cn?cn:1)*sizeof(uint32_t));
        if(!cf->map32) return VSDLSS_ERR_OOM;
        int t=vsdlss_parallel_width((double)cn*4); (void)t;
        uint32_t *m=cf->map32;
        VSDLSS_OMP(omp parallel for num_threads(t) if(t>1) schedule(static))
        for(csi i=0;i<cn;i++) m[i]=(uint32_t)src[i];
    }
    for(csi c=0;c<f->count;c++) { free(f->component[c].gather); f->component[c].gather=NULL; }
    free(f->components->vertices); f->components->vertices=NULL;
    return VSDLSS_OK;
}

/* A local -> global (or core -> local) map in either width. */
typedef struct { const uint32_t *u; const csi *w; } idxmap;
static inline csi im_at(idxmap m, csi i) { return m.u?(csi)m.u[i]:m.w[i]; }
static idxmap core_map_of(const vsdlss_m3_component_factor *cf)
{ idxmap m={cf->core_map32,cf->core_map}; return m; }

/* VSDLSS_TRACE: bytes held by the factor, by kind. */
static void trace_factor_bytes(const vsdlss_m3_factor *f)
{
    double mb=1.0/(1<<20), L=0, ridx=0, meta=0, replay=0, maps=0, ws=0, tables=0;
    const csi n=f->n;
    tables+=(f->components->vertices?(double)n*sizeof(csi):0)+(double)(f->count+1)*sizeof(csi);    /* components->vertices, offset */
    maps+=f->inv32?(double)n*4:f->inv64?(double)n*8:0;
    for(csi c=0;c<f->count;c++) {
        const vsdlss_m3_component_factor *cf=f->component+c;
        const vsdlss_reduction *r=cf->reduction;
        if(cf->gather) maps+=(double)cf->n*sizeof(csi);
        if(cf->map32) maps+=(double)cf->n*4;
        if(cf->core_map) maps+=(double)r->core_n*sizeof(csi);
        if(cf->core_map32) maps+=(double)r->core_n*4;
        if(r) {
            if(r->core_vertices) maps+=(double)r->core_n*sizeof(csi);
            for(csi q=0;q<r->pk_count;q++)
                replay+=(double)r->pk[q].count*(r->pk[q].head?4+8:1+8)+(double)r->pk[q].nbn*(4+8);
            if(r->records) replay+=(double)r->count*sizeof(vsdlss_elim_record);
        }
        ws+=(double)(cf->ws_local?cf->n:0)*8+(double)(cf->ws_core?r->core_n:0)*8+
            (double)(cf->ws_saved?r->count:0)*8;
        const vsdlss_sn_factor *s=cf->numeric;
        if(s) {
            L+=(double)s->panel_offset[s->count]*8;
            ridx+=(double)s->row_ptr[s->count]*sizeof(vsdlss_sni);
            meta+=(double)s->count*(2*sizeof(vsdlss_sni)+3*sizeof(csi))+(double)s->blk_ptr[s->count]*sizeof(vsdlss_sni)*3
                 +(double)s->count*(2*sizeof(csi)+sizeof(double));             /* + solve tree */
            fprintf(stderr,"vsdlss trace: component %lld: n %lld, core %lld, supernodes %lld, blocks %lld, row indices %lld, L %lld\n",
                    (long long)c,(long long)cf->n,(long long)r->core_n,(long long)s->count,
                    (long long)s->blk_ptr[s->count],(long long)s->row_ptr[s->count],(long long)s->panel_offset[s->count]);
        }
    }
    fprintf(stderr,"vsdlss trace: factor MB: L %.0f, row indices %.0f, supernode metadata %.0f, "
            "replay %.0f, maps %.0f, vertex tables %.0f, solve buffers %.0f, total %.0f\n",
            L*mb,ridx*mb,meta*mb,replay*mb,maps*mb,tables*mb,ws*mb,
            (L+ridx+meta+replay+maps+tables+ws)*mb);
}

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
    vsdlss_release_free_memory(2);                      /* adjacency graph */
    run_components(factor,disk_mode,factor_component,&ctx,results);
    for(component=0;component<factor->count;component++)if(results[component]!=VSDLSS_OK){
        status=results[component];goto fail;
    }
    TRACE("components factored",t0);     /* wall time of the per-component lines */
    free(results); results=NULL; free(inputs); inputs=NULL;
    status=compact_maps(factor);
    if(status!=VSDLSS_OK) goto fail;
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
    vsdlss_release_free_memory(1);
    TRACE("maps+inverse+trim",t0);
    if(trace_on()) trace_factor_bytes(factor);
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
    int pregathered;            /* local buffers already filled (two-pass gather) */
    int inplace;                /* internal order, solve directly in out (== rhs) */
} solve_ctx;

static idxmap component_map(const vsdlss_m3_factor *f, csi c)
{
    const vsdlss_m3_component_factor *cf=f->component+c;
    idxmap m={cf->map32,NULL};
    if(!m.u) m.w=cf->gather?cf->gather:f->components->vertices+f->components->offset[c];
    return m;
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
        const csi cn=f->component[c].n; const idxmap map=component_map(f,c);
        const uint64_t hi=(uint64_t)c<<bi;
        int t=cn>=65536?T:1; (void)t;
        if(f->inv32) {
            uint32_t *inv=f->inv32;
            VSDLSS_OMP(omp parallel for num_threads(t) if(t>1) schedule(static))
            for(csi i=0;i<cn;i++) inv[im_at(map,i)]=(uint32_t)(hi|(uint64_t)i);
        } else {
            uint64_t *inv=f->inv64;
            VSDLSS_OMP(omp parallel for num_threads(t) if(t>1) schedule(static))
            for(csi i=0;i<cn;i++) inv[im_at(map,i)]=hi|(uint64_t)i;
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

/* Phase 1 for one component (all buffers private). */
static vsdlss_status solve_local(void *vctx, csi c)
{
    const solve_ctx *x=(const solve_ctx*)vctx;
    const vsdlss_m3_component_factor *cf=x->f->component+c;
    const vsdlss_reduction *r=cf->reduction;
    const csi core=r->core_n, cn=cf->n;
    solve_ws *w=x->ws+c;
    double *local=x->inplace?x->out+x->f->components->offset[c]:w->local, *core_b=w->core;
    vsdlss_status status; int bad=0;
    int gt=vsdlss_parallel_width((double)cn*4); (void)gt;
    double t0=trace_now(), tl=vsdlss_ledger_level()?vsdlss_ledger_now():0;
    if(x->inplace) {
        /* No copy: only the finite scan of the caller's vector. */
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static) reduction(|:bad))
        for(csi i=0;i<cn;i++) bad|=!isfinite(local[i]);
    } else if(x->internal) {
        const double *src=x->rhs+x->f->components->offset[c];
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static) reduction(|:bad))
        for(csi i=0;i<cn;i++) { double v=src[i]; local[i]=v; bad|=!isfinite(v); }
    } else if(!x->pregathered) {
        const idxmap map=component_map(x->f,c); const double *rhs=x->rhs;
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static) reduction(|:bad))
        for(csi i=0;i<cn;i++) { double v=rhs[im_at(map,i)]; local[i]=v; bad|=!isfinite(v); }
    }
    if(bad) return VSDLSS_ERR_NONFINITE;
    TRACE("solve: gather",t0);
    LEDGER_MARK(LG_PERM_GATHER,tl);
    status=vsdlss_reduce_forward_inplace(r,local,w->saved);   /* NULL when packed */
    if(status!=VSDLSS_OK) return status;
    TRACE("solve: reduce forward",t0);
    LEDGER_MARK(LG_RED_FWD,tl);
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
            if(cf->core_contig) {
                /* Relabelled: the core RHS is local[count..n); solve it in
                 * place.  With core_vertices kept, the backward replay's
                 * scan of the core entries reports a non-finite result;
                 * without them (dropped after factorization) it is checked
                 * here, in one sequential pass over the core tail. */
                LEDGER_MARK(LG_CORE_GATHER,tl);
                status=vsdlss_sn_solve_inplace(cf->numeric,local+r->count);
                if(status!=VSDLSS_OK) return status;
                if(vsdlss_ledger_level()) tl=vsdlss_ledger_now();
                if(!r->core_vertices) {
                    const double *cv=local+r->count;
                    int ct=vsdlss_parallel_width((double)core*4); (void)ct;
                    VSDLSS_OMP(omp parallel for num_threads(ct) if(ct>1) schedule(static) reduction(|:bad))
                    for(csi k=0;k<core;k++) bad|=!isfinite(cv[k]);
                    if(bad) return VSDLSS_ERR_NONFINITE;
                }
                goto core_done;
            }
            const idxmap cm=core_map_of(cf);
            int ct=vsdlss_parallel_width((double)core*4); (void)ct;
            VSDLSS_OMP(omp parallel for num_threads(ct) if(ct>1) schedule(static))
            for(csi k=0;k<core;k++) core_b[k]=local[im_at(cm,k)];
            LEDGER_MARK(LG_CORE_GATHER,tl);
            status=vsdlss_sn_solve_inplace(cf->numeric,core_b);   /* marks LG_CORE_FWD/BWD */
            if(status!=VSDLSS_OK) return status;
            if(vsdlss_ledger_level()) tl=vsdlss_ledger_now();
            VSDLSS_OMP(omp parallel for num_threads(ct) if(ct>1) schedule(static) reduction(|:bad))
            for(csi k=0;k<core;k++) { double v=core_b[k]; local[im_at(cm,k)]=v; bad|=!isfinite(v); }
            if(bad) return VSDLSS_ERR_NONFINITE;
            LEDGER_MARK(LG_CORE_SCATTER,tl);
        }
    }
core_done:
    TRACE("solve: core",t0);
    if(vsdlss_ledger_level()) tl=vsdlss_ledger_now();
    status=vsdlss_reduce_backward_inplace(r,w->saved,local);
    TRACE("solve: reduce backward",t0);
    LEDGER_MARK(LG_RED_BWD,tl);
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
        const idxmap map=component_map(x->f,c); double *out=x->out;
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static))
        for(csi i=0;i<cn;i++) out[im_at(map,i)]=local[i];
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
 * twice: gather (caller order -> packed order, the components' local buffers
 * laid end to end) and write-back (packed -> caller order).  Done directly,
 * each element is one random memory access and the loops are latency bound.
 * Two passes make both sides sequential: pass 1 streams the source and
 * appends each value to the bucket of its destination block (2^PERM_SH
 * doubles, 256 KB, cache resident); pass 2 fills one destination block per
 * bucket.  Measured on a 16M-element random permutation: 2.7x faster than
 * the direct loop.  Pure copies: results are bitwise those of the direct
 * loops.  A plan (per thread count: each thread's slot range per bucket, and
 * each slot's offset in its block) depends only on the permutation, so it is
 * built on first use and kept with the factor.  VSDLSS_PERM2=0 disables.
 * 2026-09-26: pass 1 was bound by its B open store streams (977 at 32M,
 * each a partial line with a read for ownership), about 75% of the time.
 * It now fills one 64-byte line per bucket in a small buffer and writes
 * whole lines, non-temporal for large n; pass 2 places a bucket in a staging
 * block and streams it out; pass 1 reads uint16 bucket ids instead of
 * decoding the maps.  Gather + write-back 0.130 -> 0.064 s on a 6M grid,
 * see docs/reconstruction/perm2-wc-nt-20260926.md. */
#define PERM_SH 15
#define PERM_BLK ((csi)1<<PERM_SH)
#define PERM_MASK (PERM_BLK-1)
#define PERM_WC 8                    /* doubles per 64-byte write-combining line */
/* Smallest n using the two-pass path (tests lower it). */
csi vsdlss_perm2_min = (csi)1<<20;
/* Smallest n whose scratch and destinations are written with non-temporal
 * stores: below it the vectors can stay in the last-level cache, and the
 * next phase would rather find them there (tests lower it). */
csi vsdlss_perm2_nt_min = (csi)1<<22;
#define PERM_MIN vsdlss_perm2_min
/* Scratch layout of one plan: each (thread, bucket) segment starts on a
 * 64-byte line, so pass 1 can collect a line per bucket in a small L1/L2
 * resident buffer and write whole lines (non-temporal for large n: no read
 * for ownership of 2 x n doubles, and one open stream instead of B).  Pass 2
 * places a bucket in a staging block and streams the block out. */
typedef struct vsdlss_perm_plan {
    int T; csi B, chunk, len;        /* len: padded scratch length (doubles) */
    int nt;                          /* non-temporal stores */
    csi *posG, *posW;                /* T*B: first scratch slot of (thread, bucket), line aligned */
    csi *cntG, *cntW;                /* T*B: slots used by (thread, bucket) */
    uint16_t *offG, *offW;           /* len: destination offset of each slot in its block */
    uint16_t *bktG, *bktW;           /* n: pass-1 bucket in source order (NULL: derived) */
} vsdlss_perm_plan;

static void perm_plan_free(struct vsdlss_perm_plan *p)
{
    if(!p) return;
    free(p->posG); free(p->posW); free(p->cntG); free(p->cntW);
    free(p->offG); free(p->offW); free(p->bktG); free(p->bktW); free(p);
}

static int perm2_on(void)
{
    static int on=-1;
    if(on<0) { const char *e=getenv("VSDLSS_PERM2"); on=!(e && e[0]=='0'); }
    return on;
}
/* VSDLSS_PERM2_BUCKETS=0: derive pass-1 buckets from the maps instead of
 * keeping 2 x n uint16 bucket ids with the plan (4 bytes per vertex). */
static int perm2_bkt_on(void)
{
    static int on=-1;
    if(on<0) { const char *e=getenv("VSDLSS_PERM2_BUCKETS"); on=!(e && e[0]=='0'); }
    return on;
}
/* VSDLSS_PERM2_NT=0/1 forces plain / non-temporal stores. */
static int perm2_nt(csi n)
{
    const char *e=getenv("VSDLSS_PERM2_NT");
    if(e && e[0]) return e[0]!='0';
    return n>=vsdlss_perm2_nt_min;
}

/* Packed position of global vertex g, and the component holding packed p. */
static inline csi packed_of(const vsdlss_m3_factor *f, csi g)
{
    const int sh=f->inv_shift; const csi *off=f->components->offset;
    if(f->inv32) { uint32_t u=f->inv32[g]; return off[u>>sh]+(csi)(u&(uint32_t)(((uint64_t)1<<sh)-1)); }
    uint64_t u=f->inv64[g]; return off[u>>sh]+(csi)(u&(((uint64_t)1<<sh)-1));
}
static csi comp_of(const vsdlss_m3_factor *f, csi p)
{
    const csi *off=f->components->offset; csi lo=0, hi=f->count-1;
    while(lo<hi) { csi mid=lo+(hi-lo+1)/2; if(off[mid]<=p) lo=mid; else hi=mid-1; }
    return lo;
}

/* ---- 64-byte line and block stores ---- */
#if defined(__SSE2__) || defined(_M_X64)
#include <emmintrin.h>
#define PERM_HAVE_NT 1
static inline void line_out(double *d, const double *s, int nt)
{   /* d and s 64-byte aligned */
    if(nt) {
        _mm_stream_pd(d,_mm_load_pd(s)); _mm_stream_pd(d+2,_mm_load_pd(s+2));
        _mm_stream_pd(d+4,_mm_load_pd(s+4)); _mm_stream_pd(d+6,_mm_load_pd(s+6));
    } else memcpy(d,s,PERM_WC*sizeof(double));
}
static inline void perm_fence(int nt) { if(nt) _mm_sfence(); }
/* Copies len doubles (d 8-byte aligned); returns 1 if any is not finite
 * when check is set. */
static inline int block_out(double *d, const double *s, csi len, int nt, int check)
{
    int bad=0; csi j=0;
    if(check) for(csi i=0;i<len;i++) bad|=!isfinite(s[i]);
    if(!nt) { memcpy(d,s,(size_t)len*sizeof(double)); return bad; }
    for(;j<len && ((uintptr_t)(d+j)&15);j++) d[j]=s[j];
    for(;j+2<=len;j+=2) _mm_stream_pd(d+j,_mm_loadu_pd(s+j));
    for(;j<len;j++) d[j]=s[j];
    return bad;
}
#else
#define PERM_HAVE_NT 0
static inline void line_out(double *d, const double *s, int nt)
{ (void)nt; memcpy(d,s,PERM_WC*sizeof(double)); }
static inline void perm_fence(int nt) { (void)nt; }
static inline int block_out(double *d, const double *s, csi len, int nt, int check)
{
    int bad=0; (void)nt;
    if(check) for(csi i=0;i<len;i++) bad|=!isfinite(s[i]);
    memcpy(d,s,(size_t)len*sizeof(double)); return bad;
}
#endif

/* Doubles of scratch a solve with plan pl needs: bucket buffer, then per
 * thread a staging block and B write-combining lines. */
static size_t perm_scratch_len(const vsdlss_perm_plan *pl)
{
    return (size_t)pl->len+(size_t)pl->T*((size_t)PERM_BLK+(size_t)pl->B*PERM_WC);
}

static vsdlss_perm_plan *perm_plan_build(const vsdlss_m3_factor *f, int T)
{
    const csi n=f->n, B=(n+PERM_MASK)>>PERM_SH, chunk=(n+T-1)/T;
    const csi *off=f->components->offset;
    const size_t S=(size_t)T*(size_t)B;
    vsdlss_perm_plan *pl=(vsdlss_perm_plan*)calloc(1,sizeof(*pl));
    if(!pl) return NULL;
    pl->T=T; pl->B=B; pl->chunk=chunk; pl->nt=PERM_HAVE_NT && perm2_nt(n);
    pl->len=(n+(csi)S*PERM_WC+PERM_WC-1)/PERM_WC*PERM_WC;   /* keeps the tail 64-byte aligned */
    pl->posG=(csi*)malloc(S*sizeof(csi)); pl->posW=(csi*)malloc(S*sizeof(csi));
    pl->cntG=(csi*)calloc(S,sizeof(csi)); pl->cntW=(csi*)calloc(S,sizeof(csi));
    pl->offG=(uint16_t*)vsdlss_big_malloc((size_t)pl->len*sizeof(uint16_t));
    pl->offW=(uint16_t*)vsdlss_big_malloc((size_t)pl->len*sizeof(uint16_t));
    if(B<=65536 && perm2_bkt_on()) {
        pl->bktG=(uint16_t*)vsdlss_big_malloc((size_t)n*sizeof(uint16_t));
        pl->bktW=(uint16_t*)vsdlss_big_malloc((size_t)n*sizeof(uint16_t));
        if(!pl->bktG||!pl->bktW) { free(pl->bktG); free(pl->bktW); pl->bktG=pl->bktW=NULL; }
    }
    csi *q=(csi*)malloc(S*2*sizeof(csi));
    if(!pl->posG||!pl->posW||!pl->cntG||!pl->cntW||!pl->offG||!pl->offW||!q) { free(q); perm_plan_free(pl); return NULL; }
    VSDLSS_OMP(omp parallel num_threads(T))
    {
#ifdef _OPENMP
        const int t=omp_get_thread_num();
#else
        const int t=0;
#endif
        const csi lo=(csi)t*chunk<n?(csi)t*chunk:n, hi=lo+chunk<n?lo+chunk:n;
        csi *cg=pl->cntG+(size_t)t*B, *cw=pl->cntW+(size_t)t*B;
        for(csi g=lo;g<hi;g++) {
            const csi b=packed_of(f,g)>>PERM_SH; cg[b]++;
            if(pl->bktG) pl->bktG[g]=(uint16_t)b;
        }
        if(lo<hi) {
            csi c=comp_of(f,lo); idxmap map=component_map(f,c);
            for(csi p=lo;p<hi;p++) {
                while(p>=off[c+1]) { c++; map=component_map(f,c); }
                const csi b=im_at(map,p-off[c])>>PERM_SH; cw[b]++;
                if(pl->bktW) pl->bktW[p]=(uint16_t)b;
            }
        }
        VSDLSS_OMP(omp barrier)
        VSDLSS_OMP(omp single)
        {
            csi atg=0, atw=0;
            for(csi b=0;b<B;b++) for(int u=0;u<T;u++) {
                const size_t s=(size_t)u*B+b;
                pl->posG[s]=atg; atg=(atg+pl->cntG[s]+PERM_WC-1)/PERM_WC*PERM_WC;
                pl->posW[s]=atw; atw=(atw+pl->cntW[s]+PERM_WC-1)/PERM_WC*PERM_WC;
            }
        }
        csi *qg=q+(size_t)t*B*2, *qw=qg+B;
        memcpy(qg,pl->posG+(size_t)t*B,(size_t)B*sizeof(csi));
        memcpy(qw,pl->posW+(size_t)t*B,(size_t)B*sizeof(csi));
        for(csi g=lo;g<hi;g++) { csi p=packed_of(f,g); pl->offG[qg[p>>PERM_SH]++]=(uint16_t)(p&PERM_MASK); }
        if(lo<hi) {
            csi c=comp_of(f,lo); idxmap map=component_map(f,c);
            for(csi p=lo;p<hi;p++) {
                while(p>=off[c+1]) { c++; map=component_map(f,c); }
                csi g=im_at(map,p-off[c]); pl->offW[qw[g>>PERM_SH]++]=(uint16_t)(g&PERM_MASK);
            }
        }
    }
    free(q);
    return pl;
}

static const vsdlss_perm_plan *perm_plan_get(const vsdlss_m3_factor *fc, int T)
{
    vsdlss_m3_factor *f=(vsdlss_m3_factor*)fc;
    if(T<1 || T>=VSDLSS_PERM_PLAN_SLOTS) return NULL;
    vsdlss_perm_plan *pl=atomic_load(&f->pplan[T]);
    if(pl) return pl;
    pl=perm_plan_build(f,T);
    if(!pl) return NULL;
    vsdlss_perm_plan *expect=NULL;
    if(!atomic_compare_exchange_strong(&f->pplan[T],&expect,pl)) { perm_plan_free(pl); pl=expect; }
    return pl;
}

/* Scratch area of at least len doubles, 64-byte aligned; NULL when a
 * concurrent solve holds it. */
static double *pbuf_acquire(const vsdlss_m3_factor *fc, size_t len)
{
    vsdlss_m3_factor *f=(vsdlss_m3_factor*)fc;
    if(atomic_exchange(&f->pbuf_busy,1)) return NULL;
    if(f->pbuf && f->pbuf_len<len) { free(f->pbuf); f->pbuf=NULL; f->pbuf_len=0; }
    if(!f->pbuf && len<=SIZE_MAX/sizeof(double)) {
        f->pbuf=(double*)vsdlss_big_malloc_aligned(64,len*sizeof(double));
        if(f->pbuf) f->pbuf_len=len;
    }
    if(!f->pbuf) atomic_store(&f->pbuf_busy,0);
    return f->pbuf;
}
static void pbuf_release(const vsdlss_m3_factor *fc)
{ atomic_store(&((vsdlss_m3_factor*)fc)->pbuf_busy,0); }

/* Pass 1 append of v to thread-local bucket b (cursor q[b], line buffer w). */
#define PERM_PUT(b,v) do { const csi k_=qt[b]++; double *ln_=w+(size_t)(b)*PERM_WC; \
        ln_[k_&(PERM_WC-1)]=(v); \
        if((k_&(PERM_WC-1))==PERM_WC-1) line_out(buf+(k_-(PERM_WC-1)),ln_,nt); } while(0)
/* Flush the partial line of every bucket after pass 1. */
static void perm_flush(double *buf, const double *w, const csi *qt, csi B, int nt)
{
    for(csi b=0;b<B;b++) {
        const csi k=qt[b], r=k&(PERM_WC-1);
        if(r) memcpy(buf+(k-r),w+(size_t)b*PERM_WC,(size_t)r*sizeof(double));
    }
    perm_fence(nt);
}

/* loc[c][i] = rhs[component_map(c)[i]] for all components; returns 1 when a
 * gathered value is not finite (the direct gather's check). */
static int perm2_gather(const vsdlss_m3_factor *f, const vsdlss_perm_plan *pl, const double *rhs,
                        double *buf, double *const *loc, csi *q)
{
    const csi n=f->n, B=pl->B, chunk=pl->chunk, *off=f->components->offset;
    const int nt=pl->nt, T=pl->T;
    int bad=0;
    VSDLSS_OMP(omp parallel num_threads(T) reduction(|:bad))
    {
#ifdef _OPENMP
        const int t=omp_get_thread_num();
#else
        const int t=0;
#endif
        const csi lo=(csi)t*chunk<n?(csi)t*chunk:n, hi=lo+chunk<n?lo+chunk:n;
        csi *qt=q+(size_t)t*B;
        double *stage=buf+pl->len+(size_t)t*((size_t)PERM_BLK+(size_t)B*PERM_WC), *w=stage+PERM_BLK;
        memcpy(qt,pl->posG+(size_t)t*B,(size_t)B*sizeof(csi));
        if(pl->bktG) { const uint16_t *bk=pl->bktG; for(csi g=lo;g<hi;g++) PERM_PUT(bk[g],rhs[g]); }
        else for(csi g=lo;g<hi;g++) PERM_PUT(packed_of(f,g)>>PERM_SH,rhs[g]);
        perm_flush(buf,w,qt,B,nt);
        VSDLSS_OMP(omp barrier)
        VSDLSS_OMP(omp for schedule(dynamic,4))
        for(csi b=0;b<B;b++) {
            const csi p0=b<<PERM_SH, pend=p0+PERM_BLK<n?p0+PERM_BLK:n;
            const csi c=comp_of(f,p0);
            if(pend<=off[c+1]) {                       /* block inside one component */
                for(int u=0;u<T;u++) {
                    const size_t s=(size_t)u*B+b; const csi k0=pl->posG[s], k1=k0+pl->cntG[s];
                    for(csi k=k0;k<k1;k++) stage[pl->offG[k]]=buf[k];
                }
                bad|=block_out(loc[c]+(p0-off[c]),stage,pend-p0,nt,1);
            } else {
                for(int u=0;u<T;u++) {
                    const size_t s=(size_t)u*B+b; const csi k0=pl->posG[s], k1=k0+pl->cntG[s];
                    for(csi k=k0;k<k1;k++) {
                        const csi p=p0+pl->offG[k], cc=comp_of(f,p); double v=buf[k];
                        loc[cc][p-off[cc]]=v; bad|=!isfinite(v);
                    }
                }
            }
        }
        perm_fence(nt);
    }
    return bad;
}

/* out[component_map(c)[i]] = loc[c][i] for all components. */
static void perm2_writeback(const vsdlss_m3_factor *f, const vsdlss_perm_plan *pl, double *buf,
                            double *const *loc, double *out, csi *q)
{
    const csi n=f->n, B=pl->B, chunk=pl->chunk, *off=f->components->offset;
    const int nt=pl->nt, T=pl->T;
    VSDLSS_OMP(omp parallel num_threads(T))
    {
#ifdef _OPENMP
        const int t=omp_get_thread_num();
#else
        const int t=0;
#endif
        const csi lo=(csi)t*chunk<n?(csi)t*chunk:n, hi=lo+chunk<n?lo+chunk:n;
        csi *qt=q+(size_t)t*B;
        double *stage=buf+pl->len+(size_t)t*((size_t)PERM_BLK+(size_t)B*PERM_WC), *w=stage+PERM_BLK;
        memcpy(qt,pl->posW+(size_t)t*B,(size_t)B*sizeof(csi));
        if(lo<hi) {
            csi c=comp_of(f,lo);
            while(lo>=off[c+1]) c++;
            csi p=lo;
            while(p<hi) {                              /* one component slice at a time */
                const csi e=off[c+1]<hi?off[c+1]:hi, base=off[c];
                const double *l=loc[c];
                if(pl->bktW) { const uint16_t *bk=pl->bktW; for(;p<e;p++) PERM_PUT(bk[p],l[p-base]); }
                else { const idxmap map=component_map(f,c); for(;p<e;p++) PERM_PUT(im_at(map,p-base)>>PERM_SH,l[p-base]); }
                c++;
            }
        }
        perm_flush(buf,w,qt,B,nt);
        VSDLSS_OMP(omp barrier)
        VSDLSS_OMP(omp for schedule(dynamic,4))
        for(csi b=0;b<B;b++) {
            const csi p0=b<<PERM_SH, len=p0+PERM_BLK<n?PERM_BLK:n-p0;
            for(int u=0;u<T;u++) {
                const size_t s=(size_t)u*B+b; const csi k0=pl->posW[s], k1=k0+pl->cntW[s];
                for(csi k=k0;k<k1;k++) stage[pl->offW[k]]=buf[k];
            }
            block_out(out+p0,stage,len,nt,0);
        }
        perm_fence(nt);
    }
}

static vsdlss_status solve_common(const vsdlss_m3_factor *factor,
                                  const double *rhs, double *solution, int internal)
{
    const int inplace=internal==2;
    vsdlss_status status=VSDLSS_OK, *results=NULL; solve_ws *ws=NULL; double **loc=NULL;
    const vsdlss_perm_plan *pl=NULL; double *pbuf=NULL; csi *pq=NULL;
    csi c, taken=0;
    const int lg=vsdlss_ledger_level(); const double lstart=lg?vsdlss_ledger_now():0; double tl=lstart;
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
    solve_ctx ctx={factor,rhs,solution,ws,internal,loc,0,inplace};
    if(inverse && !factor->disk_mode && factor->n>=PERM_MIN && perm2_on()) {
        /* Two-pass gather here, two-pass write-back below (plan, scratch
         * vector and queues permitting; otherwise the direct loops). */
        int T=vsdlss_parallel_width((double)factor->n*4);
        pl=perm_plan_get(factor,T);
        if(pl) pq=(csi*)malloc((size_t)pl->T*(size_t)pl->B*sizeof(csi));
        if(pl && pq) pbuf=pbuf_acquire(factor,perm_scratch_len(pl));
        if(pbuf) {
            double tg=trace_now();
            LEDGER_MARK(LG_SETUP,tl);
            int bad=perm2_gather(factor,pl,rhs,pbuf,loc,pq);
            TRACE("solve: two-pass gather",tg);
            LEDGER_MARK(LG_PERM_GATHER,tl);
            if(bad) { status=VSDLSS_ERR_NONFINITE; goto done; }
            ctx.pregathered=1;
        }
    }
    LEDGER_MARK(LG_SETUP,tl);
    double before[LG_OTHER];
    if(lg) memcpy(before,vsdlss_ledger.wall,sizeof before);
    run_components(factor,factor->disk_mode,solve_local,&ctx,results);
    if(lg) {
        /* Concurrent components: phase sums exceed the step's wall time;
         * scale this solve's share of them to it. */
        double now_=vsdlss_ledger_now(), wall=now_-tl, sum=0;
        for(int k=0;k<LG_OTHER;k++) sum+=vsdlss_ledger.wall[k]-before[k];
        if(sum>wall && sum>0)
            for(int k=0;k<LG_OTHER;k++)
                vsdlss_ledger.wall[k]=before[k]+(vsdlss_ledger.wall[k]-before[k])*wall/sum;
        tl=now_;
    }
    for(c=0;c<factor->count;c++) if(results[c]!=VSDLSS_OK) { status=results[c]; goto done; }
    double t0=trace_now();
    if(inplace) ;                               /* solution already in place */
    else if(ctx.pregathered) perm2_writeback(factor,pl,pbuf,loc,solution,pq);
    else if(inverse) write_back_inverse(&ctx);
    else run_components(factor,factor->disk_mode,scatter_local,&ctx,results);
    TRACE("solve: scatter",t0);
    LEDGER_MARK(LG_WRITEBACK,tl);
done:
    if(pbuf) pbuf_release(factor);
    free(pq);
    if(ws) for(c=0;c<taken;c++) ws_release(factor,c,ws+c);
    free(ws); free(results); free(loc);
    if(lg) {
        double tot=vsdlss_ledger_now()-lstart, known=0;
        for(int k=0;k<LG_OTHER;k++) known+=vsdlss_ledger.wall[k];
        vsdlss_ledger.wall[LG_TOTAL]+=tot;
        /* LG_OTHER as the running remainder, so the phases always add up. */
        vsdlss_ledger.wall[LG_OTHER]=vsdlss_ledger.wall[LG_TOTAL]-known;
        vsdlss_ledger.solves++;
    }
    return status;
}

vsdlss_status vsdlss_m3_solve(const vsdlss_m3_factor *factor,
                              const double *rhs, double *solution)
{ return solve_common(factor,rhs,solution,0); }

/* Packed order, in place: x holds the RHS on entry and the solution on
 * success.  A non-finite RHS is reported before x is modified; any later
 * failure (not expected for a valid factor) may leave x partially updated,
 * unlike vsdlss_m3_solve_packed.  Saves the copy in and the copy out. */
vsdlss_status vsdlss_m3_solve_packed_inplace(const vsdlss_m3_factor *factor, double *x)
{
    if(!factor || factor->disk_mode) return VSDLSS_ERR_INVALID;
    return solve_common(factor,x,x,2);
}

vsdlss_status vsdlss_m3_solve_internal(const vsdlss_m3_factor *factor,
                                       const double *rhs, double *solution)
{ return solve_common(factor,rhs,solution,1); }

vsdlss_status vsdlss_m3_internal_order(const vsdlss_m3_factor *factor, csi *perm)
{
    if(!factor || !factor->components || !factor->component || !perm) return VSDLSS_ERR_INVALID;
    for(csi c=0;c<factor->count;c++) {
        const csi cn=factor->component[c].n; const idxmap map=component_map(factor,c);
        csi *dst=perm+factor->components->offset[c];
        for(csi i=0;i<cn;i++) dst[i]=im_at(map,i);
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
        const idxmap map=component_map(f,c), cm=core_map_of(cf);
        double *local=malloc((size_t)(cn?cn:1)*sizeof(double));
        double *perm=core?malloc((size_t)core*(size_t)nrhs*sizeof(double)):NULL;
        double *saved=cnt?malloc((size_t)cnt*(size_t)nrhs*sizeof(double)):NULL;
        if(!local||(core&&!perm)||(cnt&&!saved)){st=VSDLSS_ERR_OOM;goto next;}
        for(csi r0=0;r0<nrhs&&st==VSDLSS_OK;r0++){
            const double *b=rhs+(size_t)r0*ldrhs; int bad=0;
            for(csi k=0;k<cn;k++){local[k]=b[im_at(map,k)];bad|=!isfinite(local[k]);}
            if(bad){st=VSDLSS_ERR_NONFINITE;break;}
            st=vsdlss_reduce_forward_inplace(r,local,cnt?saved+(size_t)r0*cnt:NULL);
            if(st==VSDLSS_OK)for(csi k=0;k<core;k++)perm[(size_t)r0*core+k]=local[im_at(cm,k)];
        }
        if(st==VSDLSS_OK&&core)st=vsdlss_sn_solve_batch(cf->numeric,nrhs,perm,core);
        for(csi r0=0;r0<nrhs&&st==VSDLSS_OK;r0++){
            double *xo=dest+(size_t)r0*lddest;
            for(csi k=0;k<core;k++)local[im_at(cm,k)]=perm[(size_t)r0*core+k];
            st=vsdlss_reduce_backward_inplace(r,cnt?saved+(size_t)r0*cnt:NULL,local);
            if(st==VSDLSS_OK)for(csi k=0;k<cn;k++)xo[im_at(map,k)]=local[k];
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
