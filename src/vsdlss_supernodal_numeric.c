#define _DEFAULT_SOURCE
#include "vsdlss_m3_internal.h"
#include "vsdlss_dense.h"
#include "vsdlss_parallel.h"
#include "vsdlss_simd.h"
#include "vsdlss_ledger.h"

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

/* Optional BLAS/LAPACK for wide panels (build with -DVSDLSS_BLAS and link a
 * BLAS/LAPACK).  Source blocks at least VSDLSS_BLAS_MIN columns wide (default
 * 32; 0 disables) are applied with dgemm and panels at least that wide are
 * factored with dpotrf + dtrsm.  Results then depend on the BLAS library's
 * operation order: reproducible for one build and library, but not bitwise
 * equal to the built-in kernels.  Without VSDLSS_BLAS nothing changes. */
#ifdef VSDLSS_BLAS
void dgemm_(const char *, const char *, const int *, const int *, const int *,
            const double *, const double *, const int *, const double *,
            const int *, const double *, double *, const int *);
void dpotrf_(const char *, const int *, double *, const int *, int *);
void dtrsm_(const char *, const char *, const char *, const char *, const int *,
            const int *, const double *, const double *, const int *, double *,
            const int *);
static csi blas_min_width(void)
{
    const char *e = getenv("VSDLSS_BLAS_MIN");
    csi v = e ? (csi)atoll(e) : 32;
    return v > 0 ? v : 0;
}
/* C(m x n) = beta C - A(m x k) B(n x k)^T; dimensions fit int by the caller. */
static void blas_nt_sub(csi m, csi n, csi k, const double *A, csi lda,
                        const double *B, csi ldb, double *C, csi ldc, double beta)
{
    int M = (int)m, N = (int)n, K = (int)k, la = (int)lda, lb = (int)ldb, lc = (int)ldc;
    double alpha = -1.0;
#ifdef VSDLSS_BLAS_SERIALIZE   /* diagnostic: for BLAS builds that are not thread safe */
    VSDLSS_OMP(omp critical(vsdlss_blas))
#endif
    dgemm_("N", "T", &M, &N, &K, &alpha, A, &la, B, &lb, &beta, C, &lc);
}
#endif

static int bytes_ok(csi n, size_t z) { return n >= 0 && (uint64_t)n <= SIZE_MAX / z; }

static void *copy_array(const void *p, csi n, size_t z)
{
    void *q;
    if (!bytes_ok(n, z)) return NULL;
    q = malloc((size_t)(n ? n : 1) * z);
    if (q && n) memcpy(q, p, (size_t)n * z);
    return q;
}

/* 32-bit copy of n entries, all known to be in [-1, INT32_MAX]. */
static vsdlss_sni *copy_sni(const csi *p, csi n)
{
    vsdlss_sni *q;
    if (!bytes_ok(n, sizeof(vsdlss_sni))) return NULL;
    q = malloc((size_t)(n ? n : 1) * sizeof(vsdlss_sni));
    if (q) for (csi i = 0; i < n; ++i) q[i] = (vsdlss_sni)p[i];
    return q;
}

static void solve_tree_free(struct vsdlss_sn_solve_tree *st);

void vsdlss_sn_factor_free(vsdlss_sn_factor *f)
{
    if (!f) return;
    solve_tree_free(f->solve_tree);
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

/* Per-thread workspace.  The target panel is assembled, updated and
 * factored in `full` (column-major, leading dimension = its row count,
 * as the dense kernels expect), then packed into the factor.  Target rows
 * are found by merging sorted row lists (target_row), so no n-length map
 * per thread is needed. */
typedef struct {
    vsdlss_sni *rowmap; /* max_rows: target-local rows of the current source block */
    double *tile;       /* SCATTER_MC * SCATTER_NC */
    double *full;       /* full_cap: the current target panel, unpacked */
    size_t full_cap;
} workspace;

typedef struct {
    vsdlss_sn_factor *f;    /* layout and source blocks copied from the symbolic analysis */
    const vsdlss *lower;    /* transpose of A: column j holds rows i >= j */
    csi blas_min;           /* 0: built-in kernels only */
} context;

static void workspace_free(workspace *w)
{
    if (!w) return;
    free(w->rowmap); free(w->tile); free(w->full);
    memset(w, 0, sizeof(*w));
}

static int workspace_init(workspace *w, csi max_rows)
{
    memset(w, 0, sizeof(*w));
    w->rowmap = (vsdlss_sni *)malloc((size_t)(max_rows ? max_rows : 1) * sizeof(vsdlss_sni));
    w->tile = (double *)malloc(SCATTER_MC * SCATTER_NC * sizeof(double));
    if (!w->rowmap || !w->tile) { workspace_free(w); return 0; }
    return 1;
}

/* Local row of global row i in the target with columns [bd, bd+wd) and
 * external rows Rd[0..ext) (ascending): i - bd for its own columns, wd + p
 * for i == Rd[p], -1 when i is not a row of the target.  Callers look rows
 * up in ascending order, so the search in Rd resumes at *pos. */
static csi target_row(csi i, csi bd, csi wd, const vsdlss_sni *Rd, csi ext, csi *pos)
{
    if (i < bd) return -1;
    if (i < bd + wd) return i - bd;
    csi p = *pos;
    if (p < ext && Rd[p] < i) {
        if (p + 1 < ext && Rd[p + 1] >= i) ++p;          /* next row: the usual case */
        else {
            csi lo = p + 1, hi = ext;
            while (lo < hi) { csi mid = lo + (hi - lo) / 2; if (Rd[mid] < i) lo = mid + 1; else hi = mid; }
            p = lo;
        }
    }
    *pos = p;
    return p < ext && Rd[p] == i ? wd + p : -1;
}

/* Rows [row_lo,row_hi) (relative to `first`) of the update of target d by
 * source block (sn, first, end).  rowmap holds the target-local rows. */
static void apply_rows(const context *c, workspace *w, csi sn, csi first,
                       csi end, csi d, csi row_lo, csi row_hi, int contiguous)
{
    const vsdlss_sn_factor *f = c->f;
    csi ws = f->column_start[sn + 1] - f->column_start[sn];
    csi rs = f->row_ptr[sn + 1] - f->row_ptr[sn];      /* leading dimension of the packed external rows */
    csi kk = end - first;
    const double *src = f->panel + f->panel_offset[sn] + ws * (ws + 1) / 2 + first;
    csi bd = f->column_start[d];
    csi rd = f->column_start[d + 1] - bd + f->row_ptr[d + 1] - f->row_ptr[d];
    double *dst = w->full;                             /* target d, unpacked */
    const vsdlss_sni *R = f->row_index + f->row_ptr[sn] + first;
#ifdef VSDLSS_BLAS
    const int blas = c->blas_min && ws >= c->blas_min;
#endif
    if (contiguous) {
        /* Source rows are one run of target rows: subtract in place. */
        csi c0 = R[0] - bd;
#ifdef VSDLSS_BLAS
        /* dgemm also updates the entries above the diagonal of the target's
         * diagonal block; nothing reads that strictly upper triangle. */
        if (blas) {
            blas_nt_sub(row_hi - row_lo, kk, ws, src + row_lo, rs, src, rs,
                        dst + c0 * rd + w->rowmap[row_lo], rd, 1.0);
            return;
        }
#endif
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
#ifdef VSDLSS_BLAS
            if (blas)                       /* whole tile; the scatter keeps row >= col */
                blas_nt_sub(mc, nc, ws, src + r, rs, src + c0, rs, w->tile, mc, 0.0);
            else
#endif
            {
                memset(w->tile, 0, (size_t)mc * (size_t)nc * sizeof(double));
                vsdlss_gemm_nt_sub(mc, nc, ws, src + r, rs, src + c0, rs,
                                   w->tile, mc, tri);
            }
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
static vsdlss_status process_panel_full(const context *c, workspace *w, csi d, int inner)
{
    vsdlss_sn_factor *f = c->f;
    csi bd = f->column_start[d], wd = f->column_start[d + 1] - bd;
    csi ext = f->row_ptr[d + 1] - f->row_ptr[d], rd = wd + ext;
    const vsdlss_sni *Rd = f->row_index + f->row_ptr[d];
    if (wd < 1 || ext < 0) return VSDLSS_ERR_INVALID;
    const size_t need = (size_t)rd * (size_t)wd;
    if (need > w->full_cap) {
        free(w->full);
        w->full = (double *)malloc(need * sizeof(double));
        w->full_cap = w->full ? need : 0;
        if (!w->full) return VSDLSS_ERR_OOM;
    }
    double *panel = w->full;
    memset(panel, 0, need * sizeof(double));

    /* Assemble A(:, J_d) (lower part) into the panel. */
    for (csi j = 0; j < wd; ++j) {
        const vsdlss *L = c->lower;
        double *col = panel + j * rd;
        csi pos = 0;
        for (csi p = L->p[bd + j]; p < L->p[bd + j + 1]; ++p) {
            csi i = target_row(L->i[p], bd, wd, Rd, ext, &pos);
            if (i < j) return VSDLSS_ERR_INVALID;          /* also i == -1: not a row of d */
            col[i] += L->x[p];
            if (!isfinite(col[i])) return VSDLSS_ERR_NONFINITE;
        }
    }

    /* Subtract the source blocks in increasing source order. */
    for (csi b = f->blk_ptr[d]; b < f->blk_ptr[d + 1]; ++b) {
        csi sn = f->blk_src[b], first = f->blk_first[b], end = f->blk_end[b];
        csi m = f->row_ptr[sn + 1] - f->row_ptr[sn] - first, kk = end - first;
        const vsdlss_sni *R = f->row_index + f->row_ptr[sn] + first;
        csi ws = f->column_start[sn + 1] - f->column_start[sn];
        if (sn >= d || kk < 1 || m < kk) return VSDLSS_ERR_INVALID;
        csi pos = 0;
        for (csi r = 0; r < m; ++r) {
            csi i = target_row(R[r], bd, wd, Rd, ext, &pos);
            if (i < 0) return VSDLSS_ERR_INVALID;
            w->rowmap[r] = (vsdlss_sni)i;
        }
        if (R[kk - 1] >= bd + wd) return VSDLSS_ERR_INVALID;
        int contiguous = w->rowmap[m - 1] - w->rowmap[0] == m - 1;
        int nt = inner ? vsdlss_parallel_width((double)m * kk * ws / 4.0) : 1;
        const csi chunk = 4 * SCATTER_MC;
        csi chunks = m / chunk + (m % chunk != 0);
        if (nt > chunks) nt = (int)chunks;
#ifdef VSDLSS_BLAS
        /* BLAS kernels may round differently depending on where a row falls
         * in their register tiles, so a BLAS block is always cut at the same
         * chunk boundaries, whether the chunks then run on one thread or on
         * several: the factor stays the same for every thread count. */
        if (nt <= 1 && c->blas_min && ws >= c->blas_min) {
            for (csi r0 = 0; r0 < m; r0 += chunk)
                apply_rows(c, w, sn, first, end, d, r0, r0 + chunk < m ? r0 + chunk : m, contiguous);
        } else
#endif
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
#ifdef VSDLSS_BLAS
    if (c->blas_min && wd >= c->blas_min) {
        int W = (int)wd, E = (int)ext, LD = (int)rd, info = 0; double one = 1.0;
#ifdef VSDLSS_BLAS_SERIALIZE
        VSDLSS_OMP(omp critical(vsdlss_blas))
#endif
        {
            dpotrf_("L", &W, panel, &LD, &info);
            if (info == 0 && E > 0) dtrsm_("R", "L", "T", "N", &E, &W, &one, panel, &LD, panel + wd, &LD);
        }
        if (info != 0) return VSDLSS_ERR_NOT_POSDEF;
        for (csi j = 0; j < wd; ++j)
            for (csi i = j; i < rd; ++i)
                if (!isfinite(panel[j * rd + i])) return VSDLSS_ERR_NONFINITE;
        return VSDLSS_OK;
    }
#endif
    return vsdlss_panel_factor(panel, rd, wd);
}

/* Target d in the workspace's full panel, then packed into the factor. */
static vsdlss_status process_panel(const context *c, workspace *w, csi d, int inner)
{
    vsdlss_status st = process_panel_full(c, w, d, inner);
    if (st != VSDLSS_OK) return st;
    vsdlss_sn_factor *f = c->f;
    const csi wd = f->column_start[d + 1] - f->column_start[d];
    const csi ext = f->row_ptr[d + 1] - f->row_ptr[d], rd = wd + ext;
    const double *full = w->full;
    double *p = f->panel + f->panel_offset[d], *rect = p + wd * (wd + 1) / 2;
    for (csi j = 0; j < wd; ++j) {
        memcpy(p + VSDLSS_SN_DCOL(j, wd), full + j * rd + j, (size_t)(wd - j) * sizeof(double));
        if (ext) memcpy(rect + j * ext, full + j * rd + wd, (size_t)ext * sizeof(double));
    }
    return VSDLSS_OK;
}

/* Supernodal-tree postorder with subtree work, used to split the tree. */
typedef struct {
    csi *order;      /* count: panels in postorder */
    csi *first;      /* count: first postorder position of each subtree */
    double *work;    /* count: subtree flop estimate */
} tree_info;

static void tree_free(tree_info *t)
{ free(t->order); free(t->first); free(t->work); }

/* The tree comes from the symbolic layout (64-bit parents) during the
 * factorization and from the factor (32-bit) for the solves. */
#define PARENT(d) (p64 ? p64[d] : (csi)p32[d])
static int tree_build(const vsdlss_sn_factor *f, const csi *p64, const vsdlss_sni *p32, int solve, tree_info *t)
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
        if (PARENT(d) >= 0) { next[d] = head[PARENT(d)]; head[PARENT(d)] = d; }
    for (csi r = 0; r < count; ++r) {
        if (PARENT(r) >= 0) continue;
        csi top = 0; stack[0] = r; pos[r] = -1;
        while (top >= 0) {
            csi v = stack[top];
            if (pos[v] == -1) { pos[v] = k; }          /* entering: record first */
            if (head[v] >= 0) {
                csi c = head[v]; head[v] = next[c];
                stack[++top] = c; pos[c] = -1;
            } else {
                t->first[v] = pos[v]; t->order[k++] = v; --top;
                if (PARENT(v) >= 0) t->work[PARENT(v)] += t->work[v];
            }
        }
    }
    free(head); free(next); free(stack); free(pos);
    return k == count;
}
#undef PARENT

/* ---- Cached solve tree --------------------------------------------------
 * The solve postorder depends only on the factor; the subtree split depends
 * on the thread count as well.  Both are built once: the tree when the factor
 * is made, a split the first time a solve uses that thread count (published
 * with a compare-and-swap, so concurrent solves on one factor stay safe). */
typedef struct {
    csi nsub;
    csi *sub;        /* nsub subtree roots */
    csi *sub_end;    /* postorder position of each root */
    char *in_sub;    /* count: panel belongs to a split subtree */
    /* Push-form forward in the subtrees (see forward_push): the panels of
     * subtree i are mem[moff[i] .. moff[i+1]) in ascending index, and the
     * first ein[m] external rows of panel mem[m] lie inside its subtree.
     * NULL when the factor does not satisfy the preconditions (pull form). */
    csi *moff;
    vsdlss_sni *mem, *ein;
} sn_split;

/* Forward tree-top plan for one thread count (VSDLSS_FWD_TOP_PLAN=0
 * disables it): the postorder positions of the team targets, and for each
 * of them and each thread of a team of T the non-empty source-block row
 * ranges [s, e) that forward_pull_team would otherwise find with two binary
 * searches per (thread, block) on every solve.  It depends only on the
 * factor, the split and T, never on the right-hand side; ranges are kept in
 * ascending block order, so the subtractions and their order are unchanged
 * (same bits).  A solve whose actual team size differs from T computes the
 * ranges as before. */
typedef struct { csi b, s, e; } top_range;
typedef struct {
    csi nbig, *bigk;     /* team targets: positions in the postorder */
    int T;               /* team size the ranges were cut for */
    csi *roff;           /* nbig*T + 1: ranges of (target i, thread t) */
    top_range *rg;
} top_plan;

#define SN_SPLIT_SLOTS 257
struct vsdlss_sn_solve_tree {
    tree_info t;
    _Atomic(sn_split *) split[SN_SPLIT_SLOTS];   /* indexed by thread count */
    _Atomic(top_plan *) plan[SN_SPLIT_SLOTS];    /* indexed by thread count */
};

static void split_free(sn_split *s)
{ if (s) { free(s->sub); free(s->sub_end); free(s->in_sub); free(s->moff); free(s->mem); free(s->ein); free(s); } }

static void top_plan_free(top_plan *p)
{ if (p) { free(p->bigk); free(p->roff); free(p->rg); free(p); } }

static void solve_tree_free(struct vsdlss_sn_solve_tree *st)
{
    if (!st) return;
    tree_free(&st->t);
    for (int i = 0; i < SN_SPLIT_SLOTS; ++i) {
        split_free(atomic_load(&st->split[i]));
        top_plan_free(atomic_load(&st->plan[i]));
    }
    free(st);
}

static struct vsdlss_sn_solve_tree *solve_tree_new(const vsdlss_sn_factor *f)
{
    struct vsdlss_sn_solve_tree *st = malloc(sizeof(*st));
    if (!st) return NULL;
    for (int i = 0; i < SN_SPLIT_SLOTS; ++i) { atomic_init(&st->split[i], NULL); atomic_init(&st->plan[i], NULL); }
    if (!tree_build(f, NULL, f->sn_parent, 1, &st->t)) { free(st); return NULL; }
    return st;
}

static int cmp_sni(const void *a, const void *b)
{ vsdlss_sni x = *(const vsdlss_sni *)a, y = *(const vsdlss_sni *)b; return (x > y) - (x < y); }

/* Push-form data for the subtrees of s (see sn_split and forward_push).
 * A subtree's columns end at its root's last column, and a panel's external
 * rows ascend, so the rows inside the subtree are a prefix of each member's
 * row list.  The block lists are checked against that: every source block
 * of an in-subtree target comes from the same subtree and lies in the
 * source's prefix, and those blocks cover the prefix exactly.  Otherwise
 * the split keeps the pull form.  Built once per split (thread count). */
static void split_push_build(const vsdlss_sn_factor *f, const tree_info *t, sn_split *s)
{
    const csi count = f->count, nsub = s->nsub;
    csi total = 0, m = 0;
    for (csi i = 0; i < nsub; ++i) total += s->sub_end[i] - t->first[s->sub[i]] + 1;
    csi *moff = malloc((size_t)(nsub + 1) * sizeof(csi));
    vsdlss_sni *mem = malloc((size_t)(total ? total : 1) * sizeof(vsdlss_sni));
    vsdlss_sni *ein = malloc((size_t)(total ? total : 1) * sizeof(vsdlss_sni));
    vsdlss_sni *sid = malloc((size_t)count * sizeof(vsdlss_sni));   /* subtree of a panel, -1: top */
    csi *pe = malloc((size_t)count * sizeof(csi)), *cov = calloc((size_t)count, sizeof(csi));
    if (!moff || !mem || !ein || !sid || !pe || !cov) goto fail;
    for (csi d = 0; d < count; ++d) sid[d] = -1;
    for (csi i = 0; i < nsub; ++i) {
        const csi root = s->sub[i], end_col = f->column_start[root + 1];
        moff[i] = m;
        for (csi k = t->first[root]; k <= s->sub_end[i]; ++k) {
            const csi d = t->order[k];
            const vsdlss_sni *R = f->row_index + f->row_ptr[d];
            csi lo = 0, hi = f->row_ptr[d + 1] - f->row_ptr[d];
            while (lo < hi) { csi mid = lo + (hi - lo) / 2; if (R[mid] < end_col) lo = mid + 1; else hi = mid; }
            mem[m++] = (vsdlss_sni)d; sid[d] = (vsdlss_sni)i; pe[d] = lo;
        }
        qsort(mem + moff[i], (size_t)(m - moff[i]), sizeof(vsdlss_sni), cmp_sni);
    }
    moff[nsub] = m;
    for (csi q = 0; q < m; ++q) {
        const csi d = mem[q];
        for (csi b = f->blk_ptr[d]; b < f->blk_ptr[d + 1]; ++b) {
            const csi sn = f->blk_src[b];
            if (sid[sn] != sid[d] || f->blk_end[b] > pe[sn]) goto fail;
            cov[sn] += f->blk_end[b] - f->blk_first[b];
        }
    }
    for (csi q = 0; q < m; ++q) {
        const csi d = mem[q];
        if (cov[d] != pe[d]) goto fail;
        ein[q] = (vsdlss_sni)pe[d];
    }
    s->moff = moff; s->mem = mem; s->ein = ein;
    free(sid); free(pe); free(cov);
    return;
fail:
    free(moff); free(mem); free(ein); free(sid); free(pe); free(cov);
}

/* Maximal subtrees with at most 1/(4 nt) of the solve work. */
static sn_split *split_build(const vsdlss_sn_factor *f, const tree_info *t, int nt)
{
    csi count = f->count;
    sn_split *s = calloc(1, sizeof(*s));
    if (!s) return NULL;
    s->sub = malloc((size_t)count * sizeof(csi));
    s->sub_end = malloc((size_t)count * sizeof(csi));
    s->in_sub = calloc((size_t)count, 1);
    if (!s->sub || !s->sub_end || !s->in_sub) { split_free(s); return NULL; }
    double total = 0;
    for (csi d = 0; d < count; ++d) if (f->sn_parent[d] < 0) total += t->work[d];
    double cap = total / (4.0 * nt);
    for (csi k = count - 1; k >= 0; --k) {
        csi d = t->order[k], p = f->sn_parent[d];
        if (s->in_sub[d]) continue;
        if (t->work[d] <= cap && (p < 0 || t->work[p] > cap)) {
            s->sub[s->nsub] = d; s->sub_end[s->nsub] = k; s->nsub++;
            for (csi q = t->first[d]; q <= k; ++q) s->in_sub[t->order[q]] = 1;
        }
    }
    /* Splits are cached per thread count: keep only the nsub roots. */
    csi *sub = realloc(s->sub, (size_t)(s->nsub ? s->nsub : 1) * sizeof(csi));
    csi *sub_end = realloc(s->sub_end, (size_t)(s->nsub ? s->nsub : 1) * sizeof(csi));
    if (sub) s->sub = sub;
    if (sub_end) s->sub_end = sub_end;
    split_push_build(f, t, s);   /* optional: pull form without it */
    return s;
}

static vsdlss_status factor_tree(const context *c, workspace *ws, int nt)
{
    vsdlss_sn_factor *f = c->f;
    csi count = f->count, nsub = 0;
    tree_info t;
    csi *sub = NULL; char *in_sub = NULL;
    vsdlss_status st = VSDLSS_OK, *res = NULL;
    if (!tree_build(f, NULL, f->sn_parent, 0, &t)) return VSDLSS_ERR_OOM;
    sub = malloc((size_t)count * sizeof(csi));
    in_sub = calloc((size_t)count, 1);
    if (!sub || !in_sub) { st = VSDLSS_ERR_OOM; goto done; }
    /* Subtree roots: maximal subtrees with at most 1/(4 nt) of the work. */
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

/* A_own / s_own (optional): A and s are the caller's *A_own / *s_own, freed
 * once the factor has copied what it needs, before L is allocated (peak
 * memory); set to NULL on return. */
static vsdlss_status sn_factorize(const vsdlss *A, const vsdlss_sn_symbolic *s,
                                  vsdlss_sn_factor **out, vsdlss **A_own,
                                  vsdlss_sn_symbolic **s_own)
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
    /* Row and supernode indices are stored in 32 bits. */
    if (s->n > INT32_MAX) return VSDLSS_ERR_UNSUPPORTED;
    if (!bytes_ok(s->n + 1, sizeof(csi)) || !bytes_ok(s->count + 1, sizeof(csi)) ||
        !bytes_ok(s->l_nnz, sizeof(double)) ||
        !bytes_ok(s->row_ptr[s->count], sizeof(csi))) return VSDLSS_ERR_OOM;
    f = calloc(1, sizeof(*f)); if (!f) return VSDLSS_ERR_OOM;
    f->n = s->n; f->count = s->count; f->l_nnz = s->l_nnz;
    st = VSDLSS_ERR_OOM;
    f->column_start = copy_sni(s->column_start, s->count + 1);
    f->row_ptr = copy_array(s->row_ptr, s->count + 1, sizeof(csi));
    f->row_index = copy_sni(s->row_index, s->row_ptr[s->count]);
    f->panel_offset = malloc((size_t)(s->count + 1) * sizeof(csi));
    f->sn_parent = copy_sni(s->sn_parent, s->count);
    f->blk_ptr = copy_array(s->blk_ptr, s->count + 1, sizeof(csi));
    f->blk_src = copy_sni(s->blk_src, s->blk_ptr[s->count]);
    f->blk_first = copy_sni(s->blk_first, s->blk_ptr[s->count]);
    f->blk_end = copy_sni(s->blk_end, s->blk_ptr[s->count]);
    if (f->panel_offset) {
        /* Packed panels: the lower trapezoid only (see vsdlss_sn_factor). */
        f->panel_offset[0] = 0;
        for (csi d = 0; d < s->count; ++d) {
            csi w = s->column_start[d + 1] - s->column_start[d], e = s->row_ptr[d + 1] - s->row_ptr[d];
            f->panel_offset[d + 1] = f->panel_offset[d] + w * (w + 1) / 2 + w * e;
        }
        if (f->panel_offset[s->count] != s->l_nnz) { st = VSDLSS_ERR_INVALID; goto fail; }
    }
    lower = vsdlss_transpose(A, 1);
    if (!f->column_start || !f->row_ptr || !f->row_index || !f->panel_offset ||
        !lower || !f->sn_parent || !f->blk_ptr || !f->blk_src ||
        !f->blk_first || !f->blk_end) goto fail;
    const csi max_rows = s->max_rows;
    if (s_own) { vsdlss_sn_symbolic_free(*s_own); *s_own = NULL; }
    if (A_own) { vsdlss_spfree(*A_own); *A_own = NULL; }
    if (s_own || A_own) vsdlss_release_free_memory(2);   /* hand the inputs back before L */
    s = NULL; A = NULL;
    f->panel = panel_alloc(f->panel_offset[f->count], &f->panel_block);
    if (!f->panel) goto fail;

    nt = vsdlss_parallel_width((double)f->n * 256);
    if (nt > 1 && f->count < 2) nt = 1;
    nws = nt;
    ws = calloc((size_t)nws, sizeof(*ws));
    if (!ws) goto fail;
    for (int i = 0; i < nws; ++i) if (!workspace_init(ws + i, max_rows)) goto fail;
    {
        context c = { f, lower, 0 };
#ifdef VSDLSS_BLAS
        if (max_rows < INT_MAX) c.blas_min = blas_min_width();
#endif
        if (nt > 1) st = factor_tree(&c, ws, nt);
        else {
            st = VSDLSS_OK;
            for (csi d = 0; d < f->count && st == VSDLSS_OK; ++d)
                st = process_panel(&c, ws, d, 1);
        }
    }
    if (st != VSDLSS_OK) goto fail;
    for (int i = 0; i < nws; ++i) workspace_free(ws + i);
    free(ws); ws = NULL; nws = 0; vsdlss_spfree(lower); lower = NULL;
    if (f->count > 1 && !(f->solve_tree = solve_tree_new(f))) { st = VSDLSS_ERR_OOM; goto fail; }
    *out = f; return VSDLSS_OK;
fail:
    if (ws) for (int i = 0; i < nws; ++i) workspace_free(ws + i);
    free(ws); vsdlss_spfree(lower); vsdlss_sn_factor_free(f);
    return st;
}

vsdlss_status vsdlss_sn_factorize(const vsdlss *A, const vsdlss_sn_symbolic *s,
                                  vsdlss_sn_factor **out)
{ return sn_factorize(A, s, out, NULL, NULL); }

vsdlss_status vsdlss_sn_factorize_consume(vsdlss **A, vsdlss_sn_symbolic **s,
                                          vsdlss_sn_factor **out)
{
    vsdlss_status st = sn_factorize(A ? *A : NULL, s ? *s : NULL, out, A, s);
    if (A) { vsdlss_spfree(*A); *A = NULL; }
    if (s) { vsdlss_sn_symbolic_free(*s); *s = NULL; }
    return st;
}

/* Forward step of target d in pull form: subtract every source block that
 * lands in J_d (sources ascending, columns j ascending per entry), then
 * solve the diagonal block.  For each entry this is exactly the operation
 * sequence of the push form in vsdlss_panel_solve, so results are bitwise
 * identical to the serial solve.
 *
 * The block update runs column-outer so the panel is read contiguously; an
 * entry x[R[r]] still receives its subtractions in ascending j (R holds rows
 * below the source's columns, so no x[bs + j] is among the targets), and each
 * subtraction rounds to double either way, so the result is unchanged.
 * Entries are not checked one by one: a non-finite value stays non-finite
 * through every later subtraction and division, so the caller's single scan
 * of the result reports it. */
static int forward_pull(const vsdlss_sn_factor *f, csi d, double *x)
{
    csi bd = f->column_start[d], wd = f->column_start[d + 1] - bd;
    const double *a = f->panel + f->panel_offset[d];
    const int simd = vsdlss_simd_enabled();
    for (csi b = f->blk_ptr[d]; b < f->blk_ptr[d + 1]; ++b) {
        csi sn = f->blk_src[b], bs = f->column_start[sn], ws = f->column_start[sn + 1] - bs;
        csi rs = f->row_ptr[sn + 1] - f->row_ptr[sn];
        const double *as = f->panel + f->panel_offset[sn] + ws * (ws + 1) / 2;
        const vsdlss_sni *R = f->row_index + f->row_ptr[sn];
        const csi r0 = f->blk_first[b], r1 = f->blk_end[b];
        if (simd) { vsdlss_simd_block_update(as, rs, ws, x + bs, R, r0, r1, x); continue; }
        for (csi j = 0; j < ws; ++j) {
            const double xj = x[bs + j], *col = as + j * rs;
            for (csi r = r0; r < r1; ++r) x[R[r]] -= col[r] * xj;
        }
    }
    for (csi j = 0; j < wd; ++j) {
        const double *aj = a + VSDLSS_SN_DCOL(j, wd);
        double dj = aj[0];
        if (!isfinite(dj) || dj <= 0) return 1;
        x[bd + j] /= dj;
        if (simd) vsdlss_simd_axpy_neg(x + bd + j + 1, aj + 1, x[bd + j], wd - j - 1);
        else for (csi r = j + 1; r < wd; ++r) x[bd + r] -= aj[r - j] * x[bd + j];
    }
    return 0;
}

/* Forward step of panel s in push form, inside a split subtree: solve the
 * diagonal block (exactly as forward_pull does), then subtract its columns
 * from the first ein external rows, the ones inside the subtree.  Rows past
 * ein belong to tree-top targets, which pull them later as before.  Panels
 * of a subtree are pushed in ascending index, so each in-subtree entry gets
 * its sources' subtractions in ascending source order with the same
 * per-entry operations as forward_pull's block update: the same bits as the
 * pull form and the serial solve.  One update over the source's whole
 * prefix replaces one per target block (power-grid blocks average about
 * four rows), and the source panel is read once, front to back. */
static int forward_push(const vsdlss_sn_factor *f, csi s, csi ein, double *x)
{
    const csi bs = f->column_start[s], ws = f->column_start[s + 1] - bs;
    const double *a = f->panel + f->panel_offset[s];
    const int simd = vsdlss_simd_enabled();
    for (csi j = 0; j < ws; ++j) {
        const double *aj = a + VSDLSS_SN_DCOL(j, ws);
        double dj = aj[0];
        if (!isfinite(dj) || dj <= 0) return 1;
        x[bs + j] /= dj;
        if (simd) vsdlss_simd_axpy_neg(x + bs + j + 1, aj + 1, x[bs + j], ws - j - 1);
        else for (csi r = j + 1; r < ws; ++r) x[bs + r] -= aj[r - j] * x[bs + j];
    }
    if (ein > 0) {
        const csi rs = f->row_ptr[s + 1] - f->row_ptr[s];
        const double *as = a + ws * (ws + 1) / 2;
        const vsdlss_sni *R = f->row_index + f->row_ptr[s];
        if (simd) vsdlss_simd_block_update(as, rs, ws, x + bs, R, 0, ein, x);
        else for (csi j = 0; j < ws; ++j) {
            const double xj = x[bs + j], *col = as + j * rs;
            for (csi r = 0; r < ein; ++r) x[R[r]] -= col[r] * xj;
        }
    }
    return 0;
}

/* VSDLSS_FWD_SUB_PUSH=0: pull form in the subtrees too (A/B; same bits). */
static int sub_push_on(void)
{
    static int v = -1;
    if (v < 0) { const char *e = getenv("VSDLSS_FWD_SUB_PUSH"); v = !(e && e[0] == '0'); }
    return v;
}

/* Work of forward_pull(d) in multiply-adds (source-block updates plus the
 * diagonal triangle), used to decide whether a tree-top target is worth a
 * thread team. */
static double forward_pull_work(const vsdlss_sn_factor *f, csi d)
{
    double wd = (double)(f->column_start[d + 1] - f->column_start[d]), w = wd * wd / 2;
    for (csi b = f->blk_ptr[d]; b < f->blk_ptr[d + 1]; ++b) {
        csi sn = f->blk_src[b];
        w += (double)(f->blk_end[b] - f->blk_first[b]) *
             (double)(f->column_start[sn + 1] - f->column_start[sn]);
    }
    return w;
}

/* First position p in [lo, hi) with R[p] >= key (R ascending). */
static csi lower_bound_csi(const vsdlss_sni *R, csi lo, csi hi, csi key)
{
    while (lo < hi) { csi mid = lo + (hi - lo) / 2; if (R[mid] < key) lo = mid + 1; else hi = mid; }
    return lo;
}

/* forward_pull for a large tree-top target on a thread team.  The target's
 * columns are split into contiguous ranges, one per thread; every thread
 * applies all source blocks in the same ascending order, restricted to the
 * rows in its range.  The diagonal triangle is solved in column blocks: the
 * block itself serially, the rows below it split among the threads.  Each
 * entry therefore receives exactly the subtractions and division of
 * forward_pull, in the same order: the result is bitwise the same. */
/* Tuning (defaults measured on the power-grid cases; any value gives the
 * same bits): VSDLSS_FWD_TOP_BLK columns per serial triangle block of a
 * tree-top target, VSDLSS_FWD_TOP_WORK multiply-adds for a target to get a
 * team. */
static csi fwd_top_blk(void)
{
    static csi v = -1;
    if (v < 0) { const char *e = getenv("VSDLSS_FWD_TOP_BLK"); v = e && atoll(e) > 0 ? (csi)atoll(e) : 64; }
    return v;
}
static double fwd_top_work(void)
{
    static double v = -1;
    if (v < 0) { const char *e = getenv("VSDLSS_FWD_TOP_WORK"); v = e && atof(e) > 0 ? atof(e) : 65536.0; }
    return v;
}
#define FWD_TOP_BLK fwd_top_blk()
#define FWD_TOP_PAR_WORK fwd_top_work()   /* below this a team costs more than it saves */
/* Called by every thread (tid of T) of an existing team: it contains team
 * barriers.  *bad is set by the thread solving a triangle block. */
/* Source block b restricted to its row positions [s, e) (s < e). */
static inline void pull_block_rows(const vsdlss_sn_factor *f, csi b, csi s, csi e, double *x, int simd)
{
    csi sn = f->blk_src[b], bs = f->column_start[sn], ws = f->column_start[sn + 1] - bs;
    csi rs = f->row_ptr[sn + 1] - f->row_ptr[sn];
    const double *as = f->panel + f->panel_offset[sn] + ws * (ws + 1) / 2;
    const vsdlss_sni *R = f->row_index + f->row_ptr[sn];
    if (simd) { vsdlss_simd_block_update(as, rs, ws, x + bs, R, s, e, x); return; }
    for (csi j = 0; j < ws; ++j) {
        const double xj = x[bs + j], *col = as + j * rs;
        for (csi r = s; r < e; ++r) x[R[r]] -= col[r] * xj;
    }
}

/* VSDLSS_FWD_TOP_PLAN=0: no cached tree-top plan (A/B; same bits). */
static int top_plan_on(void)
{
    static int v = -1;
    if (v < 0) { const char *e = getenv("VSDLSS_FWD_TOP_PLAN"); v = !(e && e[0] == '0'); }
    return v;
}

/* The tree-top plan for split sp and teams of T threads (see top_plan). */
static top_plan *top_plan_build(const vsdlss_sn_factor *f, const tree_info *t, const char *in_sub, int T)
{
    const csi count = f->count;
    top_plan *p = calloc(1, sizeof(*p));
    if (!p) return NULL;
    p->T = T;
    if (!(p->bigk = malloc((size_t)(count ? count : 1) * sizeof(csi)))) goto fail;
    for (csi k = 0; k < count; ++k) {
        const csi d = t->order[k];
        if (!in_sub[d] && forward_pull_work(f, d) >= FWD_TOP_PAR_WORK) p->bigk[p->nbig++] = k;
    }
    size_t cap = 0, n = 0;
    for (csi i = 0; i < p->nbig; ++i) { const csi d = t->order[p->bigk[i]]; cap += (size_t)(f->blk_ptr[d + 1] - f->blk_ptr[d]); }
    cap *= (size_t)T;
    if (!(p->roff = malloc(((size_t)p->nbig * (size_t)T + 1) * sizeof(csi))) ||
        !(p->rg = malloc((cap ? cap : 1) * sizeof(top_range)))) goto fail;
    for (csi i = 0; i < p->nbig; ++i) {
        const csi d = t->order[p->bigk[i]], bd = f->column_start[d], wd = f->column_start[d + 1] - bd;
        for (int tid = 0; tid < T; ++tid) {
            const csi lo = bd + wd * tid / T, hi = bd + wd * (tid + 1) / T;
            p->roff[i * T + tid] = (csi)n;
            for (csi b = f->blk_ptr[d]; b < f->blk_ptr[d + 1]; ++b) {
                const vsdlss_sni *R = f->row_index + f->row_ptr[f->blk_src[b]];
                const csi s = lower_bound_csi(R, f->blk_first[b], f->blk_end[b], lo);
                const csi e = lower_bound_csi(R, s, f->blk_end[b], hi);
                if (s < e) p->rg[n++] = (top_range){ b, s, e };
            }
        }
    }
    p->roff[p->nbig * T] = (csi)n;
    return p;
fail:
    top_plan_free(p);
    return NULL;
}

/* rg: this thread's ranges for target d from a tree-top plan (nr of them),
 * or NULL to find them here. */
static void forward_pull_team(const vsdlss_sn_factor *f, csi d, double *x, int tid, int T, int *bad,
                              const top_range *rg, csi nr)
{
    const csi bd = f->column_start[d], wd = f->column_start[d + 1] - bd;
    const double *a = f->panel + f->panel_offset[d];
    const int simd = vsdlss_simd_enabled();
    {
        if (rg) for (csi q = 0; q < nr; ++q) pull_block_rows(f, rg[q].b, rg[q].s, rg[q].e, x, simd);
        else {
            const csi lo = bd + wd * tid / T, hi = bd + wd * (tid + 1) / T;
            for (csi b = f->blk_ptr[d]; b < f->blk_ptr[d + 1]; ++b) {
                const vsdlss_sni *R = f->row_index + f->row_ptr[f->blk_src[b]];
                const csi s = lower_bound_csi(R, f->blk_first[b], f->blk_end[b], lo);
                const csi e = lower_bound_csi(R, s, f->blk_end[b], hi);
                if (s < e) pull_block_rows(f, b, s, e, x, simd);
            }
        }
        VSDLSS_OMP(omp barrier)
        for (csi jb = 0; jb < wd; jb += FWD_TOP_BLK) {
            const csi je = jb + FWD_TOP_BLK < wd ? jb + FWD_TOP_BLK : wd;
            VSDLSS_OMP(omp single)
            for (csi j = jb; j < je; ++j) {
                const double *aj = a + VSDLSS_SN_DCOL(j, wd);
                double dj = aj[0];
                if (!isfinite(dj) || dj <= 0) *bad = 1;
                x[bd + j] /= dj;
                if (simd) vsdlss_simd_axpy_neg(x + bd + j + 1, aj + 1, x[bd + j], je - j - 1);
                else for (csi r = j + 1; r < je; ++r) x[bd + r] -= aj[r - j] * x[bd + j];
            }
            const csi r0 = je + (wd - je) * tid / T, r1 = je + (wd - je) * (tid + 1) / T;
            if (simd) vsdlss_simd_tri_update_packed(a, wd, jb, je, x + bd, r0, r1, x + bd);
            else for (csi r = r0; r < r1; ++r) {
                double v = x[bd + r];
                for (csi j = jb; j < je; ++j) v -= a[VSDLSS_SN_DCOL(j, wd) + r - j] * x[bd + j];
                x[bd + r] = v;
            }
            VSDLSS_OMP(omp barrier)
        }
    }
}

static int forward_pull_par(const vsdlss_sn_factor *f, csi d, double *x, int nt)
{
    int bad = 0;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt))
    {
#ifdef _OPENMP
        const int tid = omp_get_thread_num(), T = omp_get_num_threads();
#else
        const int tid = 0, T = 1;
#endif
        forward_pull_team(f, d, x, tid, T, &bad, NULL, 0);
    }
    return bad;
}

/* Forward tree top on one team for the whole top (default; 0 selects a team
 * per large target, the former schedule).  Targets run in the same order
 * either way: runs of small targets on one thread, each large target on
 * the team, so the results are the same bits. */
int vsdlss_fwd_top_team = 1;

static int backward_panel(const vsdlss_sn_factor *f, csi sn, double *x)
{
    csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
    csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn];
    vsdlss_status st = vsdlss_sn_panel_solve(f->panel + f->panel_offset[sn], b, w, ext,
        ext ? f->row_index + f->row_ptr[sn] : NULL, x, 1);
    return st == VSDLSS_OK ? 0 : st == VSDLSS_ERR_NONFINITE ? 2 : 1;
}

/* Tree-parallel solves: independent subtrees in parallel, the top of the
 * tree in order (forward: subtrees first; backward: top first).  The tree
 * and the split for nt come from the factor's cache when it has one. */
static vsdlss_status solve_tree(const vsdlss_sn_factor *f, double *x, int nt)
{
    csi count = f->count;
    tree_info local_t; const tree_info *t; int bad = 0;
    sn_split *sp = NULL, *owned = NULL;
    struct vsdlss_sn_solve_tree *cache = f->solve_tree;
    if (cache) t = &cache->t;
    else {
        if (!tree_build(f, NULL, f->sn_parent, 1, &local_t)) return VSDLSS_ERR_OOM;
        t = &local_t;
    }
    if (cache && nt < SN_SPLIT_SLOTS) sp = atomic_load(&cache->split[nt]);
    if (!sp) {
        owned = split_build(f, t, nt);
        if (!owned) { if (!cache) tree_free(&local_t); return VSDLSS_ERR_OOM; }
        sp = owned;
        if (cache && nt < SN_SPLIT_SLOTS) {
            sn_split *expect = NULL;
            if (atomic_compare_exchange_strong(&cache->split[nt], &expect, owned)) owned = NULL;
            else { split_free(owned); owned = NULL; sp = expect; }
        }
    }
    const csi nsub = sp->nsub, *sub = sp->sub, *sub_end = sp->sub_end;
    const char *in_sub = sp->in_sub;
    const int nt_top = nt;          /* team for large tree-top panels */
    if (nt > nsub) nt = (int)(nsub ? nsub : 1);
    (void)nt;
#ifdef _OPENMP
    /* VSDLSS_SOLVE_PROFILE=1: per-call phase times and the tree-top share of L. */
    static int prof = -1;
    if (prof < 0) { const char *e = getenv("VSDLSS_SOLVE_PROFILE"); prof = e && *e && *e != '0'; }
    double tp[5] = {0}; if (prof) tp[0] = omp_get_wtime();
#endif
    if (sp->moff && sub_push_on()) {
        const csi *moff = sp->moff; const vsdlss_sni *mem = sp->mem, *ein = sp->ein;
        VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(dynamic,1) reduction(|:bad))
        for (csi i = 0; i < nsub; ++i)
            for (csi q = moff[i]; !bad && q < moff[i + 1]; ++q) bad |= forward_push(f, mem[q], ein[q], x);
    } else {
        VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(dynamic,1) reduction(|:bad))
        for (csi i = 0; i < nsub; ++i)
            for (csi k = t->first[sub[i]]; !bad && k <= sub_end[i]; ++k) bad |= forward_pull(f, t->order[k], x);
    }
#ifdef _OPENMP
    if (prof) tp[1] = omp_get_wtime();
#endif
    /* Tree-top plan: cached with the split (same thread count), or built
     * for this call only. */
    top_plan *plan = NULL, *plan_owned = NULL;
    if (!bad && nt_top > 1 && vsdlss_fwd_top_team) {
        const int cached = top_plan_on() && cache && sp != owned && nt_top < SN_SPLIT_SLOTS;
        if (cached) plan = atomic_load(&cache->plan[nt_top]);
        if (!plan && (plan_owned = top_plan_build(f, t, in_sub, nt_top))) {
            plan = plan_owned;
            if (cached) {
                top_plan *expect = NULL;
                if (atomic_compare_exchange_strong(&cache->plan[nt_top], &expect, plan_owned)) plan_owned = NULL;
                else { top_plan_free(plan_owned); plan_owned = NULL; plan = expect; }
            }
        }
    }
    if (plan) {
        /* One team runs the whole top: each run of small targets on one
         * thread (the single's barrier orders it before the next large
         * target), each large target on the team.  Order as below: same
         * bits. */
        const csi nbig = plan->nbig, *bigk = plan->bigk;
        int badt = 0;
        VSDLSS_OMP(omp parallel num_threads(nt_top))
        {
#ifdef _OPENMP
            const int tid = omp_get_thread_num(), T = omp_get_num_threads();
#else
            const int tid = 0, T = 1;
#endif
            /* Ranges were cut for plan->T threads; another team size finds
             * its own. */
            const int use = T == plan->T;
            csi k = 0;
            for (csi i = 0; i <= nbig; ++i) {
                const csi kend = i < nbig ? bigk[i] : count;
                VSDLSS_OMP(omp single)
                for (csi kk = k; kk < kend; ++kk) {
                    const csi d = t->order[kk];
                    if (!in_sub[d]) badt |= forward_pull(f, d, x);
                }
                if (i < nbig) {
                    const csi r0 = use ? plan->roff[i * T + tid] : 0, r1 = use ? plan->roff[i * T + tid + 1] : 0;
                    forward_pull_team(f, t->order[kend], x, tid, T, &badt, use ? plan->rg + r0 : NULL, r1 - r0);
                }
                k = kend + 1;
            }
        }
        bad |= badt;
        top_plan_free(plan_owned);
    } else
    for (csi k = 0; k < count && !bad; ++k) {
        const csi d = t->order[k];
        if (in_sub[d]) continue;
        if (nt_top > 1 && forward_pull_work(f, d) >= FWD_TOP_PAR_WORK)
            bad |= forward_pull_par(f, d, x, nt_top);
        else
            bad |= forward_pull(f, d, x);
    }
#ifdef _OPENMP
    if (prof) tp[2] = omp_get_wtime();
#endif
    for (csi k = count - 1; k >= 0 && !bad; --k)
        if (!in_sub[t->order[k]]) bad |= backward_panel(f, t->order[k], x);
#ifdef _OPENMP
    if (prof) tp[3] = omp_get_wtime();
#endif
    if (!bad) {
        /* The subtree is order[first[root] .. sub_end] with the root last;
         * walk it backwards so parents precede children. */
        VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(dynamic,1) reduction(|:bad))
        for (csi i = 0; i < nsub; ++i)
            for (csi k = sub_end[i]; !bad && k >= t->first[sub[i]]; --k) bad |= backward_panel(f, t->order[k], x);
    }
#ifdef _OPENMP
    if (prof) {
        tp[4] = omp_get_wtime();
        double top = 0, all = 0;   /* stored entries (incl. amalgamation zeros) */
        for (csi d = 0; d < count; ++d) {
            double w = (double)(f->column_start[d + 1] - f->column_start[d]);
            double e = (double)(f->row_ptr[d + 1] - f->row_ptr[d]);
            double s = w * (w + 1) / 2 + w * e;
            all += s; if (!in_sub[d]) top += s;
        }
        fprintf(stderr, "vsdlss solve profile: n=%lld nt=%d subtrees=%lld | fwd sub %.4f top %.4f | bwd top %.4f sub %.4f s | top share of L %.1f%%\n",
                (long long)f->n, nt, (long long)nsub, tp[1] - tp[0], tp[2] - tp[1], tp[3] - tp[2], tp[4] - tp[3], 100 * top / all);
    }
#endif
    split_free(owned);
    if (!cache) tree_free(&local_t);
    return bad == 0 ? VSDLSS_OK : (bad & 1) ? VSDLSS_ERR_INVALID : VSDLSS_ERR_NONFINITE;
}

vsdlss_status vsdlss_sn_solve_inplace(const vsdlss_sn_factor *f, double *x)
{
    if (!f || !x || f->n < 1) return VSDLSS_ERR_INVALID;
    int nt = vsdlss_parallel_width((double)f->l_nnz * 2);
#ifdef VSDLSS_BLAS
    /* The tree schedule's pull-form forward step has no BLAS form with the
     * same rounding as the panel solve, so with BLAS solves every thread
     * count takes the serial path (results then match for all counts). */
    if (vsdlss_panel_solve_uses_blas()) nt = 1;
#endif
    const int lg = vsdlss_ledger_level();
    if (nt > 1 && f->count > 1 && f->sn_parent && f->blk_ptr) {
        double t0 = lg ? vsdlss_ledger_now() : 0;
        vsdlss_status st = solve_tree(f, x, nt);
        /* Tree path: forward and backward are not separated here; the
         * whole core is booked as forward+backward halves by solve_tree's
         * own profile when VSDLSS_SOLVE_PROFILE is set. */
        if (lg) vsdlss_ledger.wall[LG_CORE_FWD] += vsdlss_ledger_now() - t0;
        return st;
    }
    if (lg >= 2) {
        /* Per-panel TSC timing by width bucket (thread-cumulative). */
        const double tick = vsdlss_ledger_tsc_sec();
        for (int back = 0; back < 2; back++) {
            double t0 = vsdlss_ledger_now();
            for (csi t = 0; t < f->count; t++) {
                csi sn = back ? f->count - 1 - t : t;
                csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
                csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn];
                uint64_t c0 = vsdlss_ledger_tsc();
                vsdlss_status st = vsdlss_sn_panel_solve(f->panel + f->panel_offset[sn], b, w, ext,
                    ext ? f->row_index + f->row_ptr[sn] : NULL, x, back);
                uint64_t c1 = vsdlss_ledger_tsc();
                if (st != VSDLSS_OK) return st;
                int k = vsdlss_ledger_bucket(w);
                vsdlss_ledger.bucket_s[back][k] += (double)(c1 - c0) * tick;
                vsdlss_ledger.bucket_n[back][k]++;
                vsdlss_ledger.bucket_bytes[back][k] += 8.0 * ((double)w * (w + 1) / 2 + (double)w * ext) + 8.0 * ext;
            }
            vsdlss_ledger.wall[back ? LG_CORE_BWD : LG_CORE_FWD] += vsdlss_ledger_now() - t0;
        }
        return VSDLSS_OK;
    }
    for (int back = 0; back < 2; back++) {
        double t0 = lg ? vsdlss_ledger_now() : 0;
        for (csi t = 0; t < f->count; t++) {
            csi sn = back ? f->count - 1 - t : t;
            csi b = f->column_start[sn], w = f->column_start[sn + 1] - b;
            csi ext = f->row_ptr[sn + 1] - f->row_ptr[sn];
            vsdlss_status st = vsdlss_sn_panel_solve(f->panel + f->panel_offset[sn], b, w, ext,
                ext ? f->row_index + f->row_ptr[sn] : NULL, x, back);
            if (st != VSDLSS_OK) return st;
        }
        if (lg) vsdlss_ledger.wall[back ? LG_CORE_BWD : LG_CORE_FWD] += vsdlss_ledger_now() - t0;
    }
    return VSDLSS_OK;
}

vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *f, const double *rhs, double *out)
{
    double *x; csi j;
    if (!f || !rhs || !out || f->n < 1 || !bytes_ok(f->n, sizeof(double))) return VSDLSS_ERR_INVALID;
    x = malloc((size_t)f->n * sizeof(*x)); if (!x) return VSDLSS_ERR_OOM;
    for (j = 0; j < f->n; j++) {
        if (!isfinite(rhs[j])) { free(x); return VSDLSS_ERR_NONFINITE; }
        x[j] = rhs[j];
    }
    vsdlss_status st = vsdlss_sn_solve_inplace(f, x);
    if (st != VSDLSS_OK) { free(x); return st; }
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
            vsdlss_status st = vsdlss_sn_panel_solve(f->panel + f->panel_offset[sn], b, w, ext,
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
        const vsdlss_sni *R = f->row_index + f->row_ptr[s];
        const double *a = f->panel + f->panel_offset[s], *rect = a + w * (w + 1) / 2;
        for (csi j = 0; j < w; ++j) {
            L->p[b + j] = nz;
            for (csi i = j; i < rows; ++i) {
                if (nz >= f->l_nnz) { vsdlss_spfree(L); return VSDLSS_ERR_INVALID; }
                L->i[nz] = i < w ? b + i : R[i - w];
                L->x[nz++] = i < w ? a[VSDLSS_SN_DCOL(j, w) + i - j] : rect[j * ext + i - w];
            }
        }
    }
    L->p[f->n] = nz;
    *out = L; return VSDLSS_OK;
}
