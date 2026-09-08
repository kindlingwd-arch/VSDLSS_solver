#ifndef VSDLSS_CATALOG_H
#define VSDLSS_CATALOG_H
/* vsdlss_catalog
 *
 * A VSDLSS solver that mirrors the FUNCTION / FEATURE CATALOG of the
 * decompiled solver (the set of functions it needs), each implemented with our
 * own correct code.  Function names and responsibilities follow the
 * decompiled asim_3d vsdlss functions:
 *
 *   vectors     : newIVector_vsdlss, newFVector_vsdlss, initVector_vsdlss,
 *                 saxpy_vsdlss, innerProduct_vsdlss
 *   dense       : denseCholesky, solveDenseLower, solveDenseUpper
 *   ordering    : minDegreeOrder, vsdlss_rcm
 *   factor      : factorA_vsdlss, newFactorBlockHeader_vsdlss, newFactorChunk_vsdlss
 *   solve       : forwardSubstitute_vsdlss, backSubstitute_vsdlss,
 *                 solveWithBlockLower, solveWithBlockUpper, solveLoadCase_vsdlss
 *   entry       : solverVsdlss (reads RedHawk .hdr + mat + .rhs, writes .rsl)
 *
 * Numeric core is the (correct, verified) sparse Cholesky in vsdlss.c.
 */
#include "vsdlss.h"
#include <stdint.h>

typedef long longlong;  /* matches Ghidra 'long' handle type */
typedef unsigned long long undefined8;
typedef unsigned int   undefined4;
typedef unsigned long  ulong;
typedef vsdlss         vsdlss_matrix;   /* symmetric matrix (upper CSC) */

/* ---- vector utilities ---- */
longlong newIVector_vsdlss(longlong n, int fill, int flag);   /* int vector  */
longlong newFVector_vsdlss(undefined8 a, longlong n, int flag);/* double vector */
void     initVector_vsdlss(undefined8 a, longlong v, ulong n);
void     saxpy_vsdlss(double a, longlong x, longlong y, int n, longlong z);
double   innerProduct_vsdlss(longlong x, longlong y, int n);

/* ---- dense helpers (used by the block/dense paths) ---- */
void denseCholesky(longlong A, int n);
void solveDenseLower(longlong A, int n, double *b, longlong z);
void solveDenseUpper(longlong A, int n, longlong z, longlong x);

/* ---- ordering (fill-reducing) ---- */
csi *minDegreeOrder(const csi *Ap, const csi *Ai, csi n);   /* approximate min-degree */
csi *vsdlss_rcm(const csi *Ap, const csi *Ai, csi n);       /* reverse Cuthill-McKee  */

/* ---- factor block header / chunk (data model) ---- */
uint32_t *newFactorBlockHeader_vsdlss(int n);
uint32_t *newFactorChunk_vsdlss(uint32_t tag, int a, int b, int c, int d);

/* ---- factor / solve (uses vsdlss.c Cholesky core) ---- */
typedef struct vsdlss_native_factor {
    vsdlss_num *N;    /* sparse Cholesky factor (csn) */
} vsdlss_native_factor;

vsdlss_native_factor *factorA_vsdlss(const vsdlss *A, int order);
void  forwardSubstitute_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void  backSubstitute_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void  solveWithBlockLower(const vsdlss_native_factor *F, double *b, int n);
void  solveWithBlockUpper(const vsdlss_native_factor *F, double *b, int n);
void  solveLoadCase_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void  vsdlss_free_factor(vsdlss_native_factor *F);

/* ---- extra solve-path methods (blocked / part-factor / pre-post) ---- */
void forwardSolveBlocked_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void backwardSolveBlocked_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void solveLoadCaseBlocked_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void forwardSolveFromVsdlss(const vsdlss_native_factor *F, double *b, int n);
void backwardSolveFromVsdlss(const vsdlss_native_factor *F, double *b, int n);
void forwardBackWardSolveFromVsdlss(const vsdlss_native_factor *F, double *b, int n);
void solveLowerTriangPartFact_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void solveUpperTriangPartFact_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void solveLowerTriangForPartFact_vsdlss(const vsdlss_native_factor *F, double *b, int n);
void preSolve_vsdlss(const vsdlss_native_factor *F, const double *rhs, double *x, int n);
void postSolve_vsdlss(const vsdlss_native_factor *F, const double *x, double *out, int n);

/* ---- vector permutation / norms ---- */
void   permuteDVector_vsdlss(const longlong *p, const double *b, double *x, int n);
void   permuteDVectorInPlace_vsdlss(const longlong *p, double *b, int n);
void   permuteIVector_vsdlss(const longlong *p, const int *b, int *x, int n);
void   permuteFVector_vsdlss(const longlong *p, const double *b, double *x, int n);
void   permuteFVectorInPlace_vsdlss(const longlong *p, double *b, int n);
void   permuteIPVector_vsdlss(const longlong *p, const int *b, int *x, int n);
void   permuteIVectorInPlace_vsdlss(const longlong *p, int *b, int n);
void   invPermuteDVector_vsdlss(const longlong *p, const double *b, double *x, int n);
double infNorm_vsdlss(const double *v, int n);
double oneNorm_vsdlss(const double *v, int n);
double twoNorm_vsdlss(const double *v, int n);
double DNorm_vsdlss(longlong v, int n);

/* ---- vector math (double/int handles) ---- */
void   copyVector_vsdlss(longlong src, ulong n, longlong dst);
void   copyFVector_vsdlss(longlong src, ulong n, longlong dst);
void   copyIVector_vsdlss(longlong src, ulong n, longlong dst);
void   addVectors_vsdlss(longlong a, longlong b, int n, longlong dst);
void   subVectors_vsdlss(longlong a, longlong b, int n, longlong dst);
void   addIVectors_vsdlss(longlong a, longlong b, int n, longlong dst);
void   scaleVector_vsdlss(double s, longlong x, int n, longlong y);
void   scaleVectorDiag_vsdlss(longlong x, longlong d, int n, longlong y);
void   sqrtDVector_vsdlss(longlong x, int n, longlong y);
void   sqrtFVector_vsdlss(longlong x, int n, longlong y);
int    sumIVectorElems_vsdlss(longlong v, int n);
int    getMinIVector_vsdlss(longlong v, int n, undefined4 *idx);
int    getMaxIVector_vsdlss(longlong v, int n, undefined4 *idx);
void   copyStrArray_vsdlss(longlong src, ulong n, longlong dst);

/* ---- matrix construction / I-O / validation ---- */
vsdlss_matrix *mainMat_vsdlss(char *job, int mode, int order);
void   readMatrixFromFile_vsdlss(vsdlss_matrix *A);
void   readMatrixFromFileAndCheck_vsdlss(undefined8 A);
void   checkForNegativeDiags_vsdlss(int n, longlong diag);
void   checkResults_vsdlss(const double *x, int n, double tol);
void   writeRhsVec_vsdlss(int job, int n, longlong rhs, longlong tmp, longlong fp, FILE *f);
void   writeNodalVoltages_vsdlss(char *job, double *volt, uint n, longlong fp);

/* ---- statistics of the factorization ---- */
longlong getNonZerosInFactLForVsdlss(const vsdlss_native_factor *F);
void     getFactorStats_vsdlss(const vsdlss_native_factor *F, double *nz, double *ops);
void     printStats_vsdlss(const vsdlss_native_factor *F, int n);
void     printFloatArray_vsdlss(const double *a, int n);
void     printFVectorToLog_vsdlss(longlong v, int n);

/* ---- timing / speed reporting (user + system clock ticks) ---- */
void   casiTimes_vsdlss(clock_t *t);           /* t[0]=user, t[1]=sys */
void   initTime_vsdlss(long *t);
void  *newTime_vsdlss(void);                    /* returns long[2] */
void   deleteTime_vsdlss(void *t);
void   copyTimes_vsdlss(long *d, const long *s);
void   addTimes_vsdlss(long *d, const long *a, const long *b);
void   diffTimes_vsdlss(long *d, const long *a, const long *b);
void   scaleTime_vsdlss(long *t, double f);
void   accumDiffTimes_vsdlss(long *acc, const long *a, const long *b);
void   convertToDeciSeconds_vsdlss(long *t);
void   convertToCentiSeconds_vsdlss(long *t);
void   printTimeDeciSeconds_vsdlss(const long *t);
void   printDiffTimes_vsdlss(const long *a, const long *b);

/* ---- file CLI entry ---- */
int solverVsdlss(int argc, char **argv);

#endif

