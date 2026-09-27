/* A/B benchmark of the single-RHS panel solve kernels on one factor.
 * Generator and arguments are those of bench_pg_profile.c (copied below);
 * after one factorization it alternates vsdlss_solve_v2 = 0 (previous
 * kernels), 1 (forward only), 2 (backward only), 3 (both) in rotated order,
 * checks every solution bitwise against the previous kernels, and replays
 * each panel-width bucket alone to time the kernels without the rest of the
 * solve.  Extra env: AB_ROUNDS (default 15), AB_REPLAY (default 9).
 *
 * Original header of bench_pg_profile.c:
 * Power-grid benchmark built to a prescribed node-degree histogram.
 *
 *   ./bench_pg_profile [order] [threads] [nrhs] [shuffle] [N d1 d2 d3 d4 d5 d6p]
 *   PG_NETS=2 [PG_SPLIT=0.52] ./bench_pg_profile ...   separate VDD / GND nets
 *
 * Counts are exact numbers of nodes with degree 1, 2, 3, 4, 5 and >= 6
 * (off-diagonal neighbours).  Defaults are the customer case: N = 22875397,
 * d1 = 2561262, d2 = 15270759, d3 = 5000468, d4 = 42895, d5 = 10, d6p = 3.
 *
 * Topology (planar, like routed power straps):
 *   - junctions on a brick-wall (honeycomb) lattice: interior degree 3,
 *     boundary degree 2;
 *   - every lattice edge is a wire of 4..5 series segments (degree-2 nodes);
 *   - d1 pendant taps, each on its own wire node (the host becomes degree 3);
 *   - short straps between junctions two columns apart (degree 4);
 *   - 10 junctions with two straps (degree 5);
 *   - one wire node with two taps (degree 4, keeps the degree sum even);
 *   - 3 supply nodes of degree 8/8/10 feeding pads, strongly grounded.
 * The generator solves for lattice size, wire lengths and strap count so the
 * histogram matches; it prints the achieved histogram next to the target.
 * Numbering is shuffled unless shuffle = 0. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/resource.h>
static long minflt(void){struct rusage u;getrusage(RUSAGE_SELF,&u);return u.ru_minflt;}

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static uint64_t rng=0x9E3779B97F4A7C15ULL;
static uint64_t rnd(void){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static double uni(double a,double b){return a+(b-a)*(double)(rnd()>>11)*(1.0/9007199254740992.0);}
static long anon_huge_mb(void)
{
    FILE *f=fopen("/proc/self/smaps_rollup","r"); char line[256]; long kb=-1;
    if(!f)return -1;
    while(fgets(line,sizeof line,f)) if(!strncmp(line,"AnonHugePages:",14)){kb=atol(line+14);break;}
    fclose(f); return kb/1024;
}
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
    csi R=sp[0].R,C=sp[0].C;

    /* Histogram of the generated graph. */
    csi *deg=calloc((size_t)n,sizeof(csi)), hist[7]={0}, maxdeg=0;
    for(csi k=0;k<E.m;k++){deg[E.a[k]]++;deg[E.b[k]]++;}
    for(csi v=0;v<n;v++){if(deg[v]>maxdeg)maxdeg=deg[v];hist[deg[v]<6?deg[v]:6]++;}
    free(deg);

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
    if(!cur){puts("alloc failed");return 1;}
    for(csi j=0;j<n;j++)cur[j]=A->p[j];
    for(csi k=0;k<E.m;k++){csi a=perm[E.a[k]],b=perm[E.b[k]],col=a>b?a:b;A->i[cur[col]]=a<b?a:b;A->x[cur[col]++]=-E.g[k];}
    for(csi j=0;j<n;j++){A->i[cur[j]]=j;A->x[cur[j]++]=diag[j];}
    csi m=E.m;
    free(cur);free(diag);free(perm);free(E.a);free(E.b);free(E.g);
    double tgen=now()-t0;

    if(nets==2)printf("# nets: VDD n=%lld lattice=%lldx%lld  GND n=%lld lattice=%lldx%lld\n",
                      (long long)sp[0].N,(long long)sp[0].R,(long long)sp[0].C,(long long)sp[1].N,(long long)sp[1].R,(long long)sp[1].C);
    printf("# pg_profile n=%lld edges=%lld nnz_upper=%lld nnz_full=%lld max_degree=%lld lattice=%lldx%lld gen=%.1fs\n",
           (long long)n,(long long)m,(long long)(m+n),(long long)(2*m+n),(long long)maxdeg,(long long)R,(long long)C,tgen);
    printf("# degree  target:   d1=%lld d2=%lld d3=%lld d4=%lld d5=%lld d6+=%lld\n",(long long)D1,(long long)D2,(long long)D3,(long long)D4,(long long)D5,(long long)D6);
    printf("# degree  achieved: d1=%lld d2=%lld d3=%lld d4=%lld d5=%lld d6+=%lld (d0=%lld)\n",(long long)hist[1],(long long)hist[2],
           (long long)hist[3],(long long)hist[4],(long long)hist[5],(long long)hist[6],(long long)hist[0]);
    printf("# rss after build: %ld MB\n",peak_rss_mb());
    fflush(stdout);

    if(vsdlss_set_num_threads(threads)!=VSDLSS_OK){puts("bad thread count");return 1;}
    vsdlss_m3_factor *f=NULL;
    long f0=minflt();
    double tt=now();
    vsdlss_status st=vsdlss_factorize_m3(A,order,&f);
    double tf=now()-tt;
    long faults=minflt()-f0;
    if(st!=VSDLSS_OK){printf("factor failed: %s (peak %ld MB)\n",vsdlss_status_string(st),peak_rss_mb());return 1;}
    csi lnz=0,core=0,comps=f->count;
    for(csi k=0;k<f->count;k++){core+=f->component[k].reduction->core_n;if(f->component[k].numeric)lnz+=f->component[k].numeric->l_nnz;}
    printf("# factor: order=%d threads=%d time=%.2fs components=%lld core_n=%lld (%.2f%%) core_nnzL=%.1fM peak_rss=%ld MB page_faults=%ld\n",
           order,threads,tf,(long long)comps,(long long)core,100.0*core/n,lnz*1e-6,peak_rss_mb(),faults);
    fflush(stdout);

    (void)nrhs;
    double *b=malloc((size_t)n*sizeof(double)),*x=malloc((size_t)n*sizeof(double)),*xr=malloc((size_t)n*sizeof(double));
    if(!b||!x||!xr){puts("alloc failed");return 1;}
    for(csi i=0;i<n;i++)b[i]=uni(-1e-3,0);
    /* Reference: previous kernels. */
    vsdlss_solve_v2=0; st=vsdlss_m3_solve(f,b,xr);
    if(st!=VSDLSS_OK){printf("solve failed: %s\n",vsdlss_status_string(st));return 1;}
    double eta;vsdlss_backward_error(A,xr,b,&eta);
    for(int cfg=1;cfg<4;cfg++){
        vsdlss_solve_v2=cfg; st=vsdlss_m3_solve(f,b,x);
        if(st!=VSDLSS_OK||memcmp(x,xr,(size_t)n*sizeof(double))){printf("cfg %d differs from previous kernels\n",cfg);return 1;}
    }
    printf("# check: cfg 1/2/3 bitwise equal to cfg 0, backward_error=%.2e\n",eta);
    /* Full solve, rotated order each round. */
    const char *e=getenv("AB_ROUNDS"); int rounds=e?atoi(e):15; if(rounds<3)rounds=3; if(rounds>200)rounds=200;
    double T[4][200];
    for(int w=0;w<2;w++)for(int cfg=0;cfg<4;cfg++){vsdlss_solve_v2=cfg;vsdlss_m3_solve(f,b,x);}   /* warm */
    for(int r=0;r<rounds;r++)for(int k=0;k<4;k++){
        int cfg=(r+k)&3; vsdlss_solve_v2=cfg;
        tt=now(); st=vsdlss_m3_solve(f,b,x); T[cfg][r]=now()-tt;
        if(st!=VSDLSS_OK||memcmp(x,xr,(size_t)n*sizeof(double))){printf("round %d cfg %d differs\n",r,cfg);return 1;}
    }
    int cmpd(const void*,const void*);
    double med[4],mn[4];
    for(int cfg=0;cfg<4;cfg++){
        double s[200]; memcpy(s,T[cfg],sizeof(double)*rounds); qsort(s,rounds,sizeof(double),cmpd);
        med[cfg]=rounds&1?s[rounds/2]:(s[rounds/2-1]+s[rounds/2])/2; mn[cfg]=s[0];
    }
    static const char *name[4]={"previous","fwd_reg","back_dot8","both"};
    printf("# full solve (vsdlss_m3_solve, %d rounds, rotated order, all bitwise checked):\n",rounds);
    for(int cfg=0;cfg<4;cfg++){
        double ratio[200]; for(int r=0;r<rounds;r++)ratio[r]=T[cfg][r]/T[0][r];
        qsort(ratio,rounds,sizeof(double),cmpd);
        printf("AB_FULL cfg=%d %-10s median=%.5fs min=%.5fs paired_ratio_median=%.4f q25=%.4f q75=%.4f\n",
               cfg,name[cfg],med[cfg],mn[cfg],ratio[rounds/2],ratio[rounds/4],ratio[(3*rounds)/4]);
    }
    for(int r=0;r<rounds;r++)printf("AB_RAW round=%d t0=%.5f t1=%.5f t2=%.5f t3=%.5f\n",r,T[0][r],T[1][r],T[2][r],T[3][r]);
    /* Bucket replay: each width bucket's panels alone, forward in order then
     * backward in reverse order, on the component's core vector. */
    e=getenv("AB_REPLAY"); int reps=e?atoi(e):9; if(reps<3)reps=3; if(reps>99)reps=99;
    static const char *bn[7]={"1-6","7-31","32-63","64-127","128-255","256-511",">=512"};
    #define BKT(w) ((w)<=6?0:(w)<32?1:(w)<64?2:(w)<128?3:(w)<256?4:(w)<512?5:6)
    double R0[7][2][99],R3[7][2][99]; long long cnt[7]={0}; int ok=1;
    csi maxn=0; for(csi c=0;c<f->count;c++)if(f->component[c].numeric&&f->component[c].numeric->n>maxn)maxn=f->component[c].numeric->n;
    double *y0=malloc((size_t)maxn*sizeof(double)),*y=malloc((size_t)maxn*sizeof(double)),*yref=malloc((size_t)maxn*sizeof(double));
    if(!y0||!y||!yref){puts("alloc failed");return 1;}
    for(csi i=0;i<maxn;i++)y0[i]=uni(0.5,1.5);
    for(csi c=0;c<f->count;c++){const vsdlss_sn_factor*F=f->component[c].numeric;if(!F)continue;
        for(csi s=0;s<F->count;s++)cnt[BKT(F->column_start[s+1]-F->column_start[s])]++;}
    for(int k=0;k<7;k++){
        for(int rep=0;rep<reps;rep++)for(int side=0;side<2;side++){
            int cfg=((rep+side)&1)?3:0; vsdlss_solve_v2=cfg;
            double tf=0,tb=0;
            for(csi c=0;c<f->count;c++){const vsdlss_sn_factor*F=f->component[c].numeric;if(!F)continue;
                memcpy(y,y0,(size_t)F->n*sizeof(double));
                for(int back=0;back<2;back++){
                    double t1=now();
                    for(csi t=0;t<F->count;t++){csi s=back?F->count-1-t:t;
                        csi bb=F->column_start[s],w=F->column_start[s+1]-bb; if(BKT(w)!=k)continue;
                        csi ext=F->row_ptr[s+1]-F->row_ptr[s];
                        vsdlss_panel_solve(F->panel+F->panel_offset[s],bb,w,ext,ext?F->row_index+F->row_ptr[s]:NULL,y,back);}
                    if(back)tb+=now()-t1; else tf+=now()-t1;
                }
                if(c==0){ if(cfg==0)memcpy(yref,y,(size_t)F->n*sizeof(double));
                          else if(rep>0&&memcmp(yref,y,(size_t)F->n*sizeof(double)))ok=0; }
            }
            if(cfg==0){R0[k][0][rep]=tf;R0[k][1][rep]=tb;}else{R3[k][0][rep]=tf;R3[k][1][rep]=tb;}
        }
    }
    printf("# bucket replay (panel kernels only, %d reps each, previous vs both, alternating; results %s):\n",reps,ok?"bitwise equal":"DIFFER");
    printf("# bucket panels  fwd_prev_ms fwd_new_ms fwd_speedup  back_prev_ms back_new_ms back_speedup\n");
    for(int k=0;k<7;k++){
        double m[2][2];
        for(int d=0;d<2;d++){double s0[99],s3[99];memcpy(s0,R0[k][d],sizeof(double)*reps);memcpy(s3,R3[k][d],sizeof(double)*reps);
            qsort(s0,reps,sizeof(double),cmpd);qsort(s3,reps,sizeof(double),cmpd);m[d][0]=s0[reps/2];m[d][1]=s3[reps/2];}
        printf("AB_BUCKET %-8s %7lld  %9.3f %9.3f %6.3fx   %9.3f %9.3f %6.3fx\n",bn[k],cnt[k],
               m[0][0]*1e3,m[0][1]*1e3,m[0][0]/m[0][1],m[1][0]*1e3,m[1][1]*1e3,m[1][0]/m[1][1]);
    }
    printf("# peak_rss=%ld MB huge_pages=%ld MB\n",peak_rss_mb(),anon_huge_mb());
    free(y0);free(y);free(yref);
    vsdlss_m3_factor_free(f);vsdlss_spfree(A);free(b);free(x);free(xr);
    return ok?0:1;
}
int cmpd(const void*a,const void*b){double x=*(const double*)a,y=*(const double*)b;return (x>y)-(x<y);}
