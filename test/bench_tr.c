/* Transient stepping benchmark: vsdlss_m3_transient against solving each
 * step separately.
 *
 *   ./bench_tr <dump.bin | grid:SIDE[:CHAIN] | text:DIR> [order=5] [threads=1] [steps=20]
 *   env: TR_SRC (fraction of nodes with a source, default 0.3), TR_CH (C/h as
 *        a fraction of A's diagonal, default 0.1), TR_ROUNDS (default 3),
 *        TR_BLOCK (reduction block, default 65536)
 *
 * The factor is of the input A; Ch = TR_CH * diag(A) (grounded capacitors),
 * sources at a random TR_SRC of the nodes.  Per step, median over rounds:
 *   orig     b in caller order, vsdlss_m3_solve (gather + scatter)
 *   packed   b in packed order, vsdlss_m3_solve_packed_inplace
 *   tr       vsdlss_m3_transient_run, VSDLSS_TR_FUSE off
 *   tr+fuse  vsdlss_m3_transient_run, fused
 * and whether the final solutions agree bitwise. */
#define _GNU_SOURCE
#include "../include/vsdlss.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include "bench_input.h"

extern int vsdlss_m3_transient_fuse;
extern csi vsdlss_reduce_block;

static unsigned long long rng = 88172645463325252ULL;
static double urand(void) { rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17; return (double)(rng >> 11) / 9007199254740992.0; }
static double envd(const char *name, double def) { const char *e = getenv(name); return e && *e ? atof(e) : def; }

static void no_cb(void *ctx, csi k, const double *x, double *dep) { (void)ctx; (void)k; (void)x; (void)dep; }

int main(int argc, char **argv)
{
    if (argc < 2) { fprintf(stderr, "usage: %s <dump.bin | grid:SIDE[:CHAIN] | text:DIR> [order] [threads] [steps]\n", argv[0]); return 2; }
    const int order = argc > 2 ? atoi(argv[2]) : 5, threads = argc > 3 ? atoi(argv[3]) : 1;
    const csi K = argc > 4 ? atol(argv[4]) : 20;
    const double fsrc = envd("TR_SRC", 0.3), fch = envd("TR_CH", 0.1);
    const int rounds = (int)envd("TR_ROUNDS", 3);
    /* TR_BLOCK: block of the parallel reduction pass (default 65536).  A
     * small block leaves most records to the tail, as on grids whose BFS
     * front is wider than the block (the 61M-node EMIR case: 99.6% tail). */
    if (getenv("TR_BLOCK")) vsdlss_reduce_block = (csi)envd("TR_BLOCK", 65536);
    vsdlss_set_num_threads(threads);
    vsdlss *A = read_input(argv[1]);
    if (!A) { fprintf(stderr, "cannot read %s\n", argv[1]); return 1; }
    const csi n = A->n;
    /* Ch: fch * diag(A) */
    vsdlss *Ch = vsdlss_spalloc(n, n, n, 1, 0);
    for (csi j = 0; j < n; j++) {
        Ch->p[j] = j; Ch->i[j] = j; Ch->x[j] = 0;
        for (csi p = A->p[j]; p < A->p[j + 1]; p++) if (A->i[p] == j) Ch->x[j] = fch * A->x[p];
    }
    Ch->p[n] = n;
    csi nsrc = 0, *src = malloc((size_t)n * sizeof(csi));
    for (csi i = 0; i < n; i++) if (urand() < fsrc) src[nsrc++] = i;
    double *sv = malloc((size_t)(nsrc > 0 && K > 0 ? K * nsrc : 1) * sizeof(double));
    for (csi t = 0; t < K * nsrc; t++) sv[t] = 1e-3 * (urand() - 0.3);
    printf("# input %s: n %lld, order %d, threads %d, %lld steps, %lld sources, rounds %d\n",
           argv[1], (long long)n, order, threads, (long long)K, (long long)nsrc, rounds);
    double t0 = now();
    vsdlss_m3_factor *F = NULL;
    if (vsdlss_factorize_m3(A, order, &F) != VSDLSS_OK) { puts("factorize failed"); return 1; }
    printf("factorize %.3f s\n", now() - t0);
    csi *slot = malloc((size_t)n * sizeof(csi));
    vsdlss_m3_export_packed_slots(F, slot, n);
    double *x0 = malloc((size_t)n * 8), *xa = malloc((size_t)n * 8), *xb = malloc((size_t)n * 8), *b = malloc((size_t)n * 8);
    double *s = calloc((size_t)n, 8), *dp = malloc((size_t)n * 8), *res[4];
    for (csi g = 0; g < n; g++) { x0[g] = 0.9 + 0.01 * sin(0.31 * (double)g); dp[slot[g]] = Ch->x[g]; }
    const char *name[4] = {"orig", "packed", "tr", "tr+fuse"};
    double best[4];
    for (int m = 0; m < 4; m++) { res[m] = malloc((size_t)n * 8); best[m] = 1e300; }
    for (int r = 0; r < rounds; r++) for (int m = 0; m < 4; m++) {
        double t;
        if (m == 0) {                         /* caller order */
            memcpy(xa, x0, (size_t)n * 8);
            t = now();
            for (csi k = 0; k < K; k++) {
                memset(s, 0, (size_t)n * 8);
                for (csi j = 0; j < nsrc; j++) s[src[j]] += sv[k * nsrc + j];
                for (csi g = 0; g < n; g++) b[g] = Ch->x[g] * xa[g] + s[g];
                vsdlss_m3_solve(F, b, xa);
            }
            t = now() - t;
            memcpy(res[m], xa, (size_t)n * 8);
        } else if (m == 1) {                  /* packed order, by hand */
            memset(s, 0, (size_t)n * 8);
            for (csi g = 0; g < n; g++) xa[slot[g]] = x0[g];
            t = now();
            for (csi k = 0; k < K; k++) {
                for (csi j = 0; j < nsrc; j++) s[slot[src[j]]] = 0.0;
                for (csi j = 0; j < nsrc; j++) s[slot[src[j]]] += sv[k * nsrc + j];
                for (csi p = 0; p < n; p++) xb[p] = dp[p] * xa[p] + s[p];
                vsdlss_m3_solve_packed_inplace(F, xb);
                double *sw = xa; xa = xb; xb = sw;
            }
            t = now() - t;
            for (csi g = 0; g < n; g++) res[m][g] = xa[slot[g]];
            memset(s, 0, (size_t)n * 8);
        } else {
            vsdlss_m3_transient_fuse = m == 3;
            vsdlss_m3_transient *T = NULL;
            if (vsdlss_m3_transient_create(F, Ch, nsrc, src, 0, NULL, &T) != VSDLSS_OK) { puts("create failed"); return 1; }
            if (r == 0) {
                csi fc, u, l; vsdlss_m3_transient_info(T, &fc, &u, &l);
                printf("%-8s fused components %lld, tail chunks %lld, late units %lld\n", name[m], (long long)fc, (long long)u, (long long)l);
            }
            vsdlss_m3_transient_start(T, x0);
            t = now();
            if (vsdlss_m3_transient_run(T, K, sv, no_cb, NULL) != VSDLSS_OK) { puts("run failed"); return 1; }
            t = now() - t;
            vsdlss_m3_transient_get(T, -1, NULL, res[m]);
            vsdlss_m3_transient_free(T);
        }
        if (t < best[m]) best[m] = t;
    }
    for (int m = 0; m < 4; m++)
        printf("%-8s %8.2f ms per step (best of %d rounds)%s\n", name[m], 1e3 * best[m] / (double)K, rounds,
               m ? (memcmp(res[m], res[0], (size_t)n * 8) ? "  DIFFERS from orig" : "  bitwise = orig") : "");
    vsdlss_m3_factor_free(F);
    return 0;
}
