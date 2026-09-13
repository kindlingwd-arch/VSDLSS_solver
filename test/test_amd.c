/* Approximate minimum degree: permutation validity, determinism, fill, and
 * end-to-end accuracy.  The shapes are chosen to exercise the quotient-graph
 * paths that a smoke test would miss: isolated vertices (degree 0 at setup),
 * a vertex above the dense threshold, cliques that collapse into one
 * supervariable, mass elimination, and a grid large enough to force at least
 * one garbage collection of the element workspace. */
#include "vsdlss.h"
#include "../src/vsdlss_internal.h"
#include "../src/vsdlss_m3_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures;
#define CHECK(e) do{ if(!(e)){ \
    fprintf(stderr,"%s:%d: CHECK failed: %s\n",__FILE__,__LINE__,#e); failures++; } }while(0)

/* Upper-triangular CSC of (weighted Laplacian + I) for an undirected edge
 * list, which is symmetric positive definite for any connected or
 * disconnected simple graph. */
static vsdlss *from_edges(csi n, const csi *edges, csi m)
{
    vsdlss *A = vsdlss_spalloc(n, n, n + m, 1, 0);
    csi *deg = (csi *)calloc((size_t)n, sizeof(csi));
    csi *cnt = (csi *)calloc((size_t)n + 1, sizeof(csi));
    csi *cur, k, j, p;
    if (!A || !deg || !cnt) { free(deg); free(cnt); vsdlss_spfree(A); return NULL; }
    for (k = 0; k < m; ++k) {
        csi a = edges[2 * k], b = edges[2 * k + 1];
        csi hi = a > b ? a : b;
        ++deg[a]; ++deg[b]; ++cnt[hi];      /* stored in the higher column */
    }
    for (j = 0; j < n; ++j) ++cnt[j];       /* the diagonal */
    p = 0;
    for (j = 0; j < n; ++j) { csi c = cnt[j]; A->p[j] = p; p += c; }
    A->p[n] = p;
    cur = (csi *)malloc((size_t)n * sizeof(csi));
    if (!cur) { free(deg); free(cnt); vsdlss_spfree(A); return NULL; }
    for (j = 0; j < n; ++j) cur[j] = A->p[j];
    for (k = 0; k < m; ++k) {
        csi a = edges[2 * k], b = edges[2 * k + 1];
        csi lo = a < b ? a : b, hi = a > b ? a : b;
        A->i[cur[hi]] = lo; A->x[cur[hi]++] = -1.0;
    }
    for (j = 0; j < n; ++j) { A->i[cur[j]] = j; A->x[cur[j]++] = (double)deg[j] + 1.0; }
    /* Sort each column: entries must be in increasing row order. */
    for (j = 0; j < n; ++j) {
        csi lo = A->p[j], hi = A->p[j + 1], a, b;
        for (a = lo + 1; a < hi; ++a) {
            csi ri = A->i[a]; double rx = A->x[a];
            for (b = a; b > lo && A->i[b - 1] > ri; --b) { A->i[b] = A->i[b - 1]; A->x[b] = A->x[b - 1]; }
            A->i[b] = ri; A->x[b] = rx;
        }
    }
    free(deg); free(cnt); free(cur);
    return A;
}

static vsdlss *grid3(csi side)
{
    csi n = side * side * side, m = 0, cap = 3 * n, k = 0;
    csi *edges = (csi *)malloc((size_t)cap * 2 * sizeof(csi));
    vsdlss *A;
    if (!edges) return NULL;
    for (csi z = 0; z < side; ++z) for (csi y = 0; y < side; ++y) for (csi x = 0; x < side; ++x) {
        csi j = (z * side + y) * side + x;
        if (x + 1 < side) { edges[2 * m] = j; edges[2 * m + 1] = j + 1; ++m; }
        if (y + 1 < side) { edges[2 * m] = j; edges[2 * m + 1] = j + side; ++m; }
        if (z + 1 < side) { edges[2 * m] = j; edges[2 * m + 1] = j + side * side; ++m; }
    }
    (void)k;
    A = from_edges(n, edges, m);
    free(edges);
    return A;
}

static csi fill_of(const vsdlss *A, const csi *pinv)
{
    vsdlss *P = vsdlss_symperm(A, pinv, 1);
    vsdlss_sn_symbolic *s = NULL;
    csi nnz = -1;
    if (P && vsdlss_sn_analyze(P, &s) == VSDLSS_OK) { nnz = s->l_nnz; vsdlss_sn_symbolic_free(s); }
    vsdlss_spfree(P);
    return nnz;
}

/* Runs AMD, checks the permutation is a bijection, that it is reproducible,
 * and that a factorization built on it solves accurately.  Returns nnz(L). */
static csi check(const char *name, vsdlss *A)
{
    csi n = A ? A->n : 0, *q = NULL, *pinv = NULL, *q2 = NULL, *pinv2 = NULL, k;
    csi nnz = -1;
    double *truth = NULL, *rhs = NULL, *x = NULL, eta = 0;
    vsdlss_factor *f = NULL;
    if (!A) { fprintf(stderr, "%s: matrix build failed\n", name); failures++; return -1; }
    CHECK(vsdlss_order(A, 5, &q, &pinv) == VSDLSS_OK);
    if (!q || !pinv) { fprintf(stderr, "%s: order failed\n", name); failures++; goto done; }
    CHECK(vsdlss_validate_permutation(q, pinv, n) == VSDLSS_OK);
    CHECK(vsdlss_order(A, 5, &q2, &pinv2) == VSDLSS_OK);
    if (q2) CHECK(memcmp(q, q2, (size_t)n * sizeof(csi)) == 0);  /* deterministic */
    nnz = fill_of(A, pinv);
    CHECK(nnz >= n);

    truth = (double *)malloc((size_t)n * sizeof(double));
    rhs = (double *)malloc((size_t)n * sizeof(double));
    x = (double *)malloc((size_t)n * sizeof(double));
    if (!truth || !rhs || !x) { failures++; goto done; }
    for (k = 0; k < n; ++k) truth[k] = (k % 2 ? -1.0 : 1.0) * (0.5 + (double)k * 0.01);
    CHECK(vsdlss_spmv_sym_upper(A, truth, rhs) == VSDLSS_OK);
    CHECK(vsdlss_factorize(A, 5, &f) == VSDLSS_OK);
    if (f) {
        CHECK(vsdlss_factor_solve(f, rhs, x) == VSDLSS_OK);
        CHECK(vsdlss_backward_error(A, x, rhs, &eta) == VSDLSS_OK);
        CHECK(eta < 1e-12);
    }
    {   /* M3 must accept ordering 5 too, and agree to solver accuracy. */
        vsdlss_m3_factor *m3 = NULL;
        double *x3 = (double *)malloc((size_t)n * sizeof(double));
        CHECK(vsdlss_factorize_m3(A, 5, &m3) == VSDLSS_OK);
        if (m3 && x3) {
            CHECK(vsdlss_m3_solve(m3, rhs, x3) == VSDLSS_OK);
            CHECK(vsdlss_backward_error(A, x3, rhs, &eta) == VSDLSS_OK);
            CHECK(eta < 1e-12);
        }
        vsdlss_m3_factor_free(m3); free(x3);
    }
done:
    vsdlss_factor_free(f);
    free(q); free(pinv); free(q2); free(pinv2); free(truth); free(rhs); free(x);
    vsdlss_spfree(A);
    return nnz;
}

int main(void)
{
    /* n == 1 */
    { csi e[] = {0, 0}; (void)e; check("single", from_edges(1, NULL, 0)); }
    /* All isolated: every vertex has degree 0 at setup. */
    check("diagonal", from_edges(6, NULL, 0));
    /* Path, then a tree, then a cycle. */
    { csi e[] = {0,1, 1,2, 2,3, 3,4}; check("path", from_edges(5, e, 4)); }
    { csi e[] = {0,1, 0,2, 1,3, 1,4, 2,5}; check("tree", from_edges(6, e, 5)); }
    { csi e[] = {0,1, 1,2, 2,3, 3,4, 4,0}; check("cycle", from_edges(5, e, 5)); }
    /* Complete graph: every remaining vertex is indistinguishable, so the
       supervariable sweep must collapse them. */
    { csi e[2 * 45], m = 0;
      for (csi a = 0; a < 10; ++a) for (csi b = a + 1; b < 10; ++b) { e[2*m]=a; e[2*m+1]=b; ++m; }
      check("clique10", from_edges(10, e, m)); }
    /* One vertex adjacent to everything: above the dense threshold for large n. */
    { csi n = 600, e[1198], m = 0;
      for (csi a = 1; a < n; ++a) { e[2*m]=0; e[2*m+1]=a; ++m; }
      check("star600", from_edges(n, e, m)); }
    /* Two disconnected components of different shape. */
    { csi e[] = {0,1, 1,2, 2,0, 4,5, 5,6, 6,7, 7,4}; check("split", from_edges(8, e, 7)); }
    /* Grid large enough to need a workspace garbage collection. */
    {
        vsdlss *g = grid3(12);
        csi *ident = NULL, natural, amd;
        if (!g) { fprintf(stderr, "grid build failed\n"); failures++; }
        else {
            csi n = g->n, k;
            ident = (csi *)malloc((size_t)n * sizeof(csi));
            if (ident) { for (k = 0; k < n; ++k) ident[k] = k; natural = fill_of(g, ident); }
            else natural = -1;
            amd = check("grid12", g);           /* frees g */
            CHECK(amd > 0 && natural > 0);
            /* The whole point of the ordering: much less fill than natural. */
            if (amd > 0 && natural > 0) CHECK(amd * 2 < natural);
            free(ident);
        }
    }
    /* Ordering 5 must be rejected nowhere it is accepted, and unknown
       selectors must still be rejected. */
    {
        vsdlss *A = from_edges(4, (csi[]){0,1, 1,2, 2,3}, 3);
        csi *q = NULL, *pinv = NULL;
        CHECK(vsdlss_order(A, 6, &q, &pinv) == VSDLSS_ERR_UNSUPPORTED);
        CHECK(q == NULL && pinv == NULL);
        {
            vsdlss_m3_factor *m3 = NULL;
            CHECK(vsdlss_factorize_m3(A, 6, &m3) == VSDLSS_ERR_UNSUPPORTED);
            CHECK(m3 == NULL);
        }
        vsdlss_spfree(A);
    }
    if (failures) { fprintf(stderr, "test_amd: %d failure(s)\n", failures); return 1; }
    puts("test_amd: permutation, determinism, fill and accuracy OK");
    return 0;
}
