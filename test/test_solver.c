#include "vsdlss.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

static int failures;

#define CHECK(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #expr); \
        failures++; \
    } \
} while (0)

static void test_status_strings(void)
{
    CHECK(strcmp(vsdlss_status_string(VSDLSS_OK), "success") == 0);
    CHECK(strcmp(vsdlss_status_string(VSDLSS_ERR_INVALID), "invalid input") == 0);
    CHECK(strcmp(vsdlss_status_string(VSDLSS_ERR_OOM), "out of memory") == 0);
    CHECK(strcmp(vsdlss_status_string(VSDLSS_ERR_NOT_POSDEF), "matrix is not positive definite") == 0);
    CHECK(strcmp(vsdlss_status_string(VSDLSS_ERR_NONFINITE), "non-finite numeric value") == 0);
    CHECK(strcmp(vsdlss_status_string(VSDLSS_ERR_IO), "I/O error") == 0);
    CHECK(strcmp(vsdlss_status_string(VSDLSS_ERR_UNSUPPORTED), "unsupported operation") == 0);
    CHECK(strcmp(vsdlss_status_string((vsdlss_status)999), "unknown error") == 0);
}

static void test_matrix_normalization(void)
{
    csi p[] = {0, 1, 4};
    csi i[] = {0, 1, 0, 0};
    double x[] = {4.0, 3.0, 0.25, 0.75};
    const csi i_before[] = {0, 1, 0, 0};
    const double x_before[] = {4.0, 3.0, 0.25, 0.75};
    vsdlss A = {4, 2, 2, p, i, x, -1};
    vsdlss *C = NULL;
    double v[] = {2.0, -1.0}, y[2], eta = -1.0;
    const double b[] = {7.0, -1.0};

    CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_OK);
    CHECK(vsdlss_normalize_upper(&A, &C) == VSDLSS_OK);
    CHECK(C != NULL);
    if (C) {
        CHECK(C->p[0] == 0 && C->p[1] == 1 && C->p[2] == 3);
        CHECK(C->i[0] == 0 && C->i[1] == 0 && C->i[2] == 1);
        CHECK(fabs(C->x[0] - 4.0) < 1e-15);
        CHECK(fabs(C->x[1] - 1.0) < 1e-15);
        CHECK(fabs(C->x[2] - 3.0) < 1e-15);
        CHECK(vsdlss_spmv_sym_upper(C, v, y) == VSDLSS_OK);
        CHECK(fabs(y[0] - 7.0) < 1e-15 && fabs(y[1] + 1.0) < 1e-15);
        CHECK(vsdlss_backward_error(C, v, b, &eta) == VSDLSS_OK);
        CHECK(eta < 1e-15);
    }
    CHECK(memcmp(i, i_before, sizeof i) == 0);
    CHECK(memcmp(x, x_before, sizeof x) == 0);
    vsdlss_spfree(C);
}

static void test_matrix_validation_errors(void)
{
    csi good_p[] = {0, 1, 3};
    csi good_i[] = {0, 0, 1};
    double good_x[] = {4, 1, 3};
    vsdlss A = {3, 2, 2, good_p, good_i, good_x, -1};
    csi bad_p[] = {0, 2, 1};
    csi low_i[] = {1, 0, 1};
    csi range_i[] = {0, 0, 2};
    double nan_x[] = {4, NAN, 3};
    double inf_x[] = {4, INFINITY, 3};

    CHECK(vsdlss_validate_upper_csc(NULL) == VSDLSS_ERR_INVALID);
    A.m = 0; A.n = 0; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_INVALID);
    A.m = 2; A.n = 3; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_INVALID);
    A.m = A.n = 2; A.p = bad_p; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_INVALID);
    A.p = good_p; A.i = low_i; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_INVALID);
    A.i = range_i; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_INVALID);
    A.i = good_i; A.x = nan_x; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_NONFINITE);
    A.x = inf_x; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_NONFINITE);
    A.x = good_x; A.nz = 0; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_INVALID);
    A.nz = -1; A.p = NULL; CHECK(vsdlss_validate_upper_csc(&A) == VSDLSS_ERR_INVALID);
}

static void check_permutation(const csi *q, const csi *pinv, csi n)
{
    csi k;
    CHECK(q != NULL && pinv != NULL);
    if (!q || !pinv) return;
    CHECK(vsdlss_validate_permutation(q, pinv, n) == VSDLSS_OK);
    for (k = 0; k < n; ++k) CHECK(pinv[q[k]] == k);
}

static void test_ordering_contract(void)
{
    csi p[] = {0, 1, 3, 4, 6, 7, 8};
    csi i[] = {0, 0, 1, 2, 2, 3, 4, 5};
    double x[] = {2,-1,2,-1,2,2,2,2};
    vsdlss A = {8, 6, 6, p, i, x, -1};
    csi *q = NULL, *pinv = NULL, k;

    CHECK(vsdlss_order(&A, 2, &q, &pinv) == VSDLSS_OK);
    check_permutation(q, pinv, 6);
    if (q && pinv) for (k = 0; k < 6; ++k) CHECK(q[k] == k && pinv[k] == k);
    free(q); free(pinv); q = pinv = NULL;
    CHECK(vsdlss_order(&A, 1, &q, &pinv) == VSDLSS_OK);
    check_permutation(q, pinv, 6);
    free(q); free(pinv); q = pinv = NULL;
    CHECK(vsdlss_order(&A, 77, &q, &pinv) == VSDLSS_ERR_UNSUPPORTED);
    CHECK(q == NULL && pinv == NULL);
}

static void test_rcm_high_degree_star(void)
{
    const csi n = 1026, center = n - 1;
    const csi nnz = n + center;
    csi *p = calloc((size_t)n + 1, sizeof(*p));
    csi *i = malloc((size_t)nnz * sizeof(*i));
    double *x = malloc((size_t)nnz * sizeof(*x));
    csi *q = NULL, *pinv = NULL, col, at = 0;
    vsdlss A;
    CHECK(p && i && x);
    if (!p || !i || !x) { free(p); free(i); free(x); return; }
    for (col = 0; col < center; ++col) {
        p[col] = at; i[at] = col; x[at++] = 2.0;
    }
    p[center] = at;
    for (col = 0; col < center; ++col) { i[at] = col; x[at++] = -1e-4; }
    i[at] = center; x[at++] = 2.0; p[n] = at;
    A = (vsdlss){nnz, n, n, p, i, x, -1};
    CHECK(vsdlss_order(&A, 0, &q, &pinv) == VSDLSS_OK);
    check_permutation(q, pinv, n);
    free(q); free(pinv); free(p); free(i); free(x);
}

static void test_factor_solve_and_reuse(void)
{
    csi p[] = {0, 1, 3};
    csi i[] = {0, 0, 1};
    double a[] = {4, 1, 3};
    vsdlss A = {3, 2, 2, p, i, a, -1};
    const double rhs[][2] = {{7,-1}, {4,1}, {-3,5}};
    const double want[][2] = {{2,-1}, {1,0}, {-14.0/11.0, 23.0/11.0}};
    int order, r;
    for (order = 1; order <= 2; ++order) {
        vsdlss_factor *F = NULL;
        const vsdlss *L;
        CHECK(vsdlss_factorize(&A, order, &F) == VSDLSS_OK);
        CHECK(F != NULL && vsdlss_factor_dimension(F) == 2);
        L = vsdlss_factor_L(F);
        CHECK(L != NULL);
        check_permutation(vsdlss_factor_q(F), vsdlss_factor_pinv(F), 2);
        for (r = 0; r < 3; ++r) {
            double got[] = {99, 98};
            CHECK(vsdlss_factor_solve(F, rhs[r], got) == VSDLSS_OK);
            CHECK(fabs(got[0] - want[r][0]) < 1e-12);
            CHECK(fabs(got[1] - want[r][1]) < 1e-12);
        }
        CHECK(vsdlss_factor_L(F) == L);
        vsdlss_factor_free(F);
    }
}

static void test_factor_failure_semantics(void)
{
    csi p[] = {0, 1, 3};
    csi i[] = {0, 0, 1};
    double indefinite[] = {1, 2, 1};
    double singular[] = {1, 1, 1};
    double finite[] = {4, 1, 3};
    double nonfinite[] = {4, NAN, 3};
    vsdlss A = {3, 2, 2, p, i, indefinite, -1};
    vsdlss_factor *F = (vsdlss_factor *)(uintptr_t)1;
    double rhs[] = {7, -1}, out[] = {41, 42};

    CHECK(vsdlss_factorize(&A, 2, &F) == VSDLSS_ERR_NOT_POSDEF && F == NULL);
    A.x = singular; CHECK(vsdlss_factorize(&A, 2, &F) == VSDLSS_ERR_NOT_POSDEF && F == NULL);
    A.x = nonfinite; CHECK(vsdlss_factorize(&A, 2, &F) == VSDLSS_ERR_NONFINITE && F == NULL);
    A.x = finite; CHECK(vsdlss_factorize(&A, 77, &F) == VSDLSS_ERR_UNSUPPORTED && F == NULL);
    CHECK(vsdlss_factorize(&A, 2, &F) == VSDLSS_OK && F != NULL);
    rhs[0] = NAN;
    CHECK(vsdlss_factor_solve(F, rhs, out) == VSDLSS_ERR_NONFINITE);
    CHECK(out[0] == 41 && out[1] == 42);
    vsdlss_factor_free(F);
}

static double matrix_value(const vsdlss *A, csi row, csi col)
{
    csi p, tmp;
    if (row > col) { tmp = row; row = col; col = tmp; }
    for (p = A->p[col]; p < A->p[col + 1]; ++p)
        if (A->i[p] == row) return A->x[p];
    return 0.0;
}

static void test_factor_reconstruction(void)
{
    csi p[] = {0,1,3,6,8};
    csi i[] = {0,0,1,0,1,2,2,3};
    double a[] = {6,1,5,.5,1,7,2,8};
    vsdlss A = {8,4,4,p,i,a,-1};
    int order;
    for (order = 1; order <= 2; ++order) {
        vsdlss_factor *F = NULL;
        const vsdlss *L;
        const csi *q;
        double dense_l[16] = {0}, norm_a = 0, norm_err = 0;
        csi col, at, row, k;
        CHECK(vsdlss_factorize(&A, order, &F) == VSDLSS_OK);
        if (!F) continue;
        L = vsdlss_factor_L(F); q = vsdlss_factor_q(F);
        for (col = 0; col < 4; ++col)
            for (at = L->p[col]; at < L->p[col+1]; ++at)
                dense_l[L->i[at]*4 + col] = L->x[at];
        for (row = 0; row < 4; ++row) {
            double row_a = 0, row_e = 0;
            for (col = 0; col < 4; ++col) {
                double got = 0, want = matrix_value(&A, q[row], q[col]);
                for (k = 0; k < 4; ++k) got += dense_l[row*4+k] * dense_l[col*4+k];
                row_a += fabs(want); row_e += fabs(got-want);
            }
            if (row_a > norm_a) norm_a = row_a;
            if (row_e > norm_err) norm_err = row_e;
        }
        CHECK(norm_err / norm_a <= 1e-12);
        vsdlss_factor_free(F);
    }
}

static void test_nonconstant_tridiagonal(void)
{
    const csi n = 64, nnz = 2*n-1;
    csi *p = malloc(((size_t)n+1)*sizeof(*p));
    csi *i = malloc((size_t)nnz*sizeof(*i));
    double *a = malloc((size_t)nnz*sizeof(*a));
    double *want = malloc((size_t)n*sizeof(*want));
    double *rhs = malloc((size_t)n*sizeof(*rhs));
    double *got = malloc((size_t)n*sizeof(*got));
    csi col, at = 0;
    vsdlss A;
    CHECK(p && i && a && want && rhs && got);
    if (!p || !i || !a || !want || !rhs || !got) goto done;
    for (col=0; col<n; ++col) {
        p[col]=at;
        if (col>0) { i[at]=col-1; a[at++]=-1; }
        i[at]=col; a[at++]=4;
        want[col]=(col&1) ? -(double)(col+1)/17.0 : (double)(col+1)/13.0;
    }
    p[n]=at; A=(vsdlss){nnz,n,n,p,i,a,-1};
    CHECK(vsdlss_spmv_sym_upper(&A,want,rhs)==VSDLSS_OK);
    for (int order=1; order<=2; ++order) {
        vsdlss_factor *F=NULL; double eta=1;
        CHECK(vsdlss_factorize(&A,order,&F)==VSDLSS_OK);
        if (!F) continue;
        CHECK(vsdlss_factor_solve(F,rhs,got)==VSDLSS_OK);
        for (col=0; col<n; ++col) CHECK(fabs(got[col]-want[col])<=1e-10);
        CHECK(vsdlss_backward_error(&A,got,rhs,&eta)==VSDLSS_OK && eta<=1e-12);
        vsdlss_factor_free(F);
    }
done:
    free(p); free(i); free(a); free(want); free(rhs); free(got);
}

int main(void)
{
    test_status_strings();
    test_matrix_normalization();
    test_matrix_validation_errors();
    test_ordering_contract();
    test_rcm_high_degree_star();
    test_factor_solve_and_reuse();
    test_factor_failure_semantics();
    test_factor_reconstruction();
    test_nonconstant_tridiagonal();
    if (failures != 0) {
        fprintf(stderr, "test_solver: %d failure(s)\n", failures);
        return 1;
    }
    puts("test_solver: ALL OK");
    return 0;
}
