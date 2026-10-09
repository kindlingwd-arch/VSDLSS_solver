/* Solve-phase micro-benchmark: times the M3 solve phases of one component in
 * isolation, so a change to one phase is measured without the rest of the
 * solve (and without a second factorization per variant).
 *
 *   ./bench_phase dump.bin            order threads_list [reps]
 *   ./bench_phase grid:SIDE[:CHAIN]   order threads_list [reps]
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
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static int cmpd(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }

typedef struct { csi a, b; double w; } gedge;
static int cmp_edge(const void *x, const void *y)
{
    const gedge *p = x, *q = y;
    if (p->b != q->b) return (p->b > q->b) - (p->b < q->b);
    return (p->a > q->a) - (p->a < q->a);
}

/* Upper CSC of the grid-of-chains Laplacian, slightly grounded. */
static vsdlss *grid_chains(csi side, csi chain)
{
    csi n = side * side, e = 0, maxe = 2 * side * (side - 1) * (chain + 1);
    gedge *ed = malloc((size_t)maxe * sizeof(*ed));
    if (!ed) return NULL;
    for (csi y = 0; y < side; y++) for (csi x = 0; x < side; x++) for (int dir = 0; dir < 2; dir++) {
        csi x2 = x + (dir == 0), y2 = y + (dir == 1), prev = y * side + x;
        if (x2 >= side || y2 >= side) continue;
        for (csi c = 0; c < chain; c++) { csi node = n++; ed[e++] = (gedge){prev, node, 1.0 + 0.001 * (double)(e % 97)}; prev = node; }
        ed[e++] = (gedge){prev, y2 * side + x2, 0.5 + 0.002 * (double)(e % 89)};
    }
    for (csi k = 0; k < e; k++) if (ed[k].a > ed[k].b) { csi t = ed[k].a; ed[k].a = ed[k].b; ed[k].b = t; }
    qsort(ed, (size_t)e, sizeof(*ed), cmp_edge);
    double *diag = calloc((size_t)n, sizeof(double));
    vsdlss *A = vsdlss_spalloc(n, n, n + e, 1, 0);
    if (!diag || !A) { free(ed); free(diag); return NULL; }
    for (csi k = 0; k < e; k++) { diag[ed[k].a] += ed[k].w; diag[ed[k].b] += ed[k].w; }
    csi nz = 0, k = 0;
    for (csi col = 0; col < n; col++) {
        A->p[col] = nz;
        for (; k < e && ed[k].b == col; k++) { A->i[nz] = ed[k].a; A->x[nz++] = -ed[k].w; }
        A->i[nz] = col; A->x[nz++] = diag[col] + 1e-3;
    }
    A->p[n] = nz;
    free(ed); free(diag);
    return A;
}

static vsdlss *read_dump(const char *path)
{
    FILE *fp = fopen(path, "rb"); int64_t n, nz;
    if (!fp || fread(&n, 8, 1, fp) != 1 || fread(&nz, 8, 1, fp) != 1) { if (fp) fclose(fp); return NULL; }
    vsdlss *A = vsdlss_spalloc(n, n, nz, 1, 0);
    if (!A || fread(A->p, 8, (size_t)n + 1, fp) != (size_t)n + 1 || fread(A->i, 8, (size_t)nz, fp) != (size_t)nz ||
        fread(A->x, 8, (size_t)nz, fp) != (size_t)nz) { fclose(fp); return NULL; }
    fclose(fp);
    return A;
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
    if (argc < 4) { fprintf(stderr, "usage: bench_phase dump.bin|grid:SIDE[:CHAIN] order \"threads\" [reps]\n"); return 1; }
    const int order = atoi(argv[2]), reps = argc > 4 ? atoi(argv[4]) : 15;
    vsdlss *A = NULL;
    if (!strncmp(argv[1], "grid:", 5)) {
        csi side = atol(argv[1] + 5), chain = 3; const char *c = strchr(argv[1] + 5, ':');
        if (c) chain = atol(c + 1);
        A = grid_chains(side, chain);
    } else A = read_dump(argv[1]);
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
        double b = (double)r->pk[q].count * 12 + (double)r->pk[q].nbn * 12;
        rec_bytes[0] += b; rec_bytes[q < r->blocks ? 1 : 2] += b;
        if (q >= r->blocks) tail_rec += (double)r->pk[q].count;
    }
    double lbytes = sf ? (double)sf->panel_offset[sf->count] * 8 * 2 : 0;
    printf("# %s: n %lld (component %lld of %lld, n %lld), factor %.1f s | records %lld, blocks %lld, tail %.0f (%.1f%%) | core %lld, L %.0f MB\n",
           argv[1], (long long)A->n, (long long)best, (long long)F->count, (long long)n, tf, (long long)r->count,
           (long long)r->blocks, tail_rec, 100 * tail_rec / (double)(r->count ? r->count : 1),
           (long long)(sf ? sf->n : 0), sf ? (double)sf->panel_offset[sf->count] * 8 / 1e6 : 0);

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
