#define _POSIX_C_SOURCE 200809L
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double stamp(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return (double)t.tv_sec+t.tv_nsec*1e-9; }
int main(void) {
    const size_t n=32000000;
    double *a=malloc(n*sizeof(double)),*b=malloc(n*sizeof(double)),*c=malloc(n*sizeof(double));
    if(!a||!b||!c) return 1;
    for(int threads=1;threads<=8;threads*=2) {
        omp_set_num_threads(threads);
        #pragma omp parallel for schedule(static)
        for(size_t i=0;i<n;i++) {a[i]=0.;b[i]=(double)i*1e-7;c[i]=1.;}
        double best=1e9;
        for(int rep=0;rep<6;rep++) {
            double t=stamp();
            #pragma omp parallel for schedule(static)
            for(size_t i=0;i<n;i++) a[i]=b[i]+1.1*c[i];
            double d=stamp()-t; if(rep>0&&d<best)best=d;
        }
        printf("threads=%d triad=%.2f GB/s seconds=%.5f check=%.3f\n",threads,3.*n*sizeof(double)/best/1e9,best,a[n/2]);
    }
    free(a);free(b);free(c);
}
