#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

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

void vsdlss_sn_symbolic_free(vsdlss_sn_symbolic *s)
{
    if (!s) return;
    free(s->parent); free(s->l_col_ptr); free(s->l_row_index);
    free(s->column_start); free(s->row_ptr); free(s->row_index);
    free(s->panel_offset); free(s->l_panel_slot);
    free(s->update_ptr); free(s->update_target); free(s);
}

static vsdlss_status analyze(const vsdlss *A, vsdlss_sn_symbolic **out, int compact)
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
            ext == INT64_MAX || !checked_mul(compact?0:ext, ext + 1, &triangle)) {
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

    /* Panel-slot and update-target construction touch only per-supernode
     * output slices (l_panel_slot follows l_col_ptr, update_target follows
     * update_ptr), so both loops are data parallel over supernodes.  Both
     * inner searches walk two ascending index lists, so a single advancing
     * cursor replaces the former per-entry binary search: L column patterns
     * and the external row list R are stored in increasing row order.
     * Only integer indices are produced here; numeric results are unchanged. */
    total = z->update_ptr[z->count];
    {
    int bad = 0;
    int nt = vsdlss_parallel_width((double)z->l_nnz + (double)total);
    if (nt > z->count) nt = (int)z->count;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(guided))
        for (csi t = 0; t < z->count; ++t) {
            csi begin = z->column_start[t], end = z->column_start[t + 1];
            csi width = end - begin, ext = z->row_ptr[t + 1] - z->row_ptr[t];
            csi rows = width + ext, jj, pp;
            csi *R = z->row_index + z->row_ptr[t];
            if (ext)
                memcpy(R, z->l_row_index + z->l_col_ptr[end - 1] + 1,
                       (size_t)ext * sizeof(csi));
            for (jj = begin; jj < end; ++jj) {
                csi cursor2 = 0;
                for (pp = z->l_col_ptr[jj]; pp < z->l_col_ptr[jj + 1]; ++pp) {
                    csi row = z->l_row_index[pp], local_row;
                    if (row < end) local_row = row - begin;
                    else {
                        while (cursor2 < ext && R[cursor2] < row) ++cursor2;
                        if (cursor2 == ext || R[cursor2] != row) { bad = 1; break; }
                        local_row = width + cursor2;
                    }
                    z->l_panel_slot[pp] = z->panel_offset[t] + (jj - begin) * rows + local_row;
                }
                if (bad) break;
            }
        }
    }
    if (bad) { status = VSDLSS_ERR_INVALID; goto fail; }
    if (!compact) {
        VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
        {
            VSDLSS_OMP(omp master)
            vsdlss_parallel_observe();
            VSDLSS_OMP(omp for schedule(guided))
            for (csi t = 0; t < z->count; ++t) {
                csi ext = z->row_ptr[t + 1] - z->row_ptr[t], q, r;
                const csi *R = z->row_index + z->row_ptr[t];
                csi out = z->update_ptr[t];
                for (q = 0; q < ext; ++q) {
                    csi col = R[q], lo = z->l_col_ptr[col], hi = z->l_col_ptr[col + 1];
                    for (r = q; r < ext; ++r) {
                        csi row = R[r];
                        while (lo < hi && z->l_row_index[lo] < row) ++lo;
                        if (lo == hi || z->l_row_index[lo] != row) { bad = 1; break; }
                        z->update_target[out++] = z->l_panel_slot[lo];
                    }
                    if (bad) break;
                }
            }
        }
        if (bad) { status = VSDLSS_ERR_INVALID; goto fail; }
    }
    }
    free(stack); free(mark); free(count); free(cursor);
    *out = z; return VSDLSS_OK;
fail:
    free(stack); free(mark); free(count); free(cursor);
    vsdlss_sn_symbolic_free(z); return status;
}

vsdlss_status vsdlss_sn_analyze(const vsdlss *A, vsdlss_sn_symbolic **out)
{ return analyze(A,out,0); }
vsdlss_status vsdlss_sn_analyze_compact(const vsdlss *A, vsdlss_sn_symbolic **out)
{ return analyze(A,out,1); }
