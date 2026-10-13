/* Performance history: the same harness for every library version, each
 * built as a shared object from its commit and loaded with dlopen (see
 * docs/reconstruction/performance-history-20261013.md):
 *   ./bench_history lib.so dump.bin order threads [reps=7]
 * Loads a PG_DUMP system, factors it with vsdlss_factorize_m3, then one cold
 * and `reps` warm original-order solves (vsdlss_m3_solve).  Prints factor
 * time, cold / median warm solve, backward error (when the library has it),
 * the largest relative difference to a reference solution (REF=path: written
 * by the first run, compared by the later ones) and peak RSS. */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/resource.h>

typedef int64_t csi;
typedef struct { csi nzmax, m, n; csi *p, *i; double *x; csi nz; } vsdlss;
static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static int cmpd(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }
static void *sym(void *h, const char *s, int need) { void *p = dlsym(h, s); if (!p && need) { fprintf(stderr, "missing %s\n", s); exit(1); } return p; }

int main(int argc, char **argv)
{
    if (argc < 5) { fprintf(stderr, "usage: hist_bench lib.so dump.bin order threads [reps]\n"); return 1; }
    const int order = atoi(argv[3]), threads = atoi(argv[4]), reps = argc > 5 ? atoi(argv[5]) : 7;
    void *h = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
    if (!h) { fprintf(stderr, "dlopen: %s\n", dlerror()); return 1; }
    vsdlss *(*spalloc)(csi, csi, csi, csi, csi) = sym(h, "vsdlss_spalloc", 1);
    int (*set_threads)(int) = sym(h, "vsdlss_set_num_threads", 0);
    int (*factor)(const vsdlss *, int, void **) = sym(h, "vsdlss_factorize_m3", 1);
    int (*solve)(const void *, const double *, double *) = sym(h, "vsdlss_m3_solve", 1);
    int (*berr)(const vsdlss *, const double *, const double *, double *) = sym(h, "vsdlss_backward_error", 0);
    FILE *f = fopen(argv[2], "rb"); int64_t n, nz;
    if (!f || fread(&n, 8, 1, f) != 1 || fread(&nz, 8, 1, f) != 1) { fprintf(stderr, "cannot read %s\n", argv[2]); return 1; }
    vsdlss *A = spalloc(n, n, nz, 1, 0);
    double *b = malloc((size_t)n * 8), *x = malloc((size_t)n * 8);
    if (!A || !b || !x) return 1;
    if (fread(A->p, 8, (size_t)n + 1, f) != (size_t)n + 1 || fread(A->i, 8, (size_t)nz, f) != (size_t)nz ||
        fread(A->x, 8, (size_t)nz, f) != (size_t)nz || fread(b, 8, (size_t)n, f) != (size_t)n) { fprintf(stderr, "short dump\n"); return 1; }
    fclose(f);
    if (set_threads) set_threads(threads);
    void *F = NULL; double t0 = now();
    int st = factor(A, order, &F);
    const double tf = now() - t0;
    if (st != 0) { printf("factor failed: status %d\n", st); return 1; }
    t0 = now(); st = solve(F, b, x); const double tc = now() - t0;
    if (st != 0) { printf("solve failed: status %d\n", st); return 1; }
    double *ts = malloc((size_t)reps * sizeof(double));
    for (int k = 0; k < reps; k++) { t0 = now(); solve(F, b, x); ts[k] = now() - t0; }
    qsort(ts, (size_t)reps, sizeof(double), cmpd);
    double eta = -1; if (berr) berr(A, x, b, &eta);
    double diff = -1; const char *ref = getenv("REF");
    if (ref) {
        FILE *r = fopen(ref, "rb");
        if (!r) { r = fopen(ref, "wb"); if (r) { fwrite(x, 8, (size_t)n, r); fclose(r); } diff = 0; }
        else {
            double *y = malloc((size_t)n * 8), num = 0, den = 0;
            if (y && fread(y, 8, (size_t)n, r) == (size_t)n)
                for (csi i = 0; i < n; i++) { const double d = fabs(x[i] - y[i]); if (d > num) num = d; if (fabs(y[i]) > den) den = fabs(y[i]); }
            fclose(r); free(y); diff = den > 0 ? num / den : num;
        }
    }
    struct rusage ru; getrusage(RUSAGE_SELF, &ru);
    printf("n %lld | order %d threads %d | factor %.2f s | solve cold %.1f ms, warm median %.1f ms (min %.1f, max %.1f) | "
           "berr %.1e | max rel diff to ref %.1e | peak RSS %.2f GB\n",
           (long long)n, order, threads, tf, 1e3 * tc, 1e3 * ts[reps / 2], 1e3 * ts[0], 1e3 * ts[reps - 1],
           eta, diff, ru.ru_maxrss / 1048576.0);
    return 0;
}
