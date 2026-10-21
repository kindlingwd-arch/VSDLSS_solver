/* Memory bandwidth of the machine (STREAM-like), the ceiling the solve
 * phases are compared with in tools/perf_profile.sh.
 *
 *   ./bench_stream "threads list" [MiB per array = 1024] [reps = 10]
 *
 * For each thread count: three arrays of the given size, first touched by the
 * threads that use them (static schedule, so each thread's pages sit on its
 * NUMA node when the threads are bound, OMP_PROC_BIND=spread), then the best
 * of reps of
 *   read    s += a[i]                 (8 bytes per element)
 *   copy    c[i] = a[i]               (16; write-allocate traffic not counted)
 *   triad   a[i] = b[i] + q c[i]      (24; idem)
 * in GB/s (1e9 bytes). */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }

int main(int argc, char **argv)
{
    if (argc < 2) { fprintf(stderr, "usage: bench_stream \"threads list\" [MiB per array] [reps]\n"); return 1; }
    const size_t mib = argc > 2 ? (size_t)atol(argv[2]) : 1024;
    const int reps = argc > 3 ? atoi(argv[3]) : 10;
    const size_t n = mib * 1024 * 1024 / sizeof(double);
    char buf[256]; strncpy(buf, argv[1], 255); buf[255] = 0;
    printf("# bench_stream: %zu MiB per array (%zu doubles), best of %d\n", mib, n, reps);
    double sink = 0;
    for (char *t = strtok(buf, " ,"); t; t = strtok(NULL, " ,")) {
        const int T = atoi(t);
        if (T < 1) continue;
        double *a = malloc(n * sizeof(double)), *b = malloc(n * sizeof(double)), *c = malloc(n * sizeof(double));
        if (!a || !b || !c) { puts("out of memory"); return 1; }
        #pragma omp parallel for num_threads(T) schedule(static)
        for (size_t i = 0; i < n; i++) { a[i] = 1.0 + (double)(i & 7); b[i] = 2.0; c[i] = 0.5; }
        double best[3] = {1e300, 1e300, 1e300};
        for (int r = 0; r < reps; r++) {
            /* eight independent sums: one running sum is a chain of dependent adds
             * (about 4 cycles each), which caps the read loop below the memory */
            double s = 0, t0 = now();
            #pragma omp parallel for num_threads(T) schedule(static) reduction(+:s)
            for (size_t i = 0; i < n / 8 * 8; i += 8)
                s += ((a[i] + a[i + 1]) + (a[i + 2] + a[i + 3])) + ((a[i + 4] + a[i + 5]) + (a[i + 6] + a[i + 7]));
            double t1 = now();
            sink += s;
            #pragma omp parallel for num_threads(T) schedule(static)
            for (size_t i = 0; i < n; i++) c[i] = a[i];
            double t2 = now();
            #pragma omp parallel for num_threads(T) schedule(static)
            for (size_t i = 0; i < n; i++) a[i] = b[i] + 0.5 * c[i];
            double t3 = now();
            if (t1 - t0 < best[0]) best[0] = t1 - t0;
            if (t2 - t1 < best[1]) best[1] = t2 - t1;
            if (t3 - t2 < best[2]) best[2] = t3 - t2;
        }
        const double B = (double)n * sizeof(double);
        printf("T=%-3d read %6.1f GB/s | copy %6.1f GB/s | triad %6.1f GB/s\n", T,
               B / best[0] / 1e9, 2 * B / best[1] / 1e9, 3 * B / best[2] / 1e9);
        fflush(stdout);
        free(a); free(b); free(c);
    }
    return sink == 12345.678 ? 2 : 0;   /* keep the read loop */
}
