/* Cross-solver comparison on an identical SPD matrix:
 *   CHOLMOD 5.3 (SuiteSparse, supernodal, BLAS-3 via OpenBLAS)
 *   VSDLSS M3 (this project)
 * Same 7-point 3D Poisson grid, same right-hand side. */
#define _POSIX_C_SOURCE 200809L
#include "cholmod.h"
#include "../src/vsdlss_m3_internal.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+1e-9*t.tv_nsec;}

/* Upper-triangular CSC, identical numbers to test/bench_sn.c */
static void build(long k,long *np,long **Pp,long **Ii,double **Xx,long *nzp)
{
    long n=k*k*k,p=0;
    long *P=malloc((size_t)(n+1)*sizeof(long));
    long *I=malloc((size_t)4*n*sizeof(long));
    double *X=malloc((size_t)4*n*sizeof(double));
    for(long z=0;z<k;z++)for(long y=0;y<k;y++)for(long x=0;x<k;x++){
        long j=(z*k+y)*k+x;P[j]=p;
        if(x){I[p]=j-1;X[p++]=-1;}
        if(y){I[p]=j-k;X[p++]=-1;}
        if(z){I[p]=j-k*k;X[p++]=-1;}
        I[p]=j;X[p++]=6.5;
    }
    P[n]=p;*np=n;*Pp=P;*Ii=I;*Xx=X;*nzp=p;
}

int main(int argc,char**argv)
{
    long k=argc>1?atol(argv[1]):22;
    int threads=argc>2?atoi(argv[2]):1;
    int reps=argc>3?atoi(argv[3]):3;
    int ord=argc>4?atoi(argv[4]):5;
    long n,nz,*P,*I;double *X;
    build(k,&n,&P,&I,&X,&nz);
    printf("# grid %ldx%ldx%ld  n=%ld  nnz(upper)=%ld  threads=%d\n",k,k,k,n,nz,threads);

    double *truth=malloc((size_t)n*8),*rhs=malloc((size_t)n*8);
    for(long i=0;i<n;i++)truth[i]=(i%2?-1:1)*(0.5+(double)i*1e-4);

    /* ---- VSDLSS ---- */
    vsdlss *A=vsdlss_spalloc(n,n,nz,1,0);
    for(long j=0;j<=n;j++)A->p[j]=P[j];
    for(long q=0;q<nz;q++){A->i[q]=I[q];A->x[q]=X[q];}
    if(vsdlss_spmv_sym_upper(A,truth,rhs)!=VSDLSS_OK)return 1;
    vsdlss_set_num_threads(threads);
    {
        double bf=1e30,bs=1e30,eta=0;vsdlss_m3_factor *f=NULL;double *x=malloc((size_t)n*8);
        for(int r=-1;r<reps;r++){
            double t=now();
            if(vsdlss_factorize_m3(A,ord,&f)!=VSDLSS_OK){puts("vsdlss factor failed");return 1;}
            double a=now()-t;t=now();
            if(vsdlss_m3_solve(f,rhs,x)!=VSDLSS_OK)return 1;
            double b=now()-t;
            if(r>=0){if(a<bf)bf=a;if(b<bs)bs=b;}
            if(r<reps-1){vsdlss_m3_factor_free(f);f=NULL;}
        }
        vsdlss_backward_error(A,x,rhs,&eta);
        double err=0;for(long i=0;i<n;i++){double d=fabs(x[i]-truth[i]);if(d>err)err=d;}
        long lnnz=0,sn=0;
        for(csi ci=0;ci<f->count;ci++)if(f->component[ci].numeric){
            lnnz+=f->component[ci].numeric->l_nnz;sn+=f->component[ci].numeric->count;}
        printf("VSDLSS-M3(p%d) factor %8.4f s   solve %8.5f s   max_err %.3e  bwd_err %.3e\n",ord,bf,bs,err,eta);
        printf("            nnz(L)=%ld  supernodes=%ld\n",lnnz,sn);
        vsdlss_m3_factor_free(f);free(x);
    }

    /* ---- CHOLMOD ---- */
    {
        cholmod_common c;cholmod_l_start(&c);c.useGPU=0;c.nthreads_max=threads;
        cholmod_sparse *S=cholmod_l_allocate_sparse((size_t)n,(size_t)n,(size_t)nz,1,1,1,CHOLMOD_REAL,&c);
        long *Sp=S->p,*Si=S->i;double *Sx=S->x;
        for(long j=0;j<=n;j++)Sp[j]=P[j];
        for(long q=0;q<nz;q++){Si[q]=I[q];Sx[q]=X[q];}
        cholmod_dense *B=cholmod_l_allocate_dense((size_t)n,1,(size_t)n,CHOLMOD_REAL,&c);
        memcpy(B->x,rhs,(size_t)n*8);
        double ba=1e30,bf=1e30,bs=1e30;cholmod_factor *L=NULL;cholmod_dense *Xd=NULL;
        for(int r=-1;r<reps;r++){
            double t=now();
            L=cholmod_l_analyze(S,&c);
            double a=now()-t;t=now();
            if(!cholmod_l_factorize(S,L,&c)){puts("cholmod factorize failed");return 1;}
            double b=now()-t;t=now();
            Xd=cholmod_l_solve(CHOLMOD_A,L,B,&c);
            double d=now()-t;
            if(r>=0){if(a<ba)ba=a;if(b<bf)bf=b;if(d<bs)bs=d;}
            if(r<reps-1){cholmod_l_free_factor(&L,&c);cholmod_l_free_dense(&Xd,&c);}
        }
        double *xs=Xd->x,err=0;
        for(long i=0;i<n;i++){double e=fabs(xs[i]-truth[i]);if(e>err)err=e;}
        /* residual ||Ax-b||_inf / (||A||_inf ||x||_inf + ||b||_inf) */
        double eta=0;{
            double *res=malloc((size_t)n*8);
            vsdlss_spmv_sym_upper(A,xs,res);
            double rn=0,bn=0,xn=0;
            for(long i=0;i<n;i++){double d=fabs(res[i]-rhs[i]);if(d>rn)rn=d;
                if(fabs(rhs[i])>bn)bn=fabs(rhs[i]);if(fabs(xs[i])>xn)xn=fabs(xs[i]);}
            eta=rn/(8.5*xn+bn);free(res);
        }
        printf("CHOLMOD     analyze %6.4f s  factor %8.4f s   solve %8.5f s   max_err %.3e  bwd_err %.3e\n",
               ba,ord,bf,bs,err,eta);
        printf("            nnz(L)=%.0f  flops=%.3f GF  %s  ordering=%d  supernodes=%ld\n",
               c.lnz,c.fl*1e-9,L->is_super?"supernodal":"simplicial",
               (int)c.method[c.selected].ordering,L->is_super?(long)L->nsuper:0L);
        printf("            factor rate = %.2f GF/s\n",c.fl/bf*1e-9);
        cholmod_l_free_factor(&L,&c);cholmod_l_free_dense(&Xd,&c);
        /* Same library, same ordering, BLAS-3 turned off: isolates what the
         * supernodal/BLAS-3 kernels are worth from ordering and symbolic. */
        c.supernodal=CHOLMOD_SIMPLICIAL;
        double sf=1e30;
        for(int r=-1;r<reps;r++){
            L=cholmod_l_analyze(S,&c);
            double t=now();
            if(!cholmod_l_factorize(S,L,&c))return 1;
            double b=now()-t;
            if(r>=0&&b<sf)sf=b;
            cholmod_l_free_factor(&L,&c);
        }
        printf("CHOLMOD/simplicial (no BLAS-3, same ordering)  factor %8.4f s   %.2f GF/s\n",
               sf,c.fl/sf*1e-9);
        cholmod_l_free_sparse(&S,&c);cholmod_l_free_dense(&B,&c);cholmod_l_finish(&c);
    }
    return 0;
}
