/* vsdlss_m3_transient (transient stepping inside the solver).
 *
 * A = G + Ch on grounded resistor networks (two grids with wires and
 * dangling nodes, a tiny component; caller numbering shuffled); Ch: a
 * capacitor to ground at every node plus a few two-terminal capacitors.
 * Sources at a random 30% of the nodes (some listed twice), dep nodes whose
 * values the callback derives from the solution (as a stateful package model
 * would).  Every step must be bitwise the solution of the documented b with
 * vsdlss_m3_solve_packed, for one batch, single steps and split batches,
 * fused and unfused, relabelled or not, window or level tails, 1 and 4
 * threads.  Plus: fusion engaged with late units, invalid input, non-finite
 * sources, allocation failures in create. */
#include "../src/vsdlss_m3_internal.h"
#include "m3_test_alloc.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

extern int vsdlss_m3_relabel;
extern csi vsdlss_reorder_min;
extern int vsdlss_m3_transient_fuse;
extern long vsdlss_m3_transient_fused_steps;

static int fails;
#define CHECK(c,msg,...) do{ if(!(c)){ fails++; fprintf(stderr,"FAIL: " msg "\n",__VA_ARGS__); } }while(0)

static unsigned long long rng=88172645463325252ULL;
static double urand(void) { rng^=rng<<13; rng^=rng>>7; rng^=rng<<17; return (double)(rng>>11)/9007199254740992.0; }
static csi irand(csi n) { return (csi)(urand()*(double)n)%n; }

/* ---- network: edges (u, v, g), ground g0, component ---- */
typedef struct { csi n, ne, ce; csi *u, *v; double *g, *g0; csi *comp; } net;
static void add_edge(net *w, csi a, csi b, double g)
{
    if(w->ne==w->ce) { w->ce=w->ce?2*w->ce:1024; w->u=realloc(w->u,(size_t)w->ce*sizeof(csi)); w->v=realloc(w->v,(size_t)w->ce*sizeof(csi)); w->g=realloc(w->g,(size_t)w->ce*8); }
    w->u[w->ne]=a; w->v[w->ne]=b; w->g[w->ne++]=g;
}
static csi add_node(net *w, csi comp)
{
    csi i=w->n++;
    w->g0=realloc(w->g0,(size_t)w->n*8); w->comp=realloc(w->comp,(size_t)w->n*sizeof(csi));
    w->g0[i]=1e-3+1e-2*urand(); w->comp[i]=comp;
    return i;
}
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

/* Upper CSC of sum of edge conductances (-g off-diagonal) + diag extra, in
 * caller numbering id[]. */
static vsdlss *edges_csc(csi n, csi ne, const csi *u, const csi *v, const double *g, const double *dextra, const csi *id)
{
    csi *cnt=calloc((size_t)n+1,sizeof(csi)), *fill=calloc((size_t)n,sizeof(csi));
    double *dg=calloc((size_t)n,sizeof(double));
    for(csi i=0;i<n;i++) dg[id[i]]=dextra[i];
    for(csi e=0;e<ne;e++) { csi a=id[u[e]], b=id[v[e]]; dg[a]+=g[e]; dg[b]+=g[e]; cnt[a>b?a:b]++; }
    vsdlss *A=vsdlss_spalloc(n,n,ne+n,1,0);
    A->p[0]=0; for(csi j=0;j<n;j++) A->p[j+1]=A->p[j]+cnt[j]+1;
    for(csi e=0;e<ne;e++) {
        csi a=id[u[e]], b=id[v[e]], lo=a<b?a:b, hi=a<b?b:a, k=A->p[hi]+fill[hi]++;
        A->i[k]=lo; A->x[k]=-g[e];
    }
    for(csi j=0;j<n;j++) {
        csi k=A->p[j]+fill[j]; A->i[k]=j; A->x[k]=dg[j];
        for(csi s=A->p[j]+1;s<=k;s++) { csi ri=A->i[s]; double rx=A->x[s]; csi t=s;
            while(t>A->p[j] && A->i[t-1]>ri) { A->i[t]=A->i[t-1]; A->x[t]=A->x[t-1]; t--; }
            A->i[t]=ri; A->x[t]=rx; }
    }
    free(cnt); free(fill); free(dg);
    return A;
}

/* ---- the system: A = G + Ch ---- */
typedef struct {
    csi n, *id;                     /* node -> caller index */
    vsdlss *A, *Ch;
    csi nsrc, *src;                 /* caller indices */
    csi ndep, *dep, *dsrc;          /* dep nodes and the node each one reads */
    csi K; double *srcv;            /* K x nsrc source values */
} sys;

static sys build(void)
{
    sys S; memset(&S,0,sizeof(S));
    net w; memset(&w,0,sizeof(w));
    grid(&w,30,34,700,0);
    grid(&w,18,15,200,1);
    csi a=add_node(&w,2), b=add_node(&w,2), c=add_node(&w,2), d=add_node(&w,2);
    add_edge(&w,a,b,1.0); add_edge(&w,b,c,2.0); add_edge(&w,c,a,0.5); add_edge(&w,c,d,1.5);
    const csi n=w.n;
    S.n=n; S.id=malloc((size_t)n*sizeof(csi));
    for(csi i=0;i<n;i++) S.id[i]=i;
    for(csi i=n-1;i>0;i--) { csi j=irand(i+1), t=S.id[i]; S.id[i]=S.id[j]; S.id[j]=t; }
    /* capacitors: to ground everywhere; two-terminal between random node
     * pairs of components 0 and 1 (wires and grid alike) */
    double *cg=malloc((size_t)n*8);
    for(csi i=0;i<n;i++) cg[i]=0.5+urand();
    csi ncc=12, cu[12], cv[12]; double cc[12];
    for(csi t=0;t<ncc;t++) {
        csi comp=t%2, x, y;
        do { x=irand(n); } while(w.comp[x]!=comp);
        do { y=irand(n); } while(w.comp[y]!=comp || y==x);
        cu[t]=x; cv[t]=y; cc[t]=0.3+urand();
    }
    S.Ch=edges_csc(n,ncc,cu,cv,cc,cg,S.id);
    /* A = G + ground + Ch */
    csi ne=w.ne+ncc;
    csi *uu=malloc((size_t)ne*sizeof(csi)), *vv=malloc((size_t)ne*sizeof(csi)); double *gg=malloc((size_t)ne*8), *dx=malloc((size_t)n*8);
    for(csi e=0;e<w.ne;e++) { uu[e]=w.u[e]; vv[e]=w.v[e]; gg[e]=w.g[e]; }
    for(csi t=0;t<ncc;t++) { uu[w.ne+t]=cu[t]; vv[w.ne+t]=cv[t]; gg[w.ne+t]=cc[t]; }
    for(csi i=0;i<n;i++) dx[i]=w.g0[i]+cg[i];
    S.A=edges_csc(n,ne,uu,vv,gg,dx,S.id);
    free(uu); free(vv); free(gg); free(dx); free(cg);
    /* sources: 30% of the nodes, a few listed twice */
    S.src=malloc((size_t)n*sizeof(csi));
    for(csi i=0;i<n;i++) if(urand()<0.3) S.src[S.nsrc++]=S.id[i];
    for(csi t=0;t<20;t++) { csi again=S.src[irand(S.nsrc)]; S.src[S.nsrc++]=again; }
    /* dep nodes: some in each component */
    S.ndep=9; S.dep=malloc(9*sizeof(csi)); S.dsrc=malloc(9*sizeof(csi));
    for(csi t=0;t<S.ndep;t++) { csi comp=t%3, x, y;
        do { x=irand(n); } while(w.comp[x]!=comp);
        do { y=irand(n); } while(w.comp[y]!=comp);
        S.dep[t]=S.id[x]; S.dsrc[t]=S.id[y]; }
    S.K=12; S.srcv=malloc((size_t)(S.K*S.nsrc)*8);
    for(csi t=0;t<S.K*S.nsrc;t++) S.srcv[t]=1e-3*(urand()-0.3);
    free(w.u); free(w.v); free(w.g); free(w.g0); free(w.comp);
    return S;
}

static double dep_value(csi k, double xread) { return 0.05*xread+1e-4*sin(0.7*(double)k); }

/* ---- the reference: each b by the documented formula, solved with
 * vsdlss_m3_solve_packed; xs[k] (packed) for k = 0..K ---- */
static double *reference(const sys *S, const vsdlss_m3_factor *F, const csi *slot, const double *x0)
{
    const csi n=S->n, K=S->K;
    double *xs=malloc((size_t)((K+1)*n)*8), *d=calloc((size_t)n,8), *s=malloc((size_t)n*8), *b=malloc((size_t)n*8), *dv=malloc((size_t)S->ndep*8);
    /* partners per packed position, ascending */
    csi *pc=calloc((size_t)n+1,sizeof(csi));
    const vsdlss *C=S->Ch;
    for(csi j=0;j<n;j++) for(csi p=C->p[j];p<C->p[j+1];p++) {
        if(C->i[p]==j) { d[slot[j]]+=C->x[p]; continue; }
        pc[slot[C->i[p]]+1]++; pc[slot[j]+1]++;
    }
    for(csi p=0;p<n;p++) pc[p+1]+=pc[p];
    csi *pu=malloc((size_t)(pc[n]?pc[n]:1)*sizeof(csi)), *fl=calloc((size_t)n,sizeof(csi)); double *pv=malloc((size_t)(pc[n]?pc[n]:1)*8);
    for(csi j=0;j<n;j++) for(csi p=C->p[j];p<C->p[j+1];p++) if(C->i[p]!=j) {
        csi a=slot[C->i[p]], bb=slot[j];
        pu[pc[a]+fl[a]]=bb; pv[pc[a]+fl[a]++]=C->x[p];
        pu[pc[bb]+fl[bb]]=a; pv[pc[bb]+fl[bb]++]=C->x[p];
    }
    for(csi p=0;p<n;p++) for(csi a=pc[p]+1;a<pc[p+1];a++) {          /* sort partners */
        csi u=pu[a]; double v=pv[a]; csi t=a;
        while(t>pc[p] && pu[t-1]>u) { pu[t]=pu[t-1]; pv[t]=pv[t-1]; t--; }
        pu[t]=u; pv[t]=v;
    }
    for(csi g=0;g<n;g++) xs[slot[g]]=x0[g];
    for(csi k=0;k<K;k++) {
        const double *x=xs+k*n;
        for(csi t=0;t<S->ndep;t++) dv[t]=dep_value(k,x[slot[S->dsrc[t]]]);
        memset(s,0,(size_t)n*8);
        for(csi j=0;j<S->nsrc;j++) s[slot[S->src[j]]]+=S->srcv[k*S->nsrc+j];
        for(csi t=0;t<S->ndep;t++) s[slot[S->dep[t]]]+=dv[t];
        for(csi p=0;p<n;p++) {
            double v=d[p]*x[p];
            for(csi a=pc[p];a<pc[p+1];a++) v+=pv[a]*x[pu[a]];
            b[p]=v+s[p];
        }
        CHECK(vsdlss_m3_solve_packed(F,b,xs+(k+1)*n)==VSDLSS_OK,"%s","reference solve");
    }
    free(d); free(s); free(b); free(dv); free(pc); free(pu); free(fl); free(pv);
    return xs;
}

typedef struct { const sys *S; const csi *slot; double *got; csi seen, bad; } cbctx;
static void on_step(void *vc, csi k, const double *x, double *dep_next)
{
    cbctx *c=(cbctx *)vc; const csi n=c->S->n;
    if(k<0 || k>c->S->K) { c->bad++; return; }
    memcpy(c->got+k*n,x,(size_t)n*8);
    c->seen++;
    if(dep_next) for(csi t=0;t<c->S->ndep;t++) dep_next[t]=dep_value(k,x[c->slot[c->S->dsrc[t]]]);
}

static void scenario(const sys *S, int relabel, csi windows, int threads, int fuse, int must_fuse)
{
    char name[128]; snprintf(name,sizeof(name),"relabel=%d windows=%lld threads=%d fuse=%d",relabel,(long long)windows,threads,fuse);
    vsdlss_m3_relabel=relabel; vsdlss_reduce_tail_windows=windows; vsdlss_set_num_threads(threads); vsdlss_m3_transient_fuse=fuse;
    const csi n=S->n, K=S->K;
    vsdlss_m3_factor *F=NULL;
    CHECK(vsdlss_factorize_m3(S->A,5,&F)==VSDLSS_OK,"%s factor",name);
    if(!F) return;
    csi *slot=malloc((size_t)n*sizeof(csi));
    vsdlss_m3_export_packed_slots(F,slot,n);
    double *x0=malloc((size_t)n*8);
    for(csi g=0;g<n;g++) x0[g]=0.9+0.01*sin(0.31*(double)g);
    double *xs=reference(S,F,slot,x0);
    /* batch patterns: all, one by one, 3 + rest */
    const csi pat[3][13]={{12},{1,1,1,1,1,1,1,1,1,1,1,1},{3,9}};
    const int npat[3]={1,12,2};
    for(int pi=0;pi<3;pi++) {
        vsdlss_m3_transient *T=NULL;
        CHECK(vsdlss_m3_transient_create(F,S->Ch,S->nsrc,S->src,S->ndep,S->dep,&T)==VSDLSS_OK,"%s create",name);
        if(!T) continue;
        csi fc=0, units=0, late=0; vsdlss_m3_transient_info(T,&fc,&units,&late);
        if(must_fuse) CHECK(fc>=1 && late>=1 && late<units,"%s: fused components %lld, late units %lld of %lld",name,(long long)fc,(long long)late,(long long)units);
        if(!fuse) CHECK(fc==0,"%s: fused although off",name);
        CHECK(vsdlss_m3_transient_start(T,x0)==VSDLSS_OK,"%s start",name);
        cbctx cx={S,slot,calloc((size_t)((K+1)*n),8),0,0};
        long fs0=vsdlss_m3_transient_fused_steps;
        csi done=0;
        for(int q=0;q<npat[pi];q++) {
            vsdlss_status st=vsdlss_m3_transient_run(T,pat[pi][q],S->srcv+done*S->nsrc,on_step,&cx);
            CHECK(st==VSDLSS_OK,"%s pattern %d run: %s",name,pi,vsdlss_status_string(st));
            done+=pat[pi][q];
        }
        CHECK(cx.seen==K+1 && !cx.bad,"%s pattern %d: %lld callbacks",name,pi,(long long)cx.seen);
        csi first=-1;
        for(csi k=0;k<=K;k++) if(memcmp(cx.got+k*n,xs+k*n,(size_t)n*8) && first<0) first=k;
        CHECK(first<0,"%s pattern %d: step %lld differs from solving its b",name,pi,(long long)first);
        if(must_fuse && pi==0) CHECK(vsdlss_m3_transient_fused_steps>fs0,"%s: no fused iteration",name);
        double *xg=malloc((size_t)n*8);
        CHECK(vsdlss_m3_transient_get(T,-1,NULL,xg)==VSDLSS_OK,"%s get",name);
        int same=1; for(csi g=0;g<n;g++) same&=xg[g]==xs[K*n+slot[g]];
        CHECK(same,"%s get differs",name);
        free(xg); free(cx.got);
        vsdlss_m3_transient_free(T);
    }
    free(xs); free(x0); free(slot);
    vsdlss_m3_factor_free(F);
}

static void misuse(const sys *S)
{
    vsdlss_m3_relabel=1; vsdlss_reduce_tail_windows=64; vsdlss_set_num_threads(1); vsdlss_m3_transient_fuse=1;
    const csi n=S->n;
    vsdlss_m3_factor *F=NULL; vsdlss_m3_transient *T=NULL;
    CHECK(vsdlss_factorize_m3(S->A,5,&F)==VSDLSS_OK,"%s","misuse factor");
    /* a capacitor between components */
    csi a=-1, b=-1;
    {
        csi *slot=malloc((size_t)n*sizeof(csi)); vsdlss_m3_export_packed_slots(F,slot,n);
        const csi *off=F->components->offset;
        for(csi g=0;g<n && (a<0||b<0);g++) { if(slot[g]<off[1] && a<0) a=g; if(slot[g]>=off[1] && slot[g]<off[2] && b<0) b=g; }
        free(slot);
    }
    vsdlss *X=vsdlss_spalloc(n,n,n+1,1,0); csi nz=0;
    for(csi j=0;j<n;j++) { X->p[j]=nz; if(j==(a>b?a:b)) { X->i[nz]=a<b?a:b; X->x[nz++]=-0.1; } X->i[nz]=j; X->x[nz++]=1.0; }
    X->p[n]=nz;
    CHECK(vsdlss_m3_transient_create(F,X,0,NULL,0,NULL,&T)==VSDLSS_ERR_INVALID && !T,"%s","capacitor across components accepted");
    vsdlss_spfree(X);
    csi bad=n;
    CHECK(vsdlss_m3_transient_create(F,S->Ch,1,&bad,0,NULL,&T)==VSDLSS_ERR_INVALID,"%s","source node out of range");
    CHECK(vsdlss_m3_transient_create(NULL,S->Ch,0,NULL,0,NULL,&T)==VSDLSS_ERR_INVALID,"%s","NULL factor");
    CHECK(vsdlss_m3_transient_create(F,S->Ch,S->nsrc,S->src,S->ndep,S->dep,&T)==VSDLSS_OK,"%s","create");
    CHECK(vsdlss_m3_transient_run(T,1,S->srcv,on_step,NULL)==VSDLSS_ERR_INVALID,"%s","run before start");
    double *x0=calloc((size_t)n,8);
    CHECK(vsdlss_m3_transient_start(T,x0)==VSDLSS_OK,"%s","start");
    CHECK(vsdlss_m3_transient_run(T,1,S->srcv,NULL,NULL)==VSDLSS_ERR_INVALID,"%s","dep nodes without callback");
    double *sv=malloc((size_t)S->nsrc*8); memcpy(sv,S->srcv,(size_t)S->nsrc*8); sv[S->nsrc/2]=NAN;
    CHECK(vsdlss_m3_transient_run(T,1,sv,on_step,NULL)==VSDLSS_ERR_NONFINITE,"%s","NaN source");
    x0[3]=INFINITY;
    CHECK(vsdlss_m3_transient_start(T,x0)==VSDLSS_ERR_NONFINITE,"%s","infinite x0");
    vsdlss_m3_transient_free(T); free(sv); free(x0);
    /* allocation failures in create: OOM and nothing leaked, or success */
    size_t live0=m3_alloc_live(), points=0;
    for(size_t k=1;k<5000;k++) {
        T=NULL;
        m3_alloc_fail_at(k);
        vsdlss_status st=vsdlss_m3_transient_create(F,S->Ch,S->nsrc,S->src,S->ndep,S->dep,&T);
        m3_alloc_reset();
        if(st==VSDLSS_OK) { vsdlss_m3_transient_free(T); points=k-1; break; }
        CHECK(st==VSDLSS_ERR_OOM && !T,"create oom %zu: %s",k,vsdlss_status_string(st));
    }
    CHECK(m3_alloc_live()==live0,"create leaked %zu",m3_alloc_live()-live0);
    printf("create allocation failure sweep: %zu points\n",points);
    vsdlss_m3_factor_free(F);
}

int main(void)
{
    vsdlss_reduce_block=256; vsdlss_reduce_tail_min=16; vsdlss_reduce_tail_grain=4;
    vsdlss_reduce_tail_order_grain=4; vsdlss_reorder_min=64;
    sys S=build();
    for(int th=1;th<=4;th*=4) {
        scenario(&S,1,64,th,2,1);       /* fused (forced) */
        scenario(&S,1,64,th,1,0);       /* automatic: fused or not, the same bits */
        scenario(&S,1,64,th,0,0);       /* the same, unfused */
        scenario(&S,0,64,th,1,0);       /* not relabelled: unfused path */
        scenario(&S,1,0,th,1,0);        /* level tail: unfused path */
    }
    /* many sources, every node listed about 600 times in random order: enough
     * work for the parallel scatter, whose chunk edges then fall inside runs
     * of one position (each must still be summed in registration order) */
    {
        sys S2=S; S2.nsrc=400000;
        S2.src=malloc((size_t)S2.nsrc*sizeof(csi)); S2.srcv=malloc((size_t)(S2.K*S2.nsrc)*8);
        for(csi t=0;t<S2.nsrc;t++) S2.src[t]=S.src[irand(S.nsrc)];
        for(csi t=0;t<S2.K*S2.nsrc;t++) S2.srcv[t]=1e-3*(urand()-0.3);
        scenario(&S2,1,64,4,2,1);
        scenario(&S2,1,64,4,0,0);
        free(S2.src); free(S2.srcv);
    }
    misuse(&S);
    if(fails) { fprintf(stderr,"%d failure(s)\n",fails); return 1; }
    printf("test_transient: ALL OK (n=%lld, %lld sources, %lld dep nodes, %lld steps)\n",
           (long long)S.n,(long long)S.nsrc,(long long)S.ndep,(long long)S.K);
    return 0;
}
