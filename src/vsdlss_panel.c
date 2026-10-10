#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"
#include "vsdlss_simd.h"
#include "vsdlss_dense.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define VSDLSS_SOLVE_BLK 128
#define VSDLSS_SOLVE_GATHER 1024

/* Panel factorization is the blocked dense kernel shared with M4. */
vsdlss_status vsdlss_panel_factor(double *a, csi rows, csi width)
{
    return vsdlss_dense_potrf_panel(a, rows, width);
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
 * and the same status codes.  KV=3 (opt-in, full layout only) relaxes the
 * backward summation order. */
#define VSDLSS_KV2_FWD_MAX 128
int vsdlss_solve_kv = -1;
static int solve_kv(void)
{
    if (vsdlss_solve_kv < 0) { const char *e = getenv("VSDLSS_SOLVE_KV"); vsdlss_solve_kv = e ? atoi(e) : 2; }
    return vsdlss_solve_kv;
}

/* Forward external updates of packed (M3) panels -- the serial fwd_kv2
 * kernel and the tree solve's pull and push steps -- use
 * vsdlss_simd_block_update16: 16 destinations in registers and the last 1..3
 * rows as interleaved chains.  VSDLSS_SOLVE_UPD16=0 selects the 8-row
 * vsdlss_simd_block_update.  Same operations per entry: same bits. */
int vsdlss_solve_upd16(void)
{
    static int v = -1;
    if (v < 0) { const char *e = getenv("VSDLSS_SOLVE_UPD16"); v = !(e && e[0] == '0'); }
    return v;
}

/* ---- Blocked triangle steps of packed panels (opt-in) --------------------
 * VSDLSS_SOLVE_TRI_BLK = B (columns; 0 or unset: off) solves the diagonal
 * block of a packed panel at least TRI_MIN columns wide with the two
 * kernels below instead of the column-at-a-time loops (and instead of dtpsv
 * in BLAS builds).  Both give the same bits for every thread count and with
 * or without AVX2.
 *
 * Backward (L^T x = y).  The default loop computes column j as one chain
 * over rows j+1 .. w-1 in ascending order, which no two columns can share
 * and no team can split.  Here the columns are taken in blocks of B from
 * the bottom and, inside a block, in groups of eight from the bottom; for
 * column j of group [g0, g1) of block [jb, je):
 *   x[j] = (((x[j] - sum over rows >= je) - sum over rows g1 .. je-1)
 *                  - sum over rows j+1 .. g1-1) / L(j, j),
 * each sum in ascending row order.  The first sum only reads finished
 * blocks, so the columns of a block take it concurrently (eight chains per
 * kernel call, column ranges on different threads); the other two stay on
 * one thread.  The terms are those of the default loop in another order:
 * the result differs from it in rounding and depends on B, nothing else.
 *
 * Forward (L x = y).  Blocks of TRI_FWD_BLK columns: the block's columns
 * one after another on its own rows, then every row below the block gets
 * the block's columns in ascending order, row ranges on different threads.
 * Per entry these are the operations of the built-in forward kernels in
 * their order, so the bits are theirs for any block size (BLAS builds: not
 * those of dtpsv).
 *
 * A team: child tasks when the caller's team runs tasks
 * (vsdlss_parallel_tasks), else an own team when vsdlss_parallel_width
 * grants one.  VSDLSS_SOLVE_TRI_WORK: multiply-adds of a block's shared
 * step from which it is split (default 16384). */
#define TRI_MIN 16
#ifndef TRI_FWD_BLK
#define TRI_FWD_BLK 64
#endif
int vsdlss_solve_tri_blk = -1;
static csi tri_blk(void)
{
    if (vsdlss_solve_tri_blk < 0) {
        const char *e = getenv("VSDLSS_SOLVE_TRI_BLK");
        long v = e ? atol(e) : 0;
        vsdlss_solve_tri_blk = v <= 0 ? 0 : v > 1 << 20 ? 1 << 20 : (int)v;
    }
    return vsdlss_solve_tri_blk > 0 ? ((csi)vsdlss_solve_tri_blk + 7) / 8 * 8 : 0;
}
int vsdlss_sn_panel_tri_blocked(csi width) { return width >= TRI_MIN && tri_blk() > 0; }
/* Shared steps of the blocked triangles that were cut into pieces for a
 * team (process-wide; tests check that a team is actually used). */
static atomic_long tri_split_count;
long vsdlss_sn_panel_tri_splits(void) { return atomic_load_explicit(&tri_split_count, memory_order_relaxed); }
#ifdef _OPENMP
static double tri_par_work(void)
{
    static double v = -1;
    if (v < 0) { const char *e = getenv("VSDLSS_SOLVE_TRI_WORK"); v = e && atof(e) > 0 ? atof(e) : 16384.0; }
    return v;
}
#endif

/* x[j] -= sum_{r in [r0, r0+n)} D(r, j) x[r] for columns [j0, j1), r0 >= j1. */
static inline void tri_dots(const double *D, csi w, csi j0, csi j1, csi r0, csi n, double *x, int simd)
{
    if (n <= 0 || j1 <= j0) return;
    if (simd) { vsdlss_simd_tri_dots_packed(D, w, j0, j1, r0, n, x + r0, x + j0); return; }
    for (csi j = j0; j < j1; ++j) {
        const double *c = D + VSDLSS_SN_DCOL(j, w) + (r0 - j);
        double v = x[j];
        for (csi i = 0; i < n; ++i) v -= c[i] * x[r0 + i];
        x[j] = v;
    }
}

/* Pieces a shared step of `work` multiply-adds over `units` items is cut
 * into: one, or the team size when the caller has a team (par). */
static inline csi tri_pieces(int par, double work, csi units)
{
    csi n = 1;
#ifdef _OPENMP
    if (par && work >= tri_par_work()) { n = omp_get_num_threads(); if (n > units) n = units; if (n < 1) n = 1; }
    if (n > 1) atomic_fetch_add_explicit(&tri_split_count, 1, memory_order_relaxed);
#else
    (void)par; (void)work; (void)units;
#endif
    return n;
}

static void tri_back_run(const double *D, csi w, double *x, csi B, int par)
{
    const int simd = vsdlss_simd_enabled();
    for (csi k = (w + B - 1) / B; k-- > 0;) {
        const csi jb = k * B, je = jb + B < w ? jb + B : w, n = w - je, cols = je - jb;
        if (n > 0) {
            /* rows below the block: whole groups of eight columns per piece */
            const csi np = tri_pieces(par, (double)cols * (double)n, cols / 8);
            for (csi c = 1; c < np; ++c) {
                const csi c0 = jb + cols * c / np / 8 * 8, c1 = c + 1 < np ? jb + cols * (c + 1) / np / 8 * 8 : je;
                VSDLSS_OMP(omp task firstprivate(c0, c1))
                tri_dots(D, w, c0, c1, je, n, x, simd);
            }
            tri_dots(D, w, jb, np > 1 ? jb + cols / np / 8 * 8 : je, je, n, x, simd);
            if (np > 1) { VSDLSS_OMP(omp taskwait) }
        }
        for (csi g = (cols + 7) / 8; g-- > 0;) {
            const csi g0 = jb + 8 * g, g1 = g0 + 8 < je ? g0 + 8 : je;
            tri_dots(D, w, g0, g1, g1, je - g1, x, simd);
            for (csi j = g1; j-- > g0;) {
                const double *dj = D + VSDLSS_SN_DCOL(j, w);
                double v = x[j];
                for (csi r = j + 1; r < g1; ++r) v -= dj[r - j] * x[r];
                x[j] = v / dj[0];
            }
        }
    }
}

/* Columns [jb, je) of the packed triangle applied to rows [r0, r1), all
 * below je: x[r] -= D(r, j) * x[j], ascending j for every row.  Four
 * columns per sweep, each read front to back. */
static void tri_fwd_rows(const double *D, csi w, csi jb, csi je, csi r0, csi r1, double *x, int simd)
{
    csi j = jb;
    if (r1 <= r0) return;
    if (simd) for (; j + 4 <= je; j += 4)
        vsdlss_simd_axpy4p_neg(x + r0, D + VSDLSS_SN_DCOL(j, w) + (r0 - j), D + VSDLSS_SN_DCOL(j + 1, w) + (r0 - j - 1),
                               D + VSDLSS_SN_DCOL(j + 2, w) + (r0 - j - 2), D + VSDLSS_SN_DCOL(j + 3, w) + (r0 - j - 3),
                               x + j, r1 - r0);
    for (; j < je; ++j) {
        const double *c = D + VSDLSS_SN_DCOL(j, w) + (r0 - j);
        if (simd) vsdlss_simd_axpy_neg(x + r0, c, x[j], r1 - r0);
        else for (csi r = r0; r < r1; ++r) x[r] -= c[r - r0] * x[j];
    }
}

static void tri_fwd_run(const double *D, csi w, double *x, int par)
{
    const int simd = vsdlss_simd_enabled();
    for (csi jb = 0; jb < w; jb += TRI_FWD_BLK) {
        const csi je = jb + TRI_FWD_BLK < w ? jb + TRI_FWD_BLK : w, n = w - je;
        for (csi j = jb; j < je; ++j) {
            const double *aj = D + VSDLSS_SN_DCOL(j, w);
            x[j] /= aj[0];
            if (simd) vsdlss_simd_axpy_neg(x + j + 1, aj + 1, x[j], je - j - 1);
            else for (csi r = j + 1; r < je; ++r) x[r] -= aj[r - j] * x[j];
        }
        if (n <= 0) break;
        const csi np = tri_pieces(par, (double)(je - jb) * (double)n, n / 16);
        /* Every piece is swept once per four columns, so two pieces must
         * not share a cache line of x: cuts go where x starts a line. */
        const csi lead = (csi)((64 - ((uintptr_t)(x + je) & 63)) & 63) / 8;
#define TRI_CUT(c) ((c) >= np ? w : je + lead + (n - lead) * (c) / np / 8 * 8)
        for (csi c = 1; c < np; ++c) {
            const csi r0 = TRI_CUT(c), r1 = TRI_CUT(c + 1);
            VSDLSS_OMP(omp task firstprivate(r0, r1))
            tri_fwd_rows(D, w, jb, je, r0, r1, x, simd);
        }
        tri_fwd_rows(D, w, jb, je, je, np > 1 ? TRI_CUT(1) : w, x, simd);
#undef TRI_CUT
        if (np > 1) { VSDLSS_OMP(omp taskwait) }
    }
}

/* The diagonal block D (order w, packed, pivots already checked) applied to
 * x[0..w): L^T x = y (back) or L x = y. */
static void tri_solve(const double *D, csi w, double *x, int back)
{
    const csi B = tri_blk();
    int par = 0;
#ifdef _OPENMP
    if (vsdlss_parallel_tasks()) par = omp_get_num_threads() > 1;
    else {
        const int nt = vsdlss_parallel_width((double)w * (double)w / 2);
        if (nt > 1) {
            VSDLSS_OMP(omp parallel num_threads(nt))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp single)
                {
                    const int team = omp_get_num_threads() > 1;
                    if (back) tri_back_run(D, w, x, B, team); else tri_fwd_run(D, w, x, team);
                }
            }
            return;
        }
    }
#endif
    if (back) tri_back_run(D, w, x, B, par); else tri_fwd_run(D, w, x, par);
}

#ifdef VSDLSS_BLAS
/* Wide panels (at least VSDLSS_BLAS_SOLVE_MIN columns, default 32; 0
 * disables) are solved with BLAS. */
#include <limits.h>
#include <stdatomic.h>
void dtrsv_(const char *, const char *, const char *, const int *, const double *,
            const int *, double *, const int *);
void dtpsv_(const char *, const char *, const char *, const int *, const double *,
            double *, const int *);
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
/* Diagnostic builds for a BLAS that is not thread safe (see
 * vsdlss_supernodal_numeric.c): one BLAS call at a time. */
#ifdef VSDLSS_BLAS_SERIALIZE
#define VSDLSS_BLAS_GUARD VSDLSS_OMP(omp critical(vsdlss_blas))
#else
#define VSDLSS_BLAS_GUARD
#endif
#endif

/* Full layout (M4 disk panels): column-major rows-by-width panel, the
 * diagonal block's strictly upper part unused; 64-bit row indices. */
#define PS(name) vsdlss_panel_solve_##name
#define IDX_T csi
#define DCOL(j) (a+(j)*rows+(j))
#define RCOL(j) (a+(j)*rows+width)
#define RLD rows
#define PACKED 0
#define BLOCK_UPDATE vsdlss_simd_block_update64
#define SMALL(w) solve_##w
#define DG(w,j,r) a[(j)*rows+(r)]
#define RC(w,j) (a+(j)*rows+(w))
#include "vsdlss_panel_solve.inc"
#undef PS
#undef IDX_T
#undef DCOL
#undef RCOL
#undef RLD
#undef PACKED
#undef BLOCK_UPDATE
#undef SMALL
#undef DG
#undef RC

vsdlss_status vsdlss_panel_solve(const double *a,csi begin,csi width,
                                 csi ext,const csi *index,double *x,int back)
{ return vsdlss_panel_solve_dispatch(a,begin,width,ext,index,x,back); }

/* Packed layout (M3 supernodal factor, see vsdlss_sn_factor): the lower
 * triangle of the diagonal block by columns, then the external rows
 * column-major with leading dimension ext; 32-bit row indices. */
#define PS(name) vsdlss_sn_panel_solve_##name
#define IDX_T vsdlss_sni
#define DCOL(j) (a+VSDLSS_SN_DCOL(j,width))
#define RCOL(j) (a+width*(width+1)/2+(j)*ext)
#define RLD ext
#define PACKED 1
#define BLOCK_UPDATE sn_block_update
static inline void sn_block_update(const double *as, csi rs, csi ws, const double *xs,
                                   const vsdlss_sni *R, csi r0, csi r1, double *x)
{
    if (vsdlss_solve_upd16()) vsdlss_simd_block_update16(as, rs, ws, xs, R, r0, r1, x);
    else vsdlss_simd_block_update(as, rs, ws, xs, R, r0, r1, x);
}
#define SMALL(w) psolve_##w
#define DG(w,j,r) a[VSDLSS_SN_DCOL(j,w)+(r)-(j)]
#define RC(w,j) (a+(w)*((w)+1)/2+(j)*ext)
#include "vsdlss_panel_solve.inc"
#undef PS
#undef IDX_T
#undef DCOL
#undef RCOL
#undef RLD
#undef PACKED
#undef BLOCK_UPDATE
#undef SMALL
#undef DG
#undef RC

vsdlss_status vsdlss_sn_panel_solve(const double *a,csi begin,csi width,
                                    csi ext,const vsdlss_sni *index,double *x,int back)
{ return vsdlss_sn_panel_solve_dispatch(a,begin,width,ext,index,x,back); }

#ifdef VSDLSS_BLAS
int vsdlss_sn_panel_solve_is_blas(csi width,csi ext)
{ csi m=solve_blas_min(); return m && width>=m && width+ext<INT_MAX; }
vsdlss_status vsdlss_sn_panel_blas_forward(const double *a,csi width,csi ext,double *xb,double *t)
{ return vsdlss_sn_panel_solve_blas_fwd(a,width,ext,xb,t); }
#endif
