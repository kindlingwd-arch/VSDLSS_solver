#ifndef VSDLSS_API_H
#define VSDLSS_API_H

/* vsdlss_solver_api
 *
 * A faithful reproduction of the VSDLSS solver interface, reconstructed from
 * the decompiled solver functions (reverse-engineered from asim_3d).  The
 * function NAMES and their roles mirror the original (vsdlss, solverVsdlss,
 * factorA_vsdlss, forwardSubstitute_vsdlss, backSubstitute_vsdlss,
 * solveLoadCase_vsdlss), but the bodies are a clean, correct implementation of
 * the same sparse-direct (Cholesky LL^T) algorithm.
 *
 * The numeric engine is the reconstructed sparse-Cholesky in vsdlss.c
 * (derived from CSparse).  The solver works on the RedHawk .hdr and .mat
 * (diagonal / row / col / value) + .rhs file format via solverVsdlss.
 */

#include "vsdlss.h"

/* ---- data handles (the original used opaque long handles) ---- */
typedef vsdlss        vsdlss_matrix;   /* symmetric matrix, upper CSC      */
typedef vsdlss_num    vsdlss_factor;   /* Cholesky factor  L               */

/* ---- factor (sparse LL^T = A).  A is the symmetric matrix (upper CSC).
 * order: 0 = RCM fill-reducing, 2 = natural.  Returns factor or NULL. ---- */
vsdlss_factor *factorA_vsdlss(const vsdlss_matrix *A, int order);

/* ---- triangular solves (b in-place) ---- */
/* forward  : solve L y = b   (b -> y) */
void forwardSubstitute_vsdlss(const vsdlss_factor *F, double *b, int n);
/* backward : solve L' x = b  (b -> x) */
void backSubstitute_vsdlss(const vsdlss_factor *F, double *b, int n);

/* ---- full load-case solve: L L' x = b (b in-place -> x) ---- */
void solveLoadCase_vsdlss(const vsdlss_factor *F, double *b, int n);

/* ---- one-shot: factor + solve A x = b (b in-place -> x) ---- */
int vsdlss_solve(int order, const vsdlss_matrix *A, double *b);

/* ---- CLI/file entry (mirrors decompiled solverVsdlss) ----
 * argv = { "vsdlss", ["-p" n] ["-S"], <jobname> , NULL }.
 * Reads <job>.hdr + <job>.matd/.matf/.matt/.mato (+.rhs optional),
 * solves, writes <job>.rsl.  Returns 0 on success. */
int solverVsdlss(int argc, char **argv);

/* ---- helpers used to load a matrix from the RedHawk file format ---- */
vsdlss_matrix *vsdlss_load_matrix(const char *job, int *n, int *o);

#endif
