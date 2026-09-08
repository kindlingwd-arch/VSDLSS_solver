/* main.c
 *
 * VSDLSS-style sparse direct solver driver.
 *
 * Builds a symmetric positive-definite sparse system A (2-D 5-point Poisson
 * Laplacian), sets b = A*ones (so the exact solution is x = ones), then solves
 * it with the vsdlss kernel under four orderings and reports structural and
 * numeric measurements.  This validates that the reconstructed
 * solver "compiles and executes to solve sparse linear equations".
 */

#include "vsdlss.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* 2-D 5-point Poisson on an m x m grid, stored as the UPPER triangle of a
 * symmetric matrix in compressed-column form (column j holds rows i <= j). */
static vsdlss *build_poisson(int m)
{
    int n = m * m;
    csi row, col, i, j;
    csi nzA = (csi)n + 2 * (csi)m * (m - 1);   /* diag + left + up edges */

    vsdlss *A = vsdlss_spalloc(n, n, nzA, 1, 0);
    if (!A) return NULL;
    csi *Ap = A->p, *Ai = A->i;
    double *Ax = A->x;
    csi p = 0;
    for (col = 0; col < n; col++) {
        Ap[col] = p;
        i = col / m; j = col % m;
        Ai[p] = col; Ax[p] = 4.0; p++;                     /* diagonal */
        if (j > 0)          { row = col - 1; Ai[p] = row; Ax[p] = -1.0; p++; }
        if (i > 0)          { row = col - m; Ai[p] = row; Ax[p] = -1.0; p++; }
    }
    Ap[n] = p;
    return A;
}

static double maxabs_diff(int n, const double *x, const double *ref)
{
    int k; double r = 0;
    for (k = 0; k < n; k++) { double e = fabs(x[k] - ref[k]); if (e > r) r = e; }
    return r;
}

int main(int argc, char **argv)
{
    static const int selectors[] = {2, 1, 3, 4};
    static const char *names[] = {"natural", "RCM", "min-degree", "MLD"};
    int m = 30;
    if (argc > 1) m = atoi(argv[1]);
    if (m < 2) m = 30;

    vsdlss *A = build_poisson(m);
    if (!A) { fprintf(stderr, "matrix build failed\n"); return 1; }
    int n = (int)A->n;

    double *x      = (double *)malloc((size_t)n * sizeof(double));
    double *ones   = (double *)malloc((size_t)n * sizeof(double));
    double *rhs    = (double *)malloc((size_t)n * sizeof(double));
    int k, strategy, all_ok = 1;
    if (!x || !ones || !rhs) {
        fprintf(stderr, "vector allocation failed\n");
        vsdlss_spfree(A); free(x); free(ones); free(rhs);
        return 1;
    }
    for (k = 0; k < n; k++) ones[k] = 1.0;
    if (vsdlss_spmv_sym_upper(A, ones, rhs) != VSDLSS_OK) {
        fprintf(stderr, "matrix-vector product failed\n");
        vsdlss_spfree(A); free(x); free(ones); free(rhs);
        return 1;
    }

    printf("VSDLSS-style sparse direct solver test\n");
    printf("dimension n = %d, nnz(A upper) = %lld, grid = %dx%d\n",
           n, (long long)A->p[n], m, m);

    for (strategy = 0; strategy < 4; ++strategy) {
        csi *q = NULL, *pinv = NULL, nnz_l = 0;
        vsdlss_order_stats stats = {0, 0, 0, 0};
        vsdlss_factor *factor = NULL;
        clock_t t0 = clock(), t1, t2;
        int ok = vsdlss_order_analyze(A, selectors[strategy], &q, &pinv, &stats) == VSDLSS_OK;
        t1 = clock();
        if (ok) ok = vsdlss_factorize(A, selectors[strategy], &factor) == VSDLSS_OK;
        if (ok) {
            const vsdlss *L = vsdlss_factor_L(factor);
            nnz_l = L ? L->p[L->n] : 0;
            ok = L != NULL && vsdlss_factor_solve(factor, rhs, x) == VSDLSS_OK;
        }
        t2 = clock();
        {
            double error = ok ? maxabs_diff(n, x, ones) : INFINITY;
            printf("[%-10s] ok=%d order=%.3fs factor+solve=%.3fs nnz(L)=%lld "
                   "predicted=%lld fill=%lld tree=%lld separators=%lld error=%.3e\n",
                   names[strategy], ok,
                   (double)(t1 - t0) / CLOCKS_PER_SEC,
                   (double)(t2 - t1) / CLOCKS_PER_SEC,
                   (long long)nnz_l, (long long)stats.predicted_nnz_l,
                   (long long)stats.fill_edges_added,
                   (long long)stats.elimination_tree_height,
                   (long long)stats.separator_count, error);
            all_ok &= ok && error <= 1e-10 && nnz_l == stats.predicted_nnz_l;
        }
        free(q); free(pinv); vsdlss_factor_free(factor);
    }

    vsdlss_spfree(A);
    free(x); free(ones); free(rhs);
    return all_ok ? 0 : 1;
}
