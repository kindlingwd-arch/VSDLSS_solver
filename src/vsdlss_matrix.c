#include "vsdlss.h"

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
    for (j = 0; j < A->n; ++j) {
        for (k = A->p[j]; k < A->p[j + 1]; ++k) {
            if (A->i[k] < 0 || A->i[k] > j || A->i[k] >= A->m)
                return VSDLSS_ERR_INVALID;
            if (!isfinite(A->x[k])) return VSDLSS_ERR_NONFINITE;
        }
    }
    return VSDLSS_OK;
}

vsdlss_status vsdlss_normalize_upper(const vsdlss *A, vsdlss **out)
{
    vsdlss_status status;
    vsdlss *C = NULL;
    matrix_entry *work = NULL;
    csi j, k, src, dst = 0, max_col = 0;
    size_t bytes;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    for (j = 0; j < A->n; ++j) {
        csi len = A->p[j + 1] - A->p[j];
        if (len > max_col) max_col = len;
    }
    if (!checked_bytes(max_col > 0 ? max_col : 1, sizeof(*work), &bytes))
        return VSDLSS_ERR_INVALID;
    work = (matrix_entry *)malloc(bytes);
    C = vsdlss_spalloc(A->n, A->n, A->p[A->n], 1, 0);
    if (!work || !C) { free(work); vsdlss_spfree(C); return VSDLSS_ERR_OOM; }
    C->p[0] = 0;
    for (j = 0; j < A->n; ++j) {
        csi len = A->p[j + 1] - A->p[j];
        for (k = 0; k < len; ++k) {
            src = A->p[j] + k;
            work[k].row = A->i[src];
            work[k].value = A->x[src];
        }
        qsort(work, (size_t)len, sizeof(*work), compare_entry);
        for (k = 0; k < len; ++k) {
            if (k > 0 && work[k].row == work[k - 1].row) {
                C->x[dst - 1] += work[k].value;
                if (!isfinite(C->x[dst - 1])) {
                    free(work); vsdlss_spfree(C); return VSDLSS_ERR_NONFINITE;
                }
            } else {
                C->i[dst] = work[k].row;
                C->x[dst] = work[k].value;
                ++dst;
            }
        }
        C->p[j + 1] = dst;
    }
    C->nzmax = dst > 0 ? dst : 1;
    free(work);
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
