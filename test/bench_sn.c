/* Scaling benchmark for the in-memory M3 supernodal path on a 3D Poisson
 * grid: one connected component, so component-level parallelism gives
 * nothing and all speedup must come from the numeric kernels. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <time.h>
#include <string.h>

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}

/* Upper-triangular CSC of the 7-point Laplacian on a k^3 grid, shifted. */
static vsdlss *poisson3d(csi k)
{
    csi n=k*k*k,nz=0,j,p=0;
    vsdlss *A=vsdlss_spalloc(n,n,4*n,1,0);
    if(!A)return NULL;
    for(csi z=0;z<k;z++)for(csi y=0;y<k;y++)for(csi x=0;x<k;x++){
        j=(z*k+y)*k+x;A->p[j]=p;
        if(x)  {A->i[p]=j-1;A->x[p++]=-1;}
        if(y)  {A->i[p]=j-k;A->x[p++]=-1;}
        if(z)  {A->i[p]=j-k*k;A->x[p++]=-1;}
        A->i[p]=j;A->x[p++]=6.5;
    }
    A->p[n]=p;nz=p;(void)nz;
    return A;
}

int main(int argc,char **argv)
{
    csi k=argc>1?atoll(argv[1]):20;
    int maxthreads=argc>2?atoi(argv[2]):4;
    int nrhs=argc>3?atoi(argv[3]):8;
    vsdlss *A=poisson3d(k);
    if(!A){puts("alloc failed");return 1;}
    csi n=A->n;
    double *truth=malloc((size_t)n*8),*rhs=malloc((size_t)n*8),*x=malloc((size_t)n*8);
    double *many=malloc((size_t)n*(size_t)nrhs*8),*manyx=malloc((size_t)n*(size_t)nrhs*8);
    if(!truth||!rhs||!x||!many||!manyx)return 1;
    for(csi i=0;i<n;i++)truth[i]=(i%2?-1:1)*(0.5+(double)i*1e-4);
    if(vsdlss_spmv_sym_upper(A,truth,rhs)!=VSDLSS_OK)return 1;
    for(int r=0;r<nrhs;r++)memcpy(many+(size_t)r*n,rhs,(size_t)n*8);

    printf("# grid %lldx%lldx%lld  n=%lld\n",(long long)k,(long long)k,(long long)k,(long long)n);
    puts("stage\tthreads\tdag\tseconds\tspeedup\tmax_team\tmax_err");
    double base_f=0,base_s=0,base_m=0;
    for(int dag=0;dag<2;dag++){
      base_f=base_s=base_m=0;
      for(int nt=1;nt<=maxthreads;nt*=2){
        if(vsdlss_set_num_threads(nt)!=VSDLSS_OK)return 1;
        vsdlss_set_dag_enabled(dag);
        vsdlss_m3_factor *f=NULL;
        double tf=1e30,ts=1e30,tm=1e30,err=0;
        for(int rep=-1;rep<3;rep++){
            double t0=now();
            if(vsdlss_factorize_m3(A,0,&f)!=VSDLSS_OK){puts("factor failed");return 1;}
            double a1=now()-t0;
            t0=now();
            if(vsdlss_m3_solve(f,rhs,x)!=VSDLSS_OK)return 1;
            double a2=now()-t0;
            t0=now();
            if(vsdlss_m3_solve_many(f,nrhs,many,n,manyx,n)!=VSDLSS_OK)return 1;
            double a3=now()-t0;
            if(rep>=0){if(a1<tf)tf=a1;if(a2<ts)ts=a2;if(a3<tm)tm=a3;}
            if(rep<2)vsdlss_m3_factor_free(f),f=NULL;
        }
        for(csi i=0;i<n;i++){double d=fabs(x[i]-truth[i]);if(d>err)err=d;}
        if(nt==1){base_f=tf;base_s=ts;base_m=tm;}
        printf("factor\t%d\t%d\t%.6f\t%.3f\t%d\t%.3e\n",nt,dag,tf,base_f/tf,vsdlss_parallel_last_team_size(),err);
        printf("solve1\t%d\t%d\t%.6f\t%.3f\t%d\t%.3e\n",nt,dag,ts,base_s/ts,vsdlss_parallel_last_team_size(),err);
        printf("solve%d\t%d\t%d\t%.6f\t%.3f\t%d\t%.3e\n",nrhs,nt,dag,tm,base_m/tm,vsdlss_parallel_last_team_size(),err);
        vsdlss_m3_factor_free(f);
      }
    }
    vsdlss_spfree(A);free(truth);free(rhs);free(x);free(many);free(manyx);return 0;
}
