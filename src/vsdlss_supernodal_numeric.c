#define _DEFAULT_SOURCE
#include "vsdlss_m3_internal.h"
#include "vsdlss_dense.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#if defined(__linux__)
#include <sys/mman.h>
#endif

/* Left-looking supernodal Cholesky.
 *
 * Each target panel d is produced in one step: assemble A into it, subtract
 * every source block listed for it by the symbolic phase (in increasing source
 * order), then factor it with the blocked dense kernel.  Source rows are
 * mapped to target rows through a per-thread relative-index array, so the
 * symbolic phase needs no per-entry scatter table.
 *
 * A panel reads only panels of its own subtree, so disjoint subtrees can be
 * processed concurrently.  Phase 1 hands out independent subtrees to the
 * team; phase 2 processes the remaining top of the tree in order and uses the
 * team inside the large dense kernels instead.  Every panel sees the same
 * operations in the same order whatever the thread count or schedule, so the
 * factor is bitwise reproducible across 1..N threads.
 *
 * The old experimental DAG switch (vsdlss_set_dag_enabled) now selects the
 * same tree schedule; the default schedule already uses it when more than one
 * thread is requested. */

#define SCATTER_MC 128
#define SCATTER_NC 64

static int bytes_ok(csi n, size_t z) { return n >= 0 && (uint64_t)n <= SIZE_MAX / z; }

static void *copy_array(const void *p, csi n, size_t z)
{
    void *q;
    if (!bytes_ok(n, z)) return NULL;
    q = malloc((size_t)(n ? n : 1) * z);
    if (q && n) memcpy(q, p, (size_t)n * z);
    return q;
}

void vsdlss_sn_factor_free(vsdlss_sn_factor *f)
{
    if (!f) return;
    free(f->column_start); free(f->row_ptr); free(f->row_index);
    free(f->panel_offset); free(f->panel_block);
    free(f->sn_parent); free(f->blk_ptr); free(f->blk_src); free(f->blk_first); free(f->blk_end);
    free(f);
}

/* Zeroed panel storage.  Large factors are 2 MiB aligned and marked for
 * transparent huge pages where the kernel honours madvise: on a VM a 4 KiB
 * first-touch fault costs microseconds, and taking them one by one inside
 * the numeric loop cost more than a third of the factorization time on a
 * 30^3 grid.  The zeroing is spread over the calling thread's team, which
 * also places pages near the threads that will use them. */
#define HUGE_ALIGN ((size_t)2 << 20)
static double *panel_alloc(csi count, void **block)
{
    size_t bytes = (size_t)(count ? count : 1) * sizeof(double);
    unsigned char *raw;
    double *p;
    *block = NULL;
    if (bytes < 4 * HUGE_ALIGN) {
        p = calloc(bytes / sizeof(double), sizeof(double));
        *block = p; return p;
    }
    if (bytes > SIZE_MAX - HUGE_ALIGN) return NULL;
    raw = malloc(bytes + HUGE_ALIGN);
    if (!raw) return NULL;
    p = (double *)(raw + ((HUGE_ALIGN - ((uintptr_t)raw & (HUGE_ALIGN - 1))) & (HUGE_ALIGN - 1)));
#if defined(__linux__) && defined(MADV_HUGEPAGE)
    (void)madvise(p, bytes & ~(HUGE_ALIGN - 1), MADV_HUGEPAGE);
#endif
    {
        const size_t chunk = HUGE_ALIGN;
        csi chunks = (csi)((bytes + chunk - 1) / chunk);
        int nt = vsdlss_parallel_width((double)bytes / 64.0);
        if (nt > chunks) nt = (int)chunks;
        if (nt < 1) nt = 1;
        (void)nt;
        VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static))
        for (csi c = 0; c < chunks; ++c) {
            size_t off = (size_t)c * chunk, len = bytes - off < chunk ? bytes - off : chunk;
            memset((unsigned char *)p + off, 0, len);
        }
    }
    *block = raw; return p;
}

typedef struct {
    csi *relmap;    /* n: local row of a global row in the current target */
    csi *stamp;     /* n: target that relmap currently describes */
    csi *rowmap;    /* max_rows */
    double *tile;   /* SCATTER_MC * SCATTER_NC */
} workspace;

typedef struct {
    vsdlss_sn_factor *f;
    const vsdlss_sn_symbolic *s;
    const vsdlss *lower;    /* transpose of A: column j holds rows i >= j */
} context;

static void workspace_free(workspace *w)
{
    if (!w) return;
    free(w->relmap); free(w->stamp); free(w->rowmap); free(w->tile);
    memset(w, 0, sizeof(*w));
}

static int workspace_init(workspace *w, csi n, csi max_rows)
{
    memset(w, 0, sizeof(*w));
    w->relmap = (csi *)malloc((size_t)n * sizeof(csi));
    w->stamp = (csi *)malloc((size_t)n * sizeof(csi));
    w->rowmap = (csi *)malloc((size_t)(max_rows ? max_rows : 1) * sizeof(csi));
    w->tile = (double *)malloc(SCATTER_MC * SCATTER_NC * sizeof(double));
    if (!w->relmap || !w->stamp || !w->rowmap || !w->tile) { workspace_free(w); return 0; }
    for (csi i = 0; i < n; ++i) w->stamp[i] = -1;
    return 1;
}

/* Rows [row_lo,row_hi) (relative to `first`) of the update of target d by
 * source block (sn, first, end).  rowmap holds the target-local rows. */
static void apply_rows(const context *c, workspace *w, csi sn, csi first,
                       csi end, csi d, csi row_lo, csi row_hi, int contiguous)
{
    const vsdlss_sn_factor *f = c->f;
    csi ws = f->column_start[sn + 1] - f->column_start[sn];
    csi rs = ws + f->row_ptr[sn + 1] - f->row_ptr[sn];
    csi kk = end - first;
    const double *src = f->panel + f->panel_offset[sn] + ws + first;
    csi bd = f->column_start[d];
    csi rd = f->column_start[d + 1] - bd + f->row_ptr[d + 1] - f->row_ptr[d];
    double *dst = f->panel + f->panel_offset[d];
    const csi *R = f->row_index + f->row_ptr[sn] + first;
    if (contiguous) {
        /* Source rows are one run of target rows: subtract in place. */
        csi c0 = R[0] - bd;
        vsdlss_gemm_nt_sub(row_hi - row_lo, kk, ws, src + row_lo, rs, src, rs,
                           dst + c0 * rd + w->rowmap[row_lo], rd, row_lo);
        return;
    }
    for (csi c0 = 0; c0 < kk; c0 += SCATTER_NC) {
        csi nc = kk - c0 < SCATTER_NC ? kk - c0 : SCATTER_NC;
        csi r = row_lo > c0 ? row_lo : c0;
        while (r < row_hi) {
            csi mc = row_hi - r < SCATTER_MC ? row_hi - r : SCATTER_MC;
            csi tri = r - c0;               /* keep entries with row >= col */
            memset(w->tile, 0, (size_t)mc * (size_t)nc * sizeof(double));
            vsdlss_gemm_nt_sub(mc, nc, ws, src + r, rs, src + c0, rs,
                               w->tile, mc, tri);
            for (csi j = 0; j < nc; ++j) {
                double *col = dst + (R[c0 + j] - bd) * rd;
                const double *t = w->tile + j * mc;
                csi ib = c0 + j > r ? c0 + j - r : 0;
                for (csi i = ib; i < mc; ++i) col[w->rowmap[r + i]] += t[i];
            }
            r += mc;
        }
    }
}

/* Assemble, update and factor target d.  `inner` allows the team to be
 * used inside this panel (phase 2 only). */
static vsdlss_status process_panel(const context *c, workspace *w, csi d, int inner)
{
    vsdlss_sn_factor *f = c->f;
    const vsdlss_sn_symbolic *s = c->s;
    csi bd = f->column_start[d], wd = f->column_start[d + 1] - bd;
    csi ext = f->row_ptr[d + 1] - f->row_ptr[d], rd = wd + ext;
    const csi *Rd = f->row_index + f->row_ptr[d];
    double *panel = f->panel + f->panel_offset[d];
    if (wd < 1 || ext < 0) return VSDLSS_ERR_INVALID;
    for (csi i = 0; i < wd; ++i) { w->relmap[bd + i] = i; w->stamp[bd + i] = d; }
    for (csi i = 0; i < ext; ++i) { w->relmap[Rd[i]] = wd + i; w->stamp[Rd[i]] = d; }

    /* Assemble A(:, J_d) (lower part) into the panel. */
    for (csi j = 0; j < wd; ++j) {
        const vsdlss *L = c->lower;
        double *col = panel + j * rd;
        for (csi p = L->p[bd + j]; p < L->p[bd + j + 1]; ++p) {
            csi i = L->i[p];
            if (w->stamp[i] != d || w->relmap[i] < j) return VSDLSS_ERR_INVALID;
            col[w->relmap[i]] += L->x[p];
            if (!isfinite(col[w->relmap[i]])) return VSDLSS_ERR_NONFINITE;
        }
    }

    /* Subtract the source blocks in increasing source order. */
    for (csi b = s->blk_ptr[d]; b < s->blk_ptr[d + 1]; ++b) {
        csi sn = s->blk_src[b], first = s->blk_first[b], end = s->blk_end[b];
        csi m = f->row_ptr[sn + 1] - f->row_ptr[sn] - first, kk = end - first;
        const csi *R = f->row_index + f->row_ptr[sn] + first;
        csi ws = f->column_start[sn + 1] - f->column_start[sn];
        if (sn >= d || kk < 1 || m < kk) return VSDLSS_ERR_INVALID;
        for (csi r = 0; r < m; ++r) {
            if (w->stamp[R[r]] != d) return VSDLSS_ERR_INVALID;
            w->rowmap[r] = w->relmap[R[r]];
        }
        if (R[kk - 1] >= bd + wd) return VSDLSS_ERR_INVALID;
        int contiguous = w->rowmap[m - 1] - w->rowmap[0] == m - 1;
        int nt = inner ? vsdlss_parallel_width((double)m * kk * ws / 4.0) : 1;
        const csi chunk = 4 * SCATTER_MC;
        csi chunks = m / chunk + (m % chunk != 0);
        if (nt > chunks) nt = (int)chunks;
        if (nt <= 1) {
            apply_rows(c, w, sn, first, end, d, 0, m, contiguous);
        } else {
            /* Row chunks write disjoint target rows. The shared workspace is
             * read-only here except for the per-thread scatter tile. */
            VSDLSS_OMP(omp parallel num_threads(nt))
            {
                workspace local = *w;
                double tile[SCATTER_MC * SCATTER_NC];
                local.tile = tile;
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(dynamic, 1))
                for (csi ch = 0; ch < chunks; ++ch) {
                    csi r0 = ch * chunk, r1 = r0 + chunk < m ? r0 + chunk : m;
                    apply_rows(c, &local, sn, first, end, d, r0, r1, contiguous);
                }
            }
        }
    }
    for (csi j = 0; j < wd; ++j)
        for (csi i = j; i < rd; ++i)
            if (!isfinite(panel[j * rd + i])) return VSDLSS_ERR_NONFINITE;
    return vsdlss_panel_factor(panel, rd, wd);
}

/* Supernodal-tree postorder with subtree work, used to split the tree. */
typedef struct {
    csi *order;      /* count: panels in postorder */
    csi *first;      /* count: first postorder position of each subtree */
    double *work;    /* count: subtree flop estimate */
} tree_info;

static void tree_free(tree_info *t)
{ free(t->order); free(t->first); free(t->work); }

static int tree_build(const vsdlss_sn_factor *f, const csi *sn_parent, int solve, tree_info *t)
{
    csi count = f->count, k = 0;
    csi *head = malloc((size_t)count * sizeof(csi)), *next = malloc((size_t)count * sizeof(csi));
    csi *stack = malloc((size_t)count * sizeof(csi)), *pos = malloc((size_t)count * sizeof(csi));
    memset(t, 0, sizeof(*t));
    t->order = malloc((size_t)count * sizeof(csi));
    t->first = malloc((size_t)count * sizeof(csi));
    t->work = malloc((size_t)count * sizeof(double));
    if (!head || !next || !stack || !pos || !t->order || !t->first || !t->work) {
        free(head); free(next); free(stack); free(pos); tree_free(t); return 0;
    }
    for (csi d = 0; d < count; ++d) {
        double w = (double)(f->column_start[d + 1] - f->column_start[d]);
        double e = (double)(f->row_ptr[d + 1] - f->row_ptr[d]);
        t->work[d] = solve ? w * (w + e) + 16 : w * w * w / 3 + w * w * e + w * e * e;
        head[d] = -1;
    }
    for (csi d = count - 1; d >= 0; --d)
        if (sn_parent[d] >= 0) { next[d] = head[sn_parent[d]]; head[sn_parent[d]] = d; }
    for (csi r = 0; r < count; ++r) {
        if (sn_parent[r] >= 0) continue;
        csi top = 0; stack[0] = r; pos[r] = -1;
        while (top >= 0) {
            csi v = stack[top];
            if (pos[v] == -1) { pos[v] = k; }          /* entering: record first */
            if (head[v] >= 0) {
                csi c = head[v]; head[v] = next[c];
                stack[++top] = c; pos[c] = -1;
            } else {
                t->first[v] = pos[v]; t->order[k++] = v; --top;
                if (sn_parent[v] >= 0) t->work[sn_parent[v]] += t->work[v];
            }
        }
    }
    free(head); free(next); free(stack); free(pos);
    return k == count;
}

static vsdlss_status factor_tree(const context *c, workspace *ws, int nt)
{
    vsdlss_sn_factor *f = c->f;
    const vsdlss_sn_symbolic *s = c->s;
    csi count = f->count, nsub = 0;
    tree_info t;
    csi *sub = NULL; char *in_sub = NULL;
    vsdlss_status st = VSDLSS_OK, *res = NULL;
    if (!tree_build(f, s->sn_parent, 0, &t)) return VSDLSS_ERR_OOM;
    sub = malloc((size_t)count * sizeof(csi));
    in_sub = calloc((size_t)count, 1);
    if (!sub || !in_sub) { st = VSDLSS_ERR_OOM; goto done; }
    /* Subtree roots: maximal subtrees with at most 1/(4 nt) of the work. */
    double total = 0;
    for (csi d = 0; d < count; ++d) if (s->sn_parent[d] < 0) total += t.work[d];
    double cap = total / (4.0 * nt);
    for (csi k = count - 1; k >= 0; --k) {
        csi d = t.order[k], p = s->sn_parent[d];
        if (in_sub[d]) continue;
        if (t.work[d] <= cap && (p < 0 || t.work[p] > cap)) {
            sub[nsub++] = d;
            for (csi q = t.first[d]; q <= k; ++q) in_sub[t.order[q]] = 1;
        }
    }
    /* Largest subtrees first for load balance (stable, deterministic). */
    for (csi a = 1; a < nsub; ++a) {
        csi v = sub[a], b = a;
        while (b > 0 && t.work[sub[b - 1]] < t.work[v]) { sub[b] = sub[b - 1]; --b; }
        sub[b] = v;
    }
    res = calloc((size_t)(nsub ? nsub : 1), sizeof(*res));
    if (!res) { st = VSDLSS_ERR_OOM; goto done; }
    if (nt > nsub) nt = (int)(nsub ? nsub : 1);
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        int id = 0;
#ifdef _OPENMP
        id = omp_get_thread_num();
#endif
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(dynamic, 1))
        for (csi i = 0; i < nsub; ++i) {
            csi root = sub[i], last = t.first[root];
            vsdlss_status r = VSDLSS_OK;
            for (csi k = last; r == VSDLSS_OK && t.order[k] != root; ++k)
                r = process_panel(c, ws + id, t.order[k], 0);
            if (r == VSDLSS_OK) r = process_panel(c, ws + id, root, 0);
            res[i] = r;
        }
    }
    for (csi i = 0; i < nsub; ++i) if (res[i] != VSDLSS_OK) { st = res[i]; goto done; }
    /* Phase 2: the top of the tree, children before parents. */
    for (csi k = 0; k < count && st == VSDLSS_OK; ++k)
        if (!in_sub[t.order[k]]) st = process_panel(c, ws, t.order[k], 1);
done:
    tree_free(&t); free(sub); free(in_sub); free(res);
    return st;
}

static vsdlss_status check_symbolic(const vsdlss *A, const vsdlss_sn_symbolic *s)
{
    if (!s || s->n != A->n || s->count < 1 || s->l_nnz < 1) return VSDLSS_ERR_INVALID;
    if (s->n == INT64_MAX || s->count == INT64_MAX || s->count > s->n) return VSDLSS_ERR_OOM;
    if (!s->column_start || !s->row_ptr || !s->panel_offset || !s->blk_ptr ||
        !s->sn_parent || (s->row_ptr[s->count] && !s->row_index) ||
        (s->blk_ptr[s->count] && (!s->blk_src || !s->blk_first || !s->blk_end)))
        return VSDLSS_ERR_INVALID;
    if (s->column_start[0] != 0 || s->column_start[s->count] != s->n ||
        s->row_ptr[0] != 0 || s->panel_offset[0] != 0 || s->blk_ptr[0] != 0)
        return VSDLSS_ERR_INVALID;
    for (csi d = 0; d < s->count; ++d) {
        csi w = s->column_start[d + 1] - s->column_start[d];
        csi e = s->row_ptr[d + 1] - s->row_ptr[d];
        const csi *R = s->row_index + s->row_ptr[d];
        if (w < 1 || e < 0 || e > s->n - s->column_start[d + 1] || e > s->max_rows ||
            w + e > s->max_rows ||
            s->panel_offset[d + 1] - s->panel_offset[d] != (w + e) * w ||
            s->blk_ptr[d + 1] < s->blk_ptr[d]) return VSDLSS_ERR_INVALID;
        for (csi i = 0; i < e; ++i)
            if (R[i] < s->column_start[d + 1] || R[i] >= s->n || (i && R[i] <= R[i - 1]))
                return VSDLSS_ERR_INVALID;
        for (csi b = s->blk_ptr[d]; b < s->blk_ptr[d + 1]; ++b) {
            csi sn = s->blk_src[b];
            if (sn < 0 || sn >= d || (b > s->blk_ptr[d] && sn <= s->blk_src[b - 1]) ||
                s->blk_first[b] < 0 || s->blk_end[b] <= s->blk_first[b] ||
                s->blk_end[b] > s->row_ptr[sn + 1] - s->row_ptr[sn])
                return VSDLSS_ERR_INVALID;
        }
        if (s->sn_parent[d] != -1 && (s->sn_parent[d] <= d || s->sn_parent[d] >= s->count))
            return VSDLSS_ERR_INVALID;
    }
    return VSDLSS_OK;
}

vsdlss_status vsdlss_sn_factorize(const vsdlss *A, const vsdlss_sn_symbolic *s,
                                  vsdlss_sn_factor **out)
{
    vsdlss_sn_factor *f = NULL; vsdlss *lower = NULL;
    workspace *ws = NULL; int nws = 0, nt;
    vsdlss_status st;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!A || A->m < 1 || A->n < 1 || A->m != A->n) return VSDLSS_ERR_INVALID;
    if (A->n == INT64_MAX) return VSDLSS_ERR_OOM;
    st = vsdlss_validate_upper_csc(A); if (st != VSDLSS_OK) return st;
    if (!s) return VSDLSS_ERR_INVALID;
    st = check_symbolic(A, s); if (st != VSDLSS_OK) return st;
    if (!bytes_ok(s->n + 1, sizeof(csi)) || !bytes_ok(s->count + 1, sizeof(csi)) ||
        !bytes_ok(s->panel_offset[s->count], sizeof(double)) ||
        !bytes_ok(s->row_ptr[s->count], sizeof(csi))) return VSDLSS_ERR_OOM;
    f = calloc(1, sizeof(*f)); if (!f) return VSDLSS_ERR_OOM;
    f->n = s->n; f->count = s->count; f->l_nnz = s->l_nnz;
    st = VSDLSS_ERR_OOM;
    f->column_start = copy_array(s->column_start, s->count + 1, sizeof(csi));
    f->row_ptr = copy_array(s->row_ptr, s->count + 1, sizeof(csi));
    f->row_index = copy_array(s->row_index, s->row_ptr[s->count], sizeof(csi));
    f->panel_offset = copy_array(s->panel_offset, s->count + 1, sizeof(csi));
    f->sn_parent = copy_array(s->sn_parent, s->count, sizeof(csi));
    f->blk_ptr = copy_array(s->blk_ptr, s->count + 1, sizeof(csi));
    f->blk_src = copy_array(s->blk_src, s->blk_ptr[s->count], sizeof(csi));
    f->blk_first = copy_array(s->blk_first, s->blk_ptr[s->count], sizeof(csi));
    f->blk_end = copy_array(s->blk_end, s->blk_ptr[s->count], sizeof(csi));
    f->panel = panel_alloc(s->panel_offset[s->count], &f->panel_block);
    lower = vsdlss_transpose(A, 1);
    if (!f->column_start || !f->row_ptr || !f->row_index || !f->panel_offset ||
        !f->panel || !lower || !f->sn_parent || !f->blk_ptr || !f->blk_src ||
        !f->blk_first || !f->blk_end) goto fail;

    nt = vsdlss_parallel_width((double)f->n * 256);
    if (nt > 1 && f->count < 2) nt = 1;
    nws = nt;
    ws = calloc((size_t)nws, sizeof(*ws));
    if (!ws) goto fail;
    for (int i = 0; i < nws; ++i) if (!workspace_init(ws + i, f->n, s->max_rows)) goto fail;
    {
        context c = { f, s, lower };
        if (nt > 1) st = factor_tree(&c, ws, nt);
        else {
            st = VSDLSS_OK;
            for (csi d = 0; d < f->count && st == VSDLSS_OK; ++d)
                st = process_panel(&c, ws, d, 1);
        }
    }
    if (st != VSDLSS_OK) goto fail;
    for (int i = 0; i < nws; ++i) workspace_free(ws + i);
    free(ws); vsdlss_spfree(lower);
    *out = f; return VSDLSS_OK;
fail:
    if (ws) for (int i = 0; i < nws; ++i) workspace_free(ws + i);
    free(ws); vsdlss_spfree(lower); vsdlss_sn_factor_free(f);
    return st;
}

/* Forward step of target d in pull form: subtract every source block that
 * lands in J_d (sources ascending, columns j ascending per entry), then
 * solve the diagonal block.  For each entry this is exactly the operation
 * sequence of the push form in vsdlss_panel_solve, so results are bitwise
 * identical to the serial solve. */
static int forward_pull(const vsdlss_sn_factor *f, csi d, double *x)
{
    csi bd = f->column_start[d], wd = f->column_start[d + 1] - bd;
    csi rd = wd + f->row_ptr[d + 1] - f->row_ptr[d];
    const double *a = f->panel + f->panel_offset[d];
    for (csi b = f->blk_ptr[d]; b < f->blk_ptr[d + 1]; ++b) {
        csi sn = f->blk_src[b], bs = f->column_start[sn], ws = f->column_start[sn + 1] - bs;
        csi rs = ws + f->row_ptr[sn + 1] - f->row_ptr[sn];
        const double *as = f->panel + f->panel_offset[sn] + ws;
        const csi *R = f->row_index + f->row_ptr[sn];
        for (csi r = f->blk_first[b]; r < f->blk_end[b]; ++r) {
            double v = x[R[r]];
            for (csi j = 0; j < ws; ++j) v -= as[j * rs + r] * x[bs + j];
            x[R[r]] = v;
            if (!isfinite(v)) return 2;
        }
    }
    for (csi j = 0; j < wd; ++j) {
        double dj = a[j * rd + j];
        if (!isfinite(dj) || dj <= 0) return 1;
        x[bd + j] /= dj;
        if (!isfinite(x[bd + j])) return 2;
        for (csi r = j + 1; r < wd; ++r) x[bd + r] -= a[j * rd + r] * x[bd + j];
    }
    return 0;
}

static int backward_panel(const vsdlss_sn_factor *f, csi sn, double *x)
{
    csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
    csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn];
    vsdlss_status st = vsdlss_panel_solve(f->panel + f->panel_offset[sn], b, w, ext,
        ext ? f->row_index + f->row_ptr[sn] : NULL, x, 1);
    return st == VSDLSS_OK ? 0 : st == VSDLSS_ERR_NONFINITE ? 2 : 1;
}

/* Tree-parallel solves: independent subtrees in parallel, the top of the
 * tree in order (forward: subtrees first; backward: top first). */
static vsdlss_status solve_tree(const vsdlss_sn_factor *f, double *x, int nt)
{
    csi count = f->count, nsub = 0;
    tree_info t; csi *sub = NULL; char *in_sub = NULL; int bad = 0;
    if (!tree_build(f, f->sn_parent, 1, &t)) return VSDLSS_ERR_OOM;
    sub = malloc((size_t)count * sizeof(csi));
    in_sub = calloc((size_t)count, 1);
    if (!sub || !in_sub) { tree_free(&t); free(sub); free(in_sub); return VSDLSS_ERR_OOM; }
    double total = 0;
    for (csi d = 0; d < count; ++d) if (f->sn_parent[d] < 0) total += t.work[d];
    double cap = total / (4.0 * nt);
    for (csi k = count - 1; k >= 0; --k) {
        csi d = t.order[k], p = f->sn_parent[d];
        if (in_sub[d]) continue;
        if (t.work[d] <= cap && (p < 0 || t.work[p] > cap)) {
            sub[nsub++] = d;
            for (csi q = t.first[d]; q <= k; ++q) in_sub[t.order[q]] = 1;
        }
    }
    if (nt > nsub) nt = (int)(nsub ? nsub : 1);
    (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(dynamic,1) reduction(|:bad))
    for (csi i = 0; i < nsub; ++i) {
        csi root = sub[i];
        for (csi k = t.first[root]; !bad && t.order[k] != root; ++k) bad |= forward_pull(f, t.order[k], x);
        if (!bad) bad |= forward_pull(f, root, x);
    }
    for (csi k = 0; k < count && !bad; ++k)
        if (!in_sub[t.order[k]]) bad |= forward_pull(f, t.order[k], x);
    for (csi k = count - 1; k >= 0 && !bad; --k)
        if (!in_sub[t.order[k]]) bad |= backward_panel(f, t.order[k], x);
    if (!bad) {
        VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(dynamic,1) reduction(|:bad))
        for (csi i = 0; i < nsub; ++i) {
            csi root = sub[i];
            /* The subtree is order[first[root] .. pos(root)] with the root
             * last; walk it backwards so parents precede children. */
            csi pos = t.first[root];
            while (t.order[pos] != root) ++pos;
            for (csi k = pos; !bad && k >= t.first[root]; --k) bad |= backward_panel(f, t.order[k], x);
        }
    }
    tree_free(&t); free(sub); free(in_sub);
    return bad == 0 ? VSDLSS_OK : (bad & 1) ? VSDLSS_ERR_INVALID : VSDLSS_ERR_NONFINITE;
}

vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *f, const double *rhs, double *out)
{
    double *x; csi sn, j;
    if (!f || !rhs || !out || f->n < 1 || !bytes_ok(f->n, sizeof(double))) return VSDLSS_ERR_INVALID;
    x = malloc((size_t)f->n * sizeof(*x)); if (!x) return VSDLSS_ERR_OOM;
    for (j = 0; j < f->n; j++) {
        if (!isfinite(rhs[j])) { free(x); return VSDLSS_ERR_NONFINITE; }
        x[j] = rhs[j];
    }
    {
        int nt = vsdlss_parallel_width((double)f->l_nnz * 2);
        if (nt > 1 && f->count > 1 && f->sn_parent && f->blk_ptr) {
            vsdlss_status st = solve_tree(f, x, nt);
            if (st != VSDLSS_OK) { free(x); return st; }
            for (j = 0; j < f->n; j++) if (!isfinite(x[j])) { free(x); return VSDLSS_ERR_NONFINITE; }
            memcpy(out, x, (size_t)f->n * sizeof(*x)); free(x); return VSDLSS_OK;
        }
    }
    for (int back = 0; back < 2; back++) for (csi t = 0; t < f->count; t++) {
        sn = back ? f->count - 1 - t : t;
        csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
        csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn];
        vsdlss_status st = vsdlss_panel_solve(f->panel + f->panel_offset[sn], b, w, ext,
            ext ? f->row_index + f->row_ptr[sn] : NULL, x, back);
        if (st != VSDLSS_OK) { free(x); return st; }
    }
    for (j = 0; j < f->n; j++) if (!isfinite(x[j])) { free(x); return VSDLSS_ERR_NONFINITE; }
    memcpy(out, x, (size_t)f->n * sizeof(*x)); free(x); return VSDLSS_OK;
}

/* Solve for nrhs right-hand sides stored column-major in x (leading
 * dimension ldx), in place.  Each panel is applied to every right-hand side
 * while it is hot in cache; per right-hand side the operations are exactly
 * those of vsdlss_sn_solve, so results are bitwise identical to it.  On
 * failure x may be partially updated; callers stage it. */
vsdlss_status vsdlss_sn_solve_batch(const vsdlss_sn_factor *f, csi nrhs, double *x, csi ldx)
{
    if (!f || !x || f->n < 1 || nrhs < 1 || ldx < f->n) return VSDLSS_ERR_INVALID;
    for (csi r = 0; r < nrhs; ++r)
        for (csi j = 0; j < f->n; ++j)
            if (!isfinite(x[r * ldx + j])) return VSDLSS_ERR_NONFINITE;
    for (int back = 0; back < 2; back++) for (csi t = 0; t < f->count; t++) {
        csi sn = back ? f->count - 1 - t : t;
        csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
        csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn];
        for (csi r = 0; r < nrhs; ++r) {
            vsdlss_status st = vsdlss_panel_solve(f->panel + f->panel_offset[sn], b, w, ext,
                ext ? f->row_index + f->row_ptr[sn] : NULL, x + r * ldx, back);
            if (st != VSDLSS_OK) return st;
        }
    }
    for (csi r = 0; r < nrhs; ++r)
        for (csi j = 0; j < f->n; ++j)
            if (!isfinite(x[r * ldx + j])) return VSDLSS_ERR_NONFINITE;
    return VSDLSS_OK;
}

/* CSC export of the stored lower trapezoids.  For relaxed layouts this
 * includes the amalgamation zeros. */
vsdlss_status vsdlss_sn_export_L(const vsdlss_sn_factor *f, vsdlss **out)
{
    vsdlss *L; csi nz = 0;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (!f || f->n < 1 || f->l_nnz < 1) return VSDLSS_ERR_INVALID;
    L = vsdlss_spalloc(f->n, f->n, f->l_nnz, 1, 0); if (!L) return VSDLSS_ERR_OOM;
    for (csi s = 0; s < f->count; ++s) {
        csi b = f->column_start[s], w = f->column_start[s + 1] - b;
        csi ext = f->row_ptr[s + 1] - f->row_ptr[s], rows = w + ext;
        const csi *R = f->row_index + f->row_ptr[s];
        const double *a = f->panel + f->panel_offset[s];
        for (csi j = 0; j < w; ++j) {
            L->p[b + j] = nz;
            for (csi i = j; i < rows; ++i) {
                if (nz >= f->l_nnz) { vsdlss_spfree(L); return VSDLSS_ERR_INVALID; }
                L->i[nz] = i < w ? b + i : R[i - w];
                L->x[nz++] = a[j * rows + i];
            }
        }
    }
    L->p[f->n] = nz;
    *out = L; return VSDLSS_OK;
}
