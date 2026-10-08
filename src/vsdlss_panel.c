#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"
#include "vsdlss_simd.h"
#include "vsdlss_dense.h"

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
