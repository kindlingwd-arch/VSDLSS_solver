/* Exports the structure the pipelined-solve analysis needs
 * (tools/pipeline_analysis.py), so the analysis itself runs without the
 * solver: factor once, then for every component with a core
 *   - the core supernode tree: parent and stored entries of each supernode;
 *   - the replay units: the block segments of the parallel pass and the
 *     chunks of the tail schedule (one unit for an unscheduled tail), each
 *     with its record and neighbour counts and the core supernodes its
 *     records touch (forward: updates of those core columns; backward:
 *     reads of them).
 * Only reads the factor; the solver is not changed.
 *
 *   ./export_pipeline dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads out.txt
 *   build: make export_pipeline METIS=1 (order 6), or without METIS for order 5
 *
 * Format (text, one item per line):
 *   component <c> n <n> records <r> core <k> supernodes <S> units <U> blocks <B>
 *   S lines:  <parent or -1> <stored entries>
 *   U lines:  <B|T> <records> <neighbour entries> <m> <supernode> ... (m ids)
 * Units are listed in replay order: blocks (concurrent), then tail units. */
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

/* Records [i0, i1) of segment g (neighbours from offset o): writes one unit. */
static void unit(FILE *out, char kind, const vsdlss_pk_seg *g, csi i0, csi i1, csi o,
                 const int32_t *sn_of, int32_t *mark, int32_t stamp, int32_t *list)
{
    csi nb = 0, m = 0;
    for (csi i = i0; i < i1; i++) {
        const csi d = vsdlss_pk_degree(g, i);
        for (csi j = 0; j < d; j++) {
            const int32_t s = sn_of[g->nb[o + j]];
            if (s >= 0 && mark[s] != stamp) { mark[s] = stamp; list[m++] = s; }
        }
        o += d; nb += d;
    }
    fprintf(out, "%c %lld %lld %lld", kind, (long long)(i1 - i0), (long long)nb, (long long)m);
    for (csi k = 0; k < m; k++) fprintf(out, " %d", list[k]);
    fputc('\n', out);
}

int main(int argc, char **argv)
{
    if (argc < 5) { fprintf(stderr, "usage: export_pipeline dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads out.txt\n"); return 1; }
    const int order = atoi(argv[2]), T = atoi(argv[3]) > 0 ? atoi(argv[3]) : 1;
    interleave_memory();
    vsdlss *A = read_input(argv[1]);
    if (!A) { puts("input failed"); return 1; }
    vsdlss_set_num_threads(T);
    vsdlss_m3_factor *F = NULL; double t0 = now();
    const vsdlss_status st = vsdlss_factorize_m3(A, order, &F);
    if (st != VSDLSS_OK) {
        printf("factor failed: %s%s\n", vsdlss_status_string(st),
               st == VSDLSS_ERR_UNSUPPORTED && order == 6 ? " (order 6 needs make export_pipeline METIS=1)" : "");
        return 1;
    }
    printf("# %s: n %lld, order %d, %d threads, factor %.1f s, %lld components\n", argv[1], (long long)A->n, order, T,
           now() - t0, (long long)F->count);
    vsdlss_spfree(A);
    FILE *out = fopen(argv[4], "w");
    if (!out) { perror(argv[4]); return 1; }
    for (csi c = 0; c < F->count; c++) {
        const vsdlss_m3_component_factor *cf = F->component + c;
        const vsdlss_reduction *r = cf->reduction; const vsdlss_sn_factor *L = cf->numeric;
        if (!r || !L || !r->pk || r->records || cf->n < 100000) continue;
        const csi n = r->n, S = L->count;
        /* local vertex -> core supernode (-1: eliminated) */
        int32_t *sn_of = malloc((size_t)n * 4), *mark = malloc((size_t)S * 4), *list = malloc((size_t)S * 4);
        int32_t *col_sn = malloc((size_t)L->n * 4);
        if (!sn_of || !mark || !list || !col_sn) { puts("out of memory"); return 1; }
        for (csi s = 0; s < S; s++) { mark[s] = -1; for (csi k = L->column_start[s]; k < L->column_start[s + 1]; k++) col_sn[k] = (int32_t)s; }
        for (csi v = 0; v < n; v++) sn_of[v] = -1;
        for (csi k = 0; k < L->n; k++) {
            const csi v = cf->core_contig ? r->count + k : cf->core_map32 ? (csi)cf->core_map32[k] : cf->core_map ? cf->core_map[k] : -1;
            if (v < 0 || v >= n) { printf("# component %lld: core map unavailable, skipped\n", (long long)c); goto next; }
            sn_of[v] = col_sn[k];
        }
        {
            const int blocked = r->blocks >= 1 && r->pk_count == r->blocks + 1;
            const csi B = blocked ? r->blocks : 0;
            const vsdlss_pk_seg *tail = r->pk + B;
            const int win = blocked && r->tail_windows && r->tail_levels > 0 &&
                            tail->count == r->tail_chunk_rec[r->tail_level[r->tail_levels]];
            const csi tu = win ? r->tail_level[r->tail_levels] : (r->pk_count - B > 0 ? r->pk_count - B : 0);
            fprintf(out, "component %lld n %lld records %lld core %lld supernodes %lld units %lld blocks %lld\n",
                    (long long)c, (long long)n, (long long)r->count, (long long)L->n, (long long)S, (long long)(B + tu), (long long)B);
            for (csi s = 0; s < S; s++) {
                const csi w = L->column_start[s + 1] - L->column_start[s], e = L->row_ptr[s + 1] - L->row_ptr[s];
                fprintf(out, "%d %lld\n", L->sn_parent ? (int)L->sn_parent[s] : -1, (long long)(w * (w + 1) / 2 + w * e));
            }
            int32_t stamp = 0;
            for (csi b = 0; b < B; b++) unit(out, 'B', r->pk + b, 0, r->pk[b].count, 0, sn_of, mark, stamp++, list);
            if (win) {
                const csi *cr = r->tail_chunk_rec, *cn = r->tail_chunk_nb;
                for (csi u = 0; u < tu; u++) unit(out, 'T', tail, cr[u], cr[u + 1], cn[u], sn_of, mark, stamp++, list);
            } else
                for (csi q = B; q < r->pk_count; q++) unit(out, 'T', r->pk + q, 0, r->pk[q].count, 0, sn_of, mark, stamp++, list);
            printf("# component %lld: n %lld, records %lld, core %lld in %lld supernodes, %lld block segments, %lld tail units%s\n",
                   (long long)c, (long long)n, (long long)r->count, (long long)L->n, (long long)S, (long long)B, (long long)tu,
                   win ? " (window chunks)" : "");
        }
next:
        free(sn_of); free(mark); free(list); free(col_sn);
    }
    fclose(out);
    printf("# wrote %s\n", argv[4]);
    vsdlss_m3_factor_free(F);
    return 0;
}
