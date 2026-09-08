/* dense_fix.h — exact, correct prototypes for the dense-path functions.
 * Included (first) when recompiling the dense path so pointer-returning calls
 * are NOT truncated (implicit-int default broke 64-bit returns). Types come
 * from vsdlss_ref.h, included below. */
#ifndef DENSE_FIX_H
#define DENSE_FIX_H
#include "vsdlss_ref.h"

extern undefined8 casiMalloc_vsdlss(undefined8 n);

long *      newMatrix(int rows, int cols);
long        newVector_vsdlss(undefined8 a, ulong n, int flag);
void        convertElemArrToDense(int *elem, long dense);
void        denseCholesky(long dense, int n);
void        solveDenseLower(long dense, int n, double *b, long z);
void        solveDenseUpper(long dense, int n, long z, long x);
undefined8  deleteMatrix_vsdlss(long *dense, int n);
void        initVector_vsdlss(undefined8 a, long v, ulong n);
void        saxpy_vsdlss(double a, long x, long y, int n, long z);
void        scaleVector_vsdlss(double a, long x, int n, long y);
undefined8  denseSolve_vsdlss(long elemArr, undefined8 rhs);
#endif
