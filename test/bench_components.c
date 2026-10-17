/* Connected components + weighted adjacency (`vsdlss_components_build_graph`,
 * the "components" phase of the M3 factorization) in isolation.
 *
 *   ./bench_components SIDE[:CHAIN[:NETS]] "threads" [reps] [shuffle]
 *
 * Builds NETS (default 1) copies of a SIDE x SIDE grid whose edges are chains
 * of CHAIN (default 4) degree-2 nodes, like routed power straps; shuffle = 1
 * (default) relabels the vertices randomly, as customer netlists are.  The
 * phase is timed `reps` times per thread count (median and minimum), and
 * every result is checked bitwise against the first thread count.
 *
 *   ./bench_components 1500:4 "1 2 4" 5      # about 20M nodes, ~5 GB
 *   ./bench_components 700:4:2 "1 4" 7       # VDD/GND pair, about 9M nodes
 */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static uint64_t rng = 0x9E3779B97F4A7C15ULL;
static uint64_t rnd(void) { rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17; return rng; }
static int cmp_double(const void *x, const void *y) { double a = *(const double *)x, b = *(const double *)y; return (a > b) - (a < b); }

/* Upper CSC of NETS disjoint grid-of-chains Laplacians, built by counting
 * (no sort): column b holds rows a < b in increasing order, then b. */
static vsdlss *grid_chains(csi side, csi chain, int nets, int shuffle)
{
    csi per = side * side + 2 * side * (side - 1) * chain, n = per * nets;
    csi e = (csi)nets * 2 * side * (side - 1) * (chain + 1), k = 0;
    csi *ea = malloc((size_t)e * sizeof(csi)), *eb = malloc((size_t)e * sizeof(csi));
    csi *label = malloc((size_t)n * sizeof(csi));
    if (!ea || !eb || !label) return NULL;
    for (csi v = 0; v < n; v++) label[v] = v;
    if (shuffle) for (csi v = n - 1; v > 0; v--) { csi j = (csi)(rnd() % (uint64_t)(v + 1)), t = label[v]; label[v] = label[j]; label[j] = t; }
    for (int net = 0; net < nets; net++) {
        csi base = (csi)net * per, next = base + side * side;
        for (csi y = 0; y < side; y++) for (csi x = 0; x < side; x++) for (int dir = 0; dir < 2; dir++) {
            csi x2 = x + (dir == 0), y2 = y + (dir == 1), prev = base + y * side + x;
            if (x2 >= side || y2 >= side) continue;
            for (csi c = 0; c < chain; c++) { csi node = next++; ea[k] = prev; eb[k++] = node; prev = node; }
            ea[k] = prev; eb[k++] = base + y2 * side + x2;
        }
    }
    vsdlss *A = vsdlss_spalloc(n, n, n + e, 1, 0);
    double *diag = calloc((size_t)n, sizeof(double));
    if (!A || !diag) return NULL;
    memset(A->p, 0, (size_t)(n + 1) * sizeof(csi));
    for (csi q = 0; q < e; q++) {
        csi a = label[ea[q]], b = label[eb[q]];
        if (a > b) { csi t = a; a = b; b = t; }
        ea[q] = a; eb[q] = b; A->p[b + 1]++;
    }
    for (csi v = 0; v < n; v++) A->p[v + 1] += A->p[v] + 1;
    csi *at = malloc((size_t)n * sizeof(csi));
    if (!at) return NULL;
    for (csi v = 0; v < n; v++) at[v] = A->p[v];
    /* Rows ascending within a column: place edges by increasing a. */
    csi *cnt = calloc((size_t)n + 1, sizeof(csi)), *ord = malloc((size_t)e * sizeof(csi));
    if (!cnt || !ord) return NULL;
    for (csi q = 0; q < e; q++) cnt[ea[q] + 1]++;
    for (csi v = 0; v < n; v++) cnt[v + 1] += cnt[v];
    for (csi q = 0; q < e; q++) ord[cnt[ea[q]]++] = q;
    for (csi r = 0; r < e; r++) {
        csi q = ord[r], a = ea[q], b = eb[q];
        double w = 1.0 + 0.001 * (double)(q % 97);
        A->i[at[b]] = a; A->x[at[b]++] = -w;
        diag[a] += w; diag[b] += w;
    }
    for (csi v = 0; v < n; v++) { A->i[at[v]] = v; A->x[at[v]] = diag[v] + 1e-3; }
    free(ea); free(eb); free(label); free(diag); free(at); free(cnt); free(ord);
    return A;
}

static int same(const vsdlss_components *c1, const vsdlss_wgraph *g1,
                const vsdlss_components *c2, const vsdlss_wgraph *g2)
{
    csi n = c1->n;
    return c1->count == c2->count &&
        !memcmp(c1->offset, c2->offset, (size_t)(c1->count + 1) * sizeof(csi)) &&
        !memcmp(c1->vertices, c2->vertices, (size_t)n * sizeof(csi)) &&
        !memcmp(c1->component_of, c2->component_of, (size_t)n * sizeof(csi)) &&
        !memcmp(c1->local_of, c2->local_of, (size_t)n * sizeof(csi)) &&
        !memcmp(c1->order, c2->order, (size_t)n * sizeof(csi)) &&
        !memcmp(g1->ptr, g2->ptr, (size_t)(n + 1) * sizeof(csi)) &&
        !memcmp(g1->idx, g2->idx, (size_t)g1->ptr[n] * sizeof(csi)) &&
        !memcmp(g1->val, g2->val, (size_t)g1->ptr[n] * sizeof(double)) &&
        !memcmp(g1->diag, g2->diag, (size_t)n * sizeof(double));
}

int main(int argc, char **argv)
{
    if (argc < 3) { fprintf(stderr, "usage: bench_components SIDE[:CHAIN[:NETS]] \"threads\" [reps] [shuffle]\n"); return 1; }
    long side = 0, chain = 4, nets = 1;
    sscanf(argv[1], "%ld:%ld:%ld", &side, &chain, &nets);
    int reps = argc > 3 ? atoi(argv[3]) : 5, shuffle = argc > 4 ? atoi(argv[4]) : 1;
    if (side < 2 || chain < 0 || nets < 1 || reps < 1) { fprintf(stderr, "bad arguments\n"); return 1; }
    double t = now();
    vsdlss *A = grid_chains(side, chain, (int)nets, shuffle);
    if (!A) { fprintf(stderr, "out of memory\n"); return 1; }
    printf("# components n=%lld nnz_upper=%lld nets=%ld shuffle=%d gen=%.1fs\n",
           (long long)A->n, (long long)A->p[A->n], nets, shuffle, now() - t);
    printf("%-8s %10s %10s %s\n", "threads", "median_s", "min_s", "check");
    vsdlss_components *ref = NULL; vsdlss_wgraph *gref = NULL;
    char *list = strdup(argv[2]);
    for (char *tok = strtok(list, " ,"); tok; tok = strtok(NULL, " ,")) {
        int th = atoi(tok);
        if (vsdlss_set_num_threads(th) != VSDLSS_OK) { fprintf(stderr, "bad thread count %d\n", th); return 1; }
        double *ts = malloc((size_t)reps * sizeof(double)); int ok = 1;
        for (int r = 0; r < reps; r++) {
            vsdlss_components *c = NULL; vsdlss_wgraph *g = NULL;
            t = now();
            if (vsdlss_components_build_graph(A, &c, &g) != VSDLSS_OK) { fprintf(stderr, "build failed\n"); return 1; }
            ts[r] = now() - t;
            if (!ref) { ref = c; gref = g; continue; }
            if (r == 0 && !same(ref, gref, c, g)) ok = 0;
            vsdlss_components_free(c); vsdlss_wgraph_free(g);
        }
        qsort(ts, (size_t)reps, sizeof(double), cmp_double);
        printf("%-8d %10.3f %10.3f %s (components %lld)\n", th, ts[reps / 2], ts[0],
               ok ? "bitwise" : "MISMATCH", (long long)ref->count);
        free(ts);
        if (!ok) return 2;
    }
    free(list);
    return 0;
}
