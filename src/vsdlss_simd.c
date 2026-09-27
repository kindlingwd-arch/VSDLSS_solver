/* AVX2 kernels for the supernodal triangular solves.
 *
 * Every kernel performs, for each entry, exactly the operations of the scalar
 * loop it replaces, in the same order: one product rounded to double, then
 * one subtraction rounded to double (no FMA is enabled for these functions,
 * so the compiler cannot contract them).  Vector lanes only hold different
 * entries (independent chains), never partial sums of one entry, so results
 * are bitwise identical to the scalar code for any input.
 *
 * Used when the CPU reports AVX2 at run time; VSDLSS_SIMD=0 keeps the scalar
 * loops (for comparison).  Built with the target attribute, so the default
 * -O2 build without -march flags gets them too. */
#include "vsdlss_simd.h"

#include <math.h>
#include <stdlib.h>

#if defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#define SIMD_FN __attribute__((target("avx2")))

int vsdlss_simd_enabled(void)
{
    static int on = -1;
    if (on < 0) {
        const char *e = getenv("VSDLSS_SIMD");
        __builtin_cpu_init();
        on = !(e && e[0] == '0') && __builtin_cpu_supports("avx2");
    }
    return on;
}

/* v <- v - c * s, lane-wise (product rounded, then difference). */
#define SUBMUL(v, c, s) _mm256_sub_pd((v), _mm256_mul_pd((c), (s)))

SIMD_FN void vsdlss_simd_block_update(const double *as, csi rs, csi ws, const double *xs,
                                      const csi *R, csi r0, csi r1, double *x)
{
    csi r = r0;
    for (; r + 8 <= r1; r += 8) {
        const int c0 = R[r + 3] - R[r] == 3, c1 = R[r + 7] - R[r + 4] == 3;
        __m256d v0 = c0 ? _mm256_loadu_pd(x + R[r])
                        : _mm256_i64gather_pd(x, _mm256_loadu_si256((const __m256i *)(R + r)), 8);
        __m256d v1 = c1 ? _mm256_loadu_pd(x + R[r + 4])
                        : _mm256_i64gather_pd(x, _mm256_loadu_si256((const __m256i *)(R + r + 4)), 8);
        for (csi j = 0; j < ws; ++j) {
            const double *col = as + j * rs + r;
            const __m256d s = _mm256_set1_pd(xs[j]);
            v0 = SUBMUL(v0, _mm256_loadu_pd(col), s);
            v1 = SUBMUL(v1, _mm256_loadu_pd(col + 4), s);
        }
        if (c0) _mm256_storeu_pd(x + R[r], v0);
        else { double t[4]; _mm256_storeu_pd(t, v0); x[R[r]] = t[0]; x[R[r + 1]] = t[1]; x[R[r + 2]] = t[2]; x[R[r + 3]] = t[3]; }
        if (c1) _mm256_storeu_pd(x + R[r + 4], v1);
        else { double t[4]; _mm256_storeu_pd(t, v1); x[R[r + 4]] = t[0]; x[R[r + 5]] = t[1]; x[R[r + 6]] = t[2]; x[R[r + 7]] = t[3]; }
    }
    for (; r + 4 <= r1; r += 4) {
        const int c0 = R[r + 3] - R[r] == 3;
        __m256d v0 = c0 ? _mm256_loadu_pd(x + R[r])
                        : _mm256_i64gather_pd(x, _mm256_loadu_si256((const __m256i *)(R + r)), 8);
        for (csi j = 0; j < ws; ++j)
            v0 = SUBMUL(v0, _mm256_loadu_pd(as + j * rs + r), _mm256_set1_pd(xs[j]));
        if (c0) _mm256_storeu_pd(x + R[r], v0);
        else { double t[4]; _mm256_storeu_pd(t, v0); x[R[r]] = t[0]; x[R[r + 1]] = t[1]; x[R[r + 2]] = t[2]; x[R[r + 3]] = t[3]; }
    }
    for (; r < r1; ++r) {
        double v = x[R[r]];
        for (csi j = 0; j < ws; ++j) { double u = as[j * rs + r] * xs[j]; v -= u; }
        x[R[r]] = v;
    }
}

SIMD_FN void vsdlss_simd_block_update_contig(const double *a, csi ld, csi j0, csi j1, const double *xs,
                                             csi r0, csi r1, double *xt)
{
    csi r = r0;
    for (; r + 8 <= r1; r += 8) {
        __m256d v0 = _mm256_loadu_pd(xt + r), v1 = _mm256_loadu_pd(xt + r + 4);
        for (csi j = j0; j < j1; ++j) {
            const double *col = a + j * ld + r;
            const __m256d s = _mm256_set1_pd(xs[j]);
            v0 = SUBMUL(v0, _mm256_loadu_pd(col), s);
            v1 = SUBMUL(v1, _mm256_loadu_pd(col + 4), s);
        }
        _mm256_storeu_pd(xt + r, v0); _mm256_storeu_pd(xt + r + 4, v1);
    }
    for (; r + 4 <= r1; r += 4) {
        __m256d v0 = _mm256_loadu_pd(xt + r);
        for (csi j = j0; j < j1; ++j) v0 = SUBMUL(v0, _mm256_loadu_pd(a + j * ld + r), _mm256_set1_pd(xs[j]));
        _mm256_storeu_pd(xt + r, v0);
    }
    for (; r < r1; ++r) {
        double v = xt[r];
        for (csi j = j0; j < j1; ++j) { double u = a[j * ld + r] * xs[j]; v -= u; }
        xt[r] = v;
    }
}

SIMD_FN void vsdlss_simd_axpy_neg(double *y, const double *c, double s, csi n)
{
    const __m256d vs = _mm256_set1_pd(s);
    csi i = 0;
    for (; i + 8 <= n; i += 8) {
        _mm256_storeu_pd(y + i, SUBMUL(_mm256_loadu_pd(y + i), _mm256_loadu_pd(c + i), vs));
        _mm256_storeu_pd(y + i + 4, SUBMUL(_mm256_loadu_pd(y + i + 4), _mm256_loadu_pd(c + i + 4), vs));
    }
    for (; i + 4 <= n; i += 4) _mm256_storeu_pd(y + i, SUBMUL(_mm256_loadu_pd(y + i), _mm256_loadu_pd(c + i), vs));
    for (; i < n; ++i) { double u = c[i] * s; y[i] -= u; }
}

/* 4x4 transpose: q[i] = (p0[i], p1[i], p2[i], p3[i]). */
#define TRANSPOSE4(p0, p1, p2, p3, q0, q1, q2, q3) do { \
    __m256d t0_ = _mm256_unpacklo_pd(p0, p1), t1_ = _mm256_unpackhi_pd(p0, p1); \
    __m256d t2_ = _mm256_unpacklo_pd(p2, p3), t3_ = _mm256_unpackhi_pd(p2, p3); \
    q0 = _mm256_permute2f128_pd(t0_, t2_, 0x20); q1 = _mm256_permute2f128_pd(t1_, t3_, 0x20); \
    q2 = _mm256_permute2f128_pd(t0_, t2_, 0x31); q3 = _mm256_permute2f128_pd(t1_, t3_, 0x31); } while (0)

SIMD_FN int vsdlss_simd_dot4(const double *c0, csi ld, const double *xg, csi n, double *v)
{
    const double *c1 = c0 + ld, *c2 = c1 + ld, *c3 = c2 + ld;
    __m256d acc = _mm256_loadu_pd(v);
    csi r = 0;
    for (; r + 4 <= n; r += 4) {
        __m256d q0, q1, q2, q3;
        TRANSPOSE4(_mm256_loadu_pd(c0 + r), _mm256_loadu_pd(c1 + r), _mm256_loadu_pd(c2 + r),
                   _mm256_loadu_pd(c3 + r), q0, q1, q2, q3);
        acc = SUBMUL(acc, q0, _mm256_set1_pd(xg[r]));
        acc = SUBMUL(acc, q1, _mm256_set1_pd(xg[r + 1]));
        acc = SUBMUL(acc, q2, _mm256_set1_pd(xg[r + 2]));
        acc = SUBMUL(acc, q3, _mm256_set1_pd(xg[r + 3]));
    }
    for (; r < n; ++r)
        acc = SUBMUL(acc, _mm256_set_pd(c3[r], c2[r], c1[r], c0[r]), _mm256_set1_pd(xg[r]));
    _mm256_storeu_pd(v, acc);
    /* finite check: x - x is 0 for finite x, NaN otherwise */
    __m256d z = _mm256_sub_pd(acc, acc);
    return _mm256_movemask_pd(_mm256_cmp_pd(z, z, _CMP_UNORD_Q)) != 0;
}


/* Rows r..r+3 of four columns as four vectors (c0[i], c1[i], c2[i], c3[i]),
 * built from 128-bit loads: the high halves are inserted straight from
 * memory and only in-lane unpacks remain, instead of TRANSPOSE4's two
 * lane-crossing permutes per pair (single-port shuffles on Intel cores).
 * Same values, so the arithmetic that follows is unchanged. */
#define ROWS4(c0, c1, c2, c3, r, q0, q1, q2, q3) do { \
    __m256d a01_ = _mm256_insertf128_pd(_mm256_castpd128_pd256(_mm_loadu_pd((c0) + (r))), _mm_loadu_pd((c2) + (r)), 1); \
    __m256d b01_ = _mm256_insertf128_pd(_mm256_castpd128_pd256(_mm_loadu_pd((c1) + (r))), _mm_loadu_pd((c3) + (r)), 1); \
    __m256d a23_ = _mm256_insertf128_pd(_mm256_castpd128_pd256(_mm_loadu_pd((c0) + (r) + 2)), _mm_loadu_pd((c2) + (r) + 2), 1); \
    __m256d b23_ = _mm256_insertf128_pd(_mm256_castpd128_pd256(_mm_loadu_pd((c1) + (r) + 2)), _mm_loadu_pd((c3) + (r) + 2), 1); \
    q0 = _mm256_unpacklo_pd(a01_, b01_); q1 = _mm256_unpackhi_pd(a01_, b01_); \
    q2 = _mm256_unpacklo_pd(a23_, b23_); q3 = _mm256_unpackhi_pd(a23_, b23_); } while (0)

/* 1 when a lane of v is not finite. */
#define NONFINITE(v) (_mm256_movemask_pd(_mm256_cmp_pd(_mm256_sub_pd((v), (v)), \
                        _mm256_sub_pd((v), (v)), _CMP_UNORD_Q)) != 0)
/* Load / store four destinations x[R[r..r+3]]: one vector access when the
 * indices are consecutive, a gather / four scalar stores otherwise. */
#define XLOAD(c, r) ((c) ? _mm256_loadu_pd(x + R[r]) \
                         : _mm256_i64gather_pd(x, _mm256_loadu_si256((const __m256i *)(R + (r))), 8))
#define XSTORE(c, r, v) do { if (c) _mm256_storeu_pd(x + R[r], (v)); else { double t_[4]; \
    _mm256_storeu_pd(t_, (v)); x[R[r]] = t_[0]; x[R[(r) + 1]] = t_[1]; \
    x[R[(r) + 2]] = t_[2]; x[R[(r) + 3]] = t_[3]; } } while (0)

SIMD_FN int vsdlss_simd_ext_update(const double *a, csi ld, csi ws, const double *xs,
                                   const csi *R, csi n, double *x)
{
    __m256d bad = _mm256_setzero_pd();
    csi r = 0;
    _Static_assert(sizeof(csi) == 8, "gather expects 64-bit indices");
    for (; r + 16 <= n; r += 16) {
        const int c0 = R[r + 3] - R[r] == 3, c1 = R[r + 7] - R[r + 4] == 3;
        const int c2 = R[r + 11] - R[r + 8] == 3, c3 = R[r + 15] - R[r + 12] == 3;
        __m256d v0 = XLOAD(c0, r), v1 = XLOAD(c1, r + 4), v2 = XLOAD(c2, r + 8), v3 = XLOAD(c3, r + 12);
        const double *col = a + r;
        for (csi j = 0; j < ws; ++j, col += ld) {
            const __m256d s = _mm256_set1_pd(xs[j]);
            v0 = SUBMUL(v0, _mm256_loadu_pd(col), s);
            v1 = SUBMUL(v1, _mm256_loadu_pd(col + 4), s);
            v2 = SUBMUL(v2, _mm256_loadu_pd(col + 8), s);
            v3 = SUBMUL(v3, _mm256_loadu_pd(col + 12), s);
        }
        XSTORE(c0, r, v0); XSTORE(c1, r + 4, v1); XSTORE(c2, r + 8, v2); XSTORE(c3, r + 12, v3);
        bad = _mm256_or_pd(bad, _mm256_or_pd(_mm256_or_pd(_mm256_sub_pd(v0, v0), _mm256_sub_pd(v1, v1)),
                                             _mm256_or_pd(_mm256_sub_pd(v2, v2), _mm256_sub_pd(v3, v3))));
    }
    for (; r + 8 <= n; r += 8) {
        const int c0 = R[r + 3] - R[r] == 3, c1 = R[r + 7] - R[r + 4] == 3;
        __m256d v0 = XLOAD(c0, r), v1 = XLOAD(c1, r + 4);
        const double *col = a + r;
        for (csi j = 0; j < ws; ++j, col += ld) {
            const __m256d s = _mm256_set1_pd(xs[j]);
            v0 = SUBMUL(v0, _mm256_loadu_pd(col), s);
            v1 = SUBMUL(v1, _mm256_loadu_pd(col + 4), s);
        }
        XSTORE(c0, r, v0); XSTORE(c1, r + 4, v1);
        bad = _mm256_or_pd(bad, _mm256_or_pd(_mm256_sub_pd(v0, v0), _mm256_sub_pd(v1, v1)));
    }
    for (; r + 4 <= n; r += 4) {
        const int c0 = R[r + 3] - R[r] == 3;
        __m256d v0 = XLOAD(c0, r);
        const double *col = a + r;
        for (csi j = 0; j < ws; ++j, col += ld) v0 = SUBMUL(v0, _mm256_loadu_pd(col), _mm256_set1_pd(xs[j]));
        XSTORE(c0, r, v0);
        bad = _mm256_or_pd(bad, _mm256_sub_pd(v0, v0));
    }
    int nf = NONFINITE(bad);   /* x - x is 0 for finite x, NaN otherwise; OR keeps NaN bits */
    /* Remaining 1..3 rows: independent scalar chains interleaved. */
    if (r < n) {
        const csi m = n - r;
        double u0 = x[R[r]], u1 = m > 1 ? x[R[r + 1]] : 0, u2 = m > 2 ? x[R[r + 2]] : 0;
        const double *col = a + r;
        for (csi j = 0; j < ws; ++j, col += ld) {
            const double s = xs[j];
            { double p = col[0] * s; u0 -= p; }
            if (m > 1) { double p = col[1] * s; u1 -= p; }
            if (m > 2) { double p = col[2] * s; u2 -= p; }
        }
        x[R[r]] = u0; nf |= !isfinite(u0);
        if (m > 1) { x[R[r + 1]] = u1; nf |= !isfinite(u1); }
        if (m > 2) { x[R[r + 2]] = u2; nf |= !isfinite(u2); }
    }
    return nf;
}

SIMD_FN int vsdlss_simd_dot8(const double *c0, csi ld, const double *xg, csi n, double *v)
{
    const double *c1 = c0 + ld, *c2 = c1 + ld, *c3 = c2 + ld;
    const double *c4 = c3 + ld, *c5 = c4 + ld, *c6 = c5 + ld, *c7 = c6 + ld;
    __m256d a0 = _mm256_loadu_pd(v), a1 = _mm256_loadu_pd(v + 4);
    csi r = 0;
    for (; r + 4 <= n; r += 4) {
        __m256d p0, p1, p2, p3, q0, q1, q2, q3;
        ROWS4(c0, c1, c2, c3, r, p0, p1, p2, p3);
        ROWS4(c4, c5, c6, c7, r, q0, q1, q2, q3);
        __m256d s = _mm256_set1_pd(xg[r]);
        a0 = SUBMUL(a0, p0, s); a1 = SUBMUL(a1, q0, s);
        s = _mm256_set1_pd(xg[r + 1]);
        a0 = SUBMUL(a0, p1, s); a1 = SUBMUL(a1, q1, s);
        s = _mm256_set1_pd(xg[r + 2]);
        a0 = SUBMUL(a0, p2, s); a1 = SUBMUL(a1, q2, s);
        s = _mm256_set1_pd(xg[r + 3]);
        a0 = SUBMUL(a0, p3, s); a1 = SUBMUL(a1, q3, s);
    }
    for (; r < n; ++r) {
        const __m256d s = _mm256_set1_pd(xg[r]);
        a0 = SUBMUL(a0, _mm256_set_pd(c3[r], c2[r], c1[r], c0[r]), s);
        a1 = SUBMUL(a1, _mm256_set_pd(c7[r], c6[r], c5[r], c4[r]), s);
    }
    _mm256_storeu_pd(v, a0); _mm256_storeu_pd(v + 4, a1);
    return NONFINITE(_mm256_or_pd(_mm256_sub_pd(a0, a0), _mm256_sub_pd(a1, a1)));
}

SIMD_FN int vsdlss_simd_dot4b(const double *c0, csi ld, const double *xg, csi n, double *v)
{
    const double *c1 = c0 + ld, *c2 = c1 + ld, *c3 = c2 + ld;
    __m256d acc = _mm256_loadu_pd(v);
    csi r = 0;
    for (; r + 4 <= n; r += 4) {
        __m256d q0, q1, q2, q3;
        ROWS4(c0, c1, c2, c3, r, q0, q1, q2, q3);
        acc = SUBMUL(acc, q0, _mm256_set1_pd(xg[r]));
        acc = SUBMUL(acc, q1, _mm256_set1_pd(xg[r + 1]));
        acc = SUBMUL(acc, q2, _mm256_set1_pd(xg[r + 2]));
        acc = SUBMUL(acc, q3, _mm256_set1_pd(xg[r + 3]));
    }
    for (; r < n; ++r)
        acc = SUBMUL(acc, _mm256_set_pd(c3[r], c2[r], c1[r], c0[r]), _mm256_set1_pd(xg[r]));
    _mm256_storeu_pd(v, acc);
    return NONFINITE(_mm256_sub_pd(acc, acc));
}

SIMD_FN int vsdlss_simd_dot_tail(const double *c0, csi ld, csi m, const double *xg, csi n, double *v)
{
    /* Lanes >= m duplicate column m-1: extra independent work, never mixed
     * into a kept lane. */
    const double *c1 = m > 1 ? c0 + ld : c0, *c2 = m > 2 ? c0 + 2 * ld : c1, *c3 = c2;
    double in[4] = { v[0], m > 1 ? v[1] : v[0], m > 2 ? v[2] : 0, 0 };
    if (m <= 2) in[2] = in[1];
    in[3] = in[2];
    __m256d acc = _mm256_loadu_pd(in);
    csi r = 0;
    for (; r + 4 <= n; r += 4) {
        __m256d q0, q1, q2, q3;
        ROWS4(c0, c1, c2, c3, r, q0, q1, q2, q3);
        acc = SUBMUL(acc, q0, _mm256_set1_pd(xg[r]));
        acc = SUBMUL(acc, q1, _mm256_set1_pd(xg[r + 1]));
        acc = SUBMUL(acc, q2, _mm256_set1_pd(xg[r + 2]));
        acc = SUBMUL(acc, q3, _mm256_set1_pd(xg[r + 3]));
    }
    for (; r < n; ++r)
        acc = SUBMUL(acc, _mm256_set_pd(c3[r], c2[r], c1[r], c0[r]), _mm256_set1_pd(xg[r]));
    double out[4]; _mm256_storeu_pd(out, acc);
    int bad = 0;
    for (csi k = 0; k < m; ++k) { v[k] = out[k]; bad |= !isfinite(out[k]); }
    return bad;
}

#else   /* no x86 GCC-compatible compiler: scalar paths only */
int vsdlss_simd_enabled(void) { return 0; }
void vsdlss_simd_block_update(const double *as, csi rs, csi ws, const double *xs,
                              const csi *R, csi r0, csi r1, double *x)
{ (void)as; (void)rs; (void)ws; (void)xs; (void)R; (void)r0; (void)r1; (void)x; abort(); }
void vsdlss_simd_block_update_contig(const double *a, csi ld, csi j0, csi j1, const double *xs,
                                     csi r0, csi r1, double *xt)
{ (void)a; (void)ld; (void)j0; (void)j1; (void)xs; (void)r0; (void)r1; (void)xt; abort(); }
void vsdlss_simd_axpy_neg(double *y, const double *c, double s, csi n)
{ (void)y; (void)c; (void)s; (void)n; abort(); }
int vsdlss_simd_dot4(const double *c0, csi ld, const double *xg, csi n, double *v)
{ (void)c0; (void)ld; (void)xg; (void)n; (void)v; abort(); }
int vsdlss_simd_ext_update(const double *a, csi ld, csi ws, const double *xs,
                           const csi *R, csi n, double *x)
{ (void)a; (void)ld; (void)ws; (void)xs; (void)R; (void)n; (void)x; abort(); }
int vsdlss_simd_dot8(const double *c0, csi ld, const double *xg, csi n, double *v)
{ (void)c0; (void)ld; (void)xg; (void)n; (void)v; abort(); }
int vsdlss_simd_dot4b(const double *c0, csi ld, const double *xg, csi n, double *v)
{ (void)c0; (void)ld; (void)xg; (void)n; (void)v; abort(); }
int vsdlss_simd_dot_tail(const double *c0, csi ld, csi m, const double *xg, csi n, double *v)
{ (void)c0; (void)ld; (void)m; (void)xg; (void)n; (void)v; abort(); }
#endif
