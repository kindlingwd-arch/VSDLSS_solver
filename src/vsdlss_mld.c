#include "vsdlss_internal.h"

#include <limits.h>
#include <string.h>

typedef struct mld_context {
    const vsdlss *A;
    const vsdlss_graph *graph;
    csi *member, *distance, *queue;
    csi generation, separators;
} mld_context;

typedef struct mld_component {
    csi start, size, minimum;
} mld_component;

static vsdlss_status mark_subset(mld_context *ctx, const csi *v, csi n)
{
    csi k;
    if (ctx->generation == INT64_MAX) return VSDLSS_ERR_OOM;
    ctx->generation++;
    for (k = 0; k < n; ++k) {
        ctx->member[v[k]] = ctx->generation;
        ctx->distance[v[k]] = -1;
    }
    return VSDLSS_OK;
}

static vsdlss_status bfs(mld_context *ctx, csi start, csi *reached, csi *ecc)
{
    csi head = 0, tail = 0, maximum = 0;
    ctx->queue[tail++] = start;
    ctx->distance[start] = 0;
    while (head < tail) {
        csi v = ctx->queue[head++], p, count;
        const csi *neighbors;
        vsdlss_status status = vsdlss_graph_neighbors(ctx->graph, v, &neighbors, &count);
        if (status != VSDLSS_OK) return status;
        for (p = 0; p < count; ++p) {
            csi w = neighbors[p];
            if (ctx->member[w] == ctx->generation && ctx->distance[w] < 0) {
                ctx->distance[w] = ctx->distance[v] + 1;
                if (ctx->distance[w] > maximum) maximum = ctx->distance[w];
                ctx->queue[tail++] = w;
            }
        }
    }
    *reached = tail;
    *ecc = maximum;
    return VSDLSS_OK;
}

static csi farthest_low_degree(const mld_context *ctx, const csi *v, csi n, csi ecc)
{
    csi k, best = -1, degree_best = INT64_MAX;
    for (k = 0; k < n; ++k) if (ctx->distance[v[k]] == ecc) {
        const csi *neighbors;
        csi p, count, degree = 0;
        if (vsdlss_graph_neighbors(ctx->graph, v[k], &neighbors, &count) != VSDLSS_OK)
            return -1;
        for (p = 0; p < count; ++p)
            degree += ctx->member[neighbors[p]] == ctx->generation;
        if (degree < degree_best || (degree == degree_best && v[k] < best)) {
            best = v[k]; degree_best = degree;
        }
    }
    return best;
}

static vsdlss_status pseudo_peripheral_bfs(mld_context *ctx, const csi *v,
                                           csi n, csi *ecc)
{
    csi k, root = v[0], reached, previous = -1;
    vsdlss_status status;
    for (k = 1; k < n; ++k) if (v[k] < root) root = v[k];
    for (;;) {
        for (k = 0; k < n; ++k) ctx->distance[v[k]] = -1;
        status = bfs(ctx, root, &reached, ecc);
        if (status != VSDLSS_OK) return status;
        if (reached != n) return VSDLSS_ERR_INVALID;
        if (*ecc <= previous) return VSDLSS_OK;
        previous = *ecc;
        root = farthest_low_degree(ctx, v, n, *ecc);
        if (root < 0) return VSDLSS_ERR_INVALID;
    }
}

static int unbalanced(csi left, csi right)
{
    csi total = left + right;
    csi limit = total - total / 8 - (total % 8 != 0);
    return left == 0 || right == 0 || left > limit || right > limit;
}

static int component_after(const void *a, const void *b)
{
    const mld_component *ca = (const mld_component *)a;
    const mld_component *cb = (const mld_component *)b;
    return (ca->minimum > cb->minimum) - (ca->minimum < cb->minimum);
}

static vsdlss_status balanced_fallback(mld_context *ctx, const csi *v, csi n,
                                       signed char *side, csi *left,
                                       csi *right, csi *separator)
{
    csi k;
    *left = n / 2; *right = n - *left; *separator = 0;
    for (k = 0; k < n; ++k) side[ctx->queue[k]] = k < *left ? 0 : 1;
    for (;;) {
        csi a = -1, b = -1;
        for (k = 0; k < n && a < 0; ++k) {
            csi node = v[k], p, degree;
            const csi *neighbors;
            vsdlss_status status;
            if (side[node] == 2) continue;
            status = vsdlss_graph_neighbors(ctx->graph, node, &neighbors, &degree);
            if (status != VSDLSS_OK) return status;
            for (p = 0; p < degree; ++p) {
                csi other = neighbors[p];
                if (ctx->member[other] == ctx->generation && side[other] != 2 &&
                    side[node] != side[other]) { a = node; b = other; break; }
            }
        }
        if (a < 0) return VSDLSS_OK;
        {
            csi move;
            if (*left > *right) move = side[a] == 0 ? a : b;
            else if (*right > *left) move = side[a] == 1 ? a : b;
            else move = a > b ? a : b;
            if (side[move] == 0) (*left)--; else (*right)--;
            side[move] = 2; (*separator)++;
        }
    }
}

static vsdlss_status partition_connected(mld_context *ctx, const csi *v, csi n,
                                         signed char *side, csi *left,
                                         csi *right, csi *separator)
{
    csi k, ecc, middle;
    vsdlss_status status = mark_subset(ctx, v, n);
    if (status != VSDLSS_OK) return status;
    status = pseudo_peripheral_bfs(ctx, v, n, &ecc);
    if (status != VSDLSS_OK) return status;
    middle = ecc / 2;
    *left = *right = *separator = 0;
    for (k = 0; k < n; ++k) {
        csi node = v[k];
        if (ctx->distance[node] < middle) { side[node] = 0; (*left)++; }
        else if (ctx->distance[node] > middle) { side[node] = 1; (*right)++; }
        else { side[node] = 2; (*separator)++; }
    }
    return unbalanced(*left, *right)
        ? balanced_fallback(ctx, v, n, side, left, right, separator) : VSDLSS_OK;
}

static vsdlss_status order_set(mld_context *, const csi *, csi, csi *, csi *);

static vsdlss_status order_connected(mld_context *ctx, const csi *v, csi n,
                                     csi *out, csi *written)
{
    signed char *side = NULL;
    csi *lv = NULL, *rv = NULL, *sv = NULL;
    csi left, right, separator, li = 0, ri = 0, si = 0, k;
    vsdlss_status status;
    if (n <= 32) {
        status = vsdlss_min_degree_subset(ctx->A, v, n, out);
        if (status == VSDLSS_OK) *written = n;
        return status;
    }
    side = (signed char *)malloc((size_t)ctx->A->n);
    lv = (csi *)malloc((size_t)n * sizeof(csi));
    rv = (csi *)malloc((size_t)n * sizeof(csi));
    sv = (csi *)malloc((size_t)n * sizeof(csi));
    if (!side || !lv || !rv || !sv) { status = VSDLSS_ERR_OOM; goto done; }
    status = partition_connected(ctx, v, n, side, &left, &right, &separator);
    if (status != VSDLSS_OK) goto done;
    for (k = 0; k < n; ++k) {
        if (side[v[k]] == 0) lv[li++] = v[k];
        else if (side[v[k]] == 1) rv[ri++] = v[k];
        else sv[si++] = v[k];
    }
    if (li != left || ri != right || si != separator || separator == 0) {
        status = VSDLSS_ERR_INVALID; goto done;
    }
    if (ctx->separators > INT64_MAX - separator) { status = VSDLSS_ERR_OOM; goto done; }
    ctx->separators += separator;
    *written = 0;
    if (left) {
        csi emitted;
        status = order_set(ctx, lv, left, out + *written, &emitted);
        if (status != VSDLSS_OK) goto done;
        *written += emitted;
    }
    if (right) {
        csi emitted;
        status = order_set(ctx, rv, right, out + *written, &emitted);
        if (status != VSDLSS_OK) goto done;
        *written += emitted;
    }
    status = vsdlss_min_degree_subset(ctx->A, sv, separator, out + *written);
    if (status == VSDLSS_OK) *written += separator;
done:
    free(side); free(lv); free(rv); free(sv);
    return status;
}

/* Discover every component first, then order each; this avoids recursive peeling. */
static vsdlss_status order_set(mld_context *ctx, const csi *v, csi n,
                               csi *out, csi *written)
{
    csi *grouped = NULL;
    mld_component *component = NULL;
    csi k, components = 0, used = 0;
    vsdlss_status status = mark_subset(ctx, v, n);
    if (status != VSDLSS_OK) return status;
    grouped = (csi *)malloc((size_t)n * sizeof(csi));
    component = (mld_component *)malloc((size_t)n * sizeof(*component));
    if (!grouped || !component) { status = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < n; ++k) if (ctx->distance[v[k]] < 0) {
        csi reached, ecc, j, minimum = v[k], start = used;
        status = bfs(ctx, v[k], &reached, &ecc);
        if (status != VSDLSS_OK) goto done;
        for (j = 0; j < reached; ++j) {
            grouped[used++] = ctx->queue[j];
            if (ctx->queue[j] < minimum) minimum = ctx->queue[j];
        }
        component[components].start = start;
        component[components].size = reached;
        component[components].minimum = minimum;
        components++;
    }
    if (used != n) { status = VSDLSS_ERR_INVALID; goto done; }
    qsort(component, (size_t)components, sizeof(*component), component_after);
    *written = 0;
    for (k = 0; k < components; ++k) {
        csi emitted;
        status = order_connected(ctx, grouped + component[k].start, component[k].size,
                                 out + *written, &emitted);
        if (status != VSDLSS_OK) goto done;
        *written += emitted;
    }
done:
    free(grouped); free(component);
    return status;
}

static vsdlss_status context_init(const vsdlss *A, mld_context *ctx,
                                  vsdlss_graph **graph)
{
    vsdlss_status status;
    memset(ctx, 0, sizeof(*ctx));
    status = vsdlss_graph_build(A, graph);
    if (status != VSDLSS_OK) return status;
    ctx->A = A; ctx->graph = *graph;
    ctx->member = (csi *)calloc((size_t)A->n, sizeof(csi));
    ctx->distance = (csi *)malloc((size_t)A->n * sizeof(csi));
    ctx->queue = (csi *)malloc((size_t)A->n * sizeof(csi));
    return (!ctx->member || !ctx->distance || !ctx->queue) ? VSDLSS_ERR_OOM : VSDLSS_OK;
}

static void context_finish(mld_context *ctx, vsdlss_graph *graph)
{
    free(ctx->member); free(ctx->distance); free(ctx->queue);
    vsdlss_graph_free(graph);
}

vsdlss_status vsdlss_mld_top_partition(const vsdlss *A, signed char *side,
                                        csi *left, csi *right, csi *separator)
{
    mld_context ctx;
    vsdlss_graph *graph = NULL;
    csi *v = NULL, k;
    vsdlss_status status;
    if (!A || !side || !left || !right || !separator) return VSDLSS_ERR_INVALID;
    status = context_init(A, &ctx, &graph);
    if (status != VSDLSS_OK) { context_finish(&ctx, graph); return status; }
    v = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!v) { context_finish(&ctx, graph); return VSDLSS_ERR_OOM; }
    for (k = 0; k < A->n; ++k) v[k] = k;
    status = partition_connected(&ctx, v, A->n, side, left, right, separator);
    free(v); context_finish(&ctx, graph);
    return status;
}

vsdlss_status vsdlss_mld_order(const vsdlss *A, csi **q,
                               vsdlss_order_stats *stats)
{
    mld_context ctx;
    vsdlss_graph *graph = NULL;
    csi *v = NULL, *order = NULL, k, written = 0;
    vsdlss_status status;
    if (!q) return VSDLSS_ERR_INVALID;
    *q = NULL;
    status = context_init(A, &ctx, &graph);
    if (status != VSDLSS_OK) { context_finish(&ctx, graph); return status; }
    v = (csi *)malloc((size_t)A->n * sizeof(csi));
    order = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!v || !order) { status = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < A->n; ++k) v[k] = k;
    status = order_set(&ctx, v, A->n, order, &written);
    if (status != VSDLSS_OK) goto done;
    if (written != A->n) { status = VSDLSS_ERR_INVALID; goto done; }
    if (stats) stats->separator_count = ctx.separators;
    *q = order; order = NULL;
done:
    free(v); free(order); context_finish(&ctx, graph);
    return status;
}
