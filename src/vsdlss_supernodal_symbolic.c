#include "vsdlss_m3_internal.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int checked_count(csi n, size_t width)
{ return n >= 0 && (uint64_t)n <= SIZE_MAX / width; }

static int checked_add(csi a, csi b, csi *out)
{
    if (a < 0 || b < 0 || a > INT64_MAX - b) return 0;
    *out = a + b; return 1;
}

static int checked_mul(csi a, csi b, csi *out)
{
    if (a < 0 || b < 0 || (a && b > INT64_MAX / a)) return 0;
    *out = a * b; return 1;
}

static csi find_l_entry(const vsdlss_sn_symbolic *s, csi col, csi row)
{
    csi lo = s->l_col_ptr[col], hi = s->l_col_ptr[col + 1];
    while (lo < hi) {
        csi mid = lo + (hi - lo) / 2;
        if (s->l_row_index[mid] < row) lo = mid + 1;
        else hi = mid;
    }
    return lo < s->l_col_ptr[col + 1] && s->l_row_index[lo] == row ? lo : -1;
}

void vsdlss_sn_symbolic_free(vsdlss_sn_symbolic *s)
{
    if (!s) return;
    free(s->parent); free(s->l_col_ptr); free(s->l_row_index);
    free(s->column_start); free(s->row_ptr); free(s->row_index);
    free(s->panel_offset); free(s->l_panel_slot);
    free(s->update_ptr); free(s->update_target); free(s);
}

vsdlss_status vsdlss_sn_analyze(const vsdlss *A, vsdlss_sn_symbolic **out)
{
    vsdlss_sn_symbolic *z = NULL;
    csi *stack = NULL, *mark = NULL, *count = NULL, *cursor = NULL;
    csi n, k, p, top, j, sn, total, external_total = 0, panel_total = 0;
    csi update_total = 0;
    vsdlss_status status;

    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!A || A->n < 1) return VSDLSS_ERR_INVALID;
    n = A->n;
    if (n == INT64_MAX || !checked_count(n, sizeof(csi)) ||
        !checked_count(n + 1, sizeof(csi))) return VSDLSS_ERR_OOM;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    z = (vsdlss_sn_symbolic *)calloc(1, sizeof(*z));
    count = (csi *)calloc((size_t)n, sizeof(*count));
    stack = (csi *)malloc((size_t)n * sizeof(*stack));
    mark = (csi *)calloc((size_t)n, sizeof(*mark));
    if (!z || !count || !stack || !mark) { status = VSDLSS_ERR_OOM; goto fail; }
    z->n = n;
    z->parent = vsdlss_etree(A, 0);
    if (!z->parent) { status = VSDLSS_ERR_OOM; goto fail; }
    for (k = 0; k < n; ++k) {
        top = vsdlss_ereach(A, k, z->parent, stack, mark);
        if (top < 0) { status = VSDLSS_ERR_INVALID; goto fail; }
        for (p = top; p < n; ++p) {
            j = stack[p];
            if (count[j] == INT64_MAX) { status = VSDLSS_ERR_OOM; goto fail; }
            ++count[j];
        }
    }
    z->l_col_ptr = (csi *)malloc((size_t)(n + 1) * sizeof(csi));
    if (!z->l_col_ptr) { status = VSDLSS_ERR_OOM; goto fail; }
    z->l_col_ptr[0] = 0;
    for (j = 0; j < n; ++j) {
        if (count[j] == INT64_MAX ||
            !checked_add(z->l_col_ptr[j], count[j] + 1, &z->l_col_ptr[j + 1])) {
            status = VSDLSS_ERR_OOM; goto fail;
        }
    }
    z->l_nnz = z->l_col_ptr[n];
    if (!checked_count(z->l_nnz, sizeof(csi))) { status = VSDLSS_ERR_OOM; goto fail; }
    z->l_row_index = (csi *)malloc((size_t)z->l_nnz * sizeof(csi));
    cursor = (csi *)malloc((size_t)n * sizeof(csi));
    if (!z->l_row_index || !cursor) { status = VSDLSS_ERR_OOM; goto fail; }
    for (j = 0; j < n; ++j) {
        cursor[j] = z->l_col_ptr[j]; z->l_row_index[cursor[j]++] = j;
        mark[j] = 0;
    }
    for (k = 0; k < n; ++k) {
        top = vsdlss_ereach(A, k, z->parent, stack, mark);
        for (p = top; p < n; ++p) z->l_row_index[cursor[stack[p]]++] = k;
    }

    z->column_start = (csi *)malloc((size_t)(n + 1) * sizeof(csi));
    if (!z->column_start) { status = VSDLSS_ERR_OOM; goto fail; }
    z->count = 0; z->column_start[0] = 0;
    for (j = 0; j + 1 < n; ++j) {
        csi a0 = z->l_col_ptr[j], a1 = z->l_col_ptr[j + 1];
        csi b0 = z->l_col_ptr[j + 1], b1 = z->l_col_ptr[j + 2];
        int merge = z->parent[j] == j + 1 && a1 - a0 == b1 - b0 + 1;
        if (merge && memcmp(z->l_row_index + a0 + 1, z->l_row_index + b0,
                            (size_t)(b1 - b0) * sizeof(csi)) != 0) merge = 0;
        if (!merge) z->column_start[++z->count] = j + 1;
    }
    z->column_start[++z->count] = n;
    z->row_ptr = (csi *)malloc((size_t)(z->count + 1) * sizeof(csi));
    z->panel_offset = (csi *)malloc((size_t)(z->count + 1) * sizeof(csi));
    z->update_ptr = (csi *)malloc((size_t)(z->count + 1) * sizeof(csi));
    if (!z->row_ptr || !z->panel_offset || !z->update_ptr) {
        status = VSDLSS_ERR_OOM; goto fail;
    }
    z->row_ptr[0] = z->panel_offset[0] = z->update_ptr[0] = 0;
    for (sn = 0; sn < z->count; ++sn) {
        csi begin = z->column_start[sn], end = z->column_start[sn + 1];
        csi width = end - begin;
        csi ext = z->l_col_ptr[end - 1 + 1] - z->l_col_ptr[end - 1] - 1;
        csi rows, panel_size, triangle;
        if (!checked_add(external_total, ext, &external_total) ||
            !checked_add(width, ext, &rows) || !checked_mul(rows, width, &panel_size) ||
            !checked_add(panel_total, panel_size, &panel_total) ||
            ext == INT64_MAX || !checked_mul(ext, ext + 1, &triangle)) {
            status = VSDLSS_ERR_OOM; goto fail;
        }
        triangle /= 2;
        if (!checked_add(update_total, triangle, &update_total)) {
            status = VSDLSS_ERR_OOM; goto fail;
        }
        z->row_ptr[sn + 1] = external_total;
        z->panel_offset[sn + 1] = panel_total;
        z->update_ptr[sn + 1] = update_total;
    }
    if (!checked_count(external_total, sizeof(csi)) ||
        !checked_count(panel_total, sizeof(csi)) ||
        !checked_count(update_total, sizeof(csi))) { status = VSDLSS_ERR_OOM; goto fail; }
    if (external_total)
        z->row_index = (csi *)malloc((size_t)external_total * sizeof(csi));
    z->l_panel_slot = (csi *)malloc((size_t)z->l_nnz * sizeof(csi));
    if (update_total)
        z->update_target = (csi *)malloc((size_t)update_total * sizeof(csi));
    if ((external_total && !z->row_index) || !z->l_panel_slot ||
        (update_total && !z->update_target)) { status = VSDLSS_ERR_OOM; goto fail; }

    for (sn = 0; sn < z->count; ++sn) {
        csi begin = z->column_start[sn], end = z->column_start[sn + 1];
        csi width = end - begin, ext = z->row_ptr[sn + 1] - z->row_ptr[sn];
        csi rows = width + ext;
        if (ext)
            memcpy(z->row_index + z->row_ptr[sn],
                   z->l_row_index + z->l_col_ptr[end - 1] + 1,
                   (size_t)ext * sizeof(csi));
        for (j = begin; j < end; ++j) {
            for (p = z->l_col_ptr[j]; p < z->l_col_ptr[j + 1]; ++p) {
                csi row = z->l_row_index[p], local_row;
                if (row < end) local_row = row - begin;
                else {
                    csi lo = 0, hi = ext;
                    while (lo < hi) { csi mid = lo + (hi - lo) / 2;
                        if (z->row_index[z->row_ptr[sn] + mid] < row) lo = mid + 1;
                        else hi = mid; }
                    if (lo == ext || z->row_index[z->row_ptr[sn] + lo] != row) {
                        status = VSDLSS_ERR_INVALID; goto fail;
                    }
                    local_row = width + lo;
                }
                z->l_panel_slot[p] = z->panel_offset[sn] + (j - begin) * rows + local_row;
            }
        }
    }
    total = 0;
    for (sn = 0; sn < z->count; ++sn) {
        csi ext = z->row_ptr[sn + 1] - z->row_ptr[sn], q, r;
        for (q = 0; q < ext; ++q) for (r = q; r < ext; ++r) {
            csi col = z->row_index[z->row_ptr[sn] + q];
            csi row = z->row_index[z->row_ptr[sn] + r];
            csi entry = find_l_entry(z, col, row);
            if (entry < 0) { status = VSDLSS_ERR_INVALID; goto fail; }
            z->update_target[total++] = z->l_panel_slot[entry];
        }
    }
    free(stack); free(mark); free(count); free(cursor);
    *out = z; return VSDLSS_OK;
fail:
    free(stack); free(mark); free(count); free(cursor);
    vsdlss_sn_symbolic_free(z); return status;
}
