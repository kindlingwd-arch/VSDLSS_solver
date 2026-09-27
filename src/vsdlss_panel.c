#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"
#include "vsdlss_simd.h"
#include "vsdlss_dense.h"

#include <string.h>

#define VSDLSS_SOLVE_BLK 128
#define VSDLSS_SOLVE_GATHER 1024

/* Panel factorization is the blocked dense kernel shared with M4. */
vsdlss_status vsdlss_panel_factor(double *a, csi rows, csi width)
{
    return vsdlss_dense_potrf_panel(a, rows, width);
}

vsdlss_status vsdlss_panel_solve_generic(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
    csi rows=width+ext;
    int nt=vsdlss_parallel_width((double)width*ext),bad=0;(void)nt;
    const int simd=vsdlss_simd_enabled();
    /* External rows have no dependency on J until the triangular solve ends.
       Reverse solve applies the external contribution before solving J^T. */
    if(!back)for(csi j=0;j<width;j++){
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
                for(csi j=0;j<width;j++){
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
csi vsdlss_panel_solve_blas_min(void) { return solve_blas_min(); }
/* Forward BLAS step of one panel without the scatter: dtrsv on the
 * diagonal block, then t = L_ext * x_J (dgemv, beta 0) for the ext external
 * rows.  The push form subtracts t[r] from x[index[r]]; the tree solve
 * stores t and lets each target pull its rows, so both apply the same t
 * with one subtraction per entry and give the same bits.  Pivots are
 * checked first; non-finite results are left to the caller's scan. */
vsdlss_status vsdlss_panel_blas_forward(const double *a, csi begin, csi width,
                                        csi ext, double *x, double *t)
{
    const csi rows = width + ext;
    int W = (int)width, E = (int)ext, LD = (int)rows, one = 1;
    double p1 = 1.0, zero = 0.0;
    for (csi j = 0; j < width; j++) {
        double d = a[j * rows + j];
        if (!isfinite(d) || d <= 0) return VSDLSS_ERR_INVALID;
    }
    dtrsv_("L", "N", "N", &W, a, &LD, x + begin, &one);
    if (E) dgemv_("N", &E, &W, &p1, a + width, &LD, x + begin, &one, &zero, t, &one);
    return VSDLSS_OK;
}
/* Backward BLAS step in fixed column blocks of VSDLSS_BLAS_BWD_BLK
 * (default 64; 0 selects the single dgemv + dtrsv form, one thread):
 *   1. x_J -= L_ext^T t, one dgemv per column block;
 *   2. L_JJ^T x_J = y right-looking over the same blocks from the last:
 *      dtrsv on the diagonal block, then one dgemv per earlier column block
 *      subtracts that block's contribution.
 * Every BLAS call and its arguments depend only on the panel and the block
 * size, never on the team size; the calls of one step touch disjoint
 * columns, so a team may split them.  Results are therefore the same bits
 * for every thread count (they differ from the single-call form). */
static csi bwd_blk(void)
{
    static atomic_llong cached = -1;
    long long v = atomic_load_explicit(&cached, memory_order_relaxed);
    if (v < 0) {
        const char *e = getenv("VSDLSS_BLAS_BWD_BLK");
        v = e ? atoll(e) : 64;
        if (v < 0) v = 0;
        atomic_store_explicit(&cached, v, memory_order_relaxed);
    }
    return (csi)v;
}

/* Called by every thread (tid of T) of a team, or with T = 1 outside a
 * parallel region: contains team barriers. */
static void blas_backward_team(const double *a, csi begin, csi width, csi ext,
                               const double *t, double *x, csi B, int tid, int T)
{
    const csi rows = width + ext, nb = (width + B - 1) / B;
    int LD = (int)rows, E = (int)ext, one = 1;
    double p1 = 1.0, m1 = -1.0;
    if (ext) {
        for (csi c = tid; c < nb; c += T) {
            const csi c0 = c * B, c1 = c0 + B < width ? c0 + B : width;
            int N = (int)(c1 - c0);
            dgemv_("T", &E, &N, &m1, a + c0 * rows + width, &LD, t, &one, &p1, x + begin + c0, &one);
        }
        if (T > 1) { VSDLSS_OMP(omp barrier) }
    }
    for (csi k = nb - 1; k >= 0; --k) {
        const csi jb = k * B, je = jb + B < width ? jb + B : width;
        int M = (int)(je - jb);
        /* T == 1 may run inside another team (a subtree task): no
         * worksharing or barriers then, they would bind to that team. */
        if (T > 1) {
            VSDLSS_OMP(omp single)
            dtrsv_("L", "T", "N", &M, a + jb * rows + jb, &LD, x + begin + jb, &one);
        } else dtrsv_("L", "T", "N", &M, a + jb * rows + jb, &LD, x + begin + jb, &one);
        if (k == 0) break;
        for (csi c = tid; c < k; c += T) {
            const csi c0 = c * B, c1 = c0 + B;
            int M = (int)(je - jb), N = (int)(c1 - c0);
            dgemv_("T", &M, &N, &m1, a + c0 * rows + jb, &LD, x + begin + jb, &one, &p1, x + begin + c0, &one);
        }
        if (T > 1) { VSDLSS_OMP(omp barrier) }
    }
    (void)tid; (void)T;
}

static vsdlss_status blas_panel_solve(const double *a, csi begin, csi width,
                                      csi ext, const csi *index, double *x, int back)
{
    const csi rows = width + ext;
    int W = (int)width, E = (int)ext, LD = (int)rows, one = 1;
    double p1 = 1.0, m1 = -1.0;
    for (csi j = 0; j < width; j++) {
        double d = a[j * rows + j];
        if (!isfinite(d) || d <= 0) return VSDLSS_ERR_INVALID;
    }
    double stack[VSDLSS_SOLVE_GATHER];
    double *t = ext <= VSDLSS_SOLVE_GATHER ? stack : (double *)malloc((size_t)ext * sizeof(double));
    if (!t) return vsdlss_panel_solve_generic(a, begin, width, ext, index, x, back);
    if (!back) {
        (void)vsdlss_panel_blas_forward(a, begin, width, ext, x, t);   /* pivots checked above */
        for (csi r = 0; r < ext; r++) x[index[r]] -= t[r];
    } else {
        const csi B = bwd_blk();
        for (csi r = 0; r < ext; r++) t[r] = x[index[r]];
        if (!B) {
            if (E) dgemv_("T", &E, &W, &m1, a + width, &LD, t, &one, &p1, x + begin, &one);
            dtrsv_("L", "T", "N", &W, a, &LD, x + begin, &one);
        } else {
            /* A team only when there are several blocks and enough work. */
            int nt = width > B ? vsdlss_parallel_width((double)width * (double)(width + ext)) : 1;
            if (nt > (width + B - 1) / B) nt = (int)((width + B - 1) / B);
            if (nt > 1) {
                VSDLSS_OMP(omp parallel num_threads(nt))
                {
#ifdef _OPENMP
                    if (omp_get_thread_num() == 0) vsdlss_parallel_observe();
                    blas_backward_team(a, begin, width, ext, t, x, B, omp_get_thread_num(), omp_get_num_threads());
#endif
                }
            } else blas_backward_team(a, begin, width, ext, t, x, B, 0, 1);
        }
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
