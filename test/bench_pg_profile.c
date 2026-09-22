/* Power-grid benchmark built to a prescribed node-degree histogram.
 *
 *   ./bench_pg_profile [order] [threads] [nrhs] [shuffle] [N d1 d2 d3 d4 d5 d6p]
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
static long peak_rss_mb(void)
{
    FILE *f=fopen("/proc/self/status","r"); char line[256]; long kb=-1;
    if(!f)return -1;
    while(fgets(line,sizeof line,f)) if(!strncmp(line,"VmHWM:",6)){kb=atol(line+6);break;}
    fclose(f); return kb/1024;
}

typedef struct { csi *a,*b; double *g; csi m,cap; } edges;
static void add(edges *E,csi a,csi b,double g){E->a[E->m]=a;E->b[E->m]=b;E->g[E->m]=g;E->m++;}

int main(int argc,char **argv)
{
    int order=argc>1?atoi(argv[1]):5, threads=argc>2?atoi(argv[2]):1;
    int nrhs=argc>3?atoi(argv[3]):2, shuffle=argc>4?atoi(argv[4]):1;
    csi N=22875397,D1=2561262,D2=15270759,D3=5000468,D4=42895,D5=10,D6=3;
    if(argc>11){N=atoll(argv[5]);D1=atoll(argv[6]);D2=atoll(argv[7]);D3=atoll(argv[8]);
        D4=atoll(argv[9]);D5=atoll(argv[10]);D6=atoll(argv[11]);}
    if(D6!=3||D5<0||D4<47){fprintf(stderr,"generator expects d6p=3 and d4>=47\n");return 2;}
    double t0=now();

    /* Lattice: interior junctions I must satisfy
     *   deg3 = (I - D4 - D5) + D1   (hosts of taps are degree-3 wire nodes). */
    /* Degree-sum parity: 3 supply nodes of degree 8/8/10 (26 pads), and one
     * wire node carrying two taps (degree 4), so taps use D1-1 hosts.  Then
     *   deg3 = (I - (D4-1) - D5) + (D1-2) single-tap hosts. */
    csi I_target=D3-D1+D4+D5+1;
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
    /* Junction degrees before extras: lat[3] of degree 3, lat[2] of degree 2 (and a few 1). */
    csi S=(D4-26-20-1)/2;   /* straps; 26 pads, 20 partners, 1 double-tap host */
    if(2*S+47!=D4){fprintf(stderr,"d4 parity unsupported\n");return 2;}
    /* Wire nodes: d2 = plain wire nodes + degree-2 junctions; hosts = D1. */
    csi supply=3, H=D1-1, T=D2-lat[2]+H, extra_deg3_junctions=lat[3]-(D4+D5);
    csi n=J+T+D1+supply;
    if(n!=N) {
        /* Absorb the lattice rounding in the wire count so N is exact. */
        csi diff=N-n; T+=diff; n=N;
    }
    (void)extra_deg3_junctions;
    csi base=T/El, rem=T%El;
    csi mcap=El*(base+2)+D1+S+20+25+16;
    edges E={malloc((size_t)mcap*sizeof(csi)),malloc((size_t)mcap*sizeof(csi)),malloc((size_t)mcap*sizeof(double)),0,mcap};
    double *ground=calloc((size_t)n,sizeof(double));
    if(!E.a||!E.b||!E.g||!ground){puts("alloc failed");return 1;}

    /* Wires with taps: wire node t (0..T-1) hosts a tap iff the running share
     * of D1 over T steps up, which spreads exactly D1 taps evenly. */
    csi wire=J, tap=J+T, t=0, e=0, taps=0;
    for(csi r=0;r<R;r++)for(csi c=0;c<C;c++)for(int dir=0;dir<2;dir++){
        csi v=r*C+c,w;
        if(dir==0){ if(c+1>=C)continue; w=v+1; }
        else { if(!VERT(r,c))continue; w=v+C; }
        csi len=base+(e<rem); e++;
        csi prev=v;
        for(csi k=0;k<len;k++){
            csi node=wire++;
            add(&E,prev,node,uni(0.5,2.0));
            if((csi)((double)(t+1)*H/T)>(csi)((double)t*H/T)){
                int two=taps==0;   /* the first host carries two taps */
                for(int q=0;q<1+two;q++){add(&E,node,tap,uni(0.05,0.5));ground[tap]=1e-3;tap++;taps++;}
            }
            t++; prev=node;
        }
        add(&E,prev,w,uni(0.5,2.0));
    }
    /* Extras on interior degree-3 junctions, spread with a stride. */
    unsigned char *used=calloc((size_t)J,1);
    csi made=0; visit=0;
#define NEXT_FREE(v) do{ for(;;){ if(visit>J){fprintf(stderr,"cannot place straps\n");return 2;} \
        v=NEXT_POS(); if(jdeg[v]==3&&!used[v]&&v%C+2<C&&jdeg[v+2]==3&&!used[v+2]) break; } }while(0)
    for(csi s=0;s<S;s++){csi v;NEXT_FREE(v);used[v]=used[v+2]=1;add(&E,v,v+2,1.0);made++;}
    for(csi s=0;s<D5;s++){ /* junction with two straps: partners on both sides */
        csi v;
        for(;;){NEXT_FREE(v); if(v%C>=2&&jdeg[v-2]==3&&!used[v-2])break;}
        used[v]=used[v+2]=used[v-2]=1; add(&E,v,v+2,1.0); add(&E,v,v-2,1.0);
    }
    csi sdeg[3]={8,8,10};
    for(csi s=0;s<3;s++){
        csi sn=J+T+D1+s; ground[sn]=100.0;
        for(csi k=0;k<sdeg[s];k++){
            if(visit>J){fprintf(stderr,"cannot place pads\n");return 2;}
            csi v=NEXT_POS();
            if(jdeg[v]!=3||used[v]){k--;continue;} used[v]=1; add(&E,sn,v,5.0);}
    }
    free(used); free(jdeg); free(cut);
    if(wire!=J+T||tap!=J+T+D1){fprintf(stderr,"generator bookkeeping failed %lld %lld\n",(long long)(wire-J-T),(long long)(tap-J-T-D1));return 1;}

    /* Histogram of the generated graph. */
    csi *deg=calloc((size_t)n,sizeof(csi)), hist[7]={0}, maxdeg=0;
    for(csi k=0;k<E.m;k++){deg[E.a[k]]++;deg[E.b[k]]++;}
    for(csi v=0;v<n;v++){if(deg[v]>maxdeg)maxdeg=deg[v];hist[deg[v]<6?deg[v]:6]++;}
    free(deg);

    /* Upper CSC with shuffled numbering. */
    csi *perm=malloc((size_t)n*sizeof(csi));
    for(csi i=0;i<n;i++)perm[i]=i;
    if(shuffle)for(csi i=n-1;i>0;i--){csi j=(csi)(rnd()%(uint64_t)(i+1)),x=perm[i];perm[i]=perm[j];perm[j]=x;}
    vsdlss *A=vsdlss_spalloc(n,n,n+E.m,1,0);
    double *diag=calloc((size_t)n,sizeof(double));
    if(!A||!diag||!perm){puts("alloc failed");return 1;}
    memset(A->p,0,(size_t)(n+1)*sizeof(csi));
    for(csi k=0;k<E.m;k++){csi a=perm[E.a[k]],b=perm[E.b[k]];A->p[(a>b?a:b)+1]++;diag[a]+=E.g[k];diag[b]+=E.g[k];}
    for(csi i=0;i<n;i++){diag[perm[i]]+=ground[i];A->p[i+1]++;}
    free(ground);
    for(csi j=0;j<n;j++)A->p[j+1]+=A->p[j];
    csi *cur=malloc((size_t)n*sizeof(csi));
    for(csi j=0;j<n;j++)cur[j]=A->p[j];
    for(csi k=0;k<E.m;k++){csi a=perm[E.a[k]],b=perm[E.b[k]],col=a>b?a:b;A->i[cur[col]]=a<b?a:b;A->x[cur[col]++]=-E.g[k];}
    for(csi j=0;j<n;j++){A->i[cur[j]]=j;A->x[cur[j]++]=diag[j];}
    csi m=E.m;
    free(cur);free(diag);free(perm);free(E.a);free(E.b);free(E.g);
    double tgen=now()-t0;

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
    double *b=malloc((size_t)n*nrhs*sizeof(double)),*x=malloc((size_t)n*nrhs*sizeof(double));
    if(!b||!x){puts("alloc failed");return 1;}
    for(csi i=0;i<(csi)n*nrhs;i++)b[i]=uni(-1e-3,0);
    tt=now();st=vsdlss_m3_solve(f,b,x);double ts=now()-tt;
    if(st!=VSDLSS_OK){printf("solve failed: %s\n",vsdlss_status_string(st));return 1;}
    double eta;vsdlss_backward_error(A,x,b,&eta);
    tt=now();st=vsdlss_m3_solve_many(f,nrhs,b,n,x,n);double tm=now()-tt;
    if(st!=VSDLSS_OK){printf("solve_many failed: %s\n",vsdlss_status_string(st));return 1;}
    printf("# solve: single=%.2fs batch%d=%.2fs (%.2fs/RHS) backward_error=%.2e peak_rss=%ld MB\n",
           ts,nrhs,tm,tm/nrhs,eta,peak_rss_mb());
    vsdlss_m3_factor_free(f);vsdlss_spfree(A);free(b);free(x);
    return 0;
}
