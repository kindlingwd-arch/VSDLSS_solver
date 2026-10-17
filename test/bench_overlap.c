/* Feasibility measurements for decoupling the low-degree replays from the
 * core solve through the exact interface response (Schur complement form).
 *
 * With I the vertices eliminated by the low-degree reduction and G the core,
 * A symmetric:
 *     core RHS        yG = bG - H^T bI
 *     eliminated      xI = z - H xG,   z = A_II^{-1} bI,   H = A_II^{-1} A_IG
 * so the core solve could start before the forward replay, and the local
 * replays (computing z) could run alongside it.  Whether that pays depends on
 *   A. the structure of H: how many core vertices each eliminated vertex's
 *      solution depends on (row counts of H), and of H^T;
 *   B. whether the core solve leaves memory bandwidth idle: the core solve and
 *      the replays timed alone and at the same time on two thread groups;
 *   C. the cost of the two extra sparse products, H^T b and xI = z - H xG,
 *      with the measured structure (values are placeholders).
 * The last lines put A-C into the current (serial) and the decoupled model.
 *
 *   ./bench_overlap dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads [reps]
 *   build: make bench_overlap METIS=1 (order 6), or without METIS for order 5
 *   run with OMP_PROC_BIND unset or false (two thread groups share the CPUs)
 *
 * Only reads the factor; the solver is not changed. */
#define _GNU_SOURCE   /* getline, syscall, pthread_barrier */
#include "../src/vsdlss_m3_internal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#if defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#endif
#ifdef _OPENMP
#include <omp.h>
#endif

#include "bench_input.h"

#define CAP 8            /* supports larger than this are counted, not stored */
#define OVER 255

static double median(double *t, int r) { qsort(t, (size_t)r, 8, cmpd); return t[r / 2]; }

/* ---- A. structure of H ------------------------------------------------------
 * Backward replay: record k (pivot v, neighbours w_j) reads x[w_j], so the
 * core support of x_v is the union of the supports of the w_j; a core vertex
 * supports itself.  Records are visited in reverse replay order. */
typedef struct {
    csi n, cnt, core;
    uint8_t *sz;          /* n: support size of a pivot vertex (OVER: > CAP) */
    uint32_t *set;        /* n * CAP */
    uint8_t *pivot;       /* n: 1 for eliminated vertices */
} support;

static void support_free(support *s) { free(s->sz); free(s->set); free(s->pivot); memset(s, 0, sizeof *s); }

static int support_build(const vsdlss_reduction *r, support *s)
{
    const csi n = r->n;
    memset(s, 0, sizeof *s);
    s->n = n; s->cnt = r->count; s->core = r->core_n;
    s->sz = calloc((size_t)n, 1); s->pivot = calloc((size_t)n, 1);
    s->set = malloc((size_t)n * CAP * sizeof(uint32_t));
    if (!s->sz || !s->pivot || !s->set) { support_free(s); return 0; }
    for (csi q = 0; q < r->pk_count; q++)
        for (csi i = 0; i < r->pk[q].count; i++) s->pivot[vsdlss_pk_vertex(r->pk + q, i)] = 1;
    for (csi v = 0; v < n; v++) if (!s->pivot[v]) { s->sz[v] = 1; s->set[(size_t)v * CAP] = (uint32_t)v; }
    uint32_t tmp[3 * CAP];
    for (csi q = r->pk_count; q > 0;) {
        const vsdlss_pk_seg *g = r->pk + --q; csi o = g->nbn;
        for (csi i = g->count; i > 0;) {
            i--;
            const csi d = vsdlss_pk_degree(g, i), v = vsdlss_pk_vertex(g, i);
            o -= d;
            csi m = 0; int over = 0;
            for (csi j = 0; j < d && !over; j++) {
                const csi u = g->nb[o + j];
                if (s->sz[u] == OVER) { over = 1; break; }
                for (csi a = 0; a < s->sz[u]; a++) {
                    const uint32_t c = s->set[(size_t)u * CAP + a]; int dup = 0;
                    for (csi b = 0; b < m; b++) if (tmp[b] == c) { dup = 1; break; }
                    if (!dup) tmp[m++] = c;
                }
                if (m > CAP) over = 1;
            }
            if (over) s->sz[v] = OVER;
            else { s->sz[v] = (uint8_t)m; memcpy(s->set + (size_t)v * CAP, tmp, (size_t)m * 4); }
        }
    }
    return 1;
}

/* Prints the row counts of H and H^T; returns nnz(H) over the stored rows. */
static double support_report(const support *s, csi comp)
{
    csi hist[7] = {0}, over = 0; double nnz = 0;
    uint32_t *col = calloc((size_t)s->n, 4);
    for (csi v = 0; v < s->n; v++) {
        if (!s->pivot[v]) continue;
        if (s->sz[v] == OVER) { over++; hist[6]++; continue; }
        const int z = s->sz[v]; nnz += z;
        hist[z <= 4 ? (z ? z - 1 : 0) : z <= 8 ? 4 : 5]++;
        if (col) for (int a = 0; a < z; a++) col[s->set[(size_t)v * CAP + a]]++;
    }
    const double cnt = (double)(s->cnt ? s->cnt : 1);
    printf("# A. component %lld: eliminated %lld, core %lld | H row nnz: mean %.2f |"
           " 1: %.1f%% 2: %.1f%% 3: %.1f%% 4: %.1f%% 5-%d: %.1f%% >%d: %.2f%%\n",
           (long long)comp, (long long)s->cnt, (long long)s->core, nnz / (cnt - (double)over),
           100 * hist[0] / cnt, 100 * hist[1] / cnt, 100 * hist[2] / cnt, 100 * hist[3] / cnt, CAP,
           100 * (hist[4] + hist[5]) / cnt, CAP, 100 * hist[6] / cnt);
    if (col) {
        double used = 0, mx = 0, sum = 0;
        for (csi v = 0; v < s->n; v++) if (!s->pivot[v]) { if (col[v]) used++; sum += col[v]; if (col[v] > mx) mx = col[v]; }
        printf("#    H^T rows (core vertices): %.1f%% referenced, mean %.1f, max %.0f |"
               " H storage (CSR, 4 B index + 8 B value) %.0f MB, H^T as much again\n",
               100 * used / (double)(s->core ? s->core : 1), sum / (used ? used : 1), mx,
               (nnz * 12 + (double)s->cnt * 8) / 1e6);
        free(col);
    }
    return nnz;
}

/* ---- B. core solve and replays, alone and at the same time ------------------ */
typedef struct {
    const vsdlss_m3_component_factor *cf;
    const double *w0, *c0; double *w, *cx;
    csi n, cn;
} work;

static void run_core(const work *k)   { vsdlss_sn_solve_inplace(k->cf->numeric, k->cx); }
static void run_replay(const work *k) {
    vsdlss_reduce_forward_inplace(k->cf->reduction, k->w, NULL);
    vsdlss_reduce_backward_inplace(k->cf->reduction, NULL, k->w);
}
static void reset(const work *k) { memcpy(k->w, k->w0, (size_t)k->n * 8); if (k->cn) memcpy(k->cx, k->c0, (size_t)k->cn * 8); }

typedef struct { const work *k; int kind, threads; pthread_barrier_t *bar; double t; } job;
static void *job_run(void *a)
{
    job *j = a;
    vsdlss_set_num_threads(j->threads);     /* per calling thread */
    pthread_barrier_wait(j->bar);
    double s = now();
    if (j->kind == 0) run_core(j->k); else run_replay(j->k);
    j->t = now() - s;
    return NULL;
}
/* Both at once: core on tc threads, replays on tr threads; returns wall time. */
static double together(const work *k, int tc, int tr, double *tcore, double *trep)
{
    pthread_t a, b; pthread_barrier_t bar; job jc = {k, 0, tc, &bar, 0}, jr = {k, 1, tr, &bar, 0};
    pthread_barrier_init(&bar, NULL, 3);
    pthread_create(&a, NULL, job_run, &jc); pthread_create(&b, NULL, job_run, &jr);
    pthread_barrier_wait(&bar);
    double s = now();
    pthread_join(a, NULL); pthread_join(b, NULL);
    double wall = now() - s;
    pthread_barrier_destroy(&bar);
    *tcore = jc.t; *trep = jr.t;
    return wall;
}
static double alone(const work *k, int kind, int threads, int reps, double *ts)
{
    vsdlss_set_num_threads(threads);
    for (int q = -1; q < reps; q++) {
        reset(k);
        double s = now();
        if (kind == 0) run_core(k);
        else if (kind == 1) run_replay(k);
        else if (kind == 2) vsdlss_reduce_forward_inplace(k->cf->reduction, k->w, NULL);
        else { vsdlss_reduce_forward_inplace(k->cf->reduction, k->w, NULL); s = now(); vsdlss_reduce_backward_inplace(k->cf->reduction, NULL, k->w); }
        if (q >= 0) ts[q] = now() - s;
    }
    return median(ts, reps);
}

/* ---- C. the two extra sparse products ---------------------------------------- */
typedef struct { csi rows; csi *p; uint32_t *i; double *x; } csr;
static void csr_free(csr *a) { free(a->p); free(a->i); free(a->x); memset(a, 0, sizeof *a); }

/* H (rows: eliminated vertices, columns: core vertices as local indices) and
 * H^T (rows: core vertices in order, columns: eliminated local indices). */
static int build_h(const support *s, csr *h, csr *ht, csi **hrow, csi **htrow)
{
    const csi n = s->n; csi nnz = 0, rows = 0, crow = 0;
    for (csi v = 0; v < n; v++) if (s->pivot[v] && s->sz[v] != OVER) { nnz += s->sz[v]; rows++; } else if (!s->pivot[v]) crow++;
    h->rows = rows; ht->rows = crow;
    h->p = malloc((size_t)(rows + 1) * sizeof(csi)); h->i = malloc((size_t)(nnz ? nnz : 1) * 4); h->x = malloc((size_t)(nnz ? nnz : 1) * 8);
    ht->p = calloc((size_t)crow + 1, sizeof(csi)); ht->i = malloc((size_t)(nnz ? nnz : 1) * 4); ht->x = malloc((size_t)(nnz ? nnz : 1) * 8);
    *hrow = malloc((size_t)(rows ? rows : 1) * sizeof(csi)); *htrow = malloc((size_t)(crow ? crow : 1) * sizeof(csi));
    csi *cid = malloc((size_t)n * sizeof(csi));
    if (!h->p || !h->i || !h->x || !ht->p || !ht->i || !ht->x || !*hrow || !*htrow || !cid) { free(cid); return 0; }
    csi r = 0, k = 0, c = 0;
    for (csi v = 0; v < n; v++) if (!s->pivot[v]) { cid[v] = c; (*htrow)[c++] = v; }
    h->p[0] = 0;
    for (csi v = 0; v < n; v++) {
        if (!s->pivot[v] || s->sz[v] == OVER) continue;
        (*hrow)[r] = v;
        for (int a = 0; a < s->sz[v]; a++) {
            const uint32_t u = s->set[(size_t)v * CAP + a];
            h->i[k] = u; h->x[k] = 1e-3 * (double)(1 + k % 7); ht->p[cid[u] + 1]++; k++;
        }
        h->p[++r] = k;
    }
    for (csi q = 0; q < crow; q++) ht->p[q + 1] += ht->p[q];
    csi *at = malloc((size_t)(crow ? crow : 1) * sizeof(csi));
    if (!at) { free(cid); return 0; }
    memcpy(at, ht->p, (size_t)crow * sizeof(csi));
    for (csi q = 0; q < rows; q++)            /* rows of H^T in ascending eliminated index */
        for (csi p = h->p[q]; p < h->p[q + 1]; p++) { const csi t = at[cid[h->i[p]]]++; ht->i[t] = (uint32_t)(*hrow)[q]; ht->x[t] = h->x[p]; }
    free(at); free(cid);
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 4) { fprintf(stderr, "usage: bench_overlap dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads [reps]\n"); return 1; }
    const int order = atoi(argv[2]), T = atoi(argv[3]) > 0 ? atoi(argv[3]) : 1;
    int reps = argc > 4 ? atoi(argv[4]) : 9; if (reps < 1) reps = 1; if (reps > 64) reps = 64;
    interleave_memory();
    { const char *b = getenv("OMP_PROC_BIND");
      if (b && *b && strcmp(b, "false") && strcmp(b, "FALSE"))
          printf("# warning: OMP_PROC_BIND=%s -- the two thread groups of part B may be bound to the same CPUs; unset it\n", b); }
    vsdlss *A = read_input(argv[1]);
    if (!A) { puts("input failed"); return 1; }
    vsdlss_set_num_threads(T);
    vsdlss_m3_factor *F = NULL; double t0 = now();
    const vsdlss_status st = vsdlss_factorize_m3(A, order, &F);
    if (st != VSDLSS_OK) {
        printf("factor failed: %s%s\n", vsdlss_status_string(st),
               st == VSDLSS_ERR_UNSUPPORTED && order == 6 ? " (order 6 needs make bench_overlap METIS=1)" : "");
        return 1;
    }
    printf("# %s: n %lld, order %d, %d threads, factor %.1f s, %lld components\n", argv[1], (long long)A->n, order, T,
           now() - t0, (long long)F->count);
    vsdlss_spfree(A);

    /* A for every large component; the largest one is kept for B and C. */
    csi best = -1;
    for (csi c = 0; c < F->count; c++)
        if (F->component[c].reduction && F->component[c].numeric && (best < 0 || F->component[c].n > F->component[best].n)) best = c;
    if (best < 0) { puts("no component with a reduction and a core"); return 1; }
    support sb; memset(&sb, 0, sizeof sb); double nnz_best = 0;
    for (csi c = 0; c < F->count; c++) {
        const vsdlss_reduction *r = F->component[c].reduction;
        if (!r || !r->pk || r->records || F->component[c].n < 100000) continue;
        support s;
        if (!support_build(r, &s)) { printf("# A. component %lld: out of memory\n", (long long)c); continue; }
        const double nz = support_report(&s, c);
        if (c == best) { sb = s; nnz_best = nz; } else support_free(&s);
    }
    fflush(stdout);
    if (!sb.n) { puts("# largest component: support not built"); return 1; }

    const vsdlss_m3_component_factor *cf = F->component + best;
    const csi n = cf->n, cn = cf->numeric->n;
    double *w0 = malloc((size_t)n * 8), *w = malloc((size_t)n * 8), *c0 = malloc((size_t)cn * 8), *cx = malloc((size_t)cn * 8);
    double ts[64];
    if (!w0 || !w || !c0 || !cx) { puts("alloc failed"); return 1; }
    for (csi i = 0; i < n; i++) w0[i] = sin(0.37 * (double)i) + 0.2;
    for (csi i = 0; i < cn; i++) c0[i] = cos(0.11 * (double)i) + 0.3;
    work k = {cf, w0, c0, w, cx, n, cn};

    /* B */
    const double fwd = alone(&k, 2, T, reps, ts), bwd = alone(&k, 3, T, reps, ts), core = alone(&k, 0, T, reps, ts);
    printf("# B. component %lld (n %lld, core %lld), %d threads alone: replay fwd %.2f ms, core %.2f ms, replay bwd %.2f ms"
           " | current serial order %.2f ms\n", (long long)best, (long long)n, (long long)cn, T,
           1e3 * fwd, 1e3 * core, 1e3 * bwd, 1e3 * (fwd + core + bwd));
    double best_wall = 1e30, best_core = 0, best_rep = 0; int best_tc = 0;
    if (T >= 2) {
        int splits[3] = {T / 4, T / 2, (3 * T) / 4}, last = -1;
        for (int si = 0; si < 3; si++) {
            const int tc = splits[si] < 1 ? 1 : splits[si] >= T ? T - 1 : splits[si], tr = T - tc;
            if (tc == last) continue;
            last = tc;
            const double ca = alone(&k, 0, tc, reps, ts), ra = alone(&k, 1, tr, reps, ts);
            double tw[64], tcs[64], trs[64];
            for (int q = -1; q < reps; q++) {
                reset(&k);
                double a, b, wall = together(&k, tc, tr, &a, &b);
                if (q >= 0) { tw[q] = wall; tcs[q] = a; trs[q] = b; }
            }
            const double wall = median(tw, reps), cc = median(tcs, reps), rc = median(trs, reps);
            printf("#    split core %d + replays %d threads: alone core %.2f, replays (fwd+bwd) %.2f ms |"
                   " together core %.2f (x%.2f), replays %.2f (x%.2f), wall %.2f ms\n",
                   tc, tr, 1e3 * ca, 1e3 * ra, 1e3 * cc, cc / ca, 1e3 * rc, rc / ra, 1e3 * wall);
            if (wall < best_wall) { best_wall = wall; best_core = cc; best_rep = rc; best_tc = tc; }
        }
    }
    fflush(stdout);

    /* C */
    csr h, ht; csi *hrow = NULL, *htrow = NULL; memset(&h, 0, sizeof h); memset(&ht, 0, sizeof ht);
    double tht = 0, tcomb = 0;
    if (build_h(&sb, &h, &ht, &hrow, &htrow)) {
        double *b = w, *yc = cx;      /* b: local vector, yc: core RHS by core order */
        const csi hr = h.rows, cr = ht.rows;
        for (int pass = 0; pass < 2; pass++) {
            for (int q = -1; q < reps; q++) {
                memcpy(w, w0, (size_t)n * 8);
                double s = now();
                if (pass == 0) {
                    #pragma omp parallel for num_threads(T) schedule(static)
                    for (csi c = 0; c < cr; c++) {
                        double a = b[htrow[c]];
                        for (csi p = ht.p[c]; p < ht.p[c + 1]; p++) a -= ht.x[p] * b[ht.i[p]];
                        yc[c] = a;
                    }
                } else {
                    #pragma omp parallel for num_threads(T) schedule(static)
                    for (csi r = 0; r < hr; r++) {
                        const csi v = hrow[r]; double a = w[v];
                        for (csi p = h.p[r]; p < h.p[r + 1]; p++) a -= h.x[p] * w[h.i[p]];
                        w[v] = a;
                    }
                }
                if (q >= 0) ts[q] = now() - s;
            }
            if (pass == 0) tht = median(ts, reps); else tcomb = median(ts, reps);
        }
        printf("# C. %d threads: H^T b (core RHS) %.2f ms, x = z - H x_core %.2f ms (nnz(H) %.0f)\n",
               T, 1e3 * tht, 1e3 * tcomb, nnz_best);
    } else puts("# C. out of memory building H");

    /* Models (one component, T threads; the real solve runs the components
     * concurrently, each on a share of the threads). */
    const double cur = fwd + core + bwd;
    printf("# model, component %lld: current fwd + core + bwd = %.2f ms", (long long)best, 1e3 * cur);
    if (best_tc) {
        const double dec = (tht + best_core > best_rep ? tht + best_core : best_rep) + tcomb;
        printf(" | decoupled max(H^T b + core, replays) + combine = %.2f ms with core on %d threads (%+.1f%%)",
               1e3 * dec, best_tc, 100 * (dec - cur) / cur);
    }
    printf("\n# the decoupled numbers leave out building H (factorization) and its memory; values of H are placeholders\n");
    csr_free(&h); csr_free(&ht); free(hrow); free(htrow); support_free(&sb);
    free(w0); free(w); free(c0); free(cx);
    vsdlss_m3_factor_free(F);
    return 0;
}
