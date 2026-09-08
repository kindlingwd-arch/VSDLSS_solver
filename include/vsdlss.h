#ifndef VSDLSS_H
#define VSDLSS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

typedef int64_t csi;          /* index type */
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
vsdlss_num *vsdlss_chol(const vsdlss *A, const vsdlss_sym *S);
int vsdlss_lsolve(const vsdlss *L, double *x);
int vsdlss_ltsolve(const vsdlss *L, double *x);
int vsdlss_pvec(const csi *p, const double *b, double *x, csi n);
int vsdlss_ipvec(const csi *p, const double *b, double *x, csi n);
csi *vsdlss_pinv(const csi *p, csi n);

csi *vsdlss_post(const csi *parent, csi n);
csi vsdlss_leaf(csi i, csi j, const csi *first, csi *maxfirst,
                csi *prevleaf, csi *ancestor, csi *jleaf);
vsdlss *vsdlss_transpose(const vsdlss *A, csi values);

/* high-level convenience: solve A x = b for symmetric positive definite A.
   A is supplied in upper-triangular compressed-column form (rows i<=j).
   Returns 1 on success, 0 on failure. */
int vsdlss_cholsolve(int order, const vsdlss *A, double *b);
/* Build the sparse chol factor (with ordering); caller owns it. */
vsdlss_num *vsdlss_chol_factor(const vsdlss *A, int order);

/* reorderings (fill-reducing) */
csi *vsdlss_rcm(const csi *Ap, const csi *Ai, csi n); /* returns permi */
csi *vsdlss_identity_perm(csi n);

#endif
