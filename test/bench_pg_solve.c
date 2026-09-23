/* Single-solve scaling on the bench_pg_profile grid: one factorization
 * (with `threads`), then SOLVE_REPS timed vsdlss_m3_solve calls for each
 * thread count in SOLVE_THREADS (default "1 2 4 8 16"), a bitwise check
 * across counts, and the same for vsdlss_m3_solve_internal.  PG_DUMP=file
 * writes the system (n, nnz, p, i, x, b as int64/double) and exits, for
 * bench_cholmod.
 *
 *   ./bench_pg_solve [order] [threads] [nrhs] [shuffle] [N d1 d2 d3 d4 d5 d6p]
 *   PG_NETS=2 [PG_SPLIT=0.52] ./bench_pg_solve ...   separate VDD / GND nets
 *
 * Generator (shared with bench_pg_profile):
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

    { const char *dp=getenv("PG_DUMP");     /* n, nnz, p[n+1], i[nnz], x[nnz], b[n] (same b as below) */
      if(dp){ FILE *o=fopen(dp,"wb"); if(!o){puts("dump open failed");return 1;} int64_t nn=n,nz=A->p[n];
        fwrite(&nn,8,1,o);fwrite(&nz,8,1,o);fwrite(A->p,8,n+1,o);fwrite(A->i,8,nz,o);fwrite(A->x,8,nz,o);
        for(csi i=0;i<n;i++){double v=uni(-1e-3,0);fwrite(&v,8,1,o);}
        fclose(o); printf("# dumped to %s\n",dp); return 0; } }
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
    /* Solve-only scaling: one factor, then SOLVE_THREADS thread counts,
     * SOLVE_REPS timed single-RHS solves each (after 2 warm-ups). */
    double *b=malloc((size_t)n*sizeof(double)),*x=malloc((size_t)n*sizeof(double)),*x0=malloc((size_t)n*sizeof(double));
    if(!b||!x||!x0){puts("alloc failed");return 1;}
    for(csi i=0;i<n;i++)b[i]=uni(-1e-3,0);
    const char *tl=getenv("SOLVE_THREADS"); if(!tl)tl="1 2 4 8 16";
    const char *re=getenv("SOLVE_REPS"); int reps=re?atoi(re):15;
    int first=1; char buf[256]; strncpy(buf,tl,255); buf[255]=0;
    for(char *tok=strtok(buf," ");tok;tok=strtok(NULL," ")){
        int t=atoi(tok); if(vsdlss_set_num_threads(t)!=VSDLSS_OK){puts("bad threads");return 1;}
        for(int w=0;w<2;w++) if(vsdlss_m3_solve(f,b,x)!=VSDLSS_OK){puts("solve failed");return 1;}
        fprintf(stderr,"@@ threads=%d\n",t);
        double ts[256]; if(reps>256)reps=256;
        for(int r=0;r<reps;r++){double t0=now(); if(vsdlss_m3_solve(f,b,x)!=VSDLSS_OK){puts("solve failed");return 1;} ts[r]=now()-t0;}
        for(int i=1;i<reps;i++){double v=ts[i];int j=i;while(j>0&&ts[j-1]>v){ts[j]=ts[j-1];j--;}ts[j]=v;}
        if(first){memcpy(x0,x,(size_t)n*sizeof(double));first=0;}
        int same=!memcmp(x0,x,(size_t)n*sizeof(double));
        printf("SOLVE threads=%d min=%.4f med=%.4f max=%.4f bitwise_same=%d\n",t,ts[0],ts[reps/2],ts[reps-1],same);
        fflush(stdout); fflush(stderr);
    }
    { uint64_t h=1469598103934665603ULL; const unsigned char *p=(const unsigned char*)x;
      for(size_t i=0;i<(size_t)n*8;i++){h^=p[i];h*=1099511628211ULL;}
      printf("# xhash=%016llx\n",(unsigned long long)h); }
    double eta;vsdlss_backward_error(A,x,b,&eta); printf("# backward_error=%.2e\n",eta);
    /* Internal-order solves: permute once outside the timing, then compare
     * the result mapped back against the regular solve bitwise. */
    {
        csi *perm=malloc((size_t)n*sizeof(csi)); double *bi=malloc((size_t)n*8),*xi=malloc((size_t)n*8);
        if(!perm||!bi||!xi||vsdlss_m3_internal_order(f,perm)!=VSDLSS_OK){puts("internal setup failed");return 1;}
        for(csi p=0;p<n;p++)bi[p]=b[perm[p]];
        strncpy(buf,tl,255); buf[255]=0;
        for(char *tok=strtok(buf," ");tok;tok=strtok(NULL," ")){
            int t=atoi(tok); vsdlss_set_num_threads(t);
            for(int w=0;w<2;w++) if(vsdlss_m3_solve_internal(f,bi,xi)!=VSDLSS_OK){puts("internal solve failed");return 1;}
            double ts[256];
            for(int r=0;r<reps;r++){double t0=now(); vsdlss_m3_solve_internal(f,bi,xi); ts[r]=now()-t0;}
            for(int i=1;i<reps;i++){double v=ts[i];int j=i;while(j>0&&ts[j-1]>v){ts[j]=ts[j-1];j--;}ts[j]=v;}
            int same=1; for(csi p=0;p<n;p++) if(memcmp(&xi[p],&x0[perm[p]],8)){same=0;break;}
            printf("INTERNAL threads=%d min=%.4f med=%.4f max=%.4f bitwise_same_as_solve=%d\n",t,ts[0],ts[reps/2],ts[reps-1],same);
            fflush(stdout);
        }
        free(perm);free(bi);free(xi);
    }
    vsdlss_m3_factor_free(f);vsdlss_spfree(A);free(b);free(x);free(x0);
    return 0;
}
