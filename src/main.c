/* main.c
 *
 * VSDLSS-style sparse direct solver driver.
 *
 * Builds a symmetric positive-definite sparse system A (2-D 5-point Poisson
 * Laplacian), sets b = A*ones (so the exact solution is x = ones), then solves
 * it with the vsdlss kernel under two orderings (RCM and natural) and reports
 * the relative error and timing.  This validates that the reconstructed
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

/* y = A*x for a symmetric matrix stored upper-CSC */
static void matvec(const vsdlss *A, const double *x, double *y)
{
    csi n = A->n, p, j;
    for (j = 0; j < n; j++) y[j] = 0.0;
    for (j = 0; j < n; j++) {
        double xj = x[j];
        for (p = A->p[j]; p < A->p[j + 1]; p++) {
            csi i = A->i[p];
            double v = A->x[p];
            y[i] += v * xj;
            if (i != j) y[j] += v * x[i];
        }
    }
}

int main(int argc, char **argv)
{
    int m = 30;
    if (argc > 1) m = atoi(argv[1]);
    if (m < 2) m = 30;

    vsdlss *A = build_poisson(m);
    if (!A) { fprintf(stderr, "matrix build failed\n"); return 1; }
    int n = (int)A->n;

    double *x      = (double *)malloc((size_t)n * sizeof(double));
    double *ones   = (double *)malloc((size_t)n * sizeof(double));
    double *rhs    = (double *)malloc((size_t)n * sizeof(double));
    int k;
    for (k = 0; k < n; k++) ones[k] = 1.0;
    matvec(A, ones, rhs);                     /* b = A*ones  =>  x = ones */

    printf("VSDLSS-style sparse direct solver test\n");
    printf("dimension n = %d, nnz(A upper) = %lld, grid = %dx%d\n",
           n, (long long)A->p[n], m, m);

    /* order 2 = natural / identity */
    memcpy(x, rhs, (size_t)n * sizeof(double));
    clock_t t0 = clock();
    int ok1 = vsdlss_cholsolve(2, A, x);
    double e1 = maxabs_diff(n, x, ones);
    printf("[natural] ok=%d  max|solution-1| = %.3e  time=%.3fs\n",
           ok1, e1, (double)(clock() - t0) / CLOCKS_PER_SEC);

    /* order 1 = RCM */
    memcpy(x, rhs, (size_t)n * sizeof(double));
    t0 = clock();
    int ok2 = vsdlss_cholsolve(1, A, x);
    double e2 = maxabs_diff(n, x, ones);
    printf("[RCM    ] ok=%d  max|solution-1| = %.3e  time=%.3fs\n",
           ok2, e2, (double)(clock() - t0) / CLOCKS_PER_SEC);

    vsdlss_spfree(A);
    free(x); free(ones); free(rhs);
    return 0;
}
