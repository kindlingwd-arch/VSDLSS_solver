#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"
#include "vsdlss_simd.h"
#include "vsdlss_dense.h"

#include <string.h>

#define VSDLSS_SOLVE_BLK 128
#define VSDLSS_SOLVE_GATHER 1024
static int solve_kv(void);

/* Panel factorization is the blocked dense kernel shared with M4. */
vsdlss_status vsdlss_panel_factor(double *a, csi rows, csi width)
{
    return vsdlss_dense_potrf_panel(a, rows, width);
}


/* Forward diagonal block with fused four-column updates: columns j0..j0+3
 * are solved in order (each updates the rows of its own block), then the
 * rows below the block get the four subtractions in the same order as four
 * separate axpy calls.  Same operations per entry: same bits. Returns 1 on
 * an invalid pivot, 2 on a non-finite value. */
static int fwd_diag4(const double *a,csi rows,csi width,double *xb)
{
    csi j0=0;
    for(;j0+4<=width;j0+=4){
        for(csi j=j0;j<j0+4;j++){
            double d=a[j*rows+j];
            if(!isfinite(d)||d<=0)return 1;
            xb[j]/=d;
            if(!isfinite(xb[j]))return 2;
            for(csi r=j+1;r<j0+4;r++){ double u=a[j*rows+r]*xb[j]; xb[r]-=u; }
        }
        vsdlss_simd_axpy4_neg(xb+j0+4,a+j0*rows+j0+4,rows,xb+j0,width-j0-4);
    }
    for(csi j=j0;j<width;j++){
        double d=a[j*rows+j];
        if(!isfinite(d)||d<=0)return 1;
        xb[j]/=d;
        if(!isfinite(xb[j]))return 2;
        vsdlss_simd_axpy_neg(xb+j+1,a+j*rows+j+1,xb[j],width-j-1);
    }
    return 0;
}

vsdlss_status vsdlss_panel_solve_generic(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
    csi rows=width+ext;
    int nt=vsdlss_parallel_width((double)width*ext),bad=0;(void)nt;
    const int simd=vsdlss_simd_enabled(), kv2=solve_kv()>=2;
    /* External rows have no dependency on J until the triangular solve ends.
       Reverse solve applies the external contribution before solving J^T. */
    if(!back && simd && kv2){
        int e=fwd_diag4(a,rows,width,x+begin);
        if(e)return e==1?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE;
    } else if(!back)for(csi j=0;j<width;j++){
        double d=a[j*rows+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        x[begin+j]/=d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
        if(simd)vsdlss_simd_axpy_neg(x+begin+j+1,a+j*rows+j+1,x[begin+j],width-j-1);
        else for(csi r=j+1;r<width;r++)x[begin+r]-=a[j*rows+r]*x[begin+j];
    }
    /* External phase.  Both directions keep the per-element accumulation
       order of the original element-at-a-time loops and only change which
       values are reused, so every result is bitwise unchanged.
       Forward: hold one block of external destinations in registers and sweep
       j outward, turning the stride-`rows` panel reads into contiguous ones.
       Reverse: gather the external x values once instead of re-reading them
       through index[] for every column of J. */
    if(!back) {
        const csi blk=VSDLSS_SOLVE_BLK,nblocks=ext/blk+(ext%blk!=0);
        int fnt=nt; if(fnt>nblocks)fnt=(int)nblocks;
        if(fnt<1)fnt=1;
        (void)fnt;
        VSDLSS_OMP(omp parallel num_threads(fnt) if(fnt>1) reduction(|:bad))
        {
            VSDLSS_OMP(omp master)
            vsdlss_parallel_observe();
            VSDLSS_OMP(omp for schedule(static))
            for(csi b=0;b<nblocks;b++){
                csi t0=b*blk,t1=t0+blk,t;
                double v[VSDLSS_SOLVE_BLK];
                if(t1>ext)t1=ext;
                for(t=t0;t<t1;t++)v[t-t0]=x[index[t]];
                csi j0=0;
                if(simd && kv2) for(;j0+4<=width;j0+=4)
                    vsdlss_simd_axpy4_neg(v,a+j0*rows+width+t0,rows,x+begin+j0,t1-t0);
                for(csi j=j0;j<width;j++){
                    double c=x[begin+j];
                    const double *col=a+j*rows+width;
                    if(simd){vsdlss_simd_axpy_neg(v,col+t0,c,t1-t0);continue;}
                    for(t=t0;t<t1;t++)v[t-t0]-=col[t]*c;
                }
                for(t=t0;t<t1;t++){
                    x[index[t]]=v[t-t0];
                    if(!isfinite(v[t-t0]))bad=1;
                }
            }
        }
    } else if(ext) {
        double stack_gather[VSDLSS_SOLVE_GATHER];
        double *xg=ext<=VSDLSS_SOLVE_GATHER?stack_gather:
                   (double*)malloc((size_t)ext*sizeof(double));
        if(!xg) {
            /* Allocation-free fallback; identical arithmetic, more reads. */
            VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(static))
                for(csi t=0;t<width;t++){
                    double v=x[begin+t];
                    for(csi r=0;r<ext;r++)v-=a[t*rows+width+r]*x[index[r]];
                    x[begin+t]=v;if(!isfinite(v))bad=1;
                }
            }
        } else {
            for(csi r=0;r<ext;r++)xg[r]=x[index[r]];
            /* SIMD: columns in groups of four, one per lane, each in its
               original accumulation order. */
            const csi quads=simd?width/4:0;
            VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(static))
                for(csi q=0;q<quads;q++)
                    bad|=vsdlss_simd_dot4(a+4*q*rows+width,rows,xg,ext,x+begin+4*q);
                VSDLSS_OMP(omp for schedule(static))
                for(csi t=4*quads;t<width;t++){
                    double v=x[begin+t];
                    const double *row_t=a+t*rows+width;
                    for(csi r=0;r<ext;r++)v-=row_t[r]*xg[r];
                    x[begin+t]=v;if(!isfinite(v))bad=1;
                }
            }
            if(xg!=stack_gather)free(xg);
        }
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    if(back)for(csi j=width;j-- >0;){
        double d=a[j*rows+j],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=a[j*rows+r]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

#include "vsdlss_small_solve.inc"

/* Backward solve of one panel on one thread with the SIMD dot products:
   the arithmetic and status results of vsdlss_panel_solve_generic(back). */
static vsdlss_status simd_back_narrow(const double *a,csi begin,csi width,
                                      csi ext,const csi *index,double *x)
{
    csi rows=width+ext; int bad=0;
    if(ext){
        double stack_gather[VSDLSS_SOLVE_GATHER];
        double *xg=ext<=VSDLSS_SOLVE_GATHER?stack_gather:(double*)malloc((size_t)ext*sizeof(double));
        if(!xg)return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,1);
        for(csi r=0;r<ext;r++)xg[r]=x[index[r]];
        csi q=0;
        for(;q+4<=width;q+=4)bad|=vsdlss_simd_dot4(a+q*rows+width,rows,xg,ext,x+begin+q);
        for(csi t=q;t<width;t++){
            double v=x[begin+t];
            const double *row_t=a+t*rows+width;
            for(csi r=0;r<ext;r++)v-=row_t[r]*xg[r];
            x[begin+t]=v;if(!isfinite(v))bad=1;
        }
        if(xg!=stack_gather)free(xg);
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    for(csi j=width;j-- >0;){
        double d=a[j*rows+j],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=a[j*rows+r]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}


/* Kernel generation for single-threaded narrow/medium panels
 * (VSDLSS_SOLVE_KV, default 2; 1 = previous kernels):
 *   forward: the external update is done by vsdlss_simd_block_update, which
 *     keeps up to eight destinations in registers across all J columns
 *     (the generic path re-reads a stack buffer for every column and enters
 *     an OpenMP region per panel even when it runs on one thread);
 *   backward: columns in groups of eight (vsdlss_simd_dot8, two independent
 *     chains) instead of four.
 * Per entry the operation sequence is unchanged: bitwise identical results
 * and the same status codes. */
#define VSDLSS_KV2_FWD_MAX 128
int vsdlss_solve_kv = -1;
static int solve_kv(void)
{
    if (vsdlss_solve_kv < 0) { const char *e = getenv("VSDLSS_SOLVE_KV"); vsdlss_solve_kv = e ? atoi(e) : 2; }
    return vsdlss_solve_kv;
}

static vsdlss_status fwd_kv2(const double *a,csi begin,csi width,csi ext,const csi *index,double *x)
{
    const csi rows=width+ext;
    { int e=fwd_diag4(a,rows,width,x+begin); if(e)return e==1?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE; }
    if(ext){
        int bad=0;
        vsdlss_simd_block_update(a+width,rows,width,x+begin,index,0,ext,x);
        for(csi t=0;t<ext;t++) bad|=!isfinite(x[index[t]]);
        if(bad)return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

static vsdlss_status back_kv2(const double *a,csi begin,csi width,
                              csi ext,const csi *index,double *x)
{
    csi rows=width+ext; int bad=0;
    if(ext){
        double stack_gather[VSDLSS_SOLVE_GATHER];
        double *xg=ext<=VSDLSS_SOLVE_GATHER?stack_gather:(double*)malloc((size_t)ext*sizeof(double));
        if(!xg)return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,1);
        for(csi r=0;r<ext;r++)xg[r]=x[index[r]];
        csi q=0;
        for(;q+8<=width;q+=8)bad|=vsdlss_simd_dot8(a+q*rows+width,rows,xg,ext,x+begin+q);
        for(;q+4<=width;q+=4)bad|=vsdlss_simd_dot4(a+q*rows+width,rows,xg,ext,x+begin+q);
        for(csi t=q;t<width;t++){
            double v=x[begin+t];
            const double *row_t=a+t*rows+width;
            for(csi r=0;r<ext;r++)v-=row_t[r]*xg[r];
            x[begin+t]=v;if(!isfinite(v))bad=1;
        }
        if(xg!=stack_gather)free(xg);
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    for(csi j=width;j-- >0;){
        double d=a[j*rows+j],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=a[j*rows+r]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

/* VSDLSS_SOLVE_KV=3 (opt-in, NOT bitwise equal to the default kernels):
 * backward solve with a relaxed summation order.  Columns are processed in
 * blocks of eight from the bottom; for each block the in-panel rows below
 * it and the external rows (contiguous in every column of the block) are
 * one dot8 over a gathered vector xx = (solved in-panel x, gathered
 * external x); only the 8x8 diagonal block stays serial.  Per entry the
 * terms are the same, subtracted in a different order: in-panel rows below
 * the block, external rows, then the rows inside the block.  Same pivot and
 * finite checks. */
static vsdlss_status back_relaxed(const double *a,csi begin,csi width,
                                  csi ext,const csi *index,double *x)
{
    const csi rows=width+ext; int bad=0;
    double stack_xx[VSDLSS_SOLVE_GATHER];
    double *xx=rows<=VSDLSS_SOLVE_GATHER?stack_xx:(double*)malloc((size_t)rows*sizeof(double));
    if(!xx) return simd_back_narrow(a,begin,width,ext,index,x);
    for(csi r=0;r<ext;r++) xx[width+r]=x[index[r]];
    csi j1=width;
    while(j1>0){
        csi j0=j1>=8?j1-8:0, bw=j1-j0;
        double *xb=x+begin+j0;
        if(bw==8) bad|=vsdlss_simd_dot8(a+j0*rows+j1,rows,xx+j1,rows-j1,xb);
        else for(csi t=0;t<bw;t++){
            double v=xb[t]; const double *c=a+(j0+t)*rows;
            for(csi r=j1;r<rows;r++) v-=c[r]*xx[r];
            xb[t]=v; if(!isfinite(v)) bad=1;
        }
        if(bad) break;
        for(csi j=j1;j-- >j0;){
            double d=a[j*rows+j],v=x[begin+j];
            if(!isfinite(d)||d<=0){ if(xx!=stack_xx) free(xx); return VSDLSS_ERR_INVALID; }
            for(csi r=j+1;r<j1;r++) v-=a[j*rows+r]*x[begin+r];
            x[begin+j]=v/d; xx[j]=x[begin+j];
            if(!isfinite(x[begin+j])) bad=1;
        }
        if(bad) break;
        j1=j0;
    }
    if(xx!=stack_xx) free(xx);
    return bad?VSDLSS_ERR_NONFINITE:VSDLSS_OK;
}

#ifdef VSDLSS_BLAS
/* Wide panels (at least VSDLSS_BLAS_SOLVE_MIN columns, default 32; 0
 * disables) use dtrsv + dgemv.  Same pivot checks as the built-in path; a
 * non-finite result is reported for J here and, for the external rows, by
 * the caller's scan of the solution. */
#include <limits.h>
#include <stdatomic.h>
#include <stdlib.h>
void dtrsv_(const char *, const char *, const char *, const int *, const double *,
            const int *, double *, const int *);
void dgemv_(const char *, const int *, const int *, const double *, const double *,
            const int *, const double *, const int *, const double *, double *,
            const int *);
static csi solve_blas_min(void)
{
    static atomic_llong cached = -1;
    long long v = atomic_load_explicit(&cached, memory_order_relaxed);
    if (v < 0) {
        const char *e = getenv("VSDLSS_BLAS_SOLVE_MIN");
        v = e ? atoll(e) : 32;
        if (v < 0) v = 0;
        atomic_store_explicit(&cached, v, memory_order_relaxed);
    }
    return (csi)v;
}
int vsdlss_panel_solve_uses_blas(void) { return solve_blas_min() > 0; }
static vsdlss_status blas_panel_solve(const double *a, csi begin, csi width,
                                      csi ext, const csi *index, double *x, int back)
{
    const csi rows = width + ext;
    int W = (int)width, E = (int)ext, LD = (int)rows, one = 1;
    double p1 = 1.0, m1 = -1.0, zero = 0.0;
    for (csi j = 0; j < width; j++) {
        double d = a[j * rows + j];
        if (!isfinite(d) || d <= 0) return VSDLSS_ERR_INVALID;
    }
    double stack[VSDLSS_SOLVE_GATHER];
    double *t = ext <= VSDLSS_SOLVE_GATHER ? stack : (double *)malloc((size_t)ext * sizeof(double));
    if (!t) return vsdlss_panel_solve_generic(a, begin, width, ext, index, x, back);
    if (!back) {
        dtrsv_("L", "N", "N", &W, a, &LD, x + begin, &one);
        if (E) {
            dgemv_("N", &E, &W, &p1, a + width, &LD, x + begin, &one, &zero, t, &one);
            for (csi r = 0; r < ext; r++) x[index[r]] -= t[r];
        }
    } else {
        if (E) {
            for (csi r = 0; r < ext; r++) t[r] = x[index[r]];
            dgemv_("T", &E, &W, &m1, a + width, &LD, t, &one, &p1, x + begin, &one);
        }
        dtrsv_("L", "T", "N", &W, a, &LD, x + begin, &one);
    }
    if (t != stack) free(t);
    for (csi j = 0; j < width; j++) if (!isfinite(x[begin + j])) return VSDLSS_ERR_NONFINITE;
    return VSDLSS_OK;
}
#endif

vsdlss_status vsdlss_panel_solve(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
#ifdef VSDLSS_BLAS
    { csi m=solve_blas_min();
      if(m && width>=m && width+ext<INT_MAX) return blas_panel_solve(a,begin,width,ext,index,x,back); }
#endif
    /* Keep large external-row work on the existing parallel path. */
    if(vsdlss_parallel_width((double)width*ext)>1)
        return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,back);
    if(solve_kv()>=2 && vsdlss_simd_enabled()) {
        if(back && width>=8) return solve_kv()>=3?back_relaxed(a,begin,width,ext,index,x)
                                                 :back_kv2(a,begin,width,ext,index,x);
        /* Wider panels: the register kernel reads each column 64 bytes at a
         * time and loses to the generic 128-row blocks (measured 0.5x at
         * w = 256-512, break-even between 128 and 160). */
        if(!back && width>=7 && width<=VSDLSS_KV2_FWD_MAX) return fwd_kv2(a,begin,width,ext,index,x);
    }
    /* Backward, width >= 4: four-column SIMD dot products (same arithmetic
       as the generated scalar kernels), without the generic kernel's
       OpenMP region, whose entry cost would dominate narrow panels. */
    if(back && width>=4 && vsdlss_simd_enabled())
        return simd_back_narrow(a,begin,width,ext,index,x);
    switch(width) {
#define CASE(N) case N: return solve_##N(a,begin,ext,index,x,back)
        CASE(1); CASE(2); CASE(3); CASE(4); CASE(5); CASE(6);
#undef CASE
        default: return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,back);
    }
}
