#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <time.h>
#include <string.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static int compare(const void *a,const void *b){double x=*(const double *)a,y=*(const double *)b;return (x>y)-(x<y);}
static double median(double t[5]){qsort(t,5,sizeof(double),compare);return t[2];}
int main(void)
{
    const csi rows=1024,w=256;size_t bytes=(size_t)rows*w*sizeof(double);
    double *a=calloc((size_t)rows*w,sizeof(double)),*b=malloc(bytes),*reference=malloc(bytes);
    if(!a||!b||!reference)return 1;
    for(csi j=0;j<w;j++)for(csi i=j;i<rows;i++)a[j*rows+i]=i==j?100:0.001/(1+i-j);
    puts("mode\tn\twidth\tthreads\tmedian_seconds\tspeedup\tmax_team");
    double base=0;
    for(int nt=1;nt<=4;nt*=2){
        if(vsdlss_set_num_threads(nt)!=VSDLSS_OK)return 1;
        double times[5];
        for(int r=-1;r<5;r++){
            memcpy(b,a,bytes);double start=now();
            if(vsdlss_panel_factor(b,rows,w)!=VSDLSS_OK)return 1;
            double elapsed=now()-start;
            if(r>=0)times[r]=elapsed;
        }
        if(nt==1)memcpy(reference,b,bytes);else if(memcmp(reference,b,bytes))return 2;
        double elapsed=median(times);if(nt==1)base=elapsed;
        printf("panel_factor\t%lld\t%lld\t%d\t%.9f\t%.3f\t%d\n",(long long)rows,(long long)w,nt,elapsed,base/elapsed,vsdlss_parallel_last_team_size());
    }
    free(a);free(b);free(reference);
    const csi n=384;vsdlss *A=vsdlss_spalloc(n,n,n*(n+1)/2,1,0);if(!A)return 1;
    csi p=0;double *truth=malloc(n*8),*rhs=malloc(n*8),*x=malloc(n*8);if(!truth||!rhs||!x)return 1;
    for(csi j=0;j<n;j++){
        A->p[j]=p;truth[j]=(j%2?-1:1)*(0.5+j*0.01);
        for(csi i=0;i<=j;i++){A->i[p]=i;A->x[p++]=i==j?100:0.001/(1+j-i);}
    }
    A->p[n]=p;if(vsdlss_spmv_sym_upper(A,truth,rhs)!=VSDLSS_OK)return 1;
    for(int nt=1;nt<=4;nt*=2){
        if(vsdlss_set_num_threads(nt)!=VSDLSS_OK)return 1;
        double times[5];
        for(int r=0;r<5;r++){
            vsdlss_m4_factor *f=NULL;double start=now();
            if(vsdlss_factorize_m4_ex(A,2,4*1024*1024,NULL,96,&f)!=VSDLSS_OK)return 1;
            if(vsdlss_m4_solve(f,rhs,x)!=VSDLSS_OK)return 1;
            times[r]=now()-start;
            for(csi i=0;i<n;i++)if(!isfinite(x[i])||fabs(x[i]-truth[i])>1e-11)return 2;
            vsdlss_m4_factor_free(f);
        }
        double elapsed=median(times);if(nt==1)base=elapsed;
        printf("m4_end_to_end\t%lld\t96\t%d\t%.9f\t%.3f\t%d\n",(long long)n,nt,elapsed,base/elapsed,vsdlss_parallel_last_team_size());
    }
    vsdlss_spfree(A);free(truth);free(rhs);free(x);return 0;
}
