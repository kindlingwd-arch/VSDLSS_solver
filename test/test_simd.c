/* The AVX2 solve kernels against the scalar loops they replace: bitwise
 * equal results for random sizes (all remainders), contiguous and scattered
 * row sets, and non-finite detection.  Skips when AVX2 is unavailable. */
#include "../src/vsdlss_simd.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


#define CHECK(e) do{if(!(e)){fprintf(stderr,"FAIL simd %d: %s\n",__LINE__,#e);return 1;}}while(0)
static uint64_t s=0x243F6A8885A308D3ULL;
static uint64_t rnd(void){s^=s<<13;s^=s>>7;s^=s<<17;return s;}
static double uni(void){return (double)(rnd()>>11)*(1.0/9007199254740992.0)*2-1;}

/* Catches wrong AoS deinterleave, reordered subtractions, incomplete final
 * vectors, changed rounding, overwritten input, and missed nonfinite lanes.
 * The oracle is the existing arithmetic contract, not a SIMD helper. */
static int test_replay_fixed(void)
{
    enum {N=256, M=65};
    uint32_t nb[3*M]; double mul[3*M], piv[M], rhs[M], x[N], ref[N];
    for(int d=0;d<=3;d++) for(int n=0;n<=M;n++) for(int saved=0;saved<2;saved++) {
        for(int i=0;i<N;i++) x[i]=ref[i]=uni()*3;
        for(int i=0;i<M;i++) { piv[i]=0.1+fabs(uni()); rhs[i]=uni(); }
        for(int i=0;i<3*M;i++) { nb[i]=128+(uint32_t)(rnd()%128); mul[i]=uni(); }
        for(int i=0;i<n;i++) {
            double v=(saved?rhs[i]:ref[16+i])/piv[i];
            for(int j=0;j<d;j++) { double u=mul[d*i+j]*ref[nb[d*i+j]]; v-=u; }
            ref[16+i]=v;
        }
        CHECK(vsdlss_simd_replay_fixed(n,d,nb,mul,piv,saved?rhs:x+16,x+16,x)==0);
        CHECK(memcmp(x,ref,sizeof x)==0);
    }
    /* Hand-computed chain: 20/2 - 3*2 - 4*1 = 0, not (20-6-4)/2. */
    for(int i=0;i<N;i++) x[i]=0;
    x[128]=2; x[129]=1;
    for(int i=0;i<5;i++) { rhs[i]=20; piv[i]=2; nb[2*i]=128; nb[2*i+1]=129; mul[2*i]=3; mul[2*i+1]=4; }
    CHECK(vsdlss_simd_replay_fixed(5,2,nb,mul,piv,rhs,x+16,x)==0);
    for(int i=0;i<5;i++) CHECK(x[16+i]==0);
    for(int lane=0;lane<5;lane++) for(int kind=0;kind<3;kind++) {
        for(int i=0;i<5;i++) { rhs[i]=1; piv[i]=1; }
        rhs[lane]=kind==0?INFINITY:kind==1?NAN:DBL_MAX;
        if(kind==2) piv[lane]=0.5;
        CHECK(vsdlss_simd_replay_fixed(5,0,nb,mul,piv,rhs,x+16,x)==1);
    }
    for(int i=0;i<5;i++) { rhs[i]=-0.0; piv[i]=1; }
    CHECK(vsdlss_simd_replay_fixed(5,0,nb,mul,piv,rhs,x+16,x)==0);
    for(int i=0;i<5;i++) CHECK(signbit(x[16+i]));
    return 0;
}

int main(void)
{
    if(!vsdlss_simd_enabled()){puts("test_simd: AVX2 unavailable or VSDLSS_SIMD=0, skipped");return 0;}
    CHECK(test_replay_fixed()==0);
    enum{N=4096};
    double *x=malloc(N*8),*y=malloc(N*8),*a=malloc(N*64*8),*xs=malloc(64*8),*v=malloc(8*8),*w=malloc(8*8);
    int32_t *R=malloc(N*sizeof(int32_t));
    CHECK(x&&y&&a&&xs&&v&&w&&R);
    for(int trial=0;trial<2000;trial++){
        csi ws=1+(csi)(rnd()%40), n=(csi)(rnd()%70), rs=n+ws+(csi)(rnd()%5);
        for(csi i=0;i<rs*ws;i++)a[i]=uni();
        for(csi j=0;j<ws;j++)xs[j]=uni()*3;
        /* rows: ascending, runs of consecutive indices mixed with gaps */
        csi at=(csi)(rnd()%8);
        for(csi r=0;r<n;r++){R[r]=(int32_t)at; at+=(rnd()%3==0)?1+(csi)(rnd()%9):1;}
        csi r0=n?(csi)(rnd()%(n+1)):0, r1=r0+(n>r0?(csi)(rnd()%(n-r0+1)):0);
        for(csi i=0;i<N;i++)x[i]=y[i]=uni();
        /* block update */
        vsdlss_simd_block_update(a,rs,ws,xs,R,r0,r1,x);
        for(csi j=0;j<ws;j++){const double *col=a+j*rs;for(csi r=r0;r<r1;r++)y[R[r]]-=col[r]*xs[j];}
        CHECK(memcmp(x,y,N*8)==0);
        /* 16-row variant: same contract, same bits */
        vsdlss_simd_block_update16(a,rs,ws,xs,R,r0,r1,x);
        for(csi j=0;j<ws;j++){const double *col=a+j*rs;for(csi r=r0;r<r1;r++)y[R[r]]-=col[r]*xs[j];}
        CHECK(memcmp(x,y,N*8)==0);
        /* packed triangle of order tw, columns [j0,j1), rows [q0,q1) >= j1 */
        {
            csi tw=1+(csi)(rnd()%60), j0=(csi)(rnd()%tw), j1=j0+(csi)(rnd()%(tw-j0+1));
            csi q0=j1+(csi)(rnd()%(tw-j1+1)), q1=q0+(csi)(rnd()%(tw-q0+1));
            for(csi i=0;i<tw*(tw+1)/2;i++)a[i]=uni();
            for(csi j=0;j<64;j++)xs[j]=uni()*3;
            vsdlss_simd_tri_update_packed(a,tw,j0,j1,xs,q0,q1,x+5);
            for(csi r=q0;r<q1;r++){double t=y[5+r];for(csi j=j0;j<j1;j++)t-=a[j*tw-j*(j-1)/2+r-j]*xs[j];y[5+r]=t;}
            CHECK(memcmp(x,y,N*8)==0);
        }
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
