#include "vsdlss_internal.h"

#include <limits.h>
#include <string.h>

typedef struct vsdlss_neighbors {
    csi *items;
    csi count;
    csi capacity;
} vsdlss_neighbors;

struct vsdlss_graph {
    csi n;
    vsdlss_neighbors *adj;
    unsigned char *active;
    csi fill_edges;
};

static csi neighbor_lower_bound(const vsdlss_neighbors *list, csi value)
{
    csi lo = 0, hi = list->count;
    while (lo < hi) {
        csi mid = lo + (hi - lo) / 2;
        if (list->items[mid] < value) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

static int neighbor_contains(const vsdlss_neighbors *list, csi value)
{
    csi at = neighbor_lower_bound(list, value);
    return at < list->count && list->items[at] == value;
}

static vsdlss_status neighbor_reserve(vsdlss_neighbors *list, csi needed)
{
    csi capacity;
    csi *items;
    if (needed <= list->capacity) return VSDLSS_OK;
    capacity = list->capacity ? list->capacity : 4;
    while (capacity < needed) {
        if (capacity > INT64_MAX / 2) return VSDLSS_ERR_OOM;
        capacity *= 2;
    }
    if ((uint64_t)capacity > SIZE_MAX / sizeof(csi)) return VSDLSS_ERR_OOM;
    items = (csi *)realloc(list->items, (size_t)capacity * sizeof(csi));
    if (!items) return VSDLSS_ERR_OOM;
    list->items = items;
    list->capacity = capacity;
    return VSDLSS_OK;
}

static vsdlss_status neighbor_insert(vsdlss_neighbors *list, csi value)
{
    csi at = neighbor_lower_bound(list, value);
    vsdlss_status status;
    if (at < list->count && list->items[at] == value) return VSDLSS_OK;
    if (list->count == INT64_MAX) return VSDLSS_ERR_OOM;
    status = neighbor_reserve(list, list->count + 1);
    if (status != VSDLSS_OK) return status;
    memmove(list->items + at + 1, list->items + at,
            (size_t)(list->count - at) * sizeof(csi));
    list->items[at] = value;
    list->count++;
    return VSDLSS_OK;
}

static vsdlss_status graph_add_edge(vsdlss_graph *graph, csi a, csi b)
{
    vsdlss_status status;
    int has_ab, has_ba;
    if (a == b) return VSDLSS_OK;
    has_ab = neighbor_contains(&graph->adj[a], b);
    has_ba = neighbor_contains(&graph->adj[b], a);
    if (has_ab && has_ba) return VSDLSS_OK;
    if (!has_ab) {
        if (graph->adj[a].count == INT64_MAX) return VSDLSS_ERR_OOM;
        status = neighbor_reserve(&graph->adj[a], graph->adj[a].count + 1);
        if (status != VSDLSS_OK) return status;
    }
    if (!has_ba) {
        if (graph->adj[b].count == INT64_MAX) return VSDLSS_ERR_OOM;
        status = neighbor_reserve(&graph->adj[b], graph->adj[b].count + 1);
        if (status != VSDLSS_OK) return status;
    }
    status = neighbor_insert(&graph->adj[a], b);
    if (status != VSDLSS_OK) return status;
    return neighbor_insert(&graph->adj[b], a);
}

void vsdlss_graph_free(vsdlss_graph *graph)
{
    csi v;
    if (!graph) return;
    for (v = 0; v < graph->n; ++v) free(graph->adj[v].items);
    free(graph->adj);
    free(graph->active);
    free(graph);
}

vsdlss_status vsdlss_graph_build(const vsdlss *A, vsdlss_graph **out)
{
    vsdlss_graph *graph;
    vsdlss_status status;
    csi col, p, row;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    if ((uint64_t)A->n > SIZE_MAX / sizeof(vsdlss_neighbors)) return VSDLSS_ERR_OOM;
    graph = (vsdlss_graph *)calloc(1, sizeof(*graph));
    if (!graph) return VSDLSS_ERR_OOM;
    graph->n = A->n;
    graph->adj = (vsdlss_neighbors *)calloc((size_t)A->n, sizeof(*graph->adj));
    graph->active = (unsigned char *)malloc((size_t)A->n);
    if (!graph->adj || !graph->active) {
        vsdlss_graph_free(graph);
        return VSDLSS_ERR_OOM;
    }
    memset(graph->active, 1, (size_t)A->n);
    for (col = 0; col < A->n; ++col) {
        for (p = A->p[col]; p < A->p[col + 1]; ++p) {
            row = A->i[p];
            if (row == col) continue;
            status = graph_add_edge(graph, row, col);
            if (status != VSDLSS_OK) {
                vsdlss_graph_free(graph);
                return status;
            }
        }
    }
    *out = graph;
    return VSDLSS_OK;
}

int vsdlss_graph_has_edge(const vsdlss_graph *graph, csi a, csi b)
{
    if (!graph || a < 0 || b < 0 || a >= graph->n || b >= graph->n) return 0;
    return neighbor_contains(&graph->adj[a], b);
}

csi vsdlss_graph_degree(const vsdlss_graph *graph, csi vertex)
{
    csi p, degree = 0;
    if (!graph || vertex < 0 || vertex >= graph->n || !graph->active[vertex]) return 0;
    for (p = 0; p < graph->adj[vertex].count; ++p)
        if (graph->active[graph->adj[vertex].items[p]]) degree++;
    return degree;
}

vsdlss_status vsdlss_graph_eliminate(vsdlss_graph *graph, csi vertex,
                                     csi **neighbors, csi *count)
{
    csi degree, p, at = 0, a, b;
    csi *active_neighbors;
    vsdlss_status status;
    if (!graph || !neighbors || !count || vertex < 0 || vertex >= graph->n ||
        !graph->active[vertex]) return VSDLSS_ERR_INVALID;
    *neighbors = NULL;
    *count = 0;
    degree = vsdlss_graph_degree(graph, vertex);
    if ((uint64_t)(degree > 0 ? degree : 1) > SIZE_MAX / sizeof(csi))
        return VSDLSS_ERR_OOM;
    active_neighbors = (csi *)malloc((size_t)(degree > 0 ? degree : 1) * sizeof(csi));
    if (!active_neighbors) return VSDLSS_ERR_OOM;
    for (p = 0; p < graph->adj[vertex].count; ++p) {
        csi neighbor = graph->adj[vertex].items[p];
        if (graph->active[neighbor]) active_neighbors[at++] = neighbor;
    }
    for (a = 0; a < degree; ++a) {
        for (b = a + 1; b < degree; ++b) {
            if (!vsdlss_graph_has_edge(graph, active_neighbors[a], active_neighbors[b])) {
                if (graph->fill_edges == INT64_MAX) { free(active_neighbors); return VSDLSS_ERR_OOM; }
                status = graph_add_edge(graph, active_neighbors[a], active_neighbors[b]);
                if (status != VSDLSS_OK) { free(active_neighbors); return status; }
                graph->fill_edges++;
            }
        }
    }
    graph->active[vertex] = 0;
    *neighbors = active_neighbors;
    *count = degree;
    return VSDLSS_OK;
}

csi vsdlss_graph_fill_edges(const vsdlss_graph *graph)
{
    return graph ? graph->fill_edges : 0;
}

vsdlss_status vsdlss_graph_neighbors(const vsdlss_graph *graph, csi vertex,
                                     const csi **neighbors, csi *count)
{
    if (!graph || !neighbors || !count || vertex < 0 || vertex >= graph->n)
        return VSDLSS_ERR_INVALID;
    *neighbors = graph->adj[vertex].items;
    *count = graph->adj[vertex].count;
    return VSDLSS_OK;
}
