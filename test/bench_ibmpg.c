/* IBM power grid benchmark (ibmpg1-6, SPICE DC netlists) through the M3
 * solver, checked against the published reference solution.
 *
 *   ./bench_ibmpg net.spice.bz2 net.solution.bz2
 *   env: PG_THREADS="1 2 4 8"  PG_REPS=15  PG_SHUFFLE=1 (random numbering)
 *        PG_STATS=1   netlist / matrix / factor / conditioning report only
 *        PG_DUMP=file write the assembled system for bench_cholmod and exit
 *        PG_ORDER=k   ordering passed to vsdlss_factorize_m3 (default 0 = AMD)
 * The benchmarks (SPICE netlists and reference solutions) are published at
 * https://web.ece.ucsb.edu/~lip/PGBenchmarks/ibmpgbench.html (Nassif,
 * ASP-DAC 2008); they are not part of this repository.
 *
 * Netlist -> SPD system:
 *   R a b r       conductance 1/r between a and b ("0" is ground)
 *   I a b i       current i leaves node a and enters node b
 *   V a 0 v       pad: node a fixed at v (Dirichlet, moved to the RHS)
 *   V a b 0       zero-volt source between nodes (vias): a and b merged
 * Unknowns are the remaining merged nodes, numbered in order of first
 * appearance in the netlist unless PG_SHUFFLE is set. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static void die(const char *m){fprintf(stderr,"%s\n",m);exit(1);}

/* ---- name -> id hash ---- */
static char *arena; static size_t arena_len, arena_cap;
static size_t *name_at; static csi nnames, names_cap;
static csi *table; static size_t tcap;
static uint64_t hs(const char *s){uint64_t h=1469598103934665603ULL;while(*s){h^=(unsigned char)*s++;h*=1099511628211ULL;}return h;}
static void rehash(void){
    size_t nc=tcap?tcap*2:(1u<<20); csi *nt=malloc(nc*sizeof(csi)); if(!nt)die("oom");
    for(size_t i=0;i<nc;i++)nt[i]=-1;
    for(csi k=0;k<nnames;k++){size_t h=hs(arena+name_at[k])&(nc-1);while(nt[h]>=0)h=(h+1)&(nc-1);nt[h]=k;}
    free(table);table=nt;tcap=nc;
}
static csi lookup(const char *s,int add){
    if(!strcmp(s,"0")||!strcasecmp(s,"gnd"))return -1;
    if(add&&(size_t)(nnames+1)*2>tcap)rehash();
    if(!tcap)return -2;
    size_t h=hs(s)&(tcap-1);
    while(table[h]>=0){if(!strcmp(arena+name_at[table[h]],s))return table[h];h=(h+1)&(tcap-1);}
    if(!add)return -2;
    size_t L=strlen(s)+1;
    if(arena_len+L>arena_cap){arena_cap=arena_cap?arena_cap*2:(64u<<20);arena=realloc(arena,arena_cap);if(!arena)die("oom");}
    memcpy(arena+arena_len,s,L);
    if(nnames==names_cap){names_cap=names_cap?names_cap*2:(1<<20);name_at=realloc(name_at,names_cap*sizeof(size_t));if(!name_at)die("oom");}
    name_at[nnames]=arena_len;arena_len+=L;table[h]=nnames;return nnames++;
}

/* ---- union-find over node ids ---- */
static csi *uf;
static csi find(csi x){while(uf[x]!=x){uf[x]=uf[uf[x]];x=uf[x];}return x;}
static void unite(csi a,csi b){a=find(a);b=find(b);if(a!=b){if(a<b)uf[b]=a;else uf[a]=b;}}

typedef struct{csi a,b;double v;}elem;
typedef struct{elem *e;csi m,cap;}elist;
static void push(elist *l,csi a,csi b,double v){if(l->m==l->cap){l->cap=l->cap?l->cap*2:(1<<20);l->e=realloc(l->e,l->cap*sizeof(elem));if(!l->e)die("oom");}l->e[l->m++]=(elem){a,b,v};}

/* ---- PG_STATS: characteristics of the netlist, the matrix and the factor ---- */
static int cmpd(const void *a,const void *b){double x=*(const double*)a,y=*(const double*)b;return (x>y)-(x<y);}
static double pct(double *v,csi m,double q){if(!m)return NAN;qsort(v,m,sizeof(double),cmpd);csi k=(csi)(q*(m-1));return v[k];}
/* "n<layer>_<x>_<y>", optionally prefixed "_X_" (package side). */
static int parse_node(const char *s,int *layer,long *x,long *y,int *pkg)
{
    char *e; *pkg=0;
    if(!strncmp(s,"_X_",3)){*pkg=1;s+=3;}
    if(s[0]!='n')return 0;
    long L=strtol(s+1,&e,10); if(*e!='_')return 0;
    long X=strtol(e+1,&e,10); if(*e!='_')return 0;
    long Y=strtol(e+1,&e,10); if(*e)return 0;
    if(L<0||L>63)return 0;
    *layer=(int)L;*x=X;*y=Y;return 1;
}
typedef struct{double *v;csi m;}dvec;
static void dpush(dvec *d,double x,csi cap){if(!d->v){d->v=malloc(cap*sizeof(double));if(!d->v)die("oom");}d->v[d->m++]=x;}

static void run_stats(csi N,const elist *R,const elist *I,const elist *V,const char *fixed,
                      const double *fixv,const vsdlss *A,const double *rhs,
                      const double *ref,const char *has)
{
    const csi n=A->n;
    /* 1. Netlist structure: layers, resistor classes, pads, loads. */
    csi layer_nodes[64]={0},pkg_nodes=0,other_nodes=0; int maxl=-1;
    for(csi i=0;i<N;i++){int L,pk;long x,y;
        if(!parse_node(arena+name_at[i],&L,&x,&y,&pk)){other_nodes++;continue;}
        if(pk){pkg_nodes++;continue;}
        layer_nodes[L]++; if(L>maxl)maxl=L;}
    printf("\n[1] netlist: %lld nodes; package-side (_X_) %lld; unparsed names %lld\n",(long long)N,(long long)pkg_nodes,(long long)other_nodes);
    dvec cls[4]={{0}}; const char *cname[4]={"intra-layer wire","via (layer to layer)","package/pad","other"};
    dvec lr[64]={{0}}, ll[64]={{0}};
    for(csi k=0;k<R->m;k++){elem e=R->e[k]; int c=3;
        int La,Lb,pa=0,pb=0; long xa=0,ya=0,xb=0,yb=0;
        int oa=e.a>=0&&parse_node(arena+name_at[e.a],&La,&xa,&ya,&pa);
        int ob=e.b>=0&&parse_node(arena+name_at[e.b],&Lb,&xb,&yb,&pb);
        if(pa||pb) c=2;
        else if(oa&&ob&&La!=Lb) c=1;
        else if(oa&&ob&&La==Lb){ c=0; dpush(&lr[La],e.v,R->m); dpush(&ll[La],(double)(labs(xa-xb)+labs(ya-yb)),R->m); }
        dpush(&cls[c],e.v,R->m);}
    printf("    resistors by class (ohm): count  min  median  max\n");
    for(int c=0;c<4;c++) if(cls[c].m){ csi m=cls[c].m; double lo=pct(cls[c].v,m,0),md=pct(cls[c].v,m,0.5),hi=pct(cls[c].v,m,1);
        printf("      %-22s %9lld  %.3g  %.3g  %.3g\n",cname[c],(long long)m,lo,md,hi); free(cls[c].v);}
    printf("    per metal layer: nodes, wire segments, median R (ohm), median segment length (name units)\n");
    for(int L=0;L<=maxl;L++) if(layer_nodes[L]||lr[L].m){
        printf("      layer %2d: %9lld nodes %9lld segs  R_med=%.3g  len_med=%.0f\n",L,(long long)layer_nodes[L],(long long)lr[L].m,pct(lr[L].v,lr[L].m,0.5),pct(ll[L].v,ll[L].m,0.5));
        free(lr[L].v);free(ll[L].v);}
    csi pads_hi=0,pads_lo=0,shorts=0;
    for(csi k=0;k<V->m;k++){elem e=V->e[k]; if(e.a>=0&&e.b>=0)shorts++; else if(fabs(e.v)>0)pads_hi++; else pads_lo++;}
    double isum_v=0,isum_g=0; csi iv=0,ig=0,izero=0;
    for(csi k=0;k<I->m;k++){elem e=I->e[k]; if(e.v==0)izero++; if(e.b<0){iv++;isum_v+=e.v;} else {ig++;isum_g+=e.v;}}
    printf("    pads: %lld to VDD, %lld to GND; zero-volt via sources %lld\n",(long long)pads_hi,(long long)pads_lo,(long long)shorts);
    printf("    current sources: %lld (node->0, VDD loads, total %.3f A) + %lld (0->node, GND, total %.3f A); zero-valued %lld\n",
        (long long)iv,isum_v,(long long)ig,isum_g,(long long)izero);
    printf("    unknowns per pad: %.0f\n",(double)n/(double)(pads_hi+pads_lo?pads_hi+pads_lo:1));

    /* 2. Matrix. */
    csi *deg=calloc(n,sizeof(csi)); double *off=calloc(n,sizeof(double)),*dg=malloc(n*sizeof(double));
    dvec gv={0},span={0}; csi m=0;
    for(csi j=0;j<n;j++) for(csi p=A->p[j];p<A->p[j+1];p++){csi i=A->i[p];
        if(i==j){dg[j]=A->x[p];continue;}
        deg[i]++;deg[j]++;off[i]+=fabs(A->x[p]);off[j]+=fabs(A->x[p]);m++;
        dpush(&gv,fabs(A->x[p]),A->p[n]); dpush(&span,(double)(j-i),A->p[n]);}
    csi hist[10]={0},grounded=0; double maxd=0;
    dvec slackr={0};
    for(csi i=0;i<n;i++){hist[deg[i]<9?deg[i]:9]++; double s=dg[i]-off[i]; if(s>1e-12*dg[i]){grounded++;dpush(&slackr,s/dg[i],n);} if(dg[i]>maxd)maxd=dg[i];}
    csi rhsnz=0; for(csi i=0;i<n;i++) if(rhs[i]!=0) rhsnz++;
    printf("\n[2] matrix: n=%lld, off-diagonal pairs=%lld, mean degree %.2f\n",(long long)n,(long long)m,2.0*m/n);
    printf("    degree histogram (share of unknowns):");
    for(int d=0;d<10;d++) if(hist[d]) printf("  %s%d:%.1f%%",d==9?">=":"",d,100.0*hist[d]/n);
    printf("\n    conductance |a_ij| (S): min %.3g  median %.3g  max %.3g  -> spread %.1e\n",pct(gv.v,gv.m,0),pct(gv.v,gv.m,0.5),pct(gv.v,gv.m,1),pct(gv.v,gv.m,1)/pct(gv.v,gv.m,0));
    printf("    rows with a direct path to a fixed node (strictly diagonally dominant): %lld (%.3f%%), median slack/diag %.2g; others exactly weakly dominant\n",
        (long long)grounded,100.0*grounded/n,pct(slackr.v,slackr.m,0.5));
    printf("    RHS nonzeros: %lld (%.1f%% of unknowns)\n",(long long)rhsnz,100.0*rhsnz/n);
    printf("    numbering locality (netlist order): |i-j| over edges median %.0f, 90%% %.0f, max %.0f (n=%lld)\n",
        pct(span.v,span.m,0.5),pct(span.v,span.m,0.9),pct(span.v,span.m,1),(long long)n);
    free(gv.v);free(span.v);free(slackr.v);

    /* 3. Solver view: reduction, core, supernodes, fill, tree, flops. */
    vsdlss_set_num_threads(1);
    vsdlss_m3_factor *F=NULL; double t=now();
    if(vsdlss_factorize_m3(A,getenv("PG_ORDER")?atoi(getenv("PG_ORDER")):0,&F)!=VSDLSS_OK)die("factorize failed");
    double tf=now()-t;
    printf("\n[3] solver view (AMD, 1 thread, factor %.2fs): %lld components\n",tf,(long long)F->count);
    csi order[4]={0}; int no=0;                     /* the four largest components */
    for(csi c=0;c<F->count;c++){ csi nc=F->component[c].n; int at;
        if(no<4) at=no++; else if(nc>F->component[order[3]].n) at=3; else continue;
        while(at>0&&F->component[order[at-1]].n<nc){order[at]=order[at-1];at--;} order[at]=c; }
    double tot_flops=0; csi tot_lnz=0,tot_core=0;
    for(int q=0;q<no&&q<4;q++){ csi c=order[q]; const vsdlss_m3_component_factor *cf=F->component+c;
        const vsdlss_reduction *r=cf->reduction; const vsdlss_sn_factor *s=cf->numeric;
        printf("    component %lld: n=%lld, low-degree eliminated %lld (%.1f%%), core %lld (%.1f%%)\n",(long long)c,(long long)cf->n,
            (long long)r->count,100.0*r->count/cf->n,(long long)r->core_n,100.0*r->core_n/cf->n);
        if(!s) continue;
        csi wb[6]={0}; csi maxw=0; double flops=0; csi *depth=calloc(s->count,sizeof(csi)); csi h=0; double exts=0;
        for(csi d=s->count-1;d>=0;d--){ csi p=s->sn_parent[d]; depth[d]=p>=0?depth[p]+1:1; if(depth[d]>h)h=depth[d]; }
        for(csi d=0;d<s->count;d++){ csi w=s->column_start[d+1]-s->column_start[d], e=s->row_ptr[d+1]-s->row_ptr[d];
            exts+=e; if(w>maxw)maxw=w;
            wb[w==1?0:w<=4?1:w<=16?2:w<=64?3:w<=256?4:5]++;
            for(csi k=1;k<=w;k++) flops+=(double)(k+e)*(double)(k+e); }
        free(depth);
        printf("      supernodes %lld (mean width %.1f, max %lld, mean external rows %.1f); width 1:%.0f%% 2-4:%.0f%% 5-16:%.0f%% 17-64:%.0f%% 65-256:%.0f%% >256:%.0f%%\n",
            (long long)s->count,(double)s->n/s->count,(long long)maxw,exts/s->count,
            100.0*wb[0]/s->count,100.0*wb[1]/s->count,100.0*wb[2]/s->count,100.0*wb[3]/s->count,100.0*wb[4]/s->count,100.0*wb[5]/s->count);
        printf("      nnz(L)=%.2fM (%.1f per core column), supernodal tree height %lld, factor flops ~%.2f G\n",
            s->l_nnz*1e-6,(double)s->l_nnz/s->n,(long long)h,flops*1e-9);
        tot_flops+=flops; tot_lnz+=s->l_nnz; tot_core+=r->core_n; }
    printf("    total: core %lld (%.1f%% of n), nnz(L) %.2fM = %.1fx nnz(A upper); ~%.2f GFLOP, %.2f GFLOP/s at 1 thread\n",
        (long long)tot_core,100.0*tot_core/n,tot_lnz*1e-6,(double)tot_lnz/A->p[n],tot_flops*1e-9,tot_flops*1e-9/tf);

    /* 4. Numerics: condition number, IR drop, accuracy needs. */
    double *x=malloc(n*8),*y=malloc(n*8); double lmax=0,lmin=0,nr;
    for(csi i=0;i<n;i++)x[i]=1.0+0.37*sin((double)i);
    for(int it=0;it<60;it++){ vsdlss_spmv_sym_upper(A,x,y); nr=0; for(csi i=0;i<n;i++)nr+=y[i]*y[i]; nr=sqrt(nr); double xy=0,xx=0;
        for(csi i=0;i<n;i++){xy+=x[i]*y[i];xx+=x[i]*x[i];} lmax=xy/xx; for(csi i=0;i<n;i++)x[i]=y[i]/nr; }
    for(csi i=0;i<n;i++)x[i]=1.0;
    for(int it=0;it<40;it++){ if(vsdlss_m3_solve(F,x,y)!=VSDLSS_OK)die("solve failed"); double xy=0,yy=0;
        for(csi i=0;i<n;i++){xy+=x[i]*y[i];yy+=y[i]*y[i];} lmin=xy/yy; nr=sqrt(yy); for(csi i=0;i<n;i++)x[i]=y[i]/nr; }
    printf("\n[4] numerics: lambda_max ~%.3g (Gershgorin <= %.3g), lambda_min ~%.3g, condition number ~%.2e\n",lmax,2*maxd,lmin,lmax/lmin);
    double vmin=1e9,vmax=-1e9,gmax=-1e9,vdd=0; csi nv=0,ng=0; double dsum=0;
    for(csi i=0;i<N;i++) if(fixed[i]&&fixv[i]>vdd) vdd=fixv[i];
    /* Net of each node: connected through resistors (after via merging);
     * a net is VDD-side when one of its pads sits at a positive voltage. */
    csi *p3=malloc(N*sizeof(csi)); double *netv=calloc(N,sizeof(double)); if(!p3||!netv)die("oom");
    for(csi i=0;i<N;i++)p3[i]=i;
    #define F3(v) ({csi r_=(v);while(p3[r_]!=r_)r_=p3[r_]=p3[p3[r_]];r_;})
    for(csi k=0;k<R->m;k++){elem e=R->e[k]; if(e.a<0||e.b<0)continue; csi a=F3(find(e.a)),b=F3(find(e.b)); if(a!=b)p3[a>b?a:b]=a<b?a:b;}
    for(csi i=0;i<N;i++){csi r=find(i); if(fixed[r]&&fixv[r]>netv[F3(r)])netv[F3(r)]=fixv[r];}
    for(csi i=0;i<N;i++) if(has[i]){ if(netv[F3(find(i))]>0){nv++; if(ref[i]<vmin)vmin=ref[i]; if(ref[i]>vmax)vmax=ref[i]; dsum+=vdd-ref[i];} else {ng++; if(ref[i]>gmax)gmax=ref[i];} }
    printf("    IR drop (reference solution): VDD=%.3g V, %lld VDD-side nodes, worst drop %.2f mV (%.2f%%), mean %.2f mV; %lld GND-side nodes, worst bounce %.2f mV\n",
        vdd,(long long)nv,1e3*(vdd-vmin),100*(vdd-vmin)/vdd,1e3*dsum/(nv?nv:1),(long long)ng,1e3*gmax);
    #undef F3
    free(p3);free(netv);free(x);free(y);free(deg);free(off);free(dg);
    vsdlss_m3_factor_free(F);
}

static FILE *zopen(const char *path){
    char cmd[4096]; snprintf(cmd,sizeof cmd,"bzcat '%s'",path);
    FILE *f=popen(cmd,"r"); if(!f)die("popen failed"); return f;
}

int main(int argc,char **argv)
{
    if(argc<3)die("usage: bench_ibmpg net.spice.bz2 net.solution.bz2");
    double t0=now();
    elist R={0},I={0},V={0};
    FILE *f=zopen(argv[1]); char line[4096],t1[1024],t2[1024],t3[1024],t4[1024];
    while(fgets(line,sizeof line,f)){
        char c=(char)toupper((unsigned char)line[0]);
        if(c!='R'&&c!='I'&&c!='V')continue;
        if(sscanf(line,"%1023s %1023s %1023s %1023s",t1,t2,t3,t4)!=4)continue;
        csi a=lookup(t2,1),b=lookup(t3,1); double v=atof(t4);
        if(c=='R')push(&R,a,b,v); else if(c=='I')push(&I,a,b,v); else push(&V,a,b,v);
    }
    pclose(f);
    const csi N=nnames;
    uf=malloc(N*sizeof(csi)); double *fixv=malloc(N*sizeof(double)); char *fixed=calloc(N,1);
    if(!uf||!fixv||!fixed)die("oom");
    for(csi i=0;i<N;i++)uf[i]=i;
    csi shorts=0,pads=0;
    for(csi k=0;k<V.m;k++){elem e=V.e[k];
        if(e.a>=0&&e.b>=0){ if(e.v!=0)die("non-zero V between two nodes: unsupported"); unite(e.a,e.b); shorts++; }}
    for(csi k=0;k<V.m;k++){elem e=V.e[k];
        if(e.a>=0&&e.b>=0)continue;
        csi n=e.a>=0?e.a:e.b; double v=e.a>=0?e.v:-e.v; csi r=find(n);
        if(fixed[r]&&fixv[r]!=v)die("conflicting pad voltages");
        fixed[r]=1;fixv[r]=v;pads++;}
    /* Unknowns: unfixed roots, in first-appearance order. */
    csi *idx=malloc(N*sizeof(csi)); if(!idx)die("oom");
    for(csi i=0;i<N;i++)idx[i]=-1;
    csi n=0;
    for(csi i=0;i<N;i++){csi r=find(i);if(!fixed[r]&&idx[r]<0)idx[r]=n++;}
    const char *se=getenv("PG_SHUFFLE");
    if(se&&atoi(se)){ csi *p=malloc(n*sizeof(csi)); uint64_t s=0x9E3779B97F4A7C15ULL;
        for(csi i=0;i<n;i++)p[i]=i;
        for(csi i=n-1;i>0;i--){s^=s<<13;s^=s>>7;s^=s<<17;csi j=(csi)(s%(uint64_t)(i+1)),t=p[i];p[i]=p[j];p[j]=t;}
        for(csi i=0;i<N;i++)if(idx[i]>=0)idx[i]=p[idx[i]];
        free(p); }
    /* Assemble: off-diagonal (i<j) entries, diagonal, RHS. */
    double *diag=calloc(n,sizeof(double)),*rhs=calloc(n,sizeof(double));
    csi *ea=malloc(R.m*sizeof(csi)),*eb=malloc(R.m*sizeof(csi)); double *eg=malloc(R.m*sizeof(double));
    char *grounded=calloc(n,1);
    if(!diag||!rhs||!ea||!eb||!eg||!grounded)die("oom");
    csi m=0,selfloops=0;
    for(csi k=0;k<R.m;k++){elem e=R.e[k]; if(e.v<=0)die("non-positive resistor");
        double g=1.0/e.v;
        csi ra=e.a>=0?find(e.a):-1, rb=e.b>=0?find(e.b):-1;
        if(ra==rb){selfloops++;continue;}
        int ua=ra>=0&&!fixed[ra], ub=rb>=0&&!fixed[rb];
        double va=ra<0?0:fixv[ra], vb=rb<0?0:fixv[rb];
        if(ua&&ub){csi i=idx[ra],j=idx[rb];diag[i]+=g;diag[j]+=g;ea[m]=i<j?i:j;eb[m]=i<j?j:i;eg[m]=-g;m++;}
        else if(ua){csi i=idx[ra];diag[i]+=g;rhs[i]+=g*vb;grounded[i]=1;}
        else if(ub){csi j=idx[rb];diag[j]+=g;rhs[j]+=g*va;grounded[j]=1;}
    }
    for(csi k=0;k<I.m;k++){elem e=I.e[k];
        if(e.a>=0){csi r=find(e.a);if(!fixed[r])rhs[idx[r]]-=e.v;}
        if(e.b>=0){csi r=find(e.b);if(!fixed[r])rhs[idx[r]]+=e.v;}}
    /* Floating groups (no resistive path to a fixed node) make A singular. */
    { csi *u2=malloc(n*sizeof(csi)); char *g2=calloc(n,1); if(!u2||!g2)die("oom");
      for(csi i=0;i<n;i++)u2[i]=i;
      for(csi k=0;k<m;k++){csi a=ea[k],b=eb[k];while(u2[a]!=a)a=u2[a]=u2[u2[a]];while(u2[b]!=b)b=u2[b]=u2[u2[b]];if(a!=b)u2[a>b?a:b]=a<b?a:b;}
      csi comps=0,floating=0;
      for(csi i=0;i<n;i++){csi r=i;while(u2[r]!=r)r=u2[r];if(grounded[i])g2[r]=1;}
      for(csi i=0;i<n;i++)if(u2[i]==i){comps++;if(!g2[i])floating++;}
      printf("# groups=%lld floating=%lld\n",(long long)comps,(long long)floating);
      if(floating)die("floating groups: matrix is singular");
      free(u2);free(g2); }
    /* Upper CSC: count, fill, sort each column, merge duplicates. */
    vsdlss *A=vsdlss_spalloc(n,n,n+m,1,0); if(!A)die("oom");
    csi *cnt=calloc(n+1,sizeof(csi)); if(!cnt)die("oom");
    for(csi k=0;k<m;k++)cnt[eb[k]+1]++;
    for(csi j=0;j<n;j++)cnt[j+1]+=cnt[j]+1;
    memcpy(A->p,cnt,(n+1)*sizeof(csi));
    csi *cur=malloc(n*sizeof(csi)); for(csi j=0;j<n;j++)cur[j]=A->p[j];
    for(csi k=0;k<m;k++){csi j=eb[k];A->i[cur[j]]=ea[k];A->x[cur[j]++]=eg[k];}
    for(csi j=0;j<n;j++){A->i[cur[j]]=j;A->x[cur[j]++]=diag[j];}
    csi w=0;
    for(csi j=0;j<n;j++){csi p0=A->p[j],p1=A->p[j+1];
        for(csi p=p0+1;p<p1;p++)for(csi q=p;q>p0&&A->i[q]<A->i[q-1];q--){csi ti=A->i[q];double tx=A->x[q];A->i[q]=A->i[q-1];A->x[q]=A->x[q-1];A->i[q-1]=ti;A->x[q-1]=tx;}
        A->p[j]=w;
        for(csi p=p0;p<p1;p++){if(w>A->p[j]&&A->i[w-1]==A->i[p])A->x[w-1]+=A->x[p];else{A->i[w]=A->i[p];A->x[w]=A->x[p];w++;}}}
    A->p[n]=w;
    free(cur);free(cnt);free(ea);free(eb);free(eg);free(diag);free(grounded);
    printf("# netlist nodes=%lld R=%lld I=%lld V=%lld (pads=%lld via-shorts=%lld) unknowns=%lld nnz_upper=%lld parse+assemble=%.2fs\n",
        (long long)N,(long long)R.m,(long long)I.m,(long long)V.m,(long long)pads,(long long)shorts,(long long)n,(long long)w,now()-t0);
    fflush(stdout);

    /* Reference solution: name -> voltage. */
    double *ref=malloc(N*sizeof(double)); char *has=calloc(N,1); if(!ref||!has)die("oom");
    f=zopen(argv[2]); csi nref=0;
    while(fgets(line,sizeof line,f)){ if(sscanf(line,"%1023s %1023s",t1,t2)!=2)continue;
        csi id=lookup(t1,0); if(id<0)continue; ref[id]=atof(t2); has[id]=1; nref++; }
    pclose(f);
    { const char *dp=getenv("PG_DUMP");     /* n, nnz, p[n+1], i[nnz], x[nnz], b[n] */
      if(dp){ FILE *o=fopen(dp,"wb"); if(!o)die("dump open failed"); int64_t nn=n,nz=A->p[n];
        fwrite(&nn,8,1,o);fwrite(&nz,8,1,o);fwrite(A->p,8,n+1,o);fwrite(A->i,8,nz,o);fwrite(A->x,8,nz,o);fwrite(rhs,8,n,o);
        fclose(o); printf("# dumped to %s\n",dp); return 0; } }
    if(getenv("PG_STATS")){
        run_stats(N,&R,&I,&V,fixed,fixv,A,rhs,ref,has);
        return 0;
    }

    const char *tl=getenv("PG_THREADS"); if(!tl)tl="1 2 4 8";
    const char *re=getenv("PG_REPS"); int reps=re?atoi(re):15; if(reps>256)reps=256; if(reps<1)reps=1;
    double *x=malloc(n*8),*x0=malloc(n*8),*bi=malloc(n*8),*xi=malloc(n*8); csi *perm=malloc(n*sizeof(csi));
    if(!x||!x0||!bi||!xi||!perm)die("oom");
    char buf[256]; strncpy(buf,tl,255); buf[255]=0; int first=1;
    for(char *tok=strtok(buf," ");tok;tok=strtok(NULL," ")){
        int t=atoi(tok); if(vsdlss_set_num_threads(t)!=VSDLSS_OK)die("bad thread count");
        vsdlss_m3_factor *F=NULL; double a=now();
        if(vsdlss_factorize_m3(A,getenv("PG_ORDER")?atoi(getenv("PG_ORDER")):0,&F)!=VSDLSS_OK)die("factorize failed");
        double tf=now()-a;
        csi core=0,lnz=0; for(csi c=0;c<F->count;c++){core+=F->component[c].reduction->core_n;if(F->component[c].numeric)lnz+=F->component[c].numeric->l_nnz;}
        double ts[256],ti[256];
        for(int r=-2;r<reps;r++){a=now();if(vsdlss_m3_solve(F,rhs,x)!=VSDLSS_OK)die("solve failed");if(r>=0)ts[r]=now()-a;}
        if(vsdlss_m3_internal_order(F,perm)!=VSDLSS_OK)die("internal_order failed");
        for(csi p=0;p<n;p++)bi[p]=rhs[perm[p]];
        for(int r=-2;r<reps;r++){a=now();if(vsdlss_m3_solve_internal(F,bi,xi)!=VSDLSS_OK)die("internal solve failed");if(r>=0)ti[r]=now()-a;}
        int same=1; for(csi p=0;p<n;p++)if(memcmp(&xi[p],&x[perm[p]],8)){same=0;break;}
        if(first){memcpy(x0,x,n*8);first=0;}
        int thread_same=!memcmp(x0,x,n*8);
        for(int i=1;i<reps;i++){double v=ts[i];int j=i;while(j>0&&ts[j-1]>v){ts[j]=ts[j-1];j--;}ts[j]=v;}
        for(int i=1;i<reps;i++){double v=ti[i];int j=i;while(j>0&&ti[j-1]>v){ti[j]=ti[j-1];j--;}ti[j]=v;}
        double eta=0; vsdlss_backward_error(A,x,rhs,&eta);
        double maxerr=0,sumerr=0; csi cmp=0;
        for(csi id=0;id<N;id++)if(has[id]){csi r=find(id);double v=fixed[r]?fixv[r]:x[idx[r]];double d=fabs(v-ref[id]);if(d>maxerr)maxerr=d;sumerr+=d;cmp++;}
        printf("T=%d factor=%.3fs components=%lld core_n=%lld nnzL=%.2fM | solve med=%.4fs min=%.4fs | internal med=%.4fs min=%.4fs | bitwise internal=%d threads=%d | backward_err=%.1e | vs IBM ref: %lld nodes max|dV|=%.2e mean=%.2e\n",
            t,tf,(long long)F->count,(long long)core,lnz*1e-6,ts[reps/2],ts[0],ti[reps/2],ti[0],same,thread_same,eta,(long long)cmp,maxerr,cmp?sumerr/cmp:0);
        fflush(stdout);
        vsdlss_m3_factor_free(F);
    }
    (void)nref;
    return 0;
}
