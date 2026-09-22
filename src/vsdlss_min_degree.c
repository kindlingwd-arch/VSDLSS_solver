#include "vsdlss_internal.h"

#include <limits.h>
#include <string.h>

static int csi_less(const void *a, const void *b)
{
    csi x = *(const csi *)a, y = *(const csi *)b;
    return (x > y) - (x < y);
}

/* Subgraph columns are visited in increasing global index, exactly as the
 * former full 0..n-1 scan did, so the subgraph and the order are unchanged;
 * only the O(n) scan and map setup per call are gone. */
vsdlss_status vsdlss_min_degree_subset_ws(const vsdlss *A, const csi *vertices,
                                          csi count, csi *order, csi *map_ws)
{
    csi *map = map_ws, *next = NULL, *local_q = NULL, *cols = NULL;
    vsdlss *sub = NULL;
    csi k, c, p, nz = count, set = 0;
    vsdlss_status status = VSDLSS_ERR_OOM;
    if (!A || !vertices || !order || count < 1 || count > A->n) return VSDLSS_ERR_INVALID;
    if (!map) {
        map = (csi *)malloc((size_t)A->n * sizeof(csi));
        if (!map) goto done;
        for (k = 0; k < A->n; ++k) map[k] = -1;
    }
    cols = (csi *)malloc((size_t)count * sizeof(csi));
    if (!cols) goto done;
    for (k = 0; k < count; ++k) {
        if (vertices[k] < 0 || vertices[k] >= A->n || map[vertices[k]] >= 0) {
            status = VSDLSS_ERR_INVALID;
            goto done;
        }
        map[vertices[k]] = k; cols[set++] = vertices[k];
    }
    qsort(cols, (size_t)count, sizeof(csi), csi_less);
    for (c = 0; c < count; ++c) {
        csi col = cols[c];
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
    for (c = 0; c < count; ++c) {
        csi col = cols[c];
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
    for (c = 0; c < count; ++c) {
        csi col = cols[c];
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
    if (map) for (k = 0; k < set; ++k) map[cols[k]] = -1;
    if (map != map_ws) free(map);
    free(cols); free(next); free(local_q); vsdlss_spfree(sub);
    return status;
}

vsdlss_status vsdlss_min_degree_subset(const vsdlss *A, const csi *vertices,
                                       csi count, csi *order)
{
    return vsdlss_min_degree_subset_ws(A, vertices, count, order, NULL);
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
    /* `eliminated` replaces a linear rescan of order[0..step-1] that ran for
       every candidate at every step, which made selection O(n^3).  `degree`
       caches vsdlss_graph_degree, which is O(deg(v)); only the neighbours of
       the vertex just eliminated can change degree, so the cache is refreshed
       for exactly those.  The selection rule is unchanged: smallest degree,
       ties to the smallest index, so the permutation is identical.
       Note self-loops never exist in this graph (graph_add_edge ignores
       a == b), so the old has_edge(v, v) guard was always true and the `used`
       rescan was the only filter being applied. */
    {
    unsigned char *eliminated = (unsigned char *)calloc((size_t)A->n, 1);
    csi *degree = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!eliminated || !degree) {
        free(eliminated); free(degree); free(order);
        vsdlss_graph_free(graph);
        return VSDLSS_ERR_OOM;
    }
    for (step = 0; step < A->n; ++step) degree[step] = vsdlss_graph_degree(graph, step);
    for (step = 0; step < A->n; ++step) {
        csi vertex, best = -1, best_degree = INT64_MAX;
        csi *neighbors = NULL, count = 0, k;
        for (vertex = 0; vertex < A->n; ++vertex)
            if (!eliminated[vertex] && degree[vertex] < best_degree) {
                best = vertex;
                best_degree = degree[vertex];
            }
        if (best < 0 || predicted > INT64_MAX - best_degree) {
            free(eliminated); free(degree); free(order);
            vsdlss_graph_free(graph);
            return best < 0 ? VSDLSS_ERR_INVALID : VSDLSS_ERR_OOM;
        }
        order[step] = best;
        eliminated[best] = 1;
        predicted += best_degree;
        status = vsdlss_graph_eliminate(graph, best, &neighbors, &count);
        if (status != VSDLSS_OK) {
            free(neighbors); free(eliminated); free(degree); free(order);
            vsdlss_graph_free(graph);
            return status;
        }
        degree[best] = 0;
        for (k = 0; k < count; ++k) degree[neighbors[k]] = vsdlss_graph_degree(graph, neighbors[k]);
        free(neighbors);
    }
    free(eliminated); free(degree);
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
