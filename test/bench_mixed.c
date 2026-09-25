/* Transient EMIR workload on the bench_pg_profile grid (single RHS per step):
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
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/resource.h>

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

    /* ---------------- transient workload ----------------
     * A = G + C/h with diagonal grounded capacitance C, factored once;
     * b_k = i_k + (C/h) v_{k-1} assembled in packed order every step. */
    const char *ev;
    int steps = (ev = getenv("TR_STEPS")) ? atoi(ev) : 200;
    double h = (ev = getenv("TR_H")) ? atof(ev) : 1e-11;           /* 10 ps */
    double cfrac = (ev = getenv("TR_CFRAC")) ? atof(ev) : 1.0;     /* share of nodes with C */
    double ifrac = (ev = getenv("TR_IFRAC")) ? atof(ev) : 0.3;     /* share of nodes with loads */
    int compare_every = (ev = getenv("TR_CHECK")) ? atoi(ev) : 50;
    int alias = (ev = getenv("TR_ALIAS")) ? atoi(ev) : 0;           /* 1: build b_k in v, solve in place */
    double *cap = malloc((size_t)n * sizeof(double)), *iamp = malloc((size_t)n * sizeof(double));
    unsigned char *grp = malloc((size_t)n);
    if (!cap || !iamp || !grp) { puts("alloc failed"); return 1; }
    for (csi i = 0; i < n; i++) {
        cap[i] = uni(0, 1) < cfrac ? uni(0.5e-15, 5e-15) : 0.0;          /* 0.5-5 fF */
        iamp[i] = uni(0, 1) < ifrac ? uni(1e-6, 1e-4) : 0.0;            /* 1-100 uA */
        grp[i] = (unsigned char)(rnd() % 8);
    }
    /* Diagonal of the upper CSC is the last entry of each column. */
    for (csi j = 0; j < n; j++) {
        csi p = A->p[j + 1] - 1;
        if (A->i[p] != j) { puts("diagonal not last"); return 1; }
        A->x[p] += cap[j] / h;
    }
    if (vsdlss_set_num_threads(threads) != VSDLSS_OK) { puts("bad thread count"); return 1; }
    vsdlss_m3_factor *f = NULL;
    double tt = now();
    vsdlss_status st = vsdlss_factorize_m3(A, order, &f);
    double tf = now() - tt;
    if (st != VSDLSS_OK) { printf("factor failed: %s\n", vsdlss_status_string(st)); return 1; }
    csi lnz = 0, core = 0;
    for (csi k = 0; k < f->count; k++) { core += f->component[k].reduction->core_n; if (f->component[k].numeric) lnz += f->component[k].numeric->l_nnz; }
    printf("# factor (A = G + C/h, h=%.3g, C on %.0f%% of nodes): order=%d threads=%d time=%.2fs components=%lld core_n=%lld (%.2f%%) core_nnzL=%.1fM peak_rss=%ld MB\n",
           h, 100 * cfrac, order, threads, tf, (long long)f->count, (long long)core, 100.0 * core / n, lnz * 1e-6, peak_rss_mb());

    fflush(stdout);

    /* ---------------- mixed-precision experiment ----------------
     * Core L rounded to float (kept in double storage: models "float storage,
     * double arithmetic"); a double copy is kept for the reference.  Three
     * independent trajectories of the same transient:
     *   ref   : double L, v_k = solve(b_k(v_ref))
     *   plain : rounded L, v_k = solve(b_k(v_plain))
     *   mixed : rounded L, v_k = v_{k-1} + solve(b_k(v_{k-1}) - A v_{k-1})
     *           (residual in double; only the correction sees the rounding) */
    const int nth = threads;
    /* full CSR of A (both triangles) for a parallel residual */
    csi *rp = calloc((size_t)n + 1, sizeof(csi));
    if (!rp) { puts("alloc failed"); return 1; }
    for (csi j = 0; j < n; j++) for (csi p = A->p[j]; p < A->p[j + 1]; p++) { csi i = A->i[p]; rp[i + 1]++; if (i != j) rp[j + 1]++; }
    for (csi i = 0; i < n; i++) rp[i + 1] += rp[i];
    csi *ci = malloc((size_t)rp[n] * sizeof(csi)), *fillp = malloc((size_t)n * sizeof(csi));
    double *cv = malloc((size_t)rp[n] * sizeof(double));
    if (!ci || !cv || !fillp) { puts("alloc failed"); return 1; }
    memcpy(fillp, rp, (size_t)n * sizeof(csi));
    for (csi j = 0; j < n; j++) for (csi p = A->p[j]; p < A->p[j + 1]; p++) {
        csi i = A->i[p]; double v = A->x[p];
        ci[fillp[i]] = j; cv[fillp[i]++] = v;
        if (i != j) { ci[fillp[j]] = i; cv[fillp[j]++] = v; }
    }
    free(fillp);
    /* double copies of the core factors; round the factor's own panels */
    double **pd = calloc((size_t)f->count, sizeof(double *)), **pr = calloc((size_t)f->count, sizeof(double *));
    double rounded_max_rel = 0; csi lstored = 0;
    for (csi c = 0; c < f->count; c++) {
        vsdlss_sn_factor *s = f->component[c].numeric; if (!s) continue;
        csi sz = s->panel_offset[s->count]; lstored += sz;
        pd[c] = malloc((size_t)sz * sizeof(double)); if (!pd[c]) { puts("alloc failed"); return 1; }
        memcpy(pd[c], s->panel, (size_t)sz * sizeof(double)); pr[c] = s->panel;
        for (csi i = 0; i < sz; i++) {
            double v = s->panel[i], w = (double)(float)v;
            if (v != 0) { double e = fabs(w - v) / fabs(v); if (e > rounded_max_rel) rounded_max_rel = e; }
            s->panel[i] = w;
        }
    }
#define USE_L(arr) do { for (csi c_ = 0; c_ < f->count; c_++) if (f->component[c_].numeric) f->component[c_].numeric->panel = arr[c_]; } while (0)
    printf("# core L: %lld stored entries rounded to float (max rel rounding %.2e); double copy kept for the reference\n",
           (long long)lstored, rounded_max_rel);
    double *hg = malloc((size_t)n * 8), *vref = calloc((size_t)n, 8), *vpl = calloc((size_t)n, 8), *vmx = calloc((size_t)n, 8);
    double *bb = malloc((size_t)n * 8), *av = malloc((size_t)n * 8), *dl = malloc((size_t)n * 8);
    if (!hg || !vref || !vpl || !vmx || !bb || !av || !dl) { puts("alloc failed"); return 1; }
    for (csi i = 0; i < n; i++) hg[i] = cap[i] / h;
    double wave[8];
#define WAVES(k) do { double tk_ = (double)(k); \
        for (int q_ = 0; q_ < 8; q_++) { double ph_ = fmod(tk_ + 7.0 * q_, 40.0); \
            wave[q_] = q_ < 3 ? (ph_ < 5 ? 1.0 : 0.05) : q_ < 5 ? (tk_ >= 20.0 * (q_ - 2) ? 1.0 : 0.1) : (ph_ < 20 ? ph_ / 20 : (40 - ph_) / 20); } } while (0)
    double t_ref = 0, t_rnd = 0, t_spmv = 0, worst_pl = 0, worst_mx = 0, worst_pl_abs = 0, worst_mx_abs = 0;
    int report = (ev = getenv("TR_REPORT")) ? atoi(ev) : 20;
    printf("# step | ref: max|v| (range) | plain rounded: max|err| rel | mixed+correction: max|err| rel | |delta|/|v|\n");
    for (int k = 0; k < steps; k++) {
        WAVES(k);
        /* reference */
        VSDLSS_OMP(omp parallel for num_threads(nth) schedule(static))
        for (csi i = 0; i < n; i++) bb[i] = iamp[i] * wave[grp[i]] + hg[i] * vref[i];
        USE_L(pd);
        double t0 = now();
        if (vsdlss_m3_solve(f, bb, vref) != VSDLSS_OK) { printf("ref solve failed at %d\n", k); return 1; }
        t_ref += now() - t0;
        USE_L(pr);
        /* plain rounded */
        VSDLSS_OMP(omp parallel for num_threads(nth) schedule(static))
        for (csi i = 0; i < n; i++) bb[i] = iamp[i] * wave[grp[i]] + hg[i] * vpl[i];
        t0 = now();
        if (vsdlss_m3_solve(f, bb, vpl) != VSDLSS_OK) { printf("plain solve failed at %d\n", k); return 1; }
        t_rnd += now() - t0;
        /* mixed: residual in double, correction with the rounded factor */
        t0 = now();
        VSDLSS_OMP(omp parallel for num_threads(nth) schedule(dynamic, 4096))
        for (csi i = 0; i < n; i++) { double s = 0; for (csi p = rp[i]; p < rp[i + 1]; p++) s += cv[p] * vmx[ci[p]]; av[i] = s; }
        VSDLSS_OMP(omp parallel for num_threads(nth) schedule(static))
        for (csi i = 0; i < n; i++) bb[i] = iamp[i] * wave[grp[i]] + hg[i] * vmx[i] - av[i];
        t_spmv += now() - t0;
        if (vsdlss_m3_solve(f, bb, dl) != VSDLSS_OK) { printf("mixed solve failed at %d\n", k); return 1; }
        double dmax = 0;
        VSDLSS_OMP(omp parallel for num_threads(nth) schedule(static) reduction(max:dmax))
        for (csi i = 0; i < n; i++) { vmx[i] += dl[i]; if (fabs(dl[i]) > dmax) dmax = fabs(dl[i]); }
        if (report > 0 && (k % report == 0 || k == steps - 1)) {
            double vmax = 0, vmin = 1e300, vmx_ = -1e300, epl = 0, emx = 0, vmxmax = 0;
            VSDLSS_OMP(omp parallel for num_threads(nth) schedule(static) reduction(max:vmax,epl,emx,vmx_,vmxmax) reduction(min:vmin))
            for (csi i = 0; i < n; i++) {
                double a = fabs(vref[i]); if (a > vmax) vmax = a;
                if (vref[i] < vmin) vmin = vref[i]; if (vref[i] > vmx_) vmx_ = vref[i];
                double e1 = fabs(vpl[i] - vref[i]), e2 = fabs(vmx[i] - vref[i]);
                if (e1 > epl) epl = e1; if (e2 > emx) emx = e2;
                if (fabs(vmx[i]) > vmxmax) vmxmax = fabs(vmx[i]);
            }
            printf("  %4d | %.4e [%.4e, %.4e] | %.3e %.3e | %.3e %.3e | %.3e\n", k, vmax, vmin, vmx_,
                   epl, epl / vmax, emx, emx / vmax, dmax / (vmxmax > 0 ? vmxmax : 1));
            if (epl / vmax > worst_pl) worst_pl = epl / vmax; if (emx / vmax > worst_mx) worst_mx = emx / vmax;
            if (epl > worst_pl_abs) worst_pl_abs = epl; if (emx > worst_mx_abs) worst_mx_abs = emx;
            fflush(stdout);
        }
    }
    printf("MIXED steps=%d threads=%d | worst normwise rel error: plain rounded %.2e (abs %.2e) | mixed+correction %.2e (abs %.2e) | "
           "mean times: double solve %.4f s, rounded solve %.4f s, residual (SpMV+assembly) %.4f s\n",
           steps, threads, worst_pl, worst_pl_abs, worst_mx, worst_mx_abs, t_ref / steps, t_rnd / steps, t_spmv / steps);
    USE_L(pr);
    return 0;
}
