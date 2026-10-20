/* ECO benchmark: vsdlss_m3_modify against refactoring.
 *
 *   ./bench_eco <dump.bin | grid:SIDE[:CHAIN] | text:DIR> [order=5] [threads=1]
 *   env: ECO_EDGES (conductances changed per ECO, default 10), ECO_ROUNDS
 *        (ECOs applied one after another, default 20), ECO_GROUND (ground
 *        changes per ECO, default 0), ECO_REFACTOR=0 (skip the refactor
 *        comparison), ECO_SEED
 *
 * Factorizes A, times warm solves, then applies ECO_ROUNDS random ECOs:
 * each changes ECO_EDGES existing couplings by a factor in [0.5, 2] (so
 * about half updates, half downdates) and optionally ECO_GROUND diagonals.
 * Reports the time of each modification, the factor columns it rewrote,
 * the backward error of a solve against the modified matrix, and (unless
 * ECO_REFACTOR=0) the time of factorizing the modified matrix from scratch
 * and the difference between the two solutions. */
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



static unsigned long long rng = 88172645463325252ULL;
static double urand(void) { rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17; return (double)(rng >> 11) / 9007199254740992.0; }

static int envi(const char *name, int def) { const char *e = getenv(name); return e && *e ? atoi(e) : def; }

static double solve_time(const vsdlss_m3_factor *f, const double *b, double *x, int reps)
{
    double t[16];
    if (reps > 16) reps = 16;
    for (int r = 0; r < reps; r++) {
        double t0 = now();
        if (vsdlss_m3_solve(f, b, x) != VSDLSS_OK) { puts("solve failed"); exit(1); }
        t[r] = now() - t0;
    }
    qsort(t, (size_t)reps, sizeof(double), cmpd);
    return t[reps / 2];
}

int main(int argc, char **argv)
{
    if (argc < 2) { fprintf(stderr, "usage: %s <dump.bin | grid:SIDE[:CHAIN] | text:DIR> [order] [threads]\n", argv[0]); return 2; }
    const int order = argc > 2 ? atoi(argv[2]) : 5, threads = argc > 3 ? atoi(argv[3]) : 1;
    const int edges = envi("ECO_EDGES", 10), rounds = envi("ECO_ROUNDS", 20), ground = envi("ECO_GROUND", 0);
    const int refactor = envi("ECO_REFACTOR", 1);
    rng += (unsigned long long)envi("ECO_SEED", 0);
    vsdlss_set_num_threads(threads);
    vsdlss *A = read_input(argv[1]);
    if (!A) { fprintf(stderr, "cannot read %s\n", argv[1]); return 1; }
    const csi n = A->n, nnz = A->p[n];
    printf("# input %s: n %lld, nnz %lld, order %d, threads %d, %d ECOs of %d couplings + %d grounds\n",
           argv[1], (long long)n, (long long)nnz, order, threads, rounds, edges, ground);
    /* diagonal positions; off-diagonal entries to pick from */
    csi *dpos = malloc((size_t)n * sizeof(csi)), *off = malloc((size_t)nnz * sizeof(csi)), *colof = malloc((size_t)nnz * sizeof(csi)), noff = 0;
    for (csi j = 0; j < n; j++) {
        dpos[j] = -1;
        for (csi p = A->p[j]; p < A->p[j + 1]; p++) {
            colof[p] = j;
            if (A->i[p] == j) dpos[j] = p; else if (A->x[p] < 0) off[noff++] = p;
        }
    }
    double t0 = now();
    vsdlss_m3_factor *f = NULL;
    vsdlss_status s = vsdlss_factorize_m3(A, order, &f);
    if (s != VSDLSS_OK) { printf("factorize: %s\n", vsdlss_status_string(s)); return 1; }
    const double tf = now() - t0;
    double *b = malloc((size_t)n * 8), *x = malloc((size_t)n * 8), *y = malloc((size_t)n * 8);
    for (csi i = 0; i < n; i++) b[i] = 1e-3 * (1.0 + sin(0.37 * (double)i));
    vsdlss_m3_solve(f, b, x);                                   /* cold */
    const double ts = solve_time(f, b, x, 7);
    printf("factorize %.3f s, warm solve %.2f ms\n", tf, 1e3 * ts);

    csi cap = 3 * (csi)edges + ground, *ri = malloc((size_t)cap * sizeof(csi)), *ci = malloc((size_t)cap * sizeof(csi));
    double *dx = malloc((size_t)cap * 8), *tm = malloc((size_t)(rounds ? rounds : 1) * 8);
    csi cols_r = 0, cols_c = 0, ups = 0, downs = 0;
    for (int r = 0; r < rounds; r++) {
        csi nz = 0;
        for (int k = 0; k < edges && noff; k++) {
            const csi p = off[(csi)(urand() * (double)noff) % noff], a = A->i[p], c = colof[p];
            const double g = -A->x[p], dg = g * (urand() < 0.5 ? -0.5 * urand() : urand());   /* g -> [0.5 g, 2 g] */
            ri[nz] = a; ci[nz] = c; dx[nz++] = -dg;
            ri[nz] = a; ci[nz] = a; dx[nz++] = dg;
            ri[nz] = c; ci[nz] = c; dx[nz++] = dg;
            A->x[p] -= dg; A->x[dpos[a]] += dg; A->x[dpos[c]] += dg;  /* A + dA, for the checks */
        }
        for (int k = 0; k < ground; k++) {
            const csi i = (csi)(urand() * (double)n) % n; const double d = 1e-3 * (urand() - 0.25);
            if (A->x[dpos[i]] + d <= 0) continue;
            ri[nz] = i; ci[nz] = i; dx[nz++] = d; A->x[dpos[i]] += d;
        }
        vsdlss_m3_modify_stats st;
        t0 = now();
        s = vsdlss_m3_modify(f, nz, ri, ci, dx, &st);
        tm[r] = now() - t0;
        if (s != VSDLSS_OK) { printf("modify round %d: %s\n", r, vsdlss_status_string(s)); return 1; }
        cols_r += st.record_columns; cols_c += st.core_columns; ups += st.updates; downs += st.downdates;
    }
    if (rounds) {
        double total = 0; for (int r = 0; r < rounds; r++) total += tm[r];
        const double first = tm[0];
        qsort(tm, (size_t)rounds, sizeof(double), cmpd);
        printf("modify: first %.3f ms (builds the record index), median %.3f ms, max %.3f ms, total %.3f ms\n",
               1e3 * first, 1e3 * tm[rounds / 2], 1e3 * tm[rounds - 1], 1e3 * total);
        printf("  rank-1 terms: %lld updates, %lld downdates; columns rewritten per ECO: %.1f records, %.1f core\n",
               (long long)ups, (long long)downs, (double)cols_r / rounds, (double)cols_c / rounds);
    }
    const double ts2 = solve_time(f, b, x, 7);
    double eta = NAN;
    vsdlss_backward_error(A, x, b, &eta);
    printf("after the ECOs: warm solve %.2f ms, backward error against A + dA %.2e\n", 1e3 * ts2, eta);
    if (refactor) {
        vsdlss_m3_factor *g = NULL;
        t0 = now();
        s = vsdlss_factorize_m3(A, order, &g);
        const double tr = now() - t0;
        if (s != VSDLSS_OK) { printf("refactorize: %s\n", vsdlss_status_string(s)); return 1; }
        vsdlss_m3_solve(g, b, y);
        double d = 0, ny = 0, eta2 = NAN;
        for (csi i = 0; i < n; i++) { if (fabs(x[i] - y[i]) > d) d = fabs(x[i] - y[i]); if (fabs(y[i]) > ny) ny = fabs(y[i]); }
        vsdlss_backward_error(A, y, b, &eta2);
        printf("refactorize A + dA: %.3f s (backward error %.2e); max |x_modify - x_refactor| / max |x| = %.2e\n",
               tr, eta2, ny > 0 ? d / ny : d);
        if (rounds) printf("speed-up of one ECO (median modify vs refactorize): %.0fx\n", tr / tm[rounds / 2]);
        vsdlss_m3_factor_free(g);
    }
    vsdlss_m3_factor_free(f);
    vsdlss_spfree(A);
    free(dpos); free(off); free(colof); free(b); free(x); free(y); free(ri); free(ci); free(dx); free(tm);
    return 0;
}
