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

    /* Low-degree reduction: blocks vs sequential tail, and the dependency
     * layers of the tail in the forward and backward replays. */
    for (csi c = 0; c < f->count; c++) {
        const vsdlss_reduction *r = f->component[c].reduction;
        if (r->count < 100000 || !r->pk) continue;
        csi inblocks = 0; int blocked = r->blocks >= 1 && r->pk_count == r->blocks + 1;
        for (csi b = 0; blocked && b < r->blocks; b++) inblocks += r->pk[b].count;
        const vsdlss_pk_seg *g = r->pk + r->pk_count - 1;           /* tail (or the only segment) */
        csi *lev = calloc((size_t)r->n, sizeof(csi)), *elim = malloc((size_t)r->n * sizeof(csi));
        csi *lay = calloc((size_t)g->count + 2, sizeof(csi));
        if (!lev || !elim || !lay) { puts("alloc failed"); return 1; }
        for (csi v = 0; v < r->n; v++) elim[v] = -1;
        csi o = 0;
        for (csi i = 0; i < g->count; i++) { elim[g->head[i] & 0x3fffffffu] = i; o += (csi)(g->head[i] >> 30); }
        /* forward: record i waits for every earlier record that updates its vertex */
        csi fmax = 0; o = 0;
        for (csi i = 0; i < g->count; i++) {
            uint32_t hd = g->head[i]; csi d = (csi)(hd >> 30), v = hd & 0x3fffffffu, L = lev[v];
            if (L > fmax) fmax = L;
            for (csi j = 0; j < d; j++) { csi w = g->nb[o + j]; if (elim[w] > i && lev[w] < L + 1) lev[w] = L + 1; }
            o += d;
        }
        /* backward: record i reads the neighbours eliminated after it */
        memset(lev, 0, (size_t)r->n * sizeof(csi));
        csi bmax = 0; o = g->nbn;
        for (csi i = g->count; i > 0;) {
            i--; uint32_t hd = g->head[i]; csi d = (csi)(hd >> 30), v = hd & 0x3fffffffu, L = 0;
            o -= d;
            for (csi j = 0; j < d; j++) { csi w = g->nb[o + j]; if (elim[w] > i && lev[w] + 1 > L) L = lev[w] + 1; }
            lev[v] = L; lay[L < g->count ? L : g->count]++; if (L > bmax) bmax = L;
        }
        csi wide = 0; for (csi L = 0; L <= bmax; L++) if (lay[L] >= 4096) wide += lay[L];
        printf("# reduction comp %lld: n=%lld eliminated=%lld (blocks %lld in %lld blocks, tail %lld) | tail layers: forward %lld, backward %lld; "
               "records/layer %.0f; share of tail in layers >=4096 wide: %.1f%%\n",
               (long long)c, (long long)r->n, (long long)r->count, (long long)inblocks, (long long)(blocked ? r->blocks : 0), (long long)g->count,
               (long long)(fmax + 1), (long long)(bmax + 1), (double)g->count / (double)(bmax + 1), 100.0 * (double)wide / (double)(g->count ? g->count : 1));
        free(lev); free(elim); free(lay);
    }
    fflush(stdout);

    /* Packed (internal) order: pperm[p] = original index. */
    csi *pperm = malloc((size_t)n * sizeof(csi));
    double *hp = malloc((size_t)n * 8), *ip = malloc((size_t)n * 8), *v = calloc((size_t)n, 8), *bk = malloc((size_t)n * 8);
    unsigned char *gp = malloc((size_t)n);
    double *vg = calloc((size_t)n, 8), *bg = malloc((size_t)n * 8), *hg = malloc((size_t)n * 8);
    if (!pperm || !hp || !ip || !v || !bk || !gp || !vg || !bg || !hg) { puts("alloc failed"); return 1; }
    if (vsdlss_m3_export_packed_permutation(f, pperm, n) != VSDLSS_OK) { puts("pperm failed"); return 1; }
    for (csi p = 0; p < n; p++) { csi g = pperm[p]; hp[p] = cap[g] / h; ip[p] = iamp[g]; gp[p] = grp[g]; }
    for (csi i = 0; i < n; i++) hg[i] = cap[i] / h;
    /* 8 load groups: pulses, steps and piecewise-linear ramps with different phases */
    double wave[8];
#define WAVES(k) do { double tk_ = (double)(k); \
        for (int q_ = 0; q_ < 8; q_++) { double ph_ = fmod(tk_ + 7.0 * q_, 40.0); \
            wave[q_] = q_ < 3 ? (ph_ < 5 ? 1.0 : 0.05) : q_ < 5 ? (tk_ >= 20.0 * (q_ - 2) ? 1.0 : 0.1) : (ph_ < 20 ? ph_ / 20 : (40 - ph_) / 20); } } while (0)
    int nth = threads;
    double *trhs = malloc((size_t)steps * 8), *tsol = malloc((size_t)steps * 8), *tstep = malloc((size_t)steps * 8);
    double *trhs_g = malloc((size_t)steps * 8), *tsol_g = malloc((size_t)steps * 8);
    int bitwise_ok = 1; double worst_eta = 0;
    for (int k = 0; k < steps; k++) {
        WAVES(k);
        double t0 = now();
        /* packed flow */
        double *bb = alias ? v : bk;       /* alias: b_k overwrites v_{k-1} elementwise */
        VSDLSS_OMP(omp parallel for num_threads(nth) schedule(static))
        for (csi p = 0; p < n; p++) bb[p] = ip[p] * wave[gp[p]] + hp[p] * v[p];
        double t1 = now();
        if (vsdlss_m3_solve_packed(f, bb, v) != VSDLSS_OK) { printf("packed solve failed at step %d\n", k); return 1; }
        double t2 = now();
        trhs[k] = t1 - t0; tsol[k] = t2 - t1; tstep[k] = t2 - t0;
        /* reference flow in original numbering */
        t0 = now();
        VSDLSS_OMP(omp parallel for num_threads(nth) schedule(static))
        for (csi i = 0; i < n; i++) bg[i] = iamp[i] * wave[grp[i]] + hg[i] * vg[i];
        t1 = now();
        if (vsdlss_m3_solve(f, bg, vg) != VSDLSS_OK) { printf("solve failed at step %d\n", k); return 1; }
        t2 = now();
        trhs_g[k] = t1 - t0; tsol_g[k] = t2 - t1;
        if (compare_every > 0 && (k % compare_every == 0 || k == steps - 1)) {
            for (csi p = 0; p < n; p++) if (memcmp(&v[p], &vg[pperm[p]], 8)) { bitwise_ok = 0; break; }
            double eta; vsdlss_backward_error(A, vg, bg, &eta); if (eta > worst_eta) worst_eta = eta;
        }
    }
#define MED(a) (qsort(a, steps, 8, cmpd), a[steps / 2])
#define P95(a) (a[(int)(0.95 * (steps - 1))])
    double m1 = MED(trhs), q1 = P95(trhs), m2 = MED(tsol), q2 = P95(tsol), m3 = MED(tstep), q3 = P95(tstep);
    double m4 = MED(trhs_g), m5 = MED(tsol_g), q5 = P95(tsol_g);
    double vmin = 1e300, vmax = -1e300; for (csi p = 0; p < n; p++) { if (v[p] < vmin) vmin = v[p]; if (v[p] > vmax) vmax = v[p]; }
    /* FNV-1a over the bits of the final packed solution: equal across builds
     * and thread counts when the replay is bitwise reproducible. */
    unsigned long long hash = 1469598103934665603ULL;
    for (csi p = 0; p < n; p++) { unsigned long long u; memcpy(&u, &v[p], 8); hash = (hash ^ u) * 1099511628211ULL; }
    printf("# final solution hash %016llx\n", hash);
    printf("TRANSIENT steps=%d threads=%d | packed: rhs %.4f (p95 %.4f) + solve %.4f (p95 %.4f) = step %.4f (p95 %.4f) s | "
           "global: rhs %.4f + solve %.4f (p95 %.4f) s | total packed %.1f s | bitwise packed==global %d | worst berr %.1e | v range [%.3g, %.3g]\n",
           steps, threads, m1, q1, m2, q2, m3, q3, m4, m5, q5, m3 * steps, bitwise_ok, worst_eta, vmin, vmax);
    return 0;
}
