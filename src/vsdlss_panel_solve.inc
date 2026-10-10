/* Panel triangular solve, included once per panel layout by vsdlss_panel.c.
 *
 * The including file defines:
 *   PS(name)  function name for this layout
 *   IDX_T     row-index type
 *   DCOL(j)   pointer to the diagonal entry of column j of the diagonal
 *             block; entry (r, j), r >= j, is DCOL(j)[r - j]
 *   RCOL(j)   pointer to column j of the external rows (ext entries)
 *   RLD       distance between consecutive external columns
 *   PACKED    1 when the diagonal block is packed (BLAS: dtpsv, not dtrsv;
 *             no contiguous in-panel + external column for KV=3)
 *   BLOCK_UPDATE  vsdlss_simd_block_update for IDX_T row indices
 *   SMALL(w), DG(w,j,r), RC(w,j) for the generated narrow kernels
 * The variables a, width, ext and rows are in scope wherever the macros are
 * used.  Both layouts run exactly the same operations in the same order. */

/* Forward diagonal block with fused four-column updates: columns j0..j0+3
 * are solved in order (each updates the rows of its own block), then the
 * rows below the block get the four subtractions in the same order as four
 * separate axpy calls.  Same operations per entry: same bits. Returns 1 on
 * an invalid pivot, 2 on a non-finite value. */
static int PS(fwd_diag4)(const double *a,csi width,csi ext,double *xb)
{
    csi rows=width+ext; (void)rows; (void)ext;
    csi j0=0;
    for(;j0+4<=width;j0+=4){
        for(csi j=j0;j<j0+4;j++){
            const double *dj=DCOL(j);
            double d=dj[0];
            if(!isfinite(d)||d<=0)return 1;
            xb[j]/=d;
            if(!isfinite(xb[j]))return 2;
            for(csi r=j+1;r<j0+4;r++){ double u=dj[r-j]*xb[j]; xb[r]-=u; }
        }
        /* rows j0+4.. of columns j0..j0+3 */
        vsdlss_simd_axpy4p_neg(xb+j0+4,DCOL(j0)+4,DCOL(j0+1)+3,DCOL(j0+2)+2,DCOL(j0+3)+1,
                               xb+j0,width-j0-4);
    }
    for(csi j=j0;j<width;j++){
        const double *dj=DCOL(j);
        double d=dj[0];
        if(!isfinite(d)||d<=0)return 1;
        xb[j]/=d;
        if(!isfinite(xb[j]))return 2;
        vsdlss_simd_axpy_neg(xb+j+1,dj+1,xb[j],width-j-1);
    }
    return 0;
}

#if PACKED
/* Backward triangle J^T x = y with the blocked kernel (opt-in, see
 * vsdlss_panel.c): pivots are checked before x is touched, non-finite
 * results by one scan afterwards; the status codes of the default loop. */
static vsdlss_status PS(tri_back)(const double *a,csi begin,csi width,double *x)
{
    for(csi j=0;j<width;j++){ double d=DCOL(j)[0]; if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID; }
    tri_solve(a,width,x+begin,1);
    for(csi j=0;j<width;j++) if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    return VSDLSS_OK;
}
#endif

vsdlss_status PS(generic)(const double *a,csi begin,csi width,
                          csi ext,const IDX_T *index,double *x,int back)
{
    csi rows=width+ext; (void)rows;
    int nt=vsdlss_parallel_width((double)width*ext),bad=0;(void)nt;
    const int simd=vsdlss_simd_enabled(), kv2=solve_kv()>=2;
    /* External rows have no dependency on J until the triangular solve ends.
       Reverse solve applies the external contribution before solving J^T. */
    if(!back && simd && kv2){
        int e=PS(fwd_diag4)(a,width,ext,x+begin);
        if(e)return e==1?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE;
    } else if(!back)for(csi j=0;j<width;j++){
        const double *dj=DCOL(j);
        double d=dj[0];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        x[begin+j]/=d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
        if(simd)vsdlss_simd_axpy_neg(x+begin+j+1,dj+1,x[begin+j],width-j-1);
        else for(csi r=j+1;r<width;r++)x[begin+r]-=dj[r-j]*x[begin+j];
    }
    /* External phase.  Both directions keep the per-element accumulation
       order of the original element-at-a-time loops and only change which
       values are reused, so every result is bitwise unchanged.
       Forward: hold one block of external destinations in registers and sweep
       j outward, turning the strided panel reads into contiguous ones.
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
                    vsdlss_simd_axpy4_neg(v,RCOL(j0)+t0,RLD,x+begin+j0,t1-t0);
                for(csi j=j0;j<width;j++){
                    double c=x[begin+j];
                    const double *col=RCOL(j);
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
                    const double *row_t=RCOL(t);
                    for(csi r=0;r<ext;r++)v-=row_t[r]*x[index[r]];
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
                    bad|=vsdlss_simd_dot4(RCOL(4*q),RLD,xg,ext,x+begin+4*q);
                VSDLSS_OMP(omp for schedule(static))
                for(csi t=4*quads;t<width;t++){
                    double v=x[begin+t];
                    const double *row_t=RCOL(t);
                    for(csi r=0;r<ext;r++)v-=row_t[r]*xg[r];
                    x[begin+t]=v;if(!isfinite(v))bad=1;
                }
            }
            if(xg!=stack_gather)free(xg);
        }
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
#if PACKED
    if(back && vsdlss_sn_panel_tri_blocked(width))return PS(tri_back)(a,begin,width,x);
#endif
    if(back)for(csi j=width;j-- >0;){
        const double *dj=DCOL(j);
        double d=dj[0],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=dj[r-j]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

#include "vsdlss_small_solve.inc"

/* Backward triangle J^T x = y of the diagonal block (shared tail). */
static inline vsdlss_status PS(back_diag)(const double *a,csi begin,csi width,csi ext,double *x)
{
    csi rows=width+ext; (void)rows; (void)ext;
#if PACKED
    if(vsdlss_sn_panel_tri_blocked(width))return PS(tri_back)(a,begin,width,x);
#endif
    for(csi j=width;j-- >0;){
        const double *dj=DCOL(j);
        double d=dj[0],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=dj[r-j]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

/* Backward solve of one panel on one thread with the SIMD dot products:
   the arithmetic and status results of the generic kernel (back).  dot8:
   columns in groups of eight first (KV=2 kernel), else groups of four. */
static vsdlss_status PS(simd_back)(const double *a,csi begin,csi width,
                                   csi ext,const IDX_T *index,double *x,int dot8)
{
    csi rows=width+ext; int bad=0; (void)rows;
    if(ext){
        double stack_gather[VSDLSS_SOLVE_GATHER];
        double *xg=ext<=VSDLSS_SOLVE_GATHER?stack_gather:(double*)malloc((size_t)ext*sizeof(double));
        if(!xg)return PS(generic)(a,begin,width,ext,index,x,1);
        for(csi r=0;r<ext;r++)xg[r]=x[index[r]];
        csi q=0;
        if(dot8)for(;q+8<=width;q+=8)bad|=vsdlss_simd_dot8(RCOL(q),RLD,xg,ext,x+begin+q);
        for(;q+4<=width;q+=4)bad|=vsdlss_simd_dot4(RCOL(q),RLD,xg,ext,x+begin+q);
        for(csi t=q;t<width;t++){
            double v=x[begin+t];
            const double *row_t=RCOL(t);
            for(csi r=0;r<ext;r++)v-=row_t[r]*xg[r];
            x[begin+t]=v;if(!isfinite(v))bad=1;
        }
        if(xg!=stack_gather)free(xg);
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    return PS(back_diag)(a,begin,width,ext,x);
}

/* KV=2 forward: fused diagonal, then every external row updated with eight
   destinations held in registers across all columns. */
static vsdlss_status PS(fwd_kv2)(const double *a,csi begin,csi width,csi ext,const IDX_T *index,double *x)
{
    csi rows=width+ext; (void)rows;
    { int e=PS(fwd_diag4)(a,width,ext,x+begin); if(e)return e==1?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE; }
    if(ext){
        int bad=0;
        BLOCK_UPDATE(RCOL(0),RLD,width,x+begin,index,0,ext,x);
        for(csi t=0;t<ext;t++) bad|=!isfinite(x[index[t]]);
        if(bad)return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

#if !PACKED
/* VSDLSS_SOLVE_KV=3 (opt-in, NOT bitwise equal to the default kernels):
 * backward solve with a relaxed summation order.  Columns are processed in
 * blocks of eight from the bottom; for each block the in-panel rows below
 * it and the external rows (contiguous in every column of the block) are
 * one dot8 over a gathered vector xx = (solved in-panel x, gathered
 * external x); only the 8x8 diagonal block stays serial.  Per entry the
 * terms are the same, subtracted in a different order: in-panel rows below
 * the block, external rows, then the rows inside the block.  Same pivot and
 * finite checks.  Full layout only (a packed panel's column is not one
 * contiguous run); packed panels use the KV=2 kernel. */
static vsdlss_status PS(back_relaxed)(const double *a,csi begin,csi width,
                                      csi ext,const IDX_T *index,double *x)
{
    const csi rows=width+ext; int bad=0;
    double stack_xx[VSDLSS_SOLVE_GATHER];
    double *xx=rows<=VSDLSS_SOLVE_GATHER?stack_xx:(double*)malloc((size_t)rows*sizeof(double));
    if(!xx) return PS(simd_back)(a,begin,width,ext,index,x,0);
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
#define PS_BACK_KV3(a,begin,width,ext,index,x) PS(back_relaxed)(a,begin,width,ext,index,x)
#else
#define PS_BACK_KV3(a,begin,width,ext,index,x) PS(simd_back)(a,begin,width,ext,index,x,1)
#endif

#ifdef VSDLSS_BLAS
#if PACKED
#define PS_TRSV(tr, xb) dtpsv_("L", tr, "N", &W, a, xb, &one)
#else
#define PS_TRSV(tr, xb) dtrsv_("L", tr, "N", &W, a, &LDD, xb, &one)
#endif
/* Forward BLAS step of one wide panel without the scatter: pivot checks,
 * dtrsv (dtpsv for the packed layout) on the diagonal block, then
 * t = L_ext * x_J with one dgemv (beta 0) over all ext external rows.
 * xb points at the panel's own entries of x.  The serial solve subtracts
 * t[r] from x[index[r]] right away; the tree solve keeps t and lets every
 * target subtract its rows when it is solved.  Both apply the same t with
 * one subtraction per entry, so they give the same bits.  Returns INVALID
 * before touching xb on a bad pivot. */
static vsdlss_status PS(blas_fwd)(const double *a, csi width, csi ext, double *xb, double *t)
{
    const csi rows = width + ext; (void)rows;
    int W = (int)width, E = (int)ext, LD = (int)(RLD > 0 ? RLD : 1), one = 1;
    double p1 = 1.0, zero = 0.0;
#if !PACKED
    int LDD = (int)rows;
#endif
    for (csi j = 0; j < width; j++) {
        double d = DCOL(j)[0];
        if (!isfinite(d) || d <= 0) return VSDLSS_ERR_INVALID;
    }
#if PACKED
    if (vsdlss_sn_panel_tri_blocked(width)) {
        /* Blocked triangle (opt-in): the built-in forward kernels, which a
         * team can share, in place of dtpsv. */
        tri_solve(a, width, xb, 0);
        if (E) {
            VSDLSS_BLAS_GUARD
            dgemv_("N", &E, &W, &p1, RCOL(0), &LD, xb, &one, &zero, t, &one);
        }
        return VSDLSS_OK;
    }
#endif
    VSDLSS_BLAS_GUARD
    {
        PS_TRSV("N", xb);
        if (E) dgemv_("N", &E, &W, &p1, RCOL(0), &LD, xb, &one, &zero, t, &one);
    }
    return VSDLSS_OK;
}

/* Wide panels (at least VSDLSS_BLAS_SOLVE_MIN columns) use dtrsv (dtpsv
 * for the packed layout) + dgemv.  Same pivot checks as the built-in path;
 * a non-finite result is reported for J here and, for the external rows,
 * by the caller's scan of the solution. */
static vsdlss_status PS(blas)(const double *a, csi begin, csi width,
                              csi ext, const IDX_T *index, double *x, int back)
{
    const csi rows = width + ext; (void)rows;
    int W = (int)width, E = (int)ext, LD = (int)(RLD > 0 ? RLD : 1), one = 1;
    double p1 = 1.0, m1 = -1.0;
#if !PACKED
    int LDD = (int)rows;
#endif
    for (csi j = 0; j < width; j++) {
        double d = DCOL(j)[0];
        if (!isfinite(d) || d <= 0) return VSDLSS_ERR_INVALID;
    }
    /* t starts on a 64-byte boundary, as the tree solve's stored products
     * do: with the same panel and x, every BLAS call then sees the same
     * operand alignment in both schedules. */
    _Alignas(64) double stack[VSDLSS_SOLVE_GATHER];
    double *heap = ext <= VSDLSS_SOLVE_GATHER ? NULL : (double *)malloc(((size_t)ext + 8) * sizeof(double));
    double *t = heap ? (double *)(((uintptr_t)heap + 63) & ~(uintptr_t)63) : stack;
    if (ext > VSDLSS_SOLVE_GATHER && !heap) return PS(generic)(a, begin, width, ext, index, x, back);
    if (!back) {
        (void)PS(blas_fwd)(a, width, ext, x + begin, t);   /* pivots checked above */
        for (csi r = 0; r < ext; r++) x[index[r]] -= t[r];
    } else {
        int tri = 0;
#if PACKED
        tri = vsdlss_sn_panel_tri_blocked(width);     /* blocked triangle (opt-in) in place of dtpsv */
#endif
        VSDLSS_BLAS_GUARD
        {
            if (E) {
                for (csi r = 0; r < ext; r++) t[r] = x[index[r]];
                dgemv_("T", &E, &W, &m1, RCOL(0), &LD, t, &one, &p1, x + begin, &one);
            }
            if (!tri) PS_TRSV("T", x + begin);
        }
#if PACKED
        if (tri) tri_solve(a, width, x + begin, 1);
#endif
    }
    free(heap);
    for (csi j = 0; j < width; j++) if (!isfinite(x[begin + j])) return VSDLSS_ERR_NONFINITE;
    return VSDLSS_OK;
}
#undef PS_TRSV
#endif

static vsdlss_status PS(dispatch)(const double *a,csi begin,csi width,
                                  csi ext,const IDX_T *index,double *x,int back)
{
#ifdef VSDLSS_BLAS
    { csi m=solve_blas_min();
      if(m && width>=m && width+ext<INT_MAX) return PS(blas)(a,begin,width,ext,index,x,back); }
#endif
    /* Keep large external-row work on the existing parallel path. */
    if(vsdlss_parallel_width((double)width*ext)>1)
        return PS(generic)(a,begin,width,ext,index,x,back);
    if(solve_kv()>=2 && vsdlss_simd_enabled()) {
        if(back && width>=8) return solve_kv()>=3?PS_BACK_KV3(a,begin,width,ext,index,x)
                                                 :PS(simd_back)(a,begin,width,ext,index,x,1);
        /* Wider panels: the register kernel reads each column 64 bytes at a
         * time and loses to the generic 128-row blocks (measured 0.5x at
         * w = 256-512, break-even between 128 and 160). */
        if(!back && width>=7 && width<=VSDLSS_KV2_FWD_MAX) return PS(fwd_kv2)(a,begin,width,ext,index,x);
    }
    /* Backward, width >= 4 (KV>=2: >= 7): four-column SIMD dot products
       (same arithmetic as the generated scalar kernels), without the
       generic kernel's OpenMP region, whose entry cost would dominate
       narrow panels.  KV>=2 keeps the generated scalar kernels for widths
       4-6 in both directions (1.2-1.7x faster than the four-column SIMD
       path, whose gather and transposes do not pay off at <= 6 columns). */
    if(back && width>=(solve_kv()>=2?7:4) && vsdlss_simd_enabled())
        return PS(simd_back)(a,begin,width,ext,index,x,0);
    switch(width) {
#define CASE(N) case N: return SMALL(N)(a,begin,ext,index,x,back)
        CASE(1); CASE(2); CASE(3); CASE(4); CASE(5); CASE(6);
#undef CASE
        default: return PS(generic)(a,begin,width,ext,index,x,back);
    }
}
#undef PS_BACK_KV3
