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
    free(components);
}

vsdlss_status vsdlss_components_build(const vsdlss *A,
                                      vsdlss_components **out)
{
    vsdlss_components *components = NULL;
    csi *queue = NULL, *sizes = NULL, *adj_offset = NULL;
    csi *adj_cursor = NULL, *adjacent = NULL;
    csi seed, head, tail, col, k, component, position, adjacency_n = 0;
    vsdlss_status status;

    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!A || A->n < 1)
        return VSDLSS_ERR_INVALID;
    if (A->n == INT64_MAX || !checked_count(A->n, sizeof(csi)) ||
        !checked_count(A->n + 1, sizeof(csi))) return VSDLSS_ERR_OOM;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;

    components = (vsdlss_components *)calloc(1, sizeof(*components));
    if (!components) return VSDLSS_ERR_OOM;
    components->n = A->n;
    components->vertices = (csi *)malloc((size_t)A->n * sizeof(csi));
    components->component_of = (csi *)malloc((size_t)A->n * sizeof(csi));
    components->local_of = (csi *)malloc((size_t)A->n * sizeof(csi));
    queue = (csi *)malloc((size_t)A->n * sizeof(csi));
    sizes = (csi *)calloc((size_t)A->n, sizeof(csi));
    adj_offset = (csi *)calloc((size_t)(A->n + 1), sizeof(csi));
    adj_cursor = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!components->vertices || !components->component_of ||
        !components->local_of || !queue || !sizes || !adj_offset || !adj_cursor) {
        status = VSDLSS_ERR_OOM;
        goto fail;
    }
    for (col = 0; col < A->n; ++col) {
        for (k = A->p[col]; k < A->p[col + 1]; ++k) {
            csi row = A->i[k];
            if (row == col) continue;
            if (adjacency_n > INT64_MAX - 2) { status = VSDLSS_ERR_OOM; goto fail; }
            adjacency_n += 2;
            ++adj_offset[row + 1];
            ++adj_offset[col + 1];
        }
    }
    for (seed = 0; seed < A->n; ++seed) {
        if (adj_offset[seed + 1] > INT64_MAX - adj_offset[seed]) {
            status = VSDLSS_ERR_OOM; goto fail;
        }
        adj_offset[seed + 1] += adj_offset[seed];
        adj_cursor[seed] = adj_offset[seed];
    }
    if (!checked_count(adjacency_n, sizeof(csi))) { status = VSDLSS_ERR_OOM; goto fail; }
    if (adjacency_n > 0) {
        adjacent = (csi *)malloc((size_t)adjacency_n * sizeof(csi));
        if (!adjacent) { status = VSDLSS_ERR_OOM; goto fail; }
    }
    for (col = 0; col < A->n; ++col) {
        for (k = A->p[col]; k < A->p[col + 1]; ++k) {
            csi row = A->i[k];
            if (row == col) continue;
            adjacent[adj_cursor[row]++] = col;
            adjacent[adj_cursor[col]++] = row;
        }
    }
    for (seed = 0; seed < A->n; ++seed) components->component_of[seed] = -1;

    component = 0;
    for (seed = 0; seed < A->n; ++seed) {
        if (components->component_of[seed] >= 0) continue;
        head = 0; tail = 1; queue[0] = seed;
        components->component_of[seed] = component;
        while (head < tail) {
            csi v = queue[head++];
            csi at;
            ++sizes[component];
            for (at = adj_offset[v]; at < adj_offset[v + 1]; ++at) {
                csi neighbor = adjacent[at];
                if (components->component_of[neighbor] < 0) {
                    components->component_of[neighbor] = component;
                    queue[tail++] = neighbor;
                }
            }
        }
        ++component;
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
    free(queue); free(sizes); free(adj_offset); free(adj_cursor); free(adjacent);
    *out = components;
    return VSDLSS_OK;

fail:
    free(queue); free(sizes); free(adj_offset); free(adj_cursor); free(adjacent);
    vsdlss_components_free(components);
    return status;
}

vsdlss_status vsdlss_component_extract(const vsdlss *A,
                                       const vsdlss_components *components,
                                       csi component, vsdlss **out)
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
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
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
