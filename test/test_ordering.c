#include "vsdlss.h"
#include "../src/vsdlss_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures;

#define CHECK(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #expr); \
        failures++; \
    } \
} while (0)

/* Catches a graph builder that omits reverse adjacency or retains diagonals. */
static void test_cycle_graph_has_symmetric_offdiagonal_degrees(void)
{
    csi p[] = {0, 1, 3, 5, 8};
    csi i[] = {0, 0, 1, 1, 2, 0, 2, 3};
    double x[] = {3, -1, 3, -1, 3, -1, -1, 3};
    vsdlss A = {8, 4, 4, p, i, x, -1};
    vsdlss_graph *graph = NULL;
    csi v;

    CHECK(vsdlss_graph_build(&A, &graph) == VSDLSS_OK);
    CHECK(graph != NULL);
    if (graph) {
        for (v = 0; v < 4; ++v) CHECK(vsdlss_graph_degree(graph, v) == 2);
        CHECK(vsdlss_graph_has_edge(graph, 0, 1));
        CHECK(vsdlss_graph_has_edge(graph, 1, 0));
        CHECK(!vsdlss_graph_has_edge(graph, 0, 0));
    }
    vsdlss_graph_free(graph);
}

/* Catches elimination that deactivates a vertex without completing its neighbor clique. */
static void test_cycle_elimination_inserts_one_fill_edge(void)
{
    csi p[] = {0, 1, 3, 5, 8};
    csi i[] = {0, 0, 1, 1, 2, 0, 2, 3};
    double x[] = {3, -1, 3, -1, 3, -1, -1, 3};
    vsdlss A = {8, 4, 4, p, i, x, -1};
    vsdlss_graph *graph = NULL;
    csi *neighbors = NULL, count = -1;

    CHECK(vsdlss_graph_build(&A, &graph) == VSDLSS_OK);
    if (graph) {
        CHECK(vsdlss_graph_eliminate(graph, 0, &neighbors, &count) == VSDLSS_OK);
        CHECK(count == 2);
        if (count == 2) {
            CHECK(neighbors[0] == 1 && neighbors[1] == 3);
            CHECK(vsdlss_graph_has_edge(graph, 1, 3));
            CHECK(vsdlss_graph_has_edge(graph, 3, 1));
        }
        CHECK(vsdlss_graph_degree(graph, 0) == 0);
        CHECK(vsdlss_graph_degree(graph, 1) == 2);
        CHECK(vsdlss_graph_fill_edges(graph) == 1);
    }
    free(neighbors);
    vsdlss_graph_free(graph);
}

static void oracle_min_degree(const vsdlss *A, csi *q, csi *fill)
{
    unsigned char edges[36] = {0}, active[6];
    csi col, p, step;
    memset(active, 1, sizeof active);
    *fill = 0;
    for (col = 0; col < A->n; ++col) {
        for (p = A->p[col]; p < A->p[col + 1]; ++p) {
            csi row = A->i[p];
            if (row != col) edges[row * 6 + col] = edges[col * 6 + row] = 1;
        }
    }
    for (step = 0; step < 6; ++step) {
        csi v, best = -1, best_degree = 7;
        for (v = 0; v < 6; ++v) if (active[v]) {
            csi w, degree = 0;
            for (w = 0; w < 6; ++w) degree += active[w] && edges[v * 6 + w];
            if (degree < best_degree) { best = v; best_degree = degree; }
        }
        q[step] = best;
        for (v = 0; v < 6; ++v) if (active[v] && edges[best * 6 + v]) {
            csi w;
            for (w = v + 1; w < 6; ++w) {
                if (active[w] && edges[best * 6 + w] && !edges[v * 6 + w]) {
                    edges[v * 6 + w] = edges[w * 6 + v] = 1;
                    (*fill)++;
                }
            }
        }
        active[best] = 0;
    }
}

/* Catches selecting from stale initial degrees or omitting elimination fill. */
static void test_dynamic_minimum_degree_matches_boolean_oracle(void)
{
    csi p[] = {0, 1, 3, 6, 9, 12, 15};
    csi i[] = {0, 0,1, 0,1,2, 0,2,3, 1,3,4, 2,4,5};
    double x[] = {5,-1,5,-1,-1,5,-1,-1,5,-1,-1,5,-1,-1,5};
    vsdlss A = {15, 6, 6, p, i, x, -1};
    vsdlss_order_stats stats;
    csi expected_q[6], expected_fill, *q = NULL, *pinv = NULL, k;

    oracle_min_degree(&A, expected_q, &expected_fill);
    CHECK(vsdlss_order_analyze(&A, 3, &q, &pinv, &stats) == VSDLSS_OK);
    if (q && pinv) {
        CHECK(vsdlss_validate_permutation(q, pinv, 6) == VSDLSS_OK);
        for (k = 0; k < 6; ++k) CHECK(q[k] == expected_q[k]);
        CHECK(stats.fill_edges_added == expected_fill);
    }
    free(q);
    free(pinv);
}

/* Catches statistics disconnected from the symbolic/numeric factor structure. */
static void test_minimum_degree_prediction_matches_actual_factor(void)
{
    csi p[] = {0, 1, 3, 5, 8};
    csi i[] = {0, 0, 1, 1, 2, 0, 2, 3};
    double x[] = {3, -1, 3, -1, 3, -1, -1, 3};
    vsdlss A = {8, 4, 4, p, i, x, -1};
    vsdlss_order_stats stats;
    vsdlss_factor *factor = NULL;
    const vsdlss *L;
    csi *q = NULL, *pinv = NULL;

    CHECK(vsdlss_order_analyze(&A, 3, &q, &pinv, &stats) == VSDLSS_OK);
    CHECK(vsdlss_factorize(&A, 3, &factor) == VSDLSS_OK);
    L = vsdlss_factor_L(factor);
    CHECK(L != NULL);
    if (L) CHECK(stats.predicted_nnz_l == L->p[L->n]);
    CHECK(stats.elimination_tree_height >= 1);
    CHECK(stats.elimination_tree_height <= A.n);
    free(q);
    free(pinv);
    vsdlss_factor_free(factor);
}

static vsdlss *make_grid(csi side)
{
    csi n = side * side, nz = n + 2 * side * (side - 1);
    vsdlss *A = vsdlss_spalloc(n, n, nz, 1, 0);
    csi col, at = 0;
    if (!A) return NULL;
    for (col = 0; col < n; ++col) {
        A->p[col] = at;
        if (col >= side) { A->i[at] = col - side; A->x[at++] = -1.0; }
        if (col % side) { A->i[at] = col - 1; A->x[at++] = -1.0; }
        A->i[at] = col; A->x[at++] = 5.0;
    }
    A->p[n] = at;
    return A;
}

/* Catches an MLD selector that is absent, incomplete, or returns no separator. */
static void test_mld_orders_grid_with_separator(void)
{
    vsdlss *A = make_grid(9);
    vsdlss_order_stats stats;
    vsdlss_factor *factor = NULL;
    double *exact = NULL, *rhs = NULL, *solution = NULL;
    csi *q = NULL, *pinv = NULL;
    csi k;
    CHECK(A != NULL);
    if (!A) return;
    CHECK(vsdlss_order_analyze(A, 4, &q, &pinv, &stats) == VSDLSS_OK);
    if (q && pinv) CHECK(vsdlss_validate_permutation(q, pinv, A->n) == VSDLSS_OK);
    CHECK(stats.separator_count > 0);
    exact = (double *)malloc((size_t)A->n * sizeof(double));
    rhs = (double *)malloc((size_t)A->n * sizeof(double));
    solution = (double *)malloc((size_t)A->n * sizeof(double));
    CHECK(exact && rhs && solution);
    if (exact && rhs && solution) {
        for (k = 0; k < A->n; ++k) exact[k] = (k & 1) ? -0.5 : 1.0 + 0.01 * (double)k;
        CHECK(vsdlss_spmv_sym_upper(A, exact, rhs) == VSDLSS_OK);
        CHECK(vsdlss_factorize(A, 4, &factor) == VSDLSS_OK);
        CHECK(vsdlss_factor_solve(factor, rhs, solution) == VSDLSS_OK);
        if (factor) for (k = 0; k < A->n; ++k)
            CHECK(fabs(solution[k] - exact[k]) < 1e-11);
    }
    vsdlss_factor_free(factor);
    free(exact);
    free(rhs);
    free(solution);
    free(q);
    free(pinv);
    vsdlss_spfree(A);
}

/* Catches leaving the post-M2 default on the older RCM strategy. */
static void test_default_selector_is_mld(void)
{
    vsdlss *A = make_grid(9);
    vsdlss_order_stats default_stats, mld_stats;
    csi *default_q = NULL, *default_pinv = NULL, *mld_q = NULL, *mld_pinv = NULL;
    csi k;
    CHECK(A != NULL);
    if (!A) return;
    CHECK(vsdlss_order_analyze(A, 0, &default_q, &default_pinv, &default_stats) == VSDLSS_OK);
    CHECK(vsdlss_order_analyze(A, 4, &mld_q, &mld_pinv, &mld_stats) == VSDLSS_OK);
    if (default_q && mld_q) for (k = 0; k < A->n; ++k) CHECK(default_q[k] == mld_q[k]);
    CHECK(default_stats.separator_count == mld_stats.separator_count);
    CHECK(mld_stats.predicted_nnz_l > A->n);
    free(default_q); free(default_pinv); free(mld_q); free(mld_pinv);
    vsdlss_spfree(A);
}

static vsdlss *make_star(csi n)
{
    vsdlss *A = vsdlss_spalloc(n, n, 2 * n - 1, 1, 0);
    csi col, at = 0;
    if (!A) return NULL;
    for (col = 0; col < n; ++col) {
        A->p[col] = at;
        if (col > 0) { A->i[at] = 0; A->x[at++] = -1.0; }
        A->i[at] = col; A->x[at++] = (col == 0) ? (double)n : 2.0;
    }
    A->p[n] = at;
    return A;
}

/* Catches a nominal separator that leaves cross-side edges or a >7/8 side. */
static void test_mld_balanced_fallback_produces_separator(void)
{
    vsdlss *A = make_star(65);
    signed char *side = (signed char *)malloc(65);
    csi left = 0, right = 0, separator = 0, col, p;
    CHECK(A && side);
    if (!A || !side) { vsdlss_spfree(A); free(side); return; }
    CHECK(vsdlss_mld_top_partition(A, side, &left, &right, &separator) == VSDLSS_OK);
    CHECK(separator > 0);
    CHECK(left > 0 && right > 0);
    CHECK(left * 8 <= 7 * (left + right));
    CHECK(right * 8 <= 7 * (left + right));
    for (col = 0; col < A->n; ++col) {
        for (p = A->p[col]; p < A->p[col + 1]; ++p) if (A->i[p] != col) {
            csi row = A->i[p];
            CHECK(!((side[row] == 0 && side[col] == 1) ||
                    (side[row] == 1 && side[col] == 0)));
        }
    }
    free(side);
    vsdlss_spfree(A);
}

/* Catches recursive one-component-at-a-time peeling on disconnected input. */
static void test_mld_orders_many_isolated_vertices(void)
{
    const csi n = 2048;
    vsdlss *A = vsdlss_spalloc(n, n, n, 1, 0);
    csi *q = NULL, *pinv = NULL, k;
    CHECK(A != NULL);
    if (!A) return;
    for (k = 0; k < n; ++k) {
        A->p[k] = k; A->i[k] = k; A->x[k] = 1.0;
    }
    A->p[n] = n;
    CHECK(vsdlss_order(A, 4, &q, &pinv) == VSDLSS_OK);
    if (q && pinv) CHECK(vsdlss_validate_permutation(q, pinv, n) == VSDLSS_OK);
    free(q); free(pinv); vsdlss_spfree(A);
}

int main(void)
{
    test_cycle_graph_has_symmetric_offdiagonal_degrees();
    test_cycle_elimination_inserts_one_fill_edge();
    test_dynamic_minimum_degree_matches_boolean_oracle();
    test_minimum_degree_prediction_matches_actual_factor();
    test_mld_orders_grid_with_separator();
    test_default_selector_is_mld();
    test_mld_balanced_fallback_produces_separator();
    test_mld_orders_many_isolated_vertices();
    if (failures) {
        fprintf(stderr, "test_ordering: %d failure(s)\n", failures);
        return 1;
    }
    puts("test_ordering: ALL OK");
    return 0;
}
