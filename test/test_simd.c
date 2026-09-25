/* The AVX2 solve kernels against the scalar loops they replace: bitwise
 * equal results for random sizes (all remainders), contiguous and scattered
 * row sets, and non-finite detection.  Skips when AVX2 is unavailable. */
#include "../src/vsdlss_simd.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(e) do{if(!(e)){fprintf(stderr,"FAIL simd %d: %s\n",__LINE__,#e);return 1;}}while(0)
static uint64_t s=0x243F6A8885A308D3ULL;
static uint64_t rnd(void){s^=s<<13;s^=s>>7;s^=s<<17;return s;}
static double uni(void){return (double)(rnd()>>11)*(1.0/9007199254740992.0)*2-1;}

int main(void)
{
    if(!vsdlss_simd_enabled()){puts("test_simd: AVX2 unavailable or VSDLSS_SIMD=0, skipped");return 0;}
    enum{N=4096};
    double *x=malloc(N*8),*y=malloc(N*8),*a=malloc(N*64*8),*xs=malloc(64*8),*v=malloc(8*8),*w=malloc(8*8);
    csi *R=malloc(N*sizeof(csi));
    CHECK(x&&y&&a&&xs&&v&&w&&R);
    for(int trial=0;trial<2000;trial++){
        csi ws=1+(csi)(rnd()%40), n=(csi)(rnd()%70), rs=n+ws+(csi)(rnd()%5);
        for(csi i=0;i<rs*ws;i++)a[i]=uni();
        for(csi j=0;j<ws;j++)xs[j]=uni()*3;
        /* rows: ascending, runs of consecutive indices mixed with gaps */
        csi at=(csi)(rnd()%8);
        for(csi r=0;r<n;r++){R[r]=at; at+=(rnd()%3==0)?1+(csi)(rnd()%9):1;}
        csi r0=n?(csi)(rnd()%(n+1)):0, r1=r0+(n>r0?(csi)(rnd()%(n-r0+1)):0);
        for(csi i=0;i<N;i++)x[i]=y[i]=uni();
        /* block update */
        vsdlss_simd_block_update(a,rs,ws,xs,R,r0,r1,x);
        for(csi j=0;j<ws;j++){const double *col=a+j*rs;for(csi r=r0;r<r1;r++)y[R[r]]-=col[r]*xs[j];}
        CHECK(memcmp(x,y,N*8)==0);
        /* contiguous targets, columns [j0,j1) */
        csi j0=(csi)(rnd()%ws), j1=j0+(csi)(rnd()%(ws-j0+1));
        vsdlss_simd_block_update_contig(a,rs,j0,j1,xs,r0,r1,x+5);
        for(csi r=r0;r<r1;r++){double t=y[5+r];for(csi j=j0;j<j1;j++)t-=a[j*rs+r]*xs[j];y[5+r]=t;}
        CHECK(memcmp(x,y,N*8)==0);
        /* axpy */
        double sc=uni();
        vsdlss_simd_axpy_neg(x+3,a+1,sc,n);
        for(csi i=0;i<n;i++)y[3+i]-=a[1+i]*sc;
        CHECK(memcmp(x,y,N*8)==0);
        /* four dot products */
        if(ws>=4){
            for(int k=0;k<4;k++)v[k]=w[k]=uni();
            int bad=vsdlss_simd_dot4(a,rs,x,n,v);
            for(int k=0;k<4;k++){double t=w[k];for(csi r=0;r<n;r++)t-=a[k*rs+r]*x[r];w[k]=t;}
            CHECK(memcmp(v,w,4*8)==0 && bad==0);
        }
    }
    /* non-finite results are reported */
    for(int k=0;k<64*4;k++)a[k]=1;
    for(int k=0;k<8;k++)x[k]=1;
    x[3]=INFINITY; v[0]=v[1]=v[2]=v[3]=0;
    CHECK(vsdlss_simd_dot4(a,64,x,8,v)==1);
    x[3]=1; v[0]=v[1]=v[2]=v[3]=0;
    CHECK(vsdlss_simd_dot4(a,64,x,8,v)==0);
    free(x);free(y);free(a);free(xs);free(v);free(w);free(R);
    puts("test_simd: kernels bitwise equal to the scalar loops (2000 random cases)");
    return 0;
}
