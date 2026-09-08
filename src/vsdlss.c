/* vsdlss.c
 *
 * VSDLSS-style Very Sparse Direct Linear System Solver.
 *
 * This is a self-contained, compilable reconstruction of the algorithm
 * class implemented by "VSDLSS 4.4 (Very Sparse Direct Linear System
 * Solver)" reverse-engineered from the RedHawk-SC asim_3d binary.
 *
 * It performs, in the same order as the original solver:
 *   1. fill-reducing ordering  (vsdlss_rcm / inverse Cuthill-McKee),
 *   2. symbolic analysis       (elimination tree + column counts),
 *   3. numeric LL' factorization (sparse Cholesky) - "factorA",
 *   4. forward/back substitution (vsdlss_lsolve / vsdlss_ltsolve) - "solve".
 *
 * The code is derived from the CSparse library (Timothy A. Davis,
 * LGPL-2.1+).  All routines are prefixed with 'vsdlss_'.
 */

#include "vsdlss.h"

static csi vsdlss_tdfs(csi j, csi k, csi *head, const csi *next, csi *post, csi *stack);
csi vsdlss_leaf(csi i, csi j, const csi *first, csi *maxfirst,
                csi *prevleaf, csi *ancestor, csi *jleaf);

/* ------------------------------------------------------------------------
 * memory wrappers
 * --------------------------------------------------------------------- */
void *vsdlss_malloc(csi n, size_t size) { return malloc(size ? (size_t)n * size : 1); }
void *vsdlss_calloc(csi n, size_t size) { return calloc(size ? (size_t)n : 1, size); }
void *vsdlss_free(void *p) { if (p) free(p); return NULL; }
void *vsdlss_realloc(void *p, csi n, size_t size, csi *ok)
{
    void *pnew;
    if (!p) { pnew = malloc((size_t)n * size); *ok = (pnew != NULL); return pnew; }
    pnew = realloc(p, size ? (size_t)n * size : 1);
    *ok = (pnew != NULL);
    return ((*ok) ? pnew : p);
}

/* ------------------------------------------------------------------------
 * sparse matrix allocation
 * --------------------------------------------------------------------- */
vsdlss *vsdlss_spalloc(csi m, csi n, csi nzmax, csi values, csi triplet)
{
    vsdlss *A = (vsdlss *)vsdlss_calloc(1, sizeof(vsdlss));
    if (!A) return NULL;
    A->m = m; A->n = n;
    A->nzmax = (nzmax > 1) ? nzmax : 1;
    A->nz = triplet ? 0 : -1;
    A->p = (csi *)vsdlss_malloc(triplet ? A->nzmax : n + 1, sizeof(csi));
    A->i = (csi *)vsdlss_malloc(A->nzmax, sizeof(csi));
    A->x = values ? (double *)vsdlss_malloc(A->nzmax, sizeof(double)) : NULL;
    return ((!A->p || !A->i || (values && !A->x)) ? vsdlss_spfree(A) : A);
}

int vsdlss_sprealloc(vsdlss *A, csi nzmax)
{
    csi ok, oki, okj = 1, okx = 1;
    if (!A) return 0;
    if (nzmax <= 0) nzmax = (A->nz == -1) ? A->p[A->n] : A->nz;
    nzmax = (nzmax > 1) ? nzmax : 1;
    A->i = (csi *)vsdlss_realloc(A->i, nzmax, sizeof(csi), &oki);
    if (A->nz >= 0) A->p = (csi *)vsdlss_realloc(A->p, nzmax, sizeof(csi), &okj);
    if (A->x) A->x = (double *)vsdlss_realloc(A->x, nzmax, sizeof(double), &okx);
    ok = (oki && okj && okx);
    if (ok) A->nzmax = nzmax;
    return ok;
}

vsdlss *vsdlss_spfree(vsdlss *A)
{
    if (!A) return NULL;
    vsdlss_free(A->p); vsdlss_free(A->i); vsdlss_free(A->x);
    return (vsdlss *)vsdlss_free(A);
}

double vsdlss_cumsum(csi *p, csi *c, csi n)
{
    csi i, nz = 0;
    double nz2 = 0;
    if (!p || !c) return -1;
    for (i = 0; i < n; i++) {
        p[i] = nz; nz += c[i]; nz2 += c[i]; c[i] = p[i];
    }
    p[n] = nz;
    return nz2;
}

vsdlss *vsdlss_done(vsdlss *C, void *w, void *x, int ok)
{
    vsdlss_free(w); vsdlss_free(x);
    return ok ? C : vsdlss_spfree(C);
}
csi *vsdlss_idone(csi *p, vsdlss *C, void *w, int ok)
{
    vsdlss_spfree(C); vsdlss_free(w);
    return ok ? p : (csi *)vsdlss_free(p);
}
vsdlss_num *vsdlss_ndone(vsdlss_num *N, vsdlss *C, void *w, void *x, int ok)
{
    vsdlss_spfree(C); vsdlss_free(w); vsdlss_free(x);
    return ok ? N : vsdlss_nfree(N);
}
vsdlss_sym *vsdlss_sfree(vsdlss_sym *S)
{
    if (!S) return NULL;
    vsdlss_free(S->pinv); vsdlss_free(S->q); vsdlss_free(S->parent);
    vsdlss_free(S->cp); vsdlss_free(S->leftmost);
    return (vsdlss_sym *)vsdlss_free(S);
}
vsdlss_num *vsdlss_nfree(vsdlss_num *N)
{
    if (!N) return NULL;
    vsdlss_spfree(N->L); vsdlss_spfree(N->U); vsdlss_free(N->pinv); vsdlss_free(N->B);
    return (vsdlss_num *)vsdlss_free(N);
}

/* ------------------------------------------------------------------------
 * elimination tree and reach
 * --------------------------------------------------------------------- */
csi *vsdlss_etree(const vsdlss *A, csi ata)
{
    csi i, k, p, m, n, inext, *Ap, *Ai, *w, *parent, *ancestor, *prev;
    if (!A) return NULL;
    m = A->m; n = A->n; Ap = A->p; Ai = A->i;
    parent = (csi *)vsdlss_malloc(n, sizeof(csi));
    w = (csi *)vsdlss_malloc(n + (ata ? m : 0), sizeof(csi));
    if (!w || !parent) return (csi *)vsdlss_idone(parent, NULL, w, 0);
    ancestor = w; prev = w + n;
    if (ata) for (i = 0; i < m; i++) prev[i] = -1;
    for (k = 0; k < n; k++) {
        parent[k] = -1; ancestor[k] = -1;
        for (p = Ap[k]; p < Ap[k + 1]; p++) {
            i = ata ? (prev[Ai[p]]) : (Ai[p]);
            for (; i != -1 && i < k; i = inext) {
                inext = ancestor[i]; ancestor[i] = k;
                if (inext == -1) parent[i] = k;
            }
            if (ata) prev[Ai[p]] = k;
        }
    }
    return (csi *)vsdlss_idone(parent, NULL, w, 1);
}

#define CS_FLIP(i) (-(i) - 2)
#define CS_MARKED(w, j) ((w)[j] < 0)
#define CS_MARK(w, j)   { (w)[j] = CS_FLIP((w)[j]); }

csi vsdlss_ereach(const vsdlss *A, csi k, const csi *parent, csi *s, csi *w)
{
    csi i, p, n, len, top, *Ap, *Ai;
    if (!A || !parent || !s || !w) return -1;
    top = n = A->n; Ap = A->p; Ai = A->i;
    CS_MARK(w, k);
    for (p = Ap[k]; p < Ap[k + 1]; p++) {
        i = Ai[p];
        if (i > k) continue;
        for (len = 0; !CS_MARKED(w, i); i = parent[i]) {
            s[len++] = i;
            CS_MARK(w, i);
        }
        while (len > 0) s[--top] = s[--len];
    }
    for (p = top; p < n; p++) CS_MARK(w, s[p]);
    CS_MARK(w, k);
    return top;
}

/* ------------------------------------------------------------------------
 * post order (used by column counts / symbolic analysis)
 * --------------------------------------------------------------------- */
csi *vsdlss_post(const csi *parent, csi n)
{
    csi j, k = 0, *post, *w, *head, *next, *stack;
    if (!parent) return NULL;
    post = (csi *)vsdlss_malloc(n, sizeof(csi));
    w = (csi *)vsdlss_malloc(3 * n, sizeof(csi));
    if (!w || !post) return (csi *)vsdlss_idone(post, NULL, w, 0);
    head = w; next = w + n; stack = w + 2 * n;
    for (j = 0; j < n; j++) head[j] = -1;
    for (j = n - 1; j >= 0; j--) { if (parent[j] != -1) { next[j] = head[parent[j]]; head[parent[j]] = j; } }
    for (j = 0; j < n; j++) {
        if (parent[j] != -1) continue;
        k = (csi)vsdlss_tdfs(j, k, head, next, post, stack);
    }
    return (csi *)vsdlss_idone(post, NULL, w, 1);
}

static csi vsdlss_tdfs(csi j, csi k, csi *head, const csi *next, csi *post, csi *stack)
{
    csi i, p, top = 0;
    if (!head || !next || !post || !stack) return -1;
    stack[top] = j;
    while (top >= 0) {
        p = stack[top];
        i = head[p];
        if (i == -1) {
            top--;
            post[k++] = p;
        } else {
            head[p] = next[i];
            stack[++top] = i;
        }
    }
    return k;
}

/* ------------------------------------------------------------------------
 * column counts of L for LL'=A
 * --------------------------------------------------------------------- */
csi vsdlss_leaf(csi i, csi j, const csi *first, csi *maxfirst,
                csi *prevleaf, csi *ancestor, csi *jleaf)
{
    csi q, s, sparent, jprev;
    if (!first || !maxfirst || !prevleaf || !ancestor || !jleaf) return -1;
    *jleaf = 0;
    if (i <= j || first[j] <= maxfirst[i]) return -1;
    maxfirst[i] = first[j];
    jprev = prevleaf[i];
    prevleaf[i] = j;
    *jleaf = (jprev == -1) ? 1 : 2;
    if (*jleaf == 1) return i;
    for (q = jprev; q != ancestor[q]; q = ancestor[q]) ;
    for (s = jprev; s != q; s = sparent) {
        sparent = ancestor[s];
        ancestor[s] = q;
    }
    return q;
}

csi *vsdlss_counts(const vsdlss *A, const csi *parent, const csi *post, csi ata)
{
    csi i, j, k, n, J, s, p, q, jleaf, *ATp, *ATi, *maxfirst, *prevleaf,
        *ancestor, *colcount, *w, *first, *delta;
    vsdlss *AT;
    if (!A || !parent || !post) return NULL;
    n = A->n;
    s = 4 * n + (ata ? (n + A->m + 1) : 0);
    delta = colcount = (csi *)vsdlss_calloc(n, sizeof(csi));
    w = (csi *)vsdlss_malloc(s, sizeof(csi));
    AT = vsdlss_transpose(A, 0);
    if (!AT || !colcount || !w) return (csi *)vsdlss_idone(colcount, AT, w, 0);
    ancestor = w; maxfirst = w + n; prevleaf = w + 2 * n; first = w + 3 * n;
    for (k = 0; k < s; k++) w[k] = -1;
    for (k = 0; k < n; k++) {
        j = post[k];
        delta[j] = (first[j] == -1) ? 1 : 0;
        for (; j != -1 && first[j] == -1; j = parent[j]) first[j] = k;
    }
    ATp = AT->p; ATi = AT->i;
    for (i = 0; i < n; i++) ancestor[i] = i;
    for (k = 0; k < n; k++) {
        j = post[k];
        if (parent[j] != -1) delta[parent[j]]--;
        for (J = j; J != -1; J = -1) {
            for (p = ATp[J]; p < ATp[J + 1]; p++) {
                i = ATi[p];
                q = (csi)vsdlss_leaf(i, j, first, maxfirst, prevleaf, ancestor, &jleaf);
                if (jleaf >= 1) delta[j]++;
                if (jleaf == 2) delta[q]--;
            }
        }
        if (parent[j] != -1) ancestor[j] = parent[j];
    }
    for (j = 0; j < n; j++) if (parent[j] != -1) colcount[parent[j]] += colcount[j];
    return (csi *)vsdlss_idone(colcount, AT, w, 1);
}

/* ------------------------------------------------------------------------
 * transpose
 * --------------------------------------------------------------------- */
vsdlss *vsdlss_transpose(const vsdlss *A, csi values)
{
    csi p, q, j, i, m, n, *Ap, *Ai, *Cp, *Ci, *w;
    double *Ax, *Cx;
    vsdlss *C;
    if (!A) return NULL;
    m = A->m; n = A->n; Ap = A->p; Ai = A->i; Ax = A->x;
    C = vsdlss_spalloc(n, m, Ap[n], values && Ax != NULL, 0);
    w = (csi *)vsdlss_calloc(m, sizeof(csi));
    if (!C || !w) return (vsdlss *)vsdlss_done(C, w, NULL, 0);
    Cp = C->p; Ci = C->i; Cx = C->x;
    for (p = 0; p < Ap[n]; p++) w[Ai[p]]++;
    vsdlss_cumsum(Cp, w, m);
    for (j = 0; j < n; j++) {
        for (p = Ap[j]; p < Ap[j + 1]; p++) {
            i = Ai[p];
            Ci[q = w[i]++] = j;
            if (Cx) Cx[q] = Ax[p];
        }
    }
    return (vsdlss *)vsdlss_done(C, w, NULL, 1);
}

/* ------------------------------------------------------------------------
 * symmetric permutation: C = A(p,p), upper part stored in A
 * --------------------------------------------------------------------- */
vsdlss *vsdlss_symperm(const vsdlss *A, const csi *pinv, csi values)
{
    csi i, j, p, q, i2, j2, n, *Ap, *Ai, *Cp, *Ci, *w;
    double *Cx, *Ax;
    vsdlss *C;
    if (!A) return NULL;
    n = A->n; Ap = A->p; Ai = A->i; Ax = A->x;
    C = vsdlss_spalloc(n, n, Ap[n], values && Ax != NULL, 0);
    w = (csi *)vsdlss_calloc(n, sizeof(csi));
    if (!C || !w) return (vsdlss *)vsdlss_done(C, w, NULL, 0);
    Cp = C->p; Ci = C->i; Cx = C->x;
    for (j = 0; j < n; j++) {
        j2 = pinv ? pinv[j] : j;
        for (p = Ap[j]; p < Ap[j + 1]; p++) {
            i = Ai[p];
            if (i > j) continue;
            i2 = pinv ? pinv[i] : i;
            w[(i2 > j2 ? i2 : j2)]++;
        }
    }
    vsdlss_cumsum(Cp, w, n);
    for (j = 0; j < n; j++) {
        j2 = pinv ? pinv[j] : j;
        for (p = Ap[j]; p < Ap[j + 1]; p++) {
            i = Ai[p];
            if (i > j) continue;
            i2 = pinv ? pinv[i] : i;
            Ci[q = w[(i2 > j2 ? i2 : j2)]++] = (i2 < j2 ? i2 : j2);
            if (Cx) Cx[q] = Ax[p];
        }
    }
    return (vsdlss *)vsdlss_done(C, w, NULL, 1);
}

/* ------------------------------------------------------------------------
 * sparse Cholesky: L*L' = C where C = A(p,p)
 * --------------------------------------------------------------------- */
vsdlss_num *vsdlss_chol(const vsdlss *A, const vsdlss_sym *S)
{
    double d, lki, *Lx, *x, *Cx;
    csi top, i, p, k, n, *Li, *Lp, *cp, *pinv, *s, *c, *parent, *Cp, *Ci;
    vsdlss *L, *C, *E;
    vsdlss_num *N;
    if (!A || !S || !S->cp || !S->parent) return NULL;
    n = A->n;
    N = (vsdlss_num *)vsdlss_calloc(1, sizeof(vsdlss_num));
    c = (csi *)vsdlss_malloc(2 * n, sizeof(csi));
    x = (double *)vsdlss_malloc(n, sizeof(double));
    cp = S->cp; pinv = S->pinv; parent = S->parent;
    C = pinv ? vsdlss_symperm(A, pinv, 1) : (vsdlss *)A;
    E = pinv ? C : NULL;
    if (!N || !c || !x || !C) return (vsdlss_num *)vsdlss_ndone(N, E, c, x, 0);
    s = c + n;
    Cp = C->p; Ci = C->i; Cx = C->x;
    N->L = L = vsdlss_spalloc(n, n, cp[n], 1, 0);
    if (!L) return (vsdlss_num *)vsdlss_ndone(N, E, c, x, 0);
    Lp = L->p; Li = L->i; Lx = L->x;
    for (k = 0; k < n; k++) Lp[k] = c[k] = cp[k];
    for (k = 0; k < n; k++) {
        top = vsdlss_ereach(C, k, parent, s, c);
        x[k] = 0;
        for (p = Cp[k]; p < Cp[k + 1]; p++) if (Ci[p] <= k) x[Ci[p]] = Cx[p];
        d = x[k]; x[k] = 0;
        for (; top < n; top++) {
            i = s[top];
            lki = x[i] / Lx[Lp[i]];
            x[i] = 0;
            for (p = Lp[i] + 1; p < c[i]; p++) x[Li[p]] -= Lx[p] * lki;
            d -= lki * lki;
            p = c[i]++;
            Li[p] = k; Lx[p] = lki;
        }
        if (d <= 0) return (vsdlss_num *)vsdlss_ndone(N, E, c, x, 0);
        p = c[k]++;
        Li[p] = k; Lx[p] = sqrt(d);
    }
    Lp[n] = cp[n];
    return (vsdlss_num *)vsdlss_ndone(N, E, c, x, 1);
}

/* ------------------------------------------------------------------------
 * triangular solves
 * --------------------------------------------------------------------- */
int vsdlss_lsolve(const vsdlss *L, double *x)
{
    csi p, j, n, *Lp, *Li;
    double *Lx;
    if (!L || !x) return 0;
    n = L->n; Lp = L->p; Li = L->i; Lx = L->x;
    for (j = 0; j < n; j++) {
        x[j] /= Lx[Lp[j]];
        for (p = Lp[j] + 1; p < Lp[j + 1]; p++) x[Li[p]] -= Lx[p] * x[j];
    }
    return 1;
}
int vsdlss_ltsolve(const vsdlss *L, double *x)
{
    csi p, j, n, *Lp, *Li;
    double *Lx;
    if (!L || !x) return 0;
    n = L->n; Lp = L->p; Li = L->i; Lx = L->x;
    for (j = n - 1; j >= 0; j--) {
        for (p = Lp[j] + 1; p < Lp[j + 1]; p++) x[j] -= Lx[p] * x[Li[p]];
        x[j] /= Lx[Lp[j]];
    }
    return 1;
}

/* ------------------------------------------------------------------------
 * vector permutations
 * --------------------------------------------------------------------- */
int vsdlss_pvec(const csi *p, const double *b, double *x, csi n)
{
    csi k;
    if (!x || !b) return 0;
    for (k = 0; k < n; k++) x[k] = b[p ? p[k] : k];
    return 1;
}
int vsdlss_ipvec(const csi *p, const double *b, double *x, csi n)
{
    csi k;
    if (!x || !b) return 0;
    for (k = 0; k < n; k++) x[p ? p[k] : k] = b[k];
    return 1;
}
csi *vsdlss_pinv(const csi *p, csi n)
{
    csi k, *pinv;
    if (!p) return NULL;
    pinv = (csi *)vsdlss_malloc(n, sizeof(csi));
    if (!pinv) return NULL;
    for (k = 0; k < n; k++) pinv[p[k]] = k;
    return pinv;
}

/* column counts of L (CSC) using the same ereach routine cs_chol relies on.
 * Each reach(k) gives the rows of row-k of L; those rows are the columns j
 * that receive the entry L(k,j).  Adding a diagonal per column yields the
 * CSC column counts of L. */
static csi *vsdlss_cholcounts(const vsdlss *A, const csi *parent)
{
    csi n, k, idx, top, j, *colcount = NULL, *s = NULL, *w = NULL;
    csi i;
    if (!A || !parent) return NULL;
    n = A->n;
    colcount = (csi *)vsdlss_calloc(n, sizeof(csi));
    s = (csi *)vsdlss_malloc(n, sizeof(csi));
    w = (csi *)vsdlss_malloc(n, sizeof(csi));
    if (!colcount || !s || !w) { vsdlss_free(colcount); vsdlss_free(s); vsdlss_free(w); return NULL; }
    for (i = 0; i < n; i++) w[i] = 0;
    for (k = 0; k < n; k++) {
        top = vsdlss_ereach(A, k, parent, s, w);
        for (idx = top; idx < n; idx++) colcount[s[idx]]++;
    }
    for (j = 0; j < n; j++) colcount[j]++;   /* diagonal entry L(j,j) */
    vsdlss_free(s); vsdlss_free(w);
    return colcount;
}

/* ------------------------------------------------------------------------
 * high-level solve: A x = b, A symmetric positive definite (upper CSC)
 * --------------------------------------------------------------------- */
/* Build the sparse Cholesky factor of A (fill-reducing order if requested).
 * Returns the numeric factor (vsdlss_num) with N->pinv = ordering, or NULL.
 * The factor is on the permuted C = A(p,p); use N->pinv to permute b.      */
vsdlss_num *vsdlss_chol_factor(const vsdlss *A, int order)
{
    csi n, *pinv = NULL, *parent, *cp, *q = NULL, *colcount;
    vsdlss_sym *S;
    vsdlss_num *N;
    vsdlss *C;
    if (!A || !A->x) return NULL;
    n = A->n;

    if (order == 1) q = vsdlss_rcm(A->p, A->i, n);
    else if (order == 2) q = vsdlss_identity_perm(n);
    else { q = vsdlss_rcm(A->p, A->i, n); }
    pinv = (q) ? vsdlss_pinv(q, n) : NULL;
    vsdlss_free(q);

    C = pinv ? vsdlss_symperm(A, pinv, 1) : (vsdlss *)A;

    parent = vsdlss_etree(C, 0);
    colcount = vsdlss_cholcounts(C, parent);
    if (!colcount) return NULL;
    cp = (csi *)vsdlss_malloc(n + 1, sizeof(csi));
    vsdlss_cumsum(cp, colcount, n);
    vsdlss_free(colcount);

    S = (vsdlss_sym *)vsdlss_calloc(1, sizeof(vsdlss_sym));
    if (!S) { vsdlss_free(cp); vsdlss_free(parent); vsdlss_free(pinv); return NULL; }
    S->pinv = NULL;
    S->parent = parent;
    S->cp = cp;

    N = vsdlss_chol(C, S);
    vsdlss_sfree(S);
    if (!N) { if (C != A) vsdlss_spfree(C); vsdlss_free(pinv); return NULL; }
    N->pinv = pinv;                 /* ordering for the caller */
    if (C != A) vsdlss_spfree(C);
    return N;
}

int vsdlss_cholsolve(int order, const vsdlss *A, double *b)
{
    csi n, *pinv = NULL, *parent, *cp, *q = NULL, *colcount;
    vsdlss_sym *S;
    vsdlss_num *N;
    vsdlss *C;
    double *x;
    if (!A || !A->x) return 0;
    n = A->n;

    /* --- ordering --- */
    if (order == 1) q = vsdlss_rcm(A->p, A->i, n);
    else if (order == 2) q = vsdlss_identity_perm(n);
    pinv = (q) ? vsdlss_pinv(q, n) : NULL;   /* pinv = inverse ordering */
    vsdlss_free(q);

    /* --- permuted matrix C = A(p,p) --- */
    C = pinv ? vsdlss_symperm(A, pinv, 1) : (vsdlss *)A;

    /* --- symbolic (on C) --- */
    parent = vsdlss_etree(C, 0);
    colcount = vsdlss_cholcounts(C, parent);
    if (!colcount) { return 0; }
    cp = (csi *)vsdlss_malloc(n + 1, sizeof(csi));
    vsdlss_cumsum(cp, colcount, n);
    vsdlss_free(colcount);

    S = (vsdlss_sym *)vsdlss_calloc(1, sizeof(vsdlss_sym));
    if (!S) { vsdlss_free(cp); vsdlss_free(parent); vsdlss_free(pinv); return 0; }
    S->pinv = NULL;           /* C already permuted */
    S->parent = parent;
    S->cp = cp;

    /* --- numeric on C --- */
    N = vsdlss_chol(C, S);
    vsdlss_sfree(S);
    if (!N) { if (C != A) vsdlss_spfree(C); vsdlss_free(pinv); return 0; }

    /* --- solve: C z = b(p), then x = z(pinv) --- */
    x = (double *)vsdlss_malloc(n, sizeof(double));
    if (!x) { vsdlss_nfree(N); if (C != A) vsdlss_spfree(C); vsdlss_free(pinv); return 0; }
    vsdlss_ipvec(pinv, b, x, n);
    vsdlss_lsolve(N->L, x);
    vsdlss_ltsolve(N->L, x);
    vsdlss_pvec(pinv, x, b, n);

    vsdlss_free(x);
    vsdlss_nfree(N);
    if (C != A) vsdlss_spfree(C);
    vsdlss_free(pinv);
    return 1;
}

/* ------------------------------------------------------------------------
 * orderings
 * --------------------------------------------------------------------- */
csi *vsdlss_identity_perm(csi n)
{
    csi *q = (csi *)vsdlss_malloc(n, sizeof(csi)), k;
    if (!q) return NULL;
    for (k = 0; k < n; k++) q[k] = k;
    return q;
}

/* Reverse Cuthill-McKee ordering of the symmetric graph (upper CSC given).
 * Returns permi such that the new index of node i is permi[i]; NULL on OOM. */
csi *vsdlss_rcm(const csi *Ap, const csi *Ai, csi n)
{
    csi *degree, *perm, *adjstart, *adj, *adjpos, *visited, *queue, *order;
    csi i, j, k, p, deg, e, v, u, start, cnt, q, tmp, a, b;
    if (!Ap || !Ai || n <= 0) return NULL;

    degree = (csi *)vsdlss_calloc(n, sizeof(csi));
    adjstart = (csi *)vsdlss_malloc(n + 1, sizeof(csi));
    perm = (csi *)vsdlss_malloc(n, sizeof(csi));
    visited = (csi *)vsdlss_calloc(n, sizeof(csi));
    if (!degree || !adjstart || !perm || !visited) {
        vsdlss_free(degree); vsdlss_free(adjstart); vsdlss_free(perm);
        vsdlss_free(visited); return NULL;
    }

    /* degree of the undirected graph, using the upper-triangular pattern */
    for (j = 0; j < n; j++)
        for (p = Ap[j]; p < Ap[j + 1]; p++) {
            i = Ai[p];
            if (i < j) { degree[i]++; degree[j]++; }
        }
    adjstart[0] = 0;
    for (i = 0; i < n; i++) adjstart[i + 1] = adjstart[i] + degree[i];
    deg = adjstart[n];
    adj = (csi *)vsdlss_malloc(deg > 0 ? deg : 1, sizeof(csi));
    adjpos = (csi *)vsdlss_malloc(n, sizeof(csi));
    if (!adj || !adjpos) {
        vsdlss_free(degree); vsdlss_free(adjstart); vsdlss_free(perm);
        vsdlss_free(visited); vsdlss_free(adj); vsdlss_free(adjpos);
        return NULL;
    }
    for (i = 0; i < n; i++) adjpos[i] = adjstart[i];
    for (j = 0; j < n; j++)
        for (p = Ap[j]; p < Ap[j + 1]; p++) {
            i = Ai[p];
            if (i < j) { adj[adjpos[j]++] = i; adj[adjpos[i]++] = j; }
        }
    vsdlss_free(adjpos);

    queue = (csi *)vsdlss_malloc((n > 0 ? n : 1), sizeof(csi));
    order = (csi *)vsdlss_malloc((n > 0 ? n : 1), sizeof(csi));
    if (!queue || !order) {
        vsdlss_free(degree); vsdlss_free(adjstart); vsdlss_free(perm);
        vsdlss_free(visited); vsdlss_free(adj); vsdlss_free(queue);
        vsdlss_free(order);
        return NULL;
    }

    cnt = 0;
    for (start = 0; start < n; start++) {
        if (visited[start]) continue;
        /* pseudo-peripheral start: unvisited minimum-degree node (approx) */
        v = -1; deg = 0;
        for (i = 0; i < n; i++)
            if (!visited[i] && (v == -1 || degree[i] < deg)) { v = i; deg = degree[i]; }
        if (v == -1) break;
        /* BFS, ordering neighbors within a level by increasing degree */
        q = 0;
        queue[q++] = v; visited[v] = 1;
        for (k = 0; k < q; k++) {
            u = queue[k];
            csi nb[512], nn = 0;
            for (e = adjstart[u]; e < adjstart[u + 1]; e++) {
                csi cand = adj[e];
                if (!visited[cand]) { visited[cand] = 1; if (nn < 512) nb[nn++] = cand; }
            }
            for (a = 0; a < nn; a++)
                for (b = a + 1; b < nn; b++)
                    if (degree[nb[b]] < degree[nb[a]]) { tmp = nb[a]; nb[a] = nb[b]; nb[b] = tmp; }
            for (a = 0; a < nn; a++) queue[q++] = nb[a];
        }
        /* append BFS order for this component to the global order */
        for (k = 0; k < q; k++) order[cnt + k] = queue[k];
        cnt += q;
    }
    /* Reverse-Cuthill-McKee: numbering is the reverse of the BFS order */
    for (k = 0; k < cnt; k++) perm[order[k]] = cnt - 1 - k;
    vsdlss_free(degree); vsdlss_free(visited); vsdlss_free(queue);
    vsdlss_free(order); vsdlss_free(adj); vsdlss_free(adjstart);
    return perm;
}
