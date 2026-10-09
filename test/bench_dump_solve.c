/* Factor and solve a PG_DUMP system (int64 upper CSC: n, nnz, p, i, x, b)
 * through the public API only, so any library version can be timed on the
 * same matrix.
 *   ./bench_dump_solve file order threads [reps]
 * Prints factor time, first and median warm original-order solve time, the
 * backward error and a hash of the solution bits. */
#define _POSIX_C_SOURCE 200809L
#include "vsdlss.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static int cmpd(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }
static long peak_rss_mb(void)
{
    FILE *f = fopen("/proc/self/status", "r"); char l[256]; long kb = -1;
    if (!f) return -1;
    while (fgets(l, sizeof l, f)) if (!strncmp(l, "VmHWM:", 6)) { kb = atol(l + 6); break; }
    fclose(f); return kb / 1024;
}

int main(int argc, char **argv)
{
    if (argc < 4) { fprintf(stderr, "usage: bench_dump_solve file order threads [reps]\n"); return 1; }
    int order = atoi(argv[2]), threads = atoi(argv[3]), reps = argc > 4 ? atoi(argv[4]) : 9;
    if (reps < 1 || reps > 64) reps = 9;
    FILE *fp = fopen(argv[1], "rb"); if (!fp) { perror("open"); return 1; }
    int64_t n, nz;
    if (fread(&n, 8, 1, fp) != 1 || fread(&nz, 8, 1, fp) != 1) return 1;
    vsdlss *A = vsdlss_spalloc(n, n, nz, 1, 0);
    double *b = malloc((size_t)n * 8), *x = malloc((size_t)n * 8);
    if (!A || !b || !x) { puts("alloc failed"); return 1; }
    if (fread(A->p, 8, (size_t)n + 1, fp) != (size_t)n + 1 || fread(A->i, 8, (size_t)nz, fp) != (size_t)nz ||
        fread(A->x, 8, (size_t)nz, fp) != (size_t)nz || fread(b, 8, (size_t)n, fp) != (size_t)n) { puts("read failed"); return 1; }
    fclose(fp);
    if (vsdlss_set_num_threads(threads) != VSDLSS_OK) { puts("bad thread count"); return 1; }
    vsdlss_m3_factor *f = NULL;
    double t0 = now();
    vsdlss_status st = vsdlss_factorize_m3(A, order, &f);
    double tf = now() - t0;
    if (st != VSDLSS_OK) { printf("factor failed: %s\n", vsdlss_status_string(st)); return 1; }
    t0 = now(); st = vsdlss_m3_solve(f, b, x); double tfirst = now() - t0;
    if (st != VSDLSS_OK) { puts("solve failed"); return 1; }
    double ts[64];
    for (int r = 0; r < reps; r++) { t0 = now(); vsdlss_m3_solve(f, b, x); ts[r] = now() - t0; }
    qsort(ts, (size_t)reps, 8, cmpd);
    double eta = -1; vsdlss_backward_error(A, x, b, &eta);
    unsigned long long h = 1469598103934665603ULL;
    for (int64_t k = 0; k < n; k++) { unsigned long long u; memcpy(&u, x + k, 8); h = (h ^ u) * 1099511628211ULL; }
    { const char *xp = getenv("XDUMP"); if (xp) { FILE *xf = fopen(xp, "wb"); if (!xf || fwrite(x, 8, (size_t)n, xf) != (size_t)n) { puts("xdump failed"); return 1; } fclose(xf); } }
    printf("OURS order=%d threads=%d | factor %.2f s | solve first %.4f s, warm median %.4f s (min %.4f) | berr %.1e | peak %ld MB | hash %016llx\n",
           order, threads, tf, tfirst, ts[reps / 2], ts[0], eta, peak_rss_mb(), h);
    return 0;
}
