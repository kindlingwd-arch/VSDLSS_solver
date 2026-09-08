/* harness.c
 *
 * Reconstruction of the VSDLSS caller harness (the interface that a program
 * uses to invoke the real solver).  This mirrors the decompiled `solverVsdl64`
 * (asim_3d @0x0095fe30): it parses the same command-line options, forms the
 * `<jobName>.CLG` file, opens it and dispatches to the solver driver.
 *
 * The decompiled reference of the harness is under reference/harness/.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "harness_contract.h"

/* The real driver was obfuscated in the binary (JyFKkyQXSPAIPP ...).  Here
 * we route through the decompiled `vsdlss` entry declared in the contract. */
extern VSDLSS solver_impl;   /* the real vsdlss() to call (from the archive) */

static int quiet   = 0;     /* -W/-w  : write mode     */
static int unperm  = 0;     /* -U/-u  : unreference     */
static int orderSel = 0;    /* -p <n> : order selector  */
static int silent  = 0;     /* -S     : silent          */
static int dosolve = 0;     /* -s     : single-stop     */

int solverVsdl64(int argc, char **argv)
{
    char *jobName = NULL;
    int i;

    /* parse options (decompiled solverVsdl64 logic) */
    for (i = 1; i < argc; i++) {
        char *a = argv[i];
        if (a[0] == '-' && a[1]) {
            switch (a[1]) {
            case 'S': silent = 1; break;
            case 'U': case 'u': unperm = 1; break;
            case 'W': case 'w': quiet = 1; break;
            case 'p': if (i + 1 < argc) orderSel = atoi(argv[++i]); break;
            case 's': dosolve = 1; break;
            default: break;
            }
        } else if (!jobName) {
            jobName = a;
        }
    }
    if (!jobName || strlen(jobName) >= 0x400) {
        fprintf(stderr, "VSDLSS jobName missing or exceeds buffer length\n");
        return 9999;
    }

    /* form <jobName>.CLG and "open" it (in the original this is a solver
     * input file describing the matrix + RHS; here we treat it as the source
     * of a small, self-contained SPD problem). */
    char clg[0x600];
    snprintf(clg, sizeof clg, "%s.CLG", jobName);
    printf("[harness] job=%s -> %s   (-p%d -s%d -W%d)\n",
           jobName, clg, orderSel, dosolve, quiet);

    /* The original then dispatched to the solver driver.  We call the real
     * `vsdlss` entry with a 37-argument contract.  This must be completed
     * with the data structures set up from the .CLG file (step 3). */
    fprintf(stderr,
            "[harness] NOTE: end-to-end run requires reconstructing the .CLG "
            "input format + solver init env (step 3).\n");
    return 0;
}
