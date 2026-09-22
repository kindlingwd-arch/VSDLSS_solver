#ifndef VSDLSS_DENSE_H
#define VSDLSS_DENSE_H

#include "vsdlss.h"

/* Dense column-major kernels shared by the in-memory (M3) and disk (M4)
 * supernodal paths.  None of them allocate.
 *
 * Arithmetic contract: every output element is formed as
 *     acc = +0; for p = 0..k-1: acc += A(i,p) * B(j,p);  C(i,j) -= acc
 * with separate multiply and add (the build never contracts to FMA), so a
 * result does not depend on register blocking, the CPU dispatch variant or
 * how callers split the output among threads. */

/* C(i,j) -= sum_p A(i,p) B(j,p) for 0<=i<m, 0<=j<n, restricted to the
 * entries with i + tri >= j (tri = 0 is the lower triangle/trapezoid, a
 * negative tri starts the band below the diagonal).  Pass VSDLSS_GEMM_FULL to
 * update the whole block. */
#define VSDLSS_GEMM_FULL (INT64_MAX / 4)
void vsdlss_gemm_nt_sub(csi m, csi n, csi k, const double *A, csi lda,
                        const double *B, csi ldb, double *C, csi ldc,
                        csi tri);

/* Same contract; splits the output rows over the calling thread's team
 * when the work is large and no parallel region is active. */
void vsdlss_gemm_nt_sub_par(csi m, csi n, csi k, const double *A, csi lda,
                            const double *B, csi ldb, double *C, csi ldc,
                            csi tri);

/* In-place Cholesky of a (rows x width) column-major panel whose leading
 * width x width block is the diagonal block (lower triangle used, strictly
 * upper triangle left untouched).  Rows below are overwritten by
 * L21 = A21 L11^{-T}.  Blocked left-looking; returns NOT_POSDEF/NONFINITE. */
vsdlss_status vsdlss_dense_potrf_panel(double *a, csi rows, csi width);

#endif
