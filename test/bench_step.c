/* bench_step: per-step time ledger of a transient EMIR loop (see bottom). Generator from bench_transient.c.
 *   A = G + C/h (diagonal grounded C), factored once; every step
 *   b_k = i_k + (C/h) v_{k-1} is assembled in packed order and solved with
 *   vsdlss_m3_solve_packed, and in original order with vsdlss_m3_solve for
 *   reference (bitwise comparison and backward error every TR_CHECK steps).
 *   Also reports the low-degree reduction tail and its dependency layers.
 *
 *   ./bench_transient [order] [threads] [nrhs(ignored)] [shuffle] [N d1..d6p]
 *   env: PG_NETS=2, TR_STEPS=200, TR_H=1e-11, TR_CFRAC=1, TR_IFRAC=0.3, TR_CHECK=50,
 *        TR_ALIAS=1 (assemble b_k in the solution vector and solve in place)
 */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include "../src/vsdlss_parallel.h"
#include "../src/vsdlss_ledger.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/resource.h>

/* Allocation counter (build with -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc
 * and -DCOUNT_ALLOC): calls and bytes while `counting` is set. */
#ifdef COUNT_ALLOC
void *__real_malloc(size_t); void *__real_calloc(size_t,size_t); void *__real_realloc(void*,size_t);
static volatile int counting; static long long nalloc; static double balloc;
void *__wrap_malloc(size_t z){ if(counting){nalloc++;balloc+=z;} return __real_malloc(z); }
void *__wrap_calloc(size_t a,size_t b){ if(counting){nalloc++;balloc+=(double)a*b;} return __real_calloc(a,b); }
void *__wrap_realloc(void *p,size_t z){ if(counting){nalloc++;balloc+=z;} return __real_realloc(p,z); }
#define COUNT(on) (counting=(on))
#else
static int counting; static long long nalloc; static double balloc;
#define COUNT(on) ((void)(on))
#endif
extern int vsdlss_solve_kv;
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static int cmpd(const void *a,const void *b){double x=*(const double*)a,y=*(const double*)b;return (x>y)-(x<y);}
static uint64_t rng=0x9E3779B97F4A7C15ULL;
static uint64_t rnd(void){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static double uni(double a,double b){return a+(b-a)*(double)(rnd()>>11)*(1.0/9007199254740992.0);}
static long peak_rss_mb(void)
{
    FILE *f=fopen("/proc/self/status","r"); char line[256]; long kb=-1;
    if(!f)return -1;
    while(fgets(line,sizeof line,f)) if(!strncmp(line,"VmHWM:",6)){kb=atol(line+6);break;}
    fclose(f); return kb/1024;
}

typedef struct { csi *a,*b; double *g; csi m,cap; } edges;
static void add(edges *E,csi a,csi b,double g)
{
    if(E->m>=E->cap){fprintf(stderr,"edge capacity exceeded\n");exit(1);}
    E->a[E->m]=a;E->b[E->m]=b;E->g[E->m]=g;E->m++;
}

/* One net (one connected grid).  sdeg: degrees of the D6 supply nodes; dbl:
 * whether one wire node carries two taps (degree 4), used for parity. */
typedef struct { csi N,D1,D2,D3,D4,D5,D6; int dbl; csi sdeg[3]; csi R,C; } spec_t;

static csi pads_of(const spec_t *s){csi p=0;for(csi k=0;k<s->D6;k++)p+=s->sdeg[k];return p;}

/* Appends the edges of one net with node ids base..base+N-1 and sets their
 * ground conductances.  Returns 0 on success. */
static int gen_net(spec_t *s,csi base,edges *E,double *ground)
{
    const csi D1=s->D1,D2=s->D2,D3=s->D3,D4=s->D4,D5=s->D5,N=s->N;
    const csi pads=pads_of(s),dbl=s->dbl;
    if(s->D6<1||s->D6>3||D5<0||D4<pads+2*D5+dbl){fprintf(stderr,"unsupported degree profile\n");return 2;}
    /* Lattice: junctions of degree 3 must satisfy
     *   deg3 = (I - (D4-dbl) - D5) + (D1-2*dbl) single-tap hosts. */
    csi I_target=D3-D1+D4+D5+dbl;
    /* R even and C odd keep every boundary junction at degree >= 2.  Choose
     * the smallest such lattice with an even surplus X of degree-3 junctions;
     * X/2 interior vertical edges are then removed (each turns two degree-3
     * junctions into degree 2), so exactly I_target junctions have degree 3. */
    csi R=0,C=0,X=-1;
    for(csi r0=(csi)sqrt((double)I_target)-40;r0<(csi)sqrt((double)I_target)+40;r0++){
        csi rr=r0+(r0&1), cc;
        if(rr<4)continue;
        for(cc=(I_target/rr)|1;;cc+=2){
            csi d3=0;
            for(csi r=0;r<rr;r++)for(csi c=0;c<cc;c++){
                int d=(c>0)+(c+1<cc)+(r+1<rr&&((r+c)%2==0))+(r>0&&((r-1+c)%2==0));
                d3+=d==3;
            }
            if(d3>=I_target){ if(((d3-I_target)&1)==0&&(X<0||d3-I_target<X)){X=d3-I_target;R=rr;C=cc;} break; }
        }
    }
    if(X<0){fprintf(stderr,"no lattice found\n");return 2;}
    s->R=R;s->C=C;
    csi J=R*C;
    unsigned char *cut=calloc((size_t)J,1);      /* vertical edge below v removed */
    if(!cut){puts("alloc failed");return 1;}
    /* Positions are visited as k*P mod J with P prime and coprime to J, so
     * every junction is seen once and choices spread over the lattice. */
    csi P=1000003; while(J%P==0)P+=2;
    csi visit=0;
#define NEXT_POS() ((csi)(((unsigned long long)(visit++)*(unsigned long long)P)%(unsigned long long)J))
    {
        csi want=X/2;
        while(want>0){
            if(visit>J){fprintf(stderr,"cannot place cuts\n");return 2;}
            csi v=NEXT_POS(),r=v/C,c=v%C;
            if(r>=2&&r+3<R&&c>=2&&c+3<C&&((r+c)%2==0)&&!cut[v]&&!cut[v-C]&&!cut[v+C]){cut[v]=1;want--;}
        }
    }
#define VERT(r,c) ((r)+1<R&&(((r)+(c))%2==0)&&!cut[(r)*C+(c)])
    csi El=0;
    for(csi r=0;r<R;r++)for(csi c=0;c<C;c++){ if(c+1<C)El++; if(VERT(r,c))El++; }
    csi *jdeg=calloc((size_t)J,sizeof(csi));
    if(!jdeg){puts("alloc failed");return 1;}
    for(csi r=0;r<R;r++)for(csi c=0;c<C;c++){
        csi v=r*C+c; if(c+1<C){jdeg[v]++;jdeg[v+1]++;} if(VERT(r,c)){jdeg[v]++;jdeg[v+C]++;}
    }
    csi lat[4]={0};
    for(csi v=0;v<J;v++)lat[jdeg[v]<3?jdeg[v]:3]++;
    /* Straps; pads, 2*D5 strap partners and the double-tap host are degree 4. */
    csi S=(D4-pads-2*D5-dbl)/2;
    if(2*S+pads+2*D5+dbl!=D4){fprintf(stderr,"d4 parity unsupported\n");return 2;}
    /* Wire nodes: d2 = plain wire nodes + degree-2 junctions; hosts = D1-dbl. */
    csi supply=s->D6, H=D1-dbl, T=D2-lat[2]+H;
    csi n=J+T+D1+supply;
    if(n!=N) {
        /* Absorb the lattice rounding in the wire count so N is exact. */
        csi diff=N-n; T+=diff; n=N;
    }
    csi ebase=T/El, rem=T%El;

    /* Wires with taps: wire node t (0..T-1) hosts a tap iff the running share
     * of H over T steps up, which spreads the taps evenly. */
    csi wire=J, tap=J+T, t=0, e=0, taps=0;
    for(csi r=0;r<R;r++)for(csi c=0;c<C;c++)for(int dir=0;dir<2;dir++){
        csi v=r*C+c,w;
        if(dir==0){ if(c+1>=C)continue; w=v+1; }
        else { if(!VERT(r,c))continue; w=v+C; }
        csi len=ebase+(e<rem); e++;
        csi prev=v;
        for(csi k=0;k<len;k++){
            csi node=wire++;
            add(E,base+prev,base+node,uni(0.5,2.0));
            if((csi)((double)(t+1)*H/T)>(csi)((double)t*H/T)){
                int two=dbl&&taps==0;   /* the first host may carry two taps */
                for(int q=0;q<1+two;q++){add(E,base+node,base+tap,uni(0.05,0.5));ground[base+tap]=1e-3;tap++;taps++;}
            }
            t++; prev=node;
        }
        add(E,base+prev,base+w,uni(0.5,2.0));
    }
    /* Extras on interior degree-3 junctions, spread with a stride. */
    unsigned char *used=calloc((size_t)J,1);
    if(!used){puts("alloc failed");return 1;}
    visit=0;
#define NEXT_FREE(v) do{ for(;;){ if(visit>J){fprintf(stderr,"cannot place straps\n");return 2;} \
        v=NEXT_POS(); if(jdeg[v]==3&&!used[v]&&v%C+2<C&&jdeg[v+2]==3&&!used[v+2]) break; } }while(0)
    for(csi q=0;q<S;q++){csi v;NEXT_FREE(v);used[v]=used[v+2]=1;add(E,base+v,base+v+2,1.0);}
    for(csi q=0;q<D5;q++){ /* junction with two straps: partners on both sides */
        csi v;
        for(;;){NEXT_FREE(v); if(v%C>=2&&jdeg[v-2]==3&&!used[v-2])break;}
        used[v]=used[v+2]=used[v-2]=1; add(E,base+v,base+v+2,1.0); add(E,base+v,base+v-2,1.0);
    }
    for(csi q=0;q<supply;q++){
        csi sn=J+T+D1+q; ground[base+sn]=100.0;
        for(csi k=0;k<s->sdeg[q];k++){
            if(visit>J){fprintf(stderr,"cannot place pads\n");return 2;}
            csi v=NEXT_POS();
            if(jdeg[v]!=3||used[v]){k--;continue;} used[v]=1; add(E,base+sn,base+v,5.0);}
    }
#undef NEXT_FREE
#undef VERT
#undef NEXT_POS
    free(used); free(jdeg); free(cut);
    if(wire!=J+T||tap!=J+T+D1){fprintf(stderr,"generator bookkeeping failed %lld %lld\n",(long long)(wire-J-T),(long long)(tap-J-T-D1));return 1;}
    return 0;
}

int main(int argc,char **argv)
{
    int order=argc>1?atoi(argv[1]):5, threads=argc>2?atoi(argv[2]):1;
    int nrhs=argc>3?atoi(argv[3]):1, shuffle=argc>4?atoi(argv[4]):1;
    (void)nrhs;   /* argument kept compatible with bench_pg_profile; one RHS here */
    csi N=22875397,D1=2561262,D2=15270759,D3=5000468,D4=42895,D5=10,D6=3;
    if(argc>11){N=atoll(argv[5]);D1=atoll(argv[6]);D2=atoll(argv[7]);D3=atoll(argv[8]);
        D4=atoll(argv[9]);D5=atoll(argv[10]);D6=atoll(argv[11]);}
    /* PG_NETS=2: separate VDD and GND nets (two connected components), the
     * histogram split PG_SPLIT : 1-PG_SPLIT (default 0.52). */
    const char *env=getenv("PG_NETS"); int nets=env?atoi(env):1;
    env=getenv("PG_SPLIT"); double split=env?atof(env):0.52;
    if(D6!=3||(nets!=1&&nets!=2)||!(split>0.05&&split<0.95)){fprintf(stderr,"generator expects d6p=3, PG_NETS 1 or 2\n");return 2;}
    spec_t sp[2]; memset(sp,0,sizeof sp);
    if(nets==1){ sp[0]=(spec_t){N,D1,D2,D3,D4,D5,3,1,{8,8,10},0,0}; }
    else {
        /* VDD: 2 supply nodes (8, 8) and the double-tap host; GND: 1 supply
         * node (10).  D4 parity per net is fixed by pads + dbl, and D1+D3+D5
         * must be even per net (degree sum). */
        spec_t *a=sp,*b=sp+1;
        a->D1=(csi)llround(split*D1); a->D2=(csi)llround(split*D2); a->D3=(csi)llround(split*D3);
        a->D4=(csi)llround(split*D4); a->D5=D5/2; a->D6=2; a->dbl=1; a->sdeg[0]=8; a->sdeg[1]=8;
        if(!(a->D4&1))a->D4++;
        if((a->D1+a->D3+a->D5)&1)a->D3++;
        a->N=a->D1+a->D2+a->D3+a->D4+a->D5+a->D6;
        *b=(spec_t){N-a->N,D1-a->D1,D2-a->D2,D3-a->D3,D4-a->D4,D5-a->D5,1,0,{10,0,0},0,0};
        if((b->D4&1)||((b->D1+b->D3+b->D5)&1)){fprintf(stderr,"cannot split histogram with valid parity\n");return 2;}
    }
    double t0=now();
    csi n=N, degsum=0;
    for(int k=0;k<nets;k++)degsum+=sp[k].D1+2*sp[k].D2+3*sp[k].D3+4*sp[k].D4+5*sp[k].D5+pads_of(sp+k);
    csi mcap=degsum/2+64;
    edges E={malloc((size_t)mcap*sizeof(csi)),malloc((size_t)mcap*sizeof(csi)),malloc((size_t)mcap*sizeof(double)),0,mcap};
    double *ground=calloc((size_t)n,sizeof(double));
    if(!E.a||!E.b||!E.g||!ground){puts("alloc failed");return 1;}
    csi base=0;
    for(int k=0;k<nets;k++){ int rc=gen_net(sp+k,base,&E,ground); if(rc)return rc; base+=sp[k].N; }

    /* Histogram of the generated graph. */
    csi *deg=calloc((size_t)n,sizeof(csi)), hist[7]={0}, maxdeg=0;
    for(csi k=0;k<E.m;k++){deg[E.a[k]]++;deg[E.b[k]]++;}
    for(csi v=0;v<n;v++){if(deg[v]>maxdeg)maxdeg=deg[v];hist[deg[v]<6?deg[v]:6]++;}
    free(deg);

    /* ---- electrical values on the generated topology ----
     * PG_GRATIO=r: every edge conductance multiplied by a log-uniform factor
     * in [1/sqrt(r), sqrt(r)] (strong/weak edges; pattern unchanged).
     * TR_CCOUP=f: share of edges carrying a coupling capacitor 0.1-1 fF;
     * it adds Cc/h to that edge of A (same pattern) and a history term to b. */
    const char *ev;
    double gratio=(ev=getenv("PG_GRATIO"))?atof(ev):1.0;
    double ccfrac=(ev=getenv("TR_CCOUP"))?atof(ev):0.2;
    double h=(ev=getenv("TR_H"))?atof(ev):1e-11;
    double cfrac=(ev=getenv("TR_CFRAC"))?atof(ev):1.0;
    double ifrac=(ev=getenv("TR_IFRAC"))?atof(ev):0.3;
    if(gratio>1){ double lr=log(gratio); for(csi k=0;k<E.m;k++) E.g[k]*=exp(uni(-0.5*lr,0.5*lr)); }
    csi ncc=0; csi *cca=NULL,*ccb=NULL,*cce=NULL; double *ccv=NULL;
    {
        csi cap_=(csi)(ccfrac*E.m)+16; cca=malloc(cap_*sizeof(csi)); ccb=malloc(cap_*sizeof(csi)); ccv=malloc(cap_*sizeof(double));
        cce=malloc(cap_*sizeof(csi));
        for(csi k=0;k<E.m;k++) if(ncc<cap_ && uni(0,1)<ccfrac){ double c=uni(0.1e-15,1e-15); cca[ncc]=E.a[k]; ccb[ncc]=E.b[k]; ccv[ncc]=c/h; cce[ncc]=k; ncc++; E.g[k]+=c/h; }
    }
    /* Resistive branches for EM (conductance without the coupling part). */
    csi nbr=E.m; csi *bra=malloc(nbr*sizeof(csi)),*brb=malloc(nbr*sizeof(csi)); double *brg=malloc(nbr*sizeof(double));
    for(csi k=0;k<E.m;k++){ bra[k]=E.a[k]; brb[k]=E.b[k]; brg[k]=E.g[k]; }
    for(csi c=0;c<ncc;c++) brg[cce[c]]-=ccv[c];
    free(cce);

    /* Upper CSC with shuffled numbering (the two nets are interleaved). */
    csi *perm=malloc((size_t)n*sizeof(csi));
    if(!perm){puts("alloc failed");return 1;}
    for(csi i=0;i<n;i++)perm[i]=i;
    if(shuffle)for(csi i=n-1;i>0;i--){csi j=(csi)(rnd()%(uint64_t)(i+1)),x=perm[i];perm[i]=perm[j];perm[j]=x;}
    vsdlss *A=vsdlss_spalloc(n,n,n+E.m,1,0);
    double *diag=calloc((size_t)n,sizeof(double));
    if(!A||!diag){puts("alloc failed");return 1;}
    memset(A->p,0,(size_t)(n+1)*sizeof(csi));
    for(csi k=0;k<E.m;k++){csi a=perm[E.a[k]],b=perm[E.b[k]];A->p[(a>b?a:b)+1]++;diag[a]+=E.g[k];diag[b]+=E.g[k];}
    for(csi i=0;i<n;i++){diag[perm[i]]+=ground[i];A->p[i+1]++;}
    free(ground);
    for(csi j=0;j<n;j++)A->p[j+1]+=A->p[j];
    csi *cur=malloc((size_t)n*sizeof(csi));
    for(csi j=0;j<n;j++)cur[j]=A->p[j];
    for(csi k=0;k<E.m;k++){csi a=perm[E.a[k]],b=perm[E.b[k]],col=a>b?a:b;A->i[cur[col]]=a<b?a:b;A->x[cur[col]++]=-E.g[k];}
    for(csi j=0;j<n;j++){A->i[cur[j]]=j;A->x[cur[j]++]=diag[j];}
    free(cur);free(diag);free(E.a);free(E.b);free(E.g);
    /* Every node/branch list below is in the caller's (possibly shuffled)
     * numbering, as a netlist reader would deliver it. */
    for(csi k=0;k<nbr;k++){bra[k]=perm[bra[k]];brb[k]=perm[brb[k]];}
    for(csi k=0;k<ncc;k++){cca[k]=perm[cca[k]];ccb[k]=perm[ccb[k]];}
    free(perm);
    printf("# n=%lld branches=%lld coupling_caps=%lld gratio=%g shuffle=%d gen=%.1fs rss=%ld MB\n",(long long)n,(long long)nbr,(long long)ncc,gratio,shuffle,now()-t0,peak_rss_mb());

    /* Sources: one per loaded node, listed in node order. */
    double *cap=malloc(n*8); csi ns=0; csi *src=malloc(n*sizeof(csi)); double *samp=malloc(n*8); unsigned char *sgrp=malloc(n);
    for(csi i=0;i<n;i++){ cap[i]=uni(0,1)<cfrac?uni(0.5e-15,5e-15):0.0; if(uni(0,1)<ifrac){src[ns]=i;samp[ns]=uni(1e-6,1e-4);sgrp[ns]=(unsigned char)(rnd()%8);ns++;} }
    for(csi j=0;j<n;j++){ csi p=A->p[j+1]-1; A->x[p]+=cap[j]/h; }
    double *hc=malloc(n*8); for(csi i=0;i<n;i++) hc[i]=cap[i]/h;

    if(vsdlss_set_num_threads(threads)!=VSDLSS_OK){puts("bad thread count");return 1;}
    long rss0=peak_rss_mb();
    vsdlss_m3_factor *f=NULL; double tt=now();
    vsdlss_status st=vsdlss_factorize_m3(A,order,&f);
    double tfac=now()-tt;
    if(st!=VSDLSS_OK){printf("factor failed: %s\n",vsdlss_status_string(st));return 1;}
    double lbytes=0,ridx=0,redb=0; csi lnz=0,core=0,maxw=0,npan=0;
    for(csi k=0;k<f->count;k++){ const vsdlss_m3_component_factor *cf=f->component+k; const vsdlss_reduction *r=cf->reduction;
        core+=r->core_n;
        for(csi q=0;q<r->pk_count;q++) redb+=(r->pk[q].head?4.0:1.0)*r->pk[q].count+4.0*r->pk[q].nbn+8.0*(r->pk[q].count+r->pk[q].nbn);
        if(cf->numeric){ const vsdlss_sn_factor *s=cf->numeric; lnz+=s->l_nnz; lbytes+=8.0*s->panel_offset[s->count]; ridx+=8.0*s->row_ptr[s->count]; npan+=s->count;
            for(csi q=0;q<s->count;q++){csi w=s->column_start[q+1]-s->column_start[q]; if(w>maxw)maxw=w;} } }
    if(getenv("TR_WE")){ /* (w,e) histogram of core panels, weighted by bytes */
        double hb[8][8]={{0}}; long long hn[8][8]={{0}}; const int eb[8]={0,4,8,16,32,64,128,1<<30};
        for(csi k=0;k<f->count;k++){ const vsdlss_sn_factor *s=f->component[k].numeric; if(!s) continue;
            for(csi q=0;q<s->count;q++){ csi w=s->column_start[q+1]-s->column_start[q], e=s->row_ptr[q+1]-s->row_ptr[q];
                int bw=vsdlss_ledger_bucket(w), be=0; while(be<6&&e>=eb[be+1]) be++; if(e==0) be=7;
                hn[bw][be]++; hb[bw][be]+=8.0*(w*(w+1)/2+w*e); } }
        long long dh[4]={0}, runs=0; int last=-1;
        for(csi k=0;k<f->count;k++){ const vsdlss_reduction *r=f->component[k].reduction;
            for(csi q=0;q<r->pk_count;q++) for(csi i=0;i<r->pk[q].count;i++){ int d=(int)vsdlss_pk_degree(r->pk+q,i); dh[d]++; if(d!=last) runs++; last=d; } }
        { long long nb_=0,maxb=0,tail=0,tot=0; for(csi k=0;k<f->count;k++){ const vsdlss_reduction *r=f->component[k].reduction; if(r->blocks>=1&&r->pk_count==r->blocks+1){ nb_+=r->blocks; for(csi b=0;b<r->blocks;b++){ if(r->pk[b].count>maxb)maxb=r->pk[b].count; } tail+=r->pk[r->blocks].count; } tot+=r->count; }
          printf("# replay blocks %lld (largest %lld records), sequential tail %lld of %lld records\n",nb_,maxb,tail,tot); }
        printf("# replay records by degree 0/1/2/3: %lld %lld %lld %lld, degree changes %lld\n",dh[0],dh[1],dh[2],dh[3],runs);
        printf("# (w,e) panels / MB of L: e buckets 1-3,4-7,8-15,16-31,32-63,64-127,>=128,e=0\n");
        for(int bw=0;bw<LG_NB;bw++){ printf("#  w-bucket %d:",bw); for(int be=0;be<8;be++) printf(" %lld/%.1f",hn[bw][be],hb[bw][be]/1e6); printf("\n"); }
    }
    printf("# setup: factor %.3f s order=%d threads=%d core_n=%lld (%.1f%%) panels=%lld max_width=%lld | resident: L values %.1f MB, L row idx %.1f MB, reduction %.1f MB | rss before factor %ld MB, peak %ld MB\n",
        tfac,order,threads,(long long)core,100.0*core/n,(long long)npan,(long long)maxw,lbytes/1e6,ridx/1e6,redb/1e6,rss0,peak_rss_mb());

    /* ---- packed chain: one-time mapping (part of setup) ---- */
    tt=now();
    csi *pperm=malloc(n*sizeof(csi)), *pinv=malloc(n*sizeof(csi));
    vsdlss_m3_export_packed_permutation(f,pperm,n);
    for(csi p=0;p<n;p++) pinv[pperm[p]]=p;
    double *hcp=malloc(n*8); for(csi p=0;p<n;p++) hcp[p]=hc[pperm[p]];
    /* sources, coupling caps and branches with packed endpoints, sorted by
     * packed index (counting sort) so each step walks them in memory order */
    csi *psrc=malloc(ns*sizeof(csi)); double *psamp=malloc(ns*8); unsigned char *psgrp=malloc(ns?ns:1);
    { csi *cnt=calloc(n+1,sizeof(csi)); for(csi s=0;s<ns;s++) cnt[pinv[src[s]]+1]++; for(csi p=0;p<n;p++) cnt[p+1]+=cnt[p];
      for(csi s=0;s<ns;s++){ csi q=cnt[pinv[src[s]]]++; psrc[q]=pinv[src[s]]; psamp[q]=samp[s]; psgrp[q]=sgrp[s]; } free(cnt); }
    csi *pcca=malloc(ncc*sizeof(csi)+8),*pccb=malloc(ncc*sizeof(csi)+8); double *pccv=malloc(ncc*8+8);
    /* coupling caps: endpoints mapped, list order kept (a node's history terms
     * are then added in the same order as in G, so all flows agree bitwise) */
    for(csi s=0;s<ncc;s++){ pcca[s]=pinv[cca[s]]; pccb[s]=pinv[ccb[s]]; pccv[s]=ccv[s]; }
    csi *pbra=malloc(nbr*sizeof(csi)),*pbrb=malloc(nbr*sizeof(csi)); double *pbrg=malloc(nbr*8); csi *pbrid=malloc(nbr*sizeof(csi));
    { csi *cnt=calloc(n+1,sizeof(csi)); for(csi s=0;s<nbr;s++){ csi a=pinv[bra[s]],b=pinv[brb[s]]; cnt[(a<b?a:b)+1]++; } for(csi p=0;p<n;p++) cnt[p+1]+=cnt[p];
      for(csi s=0;s<nbr;s++){ csi a=pinv[bra[s]],b=pinv[brb[s]],q=cnt[a<b?a:b]++; pbra[q]=a; pbrb[q]=b; pbrg[q]=brg[s]; pbrid[q]=s; } free(cnt); }
    double tmap=now()-tt;
    printf("# packed chain one-time mapping: %.3f s (sources %lld, coupling caps %lld, branches %lld)\n",tmap,(long long)ns,(long long)ncc,(long long)nbr);

    /* ---- transient loop: three flows with their own state ----
     *  G  caller order end to end: vsdlss_m3_solve (library gathers/writes back)
     *  N  packed solve, but the caller converts b and v globally every step
     *  P  packed end to end: sources/caps/branches mapped once (above)
     * Each step: RHS (sources + C history + coupling history), solve, EM
     * (branch current accumulation: mean and RMS per branch).  The flows
     * run in rotating order each step. */
    int steps=(ev=getenv("TR_STEPS"))?atoi(ev):100;
    int check=(ev=getenv("TR_CHECK"))?atoi(ev):25;
    const char *modes=(ev=getenv("TR_MODES"))?ev:"GNP";
    int nm=(int)strlen(modes);
    const int kkv=(ev=getenv("TR_KKV"))?atoi(ev):1, ikv=(ev=getenv("TR_IKV"))?atoi(ev):2;
    double *vG=calloc(n,8),*bG=malloc(n*8),*vN=calloc(n,8),*bN=malloc(n*8),*vNp=malloc(n*8),*bNp=malloc(n*8),*vP=calloc(n,8),*bP=malloc(n*8);
    double *emG1=calloc(nbr,8),*emG2=calloc(nbr,8),*emN1=calloc(nbr,8),*emN2=calloc(nbr,8),*emP1=calloc(nbr,8),*emP2=calloc(nbr,8);
    if(!vG||!bG||!vN||!bN||!vNp||!bNp||!vP||!bP||!emG1||!emG2||!emN1||!emN2||!emP1||!emP2){puts("alloc failed");return 1;}
    double *vK=strchr(modes,'K')?calloc(n,8):NULL; double *vIsave;
    double *vI=calloc(n,8),*dI=malloc(ncc*8+8),*emI1=calloc(nbr,8),*emI2=calloc(nbr,8);
    vIsave=vI;
    double *T[5][4]; for(int m=0;m<5;m++)for(int s=0;s<4;s++)T[m][s]=calloc(steps,8);
    vsdlss_ledger_t L[5]; memset(L,0,sizeof L);
    double wave[8]; int bit_ok=1; double worst=0;
    for(int k=0;k<steps;k++){
        double tk=(double)k;
        for(int q=0;q<8;q++){ double ph=fmod(tk+7.0*q,40.0); wave[q]=q<3?(ph<5?1.0:0.05):q<5?(tk>=20.0*(q-2)?1.0:0.1):(ph<20?ph/20:(40-ph)/20); }
        for(int r=0;r<nm;r++){
            char m=modes[(r+k)%nm]; int mi=m=='G'?0:m=='N'?1:m=='P'?2:m=='I'?3:4;
            /* K: flow I with the previous panel kernels (A/B in one process) */
            if(mi==4){ vsdlss_solve_kv=kkv; mi=3; } else vsdlss_solve_kv=ikv;
            double *vI_=vI; if(m=='K') vI=vK; else if(m=='I') vI=vI_;
            double t0,t1,t2,t3,t4;
            vsdlss_ledger_reset();
            if(mi==0||mi==1){
                double *v=mi?vN:vG,*b=mi?bN:bG,*e1=mi?emN1:emG1,*e2=mi?emN2:emG2;
                double tc=0,t5;
                t0=now();
                for(csi i=0;i<n;i++) b[i]=hc[i]*v[i];
                for(csi s=0;s<ns;s++) b[src[s]]+=samp[s]*wave[sgrp[s]];
                for(csi s=0;s<ncc;s++){ double d=ccv[s]*(v[cca[s]]-v[ccb[s]]); b[cca[s]]+=d; b[ccb[s]]-=d; }
                t1=now();
                if(mi==0){ COUNT(k>0); int rc_=(vsdlss_m3_solve(f,b,v)); COUNT(0); if(rc_!=VSDLSS_OK){puts("solve failed");return 1;} t2=t1; t3=now(); t4=t3; }
                else {
                    for(csi p=0;p<n;p++) bNp[p]=b[pperm[p]];
                    t2=now();
                    COUNT(k>0); int rc_=(vsdlss_m3_solve_packed(f,bNp,vNp)); COUNT(0); if(rc_!=VSDLSS_OK){puts("solve failed");return 1;}
                    t3=now();
                    for(csi p=0;p<n;p++) v[pperm[p]]=vNp[p];
                    t4=now();
                    tc=(t2-t1)+(t4-t3);
                }
                for(csi e=0;e<nbr;e++){ double c=brg[e]*(v[bra[e]]-v[brb[e]]); e1[e]+=c; e2[e]+=c*c; }
                t5=now();
                T[mi][0][k]=t1-t0; T[mi][1][k]=t3-t2; T[mi][2][k]=tc; T[mi][3][k]=t5-t4;
            } else if(mi==3) {
                /* I: packed chain with one vector: b_k built over v_{k-1}, solved in place */
                t0=now();
                for(csi s=0;s<ncc;s++) dI[s]=pccv[s]*(vI[pcca[s]]-vI[pccb[s]]);
                for(csi p=0;p<n;p++) vI[p]*=hcp[p];
                for(csi s=0;s<ns;s++) vI[psrc[s]]+=psamp[s]*wave[psgrp[s]];
                for(csi s=0;s<ncc;s++){ vI[pcca[s]]+=dI[s]; vI[pccb[s]]-=dI[s]; }
                t1=now();
                COUNT(k>0); int rc_=(vsdlss_m3_solve_packed_inplace(f,vI)); COUNT(0); if(rc_!=VSDLSS_OK){puts("solve failed");return 1;}
                t2=now();
                for(csi e=0;e<nbr;e++){ double c=pbrg[e]*(vI[pbra[e]]-vI[pbrb[e]]); emI1[e]+=c; emI2[e]+=c*c; }
                t3=now();
                { int ti=m=='K'?4:3; T[ti][0][k]=t1-t0; T[ti][1][k]=t2-t1; T[ti][2][k]=0; T[ti][3][k]=t3-t2; }
                vI=vI_; if(m=='K') vI=vIsave;
            } else {
                t0=now();
                for(csi p=0;p<n;p++) bP[p]=hcp[p]*vP[p];
                for(csi s=0;s<ns;s++) bP[psrc[s]]+=psamp[s]*wave[psgrp[s]];
                for(csi s=0;s<ncc;s++){ double d=pccv[s]*(vP[pcca[s]]-vP[pccb[s]]); bP[pcca[s]]+=d; bP[pccb[s]]-=d; }
                t1=now();
                COUNT(k>0); int rc_=(vsdlss_m3_solve_packed(f,bP,vP)); COUNT(0); if(rc_!=VSDLSS_OK){puts("solve failed");return 1;}
                t2=now();
                for(csi e=0;e<nbr;e++){ double c=pbrg[e]*(vP[pbra[e]]-vP[pbrb[e]]); emP1[e]+=c; emP2[e]+=c*c; }
                t3=now();
                T[2][0][k]=t1-t0; T[2][1][k]=t2-t1; T[2][2][k]=0; T[2][3][k]=t3-t2;
            }
            if(m=='K') mi=4;
            if(vsdlss_ledger_level()&&k>0){ for(int q=0;q<LG_N;q++) L[mi].wall[q]+=vsdlss_ledger.wall[q];
                for(int d=0;d<2;d++)for(int q=0;q<LG_NB;q++){L[mi].bucket_s[d][q]+=vsdlss_ledger.bucket_s[d][q];L[mi].bucket_n[d][q]+=vsdlss_ledger.bucket_n[d][q];L[mi].bucket_bytes[d][q]+=vsdlss_ledger.bucket_bytes[d][q];}
                L[mi].solves+=vsdlss_ledger.solves; }
        }
        if(check>0&&(k%check==0||k==steps-1)&&strchr(modes,'G')){
            if(strchr(modes,'P')) for(csi p=0;p<n;p++) if(memcmp(&vP[p],&vG[pperm[p]],8)){bit_ok=0;break;}
            if(strchr(modes,'N')) if(memcmp(vN,vG,n*8)) bit_ok=0;
            if(strchr(modes,'I')) for(csi p=0;p<n;p++) if(memcmp(&vI[p],&vG[pperm[p]],8)){bit_ok=0;break;}
            double eta; vsdlss_backward_error(A,vG,bG,&eta); if(eta>worst)worst=eta;
        }
        if(vK && check>0 && (k%check==0||k==steps-1)){
            if(kkv<3 && ikv<3) { if(memcmp(vK,vI,n*8)) bit_ok=0; }
            else { double md=0; for(csi p=0;p<n;p++){ double d=fabs(vK[p]-vI[p])/(fabs(vI[p])+1e-30); if(d>md)md=d; }
                printf("# step %d: max rel diff K vs I %.2e\n",k,md); }
        }
    }
    /* placeholder */
    /* EM consistency: branch mean current of P equals G (same branch). */
    if(strchr(modes,'G')&&strchr(modes,'P')) for(csi q=0;q<nbr;q++) if(memcmp(&emP1[q],&emG1[pbrid[q]],8)){bit_ok=0;break;}
    const char *nm_[5]={"G caller-order","N packed+caller conversion","P packed chain","I packed chain in-place","K = I with TR_KKV kernels"};
    const char *sn_[4]={"rhs","solve","convert","em"};
    (void)counting; printf("# per-warm-solve allocations: %.2f calls, %.0f bytes (all flows; needs COUNT_ALLOC build)\n",(double)nalloc/((steps-1)*nm),balloc/((steps-1)*nm));
    printf("# steps=%d (step 0 = cold, excluded from medians) bitwise G==N==P (v and EM) %d worst berr %.1e peak_rss %ld MB\n",steps,bit_ok,worst,peak_rss_mb());
    for(int mi=0;mi<5;mi++){
        char m="GNPIK"[mi]; if(!strchr(modes,m)) continue;
        double med[4],p95[4],tot=0,cold=0;
        for(int s=0;s<4;s++){ cold+=T[mi][s][0]; double *a=malloc(steps*8); memcpy(a,T[mi][s]+1,(steps-1)*8); qsort(a,steps-1,8,cmpd); med[s]=a[(steps-1)/2]; p95[s]=a[(int)(0.95*(steps-2))]; tot+=med[s]; free(a); }
        double *st_=malloc(steps*8); for(int k=1;k<steps;k++) st_[k-1]=T[mi][0][k]+T[mi][1][k]+T[mi][2][k]+T[mi][3][k]; qsort(st_,steps-1,8,cmpd);
        printf("STEP %-28s", nm_[mi]);
        for(int s=0;s<4;s++) printf(" %s %.4f (p95 %.4f)", sn_[s], med[s], p95[s]);
        printf(" | step med %.4f p95 %.4f | cold step %.4f\n", st_[(steps-1)/2], st_[(int)(0.95*(steps-2))], cold);
        free(st_);
        if(L[mi].solves){
            double S=(double)L[mi].solves; const char *ph[LG_N]={"setup","gather","red_fwd","core_gather","core_fwd","core_bwd","core_scatter","red_bwd","writeback","other","TOTAL"};
            printf("LEDGER %c (wall-clock, exclusive, mean ms over %lld warm solves):",m,(long long)L[mi].solves);
            for(int q=0;q<LG_N;q++) printf(" %s %.3f",ph[q],1e3*L[mi].wall[q]/S);
            printf("\n");
            if(vsdlss_ledger_level()>=2){
                double bs=0; const char *bn[LG_NB]={"1-6","7-31","32-63","64-127","128-255","256-511",">=512"};
                for(int d=0;d<2;d++){ printf("BUCKETS %c %s (thread-cumulative TSC, mean ms / calls / GB/s):",m,d?"bwd":"fwd");
                    for(int q=0;q<LG_NB;q++){ bs+=L[mi].bucket_s[d][q]; printf(" [%s] %.3f/%lld/%.1f",bn[q],1e3*L[mi].bucket_s[d][q]/S,L[mi].bucket_n[d][q]/L[mi].solves,L[mi].bucket_s[d][q]>0?L[mi].bucket_bytes[d][q]/L[mi].bucket_s[d][q]/1e9:0); }
                    printf("\n"); }
                printf("RECONCILE %c: sum of buckets %.3f ms vs core_fwd+core_bwd wall %.3f ms (single thread: should be <=, gap = loop/timer overhead)\n",m,1e3*bs/S,1e3*(L[mi].wall[LG_CORE_FWD]+L[mi].wall[LG_CORE_BWD])/S);
            }
        }
    }
    unsigned long long hash=1469598103934665603ULL;
    if(strchr(modes,'P')) for(csi p=0;p<n;p++) vG[pperm[p]]=vP[p];
    else if(strchr(modes,'I')) for(csi p=0;p<n;p++) vG[pperm[p]]=vI[p];   /* P state in caller order */
    for(csi i=0;i<n;i++){ unsigned long long u; memcpy(&u,&vG[i],8); hash=(hash^u)*1099511628211ULL; }
    printf("# final solution hash (caller order) %016llx\n",hash);
    return 0;
}
