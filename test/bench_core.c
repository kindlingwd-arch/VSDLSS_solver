/* Core-stage micro-benchmark: the supernodal analysis, numeric factorization
 * and core solve of one ordered core matrix, without the components,
 * low-degree reduction and ordering that precede them (METIS alone is most
 * of an M3 factorization).
 *
 *   ./bench_core dump.bin order out_prefix
 *       factors a PG_DUMP system once and writes each component's ordered
 *       core (upper CSC, postordered) to out_prefix.<component>.core
 *   ./bench_core file.core "cfg1;cfg2;..." threads_list [reps]
 *       builds one supernodal factor per configuration, then times the core
 *       solve (vsdlss_sn_solve_inplace) of all configurations alternately in
 *       one process: median per configuration and the paired median ratio
 *       to the first.  A configuration is a list of NAME=value environment
 *       settings separated by spaces (for example
 *       "VSDLSS_SN_RELAX=0,0,0,0,0,0"); "-" is the default.
 *   build: make bench_core METIS=1
 *
 * Every configuration's solution is compared with the first (largest
 * relative difference) and bitwise across thread counts. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static int cmpd(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }

static vsdlss *read_csc(const char *path)
{
    FILE *fp = fopen(path, "rb"); int64_t n, nz;
    if (!fp || fread(&n, 8, 1, fp) != 1 || fread(&nz, 8, 1, fp) != 1) { if (fp) fclose(fp); return NULL; }
    vsdlss *A = vsdlss_spalloc(n, n, nz, 1, 0);
    if (!A || fread(A->p, 8, (size_t)n + 1, fp) != (size_t)n + 1 || fread(A->i, 8, (size_t)nz, fp) != (size_t)nz ||
        fread(A->x, 8, (size_t)nz, fp) != (size_t)nz) { fclose(fp); return NULL; }
    fclose(fp);
    return A;
}

/* Same layout as PG_DUMP (b = 1, unused here) so other tools can read it. */
static const char *prefix;
static void write_core(csi component, const vsdlss *C)
{
    char path[1024]; snprintf(path, sizeof path, "%s.%lld.core", prefix, (long long)component);
    FILE *fp = fopen(path, "wb"); int64_t n = C->n, nz = C->p[C->n]; int ok = fp != NULL;
    if (ok) ok = fwrite(&n, 8, 1, fp) == 1 && fwrite(&nz, 8, 1, fp) == 1 &&
                 fwrite(C->p, 8, (size_t)n + 1, fp) == (size_t)n + 1 && fwrite(C->i, 8, (size_t)nz, fp) == (size_t)nz &&
                 fwrite(C->x, 8, (size_t)nz, fp) == (size_t)nz;
    for (int64_t k = 0; ok && k < n; k++) { double one = 1; ok = fwrite(&one, 8, 1, fp) == 1; }
    if (fp) fclose(fp);
    printf("%s %s: n %lld, nnz %lld\n", ok ? "wrote" : "FAILED", path, (long long)n, (long long)nz);
}

static void apply_env(const char *cfg)
{
    char buf[512]; snprintf(buf, sizeof buf, "%s", cfg);
    for (char *t = strtok(buf, " "); t; t = strtok(NULL, " ")) {
        char *eq = strchr(t, '=');
        if (!eq) continue;
        *eq = 0; setenv(t, eq + 1, 1);
    }
}
static void clear_env(const char *cfg)
{
    char buf[512]; snprintf(buf, sizeof buf, "%s", cfg);
    for (char *t = strtok(buf, " "); t; t = strtok(NULL, " ")) {
        char *eq = strchr(t, '=');
        if (eq) { *eq = 0; unsetenv(t); }
    }
}

#define MAXC 8
#define MAXR 64
int main(int argc, char **argv)
{
    if (argc < 4) { fprintf(stderr, "usage: see the comment at the top of test/bench_core.c\n"); return 1; }
    if (!strstr(argv[1], ".core")) {                   /* dump mode */
        vsdlss *A = read_csc(argv[1]);
        if (!A) { puts("input failed"); return 1; }
        prefix = argv[3]; vsdlss_m3_core_hook = write_core;
        vsdlss_m3_factor *F = NULL;
        if (vsdlss_factorize_m3(A, atoi(argv[2]), &F) != VSDLSS_OK) { puts("factor failed"); return 1; }
        vsdlss_m3_factor_free(F); vsdlss_spfree(A);
        return 0;
    }
    vsdlss *C = read_csc(argv[1]);
    if (!C) { puts("input failed"); return 1; }
    const csi n = C->n;
    char cbuf[2048]; snprintf(cbuf, sizeof cbuf, "%s", argv[2]);
    char *cfg[MAXC]; int nc = 0;
    for (char *t = strtok(cbuf, ";"); t && nc < MAXC; t = strtok(NULL, ";")) cfg[nc++] = t;
    int threads[16], nth = 0; char tb[256]; snprintf(tb, sizeof tb, "%s", argv[3]);
    for (char *t = strtok(tb, " ,"); t && nth < 16; t = strtok(NULL, " ,")) threads[nth++] = atoi(t);
    const int reps = argc > 4 ? (atoi(argv[4]) < MAXR ? atoi(argv[4]) : MAXR) : 21;
    int tmax = 1; for (int i = 0; i < nth; i++) if (threads[i] > tmax) tmax = threads[i];
    vsdlss_set_num_threads(tmax);

    vsdlss_sn_factor *f[MAXC] = {0};
    double *x0 = malloc((size_t)n * 8), *x[MAXC], *ref[MAXC];
    for (csi i = 0; i < n; i++) x0[i] = sin(0.37 * (double)i) + 0.2;
    printf("# %s: core n %lld, nnz(A) %lld\n", argv[1], (long long)n, (long long)C->p[n]);
    for (int c = 0; c < nc; c++) {
        vsdlss_sn_symbolic *s = NULL;
        apply_env(cfg[c]);
        double t0 = now();
        if (vsdlss_sn_analyze_relaxed(C, &s) != VSDLSS_OK) { printf("analysis failed: %s\n", cfg[c]); return 1; }
        double ta = now() - t0; t0 = now();
        const csi l_nnz = s->l_nnz, zeros = s->relaxed_zeros, count = s->count;
        if (vsdlss_sn_factorize(C, s, &f[c]) != VSDLSS_OK) { printf("factor failed: %s\n", cfg[c]); return 1; }
        double tf = now() - t0;
        vsdlss_sn_symbolic_free(s);
        clear_env(cfg[c]);
        x[c] = malloc((size_t)n * 8); ref[c] = malloc((size_t)n * 8);
        printf("# [%d] %-40s L %lld (%.1f%% zeros), supernodes %lld, stored %.0f MB | analysis %.2f s, numeric %.2f s\n",
               c, cfg[c], (long long)l_nnz, 100.0 * (double)zeros / (double)(l_nnz ? l_nnz : 1), (long long)count,
               (double)f[c]->panel_offset[f[c]->count] * 8 / 1e6, ta, tf);
    }
    int mismatch = 0;
    for (int ti = 0; ti < nth; ti++) {
        vsdlss_set_num_threads(threads[ti]);
        static double ts[MAXC][MAXR];
        for (int k = -1; k < reps; k++)
            for (int cc = 0; cc < nc; cc++) {
                int c = (k & 1) ? nc - 1 - cc : cc;          /* alternate the order every round */
                memcpy(x[c], x0, (size_t)n * 8);
                double s = now(); vsdlss_sn_solve_inplace(f[c], x[c]);
                if (k >= 0) ts[c][k] = now() - s;
            }
        printf("T=%-2d", threads[ti]);
        double med0 = 0;
        for (int c = 0; c < nc; c++) {
            double r[MAXR], sorted[MAXR];
            for (int k = 0; k < reps; k++) { r[k] = ts[c][k] / ts[0][k]; sorted[k] = ts[c][k]; }
            qsort(sorted, (size_t)reps, 8, cmpd); qsort(r, (size_t)reps, 8, cmpd);
            if (c == 0) med0 = sorted[reps / 2];
            printf(" | [%d] %7.2f ms", c, 1e3 * sorted[reps / 2]);
            if (c) printf(" (x%.3f, q1 %.3f q3 %.3f)", r[reps / 2], r[reps / 4], r[3 * reps / 4]);
            if (ti == 0) memcpy(ref[c], x[c], (size_t)n * 8); else mismatch |= memcmp(ref[c], x[c], (size_t)n * 8) != 0;
        }
        (void)med0;
        printf("\n");
    }
    for (int c = 1; c < nc; c++) {
        double d = 0, m = 0;
        for (csi i = 0; i < n; i++) { double e = fabs(ref[c][i] - ref[0][i]); if (e > d) d = e; if (fabs(ref[0][i]) > m) m = fabs(ref[0][i]); }
        printf("# [%d] vs [0]: max |dx| / max |x| = %.1e%s\n", c, d / (m > 0 ? m : 1), d == 0 ? " (bitwise identical)" : "");
    }
    printf("# results across thread counts: %s\n", mismatch ? "DIFFERENT" : "bitwise identical");
    for (int c = 0; c < nc; c++) { vsdlss_sn_factor_free(f[c]); free(x[c]); free(ref[c]); }
    free(x0); vsdlss_spfree(C);
    return mismatch;
}
