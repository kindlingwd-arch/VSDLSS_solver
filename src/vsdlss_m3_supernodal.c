/* Supernodal Cholesky of the M3 reduced core in one pass: the factor of
 * vsdlss_sn_analyze + vsdlss_sn_factorize (same supernodes, same panel
 * layout, same arithmetic in the same order, hence the same bits), without
 * their per-update tables:
 *   - supernodes come from the elimination tree and the column counts: with
 *     parent(j) = j + 1, column j's structure below the diagonal is contained
 *     in {j + 1} plus column j + 1's, so the two agree exactly when
 *     count(j) = count(j + 1) + 1 -- the comparison sn_analyze makes on the
 *     stored structure of L;
 *   - only each supernode's external rows are stored (the factor needs no
 *     per-entry structure of L to solve);
 *   - an update finds its target column once per target supernode and its
 *     rows by a merge, instead of a table entry per updated value; the dot
 *     products of one column of updates run together, each still summed over
 *     the supernode's columns in order, and every value receives its updates
 *     in supernode order as before. */
#include "vsdlss_m3_internal.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fits(csi n, size_t width) { return n >= 0 && (uint64_t)n <= SIZE_MAX / width; }

/* Position of row in supernode t's panel rows, -1 when it is not one of
 * them.  External rows are searched from position `from` on, first by
 * doubling steps: rows found in ascending order cost O(1) each when they
 * are (nearly) consecutive in t. */
static csi local_row(const vsdlss_sn_factor *f, csi t, csi row, csi from)
{
    const csi begin = f->column_start[t], end = f->column_start[t + 1];
    const csi *r = f->row_index + f->row_ptr[t], ext = f->row_ptr[t + 1] - f->row_ptr[t];
    csi lo = from, hi = from, step = 1;
    if (row < end) return row - begin;
    while (hi < ext && r[hi] < row) { lo = hi + 1; hi += step; step *= 2; }
    if (hi > ext) hi = ext;
    while (lo < hi) { csi mid = lo + (hi - lo) / 2; if (r[mid] < row) lo = mid + 1; else hi = mid; }
    return lo < ext && r[lo] == row ? (end - begin) + lo : -1;
}

#ifdef _OPENMP
#define M3_SIMD _Pragma("omp simd")
#define M3_SIMD_CHECK _Pragma("omp simd reduction(+:check)")
#else
#define M3_SIMD
#define M3_SIMD_CHECK
#endif

/* Subtract the updates of external column q of a supernode panel a (rows x
 * w) from the target column: row r >= q goes to target[lpos[r]], and
 * lpos[r..run[r]) are consecutive.  Each value is vsdlss_panel_dot's sum
 * (from 0, over the w columns in order), computed for all rows together.
 * Returns nonzero when a result is not finite: x - x is +0 for finite x and
 * NaN otherwise, so the sum of those is 0 exactly when all are finite. */
static int update_column(const double *a, csi rows, csi w, csi ext, csi q, double *target,
                         const csi *lpos, const csi *run, double *dot)
{
    double check = 0;
    if (w == 1) {
        const double s = a[1 + q], *col = a + 1;
        for (csi r = q; r < ext; r = run[r]) {
            double *x = target + lpos[r];
            const double *y = col + r;
            const csi len = run[r] - r;
            M3_SIMD_CHECK
            for (csi i = 0; i < len; i++) { x[i] -= 0.0 + y[i] * s; check += x[i] - x[i]; }
        }
        return !(check == 0);
    }
    M3_SIMD
    for (csi r = q; r < ext; r++) dot[r] = 0;
    for (csi c = 0; c < w; c++) {
        const double *col = a + c * rows + w, s = col[q];
        M3_SIMD
        for (csi r = q; r < ext; r++) dot[r] += col[r] * s;
    }
    for (csi r = q; r < ext; r = run[r]) {
        double *x = target + lpos[r];
        const double *y = dot + r;
        const csi len = run[r] - r;
        M3_SIMD_CHECK
        for (csi i = 0; i < len; i++) { x[i] -= y[i]; check += x[i] - x[i]; }
    }
    return !(check == 0);
}

/* vsdlss_panel_factor without its parallel region (a team per panel costs
 * more than the panel): the same operations in the same order. */
static vsdlss_status panel_factor(double *a, csi rows, csi width)
{
    for (csi j = 0; j < width; j++) {
        double d = a[j * rows + j], check = 0;
        for (csi k = 0; k < j; k++) d -= a[k * rows + j] * a[k * rows + j];
        if (!isfinite(d)) return VSDLSS_ERR_NONFINITE;
        if (d <= 0) return VSDLSS_ERR_NOT_POSDEF;
        a[j * rows + j] = d = sqrt(d);
        for (csi i = j + 1; i < rows; i++) {
            double v = a[j * rows + i];
            for (csi k = 0; k < j; k++) v -= a[k * rows + i] * a[k * rows + j];
            v /= d;
            check += v - v;
            a[j * rows + i] = v;
        }
        if (!(check == 0)) return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

vsdlss_status vsdlss_m3_sn_factor(const vsdlss *A, vsdlss_sn_factor **out)
{
    vsdlss_sn_factor *f = NULL;
    csi *parent = NULL, *cc = NULL, *stack = NULL, *mark = NULL, *snode = NULL, *cursor = NULL;
    csi *lpos = NULL, *run = NULL; double *dot = NULL;
    csi n, j, k, p, sn, top, ext_total = 0, panel_total = 0, max_ext = 0;
    vsdlss_status st;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!A || A->n < 1) return VSDLSS_ERR_INVALID;
    n = A->n;
    if (n == INT64_MAX || !fits(n + 1, sizeof(csi))) return VSDLSS_ERR_OOM;
    st = vsdlss_validate_upper_csc(A);
    if (st != VSDLSS_OK) return st;
    st = VSDLSS_ERR_OOM;
    f = (vsdlss_sn_factor *)calloc(1, sizeof(*f));
    cc = (csi *)malloc((size_t)n * sizeof(csi));
    stack = (csi *)malloc((size_t)n * sizeof(csi));
    mark = (csi *)calloc((size_t)n, sizeof(csi));
    snode = (csi *)malloc((size_t)n * sizeof(csi));
    if (!f || !cc || !stack || !mark || !snode) goto fail;
    parent = vsdlss_etree(A, 0);
    if (!parent) goto fail;
    f->n = n;

    /* column counts of L (diagonal included) */
    for (j = 0; j < n; j++) cc[j] = 1;
    for (k = 0; k < n; k++) {
        top = vsdlss_ereach(A, k, parent, stack, mark);
        if (top < 0) { st = VSDLSS_ERR_INVALID; goto fail; }
        for (p = top; p < n; p++) cc[stack[p]]++;
    }
    f->l_nnz = 0;
    for (j = 0; j < n; j++) {
        if (f->l_nnz > INT64_MAX - cc[j]) goto fail;
        f->l_nnz += cc[j];
    }

    /* fundamental supernodes, as sn_analyze */
    f->column_start = (csi *)malloc((size_t)(n + 1) * sizeof(csi));
    if (!f->column_start) goto fail;
    f->count = 0; f->column_start[0] = 0;
    for (j = 0; j + 1 < n; j++)
        if (!(parent[j] == j + 1 && cc[j] == cc[j + 1] + 1)) f->column_start[++f->count] = j + 1;
    f->column_start[++f->count] = n;
    f->row_ptr = (csi *)malloc((size_t)(f->count + 1) * sizeof(csi));
    f->panel_offset = (csi *)malloc((size_t)(f->count + 1) * sizeof(csi));
    cursor = (csi *)malloc((size_t)(f->count + 1) * sizeof(csi));
    if (!f->row_ptr || !f->panel_offset || !cursor) goto fail;
    f->row_ptr[0] = f->panel_offset[0] = 0;
    for (sn = 0; sn < f->count; sn++) {
        const csi begin = f->column_start[sn], end = f->column_start[sn + 1], width = end - begin;
        const csi ext = cc[end - 1] - 1, rows = width + ext;
        for (j = begin; j < end; j++) snode[j] = sn;
        if (ext_total > INT64_MAX - ext || (width && rows > INT64_MAX / width) ||
            panel_total > INT64_MAX - rows * width) goto fail;
        ext_total += ext; panel_total += rows * width;
        if (ext > max_ext) max_ext = ext;
        f->row_ptr[sn + 1] = ext_total; f->panel_offset[sn + 1] = panel_total;
    }
    if (!fits(ext_total, sizeof(csi)) || !fits(panel_total, sizeof(double))) goto fail;
    free(cc); cc = NULL;

    /* external rows: row k is in supernode sn's structure when L(k, last
       column of sn) is nonzero; k ascending keeps every list sorted */
    f->row_index = (csi *)malloc((size_t)(ext_total ? ext_total : 1) * sizeof(csi));
    if (!f->row_index) goto fail;
    for (sn = 0; sn < f->count; sn++) cursor[sn] = f->row_ptr[sn];
    for (k = 0; k < n; k++) {
        top = vsdlss_ereach(A, k, parent, stack, mark);
        for (p = top; p < n; p++) {
            const csi c = stack[p], s = snode[c];
            if (c == f->column_start[s + 1] - 1) f->row_index[cursor[s]++] = k;
        }
    }
    for (sn = 0; sn < f->count; sn++) if (cursor[sn] != f->row_ptr[sn + 1]) { st = VSDLSS_ERR_INVALID; goto fail; }
    free(parent); parent = NULL; free(stack); stack = NULL; free(mark); mark = NULL; free(cursor); cursor = NULL;

    /* values of A: entry (i, j), i <= j, is L(j, i) */
    f->panel = (double *)calloc((size_t)(panel_total ? panel_total : 1), sizeof(double));
    if (!f->panel) goto fail;
    for (j = 0; j < n; j++) for (p = A->p[j]; p < A->p[j + 1]; p++) {
        const csi i = A->i[p], t = snode[i], begin = f->column_start[t];
        const csi rows = f->column_start[t + 1] - begin + f->row_ptr[t + 1] - f->row_ptr[t];
        const csi at = local_row(f, t, j, 0);
        double *slot;
        if (at < 0) { st = VSDLSS_ERR_INVALID; goto fail; }
        slot = f->panel + f->panel_offset[t] + (i - begin) * rows + at;
        *slot += A->x[p];
        if (!isfinite(*slot)) { st = VSDLSS_ERR_NONFINITE; goto fail; }
    }

    /* factor each supernode, then subtract its updates */
    lpos = (csi *)malloc((size_t)(max_ext ? max_ext : 1) * sizeof(csi));
    run = (csi *)malloc((size_t)(max_ext ? max_ext : 1) * sizeof(csi));
    dot = (double *)malloc((size_t)(max_ext ? max_ext : 1) * sizeof(double));
    if (!lpos || !run || !dot) goto fail;
    for (sn = 0; sn < f->count; sn++) {
        const csi begin = f->column_start[sn], w = f->column_start[sn + 1] - begin;
        const csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn], rows = w + ext;
        const csi *ri = f->row_index + f->row_ptr[sn];
        double *a = f->panel + f->panel_offset[sn];
        int bad = 0;
        st = panel_factor(a, rows, w);
        if (st != VSDLSS_OK) goto fail;
        for (csi q0 = 0; q0 < ext;) {
            /* rows q0..q1 of the external rows are columns of supernode t */
            const csi t = snode[ri[q0]], tb = f->column_start[t], te = f->column_start[t + 1];
            const csi trows = te - tb + f->row_ptr[t + 1] - f->row_ptr[t];
            csi q1 = q0, from = 0;
            while (q1 < ext && ri[q1] < te) q1++;
            for (csi r = q0; r < ext; r++) {
                const csi at = local_row(f, t, ri[r], from);
                if (at < 0) { st = VSDLSS_ERR_INVALID; goto fail; }
                lpos[r] = at;
                if (at >= te - tb) from = at - (te - tb);
            }
            /* runs: lpos[r..run[r]) are consecutive slots */
            run[ext - 1] = ext;
            for (csi r = ext - 1; r-- > q0;) run[r] = lpos[r + 1] == lpos[r] + 1 ? run[r + 1] : r + 1;
            for (csi q = q0; q < q1; q++)
                bad |= update_column(a, rows, w, ext, q, f->panel + f->panel_offset[t] + (ri[q] - tb) * trows, lpos, run, dot);
            q0 = q1;
        }
        if (bad) { st = VSDLSS_ERR_NONFINITE; goto fail; }
    }
    free(lpos); free(run); free(dot); free(snode);
    *out = f;
    return VSDLSS_OK;
fail:
    free(parent); free(cc); free(stack); free(mark); free(snode); free(cursor); free(lpos); free(run); free(dot);
    vsdlss_sn_factor_free(f);
    return st;
}

/* vsdlss_sn_solve in place: the same forward and backward sweeps over the
 * supernodes, the same operations on every value in the same order (an
 * external row receives its updates column by column, as the original's
 * running sum), without a parallel region per supernode.  A value that is
 * not finite stays so through every later update, so tracking whether one
 * has appeared (check: x - x sums to 0 exactly while all are finite) gives
 * the original's status at each of its tests. */
vsdlss_status vsdlss_m3_sn_solve(const vsdlss_sn_factor *f, double *x)
{
    double check = 0;
    if (!f || !x || f->n < 1) return VSDLSS_ERR_INVALID;
    for (csi j = 0; j < f->n; j++) check += x[j] - x[j];
    if (!(check == 0)) return VSDLSS_ERR_NONFINITE;
    for (csi sn = 0; sn < f->count; sn++) {
        const csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
        const csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn], rows = w + ext;
        const csi *idx = f->row_index + f->row_ptr[sn];
        const double *a = f->panel + f->panel_offset[sn];
        double *xs = x + b;
        for (csi j = 0; j < w; j++) {
            const double d = a[j * rows + j], *col = a + j * rows;
            if (!isfinite(d) || d <= 0) return VSDLSS_ERR_INVALID;
            xs[j] /= d;
            if (!isfinite(xs[j])) return VSDLSS_ERR_NONFINITE;
            for (csi r = j + 1; r < w; r++) xs[r] -= col[r] * xs[j];
        }
        for (csi j = 0; j < w; j++) {
            const double *col = a + j * rows + w, s = xs[j];
            for (csi t = 0; t < ext; t++) x[idx[t]] -= col[t] * s;
        }
        for (csi t = 0; t < ext; t++) check += x[idx[t]] - x[idx[t]];
        if (!(check == 0)) return VSDLSS_ERR_NONFINITE;
    }
    for (csi sn = f->count; sn-- > 0;) {
        const csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
        const csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn], rows = w + ext;
        const csi *idx = f->row_index + f->row_ptr[sn];
        const double *a = f->panel + f->panel_offset[sn];
        double *xs = x + b;
        for (csi t = 0; t < w; t++) {
            const double *col = a + t * rows + w;
            double v = xs[t];
            for (csi r = 0; r < ext; r++) v -= col[r] * x[idx[r]];
            xs[t] = v; check += v - v;
        }
        if (!(check == 0)) return VSDLSS_ERR_NONFINITE;
        for (csi j = w; j-- > 0;) {
            const double d = a[j * rows + j], *col = a + j * rows;
            double v = xs[j];
            if (!isfinite(d) || d <= 0) return VSDLSS_ERR_INVALID;
            for (csi r = j + 1; r < w; r++) v -= col[r] * xs[r];
            xs[j] = v / d;
            if (!isfinite(xs[j])) return VSDLSS_ERR_NONFINITE;
        }
    }
    return VSDLSS_OK;
}
