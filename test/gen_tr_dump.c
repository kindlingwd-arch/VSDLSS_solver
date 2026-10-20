/* Synthetic transient dump in the format of examples/transient_driver, to
 * try bench_tr_replay without a simulator.
 *
 *   ./gen_tr_dump <outdir> [side=200] [chain=3] [steps=8]
 *   env: GEN_PREC (significant digits written, default 17), GEN_DT (default
 *        1e-11), GEN_GROUND (conductance to ground at every node, default 0:
 *        only the pads ground the grid), GEN_CPAIR=k (k two-terminal capacitors, also written to
 *        cpair.txt; default 0), GEN_SEED
 *
 * The circuit: a grid of resistor chains (bench_input's grid_chains), a
 * capacitor C_v to ground at every node (C/dt in A), pads (Norton conductance
 * to a 1.0 V supply) at every 97th grid node, pulse load currents at 30% of
 * the nodes, optionally k capacitors between random node pairs.  Backward
 * Euler, A = G + C/dt (+ pads, + pair capacitors): the simulator's way,
 *   b_{k+1}[v] = (C_v * x_k[v]) / dt + pad + pair terms - I_{k+1}[v],
 * solved with vsdlss_m3_solve.  Written: diag.txt, data.txt, x_vector.txt
 * (DC: capacitors open), <k>b_vector.txt and <k>x_vector.txt (k = 1..steps),
 * cdiag_true.txt (C/dt of every node, for checking; bench_tr_replay reads
 * cdiag.txt only, so rename it to use it), cpair.txt when GEN_CPAIR > 0. */
#define _GNU_SOURCE
#include "../include/vsdlss.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include "bench_input.h"

static unsigned long long rng = 88172645463325252ULL;
static double urand(void) { rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17; return (double)(rng >> 11) / 9007199254740992.0; }
static int prec = 17;
static void write_vec(const char *dir, const char *name, const double *v, csi n)
{
    char p[4096]; snprintf(p, sizeof p, "%s/%s", dir, name);
    FILE *f = fopen(p, "w"); if (!f) { perror(p); exit(1); }
    for (csi i = 0; i < n; i++) fprintf(f, "%lld %.*g\n", (long long)i, prec, v[i]);
    fclose(f);
}

int main(int argc, char **argv)
{
    if (argc < 2) { fprintf(stderr, "usage: %s <outdir> [side] [chain] [steps]\n", argv[0]); return 2; }
    const char *out = argv[1], *e;
    const csi side = argc > 2 ? atol(argv[2]) : 200, chain = argc > 3 ? atol(argv[3]) : 3, steps = argc > 4 ? atol(argv[4]) : 8;
    prec = (e = getenv("GEN_PREC")) ? atoi(e) : 17;
    const double dt = (e = getenv("GEN_DT")) ? atof(e) : 1e-11;
    const csi npair = (e = getenv("GEN_CPAIR")) ? atol(e) : 0;
    const double gground = (e = getenv("GEN_GROUND")) ? atof(e) : 0.0;  /* conductance to ground at every node (grid_chains has 1e-3) */
    rng += (unsigned long long)((e = getenv("GEN_SEED")) ? atol(e) : 0);
    mkdir(out, 0755);
    vsdlss *G = grid_chains(side, chain);              /* includes 1e-3 to ground per node */
    const csi n = G->n;
    double *C = malloc((size_t)n * 8), *gpad = calloc((size_t)n, 8);
    for (csi v = 0; v < n; v++) C[v] = (0.5 + urand()) * 1e-15;          /* 0.5-1.5 fF */
    for (csi v = 0; v < side * side; v += 97) gpad[v] = 4.0;              /* pads */
    csi *pa = malloc((size_t)(npair + 1) * sizeof(csi)), *pb = malloc((size_t)(npair + 1) * sizeof(csi)); double *pc = malloc((size_t)(npair + 1) * 8);
    for (csi q = 0; q < npair; q++) { csi a = (csi)(urand() * (double)n) % n, b; do b = (csi)(urand() * (double)n) % n; while (b == a); pa[q] = a < b ? a : b; pb[q] = a < b ? b : a; pc[q] = (2 + 3 * urand()) * 1e-15; }
    /* A = G + C/dt + pads + pairs (upper CSC: add pair entries as extra column entries, then merge) */
    csi nz = G->p[n] + npair;
    csi *ea = malloc((size_t)nz * sizeof(csi)), *eb = malloc((size_t)nz * sizeof(csi)); double *ex = malloc((size_t)nz * 8); csi m = 0;
    for (csi j = 0; j < n; j++) for (csi p = G->p[j]; p < G->p[j + 1]; p++) {
        double v = G->x[p]; if (G->i[p] == j) v += C[j] / dt + gpad[j] - 1e-3 + gground;
        ea[m] = G->i[p]; eb[m] = j; ex[m++] = v; }
    double *dadd = calloc((size_t)n, 8);
    for (csi q = 0; q < npair; q++) { ea[m] = pa[q]; eb[m] = pb[q]; ex[m++] = -pc[q] / dt; dadd[pa[q]] += pc[q] / dt; dadd[pb[q]] += pc[q] / dt; }
    csi *cnt = calloc((size_t)n + 1, sizeof(csi)); for (csi q = 0; q < m; q++) cnt[eb[q] + 1]++;
    for (csi j = 0; j < n; j++) cnt[j + 1] += cnt[j];
    vsdlss *A = vsdlss_spalloc(n, n, m, 1, 0); csi *cur = malloc((size_t)n * sizeof(csi)); memcpy(cur, cnt, (size_t)n * sizeof(csi));
    for (csi q = 0; q < m; q++) { csi j = eb[q]; A->i[cur[j]] = ea[q]; A->x[cur[j]++] = ex[q] + (ea[q] == j ? dadd[j] : 0); }
    csi w = 0;
    for (csi j = 0; j < n; j++) { csi p0 = cnt[j], p1 = cnt[j + 1];
        for (csi p = p0 + 1; p < p1; p++) for (csi r = p; r > p0 && A->i[r] < A->i[r - 1]; r--) { csi ti = A->i[r]; double tx = A->x[r]; A->i[r] = A->i[r - 1]; A->x[r] = A->x[r - 1]; A->i[r - 1] = ti; A->x[r - 1] = tx; }
        A->p[j] = w;
        for (csi p = p0; p < p1; p++) { if (w > A->p[j] && A->i[w - 1] == A->i[p]) A->x[w - 1] += A->x[p]; else { A->i[w] = A->i[p]; A->x[w] = A->x[p]; w++; } } }
    A->p[n] = w;
    /* diag.txt / data.txt */
    { char p[4096]; snprintf(p, sizeof p, "%s/data.txt", out); FILE *f = fopen(p, "w");
      double *dg = malloc((size_t)n * 8);
      for (csi j = 0; j < n; j++) for (csi q = A->p[j]; q < A->p[j + 1]; q++) { if (A->i[q] == j) dg[j] = A->x[q]; else fprintf(f, "%lld %lld %.*g\n", (long long)A->i[q], (long long)j, prec, A->x[q]); }
      fclose(f); write_vec(out, "diag.txt", dg, n); free(dg); }
    { double *cd = malloc((size_t)n * 8); for (csi v = 0; v < n; v++) cd[v] = C[v] / dt; write_vec(out, "cdiag_true.txt", cd, n); free(cd); }
    if (npair) { char p[4096]; snprintf(p, sizeof p, "%s/cpair.txt", out); FILE *f = fopen(p, "w");
        for (csi q = 0; q < npair; q++) { fprintf(f, "%lld %lld %.17g\n", (long long)pa[q], (long long)pb[q], pc[q] / dt); }
        fclose(f); }
    /* loads: 30% of the nodes, pulses */
    unsigned char *load = calloc((size_t)n, 1); double *amp = malloc((size_t)n * 8), *td = malloc((size_t)n * 8);
    for (csi v = 0; v < n; v++) { load[v] = urand() < 0.3; amp[v] = 1e-4 * (0.2 + urand()); td[v] = (double)(int)(urand() * 4) * dt; }
    vsdlss_m3_factor *F = NULL;
    if (vsdlss_factorize_m3(A, 0, &F) != VSDLSS_OK) { fprintf(stderr, "factorization failed\n"); return 1; }
    /* DC: capacitors open (G + pads only) */
    vsdlss *Gd = vsdlss_spalloc(n, n, G->p[n], 1, 0);
    memcpy(Gd->p, G->p, (size_t)(n + 1) * sizeof(csi)); memcpy(Gd->i, G->i, (size_t)G->p[n] * sizeof(csi)); memcpy(Gd->x, G->x, (size_t)G->p[n] * 8);
    for (csi j = 0; j < n; j++) for (csi q = Gd->p[j]; q < Gd->p[j + 1]; q++) if (Gd->i[q] == j) Gd->x[q] += gpad[j] - 1e-3 + gground;
    vsdlss_m3_factor *Fd = NULL; vsdlss_factorize_m3(Gd, 0, &Fd);
    double *x = malloc((size_t)n * 8), *b = malloc((size_t)n * 8), *y = malloc((size_t)n * 8);
    for (csi v = 0; v < n; v++) b[v] = gpad[v] * 1.0 - (load[v] ? amp[v] * 0.1 : 0);
    vsdlss_m3_solve(Fd, b, x); vsdlss_m3_factor_free(Fd);
    write_vec(out, "x_vector.txt", x, n);
    for (csi k = 1; k <= steps; k++) {
        const double t = (double)k * dt;
        for (csi v = 0; v < n; v++) {
            double bv = (C[v] * x[v]) / dt;                          /* as the simulator: (Cld * x) / dt */
            bv += gpad[v] * 1.0;
            if (load[v]) bv -= t >= td[v] ? amp[v] : amp[v] * 0.1;
            b[v] = bv;
        }
        for (csi q = 0; q < npair; q++) { double ip = (x[pa[q]] - x[pb[q]]) * pc[q] / dt; b[pa[q]] += ip; b[pb[q]] -= ip; }
        vsdlss_m3_solve(F, b, y);
        double *tt = x; x = y; y = tt;
        char nb[64], nx[64]; snprintf(nb, sizeof nb, "%lldb_vector.txt", (long long)k); snprintf(nx, sizeof nx, "%lldx_vector.txt", (long long)k);
        write_vec(out, nb, b, n); write_vec(out, nx, x, n);
    }
    printf("wrote %s: n %lld, %lld steps, dt %g, %d digits, %lld pads, %lld pair capacitors\n",
           out, (long long)n, (long long)steps, dt, prec, (long long)((side * side + 96) / 97), (long long)npair);
    vsdlss_m3_factor_free(F);
    return 0;
}
