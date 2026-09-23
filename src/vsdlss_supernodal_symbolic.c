#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Supernodal symbolic analysis.
 *
 *   1. elimination tree, postorder and column counts of L (Gilbert-Ng-Peyton,
 *      O(nnz(A) alpha(n)); no pass over the entries of L);
 *   2. maximal supernodes: j and j+1 share a panel iff parent(j) = j+1 and
 *      |L(:,j)| = |L(:,j+1)| + 1, i.e. L(j+1:n,j) has the pattern of L(:,j+1);
 *   3. external rows of every supernode as the union of the lower pattern of
 *      A in its columns and the external rows of its child supernodes;
 *   4. optional relaxed amalgamation of a supernode into its parent;
 *   5. per-target lists of source blocks for the left-looking numeric phase.
 *
 * Everything is O(nnz(A) + sum_s |R_s| log |R_s|) time and O(n + sum |R_s|)
 * memory.  The former layout also materialised the pattern of L and, per
 * source panel, a |R|(|R|+1)/2 update-target table; on a 26^3 grid ordered by
 * MLD that table alone held 1.7e8 entries. */

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

/* width*(width+1)/2 + width*ext, the stored lower trapezoid of a panel. */
static int trapezoid(csi width, csi ext, csi *out)
{
    csi tri, rect;
    if (!checked_mul(width, width + 1, &tri) || !checked_mul(width, ext, &rect))
        return 0;
    return checked_add(tri / 2, rect, out);
}

static int cmp_csi(const void *a, const void *b)
{
    csi x = *(const csi *)a, y = *(const csi *)b;
    return (x > y) - (x < y);
}

void vsdlss_sn_symbolic_free(vsdlss_sn_symbolic *s)
{
    if (!s) return;
    free(s->sn_parent); free(s->column_start); free(s->row_ptr);
    free(s->row_index); free(s->panel_offset); free(s->blk_ptr);
    free(s->blk_src); free(s->blk_first); free(s->blk_end); free(s);
}

vsdlss_status vsdlss_postorder_permutation(const vsdlss *A, csi *q, csi *pinv)
{
    vsdlss *P = NULL; csi *parent = NULL, *post = NULL, *old = NULL, n, k;
    vsdlss_status st = VSDLSS_ERR_OOM;
    if (!A || !q || !pinv || A->n < 1) return VSDLSS_ERR_INVALID;
    n = A->n;
    P = vsdlss_symperm(A, pinv, 0);
    if (!P) goto done;
    parent = vsdlss_etree(P, 0);
    if (!parent) goto done;
    post = vsdlss_post(parent, n);
    old = (csi *)vsdlss_malloc(n, sizeof(csi));
    if (!post || !old) goto done;
    memcpy(old, q, (size_t)n * sizeof(csi));
    for (k = 0; k < n; ++k) { q[k] = old[post[k]]; pinv[q[k]] = k; }
    st = vsdlss_validate_permutation(q, pinv, n);
done:
    vsdlss_spfree(P); free(parent); free(post); free(old);
    return st;
}

/* CHOLMOD's default amalgamation rule (nrelax = 4/16/48, zrelax 0.8/0.1/0.05). */
static int relax_ok(csi cols, double zeros, double total)
{
    double frac = total > 0 ? zeros / total : 0;
    if (cols <= 4) return 1;
    if (cols <= 16 && frac < 0.8) return 1;
    if (cols <= 48 && frac < 0.1) return 1;
    return frac < 0.05;
}

static vsdlss_status analyze(const vsdlss *A, vsdlss_sn_symbolic **out, int relax)
{
    vsdlss_sn_symbolic *z = NULL;
    vsdlss *AT = NULL;
    csi *parent = NULL, *post = NULL, *cc = NULL;
    csi *fstart = NULL, *fparent = NULL, *fowner = NULL, *fptr = NULL, *frows = NULL;
    csi *head = NULL, *next = NULL, *mark = NULL, *gstart = NULL, *owner = NULL;
    csi n, j, k, fcount = 0, total, s, p;
    vsdlss_status status;

    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!A || A->n < 1) return VSDLSS_ERR_INVALID;
    n = A->n;
    if (n == INT64_MAX || !checked_count(n + 1, sizeof(csi))) return VSDLSS_ERR_OOM;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    status = VSDLSS_ERR_OOM;

    /* 1. etree, postorder, column counts (diagonal included). */
    parent = vsdlss_etree(A, 0);
    post = parent ? vsdlss_post(parent, n) : NULL;
    cc = post ? vsdlss_counts(A, parent, post, 0) : NULL;
    AT = vsdlss_transpose(A, 0);   /* column j of AT = rows i >= j of A */
    if (!cc || !AT) goto fail;

    /* 2. maximal supernodes over the given column order. */
    fstart = (csi *)vsdlss_big_malloc((size_t)(n + 1) * sizeof(csi));
    fowner = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    if (!fstart || !fowner) goto fail;
    for (j = 0; j < n; ++j) {
        if (cc[j] < 1 || cc[j] > n - j) { status = VSDLSS_ERR_INVALID; goto fail; }
        if (j == 0 || !(parent[j - 1] == j && cc[j - 1] == cc[j] + 1))
            fstart[fcount++] = j;
        fowner[j] = fcount - 1;
    }
    fstart[fcount] = n;
    fparent = (csi *)malloc((size_t)fcount * sizeof(csi));
    fptr = (csi *)malloc((size_t)(fcount + 1) * sizeof(csi));
    head = (csi *)malloc((size_t)fcount * sizeof(csi));
    next = (csi *)malloc((size_t)fcount * sizeof(csi));
    mark = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    if (!fparent || !fptr || !head || !next || !mark) goto fail;
    fptr[0] = 0;
    for (s = 0; s < fcount; ++s) {
        csi last = fstart[s + 1] - 1;
        fparent[s] = parent[last] < 0 ? -1 : fowner[parent[last]];
        if (!checked_add(fptr[s], cc[last] - 1, &fptr[s + 1])) goto fail;
        head[s] = -1;
    }
    if (!checked_count(fptr[fcount], sizeof(csi))) goto fail;
    frows = (csi *)vsdlss_big_malloc((size_t)(fptr[fcount] ? fptr[fcount] : 1) * sizeof(csi));
    if (!frows) goto fail;
    for (s = fcount - 1; s >= 0; --s)
        if (fparent[s] >= 0) { next[s] = head[fparent[s]]; head[fparent[s]] = s; }
    for (j = 0; j < n; ++j) mark[j] = -1;

    /* 3. external rows: union of A's lower pattern and children's rows. */
    for (s = 0; s < fcount; ++s) {
        csi b = fstart[s], e = fstart[s + 1], cnt = 0, want = fptr[s + 1] - fptr[s];
        csi *R = frows + fptr[s];
        for (j = b; j < e; ++j)
            for (p = AT->p[j]; p < AT->p[j + 1]; ++p) {
                csi i = AT->i[p];
                if (i >= e && mark[i] != s) {
                    if (cnt == want) { status = VSDLSS_ERR_INVALID; goto fail; }
                    mark[i] = s; R[cnt++] = i;
                }
            }
        for (csi c = head[s]; c >= 0; c = next[c])
            for (p = fptr[c]; p < fptr[c + 1]; ++p) {
                csi i = frows[p];
                if (i >= e && mark[i] != s) {
                    if (cnt == want) { status = VSDLSS_ERR_INVALID; goto fail; }
                    mark[i] = s; R[cnt++] = i;
                }
            }
        if (cnt != want) { status = VSDLSS_ERR_INVALID; goto fail; }
        qsort(R, (size_t)cnt, sizeof(csi), cmp_csi);
    }

    /* 4. amalgamation.  Supernodes s, s+1, ..., t form a group when each is
     * the last child of the next; the group keeps R_t as external rows and
     * stores zeros for entries of the lower members outside their pattern. */
    gstart = (csi *)malloc((size_t)(fcount + 1) * sizeof(csi));
    if (!gstart) goto fail;
    {
        /* Per group keyed by its first supernode: true entries and columns. */
        double *gtrue = (double *)malloc((size_t)fcount * sizeof(double));
        csi *gcols = (csi *)malloc((size_t)fcount * sizeof(csi));
        csi *gtop = (csi *)malloc((size_t)fcount * sizeof(csi));
        char *merged = (char *)calloc((size_t)fcount, 1);
        csi groups = 0;
        if (!gtrue || !gcols || !gtop || !merged) {
            free(gtrue); free(gcols); free(gtop); free(merged); goto fail;
        }
        for (s = 0; s < fcount; ++s) {
            csi w = fstart[s + 1] - fstart[s];
            gcols[s] = w; gtop[s] = s;
            gtrue[s] = (double)w * (double)(w + 1) / 2 +
                       (double)w * (double)(fptr[s + 1] - fptr[s]);
        }
        if (relax) for (s = fcount - 2; s >= 0; --s) {
            csi g = s + 1, ext, cols;
            double panel;
            if (fparent[s] != g) continue;
            ext = fptr[gtop[g] + 1] - fptr[gtop[g]];
            cols = gcols[g] + gcols[s];
            panel = (double)cols * (double)(cols + 1) / 2 + (double)cols * (double)ext;
            if (!relax_ok(cols, panel - gtrue[g] - gtrue[s], panel)) continue;
            merged[g] = 1;               /* g no longer starts a group */
            gcols[s] = cols; gtop[s] = gtop[g]; gtrue[s] += gtrue[g];
        }
        for (s = 0; s < fcount; ++s) if (!merged[s]) gstart[groups++] = s;
        gstart[groups] = fcount;
        free(gtrue); free(gcols); free(gtop); free(merged);

        z = (vsdlss_sn_symbolic *)calloc(1, sizeof(*z));
        if (!z) goto fail;
        z->n = n; z->count = groups;
    }
    z->column_start = (csi *)malloc((size_t)(z->count + 1) * sizeof(csi));
    z->row_ptr = (csi *)malloc((size_t)(z->count + 1) * sizeof(csi));
    z->panel_offset = (csi *)malloc((size_t)(z->count + 1) * sizeof(csi));
    z->sn_parent = (csi *)malloc((size_t)z->count * sizeof(csi));
    z->blk_ptr = (csi *)calloc((size_t)z->count + 1, sizeof(csi));
    if (!z->column_start || !z->row_ptr || !z->panel_offset || !z->sn_parent ||
        !z->blk_ptr) goto fail;
    z->row_ptr[0] = z->panel_offset[0] = 0;
    {
        csi strict_nnz = 0;
        for (s = 0; s < z->count; ++s) {
            csi top = gstart[s + 1] - 1, width, ext, rows, size;
            z->column_start[s] = fstart[gstart[s]];
            width = fstart[top + 1] - fstart[gstart[s]];
            ext = fptr[top + 1] - fptr[top];
            if (!checked_add(z->row_ptr[s], ext, &z->row_ptr[s + 1]) ||
                !checked_add(width, ext, &rows) ||
                !checked_mul(rows, width, &size) ||
                !checked_add(z->panel_offset[s], size, &z->panel_offset[s + 1]) ||
                !trapezoid(width, ext, &size) ||
                !checked_add(z->l_nnz, size, &z->l_nnz)) goto fail;
            if (rows > z->max_rows) z->max_rows = rows;
        }
        z->column_start[z->count] = n;
        for (j = 0; j < n; ++j) strict_nnz += cc[j];
        z->relaxed_zeros = z->l_nnz - strict_nnz;
    }
    if (!checked_count(z->row_ptr[z->count], sizeof(csi)) ||
        !checked_count(z->panel_offset[z->count], sizeof(double))) goto fail;
    z->row_index = (csi *)vsdlss_big_malloc((size_t)(z->row_ptr[z->count] ? z->row_ptr[z->count] : 1) *
                                 sizeof(csi));
    owner = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    if (!z->row_index || !owner) goto fail;
    for (s = 0; s < z->count; ++s) {
        csi top = gstart[s + 1] - 1;
        memcpy(z->row_index + z->row_ptr[s], frows + fptr[top],
               (size_t)(fptr[top + 1] - fptr[top]) * sizeof(csi));
        for (j = z->column_start[s]; j < z->column_start[s + 1]; ++j) owner[j] = s;
    }

    /* 5. source blocks, grouped by target and sorted by source. */
    total = 0;
    for (s = 0; s < z->count; ++s) {
        const csi *R = z->row_index + z->row_ptr[s];
        csi ext = z->row_ptr[s + 1] - z->row_ptr[s];
        z->sn_parent[s] = ext ? owner[R[0]] : -1;
        for (k = 0; k < ext;) {
            csi d = owner[R[k]], e = k + 1;
            if (d <= s) { status = VSDLSS_ERR_INVALID; goto fail; }
            while (e < ext && owner[R[e]] == d) ++e;
            z->blk_ptr[d + 1]++; ++total; k = e;
        }
    }
    for (s = 0; s < z->count; ++s) z->blk_ptr[s + 1] += z->blk_ptr[s];
    z->blk_src = (csi *)malloc((size_t)(total ? total : 1) * sizeof(csi));
    z->blk_first = (csi *)malloc((size_t)(total ? total : 1) * sizeof(csi));
    z->blk_end = (csi *)malloc((size_t)(total ? total : 1) * sizeof(csi));
    if (!z->blk_src || !z->blk_first || !z->blk_end) goto fail;
    for (s = 0; s < z->count; ++s) mark[s] = z->blk_ptr[s];
    for (s = 0; s < z->count; ++s) {
        const csi *R = z->row_index + z->row_ptr[s];
        csi ext = z->row_ptr[s + 1] - z->row_ptr[s];
        for (k = 0; k < ext;) {
            csi d = owner[R[k]], e = k + 1, at;
            while (e < ext && owner[R[e]] == d) ++e;
            at = mark[d]++;
            z->blk_src[at] = s; z->blk_first[at] = k; z->blk_end[at] = e; k = e;
        }
    }
    status = VSDLSS_OK;
fail:
    vsdlss_spfree(AT);
    free(parent); free(post); free(cc); free(fstart); free(fparent); free(fowner);
    free(fptr); free(frows); free(head); free(next); free(mark); free(gstart);
    free(owner);
    if (status != VSDLSS_OK) { vsdlss_sn_symbolic_free(z); return status; }
    *out = z; return VSDLSS_OK;
}

vsdlss_status vsdlss_sn_analyze(const vsdlss *A, vsdlss_sn_symbolic **out)
{ return analyze(A, out, 0); }
vsdlss_status vsdlss_sn_analyze_relaxed(const vsdlss *A, vsdlss_sn_symbolic **out)
{ return analyze(A, out, 1); }
