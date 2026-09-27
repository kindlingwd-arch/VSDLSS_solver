#ifndef VSDLSS_SIMD_H
#define VSDLSS_SIMD_H

#include "vsdlss.h"

/* 1 when the AVX2 solve kernels are used (CPU support, VSDLSS_SIMD != 0).
 * The kernels give bitwise the results of the scalar loops they replace. */
int vsdlss_simd_enabled(void);

/* Source-block update of the forward solve, rows R[r0..r1) (ascending):
 *   for j in [0, ws): for r: x[R[r]] -= as[j*rs + r] * xs[j]
 * with each x[R[r]] updated in ascending j. */
void vsdlss_simd_block_update(const double *as, csi rs, csi ws, const double *xs,
                              const csi *R, csi r0, csi r1, double *x);
/* Same with contiguous targets xt[r0..r1) and columns [j0, j1):
 *   xt[r] -= a[j*ld + r] * xs[j], ascending j. */
void vsdlss_simd_block_update_contig(const double *a, csi ld, csi j0, csi j1, const double *xs,
                                     csi r0, csi r1, double *xt);
/* y[i] -= c[i] * s for i in [0, n). */
void vsdlss_simd_axpy_neg(double *y, const double *c, double s, csi n);
/* Four backward dot products, columns c0 + k*ld (k = 0..3):
 *   v[k] -= c_k[r] * xg[r] for r ascending in [0, n).
 * Returns 1 when a result is not finite. */
int vsdlss_simd_dot4(const double *c0, csi ld, const double *xg, csi n, double *v);

/* Forward external update of one panel, all rows at once:
 *   for r in [0, n): for j in [0, ws): x[R[r]] -= a[j*ld + r] * xs[j]
 * Up to 16 destinations stay in registers across every column j (the
 * per-entry order is ascending j, as in the scalar loop).  R must be
 * strictly ascending (a run of four is detected as R[r+3]-R[r]==3) and must
 * not contain an index of xs.  Returns 1 when a result is not finite. */
int vsdlss_simd_ext_update(const double *a, csi ld, csi ws, const double *xs,
                           const csi *R, csi n, double *x);
/* Eight backward dot products (columns c0 + k*ld, k = 0..7), two independent
 * four-lane accumulators sharing each broadcast of xg[r]:
 *   v[k] -= c_k[r] * xg[r] for r ascending.  Returns 1 if not finite. */
int vsdlss_simd_dot8(const double *c0, csi ld, const double *xg, csi n, double *v);
/* vsdlss_simd_dot4 with the insert/unpack row assembly of dot8. */
int vsdlss_simd_dot4b(const double *c0, csi ld, const double *xg, csi n, double *v);
/* dot4 for the last m (1..3) columns: lanes >= m repeat column m-1 and are
 * discarded; only v[0..m) is read and written. */
int vsdlss_simd_dot_tail(const double *c0, csi ld, csi m, const double *xg, csi n, double *v);

#endif
