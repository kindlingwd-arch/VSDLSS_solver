/* Dense kernels: VSDLSS (vsdlss_gemm_nt_sub, vsdlss_dense_potrf_panel)
 * against OpenBLAS (dgemm, dpotrf + dtrsm) on supernode-like shapes. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_dense.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void dgemm_(const char*,const char*,const int*,const int*,const int*,const double*,const double*,const int*,
            const double*,const int*,const double*,double*,const int*);
void dpotrf_(const char*,const int*,double*,const int*,int*);
void dtrsm_(const char*,const char*,const char*,const char*,const int*,const int*,const double*,
            const double*,const int*,double*,const int*);

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static double rnd(void){return (double)rand()/RAND_MAX-0.5;}

int main(void)
{
    /* GEMM C(m x n) -= A(m x k) B(n x k)^T  -- the supernodal update shape */
    int shapes[][3]={{64,64,64},{256,256,256},{512,512,512},{1024,1024,1024},
                     {2000,64,8},{2000,256,32},{4000,128,128},{1500,1500,64}};
    printf("GEMM  m     n     k   | vsdlss GF/s | OpenBLAS GF/s | speedup\n");
    for(size_t s=0;s<sizeof shapes/sizeof shapes[0];s++){
        int m=shapes[s][0],n=shapes[s][1],k=shapes[s][2];
        double *A=malloc((size_t)m*k*8),*B=malloc((size_t)n*k*8),*C=malloc((size_t)m*n*8);
        for(size_t i=0;i<(size_t)m*k;i++)A[i]=rnd();
        for(size_t i=0;i<(size_t)n*k;i++)B[i]=rnd();
        double fl=2.0*m*n*k; int reps=(int)(2e9/fl)+1; if(reps>200)reps=200;
        double best1=1e9,best2=1e9;
        for(int r=0;r<3;r++){
            memset(C,0,(size_t)m*n*8); double t=now();
            for(int q=0;q<reps;q++) vsdlss_gemm_nt_sub(m,n,k,A,m,B,n,C,m,VSDLSS_GEMM_FULL);
            t=(now()-t)/reps; if(t<best1)best1=t;
            double al=-1,be=1; t=now();
            for(int q=0;q<reps;q++) dgemm_("N","T",&m,&n,&k,&al,A,&m,B,&n,&be,C,&m);
            t=(now()-t)/reps; if(t<best2)best2=t;
        }
        printf("     %5d %5d %5d |   %6.2f    |    %6.2f     | %5.1fx\n",m,n,k,fl/best1*1e-9,fl/best2*1e-9,best1/best2);
        free(A);free(B);free(C);
    }
    /* Panel Cholesky: rows x width panel (diagonal block on top) */
    int pan[][2]={{128,64},{512,256},{1024,512},{1800,1800},{3000,1500},{4000,400}};
    printf("\nPANEL rows width | vsdlss GF/s | dpotrf+dtrsm GF/s | speedup | max|diff|\n");
    for(size_t s=0;s<sizeof pan/sizeof pan[0];s++){
        int r=pan[s][0],w=pan[s][1],e=r-w;
        double *M=malloc((size_t)r*w*8),*P1=malloc((size_t)r*w*8),*P2=malloc((size_t)r*w*8);
        /* SPD diag block: G G^T + r I, external rows random */
        double *G=malloc((size_t)w*w*8); for(size_t i=0;i<(size_t)w*w;i++)G[i]=rnd();
        for(int j=0;j<w;j++)for(int i=0;i<r;i++){
            double v=0; if(i<w){for(int p=0;p<w;p++)v+=G[i+p*w]*G[j+p*w]; if(i==j)v+=w;} else v=rnd();
            M[i+(size_t)j*r]=v; }
        double fl=(double)w*w*w/3.0+(double)e*w*w;
        double best1=1e9,best2=1e9;
        for(int q=0;q<3;q++){
            memcpy(P1,M,(size_t)r*w*8); double t=now();
            if(vsdlss_dense_potrf_panel(P1,r,w)!=VSDLSS_OK){puts("vsdlss potrf failed");return 1;}
            t=now()-t; if(t<best1)best1=t;
            memcpy(P2,M,(size_t)r*w*8); t=now(); int info=0; double one=1;
            dpotrf_("L",&w,P2,&r,&info); if(info){puts("dpotrf failed");return 1;}
            if(e>0) dtrsm_("R","L","T","N",&e,&w,&one,P2,&r,P2+w,&r);
            t=now()-t; if(t<best2)best2=t;
        }
        double md=0; for(int j=0;j<w;j++)for(int i=j;i<r;i++){double d=fabs(P1[i+(size_t)j*r]-P2[i+(size_t)j*r]);if(d>md)md=d;}
        printf("      %5d %5d |   %6.2f    |      %6.2f       | %5.1fx  | %.1e\n",r,w,fl/best1*1e-9,fl/best2*1e-9,best1/best2,md);
        free(M);free(P1);free(P2);free(G);
    }
    return 0;
}
