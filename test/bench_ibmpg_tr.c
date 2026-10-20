/* IBM power grid transient benchmarks (ibmpg1t-6t) through vsdlss_m3_transient,
 * checked against stepping by hand and against the published waveforms.
 *
 *   ./bench_ibmpg_tr net.spice[.bz2] net.output[.bz2|.gz]
 *   env: PG_ORDER=k (ordering, default 0), PG_THREADS=n (default 1),
 *        PG_STEPS=n (default: all steps of .tran), PG_BLOCK=n (reduction
 *        block, default 65536; smaller gives small cases blocks and a
 *        fusable tail), PG_TAILMIN=n (smallest scheduled tail, default
 *        4096), PG_FUSE=1|2 (fusion of "tr+fuse": automatic, the default,
 *        or always), PG_ROUNDS=n (rounds of the three ways, order rotated,
 *        best time each; default 3), PG_BATCH=n (steps per run
 *        call, default all)
 * The benchmarks are published at
 * https://web.ece.ucsb.edu/~lip/PGBenchmarks/ibmpgbench.html (Nassif, ASP-DAC
 * 2008); they are not part of this repository.
 *
 * Netlist (case-insensitive element letters):
 *   R a b r           resistor; r = 0 merges a and b
 *   C a b c           capacitor
 *   L a b l           inductor (package)
 *   I a b dc [pulse(v1, v2, td, tr, tf, pw, per)]   current from a to b
 *   V a 0 v           pad: a fixed at v;  V a b 0: a and b merged (vias)
 *   .tran h tstop     step and end time;  .print tran v(node) ...  outputs
 * Backward Euler with step h: A = G + C/h + h/L; b_{k+1} = (C/h) x_k + sources:
 * current sources at t_{k+1}, constant terms of conductances to pads, and for
 * each inductor its history current (state i_L, updated from x_k in the
 * callback: the dep nodes of the interface).  x_0 is the DC solution (C open,
 * L shorted) with the sources at t = 0.
 *
 * Three ways, each must give the same bits: "manual" (b by the interface's
 * formula, vsdlss_m3_solve_packed_inplace per step), "tr" (the interface,
 * VSDLSS_TR_FUSE off) and "tr+fuse".  The printed nodes are then compared
 * with the published waveforms (their integration method is not stated; a
 * different one shows up as an O(h) difference). */
#define _GNU_SOURCE
#include "../src/vsdlss_m3_internal.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

extern int vsdlss_m3_transient_fuse;
extern csi vsdlss_reduce_block, vsdlss_reduce_tail_min;

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static void die(const char *m){fprintf(stderr,"%s\n",m);exit(1);}

/* ---- name -> id hash (as bench_ibmpg) ---- */
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

typedef struct{csi a,b;double v;}elem;
typedef struct{elem *e;csi m,cap;}elist;
static void push(elist *l,csi a,csi b,double v){if(l->m==l->cap){l->cap=l->cap?l->cap*2:(1<<16);l->e=realloc(l->e,(size_t)l->cap*sizeof(elem));if(!l->e)die("oom");}l->e[l->m++]=(elem){a,b,v};}
typedef struct{csi a,b; double dc; int pulse; double p[7];}isrc;

/* ---- union-find ---- */
static csi find(csi *uf,csi x){while(uf[x]!=x){uf[x]=uf[uf[x]];x=uf[x];}return x;}
static void unite(csi *uf,csi a,csi b){a=find(uf,a);b=find(uf,b);if(a!=b){if(a<b)uf[b]=a;else uf[a]=b;}}

static FILE *zopen(const char *path){
    char cmd[4096]; size_t L=strlen(path);
    if(L>4&&!strcmp(path+L-4,".bz2")) snprintf(cmd,sizeof cmd,"bzcat '%s'",path);
    else if(L>3&&!strcmp(path+L-3,".gz")) snprintf(cmd,sizeof cmd,"zcat '%s'",path);
    else snprintf(cmd,sizeof cmd,"cat '%s'",path);
    FILE *f=popen(cmd,"r"); if(!f)die("popen failed"); return f;
}

static double pulse_at(const isrc *s,double t)
{
    if(!s->pulse) return s->dc;
    const double v1=s->p[0],v2=s->p[1],td=s->p[2],tr=s->p[3],tf=s->p[4],pw=s->p[5],per=s->p[6];
    if(t<td) return v1;
    double tt=t-td; if(per>0) tt=fmod(tt,per);
    if(tt<tr) return tr>0?v1+(v2-v1)*tt/tr:v2;
    if(tt<tr+pw) return v2;
    if(tt<tr+pw+tf) return tf>0?v2+(v1-v2)*(tt-tr-pw)/tf:v1;
    return v1;
}

/* Upper CSC from (a<=b) triplets, duplicates summed, rows sorted. */
static vsdlss *csc(csi n,csi m,const csi *ea,const csi *eb,const double *ex)
{
    csi *cnt=calloc((size_t)n+1,sizeof(csi)); if(!cnt)die("oom");
    for(csi k=0;k<m;k++)cnt[eb[k]+1]++;
    for(csi j=0;j<n;j++)cnt[j+1]+=cnt[j];
    vsdlss *A=vsdlss_spalloc(n,n,m?m:1,1,0); if(!A)die("oom");
    csi *cur=malloc((size_t)(n?n:1)*sizeof(csi)); for(csi j=0;j<n;j++)cur[j]=cnt[j];
    for(csi k=0;k<m;k++){csi j=eb[k];A->i[cur[j]]=ea[k];A->x[cur[j]++]=ex[k];}
    csi w=0;
    for(csi j=0;j<n;j++){csi p0=cnt[j],p1=cnt[j+1];
        for(csi p=p0+1;p<p1;p++)for(csi q=p;q>p0&&A->i[q]<A->i[q-1];q--){csi ti=A->i[q];double tx=A->x[q];A->i[q]=A->i[q-1];A->x[q]=A->x[q-1];A->i[q-1]=ti;A->x[q-1]=tx;}
        A->p[j]=w;
        for(csi p=p0;p<p1;p++){if(w>A->p[j]&&A->i[w-1]==A->i[p])A->x[w-1]+=A->x[p];else{A->i[w]=A->i[p];A->x[w]=A->x[p];w++;}}}
    A->p[n]=w;
    free(cnt);free(cur);
    return A;
}

/* triplet accumulator */
typedef struct{csi *a,*b;double *x;csi m,cap;}trip;
static void tpush(trip *t,csi a,csi b,double x){
    if(t->m==t->cap){t->cap=t->cap?2*t->cap:(1<<16);t->a=realloc(t->a,(size_t)t->cap*sizeof(csi));t->b=realloc(t->b,(size_t)t->cap*sizeof(csi));t->x=realloc(t->x,(size_t)t->cap*8);if(!t->a||!t->b||!t->x)die("oom");}
    t->a[t->m]=a<b?a:b; t->b[t->m]=a<b?b:a; t->x[t->m++]=x;
}

/* ---- the transient problem in unknown numbering ---- */
typedef struct {
    csi n;                     /* unknowns */
    csi nsrc, *src;            /* source entries: unknown */
    double *sconst;            /* constant part of each entry */
    csi *sI; double *ssign;    /* entry -> current source (or -1) and sign */
    csi nind;                  /* inductors */
    csi *lx, *ly;              /* endpoint unknowns (-1: fixed/ground) */
    double *lvx, *lvy, *lg;    /* fixed endpoint voltages, h/L */
    double *il0;               /* initial inductor currents (DC) */
    csi ndep, *dep, *dind; double *dsign;   /* dep entries: unknown, inductor, sign */
} trp;

/* dep values for step k+1 from the state after step k */
static void dep_values(const trp *P,const double *il,double *out)
{ for(csi t=0;t<P->ndep;t++) out[t]=P->dsign[t]*il[P->dind[t]]; }
/* i_L,k = i_L,k-1 + (h/L)(v_x - v_y) at step k */
static void il_update(const trp *P,double *il,const double *x_unknown_of_packed,const csi *slot)
{
    for(csi q=0;q<P->nind;q++){
        double vx=P->lx[q]>=0?x_unknown_of_packed[slot[P->lx[q]]]:P->lvx[q];
        double vy=P->ly[q]>=0?x_unknown_of_packed[slot[P->ly[q]]]:P->lvy[q];
        il[q]+=P->lg[q]*(vx-vy);
    }
}

typedef struct {
    const trp *P; const csi *slot;
    double *il;                /* inductor state */
    csi nout; const csi *out_u; const double *out_fix;   /* outputs: unknown or fixed value */
    double *rec;               /* (steps+1) x nout */
    csi steps;
} cbctx;
static void on_step(void *v,csi k,const double *x,double *dep_next)
{
    cbctx *c=(cbctx *)v;
    if(k>0) il_update(c->P,c->il,x,c->slot);
    if(dep_next) dep_values(c->P,c->il,dep_next);
    if(k<=c->steps) for(csi j=0;j<c->nout;j++) c->rec[k*c->nout+j]=c->out_u[j]>=0?x[c->slot[c->out_u[j]]]:c->out_fix[j];
}

int main(int argc,char **argv)
{
    if(argc<3)die("usage: bench_ibmpg_tr net.spice[.bz2] net.output[.bz2|.gz]");
    const char *e;
    const int order=(e=getenv("PG_ORDER"))?atoi(e):0, threads=(e=getenv("PG_THREADS"))?atoi(e):1;
    vsdlss_set_num_threads(threads);
    if((e=getenv("PG_BLOCK"))) vsdlss_reduce_block=atol(e);   /* smaller block: blocks and a tail (fusion) on small cases */
    if((e=getenv("PG_TAILMIN"))) vsdlss_reduce_tail_min=atol(e);  /* smallest tail given a window schedule (default 4096) */
    const int fuse_sel=(e=getenv("PG_FUSE"))?atoi(e):1;          /* "tr+fuse" mode: 1 automatic, 2 always */
    double t0=now();
    elist R={0},C={0},L={0},V={0}; isrc *I=NULL; csi nI=0,capI=0;
    double h=0,tstop=0; char **pnames=NULL; csi npn=0;
    FILE *f=zopen(argv[1]); static char line[1<<16]; char t1[1024],t2[1024],t3[1024],t4[1024];
    while(fgets(line,sizeof line,f)){
        char c=(char)toupper((unsigned char)line[0]);
        if(c=='.'){
            if(!strncasecmp(line,".tran",5)) sscanf(line+5,"%lf %lf",&h,&tstop);
            else if(!strncasecmp(line,".print",6)){
                for(char *s=line;(s=strstr(s,"v("));){ s+=2; char *q=strchr(s,')'); if(!q)break;
                    pnames=realloc(pnames,(size_t)(npn+1)*sizeof(char*)); pnames[npn]=strndup(s,(size_t)(q-s)); npn++; s=q; }
            }
            continue;
        }
        if(c!='R'&&c!='C'&&c!='L'&&c!='I'&&c!='V')continue;
        if(sscanf(line,"%1023s %1023s %1023s %1023s",t1,t2,t3,t4)!=4)continue;
        csi a=lookup(t2,1),b=lookup(t3,1); double v=atof(t4);
        if(c=='R')push(&R,a,b,v); else if(c=='C')push(&C,a,b,v); else if(c=='L')push(&L,a,b,v); else if(c=='V')push(&V,a,b,v);
        else {
            if(nI==capI){capI=capI?2*capI:(1<<16);I=realloc(I,(size_t)capI*sizeof(isrc));if(!I)die("oom");}
            isrc s={a,b,v,0,{0}};
            char *pp=strcasestr(line,"pulse(");
            if(pp){ pp+=6; for(char *q=pp;*q&&*q!=')';q++) if(*q==',') *q=' ';
                if(sscanf(pp,"%lf %lf %lf %lf %lf %lf %lf",&s.p[0],&s.p[1],&s.p[2],&s.p[3],&s.p[4],&s.p[5],&s.p[6])==7) s.pulse=1; }
            I[nI++]=s;
        }
    }
    pclose(f);
    if(!(h>0)||!(tstop>0))die("no .tran h tstop");
    const csi N=nnames;
    csi steps=(csi)llround(tstop/h);
    if((e=getenv("PG_STEPS"))) { csi s=atol(e); if(s>0&&s<steps) steps=s; }
    printf("# netlist: names %lld, R %lld, C %lld, L %lld, I %lld (pulse), V %lld; .tran h=%g tstop=%g -> %lld steps; %lld printed nodes; parse %.2f s\n",
           (long long)N,(long long)R.m,(long long)C.m,(long long)L.m,(long long)nI,(long long)V.m,h,tstop,(long long)steps,(long long)npn,now()-t0);

    /* transient topology: vias / zero resistors merged; pads fixed */
    csi *uf=malloc((size_t)N*sizeof(csi)); char *fixed=calloc((size_t)N,1); double *fixv=calloc((size_t)N,8);
    for(csi i=0;i<N;i++)uf[i]=i;
    for(csi k=0;k<V.m;k++){elem x=V.e[k]; if(x.a>=0&&x.b>=0){ if(x.v!=0)die("non-zero V between two nodes"); unite(uf,x.a,x.b);} }
    for(csi k=0;k<R.m;k++){elem x=R.e[k]; if(x.v==0&&x.a>=0&&x.b>=0) unite(uf,x.a,x.b); }
    for(csi k=0;k<V.m;k++){elem x=V.e[k]; if(x.a>=0&&x.b>=0)continue;
        csi nd=x.a>=0?x.a:x.b; double v=x.a>=0?x.v:-x.v; csi r=find(uf,nd);
        if(fixed[r]&&fixv[r]!=v) { die("conflicting pad voltages"); } fixed[r]=1; fixv[r]=v; }
    csi *idx=malloc((size_t)N*sizeof(csi)); csi n=0;
    for(csi i=0;i<N;i++)idx[i]=-1;
    for(csi i=0;i<N;i++){csi r=find(uf,i); if(!fixed[r]&&idx[r]<0) idx[r]=n++;}
    #define U(i) ((i)<0?-1:(fixed[find(uf,(i))]?-1:idx[find(uf,(i))]))
    #define VF(i) ((i)<0?0.0:fixv[find(uf,(i))])

    /* DC topology: inductors shorted as well */
    csi *ud=malloc((size_t)N*sizeof(csi)); char *fixd=calloc((size_t)N,1); double *fvd=calloc((size_t)N,8);
    for(csi i=0;i<N;i++)ud[i]=find(uf,i);
    for(csi k=0;k<L.m;k++){elem x=L.e[k]; if(x.a>=0&&x.b>=0) unite(ud,x.a,x.b); else { /* to ground: shorted to 0 */
        csi nd=x.a>=0?x.a:x.b; csi r=find(ud,nd); fixd[r]=1; fvd[r]=0; } }
    for(csi i=0;i<N;i++) if(fixed[find(uf,i)]) { csi r=find(ud,i); if(fixd[r]&&fvd[r]!=fixv[find(uf,i)]) die("conflicting DC pad voltages"); fixd[r]=1; fvd[r]=fixv[find(uf,i)]; }
    csi *idd=malloc((size_t)N*sizeof(csi)); csi nd=0;
    for(csi i=0;i<N;i++)idd[i]=-1;
    for(csi i=0;i<N;i++){csi r=find(ud,i); if(!fixd[r]&&idd[r]<0) idd[r]=nd++;}
    #define UD(i) ((i)<0?-1:(fixd[find(ud,(i))]?-1:idd[find(ud,(i))]))
    #define VD(i) ((i)<0?0.0:fvd[find(ud,(i))])

    /* ---- DC system: R only (C open, L shorted), sources at t = 0 ---- */
    trip TD={0}; double *bd=calloc((size_t)(nd?nd:1),8);
    for(csi k=0;k<R.m;k++){elem x=R.e[k]; if(x.v==0)continue; double g=1/x.v; csi a=UD(x.a),b=UD(x.b);
        if(a>=0&&b>=0){ if(a==b)continue; tpush(&TD,a,a,g); tpush(&TD,b,b,g); tpush(&TD,a,b,-g); }
        else if(a>=0){ tpush(&TD,a,a,g); bd[a]+=g*VD(x.b); } else if(b>=0){ tpush(&TD,b,b,g); bd[b]+=g*VD(x.a); } }
    for(csi k=0;k<nI;k++){ double v=pulse_at(I+k,0); csi a=UD(I[k].a),b=UD(I[k].b); if(a>=0)bd[a]-=v; if(b>=0)bd[b]+=v; }
    vsdlss *Ad=csc(nd,TD.m,TD.a,TD.b,TD.x);
    vsdlss_m3_factor *Fd=NULL; double *xd=malloc((size_t)(nd?nd:1)*8);
    if(vsdlss_factorize_m3(Ad,order,&Fd)!=VSDLSS_OK)die("DC factorization failed (floating nodes?)");
    if(vsdlss_m3_solve(Fd,bd,xd)!=VSDLSS_OK)die("DC solve failed");
    vsdlss_m3_factor_free(Fd);
    #define VDC(i) ((i)<0?0.0:(UD(i)>=0?xd[UD(i)]:VD(i)))

    /* ---- transient system ---- */
    trip TA={0}, TC={0};
    double *cst=calloc((size_t)(n?n:1),8);      /* constant source terms */
    for(csi k=0;k<R.m;k++){elem x=R.e[k]; if(x.v==0)continue; double g=1/x.v; csi a=U(x.a),b=U(x.b);
        if(a>=0&&b>=0){ if(a==b)continue; tpush(&TA,a,a,g); tpush(&TA,b,b,g); tpush(&TA,a,b,-g); }
        else if(a>=0){ tpush(&TA,a,a,g); cst[a]+=g*VF(x.b); } else if(b>=0){ tpush(&TA,b,b,g); cst[b]+=g*VF(x.a); } }
    for(csi k=0;k<C.m;k++){elem x=C.e[k]; double g=x.v/h; csi a=U(x.a),b=U(x.b);
        if(a>=0&&b>=0){ if(a==b)continue; tpush(&TA,a,a,g); tpush(&TA,b,b,g); tpush(&TA,a,b,-g); tpush(&TC,a,a,g); tpush(&TC,b,b,g); tpush(&TC,a,b,-g); }
        else if(a>=0){ tpush(&TA,a,a,g); tpush(&TC,a,a,g); } else if(b>=0){ tpush(&TA,b,b,g); tpush(&TC,b,b,g); } }
    trp P; memset(&P,0,sizeof(P)); P.n=n; P.nind=L.m;
    P.lx=malloc((size_t)(L.m?L.m:1)*sizeof(csi)); P.ly=malloc((size_t)(L.m?L.m:1)*sizeof(csi));
    P.lvx=malloc((size_t)(L.m?L.m:1)*8); P.lvy=malloc((size_t)(L.m?L.m:1)*8); P.lg=malloc((size_t)(L.m?L.m:1)*8); P.il0=calloc((size_t)(L.m?L.m:1),8);
    for(csi q=0;q<L.m;q++){elem x=L.e[q]; double g=h/x.v; csi a=U(x.a),b=U(x.b);
        P.lx[q]=a; P.ly[q]=b; P.lvx[q]=VF(x.a); P.lvy[q]=VF(x.b); P.lg[q]=g;
        /* i_L (a -> b) = (h/L)(v_a - v_b) + history: conductance in A */
        if(a>=0&&b>=0){ tpush(&TA,a,a,g); tpush(&TA,b,b,g); tpush(&TA,a,b,-g); }
        else if(a>=0){ tpush(&TA,a,a,g); cst[a]+=g*P.lvy[q]; } else if(b>=0){ tpush(&TA,b,b,g); cst[b]+=g*P.lvx[q]; } }
    /* initial inductor currents: KCL at the a-side group in DC */
    {
        double *leave=calloc((size_t)N,8); csi *cntL=calloc((size_t)N,sizeof(csi));
        for(csi k=0;k<R.m;k++){elem x=R.e[k]; if(x.v==0)continue; double g=1/x.v; double va=VDC(x.a), vb=VDC(x.b);
            csi ra=x.a>=0?find(uf,x.a):-1, rb=x.b>=0?find(uf,x.b):-1; if(ra==rb)continue;
            if(ra>=0) { leave[ra]+=g*(va-vb); } if(rb>=0) { leave[rb]+=g*(vb-va); } }
        for(csi k=0;k<nI;k++){ double v=pulse_at(I+k,0); csi ra=I[k].a>=0?find(uf,I[k].a):-1, rb=I[k].b>=0?find(uf,I[k].b):-1;
            if(ra>=0) { leave[ra]+=v; } if(rb>=0) { leave[rb]-=v; } }
        /* KCL at the free end: i_L (a -> b) leaves a, enters b */
        for(csi q=0;q<L.m;q++){ csi a=L.e[q].a, b=L.e[q].b;
            if(a>=0 && !fixed[find(uf,a)]) cntL[find(uf,a)]++; else if(b>=0 && !fixed[find(uf,b)]) cntL[find(uf,b)]++; }
        for(csi q=0;q<L.m;q++){ csi a=L.e[q].a, b=L.e[q].b;
            if(a>=0 && !fixed[find(uf,a)]) {
                if(cntL[find(uf,a)]!=1) die("several inductors at one node: unsupported");
                P.il0[q]=-leave[find(uf,a)];
            } else if(b>=0 && !fixed[find(uf,b)]) {
                if(cntL[find(uf,b)]!=1) die("several inductors at one node: unsupported");
                P.il0[q]=leave[find(uf,b)];
            } else die("inductor between fixed nodes: unsupported");
        }
        free(leave); free(cntL);
    }
    vsdlss *A=csc(n,TA.m,TA.a,TA.b,TA.x), *Ch=csc(n,TC.m,TC.a,TC.b,TC.x);
    /* source entries: constants, then current-source endpoints */
    csi ncst=0; for(csi i=0;i<n;i++) ncst+=cst[i]!=0;
    P.src=malloc((size_t)(ncst+2*nI+1)*sizeof(csi)); P.sconst=malloc((size_t)(ncst+2*nI+1)*8);
    P.sI=malloc((size_t)(ncst+2*nI+1)*sizeof(csi)); P.ssign=malloc((size_t)(ncst+2*nI+1)*8);
    for(csi i=0;i<n;i++) if(cst[i]!=0){ P.src[P.nsrc]=i; P.sconst[P.nsrc]=cst[i]; P.sI[P.nsrc]=-1; P.ssign[P.nsrc++]=0; }
    for(csi k=0;k<nI;k++){ csi a=U(I[k].a),b=U(I[k].b);
        if(a>=0){ P.src[P.nsrc]=a; P.sconst[P.nsrc]=0; P.sI[P.nsrc]=k; P.ssign[P.nsrc++]=-1; }
        if(b>=0){ P.src[P.nsrc]=b; P.sconst[P.nsrc]=0; P.sI[P.nsrc]=k; P.ssign[P.nsrc++]=+1; } }
    /* dep entries: inductor history currents */
    P.dep=malloc((size_t)(2*L.m+1)*sizeof(csi)); P.dind=malloc((size_t)(2*L.m+1)*sizeof(csi)); P.dsign=malloc((size_t)(2*L.m+1)*8);
    for(csi q=0;q<L.m;q++){
        if(P.lx[q]>=0){ P.dep[P.ndep]=P.lx[q]; P.dind[P.ndep]=q; P.dsign[P.ndep++]=-1; }
        if(P.ly[q]>=0){ P.dep[P.ndep]=P.ly[q]; P.dind[P.ndep]=q; P.dsign[P.ndep++]=+1; } }
    csi offd=0; for(csi j=0;j<n;j++) for(csi p=Ch->p[j];p<Ch->p[j+1];p++) offd+=Ch->i[p]!=j;
    printf("# transient system: unknowns %lld (DC %lld), nnz(A) upper %lld, Ch off-diagonal %lld, source entries %lld, dep entries %lld; build %.2f s\n",
           (long long)n,(long long)nd,(long long)A->p[n],(long long)offd,(long long)P.nsrc,(long long)P.ndep,now()-t0);
    /* sources of every step */
    double *sv=malloc((size_t)(steps*P.nsrc+1)*8);
    for(csi k=0;k<steps;k++){ const double t=(double)(k+1)*h;
        for(csi j=0;j<P.nsrc;j++) sv[k*P.nsrc+j]=P.sI[j]<0?P.sconst[j]:P.ssign[j]*pulse_at(I+P.sI[j],t); }
    /* x_0 */
    double *x0=malloc((size_t)(n?n:1)*8);
    for(csi i=0;i<N;i++){ csi u=U(i); if(u>=0) x0[u]=VDC(i); }
    /* outputs */
    csi *out_u=malloc((size_t)(npn+1)*sizeof(csi)); double *out_fix=malloc((size_t)(npn+1)*8);
    for(csi j=0;j<npn;j++){ csi id=lookup(pnames[j],0); if(id<0) die("printed node not in the netlist");
        out_u[j]=U(id); out_fix[j]=VF(id); }

    t0=now();
    vsdlss_m3_factor *F=NULL;
    if(vsdlss_factorize_m3(A,order,&F)!=VSDLSS_OK) die("transient factorization failed");
    printf("# factorize (order %d, %d threads): %.3f s\n",order,threads,now()-t0);
    csi *slot=malloc((size_t)(n?n:1)*sizeof(csi)); vsdlss_m3_export_packed_slots(F,slot,n);

    /* ---- three ways ---- */
    double *rec[3], tm[3]={1e300,1e300,1e300}; const char *name[3]={"manual","tr","tr+fuse"};
    /* PG_ROUNDS rounds, the order of the three rotated every round (this
     * laptop's timings drift by 15% and more between consecutive runs of the
     * same code); best time per way, and every round must repeat its bits */
    const int rounds=(e=getenv("PG_ROUNDS"))&&atoi(e)>0?atoi(e):3; int stable[3]={1,1,1};
    double *tmp=calloc((size_t)((steps+1)*(npn?npn:1)),8);
    for(int m=0;m<3;m++) rec[m]=calloc((size_t)((steps+1)*(npn?npn:1)),8);
    const csi batch=(e=getenv("PG_BATCH"))&&atol(e)>0?atol(e):steps;
    for(int r=0;r<rounds;r++) for(int i=0;i<3;i++){ const int m=(i+r)%3;
        cbctx cx={&P,slot,malloc((size_t)(L.m?L.m:1)*8),npn,out_u,out_fix,r?tmp:rec[m],steps};
        memcpy(cx.il,P.il0,(size_t)L.m*8);
        if(m==0){
            /* the interface's b formula by hand, one packed solve per step */
            double *x=malloc((size_t)n*8), *w=malloc((size_t)n*8), *s=calloc((size_t)n,8), *d=calloc((size_t)n,8), *dv=malloc((size_t)(P.ndep+1)*8);
            csi *pc=calloc((size_t)n+1,sizeof(csi));
            for(csi j=0;j<n;j++) for(csi p=Ch->p[j];p<Ch->p[j+1];p++){ if(Ch->i[p]==j){ d[slot[j]]+=Ch->x[p]; continue; } pc[slot[Ch->i[p]]+1]++; pc[slot[j]+1]++; }
            for(csi p=0;p<n;p++) pc[p+1]+=pc[p];
            csi *pu=malloc((size_t)(pc[n]+1)*sizeof(csi)), *fl=calloc((size_t)n,sizeof(csi)); double *pv=malloc((size_t)(pc[n]+1)*8);
            for(csi j=0;j<n;j++) for(csi p=Ch->p[j];p<Ch->p[j+1];p++) if(Ch->i[p]!=j){ csi a=slot[Ch->i[p]], b=slot[j];
                pu[pc[a]+fl[a]]=b; pv[pc[a]+fl[a]++]=Ch->x[p]; pu[pc[b]+fl[b]]=a; pv[pc[b]+fl[b]++]=Ch->x[p]; }
            for(csi p=0;p<n;p++) for(csi a=pc[p]+1;a<pc[p+1];a++){ csi u=pu[a]; double v=pv[a]; csi t=a;
                while(t>pc[p]&&pu[t-1]>u){pu[t]=pu[t-1];pv[t]=pv[t-1];t--;} pu[t]=u; pv[t]=v; }
            for(csi i=0;i<n;i++) x[slot[i]]=x0[i];
            t0=now();
            on_step(&cx,0,x,dv);
            for(csi k=0;k<steps;k++){
                for(csi j=0;j<P.nsrc;j++) s[slot[P.src[j]]]=0.0;
                for(csi j=0;j<P.ndep;j++) s[slot[P.dep[j]]]=0.0;
                for(csi j=0;j<P.nsrc;j++) s[slot[P.src[j]]]+=sv[k*P.nsrc+j];
                for(csi j=0;j<P.ndep;j++) s[slot[P.dep[j]]]+=dv[j];
                for(csi p=0;p<n;p++){ double v=d[p]*x[p]; for(csi a=pc[p];a<pc[p+1];a++) v+=pv[a]*x[pu[a]]; w[p]=v+s[p]; }
                if(vsdlss_m3_solve_packed_inplace(F,w)!=VSDLSS_OK) die("packed solve failed");
                double *t=x; x=w; w=t;
                on_step(&cx,k+1,x,dv);
            }
            { double tt=now()-t0; if(tt<tm[m]) tm[m]=tt; }
            free(x);free(w);free(s);free(d);free(dv);free(pc);free(pu);free(fl);free(pv);
        } else {
            vsdlss_m3_transient_fuse=m==2?fuse_sel:0;
            vsdlss_m3_transient *T=NULL;
            if(vsdlss_m3_transient_create(F,Ch,P.nsrc,P.src,P.ndep,P.dep,&T)!=VSDLSS_OK) die("transient create failed");
            csi fc,u,l; vsdlss_m3_transient_info(T,&fc,&u,&l);
            printf("# %-8s fused components %lld, tail chunks %lld, late units %lld\n",name[m],(long long)fc,(long long)u,(long long)l);
            vsdlss_m3_transient_start(T,x0);
            t0=now();
            for(csi done=0;done<steps;){ csi s=steps-done<batch?steps-done:batch;
                if(vsdlss_m3_transient_run(T,s,sv+done*P.nsrc,on_step,&cx)!=VSDLSS_OK) die("transient run failed");
                done+=s; }
            { double tt=now()-t0; if(tt<tm[m]) tm[m]=tt; }
            vsdlss_m3_transient_free(T);
        }
        free(cx.il);
        if(r && memcmp(tmp,rec[m],(size_t)((steps+1)*npn)*8)) stable[m]=0;
    }
    for(int m=0;m<3;m++)
        printf("%-8s %8.3f ms per step (best of %d rounds)%s%s\n",name[m],1e3*tm[m]/(double)steps,rounds,
               m?(memcmp(rec[m],rec[0],(size_t)((steps+1)*npn)*8)?"  DIFFERS from manual":"  bitwise = manual"):"",
               stable[m]?"":"  NOT REPEATABLE across rounds");

    /* ---- published waveforms ---- */
    f=zopen(argv[2]); csi cur=-1, cmp=0, cmp0=0; double maxd=0, maxd0=0, vmin=1e300, vmax=-1e300; csi worst=-1; double worst_t=0;
    while(fgets(line,sizeof line,f)){
        char nm[1024];
        if(sscanf(line," Node: %1023s",nm)==1 || sscanf(line," NODE: %1023s",nm)==1){ cur=-1; for(csi j=0;j<npn;j++) if(!strcmp(pnames[j],nm)) cur=j; continue; }
        double t,v; if(cur<0 || sscanf(line,"%lf %lf",&t,&v)!=2) continue;
        const double ts=t>10*tstop?t*1e-9:t;     /* seconds (ibmpg1t), or ns as the page says */
        csi k=(csi)llround(ts/h); if(k<0||k>steps||fabs((double)k*h-ts)>1e-3*h) continue;
        double dlt=fabs(rec[2][k*npn+cur]-v);
        if(v<vmin) { vmin=v; } if(v>vmax) { vmax=v; }
        if(k==0){ if(dlt>maxd0)maxd0=dlt; cmp0++; }
        if(dlt>maxd){ maxd=dlt; worst=cur; worst_t=t; }
        cmp++;
    }
    pclose(f);
    printf("published waveforms: %lld points compared (%lld at t=0); max |difference| %.4g mV at t=0 (DC), %.4g mV overall (node %s, t=%g); printed voltages span %.4g .. %.4g V\n",
           (long long)cmp,(long long)cmp0,1e3*maxd0,1e3*maxd,worst>=0?pnames[worst]:"-",worst_t,vmin,vmax);
    vsdlss_m3_factor_free(F);
    return 0;
}
