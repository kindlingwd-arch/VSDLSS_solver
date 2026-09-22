#include "vsdlss_m3_internal.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

static int checked_count(csi n, size_t width)
{
    return n >= 0 && (uint64_t)n <= SIZE_MAX / width;
}

void vsdlss_components_free(vsdlss_components *components)
{
    if (!components) return;
    free(components->offset);
    free(components->vertices);
    free(components->component_of);
    free(components->local_of);
    free(components->order);
    free(components);
}

void vsdlss_wgraph_free(vsdlss_wgraph *g)
{
    if (!g) return;
    free(g->ptr); free(g->idx); free(g->val); free(g->diag); free(g);
}

static vsdlss_status components_build_impl(const vsdlss *A,
                                           vsdlss_components **out,
                                           int validate, vsdlss_wgraph **graph)
{
    vsdlss_components *components = NULL;
    csi *queue = NULL, *sizes = NULL, *adj_offset = NULL;
    csi *adj_cursor = NULL, *adjacent = NULL;
    double *adjval = NULL, *diag = NULL;
    csi seed, head, tail, component, position, adjacency_n = 0;
    vsdlss_status status;

    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (graph) *graph = NULL;
    if (!A || A->n < 1)
        return VSDLSS_ERR_INVALID;
    if (A->n == INT64_MAX || !checked_count(A->n, sizeof(csi)) ||
        !checked_count(A->n + 1, sizeof(csi))) return VSDLSS_ERR_OOM;
    if (validate) {
        status = vsdlss_validate_upper_csc(A);
        if (status != VSDLSS_OK) return status;
    }

    components = (vsdlss_components *)calloc(1, sizeof(*components));
    if (!components) return VSDLSS_ERR_OOM;
    components->n = A->n;
    components->vertices = (csi *)malloc((size_t)A->n * sizeof(csi));
    components->component_of = (csi *)malloc((size_t)A->n * sizeof(csi));
    components->local_of = (csi *)malloc((size_t)A->n * sizeof(csi));
    components->order = (csi *)malloc((size_t)A->n * sizeof(csi));
    /* Per-component sizes grow with the component count (usually tiny);
     * the fill cursor borrows `vertices`, which is only written at the end. */
    csi sizes_cap = 16;
    sizes = (csi *)malloc((size_t)sizes_cap * sizeof(csi));
    adj_offset = (csi *)calloc((size_t)(A->n + 1), sizeof(csi));
    adj_cursor = components->vertices;
    if (!components->vertices || !components->component_of ||
        !components->local_of || !components->order || !sizes || !adj_offset) {
        status = VSDLSS_ERR_OOM;
        goto fail;
    }
    /* Degrees: the lower part of vertex c is its own column, the upper part
     * is scattered from later columns.  Kept serial: with atomics the random
     * scatter was slower on two threads than on one (cache-line traffic). */
    {
        for (csi c = 0; c < A->n; ++c) {
            csi low = 0;
            for (csi q = A->p[c]; q < A->p[c + 1]; ++q) {
                csi row = A->i[q];
                if (row == c) continue;
                ++low; ++adj_offset[row + 1];
            }
            adj_offset[c + 1] += low; adj_cursor[c] = low; adjacency_n += 2 * low;
        }
    }
    for (seed = 0; seed < A->n; ++seed) {
        if (adj_offset[seed + 1] > INT64_MAX - adj_offset[seed]) {
            status = VSDLSS_ERR_OOM; goto fail;
        }
        adj_offset[seed + 1] += adj_offset[seed];
    }
    if (!checked_count(adjacency_n, sizeof(csi))) { status = VSDLSS_ERR_OOM; goto fail; }
    if (adjacency_n > 0) {
        adjacent = (csi *)malloc((size_t)adjacency_n * sizeof(csi));
        if (!adjacent) { status = VSDLSS_ERR_OOM; goto fail; }
    }
    if (graph) {
        adjval = (double *)malloc((size_t)(adjacency_n > 0 ? adjacency_n : 1) * sizeof(double));
        diag = (double *)calloc((size_t)A->n, sizeof(double));
        if (!adjval || !diag) { status = VSDLSS_ERR_OOM; goto fail; }
    }
    /* Each list is [neighbours < v (column v, in order)] [neighbours > v in
     * increasing column order], i.e. ascending for normalized input. */
    for (seed = 0; seed < A->n; ++seed) adj_cursor[seed] = adj_offset[seed] + adj_cursor[seed];
    {
        const csi *Ap = A->p, *Ai = A->i; const double *Ax = A->x;
        for (csi c = 0; c < A->n; ++c) {
            csi lo = adj_offset[c];
            for (csi q = Ap[c]; q < Ap[c + 1]; ++q) {
                csi row = Ai[q], at;
                if (row == c) { if (diag) diag[c] += Ax[q]; continue; }
                at = adj_cursor[row]++;
                adjacent[lo] = row; adjacent[at] = c;
                if (adjval) { adjval[lo] = Ax[q]; adjval[at] = Ax[q]; }
                ++lo;
            }
        }
    }

    for (seed = 0; seed < A->n; ++seed) components->component_of[seed] = -1;

    component = 0;
    {
    csi discovered = 0;   /* BFS queues of all components, concatenated */
    for (seed = 0; seed < A->n; ++seed) {
        if (components->component_of[seed] >= 0) continue;
        /* The BFS queue is written straight into `order`. */
        if (component == sizes_cap) {
            csi *grown;
            if (sizes_cap > INT64_MAX / 2 || !checked_count(2 * sizes_cap, sizeof(csi))) { status = VSDLSS_ERR_OOM; goto fail; }
            grown = (csi *)realloc(sizes, (size_t)(2 * sizes_cap) * sizeof(csi));
            if (!grown) { status = VSDLSS_ERR_OOM; goto fail; }
            sizes = grown; sizes_cap *= 2;
        }
        queue = components->order + discovered;
        head = 0; tail = 1; queue[0] = seed;
        components->component_of[seed] = component;
        while (head < tail) {
            csi v = queue[head++];
            csi at;
            for (at = adj_offset[v]; at < adj_offset[v + 1]; ++at) {
                csi neighbor = adjacent[at];
                if (components->component_of[neighbor] < 0) {
                    components->component_of[neighbor] = component;
                    queue[tail++] = neighbor;
                }
            }
        }
        discovered += tail;
        sizes[component] = tail;
        ++component;
    }
    }
    components->count = component;
    components->offset = (csi *)malloc((size_t)(component + 1) * sizeof(csi));
    if (!components->offset) { status = VSDLSS_ERR_OOM; goto fail; }
    components->offset[0] = 0;
    for (component = 0; component < components->count; ++component)
        components->offset[component + 1] = components->offset[component] + sizes[component];
    for (component = 0; component < components->count; ++component)
        sizes[component] = 0;
    for (seed = 0; seed < A->n; ++seed) {
        component = components->component_of[seed];
        position = sizes[component]++;
        components->local_of[seed] = position;
        components->vertices[components->offset[component] + position] = seed;
    }
    /* Components were discovered in seed order, so the concatenated queues
     * already follow `offset`; convert global vertices to local indices. */
    for (seed = 0; seed < A->n; ++seed)
        components->order[seed] = components->local_of[components->order[seed]];
    if (graph) {
        vsdlss_wgraph *g = (vsdlss_wgraph *)calloc(1, sizeof(*g));
        if (!g) { status = VSDLSS_ERR_OOM; goto fail; }
        g->n = A->n; g->ptr = adj_offset; g->idx = adjacent; g->val = adjval; g->diag = diag;
        if (!g->idx) { g->idx = (csi *)malloc(sizeof(csi)); if (!g->idx) { free(g); status = VSDLSS_ERR_OOM; goto fail; } }
        adj_offset = NULL; adjacent = NULL; adjval = NULL; diag = NULL;
        *graph = g;
    }
    free(sizes); free(adj_offset); free(adjacent); free(adjval); free(diag);
    *out = components;
    return VSDLSS_OK;

fail:
    free(sizes); free(adj_offset); free(adjacent); free(adjval); free(diag);
    vsdlss_components_free(components);
    return status;
}

vsdlss_status vsdlss_components_build(const vsdlss *A,
                                      vsdlss_components **out)
{
    return components_build_impl(A, out, 1, NULL);
}

vsdlss_status vsdlss_components_build_normalized(const vsdlss *A,
                                                 vsdlss_components **out)
{
    return components_build_impl(A, out, 0, NULL);
}

vsdlss_status vsdlss_components_build_graph(const vsdlss *A, vsdlss_components **out,
                                            vsdlss_wgraph **graph)
{
    if (!graph) return VSDLSS_ERR_INVALID;
    return components_build_impl(A, out, 0, graph);
}

static vsdlss_status component_extract_impl(const vsdlss *A,
                                            const vsdlss_components *components,
                                            csi component, vsdlss **out,
                                            int validate)
{
    vsdlss *local = NULL;
    vsdlss_status status;
    csi start, end, local_n, local_col, original_col, k, nz = 0, dst = 0;

    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!A || !components || component < 0 || component >= components->count ||
        A->n < 1 ||
        components->n != A->n || !components->offset || !components->vertices ||
        !components->component_of || !components->local_of)
        return VSDLSS_ERR_INVALID;
    if (A->n == INT64_MAX || !checked_count(A->n + 1, sizeof(csi)))
        return VSDLSS_ERR_OOM;
    if (validate) {
        status = vsdlss_validate_upper_csc(A);
        if (status != VSDLSS_OK) return status;
    }
    start = components->offset[component]; end = components->offset[component + 1];
    if (start < 0 || end <= start || end > A->n) return VSDLSS_ERR_INVALID;
    local_n = end - start;
    for (local_col = 0; local_col < local_n; ++local_col) {
        original_col = components->vertices[start + local_col];
        if (original_col < 0 || original_col >= A->n ||
            components->component_of[original_col] != component ||
            components->local_of[original_col] != local_col)
            return VSDLSS_ERR_INVALID;
        for (k = A->p[original_col]; k < A->p[original_col + 1]; ++k)
            if (components->component_of[A->i[k]] == component) ++nz;
    }
    local = vsdlss_spalloc(local_n, local_n, nz, 1, 0);
    if (!local) return VSDLSS_ERR_OOM;
    local->p[0] = 0;
    for (local_col = 0; local_col < local_n; ++local_col) {
        original_col = components->vertices[start + local_col];
        for (k = A->p[original_col]; k < A->p[original_col + 1]; ++k) {
            csi original_row = A->i[k];
            if (components->component_of[original_row] != component) continue;
            local->i[dst] = components->local_of[original_row];
            local->x[dst++] = A->x[k];
        }
        local->p[local_col + 1] = dst;
    }
    local->nzmax = dst > 0 ? dst : 1;
    *out = local;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_component_extract(const vsdlss *A,
                                       const vsdlss_components *components,
                                       csi component, vsdlss **out)
{
    return component_extract_impl(A, components, component, out, 1);
}

vsdlss_status vsdlss_component_extract_normalized(const vsdlss *A,
                                                  const vsdlss_components *components,
                                                  csi component, vsdlss **out)
{
    return component_extract_impl(A, components, component, out, 0);
}

/* Reads A once in its own column order (sequential) and scatters entries to
 * their new columns; walking the columns in the new order instead would turn
 * every read of A into a cache miss on scattered input. */
vsdlss_status vsdlss_component_extract_permuted(const vsdlss *A,
                                                const vsdlss_components *components,
                                                csi component, const csi *perm,
                                                vsdlss **out)
{
    vsdlss *local = NULL;
    csi *newidx = NULL, *cursor = NULL, start, local_n, k, p, nz = 0;
    int whole;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!A || !components || !perm || component < 0 || component >= components->count ||
        components->n != A->n || !components->offset || !components->vertices ||
        !components->component_of || !components->local_of)
        return VSDLSS_ERR_INVALID;
    start = components->offset[component];
    local_n = components->offset[component + 1] - start;
    if (local_n < 1 || !checked_count(A->n + 1, sizeof(csi))) return VSDLSS_ERR_INVALID;
    whole = local_n == A->n;          /* single component: no membership test */
    newidx = (csi *)malloc((size_t)A->n * sizeof(csi));
    cursor = (csi *)calloc((size_t)local_n + 1, sizeof(csi));
    if (!newidx || !cursor) { free(newidx); free(cursor); return VSDLSS_ERR_OOM; }
    for (k = 0; k < local_n; ++k) newidx[components->vertices[start + k]] = -1;
    for (k = 0; k < local_n; ++k) {
        csi g;
        if (perm[k] < 0 || perm[k] >= local_n) { free(newidx); free(cursor); return VSDLSS_ERR_INVALID; }
        g = components->vertices[start + perm[k]];
        if (newidx[g] >= 0) { free(newidx); free(cursor); return VSDLSS_ERR_INVALID; }
        newidx[g] = k;
    }
#define MEMBER(v) (whole || components->component_of[v] == component)
    for (csi g = 0; g < A->n; ++g) {
        if (!MEMBER(g)) continue;
        csi b = newidx[g];
        for (p = A->p[g]; p < A->p[g + 1]; ++p) {
            csi row = A->i[p], a;
            if (!whole && !MEMBER(row)) continue;
            a = newidx[row];
            ++cursor[(a > b ? a : b) + 1];
            ++nz;
        }
    }
    local = vsdlss_spalloc(local_n, local_n, nz, 1, 0);
    if (!local) { free(newidx); free(cursor); return VSDLSS_ERR_OOM; }
    local->p[0] = 0;
    for (k = 0; k < local_n; ++k) { local->p[k + 1] = local->p[k] + cursor[k + 1]; cursor[k] = local->p[k]; }
    for (csi g = 0; g < A->n; ++g) {
        if (!MEMBER(g)) continue;
        csi b = newidx[g];
        for (p = A->p[g]; p < A->p[g + 1]; ++p) {
            csi row = A->i[p], a, col, at;
            if (!whole && !MEMBER(row)) continue;
            a = newidx[row];
            col = a > b ? a : b;
            at = cursor[col]++;
            local->i[at] = a < b ? a : b; local->x[at] = A->x[p];
        }
    }
#undef MEMBER
    /* Columns are short: insertion sort keeps them normalized. */
    for (k = 0; k < local_n; ++k) {
        for (p = local->p[k] + 1; p < local->p[k + 1]; ++p) {
            csi ri = local->i[p], q = p; double xi = local->x[p];
            while (q > local->p[k] && local->i[q - 1] > ri) {
                local->i[q] = local->i[q - 1]; local->x[q] = local->x[q - 1]; --q;
            }
            local->i[q] = ri; local->x[q] = xi;
        }
    }
    local->nzmax = nz > 0 ? nz : 1;
    free(newidx); free(cursor);
    *out = local;
    return VSDLSS_OK;
}
