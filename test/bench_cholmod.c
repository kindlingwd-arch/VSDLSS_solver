/* CHOLMOD (supernodal, AMD) on a dumped system: n, nnz, p[n+1], i[nnz],
 * x[nnz] (upper CSC, int64), b[n].
 *   ./bench_cholmod file [reps]
 * BLAS threads follow OPENBLAS_NUM_THREADS / OMP_NUM_THREADS. */
#define _POSIX_C_SOURCE 200809L
#include "cholmod.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static int cmpd(const void *a,const void *b){double x=*(const double*)a,y=*(const double*)b;return (x>y)-(x<y);}

int main(int argc,char **argv)
{
    if(argc<2){fprintf(stderr,"usage: bench_cholmod file [reps]\n");return 1;}
    int reps=argc>2?atoi(argv[2]):15;
    FILE *f=fopen(argv[1],"rb"); if(!f){perror("open");return 1;}
    int64_t n,nz; if(fread(&n,8,1,f)!=1||fread(&nz,8,1,f)!=1)return 1;
    cholmod_common c; cholmod_l_start(&c);
    c.supernodal=CHOLMOD_SUPERNODAL; c.nmethods=1; c.method[0].ordering=CHOLMOD_AMD; c.postorder=1;
    cholmod_sparse *A=cholmod_l_allocate_sparse(n,n,nz,1,1,1,CHOLMOD_REAL,&c);
    cholmod_dense *B=cholmod_l_allocate_dense(n,1,n,CHOLMOD_REAL,&c);
    if(!A||!B){puts("alloc failed");return 1;}
    if(fread(A->p,8,n+1,f)!=(size_t)(n+1)||fread(A->i,8,nz,f)!=(size_t)nz||fread(A->x,8,nz,f)!=(size_t)nz||
       fread(B->x,8,n,f)!=(size_t)n){puts("short read");return 1;}
    fclose(f);
    double t=now(); cholmod_factor *L=cholmod_l_analyze(A,&c); double ta=now()-t;
    if(!L){puts("analyze failed");return 1;}
    t=now(); int ok=cholmod_l_factorize(A,L,&c); double tf=now()-t;
    if(!ok||c.status!=CHOLMOD_OK){printf("factorize failed status=%d\n",c.status);return 1;}
    /* Repeated solves reusing X, Y, E. */
    cholmod_dense *X=NULL,*Y=NULL,*E=NULL; double ts[512]; if(reps>512)reps=512;
    for(int r=-2;r<reps;r++){ t=now(); cholmod_l_solve2(CHOLMOD_A,L,B,NULL,&X,NULL,&Y,&E,&c); if(r>=0)ts[r]=now()-t; }
    qsort(ts,reps,sizeof(double),cmpd);
    /* Backward error |b - A x|_inf / (|A|_inf |x|_inf + |b|_inf). */
    double *x=X->x,*b=B->x,*r=calloc(n,8),*rowsum=calloc(n,8); int64_t *Ap=A->p,*Ai=A->i; double *Ax=A->x;
    for(int64_t j=0;j<n;j++)for(int64_t p=Ap[j];p<Ap[j+1];p++){int64_t i=Ai[p];double v=Ax[p];
        r[i]+=v*x[j];rowsum[i]+=fabs(v); if(i!=j){r[j]+=v*x[i];rowsum[j]+=fabs(v);}}
    double rn=0,an=0,xn=0,bn=0;
    for(int64_t i=0;i<n;i++){double d=fabs(b[i]-r[i]);if(d>rn)rn=d;if(rowsum[i]>an)an=rowsum[i];if(fabs(x[i])>xn)xn=fabs(x[i]);if(fabs(b[i])>bn)bn=fabs(b[i]);}
    printf("CHOLMOD n=%lld nnzA=%lld | analyze(AMD)=%.3fs factorize=%.3fs total=%.3fs | nnz(L)=%.2fM flops=%.2fG (%.2f GF/s) | solve med=%.4fs min=%.4fs | backward_err=%.1e\n",
        (long long)n,(long long)nz,ta,tf,ta+tf,c.lnz*1e-6,c.fl*1e-9,c.fl*1e-9/tf,ts[reps/2],ts[0],rn/(an*xn+bn));
    cholmod_l_free_dense(&X,&c);cholmod_l_free_dense(&Y,&c);cholmod_l_free_dense(&E,&c);
    cholmod_l_free_factor(&L,&c);cholmod_l_free_sparse(&A,&c);cholmod_l_free_dense(&B,&c);cholmod_l_finish(&c);
    return 0;
}
