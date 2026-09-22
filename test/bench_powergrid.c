/* Power-grid-like benchmark with a controlled degree distribution.
 *
 *   ./bench_powergrid [target_nnz_full] [order] [threads] [nrhs] [shuffle]
 *
 * Structure (planar, like one routed metal layer): a G x G grid of junctions;
 * every grid edge is a wire of 7 series segments (7 chain nodes); two chain
 * nodes of every wire carry a pendant tap (degree-1 node); every junction
 * carries one pendant; junctions with even (x, y) get one diagonal strap.
 * Resulting node degrees: about 25% degree 1, 50% degree 2, 20% degree 3 and
 * 5% degree 4..6 (never above 6).  Every 8th junction pendant is a supply pad
 * with a conductance to ground, which makes the matrix SPD.
 *
 * target_nnz_full counts both triangles plus the diagonal (default 2e7).
 * Node numbering is randomly shuffled unless shuffle=0. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <string.h>
#include <time.h>
#include <sys/resource.h>
static long faults(void){struct rusage u;getrusage(RUSAGE_SELF,&u);return u.ru_minflt;}

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static uint64_t rng=88172645463325252ULL;
static uint64_t rnd(void){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static double uni(double a,double b){return a+(b-a)*(double)(rnd()>>11)*(1.0/9007199254740992.0);}

typedef struct { csi a,b; double g; } edge;

int main(int argc,char **argv)
{
    double target=argc>1?atof(argv[1]):2e7;
    int order=argc>2?atoi(argv[2]):5, threads=argc>3?atoi(argv[3]):1;
    int nrhs=argc>4?atoi(argv[4]):4, shuffle=argc>5?atoi(argv[5]):1;
    const csi chain=7;
    /* nnz_full ~ n + 2E with n ~ 20 J and E ~ 21.25 J  ->  ~62.5 J */
    csi G=(csi)(sqrt(target/62.5)+0.5); if(G<3)G=3;
    csi J=G*G, wires=2*G*(G-1), diags=((G)/2)*((G)/2);
    csi n_est=J+wires*chain+wires*2+J, e_est=wires*(chain+1)+wires*2+J+diags;
    edge *E=malloc((size_t)e_est*sizeof(*E));
    double *ground=calloc((size_t)n_est,sizeof(double));
    if(!E||!ground){puts("alloc failed");return 1;}
    csi n=J,m=0;
#define JID(x,y) ((y)*G+(x))
    for(csi y=0;y<G;y++)for(csi x=0;x<G;x++)for(int dir=0;dir<2;dir++){
        csi x2=x+(dir==0),y2=y+(dir==1);
        if(x2>=G||y2>=G)continue;
        csi prev=JID(x,y);
        for(csi c=0;c<chain;c++){
            csi node=n++;
            E[m++]=(edge){prev,node,uni(0.5,2.0)};
            if(c==1||c==4){csi tap=n++;E[m++]=(edge){node,tap,uni(0.05,0.5)};
                ground[tap]=1e-3; /* load/decap to ground at taps */}
            prev=node;
        }
        E[m++]=(edge){prev,JID(x2,y2),uni(0.5,2.0)};
    }
    for(csi y=0;y<G;y++)for(csi x=0;x<G;x++){
        csi p=n++;E[m++]=(edge){JID(x,y),p,uni(0.5,2.0)};
        if((x%8==0)&&(y%8==0))ground[p]=10.0;   /* supply pad */
        if(x%2==0&&y%2==0&&x+1<G&&y+1<G)E[m++]=(edge){JID(x,y),JID(x+1,y+1),uni(0.5,2.0)};
    }
    csi *perm=malloc((size_t)n*sizeof(csi)),*deg=calloc((size_t)n,sizeof(csi));
    for(csi i=0;i<n;i++)perm[i]=i;
    if(shuffle)for(csi i=n-1;i>0;i--){csi j=(csi)(rnd()%(uint64_t)(i+1)),t=perm[i];perm[i]=perm[j];perm[j]=t;}
    for(csi k=0;k<m;k++){deg[E[k].a]++;deg[E[k].b]++;}
    csi hist[8]={0},maxdeg=0;
    for(csi i=0;i<n;i++){if(deg[i]>maxdeg)maxdeg=deg[i];hist[deg[i]<7?deg[i]:7]++;}
    /* upper CSC: entry (min,max) stored in column max */
    vsdlss *A=vsdlss_spalloc(n,n,n+m,1,0);
    double *diag=calloc((size_t)n,sizeof(double));
    csi *cnt=calloc((size_t)n+1,sizeof(csi));
    if(!A||!diag||!cnt){puts("alloc failed");return 1;}
    for(csi k=0;k<m;k++){csi a=perm[E[k].a],b=perm[E[k].b];cnt[(a>b?a:b)]++;
        diag[perm[E[k].a]]+=E[k].g;diag[perm[E[k].b]]+=E[k].g;}
    for(csi i=0;i<n;i++)diag[perm[i]]+=ground[i];
    A->p[0]=0;for(csi j=0;j<n;j++)A->p[j+1]=A->p[j]+cnt[j]+1;
    for(csi j=0;j<n;j++)cnt[j]=A->p[j];
    for(csi k=0;k<m;k++){csi a=perm[E[k].a],b=perm[E[k].b],col=a>b?a:b,row=a<b?a:b;
        A->i[cnt[col]]=row;A->x[cnt[col]++]=-E[k].g;}
    for(csi j=0;j<n;j++){A->i[cnt[j]]=j;A->x[cnt[j]++]=diag[j];}
    free(E);free(ground);free(perm);free(deg);free(diag);free(cnt);
    csi nnz_full=2*m+n;
    printf("# powergrid G=%lld n=%lld edges=%lld nnz_upper=%lld nnz_full=%lld max_degree=%lld\n",
           (long long)G,(long long)n,(long long)m,(long long)(m+n),(long long)nnz_full,(long long)maxdeg);
    printf("# degree share: d1=%.1f%% d2=%.1f%% d3=%.1f%% d4=%.1f%% d5=%.1f%% d6=%.1f%%\n",
           100.0*hist[1]/n,100.0*hist[2]/n,100.0*hist[3]/n,100.0*hist[4]/n,100.0*hist[5]/n,100.0*hist[6]/n);

    if(vsdlss_set_num_threads(threads)!=VSDLSS_OK){puts("bad thread count");return 1;}
    /* Stage view of preprocessing (same calls the M3 facade makes). */
    {
        vsdlss *N=NULL,*local=NULL;vsdlss_components *c=NULL;vsdlss_reduction *r=NULL;
        double t0=now();
        if(vsdlss_normalize_upper(A,&N)||vsdlss_components_build_normalized(N,&c))return 1;
        double t1=now();
        csi big=0;for(csi k=0;k<c->count;k++)if(c->offset[k+1]-c->offset[k]>c->offset[big+1]-c->offset[big])big=k;
        if(vsdlss_component_extract_normalized(N,c,big,&local))return 1;
        double t15=now();long f0=faults();
        if(vsdlss_reduce(local,&r))return 1;
        double t2=now();long f1=faults();
        csi d[4]={0};for(csi k=0;k<r->count;k++)if(r->records[k].degree<4)d[r->records[k].degree]++;
        printf("# components=%lld largest=%lld  eliminated d0=%lld d1=%lld d2=%lld d3=%lld  core_n=%lld (%.2f%% of n) core_nnz_upper=%lld\n",
               (long long)c->count,(long long)(c->offset[big+1]-c->offset[big]),(long long)d[0],(long long)d[1],
               (long long)d[2],(long long)d[3],(long long)r->core_n,100.0*r->core_n/n,
               (long long)(r->core?r->core->p[r->core_n]:0));
        printf("# stage normalize+components=%.3fs extract=%.3fs reduce=%.3fs\n",t1-t0,t15-t1,t2-t15);
        printf("# reduce minor page faults=%ld\n",f1-f0);
        vsdlss_reduction_free(r);vsdlss_spfree(local);vsdlss_components_free(c);vsdlss_spfree(N);
    }
    double *b=malloc((size_t)n*nrhs*sizeof(double)),*x=malloc((size_t)n*nrhs*sizeof(double));
    if(!b||!x){puts("alloc failed");return 1;}
    for(csi i=0;i<(csi)n*nrhs;i++)b[i]=uni(-1e-3,0);   /* current sinks */
    vsdlss_m3_factor *f=NULL;
    double t=now();
    vsdlss_status st=vsdlss_factorize_m3(A,order,&f);
    double tf=now()-t;
    if(st!=VSDLSS_OK){printf("factor failed: %s\n",vsdlss_status_string(st));return 1;}
    csi lnz=0;for(csi k=0;k<f->count;k++)if(f->component[k].numeric)lnz+=f->component[k].numeric->l_nnz;
    t=now();st=vsdlss_m3_solve(f,b,x);double ts=now()-t;
    if(st!=VSDLSS_OK){printf("solve failed\n");return 1;}
    double eta;vsdlss_backward_error(A,x,b,&eta);
    t=now();st=vsdlss_m3_solve_many(f,nrhs,b,n,x,n);double tm=now()-t;
    if(st!=VSDLSS_OK){printf("solve_many failed\n");return 1;}
    printf("order=%d threads=%d factor=%.3fs solve1=%.3fs solve%d=%.3fs core_nnzL=%.3gM backward_error=%.2e\n",
           order,threads,tf,ts,nrhs,tm,lnz*1e-6,eta);
    vsdlss_m3_factor_free(f);vsdlss_spfree(A);free(b);free(x);
    return 0;
}
