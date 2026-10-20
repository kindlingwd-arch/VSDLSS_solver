/* Low-rank modification of an M3 factor (ECO changes): vsdlss_m3_modify.
 *
 * The factor of A becomes the factor of A + dA without refactoring, for a
 * symmetric dA whose off-diagonal entries lie in the structure of the
 * factor (changing the value of existing couplings, adding to or taking from
 * the diagonal).  dA is split into rank-1 terms
 *     dA = sum over pairs (a,b): g (e_a - e_b)(e_a - e_b)'  +  sum_i r_i e_i e_i',
 * g = -dA(a,b), r_i = dA(i,i) + sum_b dA(i,b); g or r > 0 is an update, < 0
 * a downdate.  For a conductance change dg between a and b (dA(a,b) = -dg,
 * dA(a,a) = dA(b,b) = +dg) that is the single term g = dg.  Updates run
 * first, so every intermediate matrix lies between A + dA and A + (positive
 * part of dA) and is positive definite whenever the result is.
 *
 * A component's factor is one Cholesky factorization in elimination order:
 * the low-degree records (unit L, pivots D) at positions 0..count-1, then the
 * core (L L') at count + k.  A term w = e_a - e_b with b in the structure of
 * column a changes exactly the columns on the path from a to the root of the
 * elimination tree and leaves the structure alone (Davis & Hager 1999): the
 * vector left after column j lies in the structure of column j.  The records
 * take method C1 of Gill, Golub, Murray & Saunders (1974) on L D L'; what
 * reaches the core is a rank-1 change alpha z z' of the core's Schur
 * complement, applied to the supernodal L L' column by column in the
 * hyperbolic-rotation form (as CSparse's cs_updown).  The work is the
 * entries of the columns on the path, normally far below one solve.
 *
 * All or nothing: each column is saved before it changes and every change of
 * the call is undone when a term fails (a downdate that would lose positive
 * definiteness, a non-finite value, out of memory).  The level schedule's
 * forward plan holds copies of tail multipliers and is patched once the call
 * has succeeded.  Not concurrent with solves on the same factor. */
#include "vsdlss_m3_internal.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/* A pivot that loses more than this fraction of itself to a downdate is
 * taken as loss of positive definiteness (about 45 ulps left). */
#define ECO_TOL 1e-14
/* One neighbour-offset checkpoint per 2^CK_SHIFT records of a segment. */
#define CK_SHIFT 6

/* Per component, built on the first modification and kept with the factor. */
struct vsdlss_m3_eco {
    csi *pos;        /* local vertex -> elimination position; NULL: the identity */
    csi *ck_ptr;     /* pk_count + 1: each segment's first checkpoint in ck */
    csi *ck;         /* neighbour offset of records 0, 64, 128, ... of each segment */
};

void vsdlss_m3_eco_free(struct vsdlss_m3_eco *e)
{
    if(!e) return;
    free(e->pos); free(e->ck_ptr); free(e->ck); free(e);
}

static inline csi core_vertex(const vsdlss_m3_component_factor *cf, csi k)
{ return cf->core_map32?(csi)cf->core_map32[k]:cf->core_map[k]; }

static vsdlss_status eco_index(vsdlss_m3_component_factor *cf)
{
    if(cf->eco) return VSDLSS_OK;
    const vsdlss_reduction *r=cf->reduction;
    const csi n=r->n, cnt=r->count, core=r->core_n;
    struct vsdlss_m3_eco *e=(struct vsdlss_m3_eco *)calloc(1,sizeof(*e));
    if(!e) return VSDLSS_ERR_OOM;
    csi nck=0;
    e->ck_ptr=(csi *)malloc((size_t)(r->pk_count+1)*sizeof(csi));
    if(!e->ck_ptr) goto oom;
    for(csi q=0;q<r->pk_count;q++) { e->ck_ptr[q]=nck; nck+=(r->pk[q].count+((csi)1<<CK_SHIFT)-1)>>CK_SHIFT; }
    e->ck_ptr[r->pk_count]=nck;
    e->ck=(csi *)malloc((size_t)(nck?nck:1)*sizeof(csi));
    if(!e->ck) goto oom;
    int identity=cf->core_contig;
    for(csi q=0;q<r->pk_count;q++) {
        const vsdlss_pk_seg *g=r->pk+q; csi o=0, *ck=e->ck+e->ck_ptr[q];
        for(csi i=0;i<g->count;i++) {
            if(!(i&(((csi)1<<CK_SHIFT)-1))) ck[i>>CK_SHIFT]=o;
            o+=vsdlss_pk_degree(g,i);
            if(vsdlss_pk_vertex(g,i)!=g->k0+i) identity=0;
        }
    }
    if(!identity) {
        e->pos=(csi *)malloc((size_t)(n?n:1)*sizeof(csi));
        if(!e->pos) goto oom;
        for(csi v=0;v<n;v++) e->pos[v]=-1;
        for(csi q=0;q<r->pk_count;q++) {
            const vsdlss_pk_seg *g=r->pk+q;
            for(csi i=0;i<g->count;i++) e->pos[vsdlss_pk_vertex(g,i)]=g->k0+i;
        }
        for(csi k=0;k<core;k++) e->pos[core_vertex(cf,k)]=cnt+k;
        for(csi v=0;v<n;v++) if(e->pos[v]<0) { vsdlss_m3_eco_free(e); return VSDLSS_ERR_UNSUPPORTED; }
    }
    cf->eco=e;
    return VSDLSS_OK;
oom:
    vsdlss_m3_eco_free(e);
    return VSDLSS_ERR_OOM;
}

static inline csi pos_of(const struct vsdlss_m3_eco *e, csi v) { return e->pos?e->pos[v]:v; }

/* Record at elimination position k (< count): segment, index in it,
 * neighbour offset and degree. */
typedef struct { csi q, i, o, d; } rec_ref;
static rec_ref rec_at(const vsdlss_reduction *r, const struct vsdlss_m3_eco *e, csi k)
{
    csi lo=0, hi=r->pk_count-1;
    while(lo<hi) { csi mid=(lo+hi+1)/2; if(r->pk[mid].k0<=k) lo=mid; else hi=mid-1; }
    while(lo>0 && k>=r->pk[lo].k0+r->pk[lo].count) lo--;
    const vsdlss_pk_seg *g=r->pk+lo;
    rec_ref x; x.q=lo; x.i=k-g->k0;
    x.o=e->ck[e->ck_ptr[lo]+(x.i>>CK_SHIFT)];
    for(csi t=x.i&~(((csi)1<<CK_SHIFT)-1);t<x.i;t++) x.o+=vsdlss_pk_degree(g,t);
    x.d=vsdlss_pk_degree(g,x.i);
    return x;
}

/* Supernode owning core column k. */
static csi panel_of(const vsdlss_sn_factor *L, csi k)
{
    csi lo=0, hi=L->count-1;
    while(lo<hi) { csi mid=(lo+hi+1)/2; if((csi)L->column_start[mid]<=k) lo=mid; else hi=mid-1; }
    return lo;
}

/* 1 when the later of positions pa != pb is in the structure of the
 * earlier one's column of the factor. */
static int in_structure(const vsdlss_m3_component_factor *cf, csi pa, csi pb)
{
    const vsdlss_reduction *r=cf->reduction; const struct vsdlss_m3_eco *e=cf->eco;
    const csi lo=pa<pb?pa:pb, hi=pa<pb?pb:pa;
    if(lo<r->count) {
        const rec_ref x=rec_at(r,e,lo); const vsdlss_pk_seg *g=r->pk+x.q;
        for(csi j=0;j<x.d;j++) if(pos_of(e,g->nb[x.o+j])==hi) return 1;
        return 0;
    }
    const vsdlss_sn_factor *L=cf->numeric;
    if(!L) return 0;
    const csi k=lo-r->count, h=hi-r->count, s=panel_of(L,k);
    if(h<(csi)L->column_start[s+1]) return 1;
    csi a=L->row_ptr[s], b=L->row_ptr[s+1]-1;
    while(a<=b) {
        csi mid=a+(b-a)/2, v=L->row_index[mid];
        if(v==h) return 1;
        if(v<h) a=mid+1; else b=mid-1;
    }
    return 0;
}

/* ---- Undo log: the old values of every column a call changes. ---- */
typedef struct { int core; csi c, a, b, len, off; } undo_ent;   /* core: a = panel, b = column in it */
typedef struct { undo_ent *e; csi ne, ce; double *v; csi nv, cv; } undo_log;

static int undo_reserve(undo_log *u, csi vals)
{
    if(u->ne==u->ce) {
        csi c=u->ce?2*u->ce:64; undo_ent *p=(undo_ent *)realloc(u->e,(size_t)c*sizeof(*p));
        if(!p) return 0;
        u->e=p; u->ce=c;
    }
    if(u->nv+vals>u->cv) {
        csi c=u->cv?2*u->cv:1024; while(c<u->nv+vals) c*=2;
        double *p=(double *)realloc(u->v,(size_t)c*sizeof(double));
        if(!p) return 0;
        u->v=p; u->cv=c;
    }
    return 1;
}

/* Saves record (c, x): its pivot, then its multipliers. */
static int undo_record(undo_log *u, csi c, const vsdlss_reduction *r, rec_ref x)
{
    if(!undo_reserve(u,1+x.d)) return 0;
    const vsdlss_pk_seg *g=r->pk+x.q;
    undo_ent *t=u->e+u->ne++;
    t->core=0; t->c=c; t->a=x.q; t->b=x.i; t->len=x.d; t->off=u->nv;
    u->v[u->nv++]=g->piv[x.i];
    for(csi j=0;j<x.d;j++) u->v[u->nv++]=g->val[x.o+j];
    return 1;
}

/* Saves column j of panel s: its diagonal-block part (dn values from the
 * diagonal down), then its external rows (en values). */
static int undo_column(undo_log *u, csi c, csi s, csi j, const double *dc, csi dn, const double *ec, csi en)
{
    if(!undo_reserve(u,dn+en)) return 0;
    undo_ent *t=u->e+u->ne++;
    t->core=1; t->c=c; t->a=s; t->b=j; t->len=dn+en; t->off=u->nv;
    memcpy(u->v+u->nv,dc,(size_t)dn*sizeof(double)); u->nv+=dn;
    memcpy(u->v+u->nv,ec,(size_t)en*sizeof(double)); u->nv+=en;
    return 1;
}

static void undo_all(vsdlss_m3_factor *f, undo_log *u)
{
    for(csi k=u->ne;k>0;) {
        const undo_ent *t=u->e+(--k); const double *v=u->v+t->off;
        const vsdlss_m3_component_factor *cf=f->component+t->c;
        if(!t->core) {
            const vsdlss_reduction *r=cf->reduction; const rec_ref x=rec_at(r,cf->eco,r->pk[t->a].k0+t->b);
            vsdlss_pk_seg *g=r->pk+t->a;
            g->piv[t->b]=v[0];
            for(csi j=0;j<x.d;j++) g->val[x.o+j]=v[1+j];
        } else {
            const vsdlss_sn_factor *L=cf->numeric; const csi s=t->a, j=t->b;
            const csi W=(csi)L->column_start[s+1]-(csi)L->column_start[s], E=L->row_ptr[s+1]-L->row_ptr[s];
            double *P=L->panel+L->panel_offset[s];
            memcpy(P+VSDLSS_SN_DCOL(j,W),v,(size_t)(W-j)*sizeof(double));
            memcpy(P+W*(W+1)/2+j*E,v+(W-j),(size_t)E*sizeof(double));
        }
    }
}

/* ---- Workspace of one component.  Record phase: the term's vector as a
 * short sparse list.  Core phase: up to ECO_BATCH terms at a time (rank-k,
 * Davis & Hager 2001), so the columns their paths share -- the top of the
 * core tree, on every path -- are read and written once per batch rather
 * than once per term.  Per column the terms run in order, each on the
 * column as the previous one left it, which is exactly the arithmetic of
 * applying them one after another.  The batch's vectors live in rows
 * (B values each) allocated to the core entries they touch. ---- */
#define ECO_BATCH 64
typedef struct {
    csi ns, cs; csi *sp; double *sv;     /* record phase: positions and values */
    int nb; double sigma[ECO_BATCH];     /* pending core terms */
    csi ne, ce; csi *ek; double *ev; int *et;   /* their initial entries: core index, value, term */
    int32_t *slot;                       /* core index -> row, -1: none (core_n) */
    csi nrows, crows; csi *rows; double *wv;    /* rows: core index; wv: row r, term t at r*nb+t */
    csi nh, ch; csi *heap;               /* columns to visit */
    csi crs; csi *rs;                    /* rows of the current column */
} eco_ws;

static void ws_free(eco_ws *w)
{ free(w->sp); free(w->sv); free(w->ek); free(w->ev); free(w->et); free(w->slot);
  free(w->rows); free(w->wv); free(w->heap); free(w->rs); }

static int sp_add(eco_ws *w, csi p, double x)     /* w[p] += x */
{
    for(csi t=0;t<w->ns;t++) if(w->sp[t]==p) { w->sv[t]+=x; return 1; }
    if(w->ns==w->cs) {
        csi c=w->cs?2*w->cs:16;
        csi *a=(csi *)realloc(w->sp,(size_t)c*sizeof(csi)); if(!a) return 0; w->sp=a;
        double *b=(double *)realloc(w->sv,(size_t)c*sizeof(double)); if(!b) return 0; w->sv=b;
        w->cs=c;
    }
    w->sp[w->ns]=p; w->sv[w->ns]=x; w->ns++;
    return 1;
}

static int heap_push(eco_ws *w, csi k)
{
    if(w->nh==w->ch) {
        csi c=w->ch?2*w->ch:256; csi *a=(csi *)realloc(w->heap,(size_t)c*sizeof(csi));
        if(!a) return 0;
        w->heap=a; w->ch=c;
    }
    csi i=w->nh++;
    while(i>0) { csi p=(i-1)/2; if(w->heap[p]<=k) break; w->heap[i]=w->heap[p]; i=p; }
    w->heap[i]=k;
    return 1;
}

static csi heap_pop(eco_ws *w)
{
    csi top=w->heap[0], last=w->heap[--w->nh], i=0;
    for(;;) {
        csi c=2*i+1;
        if(c>=w->nh) break;
        if(c+1<w->nh && w->heap[c+1]<w->heap[c]) c++;
        if(w->heap[c]>=last) break;
        w->heap[i]=w->heap[c]; i=c;
    }
    if(w->nh) w->heap[i]=last;
    return top;
}

/* Row of core entry k in the batch (allocated, zero, on first use); -1: OOM. */
static csi row_of(eco_ws *w, csi k)
{
    if(w->slot[k]>=0) return w->slot[k];
    if(w->nrows==w->crows) {
        csi c=w->crows?2*w->crows:1024;
        csi *a=(csi *)realloc(w->rows,(size_t)c*sizeof(csi)); if(!a) return -1; w->rows=a;
        double *b=(double *)realloc(w->wv,(size_t)c*(size_t)w->nb*sizeof(double)); if(!b) return -1; w->wv=b;
        w->crows=c;
    }
    const csi r=w->nrows++;
    w->rows[r]=k; w->slot[k]=(int32_t)r;
    memset(w->wv+r*w->nb,0,(size_t)w->nb*sizeof(double));
    return r;
}

typedef struct { csi c, pa, pb; double alpha; } eco_term;   /* pb < 0: diagonal term */

/* Records of A + alpha w w', w = e_pa - e_pb (or e_pa), on component t->c:
 * method C1 along the path.  What reaches the core joins the pending batch
 * as sigma v v' (v = sqrt|alpha| z). */
static vsdlss_status record_phase(vsdlss_m3_factor *f, const eco_term *t, eco_ws *w, undo_log *u,
                                  vsdlss_m3_modify_stats *st)
{
    vsdlss_m3_component_factor *cf=f->component+t->c;
    const vsdlss_reduction *r=cf->reduction; const struct vsdlss_m3_eco *e=cf->eco;
    const csi cnt=r->count;
    double alpha=t->alpha;
    w->ns=0;
    if(!sp_add(w,t->pa,1.0) || (t->pb>=0 && !sp_add(w,t->pb,-1.0))) return VSDLSS_ERR_OOM;
    for(;;) {
        csi m=-1;
        for(csi a=0;a<w->ns;a++) if(w->sp[a]<cnt && (m<0 || w->sp[a]<w->sp[m])) m=a;
        if(m<0) break;
        const csi k=w->sp[m]; const double p=w->sv[m];
        w->sp[m]=w->sp[w->ns-1]; w->sv[m]=w->sv[w->ns-1]; w->ns--;
        if(p==0.0) continue;
        const rec_ref x=rec_at(r,e,k); vsdlss_pk_seg *g=r->pk+x.q;
        if(!undo_record(u,t->c,r,x)) return VSDLSS_ERR_OOM;
        const double d=g->piv[x.i], dbar=d+alpha*p*p;
        if(!isfinite(dbar)) return VSDLSS_ERR_NONFINITE;
        if(!(dbar>ECO_TOL*d)) return VSDLSS_ERR_NOT_POSDEF;
        const double beta=p*alpha/dbar;
        alpha*=d/dbar;
        g->piv[x.i]=dbar;
        int ok=1;
        for(csi j=0;j<x.d;j++) {
            const csi q=pos_of(e,g->nb[x.o+j]);
            double wq=-p*g->val[x.o+j];
            csi a=0; while(a<w->ns && w->sp[a]!=q) a++;
            if(a<w->ns) { wq+=w->sv[a]; w->sv[a]=wq; }
            else if(!sp_add(w,q,wq)) return VSDLSS_ERR_OOM;
            g->val[x.o+j]+=beta*wq;
            ok&=isfinite(g->val[x.o+j])!=0;
        }
        if(!ok || !isfinite(alpha)) return VSDLSS_ERR_NONFINITE;
        st->record_columns++;
    }
    const double scale=sqrt(fabs(alpha));
    int any=0;
    for(csi a=0;a<w->ns;a++) {
        if(w->sv[a]==0.0) continue;
        if(!cf->numeric || w->sp[a]<cnt) return VSDLSS_ERR_INVALID;   /* cannot happen */
        if(w->ne==w->ce) {
            csi c=w->ce?2*w->ce:64;
            csi *p1=(csi *)realloc(w->ek,(size_t)c*sizeof(csi)); if(!p1) return VSDLSS_ERR_OOM; w->ek=p1;
            double *p2=(double *)realloc(w->ev,(size_t)c*sizeof(double)); if(!p2) return VSDLSS_ERR_OOM; w->ev=p2;
            int *p3=(int *)realloc(w->et,(size_t)c*sizeof(int)); if(!p3) return VSDLSS_ERR_OOM; w->et=p3;
            w->ce=c;
        }
        w->ek[w->ne]=w->sp[a]-cnt; w->ev[w->ne]=scale*w->sv[a]; w->et[w->ne++]=w->nb;
        any=1;
    }
    if(any) w->sigma[w->nb++]=alpha>0?1.0:-1.0;
    return VSDLSS_OK;
}

/* The pending batch of component c on the core L L': per column on the
 * union of the terms' paths (ascending), each term's hyperbolic rotation in
 * order (cs_updown's form).  Clears the batch. */
static vsdlss_status core_flush(vsdlss_m3_factor *f, csi c, eco_ws *w, undo_log *u,
                                vsdlss_m3_modify_stats *st)
{
    if(!w->nb) return VSDLSS_OK;
    const vsdlss_m3_component_factor *cf=f->component+c;
    const vsdlss_sn_factor *L=cf->numeric;
    const csi B=w->nb, core=cf->reduction->core_n;
    vsdlss_status status=VSDLSS_OK;
    double beta[ECO_BATCH];
    if(!w->slot) {
        w->slot=(int32_t *)malloc((size_t)core*sizeof(int32_t));
        if(!w->slot) { status=VSDLSS_ERR_OOM; goto out; }
        for(csi k=0;k<core;k++) w->slot[k]=-1;
    }
    w->nrows=0; w->nh=0;
    if(w->crows) {                      /* row stride is this batch's size */
        double *b=(double *)realloc(w->wv,(size_t)w->crows*(size_t)B*sizeof(double));
        if(!b) { status=VSDLSS_ERR_OOM; goto out; }
        w->wv=b;
    }
    for(csi q=0;q<w->ne;q++) {
        const csi r=row_of(w,w->ek[q]);
        if(r<0 || !heap_push(w,w->ek[q])) { status=VSDLSS_ERR_OOM; goto out; }
        w->wv[r*B+w->et[q]]+=w->ev[q];
    }
    for(csi t=0;t<B;t++) beta[t]=1.0;
    csi done=-1;
    while(w->nh) {
        const csi k0=heap_pop(w);
        if(k0<=done) continue;
        const csi s=panel_of(L,k0), c0=L->column_start[s], c1=L->column_start[s+1];
        const csi W=c1-c0, E=L->row_ptr[s+1]-L->row_ptr[s];
        const vsdlss_sni *ri=L->row_index+L->row_ptr[s];
        double *P=L->panel+L->panel_offset[s];
        for(csi k=k0;k<c1;k++) {
            done=k;
            const csi sk=w->slot[k];
            if(sk<0) continue;
            int any=0;
            for(csi t=0;t<B;t++) any|=w->wv[sk*B+t]!=0.0;
            if(!any) continue;
            const csi j=k-c0, m=W-j-1;
            double *dc=P+VSDLSS_SN_DCOL(j,W), *ec=P+W*(W+1)/2+j*E;
            if(!undo_column(u,c,s,j,dc,W-j,ec,E)) { status=VSDLSS_ERR_OOM; goto out; }
            if(m+E>w->crs) {
                csi cap=2*(m+E); csi *a=(csi *)realloc(w->rs,(size_t)cap*sizeof(csi));
                if(!a) { status=VSDLSS_ERR_OOM; goto out; }
                w->rs=a; w->crs=cap;
            }
            for(csi q=0;q<m;q++) if((w->rs[q]=row_of(w,k+1+q))<0) { status=VSDLSS_ERR_OOM; goto out; }
            for(csi q=0;q<E;q++) if((w->rs[m+q]=row_of(w,ri[q]))<0) { status=VSDLSS_ERR_OOM; goto out; }
            double *wv=w->wv; const csi *rs=w->rs;
            /* The diagonal, term after term, gives each active term's
             * rotation; then every entry below takes the rotations in the
             * same order, held in a register (the per-entry arithmetic of
             * applying the terms one after another). */
            int act[ECO_BATCH], na=0;
            double ta[ECO_BATCH], td[ECO_BATCH], tg[ECO_BATCH];
            int ok=1;
            for(csi t=0;t<B;t++) {
                const double vk=wv[sk*B+t];
                if(vk==0.0) continue;
                const double sigma=w->sigma[t], a=vk/dc[0];
                double b2=beta[t]*beta[t]+sigma*a*a;
                if(!isfinite(b2)) { status=VSDLSS_ERR_NONFINITE; goto out; }
                if(!(b2>ECO_TOL*beta[t]*beta[t])) { status=VSDLSS_ERR_NOT_POSDEF; goto out; }
                b2=sqrt(b2);
                const double delta=sigma>0?beta[t]/b2:b2/beta[t], gamma=sigma*a/(b2*beta[t]);
                dc[0]=delta*dc[0]+(sigma>0?gamma*vk:0.0);
                beta[t]=b2;
                wv[sk*B+t]=0.0;
                ok&=isfinite(dc[0]) && dc[0]>0.0;
                act[na]=(int)t; ta[na]=a; td[na]=delta; tg[na]=gamma; na++;
            }
            /* Each entry is a chain of dependent steps (one per term): four
             * entries at a time keep the floating-point units busy. */
            int up[ECO_BATCH];
            for(int h=0;h<na;h++) up[h]=w->sigma[act[h]]>0;
            const csi mE=m+E;
            csi q=0;
            for(;q+4<=mE;q+=4) {
                double *p0=q<m?dc+1+q:ec+(q-m), *p1=q+1<m?dc+2+q:ec+(q+1-m),
                       *p2=q+2<m?dc+3+q:ec+(q+2-m), *p3=q+3<m?dc+4+q:ec+(q+3-m);
                double l0=*p0, l1=*p1, l2=*p2, l3=*p3;
                double *y0=wv+rs[q]*B, *y1=wv+rs[q+1]*B, *y2=wv+rs[q+2]*B, *y3=wv+rs[q+3]*B;
                for(int h=0;h<na;h++) {
                    const int t=act[h]; const double a=ta[h], dl=td[h], gm=tg[h];
                    const double u0=y0[t], u1=y1[t], u2=y2[t], u3=y3[t];
                    const double v0=u0-a*l0, v1=u1-a*l1, v2=u2-a*l2, v3=u3-a*l3;
                    y0[t]=v0; y1[t]=v1; y2[t]=v2; y3[t]=v3;
                    if(up[h]) { l0=dl*l0+gm*u0; l1=dl*l1+gm*u1; l2=dl*l2+gm*u2; l3=dl*l3+gm*u3; }
                    else      { l0=dl*l0+gm*v0; l1=dl*l1+gm*v1; l2=dl*l2+gm*v2; l3=dl*l3+gm*v3; }
                }
                *p0=l0; *p1=l1; *p2=l2; *p3=l3;
                ok&=isfinite(l0+l1+l2+l3)!=0;
            }
            for(;q<mE;q++) {
                double *lq=q<m?dc+1+q:ec+(q-m), l=*lq, *y=wv+rs[q]*B;
                for(int h=0;h<na;h++) {
                    const int t=act[h];
                    const double w1=y[t], w2=w1-ta[h]*l;
                    y[t]=w2;
                    l=td[h]*l+tg[h]*(up[h]?w1:w2);
                }
                *lq=l;
                ok&=isfinite(l)!=0;
            }
            if(!ok) { status=VSDLSS_ERR_NONFINITE; goto out; }
            st->core_columns++;
        }
        /* The paths go on at the parent's first column R_s[0], even through
         * columns whose entries happen to be zero: entries further up may
         * not be (as cs_updown, which walks the whole path). */
        if(E>0 && !heap_push(w,ri[0])) { status=VSDLSS_ERR_OOM; goto out; }
    }
out:
    if(w->slot) for(csi r=0;r<w->nrows;r++) w->slot[w->rows[r]]=-1;
    w->nrows=0; w->nh=0; w->nb=0; w->ne=0;
    return status;
}

/* The forward plan of a level-scheduled tail copies multipliers: record i of
 * the tail segment changed, so copy its new ones. */
static void plan_patch(vsdlss_reduction *r, csi i)
{
    struct vsdlss_tail_plan *p=r->tail_fwd;
    const vsdlss_pk_seg *g=r->pk+r->blocks;
    const csi *cr=r->tail_chunk_rec, *lv=r->tail_level, levels=r->tail_levels;
    csi lo=0, hi=lv[levels]-1;
    while(lo<hi) { csi mid=(lo+hi+1)/2; if(cr[mid]<=i) lo=mid; else hi=mid-1; }
    const csi c=lo;
    lo=0; hi=levels-1;
    while(lo<hi) { csi mid=(lo+hi+1)/2; if(lv[mid]<=c) lo=mid; else hi=mid-1; }
    const csi l=lo, d=vsdlss_pk_degree(g,i);
    /* neighbour offset of record i: its chunk's plus the degrees before it */
    csi o=r->tail_chunk_nb[c];
    for(csi t=cr[c];t<i;t++) o+=vsdlss_pk_degree(g,t);
    for(csi u=p->lp[l];u<p->lp[l+1];u++)
        if(p->src[u]==(uint32_t)i)
            for(csi j=0;j<d;j++) if(p->tgt[u]==g->nb[o+j]) { p->m[u]=g->val[o+j]; break; }
}

typedef struct { csi a, b; double x; } trip;
static int cmp_trip(const void *x, const void *y)
{
    const trip *s=(const trip *)x, *t=(const trip *)y;
    if(s->a!=t->a) return s->a<t->a?-1:1;
    if(s->b!=t->b) return s->b<t->b?-1:1;
    return 0;
}

static inline void locate(const vsdlss_m3_factor *f, csi g, csi *c, csi *v)
{
    const int sh=f->inv_shift;
    const uint64_t code=f->inv32?(uint64_t)f->inv32[g]:f->inv64[g];
    *c=(csi)(code>>sh); *v=(csi)(code&(((uint64_t)1<<sh)-1));
}

vsdlss_status vsdlss_m3_modify(vsdlss_m3_factor *f, csi nz, const csi *row, const csi *col,
                               const double *delta, vsdlss_m3_modify_stats *stats)
{
    vsdlss_m3_modify_stats st; memset(&st,0,sizeof(st));
    if(stats) *stats=st;
    if(!f || nz<0 || (nz && (!row || !col || !delta))) return VSDLSS_ERR_INVALID;
    for(csi t=0;t<nz;t++)
        if(row[t]<0 || row[t]>=f->n || col[t]<0 || col[t]>=f->n || !isfinite(delta[t])) return VSDLSS_ERR_INVALID;
    if(!nz) return VSDLSS_OK;
    if(f->disk_mode || (!f->inv32 && !f->inv64)) return VSDLSS_ERR_UNSUPPORTED;

    vsdlss_status status=VSDLSS_OK;
    trip *ed=(trip *)malloc((size_t)nz*sizeof(trip)), *dg=(trip *)malloc((size_t)(3*nz)*sizeof(trip));
    eco_term *tm=(eco_term *)malloc((size_t)(3*nz)*sizeof(eco_term));
    eco_ws *ws=(eco_ws *)calloc((size_t)f->count,sizeof(eco_ws));
    undo_log u; memset(&u,0,sizeof(u));
    csi ne=0, nd=0, nt=0;
    if(!ed || !dg || !tm || !ws) { status=VSDLSS_ERR_OOM; goto done; }
    /* Pairs merged; each pair's coupling g = -dA(a,b) moves -g ... +g onto
     * the diagonal, leaving r_i = dA(i,i) + sum_b dA(i,b). */
    for(csi t=0;t<nz;t++) {
        if(row[t]==col[t]) { dg[nd].a=row[t]; dg[nd].b=0; dg[nd++].x=delta[t]; continue; }
        ed[ne].a=row[t]<col[t]?row[t]:col[t]; ed[ne].b=row[t]<col[t]?col[t]:row[t]; ed[ne++].x=delta[t];
    }
    qsort(ed,(size_t)ne,sizeof(trip),cmp_trip);
    csi me=0;
    for(csi t=0;t<ne;t++) {
        if(me && ed[me-1].a==ed[t].a && ed[me-1].b==ed[t].b) ed[me-1].x+=ed[t].x; else ed[me++]=ed[t];
    }
    ne=me;
    for(csi t=0;t<ne;t++) {
        dg[nd].a=ed[t].a; dg[nd].b=0; dg[nd++].x=ed[t].x;
        dg[nd].a=ed[t].b; dg[nd].b=0; dg[nd++].x=ed[t].x;
    }
    qsort(dg,(size_t)nd,sizeof(trip),cmp_trip);
    csi md=0;
    for(csi t=0;t<nd;t++) { if(md && dg[md-1].a==dg[t].a) dg[md-1].x+=dg[t].x; else dg[md++]=dg[t]; }
    nd=md;

    /* Terms in component-local positions, checked before anything changes. */
    for(int pass=0;pass<2;pass++) {
        const csi m=pass?nd:ne;
        for(csi t=0;t<m;t++) {
            const trip *x=pass?dg+t:ed+t;
            const double alpha=pass?x->x:-x->x;
            if(!isfinite(alpha)) { status=VSDLSS_ERR_NONFINITE; goto done; }   /* merged sums overflowed */
            if(alpha==0.0) continue;
            csi ca, va, cb=-1, vb=-1;
            locate(f,x->a,&ca,&va);
            if(!pass) { locate(f,x->b,&cb,&vb); if(cb!=ca) { status=VSDLSS_ERR_UNSUPPORTED; goto done; } }
            vsdlss_m3_component_factor *cf=f->component+ca;
            if(cf->disk || !cf->reduction || cf->reduction->records || (cf->reduction->core_n && !cf->numeric)) {
                status=VSDLSS_ERR_UNSUPPORTED; goto done;
            }
            status=eco_index(cf);
            if(status!=VSDLSS_OK) goto done;
            eco_term *y=tm+nt++;
            y->c=ca; y->alpha=alpha; y->pa=pos_of(cf->eco,va); y->pb=pass?-1:pos_of(cf->eco,vb);
            if(!pass && !in_structure(cf,y->pa,y->pb)) { status=VSDLSS_ERR_UNSUPPORTED; goto done; }
        }
    }
    /* Updates, then downdates (input order within each).  A term's record
     * phase changes only records and its core phase only the core, so the
     * core phases can wait for a batch: the result is that of applying the
     * terms one by one. */
    for(int sign=1;sign>=-1;sign-=2)
        for(csi t=0;t<nt;t++) {
            if((tm[t].alpha>0)!=(sign>0)) continue;
            eco_ws *w=ws+tm[t].c;
            status=record_phase(f,tm+t,w,&u,&st);
            if(status==VSDLSS_OK && w->nb==ECO_BATCH) status=core_flush(f,tm[t].c,w,&u,&st);
            if(status!=VSDLSS_OK) { undo_all(f,&u); goto done; }
            if(sign>0) st.updates++; else st.downdates++;
        }
    for(csi c=0;c<f->count;c++) {
        status=core_flush(f,c,ws+c,&u,&st);
        if(status!=VSDLSS_OK) { undo_all(f,&u); goto done; }
    }
    /* Committed: refresh the copies in level-schedule forward plans. */
    for(csi k=0;k<u.ne;k++) {
        const undo_ent *x=u.e+k;
        vsdlss_reduction *r=f->component[x->c].reduction;
        if(!x->core && r->tail_fwd && x->a==r->blocks) plan_patch(r,x->b);
    }
    if(stats) *stats=st;
done:
    if(ws) for(csi c=0;c<f->count;c++) ws_free(ws+c);
    free(ws); free(tm); free(ed); free(dg); free(u.e); free(u.v);
    return status;
}
