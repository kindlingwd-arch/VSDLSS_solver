/* MLD ordering of the M3 reduced core.  Same algorithm, same tie-breaking and
 * therefore the same permutation as vsdlss_mld_order (vsdlss_mld.c,
 * vsdlss_mld_graph.c, vsdlss_mld_partition.c, vsdlss_min_degree.c), with the
 * work of every step bounded by the subgraph it handles:
 *   - subgraphs (levels, leaf and separator orderings) are built from the
 *     subset's own columns through maps kept for the whole ordering, instead
 *     of initializing an n-array and scanning the whole core per subgraph;
 *   - coarsening sorts by (degree, original) with qsort and numbers coarse
 *     vertices in one pass, instead of an insertion sort and an O(n^2) scan;
 *   - greedy growing and refinement keep links / gains in heaps (stale
 *     entries skipped on pop) instead of rescanning all vertices per move;
 *   - the separator's matching stamps visited vertices instead of clearing
 *     an n-array per left vertex, and augments without recursion;
 *   - minimum degree selects from a heap instead of scanning every vertex
 *     and every earlier step per step.
 * Every selection keeps the original's rule, so the orderings are equal
 * (test_m3 compares them with vsdlss_mld_order). */
#include "vsdlss_m3_internal.h"
#include "vsdlss_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------- heaps */

/* Entries (key, tie, v): a max-heap on key, then a min-heap on tie. */
typedef struct { csi key, tie, v; } hentry;
typedef struct { hentry *e; csi n, cap; } heap;

static int hbefore(hentry a, hentry b) { return a.key > b.key || (a.key == b.key && a.tie < b.tie); }
static int hpush(heap *h, csi key, csi tie, csi v)
{
    if (h->n == h->cap) {
        csi cap = h->cap ? 2 * h->cap : 64;
        hentry *e = (hentry *)realloc(h->e, (size_t)cap * sizeof(*e));
        if (!e) return 0;
        h->e = e; h->cap = cap;
    }
    hentry x = {key, tie, v}; csi at = h->n++;
    while (at > 0) { csi p = (at - 1) / 2; if (!hbefore(x, h->e[p])) break; h->e[at] = h->e[p]; at = p; }
    h->e[at] = x;
    return 1;
}
static void hpop(heap *h)
{
    hentry last = h->e[--h->n]; csi at = 0;
    for (;;) {
        csi c = 2 * at + 1;
        if (c >= h->n) break;
        if (c + 1 < h->n && hbefore(h->e[c + 1], h->e[c])) c++;
        if (!hbefore(h->e[c], last)) break;
        h->e[at] = h->e[c]; at = c;
    }
    if (h->n) h->e[at] = last;
}
static void hfree(heap *h) { free(h->e); h->e = NULL; h->n = h->cap = 0; }

/* --------------------------------------------------------------- levels */

typedef struct mlevel {
    csi n, nz;
    csi *offset, *neighbor, *ew, *vw, *orig, *map;
} mlevel;
typedef struct { csi a, b, w; } edge_rec;

static void level_free(mlevel *g)
{
    if (!g) return;
    free(g->offset); free(g->neighbor); free(g->ew); free(g->vw); free(g->orig); free(g->map); free(g);
}
/* Sort e[0..m) by (a, b), both in [0, n): counting sort by b, then a stable
 * counting sort by a.  The order of equal pairs is immaterial (they merge). */
static vsdlss_status sort_edges(csi n, edge_rec *e, csi m)
{
    csi *count = (csi *)malloc((size_t)(n + 1) * sizeof(csi)), k;
    edge_rec *t = (edge_rec *)malloc((size_t)(m ? m : 1) * sizeof(*t));
    if (!count || !t) { free(count); free(t); return VSDLSS_ERR_OOM; }
    memset(count, 0, (size_t)(n + 1) * sizeof(csi));
    for (k = 0; k < m; k++) count[e[k].b + 1]++;
    for (k = 0; k < n; k++) count[k + 1] += count[k];
    for (k = 0; k < m; k++) t[count[e[k].b]++] = e[k];
    memset(count, 0, (size_t)(n + 1) * sizeof(csi));
    for (k = 0; k < m; k++) count[t[k].a + 1]++;
    for (k = 0; k < n; k++) count[k + 1] += count[k];
    for (k = 0; k < m; k++) e[count[t[k].a]++] = t[k];
    free(count); free(t);
    return VSDLSS_OK;
}
/* As make_level of vsdlss_mld_graph.c: sort, merge duplicates (weights add).
 * Every adjacency list comes out in ascending neighbour order. */
static vsdlss_status make_level(csi n, const csi *vw, const csi *orig, edge_rec *e, csi m, mlevel **out)
{
    mlevel *g; csi k, u = 0, *cursor;
    if (sort_edges(n, e, m) != VSDLSS_OK) return VSDLSS_ERR_OOM;
    for (k = 0; k < m; k++) {
        if (e[k].a == e[k].b) continue;
        if (u && e[u - 1].a == e[k].a && e[u - 1].b == e[k].b) e[u - 1].w += e[k].w;
        else e[u++] = e[k];
    }
    g = (mlevel *)calloc(1, sizeof(*g));
    if (!g) return VSDLSS_ERR_OOM;
    g->n = n; g->nz = 2 * u;
    g->offset = (csi *)calloc((size_t)n + 1, sizeof(csi));
    g->neighbor = (csi *)malloc((size_t)(g->nz ? g->nz : 1) * sizeof(csi));
    g->ew = (csi *)malloc((size_t)(g->nz ? g->nz : 1) * sizeof(csi));
    g->vw = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    g->orig = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    cursor = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    if (!g->offset || !g->neighbor || !g->ew || !g->vw || !g->orig || !cursor) { free(cursor); level_free(g); return VSDLSS_ERR_OOM; }
    for (k = 0; k < u; k++) { g->offset[e[k].a + 1]++; g->offset[e[k].b + 1]++; }
    for (k = 0; k < n; k++) { g->offset[k + 1] += g->offset[k]; cursor[k] = g->offset[k]; g->vw[k] = vw[k]; g->orig[k] = orig[k]; }
    for (k = 0; k < u; k++) {
        csi p = cursor[e[k].a]++; g->neighbor[p] = e[k].b; g->ew[p] = e[k].w;
        p = cursor[e[k].b]++; g->neighbor[p] = e[k].a; g->ew[p] = e[k].w;
    }
    free(cursor);
    *out = g;
    return VSDLSS_OK;
}

/* Scratch of one thread.  Independent subsets (the two sides of a
 * separator, the components of a set) are ordered as tasks; each writes its
 * own range of the order, so the result does not depend on the schedule.
 * A task uses the scratch of the thread running it, and holds no state in it
 * across a task scheduling point (task creation or taskwait), so tasks
 * interleaved on one thread do not disturb each other. */
struct mld_shared;
typedef struct mld_ctx {
    const vsdlss *A;
    const vsdlss_graph *graph;
    struct mld_shared *sh;
    csi *member, *distance, *queue;
    csi *local;                  /* -1, or the subset position of a vertex (reset after use) */
    signed char *side;           /* side of every vertex in the current partition */
    csi generation;
} mld_ctx;
typedef struct mld_shared {
    const vsdlss *A;
    const vsdlss_graph *graph;
    mld_ctx *ctx;                /* one per thread of the team, allocated on first use */
    int threads;
} mld_shared;

#define MLD_TASK_MIN 2048        /* smallest subset ordered as a task of its own */

static void ctx_free(mld_ctx *c)
{
    free(c->member); free(c->distance); free(c->queue); free(c->local); free(c->side);
}
static mld_ctx *thread_ctx(mld_shared *sh)
{
    const csi n = sh->A->n; int t = 0; mld_ctx *c;
#ifdef _OPENMP
    t = omp_get_thread_num();
#endif
    if (t >= sh->threads) return NULL;
    c = &sh->ctx[t];
    if (!c->member) {
        c->A = sh->A; c->graph = sh->graph; c->sh = sh; c->generation = 0;
        c->member = (csi *)calloc((size_t)n, sizeof(csi));
        c->distance = (csi *)malloc((size_t)n * sizeof(csi));
        c->queue = (csi *)malloc((size_t)n * sizeof(csi));
        c->local = (csi *)malloc((size_t)n * sizeof(csi));
        c->side = (signed char *)malloc((size_t)n);
        if (!c->member || !c->distance || !c->queue || !c->local || !c->side) { ctx_free(c); memset(c, 0, sizeof(*c)); return NULL; }
        for (csi k = 0; k < n; k++) c->local[k] = -1;
    }
    return c;
}

/* vsdlss_mld_level_build for the subset v[0..n) (vertex k of the level is
 * v[k]), from the subset's columns only. */
static vsdlss_status level_build(mld_ctx *c, const csi *v, csi n, mlevel **out)
{
    const vsdlss *A = c->A; csi k, m = 0, cap = 0; vsdlss_status s;
    csi *vw = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    edge_rec *e = NULL;
    if (!vw) return VSDLSS_ERR_OOM;
    for (k = 0; k < n; k++) {
        if (v[k] < 0 || v[k] >= A->n || c->local[v[k]] >= 0) { for (csi j = 0; j < k; j++) c->local[v[j]] = -1; free(vw); return VSDLSS_ERR_INVALID; }
        c->local[v[k]] = k; vw[k] = 1; cap += A->p[v[k] + 1] - A->p[v[k]];
    }
    e = (edge_rec *)malloc((size_t)(cap ? cap : 1) * sizeof(*e));
    if (!e) { s = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < n; k++) {
        const csi old = v[k];
        for (csi p = A->p[old]; p < A->p[old + 1]; p++) {
            const csi row = A->i[p];
            if (row != old && c->local[row] >= 0) {
                csi a = k, b = c->local[row];
                if (a > b) { csi t = a; a = b; b = t; }
                e[m].a = a; e[m].b = b; e[m].w = 1; m++;
            }
        }
    }
    s = make_level(n, vw, v, e, m, out);
done:
    for (k = 0; k < n; k++) c->local[v[k]] = -1;
    free(vw); free(e);
    return s;
}

/* vsdlss_mld_coarsen_one: heavy-edge matching in (degree, original) order. */
static int by_degree_original(const void *x, const void *y)   /* (a, b) = (degree, original) */
{
    const edge_rec *p = (const edge_rec *)x, *q = (const edge_rec *)y;
    if (p->a != q->a) return (p->a > q->a) - (p->a < q->a);
    return (p->b > q->b) - (p->b < q->b);
}
static vsdlss_status coarsen_one(mlevel *f, mlevel **out)
{
    const csi n = f->n; csi *order, *mate, *id, *vw = NULL, *orig = NULL, k, j, nc = 0, m = 0, total = 0, cap, target;
    edge_rec *e = NULL; vsdlss_status s = VSDLSS_OK;
    *out = NULL;
    order = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    mate = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    id = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    if (!order || !mate || !id) { s = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < n; k++) { order[k] = k; mate[k] = -1; total += f->vw[k]; }
    target = n / 2; if (target < 1) target = 1;
    cap = (3 * total + target - 1) / target; if (cap < 2) cap = 2;
    /* keys are distinct (originals are), so this is the insertion sort's order */
    e = (edge_rec *)malloc((size_t)(n ? n : 1) * sizeof(*e));
    if (!e) { s = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < n; k++) { e[k].a = f->offset[k + 1] - f->offset[k]; e[k].b = f->orig[k]; e[k].w = k; }
    qsort(e, (size_t)n, sizeof(*e), by_degree_original);
    for (k = 0; k < n; k++) order[k] = e[k].w;
    free(e); e = NULL;
    for (k = 0; k < n; k++) {
        csi v = order[k], best = -1, bw = -1;
        if (mate[v] >= 0) continue;
        for (j = f->offset[v]; j < f->offset[v + 1]; j++) {
            csi w = f->neighbor[j];
            if (mate[w] < 0 && f->vw[v] + f->vw[w] <= cap &&
                (f->ew[j] > bw || (f->ew[j] == bw && f->orig[w] < (best < 0 ? INT64_MAX : f->orig[best])))) { best = w; bw = f->ew[j]; }
        }
        mate[v] = v; if (best >= 0) mate[best] = v;
    }
    f->map = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    if (!f->map) { s = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < n; k++) if (mate[k] == k) id[k] = nc++;      /* representatives in index order */
    for (k = 0; k < n; k++) f->map[k] = id[mate[k]];
    vw = (csi *)calloc((size_t)(nc ? nc : 1), sizeof(csi));
    orig = (csi *)malloc((size_t)(nc ? nc : 1) * sizeof(csi));
    e = (edge_rec *)malloc((size_t)(f->nz / 2 ? f->nz / 2 : 1) * sizeof(*e));
    if (!vw || !orig || !e) { s = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < nc; k++) orig[k] = INT64_MAX;
    for (k = 0; k < n; k++) { csi c = f->map[k]; vw[c] += f->vw[k]; if (f->orig[k] < orig[c]) orig[c] = f->orig[k]; }
    for (k = 0; k < n; k++) for (j = f->offset[k]; j < f->offset[k + 1]; j++) if (k < f->neighbor[j]) {
        csi a = f->map[k], b = f->map[f->neighbor[j]];
        if (a != b) { if (a > b) { csi t = a; a = b; b = t; } e[m].a = a; e[m].b = b; e[m].w = f->ew[j]; m++; }
    }
    s = make_level(nc, vw, orig, e, m, out);
done:
    free(order); free(mate); free(id); free(vw); free(orig); free(e);
    return s;
}

/* ------------------------------------------------- partition and refine */

static csi cut_weight(const mlevel *g, const signed char *part)
{
    csi v, k, sum = 0;
    for (v = 0; v < g->n; v++) for (k = g->offset[v]; k < g->offset[v + 1]; k++)
        if (v < g->neighbor[k] && part[v] != part[g->neighbor[k]]) sum += g->ew[k];
    return sum;
}
static int by_csi(const void *x, const void *y)
{
    const csi a = *(const csi *)x, b = *(const csi *)y;
    return (a > b) - (a < b);
}

/* refine of vsdlss_mld_partition.c: up to n passes; each moves the vertex of
 * largest positive gain (out - in) whose move keeps the sides balanced,
 * the smallest index among equals.  Gains are kept up to date and the
 * candidates of each (side, vertex weight) sit in a heap; a move's balance
 * depends only on the side and the weight. */
static vsdlss_status refine(const mlevel *g, signed char *part)
{
    const csi n = g->n; csi a = 0, b = 0, v, k, nw = 0, pass;
    csi *gain = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));
    csi *wid = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));      /* weight class of a vertex */
    csi *wval = (csi *)malloc((size_t)(n ? n : 1) * sizeof(csi));     /* weight of a class, ascending */
    heap *hs = NULL; vsdlss_status s = VSDLSS_OK;
    if (!gain || !wid || !wval) { s = VSDLSS_ERR_OOM; goto done; }
    {
        csi lo = n ? g->vw[0] : 0, hi = lo;
        for (v = 1; v < n; v++) { if (g->vw[v] < lo) lo = g->vw[v]; if (g->vw[v] > hi) hi = g->vw[v]; }
        if (hi - lo < 4 * n + 64) {                   /* distinct weights by marking */
            unsigned char *mark = (unsigned char *)calloc((size_t)(hi - lo + 1), 1);
            if (!mark) { s = VSDLSS_ERR_OOM; goto done; }
            for (v = 0; v < n; v++) mark[g->vw[v] - lo] = 1;
            for (v = 0; v <= hi - lo && n; v++) if (mark[v]) wval[nw++] = lo + v;
            free(mark);
        } else {
            for (v = 0; v < n; v++) wval[v] = g->vw[v];
            qsort(wval, (size_t)n, sizeof(csi), by_csi);
            for (v = 0; v < n; v++) if (!nw || wval[v] != wval[nw - 1]) wval[nw++] = wval[v];
        }
    }
    for (v = 0; v < n; v++) {
        csi lo = 0, hi = nw - 1;
        while (lo < hi) { csi mid = (lo + hi) / 2; if (wval[mid] < g->vw[v]) lo = mid + 1; else hi = mid; }
        wid[v] = lo;
    }
    hs = (heap *)calloc((size_t)(2 * (nw ? nw : 1)), sizeof(heap));
    if (!hs) { s = VSDLSS_ERR_OOM; goto done; }
    for (v = 0; v < n; v++) {
        csi in = 0, out = 0;
        if (part[v] == 0) a += g->vw[v]; else b += g->vw[v];
        for (k = g->offset[v]; k < g->offset[v + 1]; k++) { if (part[g->neighbor[k]] == part[v]) in += g->ew[k]; else out += g->ew[k]; }
        gain[v] = out - in;
        if (gain[v] > 0 && !hpush(&hs[2 * wid[v] + part[v]], gain[v], v, v)) { s = VSDLSS_ERR_OOM; goto done; }
    }
    const csi t = a + b, limit = t - t / 8 - (t % 8 != 0);
    for (pass = 0; pass < n; pass++) {
        csi best = -1, best_gain = 0;
        for (int sd = 0; sd < 2; sd++) {
            const csi from = sd ? b : a, to = sd ? a : b;
            /* moving weight w from `from` to `to`: from - w > 0, to + w > 0,
             * both <= limit */
            for (csi c = 0; c < nw; c++) {
                const csi w = wval[c];
                if (!(from - w > 0 && to + w > 0 && from - w <= limit && to + w <= limit)) continue;
                heap *h = &hs[2 * c + sd];
                while (h->n) {
                    const hentry top = h->e[0];
                    if (part[top.v] == sd && gain[top.v] == top.key && gain[top.v] > 0) break;
                    hpop(h);
                }
                if (h->n && (h->e[0].key > best_gain || (h->e[0].key == best_gain && best >= 0 && h->e[0].v < best))) {
                    best = h->e[0].v; best_gain = h->e[0].key;
                }
            }
        }
        if (best < 0) break;
        /* move best; its gain changes sign, its neighbours' by 2 * edge weight */
        const signed char from = part[best];
        part[best] = (signed char)(1 - from);
        if (from == 0) { a -= g->vw[best]; b += g->vw[best]; } else { b -= g->vw[best]; a += g->vw[best]; }
        gain[best] = -gain[best];
        if (gain[best] > 0 && !hpush(&hs[2 * wid[best] + part[best]], gain[best], best, best)) { s = VSDLSS_ERR_OOM; goto done; }
        for (k = g->offset[best]; k < g->offset[best + 1]; k++) {
            const csi u = g->neighbor[k];
            gain[u] += part[u] == from ? 2 * g->ew[k] : -2 * g->ew[k];
            if (gain[u] > 0 && !hpush(&hs[2 * wid[u] + part[u]], gain[u], u, u)) { s = VSDLSS_ERR_OOM; goto done; }
        }
    }
done:
    if (hs) for (v = 0; v < 2 * (nw ? nw : 1); v++) hfree(&hs[v]);
    free(hs); free(gain); free(wid); free(wval);
    return s;
}

/* vsdlss_mld_partition_level: five seeds; from each, grow side 0 by the
 * vertex most strongly linked to it (smallest original among equals) up to
 * half the weight, refine, keep the smallest cut (the first among equals). */
static vsdlss_status partition_level(const mlevel *g, signed char *part)
{
    const csi n = g->n; csi v, total = 0, best_cut = -1, seeds[5];
    signed char *cand; csi *link; heap h = {0}; vsdlss_status s = VSDLSS_OK;
    if (n < 2) return VSDLSS_ERR_INVALID;
    for (v = 0; v < n; v++) total += g->vw[v];
    cand = (signed char *)malloc((size_t)n);
    link = (csi *)malloc((size_t)n * sizeof(csi));
    if (!cand || !link) { free(cand); free(link); return VSDLSS_ERR_OOM; }
    seeds[0] = 0; seeds[1] = n / 4; seeds[2] = n / 2; seeds[3] = (3 * n) / 4; seeds[4] = n - 1;
    for (int si = 0; si < 5 && s == VSDLSS_OK; si++) {
        csi used = 0, chosen = seeds[si], cut;
        h.n = 0;
        for (v = 0; v < n && s == VSDLSS_OK; v++) {
            cand[v] = -1; link[v] = 0;
            if (!hpush(&h, 0, g->orig[v], v)) s = VSDLSS_ERR_OOM;
        }
        while (s == VSDLSS_OK && used < total / 2) {
            csi best = -1;
            if (chosen >= 0 && cand[chosen] < 0) best = chosen;
            else {
                while (h.n && (cand[h.e[0].v] >= 0 || link[h.e[0].v] != h.e[0].key)) hpop(&h);
                if (h.n) best = h.e[0].v;
            }
            if (best < 0) break;
            cand[best] = 0; used += g->vw[best]; chosen = -1;
            for (csi k = g->offset[best]; k < g->offset[best + 1]; k++) {
                const csi u = g->neighbor[k];
                if (cand[u] >= 0) continue;
                link[u] += g->ew[k];
                if (!hpush(&h, link[u], g->orig[u], u)) { s = VSDLSS_ERR_OOM; break; }
            }
        }
        if (s != VSDLSS_OK) break;
        for (v = 0; v < n; v++) if (cand[v] < 0) cand[v] = 1;
        s = refine(g, cand);
        if (s != VSDLSS_OK) break;
        cut = cut_weight(g, cand);
        if (best_cut < 0 || cut < best_cut) { memcpy(part, cand, (size_t)n); best_cut = cut; }
    }
    hfree(&h); free(cand); free(link);
    return s;
}

/* vsdlss_mld_node_separator: maximum matching across the cut (augmenting
 * from the left vertices in index order, neighbours in order), then the
 * vertex cover reached from unmatched left vertices by alternating paths. */
static vsdlss_status node_separator(const mlevel *g, signed char *part, csi *left, csi *right, csi *separator)
{
    const csi n = g->n; const size_t sz = (size_t)(n ? n : 1);
    csi v, k, head = 0, tail = 0, stamp = 0;
    csi *mr = (csi *)malloc(sz * sizeof(csi)), *ml = (csi *)malloc(sz * sizeof(csi));
    csi *queue = (csi *)malloc(sz * sizeof(csi)), *seen = (csi *)calloc(sz, sizeof(csi));
    csi *su = (csi *)malloc(sz * sizeof(csi)), *sk = (csi *)malloc(sz * sizeof(csi)), *sr = (csi *)malloc(sz * sizeof(csi));
    unsigned char *zl = (unsigned char *)calloc(sz, 1), *zr = (unsigned char *)calloc(sz, 1);
    vsdlss_status s = VSDLSS_OK;
    if (!mr || !ml || !queue || !seen || !su || !sk || !sr || !zl || !zr) { s = VSDLSS_ERR_OOM; goto done; }
    for (v = 0; v < n; v++) mr[v] = ml[v] = -1;
    for (v = 0; v < n; v++) if (part[v] == 0) {
        /* the recursive augment as a stack: frame t scans the neighbours of
         * su[t] from edge sk[t]; sr[t] is the right vertex it descended
         * through.  A failed frame returns to its parent's next edge. */
        csi top = 0; int found = 0;
        stamp++;
        su[0] = v; sk[0] = g->offset[v];
        while (top >= 0 && !found) {
            const csi u = su[top]; int descended = 0;
            while (sk[top] < g->offset[u + 1]) {
                const csi r = g->neighbor[sk[top]++];
                if (part[r] != 1 || seen[r] == stamp) continue;
                seen[r] = stamp; sr[top] = r;
                if (mr[r] < 0) { found = 1; break; }
                su[top + 1] = mr[r]; sk[top + 1] = g->offset[mr[r]]; top++; descended = 1; break;
            }
            if (!found && !descended) top--;
        }
        if (found) {
            for (csi t = top; t >= 0; t--) mr[sr[t]] = su[t];
            ml[v] = sr[0];                    /* the right vertex matched to v now */
        }
    }
    for (v = 0; v < n; v++) if (part[v] == 0 && ml[v] < 0) { zl[v] = 1; queue[tail++] = v; }
    while (head < tail) {
        const csi u = queue[head++];
        for (k = g->offset[u]; k < g->offset[u + 1]; k++) {
            const csi r = g->neighbor[k];
            if (part[r] != 1 || ml[u] == r || zr[r]) continue;
            zr[r] = 1;
            if (mr[r] >= 0 && !zl[mr[r]]) { zl[mr[r]] = 1; queue[tail++] = mr[r]; }
        }
    }
    *left = *right = *separator = 0;
    for (v = 0; v < n; v++) {
        if ((part[v] == 0 && !zl[v]) || (part[v] == 1 && zr[v])) part[v] = 2;
        if (part[v] == 0) (*left)++; else if (part[v] == 1) (*right)++; else (*separator)++;
    }
done:
    free(mr); free(ml); free(queue); free(seen); free(su); free(sk); free(sr); free(zl); free(zr);
    return s;
}

/* ------------------------------------------------------- minimum degree */

/* vsdlss_min_degree_subset + vsdlss_min_degree_order for v[0..n): the graph
 * of the subset (vertex k is v[k]), then repeatedly eliminate the vertex of
 * smallest current degree (smallest index among equals); an elimination
 * changes only the degrees of the eliminated vertex's neighbours. */
static vsdlss_status min_degree_subset(mld_ctx *c, const csi *v, csi n, csi *order)
{
    const vsdlss *A = c->A; csi k, p, nz = n;
    vsdlss *sub = NULL; csi *next = NULL, *deg = NULL; unsigned char *gone = NULL;
    vsdlss_graph *graph = NULL; heap h = {0}; vsdlss_status s = VSDLSS_ERR_OOM;
    if (n < 1 || n > A->n) return VSDLSS_ERR_INVALID;
    for (k = 0; k < n; k++) {
        if (v[k] < 0 || v[k] >= A->n || c->local[v[k]] >= 0) {
            for (csi j = 0; j < k; j++) c->local[v[j]] = -1;
            return VSDLSS_ERR_INVALID;
        }
        c->local[v[k]] = k;
    }
    for (k = 0; k < n; k++) for (p = A->p[v[k]]; p < A->p[v[k] + 1]; p++)
        if (A->i[p] != v[k] && c->local[A->i[p]] >= 0) nz++;
    sub = vsdlss_spalloc(n, n, nz, 1, 0);
    next = (csi *)malloc((size_t)n * sizeof(csi));
    if (!sub || !next) goto done;
    for (k = 0; k < n; k++) next[k] = 1;
    for (k = 0; k < n; k++) for (p = A->p[v[k]]; p < A->p[v[k] + 1]; p++)
        if (A->i[p] != v[k] && c->local[A->i[p]] >= 0) { const csi a = c->local[A->i[p]]; next[a > k ? a : k]++; }
    sub->p[0] = 0;
    for (k = 0; k < n; k++) sub->p[k + 1] = sub->p[k] + next[k];
    memcpy(next, sub->p, (size_t)n * sizeof(csi));
    for (k = 0; k < n; k++) { const csi at = next[k]++; sub->i[at] = k; sub->x[at] = 1.0; }
    for (k = 0; k < n; k++) for (p = A->p[v[k]]; p < A->p[v[k] + 1]; p++)
        if (A->i[p] != v[k] && c->local[A->i[p]] >= 0) {
            const csi a = c->local[A->i[p]], at = next[a > k ? a : k]++;
            sub->i[at] = a < k ? a : k; sub->x[at] = 1.0;
        }
    for (k = 0; k < n; k++) c->local[v[k]] = -1;
    s = vsdlss_graph_build(sub, &graph);
    if (s != VSDLSS_OK) goto done;
    s = VSDLSS_ERR_OOM;
    deg = (csi *)malloc((size_t)n * sizeof(csi));
    gone = (unsigned char *)calloc((size_t)n, 1);
    if (!deg || !gone) goto done;
    for (k = 0; k < n; k++) { deg[k] = vsdlss_graph_degree(graph, k); if (!hpush(&h, -deg[k], k, k)) goto done; }
    for (csi step = 0; step < n; step++) {
        csi best, *nb = NULL, cnt = 0;
        while (h.n && (gone[h.e[0].v] || -h.e[0].key != deg[h.e[0].v])) hpop(&h);
        if (!h.n) { s = VSDLSS_ERR_INVALID; goto done; }
        best = h.e[0].v; hpop(&h);
        order[step] = v[best]; gone[best] = 1;
        s = vsdlss_graph_eliminate(graph, best, &nb, &cnt);
        if (s != VSDLSS_OK) { free(nb); goto done; }
        s = VSDLSS_ERR_OOM;
        for (csi j = 0; j < cnt; j++) {
            const csi u = nb[j];
            deg[u] = vsdlss_graph_degree(graph, u);
            if (!hpush(&h, -deg[u], u, u)) { free(nb); goto done; }
        }
        free(nb);
    }
    s = VSDLSS_OK;
done:
    for (k = 0; k < n; k++) c->local[v[k]] = -1;
    hfree(&h); free(deg); free(gone); free(next); vsdlss_spfree(sub); vsdlss_graph_free(graph);
    return s;
}

/* ------------------------------------------------- nested dissection */

typedef struct { csi start, size, minimum; } mld_part;

static vsdlss_status mark_subset(mld_ctx *c, const csi *v, csi n)
{
    if (c->generation == INT64_MAX) return VSDLSS_ERR_OOM;
    c->generation++;
    for (csi k = 0; k < n; k++) { c->member[v[k]] = c->generation; c->distance[v[k]] = -1; }
    return VSDLSS_OK;
}

static vsdlss_status bfs(mld_ctx *c, csi start, csi *reached, csi *ecc)
{
    csi head = 0, tail = 0, maximum = 0;
    c->queue[tail++] = start;
    c->distance[start] = 0;
    while (head < tail) {
        const csi u = c->queue[head++]; const csi *nb; csi count;
        vsdlss_status s = vsdlss_graph_neighbors(c->graph, u, &nb, &count);
        if (s != VSDLSS_OK) return s;
        for (csi p = 0; p < count; p++) {
            const csi w = nb[p];
            if (c->member[w] == c->generation && c->distance[w] < 0) {
                c->distance[w] = c->distance[u] + 1;
                if (c->distance[w] > maximum) maximum = c->distance[w];
                c->queue[tail++] = w;
            }
        }
    }
    *reached = tail; *ecc = maximum;
    return VSDLSS_OK;
}

static csi farthest_low_degree(const mld_ctx *c, const csi *v, csi n, csi ecc)
{
    csi best = -1, degree_best = INT64_MAX;
    for (csi k = 0; k < n; k++) if (c->distance[v[k]] == ecc) {
        const csi *nb; csi count, degree = 0;
        if (vsdlss_graph_neighbors(c->graph, v[k], &nb, &count) != VSDLSS_OK) return -1;
        for (csi p = 0; p < count; p++) degree += c->member[nb[p]] == c->generation;
        if (degree < degree_best || (degree == degree_best && v[k] < best)) { best = v[k]; degree_best = degree; }
    }
    return best;
}

static vsdlss_status pseudo_peripheral_bfs(mld_ctx *c, const csi *v, csi n, csi *ecc)
{
    csi k, root = v[0], reached, previous = -1;
    for (k = 1; k < n; k++) if (v[k] < root) root = v[k];
    for (;;) {
        vsdlss_status s;
        for (k = 0; k < n; k++) c->distance[v[k]] = -1;
        s = bfs(c, root, &reached, ecc);
        if (s != VSDLSS_OK) return s;
        if (reached != n) return VSDLSS_ERR_INVALID;
        if (*ecc <= previous) return VSDLSS_OK;
        previous = *ecc;
        root = farthest_low_degree(c, v, n, *ecc);
        if (root < 0) return VSDLSS_ERR_INVALID;
    }
}

static int unbalanced(csi left, csi right)
{
    const csi total = left + right, limit = total - total / 8 - (total % 8 != 0);
    return left == 0 || right == 0 || left > limit || right > limit;
}

static vsdlss_status balanced_fallback(mld_ctx *c, const csi *v, csi n, signed char *side,
                                       csi *left, csi *right, csi *separator)
{
    csi k;
    *left = n / 2; *right = n - *left; *separator = 0;
    for (k = 0; k < n; k++) side[c->queue[k]] = k < *left ? 0 : 1;
    for (;;) {
        csi a = -1, b = -1, move;
        for (k = 0; k < n && a < 0; k++) {
            const csi node = v[k]; const csi *nb; csi degree;
            if (side[node] == 2) continue;
            vsdlss_status s = vsdlss_graph_neighbors(c->graph, node, &nb, &degree);
            if (s != VSDLSS_OK) return s;
            for (csi p = 0; p < degree; p++) {
                const csi other = nb[p];
                if (c->member[other] == c->generation && side[other] != 2 && side[node] != side[other]) { a = node; b = other; break; }
            }
        }
        if (a < 0) return VSDLSS_OK;
        if (*left > *right) move = side[a] == 0 ? a : b;
        else if (*right > *left) move = side[a] == 1 ? a : b;
        else move = a > b ? a : b;
        if (side[move] == 0) (*left)--; else (*right)--;
        side[move] = 2; (*separator)++;
    }
}

/* partition_connected of vsdlss_mld.c: coarsen, partition the coarsest
 * level, project and refine back, separator from the vertex cover; the BFS
 * level split when the cover is degenerate. */
static vsdlss_status partition_connected(mld_ctx *c, const csi *v, csi n, signed char *side,
                                         csi *left, csi *right, csi *separator)
{
    csi k, ecc, middle, depth = 0, allocated = 0;
    mlevel **level = NULL; signed char *part = NULL;
    vsdlss_status s = mark_subset(c, v, n);
    if (s != VSDLSS_OK) return s;
    *left = *right = *separator = 0;
    level = (mlevel **)calloc((size_t)n + 1, sizeof(*level));
    if (!level) return VSDLSS_ERR_OOM;
    s = level_build(c, v, n, &level[0]);
    if (s != VSDLSS_OK) goto multilevel_done;
    while (level[depth]->n > 32) {
        const csi fine_n = level[depth]->n, edges = level[depth]->nz / 2;
        const double density = fine_n > 1 ? (2.0 * (double)edges) / ((double)fine_n * (double)(fine_n - 1)) : 0.0;
        if (density > 0.20) break;
        s = coarsen_one(level[depth], &level[depth + 1]);
        if (s != VSDLSS_OK) goto multilevel_done;
        if (level[depth + 1]->n * 100 > fine_n * 85) { level_free(level[depth + 1]); level[depth + 1] = NULL; break; }
        depth++; allocated = depth;
    }
    part = (signed char *)malloc((size_t)level[depth]->n);
    if (!part) { s = VSDLSS_ERR_OOM; goto multilevel_done; }
    s = partition_level(level[depth], part);
    while (s == VSDLSS_OK && depth > 0) {
        const mlevel *fine = level[depth - 1];
        signed char *fine_part = (signed char *)malloc((size_t)fine->n);
        if (!fine_part) { s = VSDLSS_ERR_OOM; break; }
        for (k = 0; k < fine->n; k++) fine_part[k] = part[fine->map[k]];
        s = refine(fine, fine_part);
        free(part); part = fine_part; depth--;
    }
    if (s == VSDLSS_OK) s = node_separator(level[0], part, left, right, separator);
    if (s == VSDLSS_OK) for (k = 0; k < n; k++) side[v[k]] = part[k];
multilevel_done:
    free(part);
    if (level) { for (k = 0; k <= allocated; k++) level_free(level[k]); free(level); }
    if (s != VSDLSS_OK) return s;
    if (*left > 0 && *right > 0 && *separator > 0) return VSDLSS_OK;
    s = pseudo_peripheral_bfs(c, v, n, &ecc);
    if (s != VSDLSS_OK) return s;
    middle = ecc / 2;
    *left = *right = *separator = 0;
    for (k = 0; k < n; k++) {
        const csi node = v[k];
        if (c->distance[node] < middle) { side[node] = 0; (*left)++; }
        else if (c->distance[node] > middle) { side[node] = 1; (*right)++; }
        else { side[node] = 2; (*separator)++; }
    }
    return unbalanced(*left, *right) ? balanced_fallback(c, v, n, side, left, right, separator) : VSDLSS_OK;
}

static vsdlss_status order_set(mld_ctx *c, const csi *v, csi n, csi *out, csi *written);

static vsdlss_status order_connected(mld_ctx *c, const csi *v, csi n, csi *out, csi *written)
{
    csi *lv = NULL, *rv = NULL, *sv = NULL, left, right, separator, li = 0, ri = 0, si = 0, k;
    vsdlss_status s;
    if (n <= 32) {
        s = min_degree_subset(c, v, n, out);
        if (s == VSDLSS_OK) *written = n;
        return s;
    }
    lv = (csi *)malloc((size_t)n * sizeof(csi));
    rv = (csi *)malloc((size_t)n * sizeof(csi));
    sv = (csi *)malloc((size_t)n * sizeof(csi));
    if (!lv || !rv || !sv) { s = VSDLSS_ERR_OOM; goto done; }
    /* c->side is shared: it is read here, before the recursion overwrites it */
    s = partition_connected(c, v, n, c->side, &left, &right, &separator);
    if (s != VSDLSS_OK) goto done;
    for (k = 0; k < n; k++) {
        if (c->side[v[k]] == 0) lv[li++] = v[k];
        else if (c->side[v[k]] == 1) rv[ri++] = v[k];
        else sv[si++] = v[k];
    }
    if (li != left || ri != right || si != separator || separator == 0) { s = VSDLSS_ERR_INVALID; goto done; }
    {
        /* left side, right side, then the separator; the sides at once */
        mld_shared *sh = c->sh;
        vsdlss_status sl = VSDLSS_OK, sr = VSDLSS_OK;
        csi el = 0, er = 0;
        const int spawn = left >= MLD_TASK_MIN && right >= MLD_TASK_MIN;
        (void)spawn; (void)sh;
        if (left) {
            VSDLSS_OMP(omp task if(spawn) default(shared) firstprivate(sh, lv, left, out))
            {
                mld_ctx *tc = thread_ctx(sh);
                sl = tc ? order_set(tc, lv, left, out, &el) : VSDLSS_ERR_OOM;
            }
        }
        if (right) sr = order_set(c, rv, right, out + left, &er);
        VSDLSS_OMP(omp taskwait)
        s = sl != VSDLSS_OK ? sl : sr;
        if (s == VSDLSS_OK && (el != left || er != right)) s = VSDLSS_ERR_INVALID;
        if (s != VSDLSS_OK) goto done;
    }
    s = min_degree_subset(c, sv, separator, out + left + right);
    if (s == VSDLSS_OK) *written = n;
done:
    free(lv); free(rv); free(sv);
    return s;
}

static int part_after(const void *a, const void *b)
{
    const mld_part *x = (const mld_part *)a, *y = (const mld_part *)b;
    return (x->minimum > y->minimum) - (x->minimum < y->minimum);
}

/* The components of the subset, ordered by smallest vertex, each ordered. */
static vsdlss_status order_set(mld_ctx *c, const csi *v, csi n, csi *out, csi *written)
{
    csi *grouped = NULL, k, parts = 0, used = 0;
    mld_part *part = NULL;
    vsdlss_status s = mark_subset(c, v, n);
    if (s != VSDLSS_OK) return s;
    grouped = (csi *)malloc((size_t)n * sizeof(csi));
    part = (mld_part *)malloc((size_t)n * sizeof(*part));
    if (!grouped || !part) { s = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < n; k++) if (c->distance[v[k]] < 0) {
        csi reached, ecc, minimum = v[k], start = used;
        s = bfs(c, v[k], &reached, &ecc);
        if (s != VSDLSS_OK) goto done;
        for (csi j = 0; j < reached; j++) {
            grouped[used++] = c->queue[j];
            if (c->queue[j] < minimum) minimum = c->queue[j];
        }
        part[parts].start = start; part[parts].size = reached; part[parts].minimum = minimum;
        parts++;
    }
    if (used != n) { s = VSDLSS_ERR_INVALID; goto done; }
    qsort(part, (size_t)parts, sizeof(*part), part_after);
    {
        /* every component at its place in the order; large ones as tasks */
        mld_shared *sh = c->sh;
        vsdlss_status *res = (vsdlss_status *)malloc((size_t)parts * sizeof(*res));
        csi *emitted = (csi *)malloc((size_t)parts * sizeof(csi)), at = 0;
        if (!res || !emitted) { free(res); free(emitted); s = VSDLSS_ERR_OOM; goto done; }
        (void)sh;
        for (k = 0; k < parts; k++) {
            const csi *pv = grouped + part[k].start, pn = part[k].size;
            csi *pout = out + at;
            res[k] = VSDLSS_OK; emitted[k] = 0; at += pn;
            if (pn >= MLD_TASK_MIN && k + 1 < parts) {
                VSDLSS_OMP(omp task default(shared) firstprivate(sh, pv, pn, pout, k))
                {
                    mld_ctx *tc = thread_ctx(sh);
                    res[k] = tc ? order_connected(tc, pv, pn, pout, &emitted[k]) : VSDLSS_ERR_OOM;
                }
            } else res[k] = order_connected(c, pv, pn, pout, &emitted[k]);
        }
        VSDLSS_OMP(omp taskwait)
        *written = 0;
        for (k = 0; k < parts && s == VSDLSS_OK; k++) {
            if (res[k] != VSDLSS_OK) s = res[k];
            else if (emitted[k] != part[k].size) s = VSDLSS_ERR_INVALID;
            else *written += emitted[k];
        }
        free(res); free(emitted);
    }
done:
    free(grouped); free(part);
    return s;
}

vsdlss_status vsdlss_m3_mld_order(const vsdlss *A, csi **q)
{
    mld_shared sh; mld_ctx *c; vsdlss_graph *graph = NULL;
    csi *v = NULL, *order = NULL, k, written = 0;
    vsdlss_status s;
    if (!q) return VSDLSS_ERR_INVALID;
    *q = NULL;
    s = vsdlss_graph_build(A, &graph);
    if (s != VSDLSS_OK) return s;
    sh.A = A; sh.graph = graph; sh.threads = 1;
#ifdef _OPENMP
    sh.threads = omp_get_num_threads();      /* the team whose tasks may share the work */
#endif
    sh.ctx = (mld_ctx *)calloc((size_t)sh.threads, sizeof(mld_ctx));
    v = (csi *)malloc((size_t)A->n * sizeof(csi));
    order = (csi *)malloc((size_t)A->n * sizeof(csi));
    if (!sh.ctx || !v || !order || !(c = thread_ctx(&sh))) { s = VSDLSS_ERR_OOM; goto done; }
    for (k = 0; k < A->n; k++) v[k] = k;
    s = order_set(c, v, A->n, order, &written);
    if (s == VSDLSS_OK && written != A->n) s = VSDLSS_ERR_INVALID;
    if (s == VSDLSS_OK) { *q = order; order = NULL; }
done:
    free(v); free(order);
    if (sh.ctx) for (int t = 0; t < sh.threads; t++) ctx_free(&sh.ctx[t]);
    free(sh.ctx);
    vsdlss_graph_free(graph);
    return s;
}
