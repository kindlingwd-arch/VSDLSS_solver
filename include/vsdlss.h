#ifndef VSDLSS_H
#define VSDLSS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

typedef int64_t csi;          /* index type */

typedef enum vsdlss_status
{
    VSDLSS_OK = 0,
    VSDLSS_ERR_INVALID,
    VSDLSS_ERR_OOM,
    VSDLSS_ERR_NOT_POSDEF,
    VSDLSS_ERR_NONFINITE,
    VSDLSS_ERR_IO,
    VSDLSS_ERR_UNSUPPORTED
} vsdlss_status;

const char *vsdlss_status_string(vsdlss_status status);
typedef struct vsdlss_sp /* matrix in compressed-column form */
{
    csi nzmax;                /* max entries                            */
    csi m;                    /* rows                                   */
    csi n;                    /* columns                                */
    csi *p;                   /* column pointers (size n+1)             */
    csi *i;                   /* row indices (size nzmax)               */
    double *x;                /* numeric values (size nzmax)            */
    csi nz;                   /* -1 for compressed-column, else count   */
} vsdlss;

typedef struct vsdlss_sym /* symbolic Cholesky analysis */
{
    csi *pinv;                /* inverse perm. (== fill-reducing perm)  */
    csi *parent;              /* elimination tree                       */
    csi *cp;                  /* column pointers for L                  */
    csi *q;                   /* unused for Cholesky                    */
    csi *leftmost;            /* for QR, unused here                    */
    csi m2;
    double lnz;
    double unz;
} vsdlss_sym;

typedef struct vsdlss_num /* numeric Cholesky factorization */
{
    vsdlss *L;                /* sparse lower-triangular factor         */
    vsdlss *U;                /* unused for Cholesky (LU/QR)            */
    csi *pinv;                /* permutation used                       */
    double *B;                /* for QR, unused here                    */
} vsdlss_num;

typedef struct vsdlss_factor vsdlss_factor;
typedef struct vsdlss_m3_factor vsdlss_m3_factor;

typedef struct vsdlss_order_stats
{
    csi predicted_nnz_l;
    csi fill_edges_added;
    csi elimination_tree_height;
    csi separator_count;
} vsdlss_order_stats;

vsdlss_status vsdlss_factorize(const vsdlss *A, int order, vsdlss_factor **out);
vsdlss_status vsdlss_factor_solve(const vsdlss_factor *factor,
                                  const double *rhs, double *solution);
void vsdlss_factor_free(vsdlss_factor *factor);
vsdlss_status vsdlss_factorize_m3(const vsdlss *A, int order,
                                  vsdlss_m3_factor **out);
vsdlss_status vsdlss_m3_solve(const vsdlss_m3_factor *factor,
                              const double *rhs, double *solution);
void vsdlss_m3_factor_free(vsdlss_m3_factor *factor);
const vsdlss *vsdlss_factor_L(const vsdlss_factor *factor);
const csi *vsdlss_factor_q(const vsdlss_factor *factor);
const csi *vsdlss_factor_pinv(const vsdlss_factor *factor);
csi vsdlss_factor_dimension(const vsdlss_factor *factor);

vsdlss_status vsdlss_load_job(const char *job, int allow_missing_rhs,
                              vsdlss **A, double **rhs);
vsdlss_status vsdlss_write_solution(const char *job, const double *solution, csi n);

vsdlss_status vsdlss_validate_upper_csc(const vsdlss *A);
vsdlss_status vsdlss_normalize_upper(const vsdlss *A, vsdlss **out);
vsdlss_status vsdlss_spmv_sym_upper(const vsdlss *A, const double *x, double *y);
vsdlss_status vsdlss_backward_error(const vsdlss *A, const double *x,
                                    const double *b, double *eta);
vsdlss_status vsdlss_validate_permutation(const csi *q, const csi *pinv, csi n);
vsdlss_status vsdlss_order_analyze(const vsdlss *A, int order,
                                   csi **q, csi **pinv,
                                   vsdlss_order_stats *stats);
vsdlss_status vsdlss_order(const vsdlss *A, int order, csi **q, csi **pinv);

/* utilities */
void *vsdlss_malloc(csi n, size_t size);
void *vsdlss_calloc(csi n, size_t size);
void *vsdlss_free(void *p);
void *vsdlss_realloc(void *p, csi n, size_t size, csi *ok);
vsdlss *vsdlss_spalloc(csi m, csi n, csi nzmax, csi values, csi triplet);
vsdlss *vsdlss_spfree(vsdlss *A);
int vsdlss_sprealloc(vsdlss *A, csi nzmax);
double vsdlss_cumsum(csi *p, csi *c, csi n);
vsdlss *vsdlss_done(vsdlss *C, void *w, void *x, int ok);
csi *vsdlss_idone(csi *p, vsdlss *C, void *w, int ok);
vsdlss_num *vsdlss_ndone(vsdlss_num *N, vsdlss *C, void *w, void *x, int ok);
vsdlss_sym *vsdlss_sfree(vsdlss_sym *S);
vsdlss_num *vsdlss_nfree(vsdlss_num *N);

/* core */
csi *vsdlss_etree(const vsdlss *A, csi ata);
csi vsdlss_ereach(const vsdlss *A, csi k, const csi *parent, csi *s, csi *w);
csi *vsdlss_counts(const vsdlss *A, const csi *parent, const csi *post, csi ata);
vsdlss *vsdlss_symperm(const vsdlss *A, const csi *pinv, csi values);
int vsdlss_lsolve(const vsdlss *L, double *x);
int vsdlss_ltsolve(const vsdlss *L, double *x);
int vsdlss_pvec(const csi *p, const double *b, double *x, csi n);
int vsdlss_ipvec(const csi *p, const double *b, double *x, csi n);
csi *vsdlss_pinv(const csi *p, csi n);

csi *vsdlss_post(const csi *parent, csi n);
csi vsdlss_leaf(csi i, csi j, const csi *first, csi *maxfirst,
                csi *prevleaf, csi *ancestor, csi *jleaf);
vsdlss *vsdlss_transpose(const vsdlss *A, csi values);

#endif
