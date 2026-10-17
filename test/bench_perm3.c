/* Gather/write-back variants on a random bijection; extends bench_perm2_only.
 * ./bench_perm3 [nodes=32000000] [threads=4] [repeats=8] [hugepages=1]
 * hugepages=1 applies MADV_HUGEPAGE as vsdlss_big_malloc does in the library.
 *
 *  direct : packed[i]=global[p2g[i]] / global[g]=packed[g2p[g]]
 *  v0     : current two-pass (pass 1 derives the bucket from g2p/p2g)
 *  v1     : v0, pass 1 reads a precomputed uint16 bucket id (2 B, no shift)
 *  v2     : v1 + pass 1 software write-combining, 64 B non-temporal flush
 *  v3     : v2 + pass 2 fills an L2 staging block, streams it out (no RFO)
 * Every variant is compared bitwise with the direct result.
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/mman.h>
#include <emmintrin.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef SHIFT
#define SHIFT 15
#endif
#define BLK (1u << SHIFT)
#define MASK (BLK - 1u)
#define WC 8                       /* doubles per 64-byte write-combining line */

static int use_huge = 1;
static void *big(size_t bytes)
{
    void *p = NULL;
    size_t a = (size_t)2 << 20, r = (bytes + a - 1) / a * a;
    if (posix_memalign(&p, a, r ? r : a)) return NULL;
    if (use_huge) madvise(p, r, MADV_HUGEPAGE);
    return p;
}
static double now(void)
{
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
}
static uint64_t rs = 0x9e3779b97f4a7c15ULL;
static uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }

typedef struct {
    uint32_t n, B; int T; uint32_t chunk;
    uint32_t *p2g, *g2p;
    /* dense layout (v0, v1) */
    uint64_t *posG, *posW, *startG, *startW;
    uint16_t *offG, *offW;
    /* bucket id of each source element, in source order (v1..v3) */
    uint16_t *bktG, *bktW;
    /* padded layout (v2, v3): (t,b) segment starts 64 B aligned */
    uint64_t *pposG, *pposW; uint32_t *cntG, *cntW;
    uint16_t *poffG, *poffW; size_t padded;
} plan;

static void range(const plan *p, int t, uint32_t *lo, uint32_t *hi)
{
    uint64_t l = (uint64_t)t * p->chunk, h = l + p->chunk;
    *lo = l < p->n ? (uint32_t)l : p->n; *hi = h < p->n ? (uint32_t)h : p->n;
}

static int build(plan *p)
{
    const uint32_t n = p->n, B = p->B; const int T = p->T; const size_t S = (size_t)B * T;
    p->chunk = (n + T - 1) / T;
    p->p2g = big((size_t)n * 4); p->g2p = big((size_t)n * 4);
    p->offG = big((size_t)n * 2); p->offW = big((size_t)n * 2);
    p->bktG = big((size_t)n * 2); p->bktW = big((size_t)n * 2);
    p->posG = calloc(S, 8); p->posW = calloc(S, 8);
    p->startG = malloc((B + 1) * 8); p->startW = malloc((B + 1) * 8);
    p->cntG = calloc(S, 4); p->cntW = calloc(S, 4);
    p->pposG = malloc(S * 8); p->pposW = malloc(S * 8);
    p->padded = (size_t)n + S * WC;
    p->poffG = big(p->padded * 2); p->poffW = big(p->padded * 2);
    if (!p->p2g || !p->g2p || !p->offG || !p->offW || !p->bktG || !p->bktW || !p->posG ||
        !p->posW || !p->startG || !p->startW || !p->cntG || !p->cntW || !p->pposG ||
        !p->pposW || !p->poffG || !p->poffW) return 0;
    for (uint32_t i = 0; i < n; i++) p->p2g[i] = i;
    for (uint32_t i = n - 1; i > 0; i--) {
        uint32_t j = (uint32_t)(rnd() % ((uint64_t)i + 1)), x = p->p2g[i];
        p->p2g[i] = p->p2g[j]; p->p2g[j] = x;
    }
    for (uint32_t i = 0; i < n; i++) p->g2p[p->p2g[i]] = i;
    for (uint32_t i = 0; i < n; i++) { p->bktG[i] = (uint16_t)(p->g2p[i] >> SHIFT); p->bktW[i] = (uint16_t)(p->p2g[i] >> SHIFT); }
    for (int t = 0; t < T; t++) {
        uint32_t lo, hi; range(p, t, &lo, &hi);
        for (uint32_t i = lo; i < hi; i++) { p->cntG[(size_t)t * B + p->bktG[i]]++; p->cntW[(size_t)t * B + p->bktW[i]]++; }
    }
    uint64_t ag = 0, aw = 0, pg = 0, pw = 0;
    for (uint32_t b = 0; b < B; b++) {
        p->startG[b] = ag; p->startW[b] = aw;
        for (int t = 0; t < T; t++) {
            size_t s = (size_t)t * B + b;
            p->posG[s] = ag; ag += p->cntG[s]; p->posW[s] = aw; aw += p->cntW[s];
            p->pposG[s] = pg; pg = (pg + p->cntG[s] + WC - 1) / WC * WC;
            p->pposW[s] = pw; pw = (pw + p->cntW[s] + WC - 1) / WC * WC;
        }
    }
    p->startG[B] = ag; p->startW[B] = aw;
    if (ag != n || aw != n || pg > p->padded || pw > p->padded) return 0;
    uint64_t *q = malloc(S * 8 * 4); if (!q) return 0;
    uint64_t *qg = q, *qw = q + S, *rg = q + 2 * S, *rw = q + 3 * S;
    memcpy(qg, p->posG, S * 8); memcpy(qw, p->posW, S * 8);
    memcpy(rg, p->pposG, S * 8); memcpy(rw, p->pposW, S * 8);
    for (int t = 0; t < T; t++) {
        uint32_t lo, hi; range(p, t, &lo, &hi);
        for (uint32_t g = lo; g < hi; g++) {
            size_t s = (size_t)t * B + p->bktG[g]; uint16_t o = (uint16_t)(p->g2p[g] & MASK);
            p->offG[qg[s]++] = o; p->poffG[rg[s]++] = o;
        }
        for (uint32_t i = lo; i < hi; i++) {
            size_t s = (size_t)t * B + p->bktW[i]; uint16_t o = (uint16_t)(p->p2g[i] & MASK);
            p->offW[qw[s]++] = o; p->poffW[rw[s]++] = o;
        }
    }
    free(q);
    return 1;
}

static int tid(void)
{
#ifdef _OPENMP
    return omp_get_thread_num();
#else
    return 0;
#endif
}

/* ---- direct ---- */
static int direct_gather(const plan *p, const double *src, double *dst)
{
    int bad = 0; const uint32_t n = p->n;
    #pragma omp parallel for num_threads(p->T) schedule(static) reduction(|:bad)
    for (uint32_t i = 0; i < n; i++) { double v = src[p->p2g[i]]; dst[i] = v; bad |= !isfinite(v); }
    return bad;
}
static int direct_write(const plan *p, const double *src, double *dst)
{
    const uint32_t n = p->n;
    #pragma omp parallel for num_threads(p->T) schedule(static)
    for (uint32_t g = 0; g < n; g++) dst[g] = src[p->g2p[g]];
    return 0;
}

/* ---- v0 / v1: dense scratch ---- */
static int dense(const plan *p, int writeback, int bucket_ids, const double *src, double *dst,
                 double *scr, uint64_t *cur, double *tp1)
{
    const uint32_t B = p->B;
    const uint32_t *idx = writeback ? p->p2g : p->g2p;
    const uint16_t *bkt = writeback ? p->bktW : p->bktG, *off = writeback ? p->offW : p->offG;
    const uint64_t *pos = writeback ? p->posW : p->posG, *start = writeback ? p->startW : p->startG;
    int bad = 0; double t1 = 0;
    #pragma omp parallel num_threads(p->T) reduction(|:bad)
    {
        int t = tid(); uint32_t lo, hi; range(p, t, &lo, &hi);
        uint64_t *q = cur + (size_t)t * B;
        memcpy(q, pos + (size_t)t * B, (size_t)B * 8);
        if (bucket_ids) for (uint32_t g = lo; g < hi; g++) scr[q[bkt[g]]++] = src[g];
        else for (uint32_t g = lo; g < hi; g++) scr[q[idx[g] >> SHIFT]++] = src[g];
        #pragma omp barrier
        #pragma omp master
        t1 = now();
        #pragma omp for schedule(dynamic, 4)
        for (uint32_t b = 0; b < B; b++) {
            double *d = dst + ((size_t)b << SHIFT);
            for (uint64_t k = start[b]; k < start[b + 1]; k++) { double v = scr[k]; d[off[k]] = v; if (!writeback) bad |= !isfinite(v); }
        }
    }
    *tp1 = t1;
    return bad;
}

/* ---- v2 / v3: padded scratch, write-combined pass 1 ---- */
static inline void stream64(double *d, const double *s)
{
    _mm_stream_pd(d, _mm_load_pd(s)); _mm_stream_pd(d + 2, _mm_load_pd(s + 2));
    _mm_stream_pd(d + 4, _mm_load_pd(s + 4)); _mm_stream_pd(d + 6, _mm_load_pd(s + 6));
}
static int padded(const plan *p, int writeback, int staged, const double *src, double *dst,
                  double *scr, uint64_t *cur, double *const *wcbuf, double *const *stage, double *tp1)
{
    const uint32_t B = p->B, n = p->n; const int T = p->T;
    const uint16_t *bkt = writeback ? p->bktW : p->bktG, *off = writeback ? p->poffW : p->poffG;
    const uint64_t *pos = writeback ? p->pposW : p->pposG;
    const uint32_t *cnt = writeback ? p->cntW : p->cntG;
    int bad = 0; double t1 = 0;
    #pragma omp parallel num_threads(T) reduction(|:bad)
    {
        int t = tid(); uint32_t lo, hi; range(p, t, &lo, &hi);
        uint64_t *q = cur + (size_t)t * B; double *w = wcbuf[t];
        memcpy(q, pos + (size_t)t * B, (size_t)B * 8);
        for (uint32_t g = lo; g < hi; g++) {
#ifdef NO_U16
            const uint32_t b = (writeback ? p->p2g : p->g2p)[g] >> SHIFT;
#else
            const uint32_t b = bkt[g];
#endif
            const uint64_t k = q[b]++;
            double *line = w + (size_t)b * WC;
            line[k & (WC - 1)] = src[g];
            if ((k & (WC - 1)) == WC - 1) stream64(scr + (k - (WC - 1)), line);
        }
        for (uint32_t b = 0; b < B; b++) {                 /* partial lines */
            uint64_t k = q[b], r = k & (WC - 1);
            if (r) memcpy(scr + (k - r), w + (size_t)b * WC, r * 8);
        }
        _mm_sfence();
        #pragma omp barrier
        #pragma omp master
        t1 = now();
        double *st = stage[t];
        #pragma omp for schedule(dynamic, 4)
        for (uint32_t b = 0; b < B; b++) {
            const size_t base = (size_t)b << SHIFT, len = base + BLK <= n ? BLK : n - base;
            double *d = staged ? st : dst + base;
            for (int u = 0; u < T; u++) {
                const size_t s = (size_t)u * B + b; const uint64_t k0 = pos[s];
                const uint64_t k1 = k0 + cnt[s];
                for (uint64_t k = k0; k < k1; k++) d[off[k]] = scr[k];
            }
            if (staged) {
                double *o = dst + base; size_t j = 0;
                if (!writeback) for (size_t i = 0; i < len; i++) bad |= !isfinite(st[i]);
                for (; j + WC <= len; j += WC) stream64(o + j, st + j);
                for (; j < len; j++) o[j] = st[j];
            } else if (!writeback) {
                for (size_t i = 0; i < len; i++) bad |= !isfinite(d[i]);
            }
        }
        _mm_sfence();
    }
    *tp1 = t1;
    return bad;
}

static double median(double *v, int n)
{
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && v[j] < v[j - 1]; j--) { double x = v[j]; v[j] = v[j - 1]; v[j - 1] = x; }
    return n & 1 ? v[n / 2] : 0.5 * (v[n / 2 - 1] + v[n / 2]);
}

#define NV 5
static const char *names[NV] = { "direct", "v0 two-pass", "v1 +u16 bucket", "v2 +WC NT pass1", "v3 +staged NT pass2" };

int main(int argc, char **argv)
{
    unsigned long long cnt = argc > 1 ? strtoull(argv[1], NULL, 10) : 32000000ULL;
    int T = argc > 2 ? atoi(argv[2]) : 4, R = argc > 3 ? atoi(argv[3]) : 8;
    use_huge = argc > 4 ? atoi(argv[4]) : 1;
    if (cnt < 2 || cnt > (1ull << 31) || T < 1 || T > 64 || R < 1 || R > 50) return 2;
#ifdef _OPENMP
    omp_set_dynamic(0);
#endif
    plan p = { .n = (uint32_t)cnt, .B = (uint32_t)((cnt + BLK - 1) >> SHIFT), .T = T };
    if (!build(&p)) { fprintf(stderr, "plan failed\n"); return 1; }
    const uint32_t n = p.n;
    double *global = big((size_t)n * 8), *packed = big((size_t)n * 8), *ref = big((size_t)n * 8);
    double *out = big((size_t)n * 8), *scr = big(p.padded * 8);
    uint64_t *cur = malloc((size_t)p.B * T * 8);
    double *wc[64], *stage[64];
    for (int t = 0; t < T; t++) { wc[t] = big((size_t)p.B * WC * 8); stage[t] = big((size_t)BLK * 8); }
    if (!global || !packed || !ref || !out || !scr || !cur) return 1;
    for (uint32_t i = 0; i < n; i++) global[i] = (double)i + 0.125;
    memset(packed, 0, (size_t)n * 8); memset(out, 0, (size_t)n * 8); memset(scr, 0, p.padded * 8);
    direct_gather(&p, global, ref);

    double tg[NV][50], tw[NV][50], tg1[NV][50], tw1[NV][50];
    for (int r = 0; r < R; r++) {
        for (int vv = 0; vv < NV; vv++) {
            int v = (r & 1) ? NV - 1 - vv : vv;       /* alternate order */
            double t0, t1 = 0; int bad;
            t0 = now();
            if (v == 0) bad = direct_gather(&p, global, packed);
            else if (v <= 2) bad = dense(&p, 0, v == 2, global, packed, scr, cur, &t1);
            else bad = padded(&p, 0, v == 4, global, packed, scr, cur, wc, stage, &t1);
            double e = now();
            tg[v][r] = e - t0; tg1[v][r] = t1 ? t1 - t0 : 0;
            if (bad || memcmp(packed, ref, (size_t)n * 8)) { fprintf(stderr, "%s gather mismatch\n", names[v]); return 1; }
            t0 = now();
            if (v == 0) direct_write(&p, packed, out);
            else if (v <= 2) dense(&p, 1, v == 2, packed, out, scr, cur, &t1);
            else padded(&p, 1, v == 4, packed, out, scr, cur, wc, stage, &t1);
            e = now();
            tw[v][r] = e - t0; tw1[v][r] = t1 ? t1 - t0 : 0;
            if (memcmp(out, global, (size_t)n * 8)) { fprintf(stderr, "%s write mismatch\n", names[v]); return 1; }
            memset(out, 0, 64);                       /* keep the check honest */
            packed[0] = -1;
        }
    }
    printf("n=%u threads=%d repeats=%d hugepages=%d buckets=%u checked=bitwise\n", n, T, R, use_huge, p.B);
    printf("%-22s %10s %10s %10s %10s %10s\n", "variant", "gather", "(pass1)", "write", "(pass1)", "sum");
    double base = 0;
    for (int v = 0; v < NV; v++) {
        double g = median(tg[v], R) * 1e3, w = median(tw[v], R) * 1e3, g1 = median(tg1[v], R) * 1e3, w1 = median(tw1[v], R) * 1e3;
        if (v == 1) base = g + w;
        printf("%-22s %10.2f %10.2f %10.2f %10.2f %10.2f", names[v], g, g1, w, w1, g + w);
        if (v >= 1) printf("  (%.2fx of v0)", (g + w) / base);
        printf("\n");
    }
    return 0;
}
