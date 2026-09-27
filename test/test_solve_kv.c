/* Panel kernel generations and the in-place packed solve:
 *   KV=2 equals KV=1 bit for bit (forward and backward, many (w, e) shapes,
 *   contiguous and scattered external rows, non-finite propagation);
 *   KV=3 (relaxed order) agrees to rounding;
 *   vsdlss_m3_solve_packed_inplace equals vsdlss_m3_solve_packed. */
#include "../src/vsdlss_m3_internal.h"
#include "../src/vsdlss_simd.h"
#include <stdio.h>
#include <string.h>
extern int vsdlss_solve_kv;
vsdlss_status vsdlss_panel_solve(const double*,csi,csi,csi,const csi*,double*,int);
static unsigned long long s_=88172645463325252ULL;
static double rnd(void){ s_^=s_<<13; s_^=s_>>7; s_^=s_<<17; return (double)(s_>>11)/9007199254740992.0; }
static int fails;
#define CHECK(c,msg,...) do{ if(!(c)){ fails++; fprintf(stderr,"FAIL: " msg "\n",__VA_ARGS__); } }while(0)

static void panel_case(csi w,csi e,int scattered,int inject)
{
    csi rows=w+e, n=w+3*e+8;
    double *a=malloc((size_t)(rows*w+1)*8), *x0=malloc(n*8), *x1=malloc(n*8), *x2=malloc(n*8), *x3=malloc(n*8);
    csi *idx=malloc((size_t)(e+1)*sizeof(csi));
    for(csi j=0;j<w;j++) for(csi r=0;r<rows;r++) a[j*rows+r]= r==j?1.5+rnd():(r>j?(rnd()-0.5)/(1+0.2*w):0);
    for(csi t=0;t<e;t++) idx[t]=w+(scattered?3*t+(csi)(3*rnd()):t);
    for(csi i=0;i<n;i++) x0[i]=rnd()-0.5;
    if(inject && e) x0[idx[e/2]]=1.0/0.0;
    for(int back=0;back<2;back++){
        vsdlss_status s1,s2,s3;
        memcpy(x1,x0,n*8); vsdlss_solve_kv=1; s1=vsdlss_panel_solve(a,0,w,e,e?idx:NULL,x1,back);
        memcpy(x2,x0,n*8); vsdlss_solve_kv=2; s2=vsdlss_panel_solve(a,0,w,e,e?idx:NULL,x2,back);
        memcpy(x3,x0,n*8); vsdlss_solve_kv=3; s3=vsdlss_panel_solve(a,0,w,e,e?idx:NULL,x3,back);
        CHECK(s1==s2,"status kv1 %d kv2 %d w=%lld e=%lld back=%d",s1,s2,(long long)w,(long long)e,back);
        CHECK(s1==s3||inject,"status kv1 %d kv3 %d w=%lld e=%lld back=%d",s1,s3,(long long)w,(long long)e,back);
        if(s1==VSDLSS_OK){
            CHECK(!memcmp(x1,x2,n*8),"kv2 not bitwise w=%lld e=%lld back=%d",(long long)w,(long long)e,back);
            double m=0; for(csi i=0;i<n;i++){ double d=fabs(x1[i]-x3[i])/(1+fabs(x1[i])); if(d>m)m=d; }
            CHECK(m<1e-12,"kv3 differs %.2e w=%lld e=%lld back=%d",m,(long long)w,(long long)e,back);
        }
    }
    free(a);free(x0);free(x1);free(x2);free(x3);free(idx);
}

int main(void)
{
    if(!vsdlss_simd_enabled()){ puts("test_solve_kv: no AVX2, skipped"); return 0; }
    const csi W[]={1,4,6,7,8,9,12,15,16,17,31,32,33,64,127,128,129,200,300};
    const csi E[]={0,1,3,4,5,8,17,40,128,1100};
    for(unsigned i=0;i<sizeof W/sizeof*W;i++) for(unsigned k=0;k<sizeof E/sizeof*E;k++){
        panel_case(W[i],E[k],0,0); panel_case(W[i],E[k],1,0); panel_case(W[i],E[k],1,1);
    }
    vsdlss_solve_kv=2;
    /* In-place packed solve on a grid with dangling nodes (many components). */
    csi R=120,C=130,n=R*C,nz=0; vsdlss *A=vsdlss_spalloc(n,n,3*n,1,0);
    for(csi j=0;j<n;j++){ A->p[j]=nz; csi r=j/C,c=j%C;
        if(c>0&&(j%7)){A->i[nz]=j-1;A->x[nz++]=-1;} if(r>0&&(j%5)){A->i[nz]=j-C;A->x[nz++]=-1.3;}
        A->i[nz]=j;A->x[nz++]=4.5+(j%3); }
    A->p[n]=nz;
    vsdlss_m3_factor *f=NULL;
    CHECK(vsdlss_factorize_m3(A,5,&f)==VSDLSS_OK,"%s","factor");
    double *b=malloc(n*8),*x1=malloc(n*8),*x2=malloc(n*8);
    for(csi i=0;i<n;i++) b[i]=sin(0.37*(double)i);
    CHECK(vsdlss_m3_solve_packed(f,b,x1)==VSDLSS_OK,"%s","packed");
    memcpy(x2,b,n*8);
    CHECK(vsdlss_m3_solve_packed_inplace(f,x2)==VSDLSS_OK,"%s","inplace");
    CHECK(!memcmp(x1,x2,n*8),"%s","inplace not bitwise equal to packed");
    memcpy(x2,b,n*8); x2[n/3]=0.0/0.0;
    CHECK(vsdlss_m3_solve_packed_inplace(f,x2)==VSDLSS_ERR_NONFINITE,"%s","inplace nonfinite status");
    CHECK(!memcmp(x2,b,(size_t)(n/3)*8),"%s","inplace modified x on rejected RHS");
    CHECK(vsdlss_m3_solve_packed_inplace(NULL,x2)==VSDLSS_ERR_INVALID,"%s","null factor");
    vsdlss_m3_factor_free(f); vsdlss_spfree(A); free(b);free(x1);free(x2);
    if(fails){ printf("test_solve_kv: %d failures\n",fails); return 1; }
    puts("test_solve_kv: kv2 bitwise, kv3 within rounding, in-place packed solve: passed");
    return 0;
}
