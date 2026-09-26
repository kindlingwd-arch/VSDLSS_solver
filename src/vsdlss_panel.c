#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"
#include "vsdlss_simd.h"
#include "vsdlss_dense.h"

#include <string.h>

#define VSDLSS_SOLVE_BLK 128
#define VSDLSS_SOLVE_GATHER 1024
#define VSDLSS_SOLVE_JB 16

/* Diagonal block of the backward solve, J^T x = y, in column blocks of
 * VSDLSS_BACK_NB aligned to the top of J.  Blocks are processed bottom-up.
 * For each block [k0,k1) the rows below it (already solved) are applied first
 * as independent dot products, eight or four columns at a time, then the
 * small triangle is solved serially:
 *
 *   v = x[j]
 *   v -= a[j,r]*x[r]   for r = k1..width-1 ascending   (below the block)
 *   v -= a[j,r]*x[r]   for r = j+1..k1-1 ascending     (inside the block)
 *   x[j] = v / a[j,j]
 *
 * The former loop ran r = j+1..width-1 in one ascending chain, so x[j] could
 * not start before x[j+1] was final and the whole triangle was one serial
 * add-latency chain.  Splitting the sum at k1 lets every column of a block
 * accumulate its below-block part at the same time.  Panels no wider than
 * VSDLSS_BACK_NB are one block with nothing below it and keep the former
 * order exactly; wider ones round differently but deterministically: the
 * order depends only on the width, and the SIMD dot kernels reproduce the
 * scalar loops bitwise, so results do not depend on VSDLSS_SIMD or the
 * thread count. */
#define VSDLSS_BACK_NB 32

/* One block with nothing below it: the former single ascending chain.  Kept
 * separate so it inlines into the narrow-panel callers. */
static inline vsdlss_status back_diag_small(const double *a,csi rows,csi width,double *x)
{
    for(csi j=width;j-- >0;){
        double d=a[j*rows+j],v=x[j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=a[j*rows+r]*x[r];
        x[j]=v/d;
        if(!isfinite(x[j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

static vsdlss_status back_diag_blocked(const double *a,csi rows,csi width,double *x,int simd)
{
    for(csi k0=(width-1)/VSDLSS_BACK_NB*VSDLSS_BACK_NB;;k0-=VSDLSS_BACK_NB){
        const csi k1=k0+VSDLSS_BACK_NB<width?k0+VSDLSS_BACK_NB:width,below=width-k1;
        csi j=k0;
        if(below){
            if(simd){
                for(;j+8<=k1;j+=8)(void)vsdlss_simd_dot8(a+j*rows+k1,rows,x+k1,below,x+j);
                for(;j+4<=k1;j+=4)(void)vsdlss_simd_dot4(a+j*rows+k1,rows,x+k1,below,x+j);
                if(j<k1){(void)vsdlss_simd_dot_tail(a+j*rows+k1,rows,x+k1,below,x+j,(int)(k1-j));j=k1;}
            }
            for(;j<k1;j++){
                const double *col=a+j*rows;double v=x[j];
                for(csi r=k1;r<width;r++)v-=col[r]*x[r];
                x[j]=v;
            }
        }
        for(j=k1;j-- >k0;){
            const double *col=a+j*rows;
            double d=col[j],v=x[j];
            if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
            for(csi r=j+1;r<k1;r++)v-=col[r]*x[r];
            x[j]=v/d;
            if(!isfinite(x[j]))return VSDLSS_ERR_NONFINITE;
        }
        if(k0==0)return VSDLSS_OK;
    }
}

static inline vsdlss_status back_diag(const double *a,csi rows,csi width,double *x,int simd)
{
    return width<=VSDLSS_BACK_NB?back_diag_small(a,rows,width,x)
                                :back_diag_blocked(a,rows,width,x,simd);
}

/* Diagonal block of the forward solve, J x = y, column by column (axpy of
 * each solved x[j] into the rows below it).  Shared by the push-form panel
 * solves and the pull-form forward_pull.  check_finite = 0 keeps the pull
 * contract (only pivots checked here; the caller scans the solution).
 * Measured: a register-tiled variant (triangle blocks of 16/32/64 columns,
 * rows below updated by vsdlss_simd_block_update_contig) is bitwise equal
 * but no faster at 16 and up to 1.8x slower at 32/64 for widths >= 256,
 * because the tile reads the panel with stride `rows`; the axpy form
 * streams it and already matches DTRSV. */
vsdlss_status vsdlss_panel_forward_diag(const double *a,csi rows,csi width,double *x,
                                        int simd,int check_finite)
{
    for(csi j=0;j<width;j++){
        double d=a[j*rows+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        x[j]/=d;
        if(check_finite&&!isfinite(x[j]))return VSDLSS_ERR_NONFINITE;
        if(simd)vsdlss_simd_axpy_neg(x+j+1,a+j*rows+j+1,x[j],width-j-1);
        else for(csi r=j+1;r<width;r++)x[r]-=a[j*rows+r]*x[j];
    }
    return VSDLSS_OK;
}

/* One 128-row block of the forward external update (see below). */
static int forward_ext_block(const double *a,csi rows,csi begin,csi width,csi ext,
                             const csi *index,double *x,csi b,int simd)
{
    csi t0=b*VSDLSS_SOLVE_BLK,t1=t0+VSDLSS_SOLVE_BLK,t;int bad=0;
    double v[VSDLSS_SOLVE_BLK];
    if(t1>ext)t1=ext;
    for(t=t0;t<t1;t++)v[t-t0]=x[index[t]];
    /* SIMD: eight destinations stay in registers across VSDLSS_SOLVE_JB
       columns at a time (ascending j), the same per-entry sequence as the
       column-by-column loop.  Chunking j keeps one register strip from
       touching hundreds of panel columns (measured: 2x slower at 256x4096
       without it). */
    if(simd)for(csi j0=0;j0<width;j0+=VSDLSS_SOLVE_JB)
            vsdlss_simd_block_update_contig(a+width+t0,rows,j0,j0+VSDLSS_SOLVE_JB<width?j0+VSDLSS_SOLVE_JB:width,x+begin,0,t1-t0,v);
    else for(csi j=0;j<width;j++){
        double c=x[begin+j];
        const double *col=a+j*rows+width;
        for(t=t0;t<t1;t++)v[t-t0]-=col[t]*c;
    }
    for(t=t0;t<t1;t++){
        x[index[t]]=v[t-t0];
        if(!isfinite(v[t-t0]))bad=1;
    }
    return bad;
}

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
    if(!back){
        vsdlss_status st=vsdlss_panel_forward_diag(a,rows,width,x+begin,simd,1);
        if(st!=VSDLSS_OK)return st;
    }
    /* External phase.  Both directions keep the per-element accumulation
       order of the original element-at-a-time loops and only change which
       values are reused, so every result is bitwise unchanged.
       Forward: hold one block of external destinations in registers and sweep
       j outward, turning the stride-`rows` panel reads into contiguous ones.
       Reverse: gather the external x values once instead of re-reading them
       through index[] for every column of J. */
    if(!back) {
        const csi nblocks=ext/VSDLSS_SOLVE_BLK+(ext%VSDLSS_SOLVE_BLK!=0);
        int fnt=nt; if(fnt>nblocks)fnt=(int)nblocks;
        /* No team for one block or none: entering even a one-thread
           parallel region costs more than a narrow panel's whole solve. */
        if(fnt<=1) {
            for(csi b=0;b<nblocks;b++)bad|=forward_ext_block(a,rows,begin,width,ext,index,x,b,simd);
        } else {
            VSDLSS_OMP(omp parallel num_threads(fnt) reduction(|:bad))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(static))
                for(csi b=0;b<nblocks;b++)
                    bad|=forward_ext_block(a,rows,begin,width,ext,index,x,b,simd);
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
            /* SIMD: columns in groups of eight (one tail group of four),
               one per lane, each in its original accumulation order. */
            const csi octs=simd?width/8:0,quads=simd?(width-8*octs)/4:0;
            VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(static))
                for(csi q=0;q<octs+quads;q++){
                    csi c=q<octs?8*q:8*octs+4*(q-octs);
                    if(q<octs)bad|=vsdlss_simd_dot8(a+c*rows+width,rows,xg,ext,x+begin+c);
                    else bad|=vsdlss_simd_dot4(a+c*rows+width,rows,xg,ext,x+begin+c);
                }
                const csi t0=8*octs+4*quads;
                if(simd){
                    /* the last 1..3 columns: one SIMD call */
                    VSDLSS_OMP(omp single)
                    if(t0<width)
                        bad|=vsdlss_simd_dot_tail(a+t0*rows+width,rows,xg,ext,x+begin+t0,(int)(width-t0));
                } else {
                    VSDLSS_OMP(omp for schedule(static))
                    for(csi t=0;t<width;t++){
                        double v=x[begin+t];
                        const double *row_t=a+t*rows+width;
                        for(csi r=0;r<ext;r++)v-=row_t[r]*xg[r];
                        x[begin+t]=v;if(!isfinite(v))bad=1;
                    }
                }
            }
            if(xg!=stack_gather)free(xg);
        }
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    if(back)return back_diag(a,rows,width,x+begin,simd);
    return VSDLSS_OK;
}

/* External x values of a backward panel, in row order.  index[] is strictly
 * increasing, so when it is one contiguous range the values are used in
 * place instead of copied.  (Copying runs of four as blocks was measured
 * slower than the plain loop: the run test mispredicts.)  Pure data
 * movement: results are unchanged. */
static inline const double *gather_ext(const double *x,const csi *index,csi ext,double *buf)
{
    if(index[ext-1]-index[0]==ext-1)return x+index[0];
    for(csi r=0;r<ext;r++)buf[r]=x[index[r]];
    return buf;
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
        const int contig=index[ext-1]-index[0]==ext-1;
        double *buf=contig||ext<=VSDLSS_SOLVE_GATHER?stack_gather:(double*)malloc((size_t)ext*sizeof(double));
        if(!buf)return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,1);
        const double *xg=gather_ext(x,index,ext,buf);
        csi q=0;
        for(;q+8<=width;q+=8)bad|=vsdlss_simd_dot8(a+q*rows+width,rows,xg,ext,x+begin+q);
        for(;q+4<=width;q+=4)bad|=vsdlss_simd_dot4(a+q*rows+width,rows,xg,ext,x+begin+q);
        if(q<width)bad|=vsdlss_simd_dot_tail(a+q*rows+width,rows,xg,ext,x+begin+q,(int)(width-q));
        if(buf!=stack_gather)free(buf);
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    return back_diag(a,rows,width,x+begin,1);
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

/* Backward solves visit supernodes in descending order, and the factor
 * stores panels contiguously in ascending order, so the next panel ends
 * right below this one.  Hint the hardware with that region while this
 * panel is solved: min(this panel's size, 8 KB).  Prefetches cannot fault;
 * for a panel that is not part of such a layout (M4 disk blocks) they are
 * only wasted hints.  Measured on the 8M dual-net core (METIS): backward
 * 209 -> 183 ms, width 7-32 -17%; reading the same panels descending costs
 * 28% more than ascending without it.  Caps of 4/16/64 KB: 187/185/183 ms,
 * 64 KB made width 33-128 8% slower. */
#define VSDLSS_BACK_PREFETCH 8192
static inline void back_prefetch(const double *a,csi rows,csi width)
{
    size_t len=(size_t)rows*(size_t)width*sizeof(double);
    if(len>VSDLSS_BACK_PREFETCH)len=VSDLSS_BACK_PREFETCH;
    const char *p=(const char *)a;
    for(size_t o=64;o<=len;o+=64)__builtin_prefetch(p-o,0,3);
}

vsdlss_status vsdlss_panel_solve(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
    if(back)back_prefetch(a,width+ext,width);
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

/* Forward solve of one panel restricted to its first `prefix` external rows
 * (rows = width + ext stays the leading dimension).  Used by the subtree
 * phase of the tree-parallel solve: rows inside the thread's own subtree are
 * pushed here, the rest are pulled later by their tree-top targets.  Same
 * per-entry operation sequence as vsdlss_panel_solve(back = 0); serial. */
vsdlss_status vsdlss_panel_forward_prefix(const double *a,csi begin,csi width,csi ext,
                                          csi prefix,const csi *index,double *x)
{
    const csi rows=width+ext;
    const int simd=vsdlss_simd_enabled();
    int bad=0;
    vsdlss_status st=vsdlss_panel_forward_diag(a,rows,width,x+begin,simd,1);
    if(st!=VSDLSS_OK)return st;
    const csi nblocks=prefix/VSDLSS_SOLVE_BLK+(prefix%VSDLSS_SOLVE_BLK!=0);
    for(csi b=0;b<nblocks;b++)bad|=forward_ext_block(a,rows,begin,width,prefix,index,x,b,simd);
    return bad?VSDLSS_ERR_NONFINITE:VSDLSS_OK;
}
