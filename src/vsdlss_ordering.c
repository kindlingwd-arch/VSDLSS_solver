#include "vsdlss.h"
#include "vsdlss_internal.h"

#include <string.h>

vsdlss_status vsdlss_validate_permutation(const csi *q, const csi *pinv, csi n)
{
    unsigned char *seen;
    csi k;
    if (!q || !pinv || n < 1 || (uint64_t)n > SIZE_MAX) return VSDLSS_ERR_INVALID;
    seen = (unsigned char *)calloc((size_t)n, 1);
    if (!seen) return VSDLSS_ERR_OOM;
    for (k = 0; k < n; ++k) {
        if (q[k] < 0 || q[k] >= n || seen[q[k]] || pinv[q[k]] != k) {
            free(seen);
            return VSDLSS_ERR_INVALID;
        }
        seen[q[k]] = 1;
    }
    free(seen);
    return VSDLSS_OK;
}

static csi *vsdlss_identity_perm(csi n)
{
    csi *q, k;
    if (n < 1 || (uint64_t)n > SIZE_MAX / sizeof(csi)) return NULL;
    q = (csi *)malloc((size_t)n * sizeof(csi));
    if (!q) return NULL;
    for (k = 0; k < n; ++k) q[k] = k;
    return q;
}

static int neighbor_after(csi a, csi b, const csi *degree)
{
    return degree[a] > degree[b] || (degree[a] == degree[b] && a > b);
}

static csi *vsdlss_rcm(const csi *Ap, const csi *Ai, csi n)
{
    csi *degree = NULL, *adjp = NULL, *adj = NULL, *next = NULL;
    csi *queue = NULL, *neighbors = NULL, *q = NULL;
    unsigned char *visited = NULL;
    csi j, p, row, edges, out = 0;
    if (!Ap || !Ai || n < 1 || (uint64_t)n > SIZE_MAX / sizeof(csi)) return NULL;
    degree = (csi *)calloc((size_t)n, sizeof(csi));
    adjp = (csi *)malloc(((size_t)n + 1) * sizeof(csi));
    queue = (csi *)malloc((size_t)n * sizeof(csi));
    neighbors = (csi *)malloc((size_t)n * sizeof(csi));
    q = (csi *)malloc((size_t)n * sizeof(csi));
    visited = (unsigned char *)calloc((size_t)n, 1);
    if (!degree || !adjp || !queue || !neighbors || !q || !visited) goto oom;
    for (j = 0; j < n; ++j) {
        for (p = Ap[j]; p < Ap[j + 1]; ++p) {
            row = Ai[p];
            if (row < j) { ++degree[row]; ++degree[j]; }
        }
    }
    adjp[0] = 0;
    for (j = 0; j < n; ++j) {
        if (degree[j] > INT64_MAX - adjp[j]) goto oom;
        adjp[j + 1] = adjp[j] + degree[j];
    }
    edges = adjp[n];
    if ((uint64_t)(edges > 0 ? edges : 1) > SIZE_MAX / sizeof(csi)) goto oom;
    adj = (csi *)malloc((size_t)(edges > 0 ? edges : 1) * sizeof(csi));
    next = (csi *)malloc((size_t)n * sizeof(csi));
    if (!adj || !next) goto oom;
    memcpy(next, adjp, (size_t)n * sizeof(csi));
    for (j = 0; j < n; ++j) {
        for (p = Ap[j]; p < Ap[j + 1]; ++p) {
            row = Ai[p];
            if (row < j) { adj[next[row]++] = j; adj[next[j]++] = row; }
        }
    }
    while (out < n) {
        csi start = -1, head = 0, tail = 0, component_start = out, a, b;
        for (j = 0; j < n; ++j)
            if (!visited[j] && (start < 0 || degree[j] < degree[start] ||
                                (degree[j] == degree[start] && j < start))) start = j;
        if (start < 0) goto oom;
        queue[tail++] = start;
        visited[start] = 1;
        while (head < tail) {
            csi node = queue[head++], count = 0;
            q[out++] = node;
            for (p = adjp[node]; p < adjp[node + 1]; ++p) {
                csi candidate = adj[p];
                if (!visited[candidate]) {
                    visited[candidate] = 1;
                    neighbors[count++] = candidate;
                }
            }
            for (a = 1; a < count; ++a) {
                csi value = neighbors[a];
                b = a;
                while (b > 0 && neighbor_after(neighbors[b - 1], value, degree)) {
                    neighbors[b] = neighbors[b - 1];
                    --b;
                }
                neighbors[b] = value;
            }
            for (a = 0; a < count; ++a) queue[tail++] = neighbors[a];
        }
        for (a = component_start, b = out - 1; a < b; ++a, --b) {
            csi tmp = q[a]; q[a] = q[b]; q[b] = tmp;
        }
    }
    free(degree); free(adjp); free(adj); free(next); free(queue);
    free(neighbors); free(visited);
    return q;
oom:
    free(degree); free(adjp); free(adj); free(next); free(queue);
    free(neighbors); free(q); free(visited);
    return NULL;
}

static vsdlss_status analyze_elimination(const vsdlss *A, const csi *q,
                                         vsdlss_order_stats *stats)
{
    vsdlss_graph *graph = NULL;
    vsdlss_status status;
    csi k, predicted = A->n;
    status = vsdlss_graph_build(A, &graph);
    if (status != VSDLSS_OK) return status;
    for (k = 0; k < A->n; ++k) {
        csi degree = vsdlss_graph_degree(graph, q[k]);
        csi *neighbors = NULL, count = 0;
        if (predicted > INT64_MAX - degree) {
            vsdlss_graph_free(graph);
            return VSDLSS_ERR_OOM;
        }
        predicted += degree;
        status = vsdlss_graph_eliminate(graph, q[k], &neighbors, &count);
        free(neighbors);
        if (status != VSDLSS_OK) { vsdlss_graph_free(graph); return status; }
    }
    stats->predicted_nnz_l = predicted;
    stats->fill_edges_added = vsdlss_graph_fill_edges(graph);
    vsdlss_graph_free(graph);
    return VSDLSS_OK;
}

vsdlss_status vsdlss_order_analyze(const vsdlss *A, int order,
                                   csi **q, csi **pinv,
                                   vsdlss_order_stats *stats)
{
    csi *local_q = NULL, *local_pinv = NULL, *parent = NULL, k;
    vsdlss *permuted = NULL;
    vsdlss_order_stats local_stats = {0, 0, 0, 0};
    vsdlss_status status;
    if (!q || !pinv) return VSDLSS_ERR_INVALID;
    *q = NULL; *pinv = NULL;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    if (order == 1) local_q = vsdlss_rcm(A->p, A->i, A->n);
    else if (order == 2) local_q = vsdlss_identity_perm(A->n);
    else if (order == 3) {
        status = vsdlss_min_degree_order(A, &local_q, &local_stats);
        if (status != VSDLSS_OK) return status;
    }
    else if (order == 0 || order == 4) {
        status = vsdlss_mld_order(A, &local_q, &local_stats);
        if (status != VSDLSS_OK) return status;
    }
    else return VSDLSS_ERR_UNSUPPORTED;
    if (!local_q) return VSDLSS_ERR_OOM;
    local_pinv = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!local_pinv) { free(local_q); return VSDLSS_ERR_OOM; }
    for (k = 0; k < A->n; ++k) local_pinv[local_q[k]] = k;
    status = vsdlss_validate_permutation(local_q, local_pinv, A->n);
    if (status != VSDLSS_OK) { free(local_q); free(local_pinv); return status; }
    if (!stats) {
        *q = local_q;
        *pinv = local_pinv;
        return VSDLSS_OK;
    }
    status = analyze_elimination(A, local_q, &local_stats);
    if (status != VSDLSS_OK) { free(local_q); free(local_pinv); return status; }
    permuted = vsdlss_symperm(A, local_pinv, 0);
    if (!permuted) { free(local_q); free(local_pinv); return VSDLSS_ERR_OOM; }
    parent = vsdlss_etree(permuted, 0);
    vsdlss_spfree(permuted);
    if (!parent) { free(local_q); free(local_pinv); return VSDLSS_ERR_OOM; }
    for (k = 0; k < A->n; ++k) {
        csi node = k, depth = 1;
        while (parent[node] >= 0) {
            node = parent[node];
            if (depth == INT64_MAX || depth > A->n) {
                free(parent); free(local_q); free(local_pinv);
                return VSDLSS_ERR_INVALID;
            }
            depth++;
        }
        if (depth > local_stats.elimination_tree_height)
            local_stats.elimination_tree_height = depth;
    }
    free(parent);
    *q = local_q; *pinv = local_pinv;
    if (stats) *stats = local_stats;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_order(const vsdlss *A, int order, csi **q, csi **pinv)
{
    return vsdlss_order_analyze(A, order, q, pinv, NULL);
}
