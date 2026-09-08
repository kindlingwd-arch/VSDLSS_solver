#include "vsdlss_internal.h"

#include <limits.h>
#include <string.h>

vsdlss_status vsdlss_min_degree_subset(const vsdlss *A, const csi *vertices,
                                       csi count, csi *order)
{
    csi *map = NULL, *next = NULL, *local_q = NULL;
    vsdlss *sub = NULL;
    csi k, col, p, nz = count;
    vsdlss_status status = VSDLSS_ERR_OOM;
    if (!A || !vertices || !order || count < 1 || count > A->n) return VSDLSS_ERR_INVALID;
    map = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!map) goto done;
    for (k = 0; k < A->n; ++k) map[k] = -1;
    for (k = 0; k < count; ++k) {
        if (vertices[k] < 0 || vertices[k] >= A->n || map[vertices[k]] >= 0) {
            status = VSDLSS_ERR_INVALID;
            goto done;
        }
        map[vertices[k]] = k;
    }
    for (col = 0; col < A->n; ++col) if (map[col] >= 0) {
        for (p = A->p[col]; p < A->p[col + 1]; ++p)
            if (A->i[p] != col && map[A->i[p]] >= 0) {
                if (nz == INT64_MAX) goto done;
                nz++;
            }
    }
    sub = vsdlss_spalloc(count, count, nz, 1, 0);
    next = (csi *)calloc((size_t)count, sizeof(csi));
    if (!sub || !next) goto done;
    for (k = 0; k < count; ++k) next[k] = 1;
    for (col = 0; col < A->n; ++col) if (map[col] >= 0) {
        for (p = A->p[col]; p < A->p[col + 1]; ++p) if (A->i[p] != col && map[A->i[p]] >= 0) {
            csi a = map[A->i[p]], b = map[col];
            next[a > b ? a : b]++;
        }
    }
    sub->p[0] = 0;
    for (k = 0; k < count; ++k) sub->p[k + 1] = sub->p[k] + next[k];
    memcpy(next, sub->p, (size_t)count * sizeof(csi));
    for (k = 0; k < count; ++k) {
        csi at = next[k]++;
        sub->i[at] = k;
        sub->x[at] = 1.0;
    }
    for (col = 0; col < A->n; ++col) if (map[col] >= 0) {
        for (p = A->p[col]; p < A->p[col + 1]; ++p) if (A->i[p] != col && map[A->i[p]] >= 0) {
            csi a = map[A->i[p]], b = map[col], upper_col = a > b ? a : b;
            csi at = next[upper_col]++;
            sub->i[at] = a < b ? a : b;
            sub->x[at] = 1.0;
        }
    }
    status = vsdlss_min_degree_order(sub, &local_q, NULL);
    if (status != VSDLSS_OK) goto done;
    for (k = 0; k < count; ++k) order[k] = vertices[local_q[k]];
done:
    free(map); free(next); free(local_q); vsdlss_spfree(sub);
    return status;
}

vsdlss_status vsdlss_min_degree_order(const vsdlss *A, csi **q,
                                      vsdlss_order_stats *stats)
{
    vsdlss_graph *graph = NULL;
    csi *order = NULL;
    csi step, predicted;
    vsdlss_status status;
    if (!q) return VSDLSS_ERR_INVALID;
    *q = NULL;
    status = vsdlss_graph_build(A, &graph);
    if (status != VSDLSS_OK) return status;
    if ((uint64_t)A->n > SIZE_MAX / sizeof(csi)) {
        vsdlss_graph_free(graph);
        return VSDLSS_ERR_OOM;
    }
    order = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!order) {
        vsdlss_graph_free(graph);
        return VSDLSS_ERR_OOM;
    }
    predicted = A->n;
    for (step = 0; step < A->n; ++step) {
        csi vertex, best = -1, best_degree = INT64_MAX;
        csi *neighbors = NULL, count = 0;
        for (vertex = 0; vertex < A->n; ++vertex) {
            csi degree = vsdlss_graph_degree(graph, vertex);
            if (degree > 0 || !vsdlss_graph_has_edge(graph, vertex, vertex)) {
                /* Eliminated and isolated vertices both report zero.  An eliminated
                   vertex is recognized because it already appears in order. */
                csi prior;
                int used = 0;
                for (prior = 0; prior < step; ++prior) used |= order[prior] == vertex;
                if (!used && (degree < best_degree ||
                              (degree == best_degree && vertex < best))) {
                    best = vertex;
                    best_degree = degree;
                }
            }
        }
        if (best < 0 || predicted > INT64_MAX - best_degree) {
            free(order);
            vsdlss_graph_free(graph);
            return best < 0 ? VSDLSS_ERR_INVALID : VSDLSS_ERR_OOM;
        }
        order[step] = best;
        predicted += best_degree;
        status = vsdlss_graph_eliminate(graph, best, &neighbors, &count);
        free(neighbors);
        if (status != VSDLSS_OK) {
            free(order);
            vsdlss_graph_free(graph);
            return status;
        }
    }
    if (stats) {
        stats->predicted_nnz_l = predicted;
        stats->fill_edges_added = vsdlss_graph_fill_edges(graph);
        stats->elimination_tree_height = 0;
        stats->separator_count = 0;
    }
    vsdlss_graph_free(graph);
    *q = order;
    return VSDLSS_OK;
}
