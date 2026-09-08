/* VSDLSS harness contract (reverse-engineered interface).
 *
 * This is the reconstructed calling interface of the VSDLSS solver.  It lets
 * a C driver talk to the real (decompiled) functions.  It is not the
 * self-contained solver (see src/vsdlss.c) but the ABI/interface layer.
 */

#ifndef VSDLSS_HARNESS_H
#define VSDLSS_HARNESS_H

#include <stdint.h>
#include <stdio.h>

typedef unsigned char      undefined;
typedef unsigned int       undefined4;
typedef unsigned long long undefined8;
typedef long               iconst;

/* ---- minimal std::string layout (used inside CasiSolver) ---- */
typedef struct string24 {      /* 24 bytes: ptr @0x8, size @0x10, cap/data */
    undefined8 vtable;         /* @0x00 : object vtable pointer (not here) */
    char       *ptr;           /* @0x08 */
    size_t      size;          /* @0x10 */
    char       *data;          /* @0x18 */
} string24;

/* ---- CasiSolver<T>: vtable + std::string(jobname) + solver data ---- */
typedef struct CasiSolver {
    undefined8 vtable;         /* @0x00 */
    string24   jobname;        /* @0x08..0x20 : solver job name */
    void      *matrix;         /* @0x28 : Symmetric<T>*  */
    void      *rhs;            /* @0x30 : RHS vector     */
} CasiSolver;

/* ---- the top-level, command-line style solver entry (solverVsdl64) ----
 * Parses argv (options -S -U -u -W -w -p <n> -s) then the job name, appends
 * ".CLG", opens it and runs the solver.  argv[0]-style: first arg is argc.
 */
int solverVsdl64(int argc, char **argv);

/* ---- the actual 37-argument solver (vsdlss @ 0x009dad10) ----
 * Contract (annotated from the decompiled body):
 *   p2    : char* jobname
 *   p3    : int   nElems (number of electrical nodes)      -> DAT_00e96564
 *   p5    : int   reorder/mode  (2: keep, 0: single-solve, 4: values-only)
 *   p9    : int   reduceProblemSizeF
 *   p14   : int   (if in {3,4} -> halved;  "dense-ish")
 *   p20,p21: void* (*malloc)(), void* (*free)()           -> appMalloc/appFree
 *   p36   : int   (0 enables dense handling w/ p10)
 * Returns: 0 on success, else error/status code.
 */
typedef uint (VSDLSS)(undefined8 p1, char *p2, int p3, int p4, int p5, int p6,
                      undefined4 p7, undefined8 p8, int p9, int p10, int p11,
                      char p12, undefined8 p13, int p14, int p15, undefined8 p16,
                      undefined8 p17, undefined4 p18, undefined8 p19,
                      undefined8 p20, undefined8 p21, undefined8 p22,
                      undefined8 p23, undefined8 p24, undefined8 p25,
                      undefined8 p26, undefined8 p27, undefined8 p28,
                      int p29, int p30, int p31, int p32, undefined8 p33,
                      undefined8 p34, int p35, int p36, int p37);

#endif
