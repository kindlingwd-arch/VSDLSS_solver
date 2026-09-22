#include "vsdlss_dense.h"
#include "vsdlss_parallel.h"

#include <string.h>

/* Register tile.  MR rows of A are loaded as two 4-wide vectors, each of the
 * NR columns of B is broadcast; 8 vector accumulators fit the 16 registers of
 * AVX2 and leave room on AVX-512.  The SSE2 baseline clone splits every
 * vector in two and spills a little, which is still faster than scalar. */
#define MR 8
#define NR 4
#define MC 64   /* multiple of MR */

#if defined(__GNUC__) || defined(__clang__)
#define VSDLSS_HAVE_VECTOR 1
typedef double vsd_v4 __attribute__((vector_size(32)));
#endif

/* Runtime CPU dispatch through GNU ifunc.  Opt out with -DVSDLSS_NO_CLONES
 * (for example for sanitizer builds or toolchains without ifunc). */
#if defined(VSDLSS_HAVE_VECTOR) && defined(__x86_64__) && defined(__linux__) && \
    !defined(VSDLSS_NO_CLONES) && \
    ((defined(__clang__) && __clang_major__ >= 14) || \
     (!defined(__clang__) && __GNUC__ >= 6))
#define VSDLSS_CLONES __attribute__((target_clones("avx512f", "avx2", "default")))
#else
#define VSDLSS_CLONES
#endif

/* Scalar element with the documented operation order. */
static inline double dot_nt(csi k, const double *a, csi lda,
                            const double *b, csi ldb)
{
    double acc = 0;
    for (csi p = 0; p < k; ++p) acc += a[p * lda] * b[p * ldb];
    return acc;
}

VSDLSS_CLONES
void vsdlss_gemm_nt_sub(csi m, csi n, csi k, const double *A, csi lda,
                        const double *B, csi ldb, double *C, csi ldc,
                        csi tri)
{
    if (m <= 0 || n <= 0 || k <= 0) return;
    /* Row blocks of MC keep an MC x k sliver of A in L2 while all column
     * tiles pass over it; without this a tall A is streamed from memory once
     * per NR columns. */
    for (csi ib0 = 0; ib0 < m; ib0 += MC) {
    csi ib1 = m - ib0 < MC ? m : ib0 + MC;
    for (csi j0 = 0; j0 < n; j0 += NR) {
        csi nr = n - j0 < NR ? n - j0 : NR;
        csi i0 = ib0;
        if (j0 - tri > ib0) i0 = ib0 + ((j0 - tri - ib0) / MR) * MR;
        for (; i0 < ib1; i0 += MR) {
            csi mr = ib1 - i0 < MR ? ib1 - i0 : MR;
            int masked = i0 + tri < j0 + nr - 1;
#ifdef VSDLSS_HAVE_VECTOR
            if (mr == MR && nr == NR) {
                const double *a = A + i0, *b = B + j0;
                vsd_v4 c00 = {0}, c10 = {0}, c01 = {0}, c11 = {0};
                vsd_v4 c02 = {0}, c12 = {0}, c03 = {0}, c13 = {0};
                for (csi p = 0; p < k; ++p) {
                    vsd_v4 a0, a1;
                    memcpy(&a0, a, sizeof(a0));
                    memcpy(&a1, a + 4, sizeof(a1));
                    double b0 = b[0], b1 = b[1], b2 = b[2], b3 = b[3];
                    c00 += a0 * b0; c10 += a1 * b0;
                    c01 += a0 * b1; c11 += a1 * b1;
                    c02 += a0 * b2; c12 += a1 * b2;
                    c03 += a0 * b3; c13 += a1 * b3;
                    a += lda; b += ldb;
                }
                double t[NR][MR];
                memcpy(&t[0][0], &c00, 32); memcpy(&t[0][4], &c10, 32);
                memcpy(&t[1][0], &c01, 32); memcpy(&t[1][4], &c11, 32);
                memcpy(&t[2][0], &c02, 32); memcpy(&t[2][4], &c12, 32);
                memcpy(&t[3][0], &c03, 32); memcpy(&t[3][4], &c13, 32);
                for (csi j = 0; j < NR; ++j) {
                    double *c = C + (j0 + j) * ldc + i0;
                    csi ib = masked && j0 + j > i0 + tri ? j0 + j - i0 - tri : 0;
                    for (csi i = ib; i < MR; ++i) c[i] -= t[j][i];
                }
                continue;
            }
#endif
            for (csi j = 0; j < nr; ++j) {
                csi col = j0 + j;
                csi ib = masked && col > i0 + tri ? col - i0 - tri : 0;
                for (csi i = ib; i < mr; ++i)
                    C[col * ldc + i0 + i] -= dot_nt(k, A + i0 + i, lda, B + col, ldb);
            }
        }
    }
    }
}

void vsdlss_gemm_nt_sub_par(csi m, csi n, csi k, const double *A, csi lda,
                            const double *B, csi ldb, double *C, csi ldc,
                            csi tri)
{
    double work = (double)m * (double)n * (double)k;
    int nt = vsdlss_parallel_width(work / 8.0);
    const csi chunk = 8 * MR;
    csi chunks = m / chunk + (m % chunk != 0);
    if (nt > chunks) nt = (int)chunks;
    if (nt <= 1) {
        vsdlss_gemm_nt_sub(m, n, k, A, lda, B, ldb, C, ldc, tri);
        return;
    }
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(dynamic, 1))
        for (csi c = 0; c < chunks; ++c) {
            csi r0 = c * chunk, rn = m - r0 < chunk ? m - r0 : chunk;
            vsdlss_gemm_nt_sub(rn, n, k, A + r0, lda, B, ldb, C + r0, ldc,
                               tri + r0);
        }
    }
}

#define NB 48
#define TRSM_TILE 32

/* X(first:first+cnt, 0:kb) := X L^{-T} for the kb x kb factored block at the
 * top of `blk`.  Full tiles use a compile-time trip count so the row loop is
 * vectorised; the per-element order is identical for partial tiles. */
VSDLSS_CLONES
static int trsm_tile(double *blk, csi rows, csi kb, csi first, csi cnt)
{
    double v[TRSM_TILE];
    int bad = 0;
    for (csi j = 0; j < kb; ++j) {
        double *cj = blk + j * rows + first;
        if (cnt == TRSM_TILE) {
            for (csi i = 0; i < TRSM_TILE; ++i) v[i] = cj[i];
            for (csi p = 0; p < j; ++p) {
                double coeff = blk[p * rows + j];
                const double *cp = blk + p * rows + first;
                for (csi i = 0; i < TRSM_TILE; ++i) v[i] -= cp[i] * coeff;
            }
            double d = blk[j * rows + j];
            for (csi i = 0; i < TRSM_TILE; ++i) { v[i] /= d; cj[i] = v[i]; }
            for (csi i = 0; i < TRSM_TILE; ++i) if (!isfinite(v[i])) bad = 1;
        } else {
            for (csi i = 0; i < cnt; ++i) v[i] = cj[i];
            for (csi p = 0; p < j; ++p) {
                double coeff = blk[p * rows + j];
                const double *cp = blk + p * rows + first;
                for (csi i = 0; i < cnt; ++i) v[i] -= cp[i] * coeff;
            }
            double d = blk[j * rows + j];
            for (csi i = 0; i < cnt; ++i) {
                v[i] /= d;
                if (!isfinite(v[i])) bad = 1;
                cj[i] = v[i];
            }
        }
    }
    return bad;
}

vsdlss_status vsdlss_dense_potrf_panel(double *a, csi rows, csi width)
{
    if (!a || width < 1 || rows < width) return VSDLSS_ERR_INVALID;
    for (csi jb = 0; jb < width; jb += NB) {
        csi kb = width - jb < NB ? width - jb : NB;
        double *blk = a + jb * rows + jb;         /* A(jb:, jb:jb+kb) */
        /* Left-looking block update from all previous columns. */
        if (jb)
            vsdlss_gemm_nt_sub_par(rows - jb, kb, jb, a + jb, rows, a + jb, rows,
                                   blk, rows, 0);
        /* Unblocked factor of the kb x kb diagonal block. */
        for (csi j = 0; j < kb; ++j) {
            double *cj = blk + j * rows;
            double d = cj[j];
            for (csi p = 0; p < j; ++p) d -= blk[p * rows + j] * blk[p * rows + j];
            if (!isfinite(d)) return VSDLSS_ERR_NONFINITE;
            if (d <= 0) return VSDLSS_ERR_NOT_POSDEF;
            d = sqrt(d); cj[j] = d;
            for (csi p = 0; p < j; ++p) {
                double coeff = blk[p * rows + j];
                const double *cp = blk + p * rows;
                for (csi i = j + 1; i < kb; ++i) cj[i] -= cp[i] * coeff;
            }
            for (csi i = j + 1; i < kb; ++i) {
                cj[i] /= d;
                if (!isfinite(cj[i])) return VSDLSS_ERR_NONFINITE;
            }
        }
        /* TRSM of the rows below the diagonal block, one row tile at a time
         * so every tile finishes all kb columns from registers/L1. */
        csi below = rows - jb - kb;
        if (below <= 0) continue;
        csi tiles = below / TRSM_TILE + (below % TRSM_TILE != 0);
        int nt = vsdlss_parallel_width((double)below * kb * kb), bad = 0;
        if (nt > tiles) nt = (int)tiles;
        if (nt < 1) nt = 1;
        (void)nt;
        VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
        {
            VSDLSS_OMP(omp master)
            vsdlss_parallel_observe();
            VSDLSS_OMP(omp for schedule(static))
            for (csi t = 0; t < tiles; ++t) {
                csi first = kb + t * TRSM_TILE, cnt = rows - jb - first;
                if (cnt > TRSM_TILE) cnt = TRSM_TILE;
                bad |= trsm_tile(blk, rows, kb, first, cnt);
            }
        }
        if (bad) return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}
