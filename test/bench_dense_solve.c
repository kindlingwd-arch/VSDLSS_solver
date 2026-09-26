/* Single-RHS SOLVE benchmark. No factorization, ordering or allocation in core
 * timing. CHOLMOD 5.3 super_solve worker uses DTRSV + DGEMV for one RHS.
 * The panel-core adapter removes sparse gather/scatter only; check_core verifies
 * its output bitwise against the real VSDLSS panel entry before benchmarking.
 * The diagonal-only case also calls the actual CHOLMOD super solve API on a
 * complete valid one-supernode factor. No incomplete factor objects are used.
 */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include "../src/vsdlss_simd.h"
#include <cholmod.h>
#ifdef VSDLSS_BLAS
#error "This benchmark must use the built-in VSDLSS path; remove VSDLSS_BLAS"
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>

void dtrsv_(const char*,const char*,const char*,const int*,const double*,const int*,double*,const int*);
void dgemv_(const char*,const int*,const int*,const double*,const double*,const int*,const double*,const int*,const double*,double*,const int*);

typedef struct {
    int w,e,rows,back;
    double *a,*b,*want,*scratch;
    csi *index;
    cholmod_factor L;
    cholmod_dense E;
    cholmod_common common;
    int64_t super[2],pi[2],px[2];
    int64_t *s,*colcount;
} problem;
static volatile double sink;
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static void fail(const char*s){fprintf(stderr,"FAIL: %s\n",s);exit(1);}
static void *alloc(size_t n,size_t z){void*p=calloc(n?n:1,z);if(!p)fail("allocation");return p;}
static int cmp(const void*a,const void*b){double x=*(const double*)a,y=*(const double*)b;return (x>y)-(x<y);}

/* Compute-only transcription of the current generic/simd_back_narrow paths:
 * contiguous external vector, no sparse index/gather or heap allocation.
 * Retains checks, 128-row forward tile, dot4, and per-element operation order.
 * Not a proposed replacement implementation. */
static int native_core(problem*p,double*x)
{
    const int w=p->w,e=p->e,ld=p->rows;const double*a=p->a;
    if(!e)return vsdlss_panel_solve(a,0,w,0,NULL,x,p->back)==VSDLSS_OK;
    int bad=0;
    if(!p->back){
        for(int j=0;j<w;j++){
            double d=a[(size_t)j*ld+j];if(!isfinite(d)||d<=0)return 0;
            x[j]/=d;if(!isfinite(x[j]))return 0;
            vsdlss_simd_axpy_neg(x+j+1,a+(size_t)j*ld+j+1,x[j],w-j-1);
        }
        for(int r=0;r<e;r+=128){
            int len=e-r<128?e-r:128;double v[128];
            memcpy(v,x+w+r,(size_t)len*8);
            for(int j=0;j<w;j++)vsdlss_simd_axpy_neg(v,a+(size_t)j*ld+w+r,x[j],len);
            for(int i=0;i<len;i++){x[w+r+i]=v[i];bad|=!isfinite(v[i]);}
        }
    }else{
        int j=0;
        for(;j+4<=w;j+=4)bad|=vsdlss_simd_dot4(a+(size_t)j*ld+w,ld,x+w,e,x+j);
        for(;j<w;j++){double v=x[j];for(int r=0;r<e;r++)v-=a[(size_t)j*ld+w+r]*x[w+r];x[j]=v;bad|=!isfinite(v);}
        for(int j=w;j-->0;){
            double d=a[(size_t)j*ld+j],v=x[j];if(!isfinite(d)||d<=0)return 0;
            for(int i=j+1;i<w;i++)v-=a[(size_t)j*ld+i]*x[i];
            x[j]=v/d;bad|=!isfinite(x[j]);
        }
    }
    return !bad;
}
static int blas_core(problem*p,double*x)
{
    int one=1;double neg=-1,pos=1;
    if(!p->back)dtrsv_("L","N","N",&p->w,p->a,&p->rows,x,&one);
    if(p->e){
        if(p->back)dgemv_("T",&p->e,&p->w,&neg,p->a+p->w,&p->rows,x+p->w,&one,&pos,x,&one);
        else dgemv_("N",&p->e,&p->w,&neg,p->a+p->w,&p->rows,x,&one,&pos,x+p->w,&one);
    }
    if(p->back)dtrsv_("L","T","N",&p->w,p->a,&p->rows,x,&one);
    return 1;
}
static int native_api(problem*p,double*x){return vsdlss_panel_solve(p->a,0,p->w,p->e,p->index,x,p->back)==VSDLSS_OK;}
/* Exact single-panel operation sequence from CHOLMOD's REAL/DOUBLE nrhs=1
 * worker, including sparse gather/scatter, with preallocated E. This adapter
 * does NOT include the CHOLMOD public entry validation or supernode iteration. */
static int cholmod_panel_adapter(problem*p,double*x)
{
    int one=1;double neg=-1,pos=1;
    for(int i=0;i<p->e;i++)p->scratch[i]=x[p->index[i]];
    if(!p->back)dtrsv_("L","N","N",&p->w,p->a,&p->rows,x,&one);
    if(p->e){
        if(p->back)dgemv_("T",&p->e,&p->w,&neg,p->a+p->w,&p->rows,p->scratch,&one,&pos,x,&one);
        else dgemv_("N",&p->e,&p->w,&neg,p->a+p->w,&p->rows,x,&one,&pos,p->scratch,&one);
    }
    if(p->back)dtrsv_("L","T","N",&p->w,p->a,&p->rows,x,&one);
    else for(int i=0;i<p->e;i++)x[p->index[i]]=p->scratch[i];
    return 1;
}
static int cholmod_actual(problem*p,double*x)
{
    cholmod_dense X={0};X.nrow=p->w;X.ncol=1;X.d=p->w;X.nzmax=p->w;X.x=x;X.xtype=CHOLMOD_REAL;X.dtype=CHOLMOD_DOUBLE;
    return p->back?cholmod_l_super_ltsolve(&p->L,&X,&p->E,&p->common):cholmod_l_super_lsolve(&p->L,&X,&p->E,&p->common);
}
typedef int(*kernel)(problem*,double*);
static double verify(problem*p,kernel f,int exact)
{
    double*x=alloc(p->rows,8);memcpy(x,p->b,p->rows*8);if(!f(p,x))fail("kernel status");double err=0;
    for(int i=0;i<p->rows;i++){if(!isfinite(x[i]))fail("nonfinite output");double d=fabs(x[i]-p->want[i]);if(d>err)err=d;}
    if(err>1e-11)fail("known-solution check");
    if(exact){double*y=alloc(p->rows,8);memcpy(y,p->b,p->rows*8);if(!native_api(p,y)||memcmp(x,y,p->rows*8))fail("core differs bitwise from production panel");free(y);}
    free(x);return err;
}
static void init(problem*p,int w,int e,int back)
{
    memset(p,0,sizeof(*p));p->w=w;p->e=e;p->rows=w+e;p->back=back;
    p->a=alloc((size_t)p->rows*w,8);p->b=alloc(p->rows,8);p->want=alloc(p->rows,8);p->scratch=alloc(e,8);p->index=alloc(e,sizeof(csi));
    for(int i=0;i<e;i++)p->index[i]=w+i;
    for(int j=0;j<w;j++)for(int i=j;i<p->rows;i++)p->a[(size_t)j*p->rows+i]=i==j?1.5+(j%7)*.03:.02*sin(i*.31+j*.71)/sqrt((double)w);
    for(int i=0;i<p->rows;i++)p->want[i]=.5+sin(i*.17);
    if(!back){
        for(int i=0;i<w;i++){long double s=0;for(int j=0;j<=i;j++)s+=(long double)p->a[(size_t)j*p->rows+i]*p->want[j];p->b[i]=(double)s;}
        for(int i=w;i<p->rows;i++){long double s=p->want[i];for(int j=0;j<w;j++)s+=(long double)p->a[(size_t)j*p->rows+i]*p->want[j];p->b[i]=(double)s;}
    }else{
        for(int j=0;j<w;j++){long double s=0;for(int i=j;i<p->rows;i++)s+=(long double)p->a[(size_t)j*p->rows+i]*p->want[i];p->b[j]=(double)s;}
        for(int i=w;i<p->rows;i++)p->b[i]=p->want[i];
    }
    cholmod_l_start(&p->common);p->common.nthreads_max=1;
    if(!e){
        p->s=alloc(w,sizeof(int64_t));p->colcount=alloc(w,sizeof(int64_t));
        for(int i=0;i<w;i++){p->s[i]=i;p->colcount[i]=w-i;}
        p->super[1]=w;p->pi[1]=w;p->px[1]=(int64_t)w*w;
        p->L.n=w;p->L.minor=w;p->L.nsuper=1;p->L.ssize=w;p->L.xsize=(size_t)w*w;
        p->L.ColCount=p->colcount;p->L.is_monotonic=1;
        p->L.super=p->super;p->L.pi=p->pi;p->L.px=p->px;p->L.s=p->s;p->L.x=p->a;
        p->L.is_ll=1;p->L.is_super=1;p->L.itype=CHOLMOD_LONG;p->L.xtype=CHOLMOD_REAL;p->L.dtype=CHOLMOD_DOUBLE;
        p->E.x=p->scratch;p->E.nzmax=1;p->E.nrow=1;p->E.ncol=1;p->E.d=1;p->E.xtype=CHOLMOD_REAL;p->E.dtype=CHOLMOD_DOUBLE;
        if(!cholmod_l_check_factor(&p->L,&p->common))fail("CHOLMOD factor validation");
    }
}
static void destroy(problem*p){cholmod_l_finish(&p->common);free(p->a);free(p->b);free(p->want);free(p->scratch);free(p->index);free(p->s);free(p->colcount);}
static void compare(problem*p,const char*scope,kernel left,kernel right,int rounds)
{
    double el=verify(p,left,left==native_core),er=verify(p,right,0);
    double fl=(double)p->w*p->w+2.*p->w*p->e;
    int batch=(int)(1000000./fl);if(batch<4)batch=4;if(batch>1024)batch=1024;
    if((size_t)batch*p->rows>262144)batch=262144/p->rows;
    if(batch<1)batch=1;
    double *x=alloc((size_t)batch*p->rows,8),*ts[2]={alloc(rounds,8),alloc(rounds,8)};
    kernel fs[2]={left,right};
    for(int r=-2;r<rounds;r++)for(int seq=0;seq<2;seq++){
        int k=(seq+(r&1))&1;
        for(int j=0;j<batch;j++)memcpy(x+(size_t)j*p->rows,p->b,p->rows*8);
        double t=now();for(int j=0;j<batch;j++)if(!fs[k](p,x+(size_t)j*p->rows))fail("timed kernel");
        t=(now()-t)/batch;if(r>=0)ts[k][r]=t;
        sink+=x[(size_t)(batch-1)*p->rows];
    }
    qsort(ts[0],rounds,8,cmp);qsort(ts[1],rounds,8,cmp);int med=rounds/2;
    printf("%s,%d,%d,%s,%d,%.6f,%.6f,%.4f,%.6f,%.6f,%.6f,%.6f,%.3e,%.3e\n",scope,p->w,p->e,p->back?"backward":"forward",batch,
        ts[0][med]*1e6,ts[1][med]*1e6,ts[0][med]/ts[1][med],ts[0][0]*1e6,ts[0][rounds-1]*1e6,ts[1][0]*1e6,ts[1][rounds-1]*1e6,el,er);
    fflush(stdout);free(x);free(ts[0]);free(ts[1]);
}
int main(int argc,char**argv)
{
    int rounds=argc>1?atoi(argv[1]):9;if(rounds<3||rounds>101||!(rounds&1))fail("rounds must be odd, 3..101");
    if(vsdlss_set_num_threads(1)!=VSDLSS_OK||!vsdlss_simd_enabled())fail("requires SIMD enabled, single thread");
    const int shapes[][2]={{1,0},{2,0},{3,0},{4,0},{6,0},{8,0},{16,0},{32,0},{64,0},{128,0},{256,0},{512,0},{787,0},{1024,0},{2048,0},
        {1,64},{2,64},{3,128},{4,128},{6,256},{8,512},{16,512},{32,512},{64,512},{128,512},{256,512},{512,512},{787,1024},{64,4096},{256,4096}};
    puts("scope,width,external,direction,batch,vsdlss_us,cholmod_or_blas_us,v_over_c,v_min_us,v_max_us,c_min_us,c_max_us,v_max_error,c_max_error");
    for(size_t s=0;s<sizeof(shapes)/sizeof(shapes[0]);s++)for(int b=0;b<2;b++){
        problem p;init(&p,shapes[s][0],shapes[s][1],b);
        if(!p.e)compare(&p,"diag_vs_blas",native_core,blas_core,rounds);
        else if(p.w>=8)compare(&p,"panel_compute_adapter",native_core,blas_core,rounds);
        if(!p.e)compare(&p,"actual_api",native_api,cholmod_actual,rounds);
        else compare(&p,"panel_total",native_api,cholmod_panel_adapter,rounds);
        destroy(&p);
    }
    fprintf(stderr,"PASS: 60 shape/direction pairs; known-solution checks; 50 core checks bitwise match production (small external panels tested via API only). sink=%g\n",sink);
    return 0;
}
