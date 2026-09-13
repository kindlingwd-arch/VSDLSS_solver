/* Approximate minimum degree ordering.
 *
 * vsdlss_min_degree_order keeps an explicit elimination graph: every
 * elimination materialises the clique among the pivot's neighbours, so its
 * cost grows with the square of the degree and it spends most of its time
 * inserting fill edges it will never look at again.  That is the classic
 * reason exact minimum degree is unusable at scale, not an artefact of this
 * implementation.
 *
 * This file implements the quotient-graph formulation instead (Amestoy,
 * Davis and Duff, "An Approximate Minimum Degree Ordering Algorithm", SIAM J.
 * Matrix Anal. Appl. 17(4), 1996).  An eliminated pivot becomes an *element*
 * and its clique is represented by that element rather than by edges, so no
 * fill is ever written down.  A variable's adjacency is then a list of
 * elements followed by a list of variables, held in one workspace array whose
 * free space is reclaimed by garbage collection.  Three further devices from
 * the paper are needed for the cost to actually come down:
 *
 *   - approximate external degree: the true degree needs a set union over the
 *     variable's elements, so an upper bound computed from set *differences*
 *     is used instead, which is what makes the update linear in the adjacency;
 *   - supervariables: variables with identical adjacency are detected by
 *     hashing and eliminated together, which is also what produces the wide
 *     supernodes the numeric stage wants;
 *   - element absorption (including aggressive absorption) and mass
 *     elimination, which keep the quotient graph from growing.
 *
 * The pivot selection is minimum *approximate* degree, so the permutation is
 * not identical to vsdlss_min_degree_order's exact-degree one and neither
 * dominates the other on fill.  Both are kept: order 3 stays the exact,
 * evidence-matched reconstruction, order 5 is this one.
 */

#include "vsdlss_internal.h"

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* A node's list pointer is negated while the node is dead so that the
 * assembly-tree parent can be recovered after the main loop. */
#define AMD_FLIP(i) (-(i) - 2)

static csi amd_max(csi a, csi b) { return a > b ? a : b; }
static csi amd_min(csi a, csi b) { return a < b ? a : b; }

/* Keep the mark counter below the range where mark + lemax could overflow;
 * resetting w[] to 1 for live nodes restores the invariant w[0..n-1] < mark. */
static csi amd_wclear(csi mark, csi lemax, csi *w, csi n)
{
    if (mark < 2 || mark > INT64_MAX - lemax) {
        csi k;
        for (k = 0; k < n; ++k) if (w[k] != 0) w[k] = 1;
        mark = 2;
    }
    return mark;
}

/* Iterative postorder of the assembly tree rooted at j; writes into post[k..]
 * and returns the next free slot. */
static csi amd_tdfs(csi j, csi k, csi *head, const csi *next, csi *post, csi *stack)
{
    csi top = 0;
    stack[0] = j;
    while (top >= 0) {
        csi p = stack[top], i = head[p];
        if (i == -1) { --top; post[k++] = p; }
        else { head[p] = next[i]; stack[++top] = i; }
    }
    return k;
}

/* Pattern of A + A' with the diagonal dropped, plus elbow room for the
 * quotient graph.  The caller's matrix is upper triangular with a diagonal,
 * so each off-diagonal entry contributes one edge in each direction. */
static vsdlss_status amd_build_graph(const vsdlss *A, csi **out_p, csi **out_i,
                                     csi *out_nzmax)
{
    csi n = A->n, j, p, cnz = 0, t, *Cp = NULL, *Ci = NULL, *cursor = NULL;
    if ((uint64_t)(n + 1) > SIZE_MAX / sizeof(csi)) return VSDLSS_ERR_OOM;
    Cp = (csi *)calloc((size_t)n + 1, sizeof(csi));
    cursor = (csi *)malloc((size_t)n * sizeof(csi));
    if (!Cp || !cursor) { free(Cp); free(cursor); return VSDLSS_ERR_OOM; }
    for (j = 0; j < n; ++j)
        for (p = A->p[j]; p < A->p[j + 1]; ++p) {
            csi i = A->i[p];
            if (i == j) continue;
            if (Cp[j] == INT64_MAX || Cp[i] == INT64_MAX || cnz > INT64_MAX - 2) {
                free(Cp); free(cursor); return VSDLSS_ERR_OOM;
            }
            Cp[j]++; Cp[i]++; cnz += 2;
        }
    /* Elbow room: the quotient graph appends new element lists at the end and
     * garbage collects when it runs out.  cnz/5 + 2n is the AMD default. */
    if (cnz > INT64_MAX - cnz / 5 || cnz + cnz / 5 > INT64_MAX - 2 * n) {
        free(Cp); free(cursor); return VSDLSS_ERR_OOM;
    }
    t = cnz + cnz / 5 + 2 * n;
    if (t < 1) t = 1;
    if ((uint64_t)t > SIZE_MAX / sizeof(csi)) { free(Cp); free(cursor); return VSDLSS_ERR_OOM; }
    Ci = (csi *)malloc((size_t)t * sizeof(csi));
    if (!Ci) { free(Cp); free(cursor); return VSDLSS_ERR_OOM; }
    {   /* prefix sum in place: Cp[j] holds the count, becomes the start */
        csi total = 0;
        for (j = 0; j < n; ++j) { csi c = Cp[j]; Cp[j] = total; cursor[j] = total; total += c; }
        Cp[n] = total;
    }
    for (j = 0; j < n; ++j)
        for (p = A->p[j]; p < A->p[j + 1]; ++p) {
            csi i = A->i[p];
            if (i == j) continue;
            Ci[cursor[j]++] = i;
            Ci[cursor[i]++] = j;
        }
    free(cursor);
    *out_p = Cp; *out_i = Ci; *out_nzmax = t;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_amd_order(const vsdlss *A, csi **out_q,
                               vsdlss_order_stats *stats)
{
    csi n, *Cp = NULL, *Ci = NULL, *W = NULL, *P = NULL;
    csi *len, *nv, *next, *head, *elen, *degree, *w, *hhead, *last;
    csi nzmax, cnz, mark, lemax = 0, nel = 0, mindeg = 0, dense;
    csi i, j, k, k1, k2, e, p, pk, pk1, pk2, p1, p2, p3, p4, pn, pj;
    csi d, dk, dext, elenk, eln, h, ln, nvi, nvj, nvk, wnvi;
    vsdlss_status status;

    if (!out_q) return VSDLSS_ERR_INVALID;
    *out_q = NULL;
    status = vsdlss_validate_upper_csc(A);
    if (status != VSDLSS_OK) return status;
    n = A->n;
    if (n == INT64_MAX || (uint64_t)(n + 1) > SIZE_MAX / sizeof(csi) / 8)
        return VSDLSS_ERR_OOM;
    status = amd_build_graph(A, &Cp, &Ci, &nzmax);
    if (status != VSDLSS_OK) return status;
    cnz = Cp[n];
    P = (csi *)malloc((size_t)(n + 1) * sizeof(csi));
    W = (csi *)malloc((size_t)(n + 1) * 8 * sizeof(csi));
    if (!P || !W) { free(P); free(W); free(Cp); free(Ci); return VSDLSS_ERR_OOM; }
    len = W; nv = W + (n + 1); next = W + 2 * (n + 1); head = W + 3 * (n + 1);
    elen = W + 4 * (n + 1); degree = W + 5 * (n + 1); w = W + 6 * (n + 1);
    hhead = W + 7 * (n + 1);
    last = P;                               /* P doubles as `last` until the end */

    /* --- initialise the quotient graph -------------------------------- */
    for (k = 0; k < n; ++k) len[k] = Cp[k + 1] - Cp[k];
    len[n] = 0;
    for (i = 0; i <= n; ++i) {
        head[i] = -1; last[i] = -1; next[i] = -1; hhead[i] = -1;
        nv[i] = 1; w[i] = 1; elen[i] = 0; degree[i] = len[i];
    }
    mark = amd_wclear(0, 0, w, n);
    elen[n] = -2;                           /* node n is a permanently dead element */
    Cp[n] = -1;                             /* and a root of the assembly tree */
    w[n] = 0;

    /* Rows far denser than the rest distort every degree update; AMD sets
     * them aside into the dummy element n and orders them last. */
    dense = amd_max(16, (csi)(10.0 * sqrt((double)n)));
    dense = amd_min(n - 2, dense);
    if (dense < 0) dense = 0;

    for (i = 0; i < n; ++i) {
        d = degree[i];
        if (d == 0) {                       /* isolated: already eliminated */
            elen[i] = -2; ++nel; Cp[i] = -1; w[i] = 0;
        } else if (d > dense) {
            nv[i] = 0; elen[i] = -1; ++nel;
            Cp[i] = AMD_FLIP(n); nv[n]++;
        } else {
            if (head[d] != -1) last[head[d]] = i;
            next[i] = head[d]; head[d] = i;
        }
    }

    while (nel < n) {
        /* --- select the pivot: minimum approximate degree --------------- */
        for (k = -1; mindeg < n && (k = head[mindeg]) == -1; ++mindeg) ;
        if (k < 0) { status = VSDLSS_ERR_INVALID; goto done; }
        if (next[k] != -1) last[next[k]] = -1;
        head[mindeg] = next[k];
        elenk = elen[k];
        nvk = nv[k];
        nel += nvk;

        /* --- garbage collect if the new element will not fit ------------ */
        if (elenk > 0 && cnz + mindeg >= nzmax) {
            for (j = 0; j < n; ++j) {
                if ((p = Cp[j]) >= 0) {     /* j is a live node */
                    Cp[j] = Ci[p];          /* stash the first entry */
                    Ci[p] = AMD_FLIP(j);    /* and mark the list head */
                }
            }
            {   /* slide every live list down over the dead ones */
                csi q = 0;
                for (p = 0; p < cnz; ) {
                    if ((j = AMD_FLIP(Ci[p++])) >= 0) {
                        Ci[q] = Cp[j];      /* restore the first entry */
                        Cp[j] = q++;
                        for (k2 = 0; k2 < len[j] - 1; ++k2) Ci[q++] = Ci[p++];
                    }
                }
                cnz = q;
            }
        }

        /* --- build element k: absorb its elements and collect Lk -------- */
        dk = 0;
        nv[k] = -nvk;                       /* negative nv marks membership in Lk */
        p = Cp[k];
        pk1 = (elenk == 0) ? p : cnz;       /* overwrite k's list when it has none */
        pk2 = pk1;
        for (k1 = 1; k1 <= elenk + 1; ++k1) {
            if (k1 > elenk) { e = k; pj = p; ln = len[k] - elenk; }
            else { e = Ci[p++]; pj = Cp[e]; ln = len[e]; }
            for (k2 = 1; k2 <= ln; ++k2) {
                i = Ci[pj++];
                if ((nvi = nv[i]) <= 0) continue;   /* already in Lk, or dead */
                dk += nvi;
                nv[i] = -nvi;
                Ci[pk2++] = i;
                if (next[i] != -1) last[next[i]] = last[i];
                if (last[i] != -1) next[last[i]] = next[i];
                else head[degree[i]] = next[i];
            }
            if (e != k) { Cp[e] = AMD_FLIP(k); w[e] = 0; }  /* absorb e into k */
        }
        if (elenk != 0) cnz = pk2;
        degree[k] = dk;
        Cp[k] = pk1; len[k] = pk2 - pk1;
        elen[k] = -2;                       /* k is an element now */

        /* --- set differences: w[e] = |Le \ Lk| for every touched e ------ */
        mark = amd_wclear(mark, lemax, w, n);
        for (pk = pk1; pk < pk2; ++pk) {
            i = Ci[pk];
            if ((eln = elen[i]) <= 0) continue;
            nvi = -nv[i];
            wnvi = mark - nvi;
            for (p = Cp[i]; p <= Cp[i] + eln - 1; ++p) {
                e = Ci[p];
                if (w[e] >= mark) w[e] -= nvi;
                else if (w[e] != 0) w[e] = degree[e] + wnvi;
            }
        }

        /* --- approximate degree update --------------------------------- */
        for (pk = pk1; pk < pk2; ++pk) {
            i = Ci[pk];
            p1 = Cp[i];
            p2 = p1 + elen[i] - 1;
            pn = p1;
            for (h = 0, d = 0, p = p1; p <= p2; ++p) {
                e = Ci[p];
                if (w[e] == 0) continue;    /* e is absorbed into k */
                dext = w[e] - mark;         /* |Le \ Lk| */
                if (dext > 0) { d += dext; Ci[pn++] = e; h += e; }
                else { Cp[e] = AMD_FLIP(k); w[e] = 0; }  /* aggressive absorption */
            }
            elen[i] = pn - p1 + 1;          /* keep room for k itself */
            p3 = pn;
            p4 = p1 + len[i];
            for (p = p2 + 1; p < p4; ++p) { /* the variables in i's list */
                j = Ci[p];
                if ((nvj = nv[j]) <= 0) continue;
                d += nvj;
                Ci[pn++] = j;
                h += j;
            }
            if (d == 0) {                   /* mass elimination: i goes with k */
                Cp[i] = AMD_FLIP(k);
                nvi = -nv[i];
                dk -= nvi; nvk += nvi; nel += nvi;
                nv[i] = 0; elen[i] = -1;
            } else {
                degree[i] = amd_min(degree[i], d);  /* the bound can only improve */
                Ci[pn] = Ci[p3];            /* move the first variable aside */
                Ci[p3] = Ci[p1];            /* and the first element */
                Ci[p1] = k;                 /* so that k leads i's element list */
                len[i] = pn - p1 + 1;
                h = ((h < 0) ? -h : h) % n; /* hash on the adjacency */
                next[i] = hhead[h];
                hhead[h] = i;
                last[i] = h;                /* remember the bucket for the sweep */
            }
        }
        degree[k] = dk;
        lemax = amd_max(lemax, dk);
        mark = amd_wclear(mark + lemax, lemax, w, n);

        /* --- supervariable detection ----------------------------------- */
        for (pk = pk1; pk < pk2; ++pk) {
            i = Ci[pk];
            if (nv[i] >= 0) continue;       /* only variables still in Lk */
            h = last[i];
            i = hhead[h];
            hhead[h] = -1;                  /* the bucket is emptied as it is swept */
            for (; i != -1 && next[i] != -1; i = next[i], ++mark) {
                ln = len[i];
                eln = elen[i];
                for (p = Cp[i] + 1; p <= Cp[i] + ln - 1; ++p) w[Ci[p]] = mark;
                {
                    csi jlast = i;
                    for (j = next[i]; j != -1; ) {
                        int ok = (len[j] == ln) && (elen[j] == eln);
                        for (p = Cp[j] + 1; ok && p <= Cp[j] + ln - 1; ++p)
                            if (w[Ci[p]] != mark) ok = 0;
                        if (ok) {           /* i and j have identical adjacency */
                            Cp[j] = AMD_FLIP(i);
                            nv[i] += nv[j];
                            nv[j] = 0; elen[j] = -1;
                            j = next[j];
                            next[jlast] = j;
                        } else { jlast = j; j = next[j]; }
                    }
                }
            }
        }

        /* --- finalise element k and re-insert its variables ------------- */
        for (p = pk1, pk = pk1; pk < pk2; ++pk) {
            i = Ci[pk];
            if ((nvi = -nv[i]) <= 0) continue;   /* skip absorbed variables */
            nv[i] = nvi;
            d = degree[i] + dk - nvi;            /* the approximate degree */
            d = amd_min(d, n - nel - nvi);
            if (d < 0) d = 0;
            if (head[d] != -1) last[head[d]] = i;
            next[i] = head[d]; last[i] = -1; head[d] = i;
            mindeg = amd_min(mindeg, d);
            degree[i] = d;
            Ci[p++] = i;
        }
        nv[k] = nvk;
        if ((len[k] = p - pk1) == 0) { Cp[k] = -1; w[k] = 0; }  /* k is a root */
        if (elenk != 0) cnz = p;
    }

    /* --- postorder the assembly tree ---------------------------------- */
    for (i = 0; i < n; ++i) Cp[i] = AMD_FLIP(Cp[i]);
    for (j = 0; j <= n; ++j) head[j] = -1;
    for (j = n; j >= 0; --j) {              /* absorbed variables, under parents */
        if (nv[j] > 0) continue;
        next[j] = head[Cp[j]];
        head[Cp[j]] = j;
    }
    for (e = n; e >= 0; --e) {              /* elements, under their parents */
        if (nv[e] <= 0) continue;
        if (Cp[e] != -1) { next[e] = head[Cp[e]]; head[Cp[e]] = e; }
    }
    for (k = 0, i = 0; i <= n; ++i)
        if (Cp[i] == -1) k = amd_tdfs(i, k, head, next, P, w);
    if (k != n + 1 || P[n] != n) { status = VSDLSS_ERR_INVALID; goto done; }

    if (stats) {
        stats->predicted_nnz_l = 0;
        stats->fill_edges_added = 0;
        stats->elimination_tree_height = 0;
        stats->separator_count = 0;
    }
    {   /* P is `last`'s storage; hand back exactly the first n entries. */
        csi *q = (csi *)malloc((size_t)n * sizeof(csi));
        if (!q) { status = VSDLSS_ERR_OOM; goto done; }
        memcpy(q, P, (size_t)n * sizeof(csi));
        *out_q = q;
    }
    status = VSDLSS_OK;
done:
    free(P); free(W); free(Cp); free(Ci);
    return status;
}
