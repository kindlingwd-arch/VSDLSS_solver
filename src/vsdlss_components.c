#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

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

/* Lock-free union-find with linking by index: the larger root is always
 * hung under the smaller one, so every root is the minimum vertex of its
 * tree and the final roots do not depend on the order of the unions. */
static csi uf_find(csi *parent, csi x)
{
    for (;;) {
        csi p = __atomic_load_n(&parent[x], __ATOMIC_RELAXED);
        if (p == x) return x;
        csi g = __atomic_load_n(&parent[p], __ATOMIC_RELAXED);
        if (g != p) __atomic_compare_exchange_n(&parent[x], &p, g, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
        x = g;
    }
}

static void uf_union(csi *parent, csi a, csi b)
{
    for (;;) {
        a = uf_find(parent, a); b = uf_find(parent, b);
        if (a == b) return;
        if (a < b) { csi t = a; a = b; b = t; }          /* hang a (larger) under b */
        csi expect = a;
        if (__atomic_compare_exchange_n(&parent[a], &expect, b, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED))
            return;
    }
}

/* Connected components, BFS order within each component and (optionally)
 * the weighted symmetric adjacency.  Components are numbered by their
 * smallest vertex and each BFS starts there, visiting neighbours in list
 * order; the result is the same for every thread count.
 *   - Adjacency: rows are split into R ranges; each thread fills the lists
 *     of its own range (the column's lower part, then the scattered upper
 *     entries in increasing column order), so there are no atomics and the
 *     lists equal the serial ones.
 *   - With threads, the components are first labelled by union-find over
 *     the matrix entries, then the BFS of different components run
 *     concurrently (large components, e.g. separate VDD and GND nets,
 *     first). */
static vsdlss_status components_build_impl(const vsdlss *A,
                                           vsdlss_components **out,
                                           int validate, vsdlss_wgraph **graph)
{
    vsdlss_components *components = NULL;
    csi *sizes = NULL, *adj_offset = NULL, *roots = NULL, *sched = NULL;
    csi *adj_cursor = NULL, *adjacent = NULL;
    double *adjval = NULL, *diag = NULL;
    csi seed, component, adjacency_n = 0;
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
    const csi n = A->n, *Ap = A->p, *Ai = A->i; const double *Ax = A->x;
    int T = vsdlss_parallel_width((double)Ap[n] * 4);
    /* Every pass here is memory bound.  On a 2-vCPU VM none of the parallel
     * forms paid (row-range scatter, union-find overlapped with the fill,
     * concurrent BFS of two nets: all at or below serial speed), so below 3
     * threads everything stays serial.  From 3 threads: the degree pass uses
     * up to 4 row ranges, the fill up to min(4, T/2) while the remaining
     * threads label components by union-find, and the BFS of different
     * components run concurrently. */
    if (T < 3) T = 1;
    int R = T > 4 ? 4 : T;
    int RF = T >= 3 ? (T / 2 > 4 ? 4 : T / 2) : 1;
    csi bound[5], fbound[5];
    for (int t = 0; t <= R; ++t) bound[t] = n * t / R;
    for (int t = 0; t <= RF; ++t) fbound[t] = n * t / RF;

    components = (vsdlss_components *)calloc(1, sizeof(*components));
    if (!components) return VSDLSS_ERR_OOM;
    components->n = n;
    components->vertices = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    components->component_of = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    components->local_of = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    components->order = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    adj_offset = (csi *)vsdlss_big_calloc((size_t)(n + 1), sizeof(csi));
    /* The fill cursor borrows `vertices`, which is only written at the end. */
    adj_cursor = components->vertices;
    if (!components->vertices || !components->component_of ||
        !components->local_of || !components->order || !adj_offset) {
        status = VSDLSS_ERR_OOM;
        goto fail;
    }
    /* Degrees: the lower part of vertex c is its own column, the upper part
     * comes from later columns (thread t counts the rows of its range). */
    {
        csi low_total = 0;
        /* Loops over ranges (not thread ids), so a smaller team than
         * requested still covers every range. */
        VSDLSS_OMP(omp parallel for num_threads(R) if(R>1) schedule(static,1) reduction(+:low_total))
        for (int t = 0; t < R; ++t) {
            const csi lo = bound[t], hi = bound[t + 1];
            for (csi c = lo; c < hi; ++c) {
                csi low = 0;
                for (csi q = Ap[c]; q < Ap[c + 1]; ++q) low += Ai[q] != c;
                adj_cursor[c] = low; low_total += low;
            }
            for (csi c = lo + 1; c < n; ++c)
                for (csi q = Ap[c]; q < Ap[c + 1]; ++q) {
                    csi row = Ai[q];
                    if (row >= lo && row < hi && row != c) ++adj_offset[row + 1];
                }
            for (csi c = lo; c < hi; ++c) adj_offset[c + 1] += adj_cursor[c];
        }
        adjacency_n = 2 * low_total;
    }
    for (seed = 0; seed < n; ++seed) {
        if (adj_offset[seed + 1] > INT64_MAX - adj_offset[seed]) {
            status = VSDLSS_ERR_OOM; goto fail;
        }
        adj_offset[seed + 1] += adj_offset[seed];
    }
    if (!checked_count(adjacency_n, sizeof(csi))) { status = VSDLSS_ERR_OOM; goto fail; }
    if (adjacency_n > 0) {
        adjacent = (csi *)vsdlss_big_malloc((size_t)adjacency_n * sizeof(csi));
        if (!adjacent) { status = VSDLSS_ERR_OOM; goto fail; }
    }
    if (graph) {
        adjval = (double *)vsdlss_big_malloc((size_t)(adjacency_n > 0 ? adjacency_n : 1) * sizeof(double));
        diag = (double *)vsdlss_big_calloc((size_t)n, sizeof(double));
        if (!adjval || !diag) { status = VSDLSS_ERR_OOM; goto fail; }
    }
    /* Each list is [neighbours < v (column v, in order)] [neighbours > v in
     * increasing column order], i.e. ascending for normalized input.  With
     * threads, threads RF.. run the union-find over the matrix entries while
     * threads 0..RF-1 fill (component_of holds the union-find parents). */
    csi *mark = components->component_of;
    const int UF = T > 1 ? T - RF : 0;
    if (UF) {
        VSDLSS_OMP(omp parallel for num_threads(T) schedule(static))
        for (csi v = 0; v < n; ++v) mark[v] = v;
    }
    VSDLSS_OMP(omp parallel for num_threads(RF + UF) if(RF + UF > 1) schedule(static,1))
    for (int t = 0; t < RF + UF; ++t) {
        if (t < RF) {
            const csi lo = fbound[t], hi = fbound[t + 1];
            for (csi c = lo; c < hi; ++c) {
                csi at = adj_offset[c];
                for (csi q = Ap[c]; q < Ap[c + 1]; ++q) {
                    csi row = Ai[q];
                    if (row == c) { if (diag) diag[c] += Ax[q]; continue; }
                    adjacent[at] = row; if (adjval) adjval[at] = Ax[q];
                    ++at;
                }
                adj_cursor[c] = at;
            }
            for (csi c = lo + 1; c < n; ++c)
                for (csi q = Ap[c]; q < Ap[c + 1]; ++q) {
                    csi row = Ai[q];
                    if (row < lo || row >= hi || row == c) continue;
                    csi at = adj_cursor[row]++;
                    adjacent[at] = c; if (adjval) adjval[at] = Ax[q];
                }
        } else {
            const csi chunk = 4096, u = t - RF;
            for (csi c0 = u * chunk; c0 < n; c0 += (csi)UF * chunk) {
                csi c1 = c0 + chunk < n ? c0 + chunk : n;
                for (csi c = c0; c < c1; ++c)
                    for (csi q = Ap[c]; q < Ap[c + 1]; ++q)
                        if (Ai[q] != c) uf_union(mark, Ai[q], c);
            }
        }
    }

    /* Component labels.  Serial: one BFS after another, seeds in increasing
     * order.  Parallel: union-find, roots (= smallest vertices) numbered in
     * increasing order, then the BFS run per component. */
    csi count = 0;
    if (!UF) {
        csi cap = 16, discovered = 0;
        sizes = (csi *)malloc((size_t)cap * sizeof(csi));
        if (!sizes) { status = VSDLSS_ERR_OOM; goto fail; }
        for (seed = 0; seed < n; ++seed) mark[seed] = -1;
        for (seed = 0; seed < n; ++seed) {
            if (mark[seed] >= 0) continue;
            if (count == cap) {
                csi *grown;
                if (cap > INT64_MAX / 2 || !checked_count(2 * cap, sizeof(csi))) { status = VSDLSS_ERR_OOM; goto fail; }
                grown = (csi *)realloc(sizes, (size_t)(2 * cap) * sizeof(csi));
                if (!grown) { status = VSDLSS_ERR_OOM; goto fail; }
                sizes = grown; cap *= 2;
            }
            csi *queue = components->order + discovered, head = 0, tail = 1;
            queue[0] = seed; mark[seed] = count;
            while (head < tail) {
                csi v = queue[head++];
                for (csi at = adj_offset[v]; at < adj_offset[v + 1]; ++at) {
                    csi w = adjacent[at];
                    if (mark[w] < 0) { mark[w] = count; queue[tail++] = w; }
                }
            }
            discovered += tail; sizes[count++] = tail;
        }
        components->count = count;
        components->offset = (csi *)malloc((size_t)(count + 1) * sizeof(csi));
        if (!components->offset) { status = VSDLSS_ERR_OOM; goto fail; }
        components->offset[0] = 0;
        for (component = 0; component < count; ++component)
            components->offset[component + 1] = components->offset[component] + sizes[component];
    } else {
        csi *parent = mark, *id = components->local_of;
        VSDLSS_OMP(omp parallel for num_threads(T) schedule(static))
        for (csi v = 0; v < n; ++v) __atomic_store_n(&parent[v], uf_find(parent, v), __ATOMIC_RELAXED);
        for (csi v = 0; v < n; ++v) if (parent[v] == v) id[v] = count++;
        components->count = count;
        sizes = (csi *)calloc((size_t)count, sizeof(csi));
        roots = (csi *)malloc((size_t)count * sizeof(csi));
        sched = (csi *)malloc((size_t)count * sizeof(csi));
        components->offset = (csi *)malloc((size_t)(count + 1) * sizeof(csi));
        if (!sizes || !roots || !sched || !components->offset) { status = VSDLSS_ERR_OOM; goto fail; }
        for (csi v = 0; v < n; ++v) { if (parent[v] == v) roots[id[v]] = v; sizes[id[parent[v]]]++; }
        components->offset[0] = 0;
        for (component = 0; component < count; ++component)
            components->offset[component + 1] = components->offset[component] + sizes[component];
        /* BFS schedule: components with >= n/(4T) vertices first (largest
         * first), then the rest in index order. */
        csi limit = n / (4 * (csi)T), nb = 0;
        for (component = 0; component < count; ++component)
            if (sizes[component] >= limit) {
                csi at = nb++;
                while (at > 0 && sizes[sched[at - 1]] < sizes[component]) { sched[at] = sched[at - 1]; --at; }
                sched[at] = component;
            }
        for (component = 0; component < count; ++component)
            if (sizes[component] < limit) sched[nb++] = component;
        /* mark[v] = root (>= 0) means unvisited; a visit stores -1-component. */
        VSDLSS_OMP(omp parallel for num_threads(T) schedule(dynamic,1))
        for (csi s = 0; s < count; ++s) {
            const csi comp = sched[s], root = roots[comp];
            csi *queue = components->order + components->offset[comp], head = 0, tail = 1;
            queue[0] = root; mark[root] = -1 - comp;
            while (head < tail) {
                csi v = queue[head++];
                for (csi at = adj_offset[v]; at < adj_offset[v + 1]; ++at) {
                    csi w = adjacent[at];
                    if (mark[w] >= 0) { mark[w] = -1 - comp; queue[tail++] = w; }
                }
            }
        }
        VSDLSS_OMP(omp parallel for num_threads(T) schedule(static))
        for (csi v = 0; v < n; ++v) mark[v] = -1 - mark[v];
    }
    for (component = 0; component < count; ++component) sizes[component] = 0;
    for (seed = 0; seed < n; ++seed) {
        component = mark[seed];
        csi position = sizes[component]++;
        components->local_of[seed] = position;
        components->vertices[components->offset[component] + position] = seed;
    }
    /* The concatenated BFS queues follow `offset`; convert global vertices
     * to local indices. */
    VSDLSS_OMP(omp parallel for num_threads(T) if(T>1) schedule(static))
    for (csi v = 0; v < n; ++v)
        components->order[v] = components->local_of[components->order[v]];
    if (graph) {
        vsdlss_wgraph *g = (vsdlss_wgraph *)calloc(1, sizeof(*g));
        if (!g) { status = VSDLSS_ERR_OOM; goto fail; }
        g->n = n; g->ptr = adj_offset; g->idx = adjacent; g->val = adjval; g->diag = diag;
        if (!g->idx) { g->idx = (csi *)malloc(sizeof(csi)); if (!g->idx) { free(g); status = VSDLSS_ERR_OOM; goto fail; } }
        adj_offset = NULL; adjacent = NULL; adjval = NULL; diag = NULL;
        *graph = g;
    }
    free(sizes); free(roots); free(sched); free(adj_offset); free(adjacent); free(adjval); free(diag);
    *out = components;
    return VSDLSS_OK;

fail:
    free(sizes); free(roots); free(sched); free(adj_offset); free(adjacent); free(adjval); free(diag);
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
