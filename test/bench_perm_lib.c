/* Library gather / write-back at full size, without a factorization.
 *
 *   ./bench_perm_lib [nodes=32000000] [threads=4] [repeats=8] [small=100]
 *
 * Builds a component layout like the power grids (two nets splitting the
 * vertices 52/48 plus `small` components of 1..300 vertices, global ids a
 * random bijection), the factor's inverse map with the library's own
 * build_inverse, and runs the library's gather / write-back code on it:
 * the direct loops (per-component gather as in solve_local, and
 * write_back_inverse) and the two-pass path (perm2_gather /
 * perm2_writeback with the plan and scratch handling of solve_common).
 * Every run is checked bitwise: packed[p] == rhs[vertices[p]] after the
 * gather, out == rhs after the write-back.
 *
 * The file includes src/vsdlss_m3.c, so it compiles against either tree:
 * the current one (block size chosen per VSDLSS_PERM_SH: unset = L2 rule,
 * 8..16, auto) or an older checkout (fixed 2^15 blocks), for A/B:
 *
 *   git worktree add /tmp/old c3fffd0
 *   make -B bench_perm_lib PERM_TREE=/tmp/old && mv bench_perm_lib bench_perm_lib_old
 *   make -B bench_perm_lib
 *   ./bench_perm_lib_old 32000000 8 8; ./bench_perm_lib 32000000 8 8
 */
#include "vsdlss_m3.c"

#include <stdio.h>

static uint64_t rs = 0x9e3779b97f4a7c15ULL;
static uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }

static double wall(void)
{
#ifdef _OPENMP
    return omp_get_wtime();
#else
    struct timespec ts; timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
#endif
}

static double median(double *v, int n)
{
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && v[j] < v[j - 1]; j--) { double x = v[j]; v[j] = v[j - 1]; v[j - 1] = x; }
    return n & 1 ? v[n / 2] : 0.5 * (v[n / 2 - 1] + v[n / 2]);
}

/* Library direct gather: solve_local's loop, component by component. */
static void direct_gather(const vsdlss_m3_factor *f, const double *rhs, double *const *loc)
{
    for (csi c = 0; c < f->count; c++) {
        const csi cn = f->component[c].n, *map = component_map(f, c);
        double *local = loc[c]; int bad = 0;
        int gt = vsdlss_parallel_width((double)cn * 4); (void)gt;
        VSDLSS_OMP(omp parallel for num_threads(gt) if(gt>1) schedule(static) reduction(|:bad))
        for (csi i = 0; i < cn; i++) { double v = rhs[map[i]]; local[i] = v; bad |= !isfinite(v); }
        if (bad) abort();
    }
}

/* One two-pass gather + write-back, as solve_common does it. */
static int two_pass(vsdlss_m3_factor *f, const double *rhs, double *const *loc, double *out,
                    double *tg, double *tw, int *shift)
{
    const vsdlss_perm_plan *pl; double *pbuf; csi *pq; int T = vsdlss_parallel_width((double)f->n * 4);
#ifdef PERM_SH_DEFAULT                 /* current tree: block size chosen per plan */
    int slot = -1;
    if (!pbuf_lock(f)) return 0;
    pl = perm_plan_pick(f, T, &slot);
    pq = pl ? (csi *)malloc((size_t)pl->T * (size_t)pl->B * sizeof(csi)) : NULL;
    pbuf = pl && pq ? pbuf_ensure(f, perm_scratch_len(pl)) : NULL;
    *shift = pl ? pl->sh : 0;
#else                                  /* older tree: fixed 2^15 */
    pl = perm_plan_get(f, T);
    pq = pl ? (csi *)malloc((size_t)pl->T * (size_t)pl->B * sizeof(csi)) : NULL;
    pbuf = pl && pq ? pbuf_acquire(f) : NULL;
    *shift = 15;
#endif
    if (!pbuf) { free(pq); return 0; }
    double t0 = wall();
    if (perm2_gather(f, pl, rhs, pbuf, loc, pq)) abort();
    double t1 = wall();
    perm2_writeback(f, pl, pbuf, loc, out, pq);
    double t2 = wall();
    *tg = t1 - t0; *tw = t2 - t1;
#ifdef PERM_SH_DEFAULT
    if (slot >= 0) perm_tune_record(f, T, slot, t2 - t0);
#endif
    pbuf_release(f);
    free(pq);
    return 1;
}

static int check_gather(const vsdlss_m3_factor *f, const double *rhs, const double *packed)
{
    const csi *v = f->components->vertices; int bad = 0;
    VSDLSS_OMP(omp parallel for reduction(|:bad))
    for (csi p = 0; p < f->n; p++) bad |= memcmp(&packed[p], &rhs[v[p]], 8) != 0;
    return !bad;
}

int main(int argc, char **argv)
{
    const csi n = argc > 1 ? (csi)strtoll(argv[1], NULL, 10) : 32000000;
    const int T = argc > 2 ? atoi(argv[2]) : 4, R = argc > 3 ? atoi(argv[3]) : 8;
    const csi small = argc > 4 ? (csi)strtoll(argv[4], NULL, 10) : 100;
    if (n < 1000 || T < 1 || R < 1 || R > 64 || small < 0) return 2;
    if (vsdlss_set_num_threads(T) != VSDLSS_OK) return 2;

    /* Layout: small components first-sized, the rest split 52/48. */
    const csi count = small + 2;
    csi *off = (csi *)malloc((size_t)(count + 1) * sizeof(csi)), rest = n;
    csi *sz = (csi *)malloc((size_t)count * sizeof(csi));
    for (csi c = 2; c < count; c++) { sz[c] = 1 + (csi)(rnd() % 300); rest -= sz[c]; }
    if (rest < 2) return 2;
    sz[0] = (csi)(0.52 * (double)rest); sz[1] = rest - sz[0];
    off[0] = 0; for (csi c = 0; c < count; c++) off[c + 1] = off[c] + sz[c];
    csi *vert = (csi *)vsdlss_big_malloc((size_t)n * sizeof(csi));
    if (!off || !sz || !vert) return 1;
    for (csi i = 0; i < n; i++) vert[i] = i;
    for (csi i = n - 1; i > 0; i--) { csi j = (csi)(rnd() % (uint64_t)(i + 1)), x = vert[i]; vert[i] = vert[j]; vert[j] = x; }

    vsdlss_components comp = { .n = n, .count = count, .offset = off, .vertices = vert };
    vsdlss_m3_factor *f = (vsdlss_m3_factor *)calloc(1, sizeof(*f));
    if (!f) return 1;
    f->n = n; f->count = count; f->components = &comp;
    f->component = (vsdlss_m3_component_factor *)calloc((size_t)count, sizeof(*f->component));
    if (!f->component) return 1;
    for (csi c = 0; c < count; c++) f->component[c].n = sz[c];
    if (build_inverse(f) != VSDLSS_OK || (!f->inv32 && !f->inv64)) { fprintf(stderr, "inverse failed\n"); return 1; }

    double *rhs = (double *)vsdlss_big_malloc((size_t)n * 8), *packed = (double *)vsdlss_big_malloc((size_t)n * 8);
    double *out = (double *)vsdlss_big_malloc((size_t)n * 8);
    double **loc = (double **)malloc((size_t)count * sizeof(*loc));
    if (!rhs || !packed || !out || !loc) return 1;
    for (csi c = 0; c < count; c++) loc[c] = packed + off[c];   /* 8-byte aligned in general */
    for (csi g = 0; g < n; g++) rhs[g] = (double)g + 0.125;
    memset(packed, 0, (size_t)n * 8); memset(out, 0, (size_t)n * 8);

    solve_ctx ctx = { f, rhs, out, NULL, 0, loc, 0 };
    double dg[64], dw[64], pg[64], pw[64]; int sh = 0;

    /* Warm-up (and, with VSDLSS_PERM_SH=auto, the block-size measurement). */
    for (int r = 0; r < 8; r++) {
        double a, b;
        if (!two_pass(f, rhs, loc, out, &a, &b, &sh)) { fprintf(stderr, "two-pass unavailable\n"); return 1; }
    }
    for (int r = 0; r < R; r++) {
        memset(packed, 0, 64); out[0] = -1;
        { double s = wall(); direct_gather(f, rhs, loc); dg[r] = wall() - s; }
        if (!check_gather(f, rhs, packed)) { fprintf(stderr, "direct gather mismatch\n"); return 1; }
        { double s = wall(); write_back_inverse(&ctx); dw[r] = wall() - s; }
        if (memcmp(out, rhs, (size_t)n * 8)) { fprintf(stderr, "direct write-back mismatch\n"); return 1; }
        memset(packed, 0, 64); out[0] = -1;
        if (!two_pass(f, rhs, loc, out, &pg[r], &pw[r], &sh)) return 1;
        if (!check_gather(f, rhs, packed)) { fprintf(stderr, "two-pass gather mismatch\n"); return 1; }
        if (memcmp(out, rhs, (size_t)n * 8)) { fprintf(stderr, "two-pass write-back mismatch\n"); return 1; }
    }
    const char *mode = getenv("VSDLSS_PERM_SH");
    printf("n=%lld components=%lld (largest %lld) threads=%d repeats=%d inverse=%s block=2^%d (VSDLSS_PERM_SH=%s, L2=%ld)\n",
           (long long)n, (long long)count, (long long)sz[0], T, R, f->inv32 ? "32" : "64", sh,
           mode ? mode : "unset",
#ifdef PERM_SH_DEFAULT
           vsdlss_l2_cache_bytes()
#else
           0L
#endif
           );
    double a = median(dg, R) * 1e3, b = median(dw, R) * 1e3, c = median(pg, R) * 1e3, d = median(pw, R) * 1e3;
    printf("direct    gather %8.2f ms  write-back %8.2f ms  sum %8.2f ms\n", a, b, a + b);
    printf("two-pass  gather %8.2f ms  write-back %8.2f ms  sum %8.2f ms  checked=bitwise\n", c, d, c + d);
    return 0;
}
