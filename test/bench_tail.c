/* Reduction-tail benchmark: only the low-degree replay of a tail-dominated
 * network, without ordering, factorization or file input.
 *
 *   ./bench_tail [n=8000000] [threads="1 2 4 8"] [reps=9] [lmin=20] [lmax=200]
 *
 * Builds a power-network-like graph -- a small G x G grid (2% of the nodes)
 * joined by wires of lmin..lmax degree-2 nodes between random grid nodes --
 * with the vertices numbered at random, so that nearly every eliminated
 * vertex has neighbours in other blocks of the parallel reduction pass and
 * its record lands in the sequential tail (as on a 61M-node EMIR case:
 * 99.6% of the records, 666 levels).  Then the packed low-degree
 * elimination of the M3 factorization (vsdlss_reduce_run_packed) twice:
 * with the level schedule (default) and without (VSDLSS_TAIL_MIN semantics:
 * vsdlss_reduce_tail_min huge), each followed by the solve-order relabel and
 * the forward plan, with the largest thread count tested (the step times are
 * printed); then times per thread count the in-place forward and backward
 * replays of each (median of reps; the vector is reset outside the timing).
 * Both must give the same bits.
 *
 * VSDLSS_INTERLEAVE=1 interleaves the memory over the NUMA nodes (servers);
 * BENCH_RELABEL=0 skips the relabel.
 *   build: make bench_tail */
#define _GNU_SOURCE
#include "../src/vsdlss_m3_internal.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#if defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#endif

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static int cmpd(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }
static uint64_t s_rng = 88172645463325252ULL;
static uint64_t rnd(void) { s_rng ^= s_rng << 13; s_rng ^= s_rng >> 7; s_rng ^= s_rng << 17; return s_rng; }

static void interleave_memory(void)
{
    const char *e = getenv("VSDLSS_INTERLEAVE");
    if (!e || atoi(e) == 0) { puts("# numa: off (VSDLSS_INTERLEAVE=1 to interleave)"); return; }
#if defined(__linux__) && defined(SYS_set_mempolicy)
    FILE *f = fopen("/sys/devices/system/node/online", "r"); char list[256] = "";
    if (!f || !fgets(list, sizeof list, f)) { if (f) fclose(f); puts("# numa: no NUMA information"); return; }
    fclose(f);
    unsigned long mask[16] = {0}; int maxnode = 0, nodes = 0;
    for (char *s = list; *s && *s != '\n';) {
        char *end; long a = strtol(s, &end, 10), b = a;
        if (end == s) break;
        if (*end == '-') { s = end + 1; b = strtol(s, &end, 10); }
        for (long nd = a; nd <= b && nd < 1024; nd++) { mask[nd / 64] |= 1UL << (nd % 64); nodes++; if (nd + 1 > maxnode) maxnode = (int)nd + 1; }
        s = *end == ',' ? end + 1 : end;
    }
    if (nodes < 2) { puts("# numa: single NUMA node"); return; }
    if (syscall(SYS_set_mempolicy, 3, mask, (unsigned long)maxnode + 1) != 0) printf("# numa: set_mempolicy failed: %s\n", strerror(errno));
    else printf("# numa: interleaved over nodes %s", list);
#else
    puts("# numa: not supported");
#endif
}

typedef struct { csi a, b; double w; } edge;
static int cmp_edge(const void *x, const void *y)
{
    const edge *p = x, *q = y;
    if (p->b != q->b) return (p->b > q->b) - (p->b < q->b);
    return (p->a > q->a) - (p->a < q->a);
}

/* Grid + wires, vertices numbered by a random permutation; upper CSC. */
static vsdlss *build(csi n, int lmin, int lmax)
{
    const csi G = (csi)sqrt(0.02 * (double)n), ng = G * G;
    const csi m = 2 * G * (G - 1) + 2 * (n - ng);
    edge *e = malloc((size_t)m * sizeof(edge)); csi *perm = malloc((size_t)n * sizeof(csi));
    double *diag = calloc((size_t)n, sizeof(double));
    if (!e || !perm || !diag) return NULL;
    for (csi i = 0; i < n; i++) perm[i] = i;
    for (csi i = n - 1; i > 0; i--) { csi j = (csi)(rnd() % (uint64_t)(i + 1)), t = perm[i]; perm[i] = perm[j]; perm[j] = t; }
    csi k = 0;
#define ADD(u, v, w) do { csi a_ = perm[u], b_ = perm[v]; e[k++] = (edge){a_ < b_ ? a_ : b_, a_ < b_ ? b_ : a_, (w)}; } while (0)
    for (csi y = 0; y < G; y++) for (csi x = 0; x < G; x++) {
        const csi v = y * G + x;
        if (x + 1 < G) ADD(v, v + 1, 1.0 + (double)(rnd() % 100) / 100.0);
        if (y + 1 < G) ADD(v, v + G, 1.0 + (double)(rnd() % 100) / 100.0);
    }
    for (csi i = ng; i < n;) {
        csi len = lmin + (csi)(rnd() % (uint64_t)(lmax - lmin + 1));
        if (i + len > n) len = n - i;
        csi prev = (csi)(rnd() % (uint64_t)ng);
        for (csi c = 0; c < len; c++, i++) { ADD(prev, i, 0.5 + (double)(rnd() % 1000) / 1000.0); prev = i; }
        ADD(prev, (csi)(rnd() % (uint64_t)ng), 0.5 + (double)(rnd() % 1000) / 1000.0);
    }
#undef ADD
    free(perm);
    qsort(e, (size_t)k, sizeof(edge), cmp_edge);
    for (csi q = 0; q < k; q++) { diag[e[q].a] += e[q].w; diag[e[q].b] += e[q].w; }
    vsdlss *A = vsdlss_spalloc(n, n, n + k, 1, 0);
    if (!A) return NULL;
    csi z = 0, q = 0;
    for (csi j = 0; j < n; j++) {
        A->p[j] = z;
        for (; q < k && e[q].b == j; q++) { A->i[z] = e[q].a; A->x[z++] = -e[q].w; }
        A->i[z] = j; A->x[z++] = diag[j] + 1e-3;
    }
    A->p[n] = z;
    free(e); free(diag);
    return A;
}

/* The M3 solve-order relabel (vsdlss_m3.c: relabel_component): vertices
 * numbered [eliminated in record order, core], so record i pivots on vertex
 * k0+i and the replays read their pivots sequentially.  id[old] = new. */
static int relabel_on = 1;
static vsdlss_status relabel(vsdlss_reduction *r, csi **ids)
{
    const csi n = r->n; csi *id = malloc((size_t)n * sizeof(csi)), pos = 0;
    if (!id) return VSDLSS_ERR_OOM;
    for (csi q = 0; q < r->pk_count; q++) for (csi i = 0; i < r->pk[q].count; i++) id[r->pk[q].head[i] & 0x3fffffffu] = pos++;
    for (csi k = 0; k < r->core_n; k++) { id[r->core_vertices[k]] = pos++; r->core_vertices[k] = id[r->core_vertices[k]]; }
    for (csi q = 0; q < r->pk_count; q++) {
        vsdlss_pk_seg *g = r->pk + q;
        for (csi j = 0; j < g->nbn; j++) g->nb[j] = (uint32_t)id[g->nb[j]];
        uint8_t *deg = malloc((size_t)(g->count ? g->count : 1));
        if (!deg) return VSDLSS_ERR_OOM;
        for (csi i = 0; i < g->count; i++) deg[i] = (uint8_t)(g->head[i] >> 30);
        free(g->head); g->head = NULL; g->deg = deg;
    }
    *ids = id;
    return VSDLSS_OK;
}

static vsdlss_reduction *reduce(const vsdlss *A, int schedule, double *secs, csi **ids)
{
    const csi saved = vsdlss_reduce_tail_min;
    if (!schedule) vsdlss_reduce_tail_min = (csi)1 << 62;
    vsdlss_reduction *r = NULL; double t = now();
    /* the packed (direct) path of the M3 factorization */
    vsdlss_reduce_input *in = NULL;
    vsdlss_status st = vsdlss_reduce_prepare_csc(A, &in);
    if (st == VSDLSS_OK) st = vsdlss_reduce_run_packed(in, &r, NULL);
    const double t1 = now();
    if (st == VSDLSS_OK && relabel_on) st = relabel(r, ids);
    const double t2 = now();
    if (st == VSDLSS_OK) st = vsdlss_reduce_tail_plan(r);
    *secs = now() - t;
    printf("# %s: reduce %.2f s, relabel %.2f s, plan %.2f s\n", schedule ? "schedule" : "serial tail",
           t1 - t, t2 - t1, now() - t2);
    vsdlss_reduce_tail_min = saved;
    if (st != VSDLSS_OK) { printf("reduction failed: %s\n", vsdlss_status_string(st)); return NULL; }
    return r;
}

int main(int argc, char **argv)
{
    const csi n = argc > 1 ? atoll(argv[1]) : 8000000;
    const char *tl = argc > 2 ? argv[2] : "1 2 4 8";
    const int reps = argc > 3 ? atoi(argv[3]) : 9;
    const int lmin = argc > 4 ? atoi(argv[4]) : 20, lmax = argc > 5 ? atoi(argv[5]) : 200;
    interleave_memory();
    double t0 = now();
    vsdlss *A = build(n, lmin, lmax);
    if (!A) { puts("out of memory building the graph"); return 1; }
    printf("# graph: n %lld, nnz %lld (grid 2%% + wires of %d..%d nodes, random numbering), built in %.1f s\n",
           (long long)n, (long long)A->p[n], lmin, lmax, now() - t0);
    double ts, tp;
    { const char *e = getenv("BENCH_RELABEL"); if (e) relabel_on = atoi(e); }
    int rt = 1;                    /* the reduction runs with the largest thread count tested */
    { char tb[256]; snprintf(tb, sizeof tb, "%s", tl);
      for (char *s = strtok(tb, " ,"); s; s = strtok(NULL, " ,")) if (atoi(s) > rt) rt = atoi(s); }
    vsdlss_set_num_threads(rt);
    printf("# reduction with %d threads\n", rt);
    csi *ids_s = NULL, *ids_p = NULL;
    vsdlss_reduction *rs = reduce(A, 0, &ts, &ids_s), *rp = reduce(A, 1, &tp, &ids_p);
    if (!rs || !rp) return 1;
    vsdlss_spfree(A);
    double tail = 0; csi nbn = 0;
    for (csi q = rp->blocks; q < rp->pk_count; q++) { tail += (double)rp->pk[q].count; nbn += rp->pk[q].nbn; }
    printf("# reduction: records %lld (%.1f%% of n), core %lld, blocks %lld, tail %.0f (%.1f%%) | "
           "level schedule: %lld levels, %.0f records per level | reduce %.1f s serial tail, %.1f s with schedule\n",
           (long long)rp->count, 100.0 * (double)rp->count / (double)n, (long long)rp->core_n, (long long)rp->blocks,
           tail, 100 * tail / (double)rp->count, (long long)rp->tail_levels,
           rp->tail_levels ? tail / (double)rp->tail_levels : 0, ts, tp);
    if (!rp->tail_levels) puts("# NOTE: no level schedule was built (tail too small or too deep)");
    printf("# solve-order relabel: %s (BENCH_RELABEL=0/1)\n", relabel_on ? "on, as in the M3 solve" : "off");
    double *w0 = malloc((size_t)n * 8), *w = malloc((size_t)n * 8), *ref = malloc((size_t)n * 8), *out = malloc((size_t)n * 8);
    double *in[2] = {malloc((size_t)n * 8), malloc((size_t)n * 8)};
    if (!w0 || !w || !ref || !out || !in[0] || !in[1]) return 1;
    for (csi i = 0; i < n; i++) w0[i] = sin(0.37 * (double)i) + 0.2;
    const csi *ids[2] = {ids_s, ids_p};     /* the input in each reduction's numbering */
    for (int c = 0; c < 2; c++) for (csi i = 0; i < n; i++) in[c][ids[c] ? ids[c][i] : i] = w0[i];
    char buf[256]; snprintf(buf, sizeof buf, "%s", tl);
    int first = 1, mismatch = 0;
    double base_f = 0, base_b = 0;
    printf("# times in ms, median of %d; speedup is against the serial tail at 1 thread\n", reps);
    for (char *tok = strtok(buf, " ,"); tok; tok = strtok(NULL, " ,")) {
        const int T = atoi(tok);
        vsdlss_set_num_threads(T);
        double med[2][2];
        for (int which = 0; which < 2; which++) {
            const vsdlss_reduction *r = which ? rp : rs;
            double tf[64], tb[64]; const int R = reps < 64 ? reps : 64;
            for (int k = -1; k < R; k++) {
                memcpy(w, in[which], (size_t)n * 8);
                double s = now(); vsdlss_reduce_forward_inplace(r, w, NULL); double f = now() - s;
                s = now(); vsdlss_reduce_backward_inplace(r, NULL, w); double b = now() - s;
                if (k >= 0) { tf[k] = f; tb[k] = b; }
            }
            qsort(tf, (size_t)R, 8, cmpd); qsort(tb, (size_t)R, 8, cmpd);
            med[which][0] = tf[R / 2]; med[which][1] = tb[R / 2];
            for (csi i = 0; i < n; i++) out[i] = w[ids[which] ? ids[which][i] : i];   /* original numbering */
            if (first && !which) memcpy(ref, out, (size_t)n * 8);
            else if (memcmp(ref, out, (size_t)n * 8)) mismatch = 1;
        }
        if (first) { base_f = med[0][0]; base_b = med[0][1]; first = 0; }
        printf("T=%-3d | serial tail: fwd %8.2f bwd %8.2f | level schedule: fwd %8.2f bwd %8.2f | speedup fwd %5.2fx bwd %5.2fx\n",
               T, 1e3 * med[0][0], 1e3 * med[0][1], 1e3 * med[1][0], 1e3 * med[1][1],
               base_f / med[1][0], base_b / med[1][1]);
        fflush(stdout);
    }
    printf("# results: %s\n", mismatch ? "DIFFERENT (bug)" : "bitwise identical across schedules and thread counts");
    vsdlss_reduction_free(rs); vsdlss_reduction_free(rp); free(w0); free(w); free(ref); free(out); free(in[0]); free(in[1]); free(ids_s); free(ids_p);
    return mismatch;
}
