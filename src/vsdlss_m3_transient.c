/* Transient stepping inside the solver: vsdlss_m3_transient_*.
 *
 * Backward Euler with a fixed step: A x_{k+1} = b_{k+1}, A = G + C/h (the
 * factor's matrix), b_{k+1} = Ch x_k + s_{k+1}, Ch = C/h.  The caller gives Ch
 * once (diagonal: grounded capacitors; off-diagonal: the few two-terminal
 * ones), the nodes that get source values each step, and the nodes whose
 * next right-hand side depends on the solution through state the caller
 * keeps (dep nodes, values from a callback).  Everything runs in the factor's
 * packed order: no gather or scatter per step.
 *
 * b formula (fixed, so that steps can be checked bitwise against solving each
 * b with vsdlss_m3_solve_packed): b[v] = d[v] x[v] (+ Ch[v][u] x[u] over the
 * off-diagonal partners u, ascending packed position) + s[v], where s[v]
 * starts at 0.0, adds the step's source values at v in registration order,
 * then the dep values at v in dep order.
 *
 * Components whose reduction allows it (vsdlss_reduce_tr_fusable) fuse the
 * backward pass of step k with the forward pass of step k+1 unit by unit
 * (see vsdlss_reduction.c): an iteration is
 *   A: core solve of step k, then every unit backward k (+ forward k+1);
 *   callback(k, x_k) -> dep values for step k+1;
 *   B: right-hand sides of the special vertices, late units' forward k+1,
 *      deferred core updates.
 * Other components solve each step separately (P1: the same arithmetic as
 * vsdlss_m3_solve_packed_inplace on b), in B for the next step.  Either way
 * each step's solution is bitwise that of solving its b separately. */
#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Fusion: 0 off, 1 (default) where at most half of a component's records are
 * in late units (with more, the late units' second pass costs what fusion
 * saves: measured on ibmpg3t, 4 threads, 66 -> 86 ms per step), 2 always.
 * -1: VSDLSS_TR_FUSE. */
int vsdlss_m3_transient_fuse = -1;
long vsdlss_m3_transient_fused_steps;    /* tests: fused component iterations */

static int fuse_mode(void)
{
    if(vsdlss_m3_transient_fuse<0) { const char *e=getenv("VSDLSS_TR_FUSE"); vsdlss_m3_transient_fuse=e&&*e?atoi(e):1; }
    return vsdlss_m3_transient_fuse;
}

typedef struct {
    int fused;
    vsdlss_tr_plan *plan;
    unsigned char *special;      /* local vertex: right-hand side needs other values */
    csi nsp, *sp;                /* special vertices, ascending (local) */
    csi *op, *oc; double *ov;    /* their off-diagonal partners (local, ascending) and Ch values */
    double *core;                /* non-contiguous core: gathered core vector */
    double *spb;                 /* right-hand sides of the special vertices (separate steps) */
} tr_comp;

struct vsdlss_m3_transient {
    const vsdlss_m3_factor *f;
    csi n, k;
    int started, asked;          /* asked: the callback has given the dep values for step k+1 */
    double *x, *w, *s, *d;       /* packed order */
    csi *slot;                   /* caller node -> packed position when the factor has no inverse map */
    csi nsrc, *srcpos, *srcord;  /* sources sorted by (position, index): srcord[t] the index, srcpos[t] its position */
    csi nreset, *reset;          /* positions of sources and dep nodes (s is rebuilt there) */
    csi ndep, *deppos; double *dep;
    tr_comp *comp;
    /* the current run */
    void (*cb)(void *, csi, const double *, double *); void *ctx;
    int next;
};

static inline csi packed_pos(const vsdlss_m3_transient *T, csi g)
{
    const vsdlss_m3_factor *f=T->f;
    if(T->slot) return T->slot[g];
    const int sh=f->inv_shift;
    const uint64_t code=f->inv32?(uint64_t)f->inv32[g]:f->inv64[g];
    return f->components->offset[code>>sh]+(csi)(code&(((uint64_t)1<<sh)-1));
}

static csi comp_of_pos(const vsdlss_m3_factor *f, csi p)
{
    const csi *off=f->components->offset; csi lo=0, hi=f->count-1;
    while(lo<hi) { csi mid=(lo+hi+1)/2; if(off[mid]<=p) lo=mid; else hi=mid-1; }
    return lo;
}

static inline csi core_vertex(const vsdlss_m3_component_factor *cf, csi k)
{ return cf->core_map32?(csi)cf->core_map32[k]:cf->core_map[k]; }

void vsdlss_m3_transient_free(vsdlss_m3_transient *T)
{
    if(!T) return;
    if(T->comp) for(csi c=0;c<T->f->count;c++) {
        tr_comp *q=T->comp+c;
        vsdlss_reduce_tr_plan_free(q->plan);
        free(q->special); free(q->sp); free(q->op); free(q->oc); free(q->ov); free(q->core); free(q->spb);
    }
    free(T->comp); free(T->x); free(T->w); free(T->s); free(T->d); free(T->slot);
    free(T->srcpos); free(T->srcord); free(T->reset); free(T->deppos); free(T->dep);
    free(T);
}

typedef struct { csi a, b; double v; csi i; } pair_t;   /* a, b: packed; i: input order */
static int cmp_pair(const void *x, const void *y)
{
    const pair_t *p=(const pair_t *)x, *q=(const pair_t *)y;
    if(p->a!=q->a) return p->a<q->a?-1:1;
    if(p->b!=q->b) return p->b<q->b?-1:1;
    return p->i<q->i?-1:p->i>q->i;
}
typedef struct { csi p, i; } pos_t;
static int cmp_pos(const void *x, const void *y)
{
    const pos_t *p=(const pos_t *)x, *q=(const pos_t *)y;
    if(p->p!=q->p) return p->p<q->p?-1:1;
    return p->i<q->i?-1:p->i>q->i;
}

vsdlss_status vsdlss_m3_transient_create(const vsdlss_m3_factor *f, const vsdlss *Ch,
                                         csi nsrc, const csi *src_node, csi ndep, const csi *dep_node,
                                         vsdlss_m3_transient **out)
{
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(!f || !Ch || nsrc<0 || ndep<0 || (nsrc && !src_node) || (ndep && !dep_node)) return VSDLSS_ERR_INVALID;
    if(f->disk_mode) return VSDLSS_ERR_UNSUPPORTED;
    if(vsdlss_validate_upper_csc(Ch)!=VSDLSS_OK || Ch->n!=f->n || Ch->m!=f->n) return VSDLSS_ERR_INVALID;
    const csi n=f->n;
    for(csi j=0;j<nsrc;j++) if(src_node[j]<0 || src_node[j]>=n) return VSDLSS_ERR_INVALID;
    for(csi j=0;j<ndep;j++) if(dep_node[j]<0 || dep_node[j]>=n) return VSDLSS_ERR_INVALID;
    for(csi c=0;c<f->count;c++) if(f->component[c].reduction && f->component[c].reduction->records) return VSDLSS_ERR_UNSUPPORTED;
    for(csi p=0;p<Ch->p[n];p++) if(!isfinite(Ch->x[p])) return VSDLSS_ERR_NONFINITE;

    vsdlss_status st=VSDLSS_OK;
    pair_t *pr=NULL; pos_t *ps=NULL;
    vsdlss_m3_transient *T=(vsdlss_m3_transient *)calloc(1,sizeof(*T));
    if(!T) return VSDLSS_ERR_OOM;
    T->f=f; T->n=n; T->nsrc=nsrc; T->ndep=ndep;
    T->comp=(tr_comp *)calloc((size_t)f->count,sizeof(tr_comp));
    T->x=(double *)calloc((size_t)(n?n:1),sizeof(double));
    T->w=(double *)calloc((size_t)(n?n:1),sizeof(double));
    T->s=(double *)calloc((size_t)(n?n:1),sizeof(double));
    T->d=(double *)calloc((size_t)(n?n:1),sizeof(double));
    T->dep=(double *)calloc((size_t)(ndep?ndep:1),sizeof(double));
    if(!T->comp || !T->x || !T->w || !T->s || !T->d || !T->dep) goto oom;
    if(!f->inv32 && !f->inv64) {
        T->slot=(csi *)malloc((size_t)(n?n:1)*sizeof(csi));
        if(!T->slot) goto oom;
        if((st=vsdlss_m3_export_packed_slots(f,T->slot,n))!=VSDLSS_OK) goto fail;
    }
    /* Ch: diagonal into d; off-diagonal pairs (both directions), merged */
    csi np=0;
    for(csi j=0;j<n;j++) for(csi p=Ch->p[j];p<Ch->p[j+1];p++) np+=Ch->i[p]!=j;
    pr=(pair_t *)malloc((size_t)(np?2*np:1)*sizeof(pair_t));
    if(!pr) goto oom;
    np=0;
    for(csi j=0;j<n;j++) {
        const csi pj=packed_pos(T,j);
        for(csi p=Ch->p[j];p<Ch->p[j+1];p++) {
            const csi i=Ch->i[p];
            if(i==j) { T->d[pj]+=Ch->x[p]; continue; }
            const csi pi=packed_pos(T,i);
            if(comp_of_pos(f,pi)!=comp_of_pos(f,pj)) { st=VSDLSS_ERR_INVALID; goto fail; }  /* would couple components */
            pr[np].a=pi; pr[np].b=pj; pr[np].v=Ch->x[p]; pr[np].i=np; np++;
            pr[np].a=pj; pr[np].b=pi; pr[np].v=Ch->x[p]; pr[np].i=np; np++;
        }
    }
    qsort(pr,(size_t)np,sizeof(pair_t),cmp_pair);
    csi mp=0;
    for(csi t=0;t<np;t++) { if(mp && pr[mp-1].a==pr[t].a && pr[mp-1].b==pr[t].b) pr[mp-1].v+=pr[t].v; else pr[mp++]=pr[t]; }
    np=mp;
    /* sources and dep nodes */
    T->srcpos=(csi *)malloc((size_t)(nsrc?nsrc:1)*sizeof(csi));
    T->srcord=(csi *)malloc((size_t)(nsrc?nsrc:1)*sizeof(csi));
    T->deppos=(csi *)malloc((size_t)(ndep?ndep:1)*sizeof(csi));
    ps=(pos_t *)malloc((size_t)(nsrc+ndep?nsrc+ndep:1)*sizeof(pos_t));
    T->reset=(csi *)malloc((size_t)(nsrc+ndep?nsrc+ndep:1)*sizeof(csi));
    if(!T->srcpos || !T->srcord || !T->deppos || !ps || !T->reset) goto oom;
    for(csi j=0;j<nsrc;j++) { T->srcpos[j]=packed_pos(T,src_node[j]); ps[j].p=T->srcpos[j]; ps[j].i=j; }
    qsort(ps,(size_t)nsrc,sizeof(pos_t),cmp_pos);
    for(csi j=0;j<nsrc;j++) T->srcord[j]=ps[j].i;
    for(csi j=0;j<ndep;j++) T->deppos[j]=packed_pos(T,dep_node[j]);
    for(csi j=0;j<nsrc;j++) ps[j].p=T->srcpos[j];
    for(csi t=0;t<nsrc;t++) T->srcpos[t]=packed_pos(T,src_node[T->srcord[t]]);   /* from now on in sorted order */
    for(csi j=0;j<ndep;j++) ps[nsrc+j].p=T->deppos[j];
    for(csi j=0;j<nsrc+ndep;j++) ps[j].i=0;
    qsort(ps,(size_t)(nsrc+ndep),sizeof(pos_t),cmp_pos);
    for(csi j=0;j<nsrc+ndep;j++) if(!T->nreset || T->reset[T->nreset-1]!=ps[j].p) T->reset[T->nreset++]=ps[j].p;
    /* components: special vertices, fusion plan */
    for(csi c=0;c<f->count;c++) {
        const vsdlss_m3_component_factor *cf=f->component+c;
        tr_comp *q=T->comp+c;
        const csi off=f->components->offset[c], cn=cf->n;
        q->special=(unsigned char *)calloc((size_t)(cn?cn:1),1);
        if(!q->special) goto oom;
        for(csi t=0;t<np;t++) if(pr[t].a>=off && pr[t].a<off+cn) q->special[pr[t].a-off]=1;
        for(csi j=0;j<ndep;j++) if(T->deppos[j]>=off && T->deppos[j]<off+cn) q->special[T->deppos[j]-off]=1;
        for(csi v=0;v<cn;v++) q->nsp+=q->special[v];
        q->sp=(csi *)malloc((size_t)(q->nsp?q->nsp:1)*sizeof(csi));
        q->op=(csi *)calloc((size_t)q->nsp+1,sizeof(csi));
        if(!q->sp || !q->op) goto oom;
        csi m=0;
        for(csi v=0;v<cn;v++) if(q->special[v]) q->sp[m++]=v;
        csi lo=0; while(lo<np && pr[lo].a<off) lo++;       /* this component's pairs are contiguous */
        csi hi=lo; while(hi<np && pr[hi].a<off+cn) hi++;
        q->oc=(csi *)malloc((size_t)(hi-lo?hi-lo:1)*sizeof(csi));
        q->ov=(double *)malloc((size_t)(hi-lo?hi-lo:1)*sizeof(double));
        if(!q->oc || !q->ov) goto oom;
        for(csi a=0,t=lo;a<q->nsp;a++) {     /* pairs and special vertices both ascend */
            q->op[a+1]=q->op[a];
            for(;t<hi && pr[t].a-off==q->sp[a];t++) { q->oc[q->op[a+1]]=pr[t].b-off; q->ov[q->op[a+1]]=pr[t].v; q->op[a+1]++; }
        }
        const vsdlss_reduction *r=cf->reduction;
        const int mode=fuse_mode();
        if(mode && cf->core_contig && (r->core_n==0 || cf->numeric) && vsdlss_reduce_tr_fusable(r)) {
            st=vsdlss_reduce_tr_plan(r,q->special,&q->plan);
            if(st==VSDLSS_OK) {
                csi rec=0, late=0;
                vsdlss_reduce_tr_counts(q->plan,NULL,NULL,&rec,&late);
                if(mode==1 && 2*late>rec) { vsdlss_reduce_tr_plan_free(q->plan); q->plan=NULL; }
                else q->fused=1;
            } else if(st!=VSDLSS_ERR_UNSUPPORTED) goto fail;
            st=VSDLSS_OK;
        }
        if(!q->fused && r->core_n && !cf->core_contig) {
            q->core=(double *)malloc((size_t)r->core_n*sizeof(double));
            if(!q->core) goto oom;
        }
        if(!q->fused) {
            q->spb=(double *)malloc((size_t)(q->nsp?q->nsp:1)*sizeof(double));
            if(!q->spb) goto oom;
        }
    }
    free(pr); free(ps);
    *out=T;
    return VSDLSS_OK;
oom:
    st=VSDLSS_ERR_OOM;
fail:
    free(pr); free(ps);
    vsdlss_m3_transient_free(T);
    return st;
}

static int all_finite(const double *v, csi m)
{
    int bad=0, nt=vsdlss_parallel_width((double)m*4); (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static) reduction(|:bad))
    for(csi k=0;k<m;k++) bad|=!isfinite(v[k]);
    return !bad;
}

vsdlss_status vsdlss_m3_transient_start(vsdlss_m3_transient *T, const double *x0)
{
    if(!T || !x0) return VSDLSS_ERR_INVALID;
    const csi n=T->n;
    if(!all_finite(x0,n)) return VSDLSS_ERR_NONFINITE;
    int nt=vsdlss_parallel_width((double)n*4); (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
    for(csi g=0;g<n;g++) T->x[packed_pos(T,g)]=x0[g];
    memset(T->dep,0,(size_t)(T->ndep?T->ndep:1)*sizeof(double));
    T->k=0; T->started=1; T->asked=0;
    return VSDLSS_OK;
}

/* s for one step: sources (registration order per position), not the dep
 * values.  In parallel: the reset positions are distinct; the sorted sources
 * are cut where the position changes, so every position is summed by one
 * thread in registration order, as serially (server, 61M nodes with 50M
 * sources: the serial loops took most of a step). */
static void scatter_sources(vsdlss_m3_transient *T, const double *row)
{
    double *s=T->s; const csi *pos=T->srcpos, *ord=T->srcord, ns=T->nsrc, nr=T->nreset, *reset=T->reset;
    int nt=vsdlss_parallel_width((double)(ns+nr)*4); (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
#ifdef _OPENMP
        const int id=omp_get_thread_num(), nth=omp_get_num_threads();
#else
        const int id=0, nth=1;
#endif
        for(csi t=nr*id/nth;t<nr*(id+1)/nth;t++) s[reset[t]]=0.0;
        VSDLSS_OMP(omp barrier)
        csi lo=ns*id/nth, hi=ns*(id+1)/nth;
        while(lo>0 && lo<ns && pos[lo]==pos[lo-1]) lo++;
        while(hi>0 && hi<ns && pos[hi]==pos[hi-1]) hi++;
        for(csi t=lo;t<hi;t++) s[pos[t]]+=row[ord[t]];
    }
}
static void add_dep(vsdlss_m3_transient *T)
{
    for(csi j=0;j<T->ndep;j++) T->s[T->deppos[j]]+=T->dep[j];
}

/* b of component-local vertex v (x, w, d, s: the component's slices). */
static inline double rhs_special(const tr_comp *q, csi a, const double *x, const double *d, const double *s)
{
    const csi v=q->sp[a];
    double b=d[v]*x[v];
    for(csi t=q->op[a];t<q->op[a+1];t++) b+=q->ov[t]*x[q->oc[t]];
    return b+s[v];
}
/* w[v] = b[v] at the special vertices (in parallel: each writes its own) */
static void rhs_specials(const tr_comp *q, const double *x, double *w, const double *d, const double *s)
{
    int nt=vsdlss_parallel_width((double)q->nsp*16); (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
    for(csi a=0;a<q->nsp;a++) w[q->sp[a]]=rhs_special(q,a,x,d,s);
}
static void rhs_all(const tr_comp *q, csi cn, const double *x, double *w, const double *d, const double *s)
{
    int nt=vsdlss_parallel_width((double)cn*2); (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
    for(csi v=0;v<cn;v++) w[v]=d[v]*x[v]+s[v];
    rhs_specials(q,x,w,d,s);
}

static vsdlss_status core_finite(const double *v, csi m)
{
    int bad=0, nt=vsdlss_parallel_width((double)m*4); (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static) reduction(|:bad))
    for(csi k=0;k<m;k++) bad|=!isfinite(v[k]);
    return bad?VSDLSS_ERR_NONFINITE:VSDLSS_OK;
}

/* One separate step of component c, in place on x (the arithmetic of
 * vsdlss_m3_solve_packed_inplace on b): the special vertices' b first (they
 * read other entries of x), then b = d x + s elsewhere, then the solve. */
static vsdlss_status solve_separate(vsdlss_m3_transient *T, csi c)
{
    const vsdlss_m3_component_factor *cf=T->f->component+c;
    const tr_comp *q=T->comp+c;
    const vsdlss_reduction *r=cf->reduction;
    const csi off=T->f->components->offset[c], cn=cf->n, core=r->core_n;
    double *w=T->x+off; const double *d=T->d+off, *s=T->s+off;
    {
        int nt=vsdlss_parallel_width((double)q->nsp*16); (void)nt;
        VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
        for(csi a=0;a<q->nsp;a++) q->spb[a]=rhs_special(q,a,w,d,s);
        nt=vsdlss_parallel_width((double)cn*2);
        VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
        for(csi v=0;v<cn;v++) w[v]=d[v]*w[v]+s[v];
        for(csi a=0;a<q->nsp;a++) w[q->sp[a]]=q->spb[a];
    }
    vsdlss_status st=vsdlss_reduce_forward_inplace(r,w,NULL);
    if(st!=VSDLSS_OK) return st;
    if(core) {
        if(cf->core_contig) {
            if((st=vsdlss_sn_solve_inplace(cf->numeric,w+r->count))!=VSDLSS_OK) return st;
            if((st=core_finite(w+r->count,core))!=VSDLSS_OK) return st;
        } else {
            int nt=vsdlss_parallel_width((double)core*4); (void)nt;
            VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
            for(csi k=0;k<core;k++) q->core[k]=w[core_vertex(cf,k)];
            if((st=vsdlss_sn_solve_inplace(cf->numeric,q->core))!=VSDLSS_OK) return st;
            if((st=core_finite(q->core,core))!=VSDLSS_OK) return st;
            VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
            for(csi k=0;k<core;k++) w[core_vertex(cf,k)]=q->core[k];
        }
    }
    return vsdlss_reduce_backward_inplace(r,NULL,w);
}

/* Before the first iteration: fused components run the forward pass of the
 * next step, the others solve it. */
static vsdlss_status phase_pro(void *vt, csi c)
{
    vsdlss_m3_transient *T=(vsdlss_m3_transient *)vt;
    const tr_comp *q=T->comp+c;
    if(!q->fused) return solve_separate(T,c);
    const vsdlss_m3_component_factor *cf=T->f->component+c;
    const csi off=T->f->components->offset[c];
    rhs_all(q,cf->n,T->x+off,T->w+off,T->d+off,T->s+off);
    return vsdlss_reduce_forward_inplace(cf->reduction,T->w+off,NULL);
}

static vsdlss_status phase_a(void *vt, csi c)
{
    vsdlss_m3_transient *T=(vsdlss_m3_transient *)vt;
    const tr_comp *q=T->comp+c;
    if(!q->fused) return VSDLSS_OK;
    const vsdlss_m3_component_factor *cf=T->f->component+c;
    const vsdlss_reduction *r=cf->reduction;
    const csi off=T->f->components->offset[c], cn=cf->n, cnt=r->count, core=r->core_n;
    double *x=T->x+off, *w=T->w+off; const double *d=T->d+off, *s=T->s+off;
    vsdlss_status st;
    if(core) {
        memcpy(x+cnt,w+cnt,(size_t)core*sizeof(double));
        if((st=vsdlss_sn_solve_inplace(cf->numeric,x+cnt))!=VSDLSS_OK) return st;
        if((st=core_finite(x+cnt,core))!=VSDLSS_OK) return st;
        if(T->next) {
            const unsigned char *sp=q->special;
            int nt=vsdlss_parallel_width((double)core*2); (void)nt;
            VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
            for(csi v=cnt;v<cn;v++) if(!sp[v]) w[v]=d[v]*x[v]+s[v];
        }
    }
    st=vsdlss_reduce_tr_step_a(r,q->plan,x,w,d,s,q->special,T->next);
    if(st==VSDLSS_OK && T->next) {
        VSDLSS_OMP(omp atomic)
        vsdlss_m3_transient_fused_steps++;
    }
    return st;
}

static vsdlss_status phase_b(void *vt, csi c)
{
    vsdlss_m3_transient *T=(vsdlss_m3_transient *)vt;
    const tr_comp *q=T->comp+c;
    if(!q->fused) return solve_separate(T,c);
    const vsdlss_m3_component_factor *cf=T->f->component+c;
    const csi off=T->f->components->offset[c];
    const double *x=T->x+off, *d=T->d+off, *s=T->s+off; double *w=T->w+off;
    rhs_specials(q,x,w,d,s);
    return vsdlss_reduce_tr_step_b(cf->reduction,q->plan,w);
}

static vsdlss_status run_phase(vsdlss_m3_transient *T, vsdlss_status (*fn)(void *, csi))
{
    const csi count=T->f->count;
    vsdlss_status small[8], *res=count<=8?small:(vsdlss_status *)malloc((size_t)count*sizeof(vsdlss_status));
    if(!res) return VSDLSS_ERR_OOM;
    for(csi c=0;c<count;c++) res[c]=VSDLSS_OK;
    vsdlss_m3_run_components(T->f,fn,T,res);
    vsdlss_status st=VSDLSS_OK;
    for(csi c=0;c<count && st==VSDLSS_OK;c++) st=res[c];
    if(res!=small) free(res);
    return st;
}

static vsdlss_status ask(vsdlss_m3_transient *T)
{
    if(T->cb) T->cb(T->ctx,T->k,T->x,T->ndep?T->dep:NULL);
    T->asked=1;
    for(csi j=0;j<T->ndep;j++) if(!isfinite(T->dep[j])) return VSDLSS_ERR_NONFINITE;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_m3_transient_run(vsdlss_m3_transient *T, csi nsteps, const double *src,
                                      void (*cb)(void *ctx, csi k, const double *x_packed, double *dep_next),
                                      void *ctx)
{
    if(!T || !T->started || nsteps<0 || (nsteps && T->nsrc && !src) || (T->ndep && !cb)) return VSDLSS_ERR_INVALID;
    if(nsteps && !all_finite(src,nsteps*T->nsrc)) return VSDLSS_ERR_NONFINITE;
    if(!nsteps) return VSDLSS_OK;
    T->cb=cb; T->ctx=ctx;
    vsdlss_status st=VSDLSS_OK;
    if(!T->asked && (st=ask(T))!=VSDLSS_OK) goto fail;
    scatter_sources(T,src); add_dep(T);
    if((st=run_phase(T,phase_pro))!=VSDLSS_OK) goto fail;
    for(csi j=1;j<=nsteps;j++) {
        T->next=j<nsteps;
        if(T->next) scatter_sources(T,src+j*T->nsrc);
        if((st=run_phase(T,phase_a))!=VSDLSS_OK) goto fail;
        T->k++;
        if((st=ask(T))!=VSDLSS_OK) goto fail;
        if(T->next) {
            add_dep(T);
            if((st=run_phase(T,phase_b))!=VSDLSS_OK) goto fail;
        }
    }
    T->cb=NULL; T->ctx=NULL;
    return VSDLSS_OK;
fail:
    T->started=0; T->cb=NULL; T->ctx=NULL;
    return st;
}

vsdlss_status vsdlss_m3_transient_get(const vsdlss_m3_transient *T, csi nidx, const csi *node, double *out)
{
    if(!T || !out || !T->started) return VSDLSS_ERR_INVALID;
    if(nidx<0 || !node) { for(csi g=0;g<T->n;g++) out[g]=T->x[packed_pos(T,g)]; return VSDLSS_OK; }
    for(csi t=0;t<nidx;t++) if(node[t]<0 || node[t]>=T->n) return VSDLSS_ERR_INVALID;
    for(csi t=0;t<nidx;t++) out[t]=T->x[packed_pos(T,node[t])];
    return VSDLSS_OK;
}

vsdlss_status vsdlss_m3_transient_info(const vsdlss_m3_transient *T, csi *fused_components,
                                       csi *units, csi *late_units)
{
    if(!T) return VSDLSS_ERR_INVALID;
    csi fc=0, u=0, l=0;
    for(csi c=0;c<T->f->count;c++) if(T->comp[c].fused) {
        csi a, b; vsdlss_reduce_tr_counts(T->comp[c].plan,&a,&b,NULL,NULL); fc++; u+=a; l+=b;
    }
    if(fused_components) *fused_components=fc;
    if(units) *units=u;
    if(late_units) *late_units=l;
    return VSDLSS_OK;
}
