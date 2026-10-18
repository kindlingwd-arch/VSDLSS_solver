/* Neighbour-index statistics of the packed low-degree records, for a
 * smaller record format: factor once (as in production), then for every
 * component with a solve-order relabel (record i of a segment eliminates
 * vertex k = k0 + i) the distance d = nb - k of each neighbour index:
 *   - neighbours that are records (later eliminated vertices, d > 0) and
 *     neighbours in the core (vertex >= records count);
 *   - how many fit in 8 / 16 bits, and how many are the next record (d = 1);
 * and the bytes the replays stream per record now (degree byte, pivot,
 * 4-byte index and 8-byte multiplier per neighbour) against the same with
 * 16-bit distances (core neighbours and longer distances kept at 4 bytes,
 * one flag bit each).  Only reads the factor; the solver is not changed.
 *
 *   ./nb_stats dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads
 *   build: make nb_stats METIS=1 (order 6), or without METIS for order 5 */
#define _GNU_SOURCE
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

typedef struct {
    long long records, deg[5], nbs, core, next, fit8, fit16, far;
    long long by_deg_next[5];      /* records of degree d with a neighbour d = 1 */
} stats;

static void count_seg(const vsdlss_pk_seg *g, csi count, stats *s)
{
    csi o = 0;
    for (csi i = 0; i < g->count; i++) {
        const csi d = vsdlss_pk_degree(g, i), k = g->k0 + i;
        int has_next = 0;
        s->records++; s->deg[d < 4 ? d : 4]++;
        for (csi j = 0; j < d; j++) {
            const csi v = (csi)g->nb[o + j], dist = v - k;
            s->nbs++;
            if (v >= count) { s->core++; continue; }
            if (dist == 1) { s->next++; has_next = 1; }
            if (dist > 0 && dist < 256) s->fit8++;
            if (dist > 0 && dist < 65536) s->fit16++; else s->far++;
        }
        if (has_next) s->by_deg_next[d < 4 ? d : 4]++;
        o += d;
    }
}

static void print_stats(const char *name, const stats *s)
{
    const double R = s->records ? (double)s->records : 1, N = s->nbs ? (double)s->nbs : 1;
    printf("  %s: %lld records (degree 0/1/2/3/4+: %.1f / %.1f / %.1f / %.1f / %.1f %%), %lld neighbours (%.2f per record)\n",
           name, s->records, 100 * s->deg[0] / R, 100 * s->deg[1] / R, 100 * s->deg[2] / R, 100 * s->deg[3] / R,
           100 * s->deg[4] / R, s->nbs, s->nbs / R);
    printf("    neighbour in the core %.1f %% | record neighbours: next record (d = 1) %.1f %%, d < 2^8 %.1f %%, d < 2^16 %.1f %%, farther %.1f %%\n",
           100 * s->core / N, 100 * s->next / N, 100 * s->fit8 / N, 100 * s->fit16 / N, 100 * s->far / N);
    printf("    records with the next record as a neighbour: degree 1 %.1f %%, degree 2 %.1f %%, degree 3 %.1f %%\n",
           s->deg[1] ? 100.0 * s->by_deg_next[1] / s->deg[1] : 0, s->deg[2] ? 100.0 * s->by_deg_next[2] / s->deg[2] : 0,
           s->deg[3] ? 100.0 * s->by_deg_next[3] / s->deg[3] : 0);
    /* bytes streamed by the backward replay (forward: the same less the pivot) */
    const double now = 9.0 * s->records + 12.0 * s->nbs;
    const double idx16 = 9.0 * s->records + 8.0 * s->nbs + 2.0 * s->fit16 + 4.0 * (s->nbs - s->fit16) + s->nbs / 8.0;
    printf("    bytes per record (backward): now %.1f, 16-bit distances %.1f (%.1f %% less)\n",
           now / R, idx16 / R, 100 * (now - idx16) / now);
}

int main(int argc, char **argv)
{
    if (argc < 4) { fprintf(stderr, "usage: nb_stats dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads\n"); return 1; }
    const int order = atoi(argv[2]), T = atoi(argv[3]) > 0 ? atoi(argv[3]) : 1;
    interleave_memory();
    vsdlss *A = read_input(argv[1]);
    if (!A) { puts("input failed"); return 1; }
    vsdlss_set_num_threads(T);
    vsdlss_m3_factor *F = NULL; double t0 = now();
    const vsdlss_status st = vsdlss_factorize_m3(A, order, &F);
    if (st != VSDLSS_OK) {
        printf("factor failed: %s%s\n", vsdlss_status_string(st),
               st == VSDLSS_ERR_UNSUPPORTED && order == 6 ? " (order 6 needs make nb_stats METIS=1)" : "");
        return 1;
    }
    printf("# %s: n %lld, order %d, %d threads, factor %.1f s, %lld components\n", argv[1], (long long)A->n, order, T,
           now() - t0, (long long)F->count);
    vsdlss_spfree(A);
    stats all; memset(&all, 0, sizeof all);
    for (csi c = 0; c < F->count; c++) {
        const vsdlss_reduction *r = F->component[c].reduction;
        if (!r || !r->pk || r->records || F->component[c].n < 100000) continue;
        if (r->pk[0].head) { printf("# component %lld: no solve-order relabel, skipped\n", (long long)c); continue; }
        const int blocked = r->blocks >= 1 && r->pk_count == r->blocks + 1;
        stats blk, tail; memset(&blk, 0, sizeof blk); memset(&tail, 0, sizeof tail);
        for (csi q = 0; q < r->pk_count; q++) count_seg(r->pk + q, r->count, blocked && q < r->blocks ? &blk : &tail);
        printf("component %lld: n %lld, records %lld, core %lld\n", (long long)c, (long long)r->n,
               (long long)r->count, (long long)(r->n - r->count));
        if (blk.records) print_stats("block segments", &blk);
        print_stats(blocked ? "tail" : "records", &tail);
        long long *a = &all.records, *b = &blk.records, *t = &tail.records;
        for (size_t k = 0; k < sizeof all / sizeof(long long); k++) a[k] += b[k] + t[k];
    }
    if (all.records) { printf("all components:\n"); print_stats("all records", &all); }
    vsdlss_m3_factor_free(F);
    return 0;
}
