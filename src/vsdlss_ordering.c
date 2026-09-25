#include "vsdlss.h"
#include "vsdlss_internal.h"

#include <string.h>

#ifdef VSDLSS_METIS
/* Optional METIS nested dissection (order 6; build with -DVSDLSS_METIS and a
 * METIS 5 whose idx_t is 64-bit, the width of csi). */
#include "metis.h"
#include "vsdlss_parallel.h"
#if IDXTYPEWIDTH != 64
#error "VSDLSS_METIS needs METIS built with IDXTYPEWIDTH 64"
#endif
static vsdlss_status metis_order(const vsdlss *A, csi **q)
{
    const csi n = A->n;
    csi *deg = (csi *)calloc((size_t)n + 1, sizeof(csi));
    if (!deg) return VSDLSS_ERR_OOM;
    for (csi j = 0; j < n; ++j)
        for (csi p = A->p[j]; p < A->p[j + 1]; ++p)
            if (A->i[p] != j) { deg[A->i[p] + 1]++; deg[j + 1]++; }
    for (csi j = 0; j < n; ++j) deg[j + 1] += deg[j];
    idx_t *xadj = (idx_t *)malloc(((size_t)n + 1) * sizeof(idx_t));
    idx_t *adj = (idx_t *)malloc(((size_t)deg[n] ? (size_t)deg[n] : 1) * sizeof(idx_t));
    idx_t *perm = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t *iperm = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!xadj || !adj || !perm || !iperm) {
        free(deg); free(xadj); free(adj); free(perm); free(iperm); return VSDLSS_ERR_OOM;
    }
    for (csi j = 0; j <= n; ++j) xadj[j] = deg[j];
    for (csi j = 0; j < n; ++j)
        for (csi p = A->p[j]; p < A->p[j + 1]; ++p) {
            csi i = A->i[p];
            if (i != j) { adj[deg[i]++] = j; adj[deg[j]++] = i; }
        }
    free(deg);
    idx_t nv = n, options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    options[METIS_OPTION_NUMBERING] = 0;
    options[METIS_OPTION_SEED] = 1;           /* reproducible orderings */
    {   /* Tuning knobs for experiments (METIS defaults otherwise). */
        const char *e;
        if ((e = getenv("VSDLSS_METIS_NITER")) && atoi(e) > 0) options[METIS_OPTION_NITER] = atoi(e);
        if ((e = getenv("VSDLSS_METIS_CTYPE")) && *e) options[METIS_OPTION_CTYPE] = atoi(e);
        if ((e = getenv("VSDLSS_METIS_RTYPE")) && *e) options[METIS_OPTION_RTYPE] = atoi(e);
        /* separators tried per bisection (best kept), balance tolerance
         * (x1000), connected-component and compression handling */
        if ((e = getenv("VSDLSS_METIS_NSEPS")) && atoi(e) > 0) options[METIS_OPTION_NSEPS] = atoi(e);
        if ((e = getenv("VSDLSS_METIS_UFACTOR")) && atoi(e) > 0) options[METIS_OPTION_UFACTOR] = atoi(e);
        if ((e = getenv("VSDLSS_METIS_CCORDER")) && *e) options[METIS_OPTION_CCORDER] = atoi(e);
        if ((e = getenv("VSDLSS_METIS_COMPRESS")) && *e) options[METIS_OPTION_COMPRESS] = atoi(e);
        if ((e = getenv("VSDLSS_METIS_NO2HOP")) && *e) options[METIS_OPTION_NO2HOP] = atoi(e);
    }
    int rc;
#ifdef VSDLSS_METIS_THREADSAFE
    /* METIS built with thread-local GKlib random state: calls may overlap. */
    rc = METIS_NodeND(&nv, xadj, adj, NULL, options, perm, iperm);
#else
    /* Stock METIS 5.1 keeps its random-number state in GKlib globals, so
     * concurrent calls from different components race on it: orderings then
     * depend on thread timing and the state can even be indexed out of
     * range.  Serialize the calls; each reseeds, so results are the same for
     * every thread count. */
    VSDLSS_OMP(omp critical(vsdlss_metis))
    rc = METIS_NodeND(&nv, xadj, adj, NULL, options, perm, iperm);
#endif
    free(xadj); free(adj); free(iperm);
    if (rc != METIS_OK) { free(perm); return rc == METIS_ERROR_MEMORY ? VSDLSS_ERR_OOM : VSDLSS_ERR_INVALID; }
    /* METIS: row k of the permuted matrix is row perm[k], i.e. q[new] = old. */
    *q = (csi *)perm;
    return VSDLSS_OK;
}
#endif

#ifdef VSDLSS_KAHIP
/* Optional KaHIP nested dissection with data reduction (order 7; build with
 * -DVSDLSS_KAHIP and KaHIP's library, 32-bit kahip_idx).  Ost, Schulz,
 * Strash, "Engineering Data Reduction for Nested Dissection", ALENEX 2021.
 * VSDLSS_KAHIP_MODE: 0 fast, 1 eco (default), 2 strong. */
#include <stdbool.h>
#include <stdint.h>
#include "vsdlss_parallel.h"
void reduced_nd(int *n, int32_t *xadj, int32_t *adjncy, bool suppress_output,
                int seed, int mode, int *ordering);
static vsdlss_status kahip_order(const vsdlss *A, csi **q)
{
    const csi n = A->n;
    csi m = 0;
    for (csi j = 0; j < n; ++j)
        for (csi p = A->p[j]; p < A->p[j + 1]; ++p) if (A->i[p] != j) m += 2;
    if (n >= INT32_MAX || m >= INT32_MAX) return VSDLSS_ERR_UNSUPPORTED;
    int32_t *xadj = (int32_t *)calloc((size_t)n + 1, sizeof(int32_t));
    int32_t *adj = (int32_t *)malloc((size_t)(m ? m : 1) * sizeof(int32_t));
    int *ord = (int *)malloc((size_t)n * sizeof(int));
    csi *out = (csi *)malloc((size_t)n * sizeof(csi));
    if (!xadj || !adj || !ord || !out) { free(xadj); free(adj); free(ord); free(out); return VSDLSS_ERR_OOM; }
    for (csi j = 0; j < n; ++j)
        for (csi p = A->p[j]; p < A->p[j + 1]; ++p)
            if (A->i[p] != j) { xadj[A->i[p] + 1]++; xadj[j + 1]++; }
    for (csi j = 0; j < n; ++j) xadj[j + 1] += xadj[j];
    {
        int32_t *fill = (int32_t *)malloc((size_t)n * sizeof(int32_t));
        if (!fill) { free(xadj); free(adj); free(ord); free(out); return VSDLSS_ERR_OOM; }
        memcpy(fill, xadj, (size_t)n * sizeof(int32_t));
        for (csi j = 0; j < n; ++j)
            for (csi p = A->p[j]; p < A->p[j + 1]; ++p) {
                csi i = A->i[p];
                if (i != j) { adj[fill[i]++] = (int32_t)j; adj[fill[j]++] = (int32_t)i; }
            }
        free(fill);
    }
    const char *e = getenv("VSDLSS_KAHIP_MODE");
    int nn = (int)n, mode = e && *e ? atoi(e) : 1;
    /* KaHIP keeps its random state and output redirection in globals. */
    VSDLSS_OMP(omp critical(vsdlss_kahip))
    reduced_nd(&nn, xadj, adj, true, 1, mode, ord);
    free(xadj); free(adj);
    /* ord[v] is the position of vertex v: q[position] = v. */
    for (csi k = 0; k < n; ++k) out[k] = -1;
    for (csi v = 0; v < n; ++v) {
        if (ord[v] < 0 || ord[v] >= n || out[ord[v]] != -1) { free(ord); free(out); return VSDLSS_ERR_INVALID; }
        out[ord[v]] = v;
    }
    free(ord);
    *q = out;
    return VSDLSS_OK;
}
#endif

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
    else if (order == 0 || order == 5) {
        /* 0 = default = AMD since 2026-09-22 (fastest and least fill on the
         * power-grid cores measured); 4 still selects MLD explicitly. */
        status = vsdlss_amd_order(A, &local_q, &local_stats);
        if (status != VSDLSS_OK) return status;
    }
    else if (order == 4) {
        status = vsdlss_mld_order(A, &local_q, &local_stats);
        if (status != VSDLSS_OK) return status;
    }
#ifdef VSDLSS_METIS
    else if (order == 6) {
        status = metis_order(A, &local_q);
        if (status != VSDLSS_OK) return status;
    }
#endif
#ifdef VSDLSS_KAHIP
    else if (order == 7) {
        status = kahip_order(A, &local_q);
        if (status != VSDLSS_OK) return status;
    }
#endif
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
