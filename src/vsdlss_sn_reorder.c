/* Reordering of the columns inside each supernode, so that the rows a
 * descendant supernode has in it become contiguous.
 *
 * The triangular solves spend most of their time in the external rows of
 * narrow supernodes: x is read and written through the row lists, which on
 * power grids are runs of only ~2 consecutive indices.  A supernode's
 * columns can be permuted freely: its diagonal block is stored dense, its
 * external row set is unchanged, and neither the fill nor the operation
 * count changes (Pichon, Faverge, Ramet, Roman, SIMAX 38(1), 2017).  The
 * order only decides which of those rows are adjacent.
 *
 * Heuristic: partition refinement (Jacquelin, Ng, Peyton, 2017).  The
 * columns of supernode S start as one part; the sets
 *     C(d) = rows of descendant d that fall in S
 * are applied largest first, each splitting every part it cuts into its
 * members followed by the rest.  A set applied while it lies inside one part
 * ends up contiguous, and parts never interleave afterwards, so the large
 * contribution sets (which cost the most row accesses) come out as runs.
 *
 * Changes the elimination order inside supernodes, hence the rounding of
 * the factor: results are deterministic and thread-count independent, but
 * not bitwise those of the unreordered factor. */
#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int32_t start, size; } group_t;

static int cmp_group(const void *a, const void *b)
{
    const group_t *x = (const group_t *)a, *y = (const group_t *)b;
    if (x->size != y->size) return x->size < y->size ? 1 : -1;     /* larger first */
    return (x->start > y->start) - (x->start < y->start);          /* then descendant order */
}

/* Orders one supernode's w columns (local ids 0..w-1) given its incidence
 * list rows[0..m) grouped by descendant (groups g[0..ng), in list order).
 * ord[position] = local column.  Scratch sized for w and m. */
static void refine_one(csi w, const int32_t *rows, group_t *g, csi ng,
                       int32_t *ord, int32_t *pos, int32_t *part, int32_t *pstart,
                       int32_t *pend, int32_t *moved, int32_t *touched)
{
    for (csi i = 0; i < w; ++i) { ord[i] = (int32_t)i; pos[i] = (int32_t)i; part[i] = 0; }
    pstart[0] = 0; pend[0] = (int32_t)w; moved[0] = 0;
    int32_t nparts = 1;
    qsort(g, (size_t)ng, sizeof(*g), cmp_group);
    for (csi k = 0; k < ng; ++k) {
        const int32_t *r = rows + g[k].start;
        int32_t nt = 0;
        for (int32_t t = 0; t < g[k].size; ++t) {
            const int32_t c = r[t], p = part[c];
            if (moved[p] == 0) touched[nt++] = p;
            const int32_t at = pstart[p] + moved[p]++, other = ord[at];
            const int32_t here = pos[c];
            ord[here] = other; pos[other] = here;          /* swap c to the front of p */
            ord[at] = c; pos[c] = at;
        }
        for (int32_t t = 0; t < nt; ++t) {
            const int32_t p = touched[t], m = moved[p];
            moved[p] = 0;
            if (m == pend[p] - pstart[p]) continue;        /* whole part: nothing to split */
            const int32_t np = nparts++;
            pstart[np] = pstart[p]; pend[np] = pstart[p] + m; moved[np] = 0;
            pstart[p] = pend[np];
            for (int32_t at = pstart[np]; at < pend[np]; ++at) part[ord[at]] = np;
        }
    }
}

vsdlss_status vsdlss_sn_reorder_within(const vsdlss_sn_symbolic *s, csi **perm_out)
{
    if (!s || !perm_out) return VSDLSS_ERR_INVALID;
    *perm_out = NULL;
    const csi n = s->n, count = s->count, nnz = s->row_ptr[count];
    if (n >= INT32_MAX || nnz >= INT32_MAX) return VSDLSS_ERR_UNSUPPORTED;
    csi *perm = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    int32_t *col2sn = (int32_t *)malloc((size_t)(n ? n : 1) * sizeof(int32_t));
    csi *iptr = (csi *)calloc((size_t)count + 1, sizeof(csi));
    int32_t *irow = (int32_t *)malloc((size_t)(nnz ? nnz : 1) * sizeof(int32_t));
    int32_t *idesc = (int32_t *)malloc((size_t)(nnz ? nnz : 1) * sizeof(int32_t));
    vsdlss_status st = VSDLSS_ERR_OOM;
    if (!perm || !col2sn || !iptr || !irow || !idesc) goto done;
    for (csi k = 0; k < n; ++k) perm[k] = k;
    csi wmax = 0, mmax = 0;
    for (csi S = 0; S < count; ++S) {
        for (csi c = s->column_start[S]; c < s->column_start[S + 1]; ++c) col2sn[c] = (int32_t)S;
        if (s->column_start[S + 1] - s->column_start[S] > wmax) wmax = s->column_start[S + 1] - s->column_start[S];
    }
    /* Incidences (descendant d, row r in supernode S), grouped by S, in d order. */
    for (csi p = 0; p < nnz; ++p) iptr[col2sn[s->row_index[p]] + 1]++;
    for (csi S = 0; S < count; ++S) {
        if (iptr[S + 1] > mmax) mmax = iptr[S + 1];
        iptr[S + 1] += iptr[S];
    }
    {
        csi *fill = (csi *)malloc((size_t)count * sizeof(csi));
        if (!fill) goto done;
        memcpy(fill, iptr, (size_t)count * sizeof(csi));
        for (csi d = 0; d < count; ++d)
            for (csi p = s->row_ptr[d]; p < s->row_ptr[d + 1]; ++p) {
                const csi r = s->row_index[p], S = col2sn[r], at = fill[S]++;
                irow[at] = (int32_t)(r - s->column_start[S]); idesc[at] = (int32_t)d;
            }
        free(fill);
    }
    int bad = 0;
    int nt = vsdlss_parallel_width((double)nnz * 8); (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
    {
        int32_t *scr = (int32_t *)malloc((size_t)(7 * wmax + 1) * sizeof(int32_t));
        group_t *g = (group_t *)malloc((size_t)(mmax + 1) * sizeof(group_t));
        if (!scr || !g) bad = 1;
        VSDLSS_OMP(omp for schedule(dynamic, 64))
        for (csi S = 0; S < count; ++S) {
            const csi c0 = s->column_start[S], w = s->column_start[S + 1] - c0;
            const csi a = iptr[S], b = iptr[S + 1];
            if (bad || w < 3 || b - a < 2) continue;
            csi ng = 0;
            for (csi p = a; p < b; ) {
                csi q = p; while (q < b && idesc[q] == idesc[p]) ++q;
                g[ng].start = (int32_t)(p - a); g[ng].size = (int32_t)(q - p); ++ng; p = q;
            }
            if (ng < 2) continue;                      /* one set: already one run */
            int32_t *ord = scr, *pos = scr + wmax, *part = scr + 2 * wmax, *pstart = scr + 3 * wmax,
                    *pend = scr + 4 * wmax, *moved = scr + 5 * wmax, *touched = scr + 6 * wmax;
            refine_one(w, irow + a, g, ng, ord, pos, part, pstart, pend, moved, touched);
            for (csi i = 0; i < w; ++i) perm[c0 + i] = c0 + ord[i];
        }
        free(scr); free(g);
    }
    if (bad) goto done;
    *perm_out = perm; perm = NULL; st = VSDLSS_OK;
done:
    free(perm); free(col2sn); free(iptr); free(irow); free(idesc);
    return st;
}
