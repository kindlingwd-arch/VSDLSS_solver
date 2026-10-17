/* Solve-phase micro-benchmark: times the M3 solve phases of one component in
 * isolation, so a change to one phase is measured without the rest of the
 * solve (and without a second factorization per variant).
 *
 *   ./bench_phase dump.bin            order threads_list [reps]
 *   ./bench_phase grid:SIDE[:CHAIN]   order threads_list [reps]
 *   ./bench_phase text:DIR            order threads_list [reps]   (DIR/diag.txt, DIR/data.txt)
 *   build: make bench_phase METIS=1 (order 6), or without METIS for order 5
 *
 * dump.bin is a PG_DUMP system (int64 n, nnz, upper CSC p, i, x, b).
 * grid:SIDE builds a SIDE x SIDE grid whose edges are chains of CHAIN
 * (default 3) intermediate nodes, a power-grid-like mix of degree 2 and 4;
 * small sides keep the working set in cache ("hot"), which removes the
 * memory-bandwidth ceiling of a laptop and exposes scheduling and serial
 * costs that dominate on a high-bandwidth server.
 *
 * The largest component is factored once with `order`; then for each thread
 * count in threads_list ("1 2 4 8"), the median over reps of:
 *   replay fwd / bwd       vsdlss_reduce_{forward,backward}_inplace, all records
 *   blocks fwd / bwd       the parallel block segments only
 *   tail fwd / bwd         the sequential tail segment only
 *   core fwd+bwd           vsdlss_sn_solve_inplace on the core
 * with the bytes each phase streams (records or L) per millisecond.  The
 * vector is reset (outside the timing) before every repetition.  Results
 * are bitwise compared across thread counts (bwd after fwd, core). */
#define _GNU_SOURCE   /* getline, syscall */
#include "../src/vsdlss_m3_internal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#if defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#endif

#include "bench_input.h"

/* Level structure of the sequential tail (segments q >= blocks): a record's
 * level is one more than the last level touching its pivot or a neighbour, so
 * the records of one level touch disjoint vertices and could be replayed
 * concurrently.  Prints the depth and how parallel the levels are. */
static void tail_levels(const vsdlss_reduction *r)
{
    double tail = 0;
    for (csi q = r->blocks; q < r->pk_count; q++) tail += (double)r->pk[q].count;
    if (tail < 1) { puts("# tail: empty"); return; }
    if (r->tail_windows) {           /* the stored order is by component: levels would mean nothing here */
        const csi chunks = r->tail_level[r->tail_levels];
        printf("# tail: %.0f records, window schedule: %lld windows, %lld chunks (%.0f records each), "
               "deferred core updates %lld of %lld (%lld core vertices)\n",
               tail, (long long)r->tail_levels, (long long)chunks, tail / (double)(chunks ? chunks : 1),
               (long long)r->tail_core_slots, (long long)r->pk[r->blocks].nbn, (long long)r->tail_core_groups);
        return;
    }
    int *last = calloc((size_t)r->n, sizeof(int)), *lv = malloc((size_t)tail * sizeof(int));
    if (!last || !lv) { free(last); free(lv); puts("# tail levels: out of memory"); return; }
    csi k = 0; int maxl = 0;
    for (csi q = r->blocks; q < r->pk_count; q++) {
        const vsdlss_pk_seg *g = r->pk + q; csi o = 0;
        for (csi i = 0; i < g->count; i++) {
            const csi v = vsdlss_pk_vertex(g, i), d = vsdlss_pk_degree(g, i);
            int l = last[v];
            for (csi j = 0; j < d; j++) if (last[g->nb[o + j]] > l) l = last[g->nb[o + j]];
            l++; last[v] = l; for (csi j = 0; j < d; j++) last[g->nb[o + j]] = l;
            lv[k++] = l; if (l > maxl) maxl = l; o += d;
        }
    }
    long *sz = calloc((size_t)maxl + 1, sizeof(long));
    if (!sz) { free(last); free(lv); return; }
    for (csi i = 0; i < k; i++) sz[lv[i]]++;
    double big = 0; static const int P[4] = {8, 16, 32, 64}; double steps[4] = {0};
    for (int l = 1; l <= maxl; l++) {
        if (sz[l] >= 256) big += (double)sz[l];
        for (int p = 0; p < 4; p++) steps[p] += (double)((sz[l] + P[p] - 1) / P[p]);
    }
    printf("# tail: %.0f records in %d levels (%.0f per level on average), %.1f%% of them in levels of >= 256 | "
           "ideal speedup of a level-parallel tail: 8 thr %.1fx, 16 thr %.1fx, 32 thr %.1fx, 64 thr %.1fx\n",
           tail, maxl, tail / maxl, 100 * big / tail, tail / steps[0], tail / steps[1], tail / steps[2], tail / steps[3]);
    free(last); free(lv); free(sz);
}

/* Dependency statistics of the tail under three conflict rules, for the
 * window schedule (VSDLSS_TAIL_WINDOWS):
 *   all  -- two records conflict when they touch a common vertex (current);
 *   fwd  -- forward replay with the updates of core vertices deferred (applied
 *           afterwards in record order): core vertices no longer link records;
 *   bwd  -- backward replay: a record only waits for the records whose pivots
 *           it reads; records reading a common value do not conflict.
 * Prints the depth under each rule and the number of windows the greedy
 * window builder would make (largest component <= 1/K of the window). */
static uint32_t df_find(uint32_t *p, uint32_t v) { while (p[v] != v) { p[v] = p[p[v]]; v = p[v]; } return v; }
static long tail_windows_count(const vsdlss_pk_seg *g, const csi *nbo, const int *lev, int maxl, const int *owner,
                               const unsigned char *core, int rule, const int *lb, csi K, csi n)
{
    const csi cnt = g->count;
    csi *start = calloc((size_t)maxl + 2, sizeof(csi)), *ord = malloc((size_t)cnt * sizeof(csi));
    uint32_t *par = malloc((size_t)n * 4), *sz = malloc((size_t)n * 4), *st = calloc((size_t)n, 4);
    long windows = -1;
    if (!start || !ord || !par || !sz || !st) goto out;
    for (csi i = 0; i < cnt; i++) start[lev[i] + 1]++;
    for (int l = 1; l <= maxl + 1; l++) start[l] += start[l - 1];
    { csi *f = malloc((size_t)(maxl + 2) * sizeof(csi)); if (!f) goto out;
      memcpy(f, start, (size_t)(maxl + 2) * sizeof(csi));
      for (csi i = 0; i < cnt; i++) ord[f[lev[i]]++] = i;
      free(f); }
    uint32_t W = 0; windows = 0;
#define TCH(v) do { if (st[v] != W) { st[v] = W; par[v] = (uint32_t)(v); sz[v] = 0; } } while (0)
    for (int a = 1; a <= maxl;) {
        int b; csi total = 0; uint32_t maxc = 0;
        W++;
        {
            for (b = a; b <= maxl; b++) {
                for (csi j = start[b]; j < start[b + 1]; j++) {
                    const csi i = ord[j], p = vsdlss_pk_vertex(g, i), d = vsdlss_pk_degree(g, i);
                    TCH(p); uint32_t r0 = df_find(par, (uint32_t)p);
                    for (csi k = 0; k < d; k++) {
                        const csi v = g->nb[nbo[i] + k];
                        if (rule >= 1 && core[v]) continue;
                        if (rule == 2 && (owner[v] < 0 || lb[owner[v]] < a)) continue;
                        TCH(v); uint32_t r1 = df_find(par, (uint32_t)v);
                        if (r1 == r0) continue;
                        if (sz[r0] < sz[r1]) { uint32_t x = r0; r0 = r1; r1 = x; }
                        par[r1] = r0; sz[r0] += sz[r1];
                    }
                    if (++sz[r0] > maxc) maxc = sz[r0];
                }
                total += start[b + 1] - start[b];
                if (b > a && (csi)maxc * K > total) break;   /* the window is [a, b) */
            }
        }
        windows++; a = b;
    }
#undef TCH
out:
    free(start); free(ord); free(par); free(sz); free(st);
    return windows;
}
static void tail_deps(const vsdlss_reduction *r)
{
    if (r->pk_count != r->blocks + 1 || r->blocks < 1) return;
    const vsdlss_pk_seg *g = r->pk + r->blocks; const csi cnt = g->count, n = r->n;
    if (cnt < 2) return;
    int *owner = malloc((size_t)n * sizeof(int)), *la = calloc((size_t)n, sizeof(int)), *lf = calloc((size_t)n, sizeof(int));
    int *lev_a = malloc((size_t)cnt * sizeof(int)), *lev_f = malloc((size_t)cnt * sizeof(int)), *lev_b = malloc((size_t)cnt * sizeof(int));
    unsigned char *core = malloc((size_t)n); csi *nbo = malloc((size_t)(cnt + 1) * sizeof(csi));
    if (!owner || !la || !lf || !lev_a || !lev_f || !lev_b || !core || !nbo) { puts("# tail deps: out of memory"); goto out; }
    memset(core, 1, (size_t)n);
    for (csi v = 0; v < n; v++) owner[v] = -1;
    for (csi q = 0; q < r->pk_count; q++) for (csi i = 0; i < r->pk[q].count; i++) core[vsdlss_pk_vertex(r->pk + q, i)] = 0;
    nbo[0] = 0;
    for (csi i = 0; i < cnt; i++) { owner[vsdlss_pk_vertex(g, i)] = (int)i; nbo[i + 1] = nbo[i] + vsdlss_pk_degree(g, i); }
    int ma = 0, mf = 0, mb = 0;
    for (csi i = 0; i < cnt; i++) {
        const csi p = vsdlss_pk_vertex(g, i), d = vsdlss_pk_degree(g, i);
        int a = la[p], f = lf[p];
        for (csi k = 0; k < d; k++) { const csi v = g->nb[nbo[i] + k]; if (la[v] > a) a = la[v]; if (!core[v] && lf[v] > f) f = lf[v]; }
        a++; f++; la[p] = a; lf[p] = f;
        for (csi k = 0; k < d; k++) { const csi v = g->nb[nbo[i] + k]; la[v] = a; if (!core[v]) lf[v] = f; }
        lev_a[i] = a; lev_f[i] = f; if (a > ma) ma = a; if (f > mf) mf = f;
    }
    for (csi i = cnt; i > 0;) {                     /* backward: wait for the owners of what i reads */
        i--; int b = 0; const csi d = vsdlss_pk_degree(g, i);
        for (csi k = 0; k < d; k++) { const int o = owner[g->nb[nbo[i] + k]]; if (o >= 0 && lev_b[o] > b) b = lev_b[o]; }
        lev_b[i] = b + 1; if (b + 1 > mb) mb = b + 1;
    }
    printf("# tail deps: depth all %d, fwd (core deferred) %d, bwd (reads) %d", ma, mf, mb);
    for (csi K = 64; K <= 256; K *= 4)
        printf(" | windows K=%lld: all %ld, fwd %ld, bwd %ld", (long long)K,
               tail_windows_count(g, nbo, lev_a, ma, owner, core, 0, lev_b, K, n),
               tail_windows_count(g, nbo, lev_f, mf, owner, core, 1, lev_b, K, n),
               tail_windows_count(g, nbo, lev_b, mb, owner, core, 2, lev_b, K, n));
    printf("\n");
out:
    free(owner); free(la); free(lf); free(lev_a); free(lev_f); free(lev_b); free(core); free(nbo);
}

/* A view of the reduction with only the block segments (part 1) or only the
 * tail (part 2) non-empty; the shape checks still pass. */
static vsdlss_reduction view(const vsdlss_reduction *r, int part, vsdlss_pk_seg *seg, csi *bp)
{
    vsdlss_reduction v = *r;
    for (csi q = 0; q < r->pk_count; q++) {
        int keep = part == 0 || (part == 1 && q < r->blocks) || (part == 2 && q >= r->blocks);
        seg[q] = r->pk[q];
        if (!keep) { seg[q].count = 0; seg[q].nbn = 0; }
    }
    for (csi b = 0; b <= r->blocks; b++) bp[b] = part == 2 ? 0 : r->block_ptr[b];
    if (part == 1 && r->blocks) bp[r->blocks] = r->block_ptr[r->blocks];
    v.pk = seg; v.block_ptr = bp;
    return v;
}

#define MAXT 16
int main(int argc, char **argv)
{
    if (argc < 4) { fprintf(stderr, "usage: bench_phase dump.bin|grid:SIDE[:CHAIN]|text:DIR order \"threads\" [reps]\n"); return 1; }
    const int order = atoi(argv[2]), reps = argc > 4 ? atoi(argv[4]) : 15;
    interleave_memory();                     /* before the large allocations */
    vsdlss *A = read_input(argv[1]);
    if (!A) { puts("input failed"); return 1; }
    int threads[MAXT], nth = 0; char buf[256]; strncpy(buf, argv[3], 255); buf[255] = 0;
    for (char *t = strtok(buf, " ,"); t && nth < MAXT; t = strtok(NULL, " ,")) threads[nth++] = atoi(t);
    int tmax = 1; for (int i = 0; i < nth; i++) if (threads[i] > tmax) tmax = threads[i];
    vsdlss_set_num_threads(tmax);
    vsdlss_m3_factor *F = NULL; double t0 = now();
    if (vsdlss_factorize_m3(A, order, &F) != VSDLSS_OK) { puts("factor failed"); return 1; }
    const double tf = now() - t0;
    csi best = -1;
    for (csi c = 0; c < F->count; c++) if (F->component[c].reduction && (best < 0 || F->component[c].n > F->component[best].n)) best = c;
    if (best < 0) { puts("no reduced component"); return 1; }
    const vsdlss_m3_component_factor *cf = F->component + best;
    const vsdlss_reduction *r = cf->reduction; const vsdlss_sn_factor *sf = cf->numeric;
    if (!r->pk || r->records) { puts("reduction not packed"); return 1; }
    const csi n = r->n;
    double tail_rec = 0, rec_bytes[3] = {0};
    for (csi q = 0; q < r->pk_count; q++) {
        double b = (double)r->pk[q].count * (r->pk[q].head ? 12 : 9) + (double)r->pk[q].nbn * 12;
        rec_bytes[0] += b; rec_bytes[q < r->blocks ? 1 : 2] += b;
        if (q >= r->blocks) tail_rec += (double)r->pk[q].count;
    }
    double lbytes = sf ? (double)sf->panel_offset[sf->count] * 8 * 2 : 0;
    printf("# %s: n %lld (component %lld of %lld, n %lld), factor %.1f s | records %lld, blocks %lld, tail %.0f (%.1f%%) | core %lld, L %.0f MB\n",
           argv[1], (long long)A->n, (long long)best, (long long)F->count, (long long)n, tf, (long long)r->count,
           (long long)r->blocks, tail_rec, 100 * tail_rec / (double)(r->count ? r->count : 1),
           (long long)(sf ? sf->n : 0), sf ? (double)sf->panel_offset[sf->count] * 8 / 1e6 : 0);
    for (csi c = 0; c < F->count; c++)        /* every large component, not just the measured one */
        if (F->component[c].reduction && F->component[c].n >= 100000)
            printf("# component %lld: n %lld, records %lld (%.1f%% of n), core %lld\n", (long long)c,
                   (long long)F->component[c].n, (long long)F->component[c].reduction->count,
                   100.0 * (double)F->component[c].reduction->count / (double)F->component[c].n,
                   (long long)F->component[c].reduction->core_n);
    tail_levels(r);
    if (getenv("BENCH_TAIL_DEPS") && !r->tail_windows) tail_deps(r);
    fflush(stdout);

    vsdlss_pk_seg *seg = malloc((size_t)r->pk_count * sizeof(*seg)); csi *bp = malloc((size_t)(r->blocks + 1) * sizeof(csi));
    double *w0 = malloc((size_t)n * 8), *w = malloc((size_t)n * 8), *ref = malloc((size_t)n * 8);
    const csi cn = sf ? sf->n : 0;
    double *c0 = malloc((size_t)(cn ? cn : 1) * 8), *cx = malloc((size_t)(cn ? cn : 1) * 8), *cref = malloc((size_t)(cn ? cn : 1) * 8);
    double ts[64];
    if (!seg || !bp || !w0 || !w || !ref || !c0 || !cx || !cref) { puts("alloc failed"); return 1; }
    for (csi i = 0; i < n; i++) w0[i] = sin(0.37 * (double)i) + 0.2;
    for (csi i = 0; i < cn; i++) c0[i] = cos(0.11 * (double)i) + 0.3;
    static const char *name[3] = {"replay", "blocks", "tail"};
    int mismatch = 0;
    for (int ti = 0; ti < nth; ti++) {
        vsdlss_set_num_threads(threads[ti]);
        printf("T=%-2d", threads[ti]);
        for (int part = 0; part < 3; part++) {
            vsdlss_reduction v = view(r, part, seg, bp);
            if (part && !r->blocks) continue;
            double med[2];
            for (int dir = 0; dir < 2; dir++) {
                int R = reps < 64 ? reps : 64;
                for (int k = -1; k < R; k++) {
                    memcpy(w, w0, (size_t)n * 8);
                    if (dir) vsdlss_reduce_forward_inplace(&v, w, NULL);
                    double s = now();
                    if (dir) vsdlss_reduce_backward_inplace(&v, NULL, w);
                    else vsdlss_reduce_forward_inplace(&v, w, NULL);
                    if (k >= 0) ts[k] = now() - s;
                }
                qsort(ts, (size_t)R, 8, cmpd); med[dir] = ts[R / 2];
            }
            if (part == 0) {     /* full replay result, compared across thread counts */
                if (ti == 0) memcpy(ref, w, (size_t)n * 8); else mismatch |= memcmp(ref, w, (size_t)n * 8) != 0;
            }
            printf(" | %s fwd %7.2f bwd %7.2f ms (%4.1f GB/s)", name[part], 1e3 * med[0], 1e3 * med[1],
                   rec_bytes[part] * 2 / (med[0] + med[1]) / 1e9);
        }
        if (sf) {
            int R = reps < 64 ? reps : 64;
            for (int k = -1; k < R; k++) {
                memcpy(cx, c0, (size_t)cn * 8);
                double s = now(); vsdlss_sn_solve_inplace(sf, cx); if (k >= 0) ts[k] = now() - s;
            }
            qsort(ts, (size_t)R, 8, cmpd);
            if (ti == 0) memcpy(cref, cx, (size_t)cn * 8); else mismatch |= memcmp(cref, cx, (size_t)cn * 8) != 0;
            printf(" | core fwd+bwd %7.2f ms (%4.1f GB/s)", 1e3 * ts[R / 2], lbytes / ts[R / 2] / 1e9);
        }
        printf("\n");
    }
    printf("# results across thread counts: %s\n", mismatch ? "DIFFERENT" : "bitwise identical");
    vsdlss_m3_factor_free(F); vsdlss_spfree(A);
    free(seg); free(bp); free(w0); free(w); free(ref); free(c0); free(cx); free(cref);
    return mismatch;
}
