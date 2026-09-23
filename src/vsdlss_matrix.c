#include "vsdlss.h"
#include "vsdlss_parallel.h"

#include <float.h>
#include <limits.h>
#include <string.h>

typedef struct matrix_entry {
    csi row;
    double value;
} matrix_entry;

static int checked_bytes(csi count, size_t width, size_t *bytes)
{
    if (!bytes || count < 0 || (uint64_t)count > SIZE_MAX / width) return 0;
    *bytes = (size_t)count * width;
    return 1;
}

static int compare_entry(const void *a, const void *b)
{
    const matrix_entry *ea = (const matrix_entry *)a;
    const matrix_entry *eb = (const matrix_entry *)b;
    return (ea->row > eb->row) - (ea->row < eb->row);
}

vsdlss_status vsdlss_validate_upper_csc(const vsdlss *A)
{
    csi j, k, nnz;
    size_t ignored;
    if (!A || A->m < 1 || A->n < 1 || A->m != A->n || A->nz != -1 ||
        !A->p || !A->i || !A->x || A->nzmax < 0)
        return VSDLSS_ERR_INVALID;
    if (!checked_bytes(A->n + 1, sizeof(csi), &ignored) || A->p[0] != 0)
        return VSDLSS_ERR_INVALID;
    for (j = 0; j < A->n; ++j)
        if (A->p[j] < 0 || A->p[j] > A->p[j + 1]) return VSDLSS_ERR_INVALID;
    nnz = A->p[A->n];
    if (nnz < 0 || nnz > A->nzmax ||
        !checked_bytes(nnz, sizeof(csi), &ignored) ||
        !checked_bytes(nnz, sizeof(double), &ignored))
        return VSDLSS_ERR_INVALID;
    /* Entry checks in parallel; the status of the first offending entry in
     * column-major order is returned, exactly as a serial scan would. */
    {
        csi first_bad_index = INT64_MAX, first_bad_value = INT64_MAX;
        int nt = vsdlss_parallel_width((double)nnz * 2);
        (void)nt; (void)k;
        if (nt <= 1) {
            for (j = 0; j < A->n; ++j)
                for (csi q = A->p[j]; q < A->p[j + 1]; ++q) {
                    if (A->i[q] < 0 || A->i[q] > j || A->i[q] >= A->m) return VSDLSS_ERR_INVALID;
                    if (!isfinite(A->x[q])) return VSDLSS_ERR_NONFINITE;
                }
            return VSDLSS_OK;
        }
        VSDLSS_OMP(omp parallel for num_threads(nt) schedule(static) reduction(min:first_bad_index,first_bad_value))
        for (csi c = 0; c < A->n; ++c) {
            for (csi q = A->p[c]; q < A->p[c + 1]; ++q) {
                if (A->i[q] < 0 || A->i[q] > c || A->i[q] >= A->m) { if (q < first_bad_index) first_bad_index = q; }
                else if (!isfinite(A->x[q]) && q < first_bad_value) first_bad_value = q;
            }
        }
        if (first_bad_index < first_bad_value) return VSDLSS_ERR_INVALID;
        if (first_bad_value != INT64_MAX) return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

/* Validated upper CSC whose columns are strictly increasing (sorted, no
 * duplicates): exactly what vsdlss_normalize_upper would return. */
int vsdlss_is_normalized_upper(const vsdlss *A)
{
    int ok = 1, nt = vsdlss_parallel_width((double)A->p[A->n] * 2);
    (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static) reduction(&&:ok))
    for (csi j = 0; j < A->n; ++j)
        for (csi k = A->p[j] + 1; k < A->p[j + 1]; ++k)
            if (A->i[k] <= A->i[k - 1]) ok = 0;
    return ok;
}

/* Output length of one column after sorting and merging duplicates, or -1
 * when the column is already strictly increasing (copied verbatim). */
static csi column_unique(const vsdlss *A, csi j, matrix_entry *work)
{
    csi len = A->p[j + 1] - A->p[j], k, u = 0;
    const csi *ri = A->i + A->p[j];
    for (k = 1; k < len; ++k) if (ri[k] <= ri[k - 1]) break;
    if (k >= len) return -1;
    for (k = 0; k < len; ++k) { work[k].row = ri[k]; work[k].value = A->x[A->p[j] + k]; }
    qsort(work, (size_t)len, sizeof(*work), compare_entry);
    for (k = 0; k < len; ++k) if (k == 0 || work[k].row != work[k - 1].row) ++u;
    return u;
}

/* Sorted/merged columns.  Columns that are already strictly increasing (the
 * usual case) are copied without sorting; the others are sorted exactly as
 * before, so the result is unchanged.  Both passes are column-parallel. */
vsdlss_status vsdlss_normalize_upper(const vsdlss *A, vsdlss **out)
{
    vsdlss_status status;
    vsdlss *C = NULL;
    csi j, max_col = 0, *len = NULL;
    size_t bytes;
    int bad = 0, nt;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    for (j = 0; j < A->n; ++j) {
        csi l = A->p[j + 1] - A->p[j];
        if (l > max_col) max_col = l;
    }
    if (!checked_bytes(max_col > 0 ? max_col : 1, sizeof(matrix_entry), &bytes))
        return VSDLSS_ERR_INVALID;
    len = (csi *)vsdlss_big_malloc((size_t)(A->n + 1) * sizeof(csi));
    C = vsdlss_spalloc(A->n, A->n, A->p[A->n], 1, 0);
    if (!len || !C) { free(len); vsdlss_spfree(C); return VSDLSS_ERR_OOM; }
    nt = vsdlss_parallel_width((double)A->p[A->n] * 4);
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
    {
        matrix_entry *work = (matrix_entry *)malloc(bytes);
        if (!work) bad = 1;
        VSDLSS_OMP(omp for schedule(static))
        for (csi c = 0; c < A->n; ++c) {
            csi u;
            if (!work) continue;
            u = column_unique(A, c, work);
            len[c] = u < 0 ? A->p[c + 1] - A->p[c] : u;
        }
        free(work);
    }
    if (bad) { free(len); vsdlss_spfree(C); return VSDLSS_ERR_OOM; }
    C->p[0] = 0;
    for (j = 0; j < A->n; ++j) C->p[j + 1] = C->p[j] + len[j];
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
    {
        matrix_entry *work = (matrix_entry *)malloc(bytes);
        if (!work) bad = 1;
        VSDLSS_OMP(omp for schedule(static))
        for (csi c = 0; c < A->n; ++c) {
            csi l = A->p[c + 1] - A->p[c], dst = C->p[c], k;
            if (!work) continue;
            if (len[c] == l) {
                /* Either strictly increasing or sorted without duplicates. */
                const csi *ri = A->i + A->p[c]; int sorted = 1;
                for (k = 1; k < l; ++k) if (ri[k] <= ri[k - 1]) { sorted = 0; break; }
                if (sorted) {
                    memcpy(C->i + dst, ri, (size_t)l * sizeof(csi));
                    memcpy(C->x + dst, A->x + A->p[c], (size_t)l * sizeof(double));
                    continue;
                }
            }
            for (k = 0; k < l; ++k) { work[k].row = A->i[A->p[c] + k]; work[k].value = A->x[A->p[c] + k]; }
            qsort(work, (size_t)l, sizeof(*work), compare_entry);
            for (k = 0; k < l; ++k) {
                if (k > 0 && work[k].row == work[k - 1].row) {
                    C->x[dst - 1] += work[k].value;
                    if (!isfinite(C->x[dst - 1])) bad = 2;
                } else {
                    C->i[dst] = work[k].row;
                    C->x[dst] = work[k].value;
                    ++dst;
                }
            }
        }
        free(work);
    }
    free(len);
    if (bad) { vsdlss_spfree(C); return (bad & 1) ? VSDLSS_ERR_OOM : VSDLSS_ERR_NONFINITE; }
    C->nzmax = C->p[A->n] > 0 ? C->p[A->n] : 1;
    *out = C;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_spmv_sym_upper(const vsdlss *A, const double *x, double *y)
{
    vsdlss_status status = vsdlss_validate_upper_csc(A);
    csi j, k;
    if (status != VSDLSS_OK || !x || !y) return status != VSDLSS_OK ? status : VSDLSS_ERR_INVALID;
    for (j = 0; j < A->n; ++j) {
        if (!isfinite(x[j])) return VSDLSS_ERR_NONFINITE;
        y[j] = 0.0;
    }
    for (j = 0; j < A->n; ++j) {
        for (k = A->p[j]; k < A->p[j + 1]; ++k) {
            csi row = A->i[k];
            y[row] += A->x[k] * x[j];
            if (row != j) y[j] += A->x[k] * x[row];
        }
    }
    for (j = 0; j < A->n; ++j) if (!isfinite(y[j])) return VSDLSS_ERR_NONFINITE;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_backward_error(const vsdlss *A, const double *x,
                                    const double *b, double *eta)
{
    double *ax = NULL, *rows = NULL;
    double norm_a = 0.0, norm_x = 0.0, norm_b = 0.0, norm_r = 0.0, denom;
    csi j, k;
    vsdlss_status status;
    size_t bytes;
    if (!eta || !b) return VSDLSS_ERR_INVALID;
    *eta = NAN;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK || !x) return status != VSDLSS_OK ? status : VSDLSS_ERR_INVALID;
    if (!checked_bytes(A->n, sizeof(double), &bytes)) return VSDLSS_ERR_INVALID;
    ax = (double *)malloc(bytes);
    rows = (double *)calloc((size_t)A->n, sizeof(double));
    if (!ax || !rows) { free(ax); free(rows); return VSDLSS_ERR_OOM; }
    status = vsdlss_spmv_sym_upper(A, x, ax);
    if (status != VSDLSS_OK) { free(ax); free(rows); return status; }
    for (j = 0; j < A->n; ++j) {
        if (!isfinite(b[j])) { free(ax); free(rows); return VSDLSS_ERR_NONFINITE; }
        if (fabs(x[j]) > norm_x) norm_x = fabs(x[j]);
        if (fabs(b[j]) > norm_b) norm_b = fabs(b[j]);
        if (fabs(b[j] - ax[j]) > norm_r) norm_r = fabs(b[j] - ax[j]);
        for (k = A->p[j]; k < A->p[j + 1]; ++k) {
            csi row = A->i[k];
            rows[row] += fabs(A->x[k]);
            if (row != j) rows[j] += fabs(A->x[k]);
        }
    }
    for (j = 0; j < A->n; ++j) if (rows[j] > norm_a) norm_a = rows[j];
    denom = norm_a * norm_x + norm_b;
    if (!isfinite(denom)) status = VSDLSS_ERR_NONFINITE;
    else if (denom == 0.0) *eta = norm_r == 0.0 ? 0.0 : INFINITY;
    else *eta = norm_r / denom;
    free(ax);
    free(rows);
    return status;
}
