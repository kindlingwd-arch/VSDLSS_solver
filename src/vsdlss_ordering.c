#include "vsdlss.h"

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

vsdlss_status vsdlss_order(const vsdlss *A, int order, csi **q, csi **pinv)
{
    csi *local_q = NULL, *local_pinv = NULL, k;
    vsdlss_status status;
    if (!q || !pinv) return VSDLSS_ERR_INVALID;
    *q = NULL; *pinv = NULL;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    if (order == 0 || order == 1) local_q = vsdlss_rcm(A->p, A->i, A->n);
    else if (order == 2) local_q = vsdlss_identity_perm(A->n);
    else return VSDLSS_ERR_UNSUPPORTED;
    if (!local_q) return VSDLSS_ERR_OOM;
    local_pinv = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!local_pinv) { free(local_q); return VSDLSS_ERR_OOM; }
    for (k = 0; k < A->n; ++k) local_pinv[local_q[k]] = k;
    status = vsdlss_validate_permutation(local_q, local_pinv, A->n);
    if (status != VSDLSS_OK) { free(local_q); free(local_pinv); return status; }
    *q = local_q; *pinv = local_pinv;
    return VSDLSS_OK;
}
