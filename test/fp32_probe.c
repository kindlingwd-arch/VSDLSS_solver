/* Accuracy of a single-precision factor (FP32 storage, FP64 arithmetic):
 * factor once in double, solve, then round the stored factor values to
 * float in place (the low-degree records' multipliers and pivots, the core
 * panels) and solve the same right-hand side again.  Reports the solution
 * difference and both backward errors, for
 *   off  multipliers + core off-diagonal blocks rounded (pivots, diagonal
 *        triangles kept in double)
 *   all  every stored value rounded
 * and, with refine > 0, one step of iterative refinement with the rounded
 * factor (r = b - A x, x += solve(r)).  Only the stored values change; the
 * solver code is not.
 *
 *   ./fp32_probe dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads [refine=1]
 *   build: make fp32_probe METIS=1 (order 6), or without METIS for order 5 */
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

static double inf_norm(const double *x, csi n)
{ double m = 0; for (csi i = 0; i < n; i++) if (fabs(x[i]) > m) m = fabs(x[i]); return m; }

static double rel_diff(const double *x, const double *y, csi n)
{ double m = 0; for (csi i = 0; i < n; i++) if (fabs(x[i] - y[i]) > m) m = fabs(x[i] - y[i]); return m / inf_norm(y, n); }

static double round_f(double v) { return (double)(float)v; }

/* Round the factor's values (all = 0: keep pivots and diagonal triangles). */
static void round_factor(vsdlss_m3_factor *F, int all)
{
    for (csi c = 0; c < F->count; c++) {
        vsdlss_m3_component_factor *cf = F->component + c;
        vsdlss_reduction *r = cf->reduction;
        if (r && r->pk) for (csi q = 0; q < r->pk_count; q++) {
            vsdlss_pk_seg *g = r->pk + q;
            for (csi o = 0; o < g->nbn; o++) g->val[o] = round_f(g->val[o]);
            if (all) for (csi i = 0; i < g->count; i++) g->piv[i] = round_f(g->piv[i]);
        }
        vsdlss_sn_factor *L = cf->numeric;
        if (!L) continue;
        for (csi s = 0; s < L->count; s++) {
            const csi w = L->column_start[s + 1] - L->column_start[s];
            double *p = L->panel + L->panel_offset[s], *e = p + w * (w + 1) / 2, *end = L->panel + L->panel_offset[s + 1];
            for (double *v = all ? p : e; v < end; v++) *v = round_f(*v);
        }
    }
}

typedef struct { double *val, *piv, *panel; } saved_vals;

static void save_or_restore(vsdlss_m3_factor *F, saved_vals *sv, int restore)
{
    for (csi c = 0; c < F->count; c++) {
        vsdlss_m3_component_factor *cf = F->component + c;
        vsdlss_reduction *r = cf->reduction;
        if (r && r->pk) for (csi q = 0; q < r->pk_count; q++) {
            vsdlss_pk_seg *g = r->pk + q;
            if (restore) { memcpy(g->val, sv->val, (size_t)g->nbn * 8); memcpy(g->piv, sv->piv, (size_t)g->count * 8); }
            else { memcpy(sv->val, g->val, (size_t)g->nbn * 8); memcpy(sv->piv, g->piv, (size_t)g->count * 8); }
            sv->val += g->nbn; sv->piv += g->count;
        }
        vsdlss_sn_factor *L = cf->numeric;
        if (!L) continue;
        const size_t z = (size_t)L->panel_offset[L->count] * 8;
        if (restore) memcpy(L->panel, sv->panel, z); else memcpy(sv->panel, L->panel, z);
        sv->panel += L->panel_offset[L->count];
    }
}

int main(int argc, char **argv)
{
    if (argc < 4) { fprintf(stderr, "usage: fp32_probe dump.bin|grid:SIDE[:CHAIN]|text:DIR order threads [refine=1]\n"); return 1; }
    const int order = atoi(argv[2]), T = atoi(argv[3]) > 0 ? atoi(argv[3]) : 1, refine = argc > 4 ? atoi(argv[4]) : 1;
    interleave_memory();
    vsdlss *A = read_input(argv[1]);
    if (!A) { puts("input failed"); return 1; }
    const csi n = A->n;
    vsdlss_set_num_threads(T);
    vsdlss_m3_factor *F = NULL; double t0 = now();
    vsdlss_status st = vsdlss_factorize_m3(A, order, &F);
    if (st != VSDLSS_OK) {
        printf("factor failed: %s%s\n", vsdlss_status_string(st),
               st == VSDLSS_ERR_UNSUPPORTED && order == 6 ? " (order 6 needs make fp32_probe METIS=1)" : "");
        return 1;
    }
    printf("# %s: n %lld, order %d, %d threads, factor %.1f s\n", argv[1], (long long)n, order, T, now() - t0);
    /* sizes of the stored values */
    csi nval = 0, npiv = 0, npanel = 0;
    for (csi c = 0; c < F->count; c++) {
        const vsdlss_reduction *r = F->component[c].reduction;
        if (r && r->pk) for (csi q = 0; q < r->pk_count; q++) { nval += r->pk[q].nbn; npiv += r->pk[q].count; }
        if (F->component[c].numeric) npanel += F->component[c].numeric->panel_offset[F->component[c].numeric->count];
    }
    printf("# stored values: %lld multipliers, %lld pivots, %lld core panel entries (%.0f MB as double)\n",
           (long long)nval, (long long)npiv, (long long)npanel, (double)(nval + npiv + npanel) * 8 / 1e6);
    saved_vals base = { malloc((size_t)(nval ? nval : 1) * 8), malloc((size_t)(npiv ? npiv : 1) * 8), malloc((size_t)(npanel ? npanel : 1) * 8) };
    double *b = malloc((size_t)n * 8), *x0 = malloc((size_t)n * 8), *x = malloc((size_t)n * 8), *r = malloc((size_t)n * 8), *d = malloc((size_t)n * 8);
    if (!base.val || !base.piv || !base.panel || !b || !x0 || !x || !r || !d) { puts("out of memory"); return 1; }
    saved_vals sv = base; save_or_restore(F, &sv, 0);
    /* right-hand side: smooth load plus noise, like tap currents */
    for (csi i = 0; i < n; i++) b[i] = 1e-3 * (1.0 + 0.5 * sin(1e-4 * (double)i)) + 1e-4 * sin(0.37 * (double)i);
    double eta;
    if (vsdlss_m3_solve(F, b, x0) != VSDLSS_OK || vsdlss_backward_error(A, x0, b, &eta) != VSDLSS_OK) { puts("solve failed"); return 1; }
    printf("double factor:      backward error %.2e, |x|_inf %.3e\n", eta, inf_norm(x0, n));
    for (int all = 0; all < 2; all++) {
        sv = base; save_or_restore(F, &sv, 1);
        round_factor(F, all);
        if (vsdlss_m3_solve(F, b, x) != VSDLSS_OK) { puts("solve failed"); return 1; }
        vsdlss_backward_error(A, x, b, &eta);
        printf("float %-3s factor:   backward error %.2e, |x - x_double|_inf / |x|_inf %.2e\n", all ? "all" : "off", eta, rel_diff(x, x0, n));
        for (int k = 1; k <= refine; k++) {
            /* r = b - A x (upper CSC, symmetric) */
            for (csi i = 0; i < n; i++) r[i] = b[i];
            for (csi j = 0; j < n; j++)
                for (csi p = A->p[j]; p < A->p[j + 1]; p++) {
                    const csi i = A->i[p]; const double a = A->x[p];
                    r[i] -= a * x[j];
                    if (i != j) r[j] -= a * x[i];
                }
            if (vsdlss_m3_solve(F, r, d) != VSDLSS_OK) { puts("solve failed"); return 1; }
            for (csi i = 0; i < n; i++) x[i] += d[i];
            vsdlss_backward_error(A, x, b, &eta);
            printf("  + %d refinement:   backward error %.2e, |x - x_double|_inf / |x|_inf %.2e\n", k, eta, rel_diff(x, x0, n));
        }
    }
    vsdlss_m3_factor_free(F); vsdlss_spfree(A);
    free(base.val); free(base.piv); free(base.panel); free(b); free(x0); free(x); free(r); free(d);
    return 0;
}
