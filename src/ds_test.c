/* ds_test.c — self-test for the reconstructed VSDLSS data structures. */
#include "vsdlss_ds.h"
#include <stdio.h>
#include <stdlib.h>

static int failures = 0;
static void check(int cond, const char *what)
{
    if (!cond) { fprintf(stderr, "  FAIL: %s\n", what); failures++; }
}

int main(void)
{
    /* ---- priority queue (min-heap on node indices) ---- */
    {
        longlong h = newPq_vsdlss(16);
        int i, n = 10;
        for (i = 0; i < n; i++) insertPqInt_vsdlss(h, i, (double)i);
        int prev = -1, ok = 1;
        while (moreInPq_vsdlss(h)) {
            unsigned int node = (unsigned)-1;
            deqPq_vsdlss(h, &node);
            if ((int)node != prev + 1) ok = 0;   /* should come out 0..9 */
            prev = (int)node;
        }
        check(ok && prev == n - 1, "pq yields monotonic dequeue");
        check(moreInPq_vsdlss(h) == 0, "pq empty after exhausting");
        deletePq_vsdlss(h);
    }

    /* ---- stack (LIFO) ---- */
    {
        longlong s = newStack_vsdlss(4);
        pushAStack_vsdlss(s, 1); pushAStack_vsdlss(s, 2); pushAStack_vsdlss(s, 3);
        check(emptyStack_vsdlss(s) == 0, "stack non-empty");
        check(popAStack_vsdlss(s) == 3, "stack pops LIFO (3)");
        check(popAStack_vsdlss(s) == 2, "stack pops LIFO (2)");
        check(popAStack_vsdlss(s) == 1, "stack pops LIFO (1)");
        check(emptyStack_vsdlss(s) == 1, "stack empty");
        deleteStack_vsdlss(s);
    }

    /* ---- hash table (open addressing) ---- */
    {
        longlong t = newHashTable_vsdlss(16);
        check(hashBucketEmpty_vsdlss(42, (int*)t) == 1, "hash empty before insert");
        insertIntoHt_vsdlss(42, 7, (int*)t);
        check(hashBucketEmpty_vsdlss(42, (int*)t) == 0, "hash occupied after insert");
        deleteBucketFromHt_vsdlss(42, (int*)t);
        check(hashBucketEmpty_vsdlss(42, (int*)t) == 1, "hash empty after delete");
        deleteHashTable_vsdlss(t);
    }

    /* ---- MLD ordering: identity fallback (b=0) is a valid permutation ---- */
    {
        int n = 8, i;
        int perm[8];
        MLDOrder_vsdlss(n, 0, 0, 0, 0, 0, 0, (undefined4*)perm, 0);
        int seen[8] = {0}, ok = 1;
        for (i = 0; i < n; i++) {
            if (perm[i] < 0 || perm[i] >= n || seen[perm[i]]) { ok = 0; break; }
            seen[perm[i]] = 1;
        }
        check(ok, "MLD identity fallback is a valid permutation");
    }

    /* ---- MLD ordering: genuine ordering from a small symmetric matrix ---- */
    {
        /* 4x4 tridiagonal symmetric (lower-only CSC). p/i avoid zero rows. */
        csi p[5] = {0, 1, 2, 3, 4};
        csi i[4] = {0, 1, 2, 3};
        double x[4] = {2, 2, 2, 2};
        vsdlss_matrix A;
        A.nzmax = 4; A.m = 4; A.n = 4; A.p = p; A.i = i; A.x = x; A.nz = -1;
        int perm[4], n = 4, k;
        MLDOrder_vsdlss(n, 0, (undefined8)(longlong)&A, 0, 0, 0, 0, (undefined4*)perm, 0);
        int seen[4] = {0}, ok = 1;
        for (k = 0; k < n; k++) {
            if (perm[k] < 0 || perm[k] >= n || seen[perm[k]]) { ok = 0; break; }
            seen[perm[k]] = 1;
        }
        check(ok, "MLD genuine ordering is a valid permutation");
    }

    if (failures == 0) { printf("ds_test: ALL OK\n"); return 0; }
    fprintf(stderr, "ds_test: %d checks failed\n", failures);
    return 1;
}
