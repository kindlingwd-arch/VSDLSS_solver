/* vsdlss_m3_modify (ECO low-rank modification).
 *
 * Grounded resistor networks (2-D grids with wires of degree-2 nodes and
 * dangling nodes, a second grid, a tiny component), caller numbering
 * shuffled.  After each modification the factor must solve A + dA: backward
 * error against A + dA, and agreement with a new factorization of A + dA.
 * Covered: conductance increases and decreases (updates and downdates),
 * ground changes, either triangle and split duplicates, several rounds on
 * one factor, record and core columns, relabelled and unrelabelled
 * components, window and level tail schedules (the level schedule's forward
 * plan is patched), 1 and 4 threads.  All or nothing: a failing call (loss
 * of definiteness after valid terms, coupling across components, invalid
 * input, every allocation failure) leaves the solutions bitwise unchanged. */
#include "../src/vsdlss_m3_internal.h"
#include "m3_test_alloc.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

extern int vsdlss_m3_relabel;
extern csi vsdlss_reorder_min;

static int fails;
#define CHECK(c,msg,...) do{ if(!(c)){ fails++; fprintf(stderr,"FAIL: " msg "\n",__VA_ARGS__); } }while(0)

static unsigned long long rng=88172645463325252ULL;
static double urand(void) { rng^=rng<<13; rng^=rng>>7; rng^=rng<<17; return (double)(rng>>11)/9007199254740992.0; }
static csi irand(csi n) { return (csi)(urand()*(double)n)%n; }

typedef struct { csi n, ne, ce; csi *u, *v; double *g, *g0; csi *comp; } net;

static void add_edge(net *w, csi a, csi b, double g)
{
    if(w->ne==w->ce) { w->ce=w->ce?2*w->ce:1024; w->u=realloc(w->u,w->ce*sizeof(csi)); w->v=realloc(w->v,w->ce*sizeof(csi)); w->g=realloc(w->g,w->ce*8); }
    w->u[w->ne]=a; w->v[w->ne]=b; w->g[w->ne++]=g;
}
static csi add_node(net *w, csi comp)
{
    csi i=w->n++;
    w->g0=realloc(w->g0,w->n*8); w->comp=realloc(w->comp,w->n*sizeof(csi));
    w->g0[i]=1e-3+1e-2*urand(); w->comp[i]=comp;
    return i;
}
/* R x C grid (core) with wires: chains of 1..6 degree-2 nodes between two
 * grid nodes, and dangling chains (degree 1 ends). */
static void grid(net *w, csi R, csi C, csi wires, csi comp)
{
    csi base=w->n;
    for(csi k=0;k<R*C;k++) add_node(w,comp);
    for(csi r=0;r<R;r++) for(csi c=0;c<C;c++) {
        csi k=base+r*C+c;
        if(c+1<C) add_edge(w,k,k+1,0.5+1.5*urand());
        if(r+1<R) add_edge(w,k,k+C,0.5+1.5*urand());
    }
    for(csi t=0;t<wires;t++) {
        csi a=base+irand(R*C), len=1+irand(6), prev=a;
        for(csi s=0;s<len;s++) { csi m=add_node(w,comp); add_edge(w,prev,m,0.2+3*urand()); prev=m; }
        if(t%4) { csi b=base+irand(R*C); if(b!=a) add_edge(w,prev,b,0.2+3*urand()); }
    }
}

static net build_net(void)
{
    net w; memset(&w,0,sizeof(w));
    grid(&w,30,34,700,0);
    grid(&w,18,15,200,1);
    csi a=add_node(&w,2), b=add_node(&w,2), c=add_node(&w,2), d=add_node(&w,2);   /* tiny component */
    add_edge(&w,a,b,1.0); add_edge(&w,b,c,2.0); add_edge(&w,c,a,0.5); add_edge(&w,c,d,1.5);
    return w;
}

static net copy_net(const net *s)
{
    net w=*s;
    w.u=malloc(s->ce*sizeof(csi)); w.v=malloc(s->ce*sizeof(csi)); w.g=malloc(s->ce*8);
    w.g0=malloc(s->n*8); w.comp=malloc(s->n*sizeof(csi));
    memcpy(w.u,s->u,s->ne*sizeof(csi)); memcpy(w.v,s->v,s->ne*sizeof(csi)); memcpy(w.g,s->g,s->ne*8);
    memcpy(w.g0,s->g0,s->n*8); memcpy(w.comp,s->comp,s->n*sizeof(csi));
    return w;
}
static void free_net(net *w) { free(w->u); free(w->v); free(w->g); free(w->g0); free(w->comp); }

/* Upper CSC of the network in caller numbering id[node]. */
static vsdlss *to_csc(const net *w, const csi *id)
{
    const csi n=w->n; csi *cnt=calloc((size_t)n+1,sizeof(csi));
    double *dg=malloc((size_t)n*8);
    for(csi i=0;i<n;i++) dg[id[i]]=w->g0[i];
    for(csi e=0;e<w->ne;e++) { csi a=id[w->u[e]], b=id[w->v[e]]; dg[a]+=w->g[e]; dg[b]+=w->g[e]; cnt[a>b?a:b]++; }
    vsdlss *A=vsdlss_spalloc(n,n,w->ne+n,1,0);
    A->p[0]=0; for(csi j=0;j<n;j++) A->p[j+1]=A->p[j]+cnt[j]+1;
    csi *fill=calloc((size_t)n,sizeof(csi));
    for(csi e=0;e<w->ne;e++) {
        csi a=id[w->u[e]], b=id[w->v[e]], lo=a<b?a:b, hi=a<b?b:a, k=A->p[hi]+fill[hi]++;
        A->i[k]=lo; A->x[k]=-w->g[e];
    }
    for(csi j=0;j<n;j++) {
        csi k=A->p[j]+fill[j]; A->i[k]=j; A->x[k]=dg[j];
        for(csi s=A->p[j]+1;s<=k;s++) { csi ri=A->i[s]; double rx=A->x[s]; csi t=s;    /* sort rows */
            while(t>A->p[j] && A->i[t-1]>ri) { A->i[t]=A->i[t-1]; A->x[t]=A->x[t-1]; t--; }
            A->i[t]=ri; A->x[t]=rx; }
    }
    free(cnt); free(fill); free(dg);
    return A;
}

/* A random ECO on w (applied to it) as dA triplets in caller numbering:
 * nedge conductance changes in [-0.8 g, +1.5 g], nground ground changes in
 * [-0.5 g0, +3 g0]; either triangle, some split in two halves. */
typedef struct { csi nz, cap; csi *r, *c; double *x; } trips;
static void tpush(trips *t, csi r, csi c, double x)
{
    if(t->nz==t->cap) { t->cap=t->cap?2*t->cap:64; t->r=realloc(t->r,t->cap*sizeof(csi)); t->c=realloc(t->c,t->cap*sizeof(csi)); t->x=realloc(t->x,t->cap*8); }
    t->r[t->nz]=r; t->c[t->nz]=c; t->x[t->nz++]=x;
}
static trips random_eco(net *w, const csi *id, csi nedge, csi nground)
{
    trips t; memset(&t,0,sizeof(t));
    for(csi k=0;k<nedge;k++) {
        csi e=irand(w->ne); double dgv=w->g[e]*(-0.8+2.3*urand());
        csi a=id[w->u[e]], b=id[w->v[e]];
        w->g[e]+=dgv;
        if(k%3==0) { tpush(&t,b,a,-0.5*dgv); tpush(&t,a,b,-0.5*dgv); }
        else if(k%3==1) tpush(&t,b,a,-dgv);
        else tpush(&t,a,b,-dgv);
        tpush(&t,a,a,dgv); tpush(&t,b,b,dgv);
    }
    for(csi k=0;k<nground;k++) {
        csi i=irand(w->n); double d=w->g0[i]*(-0.5+3.5*urand());
        w->g0[i]+=d; tpush(&t,id[i],id[i],d);
    }
    return t;
}
static void free_trips(trips *t) { free(t->r); free(t->c); free(t->x); }

static void solve(const vsdlss_m3_factor *f, const double *b, double *x, csi n, const char *name)
{
    vsdlss_status s=vsdlss_m3_solve(f,b,x);
    CHECK(s==VSDLSS_OK,"%s solve %s",name,vsdlss_status_string(s));
    if(s!=VSDLSS_OK) memset(x,0,n*8);
}

/* f solves w: backward error against w's matrix and agreement with a new
 * factorization of it. */
static void check_solves(const vsdlss_m3_factor *f, const net *w, const csi *id, const char *name)
{
    const csi n=w->n; vsdlss *A=to_csc(w,id); vsdlss_m3_factor *g=NULL;
    double *b=malloc(n*8), *x=malloc(n*8), *y=malloc(n*8), eta=1;
    for(csi i=0;i<n;i++) b[i]=sin(0.7*(double)i)+0.3;
    solve(f,b,x,n,name);
    CHECK(vsdlss_backward_error(A,x,b,&eta)==VSDLSS_OK && eta<1e-13,"%s backward error %.3g",name,eta);
    CHECK(vsdlss_factorize_m3(A,5,&g)==VSDLSS_OK,"%s refactor",name);
    solve(g,b,y,n,name);
    double dx=0, ny=0;
    for(csi i=0;i<n;i++) { if(fabs(x[i]-y[i])>dx) dx=fabs(x[i]-y[i]); if(fabs(y[i])>ny) ny=fabs(y[i]); }
    CHECK(dx<=1e-9*ny,"%s differs from a new factorization: %.3g relative",name,dx/ny);
    vsdlss_m3_factor_free(g); vsdlss_spfree(A); free(b); free(x); free(y);
}

static int plans_seen, records_seen, cores_seen;
static csi plan_copies;

/* The level schedule's forward plan copies tail multipliers (and replays from
 * them only at sizes beyond this test): every copy must equal its record's
 * current multiplier.  Returns the number of copies checked. */
static csi plan_consistent(const vsdlss_m3_factor *f, const char *name)
{
    csi checked=0;
    for(csi c=0;c<f->count;c++) {
        const vsdlss_reduction *r=f->component[c].reduction; const struct vsdlss_tail_plan *p=r->tail_fwd;
        if(!p) continue;
        const vsdlss_pk_seg *g=r->pk+r->blocks;
        csi *off=malloc((size_t)(g->count+1)*sizeof(csi));
        off[0]=0; for(csi i=0;i<g->count;i++) off[i+1]=off[i]+vsdlss_pk_degree(g,i);
        csi bad=0;
        for(csi u=0;u<p->lp[p->levels];u++) {
            const csi i=p->src[u]; int found=0;
            for(csi o=off[i];o<off[i+1];o++) if(g->nb[o]==p->tgt[u]) { found=1; bad+=g->val[o]!=p->m[u]; }
            bad+=!found; checked++;
        }
        CHECK(!bad,"%s component %lld: %lld stale forward-plan multipliers",name,(long long)c,(long long)bad);
        free(off);
    }
    return checked;
}

static void scenario(const net *w0, const csi *id, int relabel, csi windows, int threads)
{
    char name[96]; snprintf(name,sizeof(name),"relabel=%d windows=%lld threads=%d",relabel,(long long)windows,threads);
    vsdlss_m3_relabel=relabel; vsdlss_reduce_tail_windows=windows; vsdlss_set_num_threads(threads);
    net w=copy_net(w0); vsdlss *A=to_csc(&w,id); vsdlss_m3_factor *f=NULL;
    CHECK(vsdlss_factorize_m3(A,5,&f)==VSDLSS_OK,"%s factor",name);
    if(!f) { vsdlss_spfree(A); free_net(&w); return; }
    for(csi c=0;c<f->count;c++) plans_seen+=f->component[c].reduction->tail_fwd!=NULL;
    for(int round=0;round<4;round++) {
        /* one coupling; two ordinary ECOs; one larger than a batch (several core batches) */
        trips t=random_eco(&w,id,round==0?1:round==3?90:12,round==0?0:round==3?20:5);
        vsdlss_m3_modify_stats st;
        vsdlss_status s=vsdlss_m3_modify(f,t.nz,t.r,t.c,t.x,&st);
        CHECK(s==VSDLSS_OK,"%s round %d modify %s",name,round,vsdlss_status_string(s));
        records_seen+=st.record_columns>0; cores_seen+=st.core_columns>0;
        char nm[128]; snprintf(nm,sizeof(nm),"%s round %d",name,round);
        check_solves(f,&w,id,nm);
        plan_copies+=plan_consistent(f,nm);
        free_trips(&t);
    }
    vsdlss_m3_factor_free(f); vsdlss_spfree(A); free_net(&w);
}

/* Failing calls leave the factor bitwise unchanged. */
static void unchanged(const vsdlss_m3_factor *f, const double *b, const double *x0, csi n, const char *name)
{
    double *x=malloc(n*8);
    solve(f,b,x,n,name);
    CHECK(!memcmp(x,x0,n*8),"%s: factor changed by a failing call",name);
    free(x);
}

static void failures(const net *w0, const csi *id)
{
    vsdlss_m3_relabel=1; vsdlss_reduce_tail_windows=64; vsdlss_set_num_threads(1);
    const csi n=w0->n; vsdlss *A=to_csc(w0,id); vsdlss_m3_factor *f=NULL;
    CHECK(vsdlss_factorize_m3(A,5,&f)==VSDLSS_OK,"%s","failures: factor");
    double *b=malloc(n*8), *x0=malloc(n*8);
    for(csi i=0;i<n;i++) b[i]=cos(0.3*(double)i);
    solve(f,b,x0,n,"failures");
    /* valid updates, then a downdate past definiteness */
    net w=copy_net(w0); trips t=random_eco(&w,id,10,4);
    csi v=id[irand(n)];
    double dvv=0; for(csi k=A->p[v];k<A->p[v+1];k++) if(A->i[k]==v) dvv=A->x[k];
    tpush(&t,v,v,-10*dvv);
    CHECK(vsdlss_m3_modify(f,t.nz,t.r,t.c,t.x,NULL)==VSDLSS_ERR_NOT_POSDEF,"%s","indefinite result not reported");
    unchanged(f,b,x0,n,"indefinite");
    /* a coupling across components */
    csi a=-1, c=-1;
    for(csi i=0;i<n && (a<0||c<0);i++) { if(w0->comp[i]==0 && a<0) a=i; if(w0->comp[i]==1 && c<0) c=i; }
    csi rr[3]={id[a],id[a],id[c]}, cc[3]={id[c],id[a],id[c]}; double xx[3]={-0.1,0.1,0.1};
    CHECK(vsdlss_m3_modify(f,3,rr,cc,xx,NULL)==VSDLSS_ERR_UNSUPPORTED,"%s","coupling across components accepted");
    unchanged(f,b,x0,n,"across components");
    /* invalid input */
    csi bad=n; double nan_=NAN;
    CHECK(vsdlss_m3_modify(f,1,&bad,&bad,xx,NULL)==VSDLSS_ERR_INVALID,"%s","index out of range");
    CHECK(vsdlss_m3_modify(f,1,rr,cc,&nan_,NULL)==VSDLSS_ERR_INVALID,"%s","NaN delta");
    CHECK(vsdlss_m3_modify(f,-1,rr,cc,xx,NULL)==VSDLSS_ERR_INVALID,"%s","negative count");
    CHECK(vsdlss_m3_modify(NULL,1,rr,cc,xx,NULL)==VSDLSS_ERR_INVALID,"%s","NULL factor");
    CHECK(vsdlss_m3_modify(f,0,NULL,NULL,NULL,NULL)==VSDLSS_OK,"%s","empty modification");
    unchanged(f,b,x0,n,"invalid input");
    /* a pair of distant grid nodes: rejected as a structural change, or (if
     * fill couples them) applied correctly */
    csi far[2]={0,29*34+33}; csi fr[3]={id[far[0]],id[far[0]],id[far[1]]}, fc[3]={id[far[1]],id[far[0]],id[far[1]]};
    double fx[3]={-0.25,0.25,0.25};
    vsdlss_status s=vsdlss_m3_modify(f,3,fr,fc,fx,NULL);
    CHECK(s==VSDLSS_ERR_UNSUPPORTED || s==VSDLSS_OK,"distant pair: %s",vsdlss_status_string(s));
    if(s==VSDLSS_ERR_UNSUPPORTED) unchanged(f,b,x0,n,"distant pair");
    vsdlss_m3_factor_free(f);
    free_trips(&t); free_net(&w);

    /* Every allocation of a modification fails once: OOM with the factor
     * unchanged, then the same call succeeds; nothing leaks. */
    size_t live0=m3_alloc_live(), steps=0;
    for(size_t k=1;k<10000;k++) {
        vsdlss_m3_factor *g=NULL;
        CHECK(vsdlss_factorize_m3(A,5,&g)==VSDLSS_OK,"oom %zu: factor",k);
        net w2=copy_net(w0); rng=12345; trips t2=random_eco(&w2,id,8,3);
        m3_alloc_fail_at(k);
        vsdlss_status st=vsdlss_m3_modify(g,t2.nz,t2.r,t2.c,t2.x,NULL);
        m3_alloc_reset();
        int done=st==VSDLSS_OK;
        if(!done) {
            CHECK(st==VSDLSS_ERR_OOM,"oom %zu: %s",k,vsdlss_status_string(st));
            unchanged(g,b,x0,n,"allocation failure");
            CHECK(vsdlss_m3_modify(g,t2.nz,t2.r,t2.c,t2.x,NULL)==VSDLSS_OK,"oom %zu: retry",k);
        }
        if(k==1 || done) check_solves(g,&w2,id,done?"oom sweep (no failure)":"oom retry");
        vsdlss_m3_factor_free(g); free_trips(&t2); free_net(&w2);
        steps=k;
        if(done) break;
    }
    CHECK(m3_alloc_live()==live0,"allocations leaked: %zu",m3_alloc_live()-live0);
    printf("allocation failure sweep: %zu points\n",steps-1);
    vsdlss_spfree(A); free(b); free(x0);
}

int main(void)
{
    /* small thresholds: blocked reduction with a scheduled tail; the tiny
     * component keeps its numbering (not relabelled) */
    vsdlss_reduce_block=256; vsdlss_reduce_tail_min=16; vsdlss_reduce_tail_grain=4;
    vsdlss_reduce_tail_order_grain=4; vsdlss_reorder_min=64;
    net w=build_net();
    csi *id=malloc(w.n*sizeof(csi));
    for(csi i=0;i<w.n;i++) id[i]=i;
    for(csi i=w.n-1;i>0;i--) { csi j=irand(i+1), s=id[i]; id[i]=id[j]; id[j]=s; }
    for(int relabel=1;relabel>=0;relabel--)
        for(int win=0;win<2;win++)
            for(int th=1;th<=4;th*=4) scenario(&w,id,relabel,win?64:0,th);
    CHECK(plans_seen>0 && plan_copies>0,"no level-schedule forward plan exercised (%d plans)",plans_seen);
    CHECK(records_seen>0 && cores_seen>0,"record columns %d, core columns %d",records_seen,cores_seen);
    failures(&w,id);
    free(id); free_net(&w);
    if(fails) { fprintf(stderr,"%d failure(s)\n",fails); return 1; }
    printf("test_modify: ALL OK (n=%lld)\n",(long long)w.n);
    return 0;
}
