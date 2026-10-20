/* Replay a simulator's transient dump through vsdlss_m3_transient.
 *
 *   ./bench_tr_replay <dir> [order=0] [threads=1]
 *   <dir>: diag.txt ("i value" or "value"), data.txt ("row col value", strict
 *     upper, zero-based), x_vector.txt (step 0) and, for steps k = 1..N,
 *     <k>b_vector.txt and <k>x_vector.txt ("i value", sequential) -- the files
 *     of examples/transient_driver.  Optional: cdiag.txt ("i value": C/dt of
 *     node i, all of it), cpair.txt ("i j value": C/dt of a two-terminal
 *     capacitor between i and j).
 *   env: TR_STEPS (at most this many steps, default 10), TR_CH = excess
 *     (default) | ratio, DT (seconds: capacitance statistics), TR_TOL (relative
 *     size below which a reconstructed source is zero, default 1e-9: the
 *     excess carries a cancellation error of about eps * G_vv / (C/dt)),
 *     TR_RTOL (b/x counts as step-independent within this relative
 *     difference, default 1e-9; raise it for text with few digits),
 *     PG_ROUNDS (timing rounds, default 3), PG_FUSE (1 automatic, 2 always),
 *     PG_BLOCK / PG_TAILMIN (reduction block and smallest scheduled tail:
 *     let small cases take the fused path),
 *     TR_NOCACHE=1 (do not write the binary cache), TR_CHECK_CDIAG=file (the
 *     true C/dt, e.g. gen_tr_dump's cdiag_true.txt: report the error of Ch),
 *     TR_SAVE=outdir (write the reconstructed Ch, x0 and sources: see
 *     save_inputs), TR_SAVE_FMT=bin (sources in binary)
 *
 * The matrix is A = G + C/dt.  Ch = C/dt is taken
 *   - from cdiag.txt when present, else
 *   - TR_CH=excess: A_vv - sum_u |A_vu| (a grounded capacitor adds to the
 *     diagonal only, a resistor to both diagonal and off-diagonal; pad and
 *     other ground conductances end up here too), clamped at 0;
 *   - TR_CH=ratio: b_{k+1}[v] / x_k[v] where that ratio is the same in every
 *     step (a node without current source), the excess elsewhere;
 * plus the cpair.txt capacitors (C/dt on both diagonals, -C/dt between).
 * Sources are what Ch does not explain: s_{k+1} = b_{k+1} - Ch x_k with the
 * dumped b and x, set to 0 below TR_TOL of |b| + |Ch x|.  Whatever Ch is, the
 * replay is exact up to rounding: s absorbs every other right-hand side term
 * (package inductor history, leakage, ...).
 *
 * Cross-check: the share of nodes whose source is zero in a step (nodes
 * without load current: their b is exactly the capacitor term), and for
 * nodes with a step-independent ratio, how far that ratio is from the excess
 * (text with few significant digits makes the excess noise; then use
 * TR_CH=ratio or cdiag.txt).
 *
 * Then four ways: "orig" (the dumped b, vsdlss_m3_solve: the current usage),
 * "manual" (packed b by the interface's formula, vsdlss_m3_solve_packed_inplace),
 * "tr" (the interface, unfused), "tr+fuse".  A validation pass compares every
 * step with <k>x_vector.txt and checks manual / tr / tr+fuse bitwise (hash of
 * each step's solution); timing rounds follow (order rotated, best each).
 *
 * The text is parsed once; matrix and vectors are cached as binary files in
 * <dir>/.trcache (used while newer than the text). */
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

extern int vsdlss_m3_transient_fuse;
extern csi vsdlss_reduce_block, vsdlss_reduce_tail_min;

static void die(const char *m) { fprintf(stderr, "%s\n", m); exit(1); }
static const char *dir;
static char *pathf(const char *fmt, long long k)
{
    static char buf[8][4096]; static int r; char *p = buf[r++ & 7], name[512];
    snprintf(name, sizeof name, fmt, k);
    snprintf(p, 4096, "%s/%s", dir, name);
    return p;
}
static double mtime(const char *p) { struct stat s; return stat(p, &s) ? -1 : (double)s.st_mtime; }
static int newer(const char *cache, const char *src) { double c = mtime(cache), t = mtime(src); return c >= 0 && t >= 0 && c >= t; }
static int cache_on(void) { const char *e = getenv("TR_NOCACHE"); return !(e && atoi(e)); }

/* "i value" (sequential) or "value" per line; NULL when the file is missing */
static double *read_vec_text(const char *path, csi n)
{
    FILE *f = fopen(path, "r");
    if (!f) return NULL;
    double *v = malloc((size_t)n * 8); char *line = NULL; size_t cap = 0; csi m = 0;
    while (text_line(f, &line, &cap)) {
        char *e; double a = strtod(line, &e), b; char *e2; b = strtod(e, &e2);
        if (e2 != e) { if ((csi)a != m) die("vector file is not sequential"); a = b; }
        if (m >= n) die("vector file longer than the matrix");
        v[m++] = a;
    }
    fclose(f); free(line);
    if (m != n) { fprintf(stderr, "%s: %lld values, matrix %lld\n", path, (long long)m, (long long)n); exit(1); }
    return v;
}
static double *read_vec(const char *txt, const char *bin, csi n)
{
    if (newer(bin, txt)) {
        FILE *f = fopen(bin, "rb"); double *v = malloc((size_t)n * 8);
        if (f && fread(v, 8, (size_t)n, f) == (size_t)n) { fclose(f); return v; }
        if (f) { fclose(f); } free(v);
    }
    double *v = read_vec_text(txt, n);
    if (v && cache_on()) { FILE *f = fopen(bin, "wb"); if (f) { fwrite(v, 8, (size_t)n, f); fclose(f); } }
    return v;
}
static vsdlss *read_matrix(void)
{
    char *bin = pathf(".trcache/A.bin", 0), *dt = pathf("data.txt", 0);
    if (newer(bin, dt)) { vsdlss *A = read_dump(bin); if (A) { puts("# matrix from the binary cache"); return A; } }
    vsdlss *A = read_text(dir);
    if (A && cache_on()) {
        FILE *f = fopen(bin, "wb"); int64_t n = A->n, nz = A->p[A->n];
        if (f) { fwrite(&n, 8, 1, f); fwrite(&nz, 8, 1, f); fwrite(A->p, 8, (size_t)n + 1, f); fwrite(A->i, 8, (size_t)nz, f); fwrite(A->x, 8, (size_t)nz, f); fclose(f); }
    }
    return A;
}

static uint64_t fnv(const double *x, csi n)
{ uint64_t h = 1469598103934665603ULL; const unsigned char *p = (const unsigned char *)x; for (size_t i = 0; i < (size_t)n * 8; i++) { h ^= p[i]; h *= 1099511628211ULL; } return h; }

static int cmpd2(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }
static double pctl(double *v, csi m, double q) { if (!m) return NAN; qsort(v, (size_t)m, 8, cmpd2); return v[(csi)(q * (double)(m - 1))]; }

/* TR_SAVE=<outdir>: the reconstructed inputs of vsdlss_m3_transient, for a
 * simulator or another test to reuse.  Caller (dump) node numbering, zero-based:
 *   cdiag.txt      "i value": C/dt of node i without the pair capacitors (the
 *                  format bench_tr_replay reads: copy it into a dump dir)
 *   cpair.txt      "i j value": the pair capacitors (only when given)
 *   x0.txt         "i value": the start state (the dump's x_vector.txt)
 *   src_nodes.txt  one node per line: the nsrc nodes with a source
 *   <k>s.txt       nsrc values, k = 1..N, line j for src_nodes line j:
 *                  b_k = Ch x_{k-1} + s_k (scattered to the src nodes)
 *   meta.txt       n, steps, nsrc, npair, where Ch came from, TR_TOL
 * TR_SAVE_FMT=bin writes src_nodes.bin (int64) and <k>s.bin (double) instead
 * of the text files (much smaller and faster at 10^7 nodes).  Values with 17
 * significant digits: read back they are bit for bit the same. */
static FILE *wopen(const char *out, const char *fmt, long long k)
{
    char name[256], p[4096]; snprintf(name, sizeof name, fmt, k); snprintf(p, sizeof p, "%s/%s", out, name);
    FILE *f = fopen(p, "w"); if (!f) { perror(p); exit(1); }
    return f;
}
static void save_inputs(const char *out, csi n, csi N, const double *c, csi npair, const csi *pa, const csi *pb, const double *pv,
                        csi nsrc, const csi *src, const double *sv, const double *x0, const char *ch_from, double tol)
{
    const char *e = getenv("TR_SAVE_FMT"); const int bin = e && !strcmp(e, "bin");
    double t0 = now(); FILE *f;
    mkdir(out, 0755);
    f = wopen(out, "cdiag.txt", 0); for (csi v = 0; v < n; v++) fprintf(f, "%lld %.17g\n", (long long)v, c[v]); fclose(f);
    if (npair) { f = wopen(out, "cpair.txt", 0); for (csi q = 0; q < npair; q++) fprintf(f, "%lld %lld %.17g\n", (long long)pa[q], (long long)pb[q], pv[q]); fclose(f); }
    f = wopen(out, "x0.txt", 0); for (csi v = 0; v < n; v++) fprintf(f, "%lld %.17g\n", (long long)v, x0[v]); fclose(f);
    if (bin) {
        f = wopen(out, "src_nodes.bin", 0); for (csi j = 0; j < nsrc; j++) { int64_t q = src[j]; fwrite(&q, 8, 1, f); } fclose(f);
        for (csi k = 0; k < N; k++) { f = wopen(out, "%llds.bin", k + 1); fwrite(sv + k * nsrc, 8, (size_t)nsrc, f); fclose(f); }
    } else {
        f = wopen(out, "src_nodes.txt", 0); for (csi j = 0; j < nsrc; j++) fprintf(f, "%lld\n", (long long)src[j]); fclose(f);
        for (csi k = 0; k < N; k++) { f = wopen(out, "%llds.txt", k + 1); for (csi j = 0; j < nsrc; j++) fprintf(f, "%.17g\n", sv[k * nsrc + j]); fclose(f); }
    }
    f = wopen(out, "meta.txt", 0);
    fprintf(f, "n %lld\nsteps %lld\nnsrc %lld\nnpair %lld\nch %s\ntr_tol %g\nformat %s\n", (long long)n, (long long)N, (long long)nsrc, (long long)npair, ch_from, tol, bin ? "bin" : "txt");
    fclose(f);
    printf("# saved Ch, x0 and the sources of %lld steps to %s (%s, %.1f s)\n", (long long)N, out, bin ? "binary sources" : "text", now() - t0);
}

/* ---- validation / timing callbacks ---- */
typedef struct { const csi *slot; csi n, steps; double *xu; uint64_t *hash; double *err; double *xmax; int compare; } vctx;
static double **xsim;            /* x_k of the dump, caller order, k = 0..N */
static void on_step(void *v, csi k, const double *x, double *dep)
{
    (void)dep; vctx *c = (vctx *)v;
    if (!c->compare || k > c->steps) return;
    for (csi i = 0; i < c->n; i++) c->xu[i] = x[c->slot[i]];
    c->hash[k] = fnv(c->xu, c->n);
    double e = 0, m = 0; const double *r = xsim[k];
    for (csi i = 0; i < c->n; i++) { double d = fabs(c->xu[i] - r[i]); if (d > e) e = d; if (fabs(r[i]) > m) m = fabs(r[i]); }
    c->err[k] = e; c->xmax[k] = m;
}

int main(int argc, char **argv)
{
    if (argc < 2) die("usage: bench_tr_replay <dir> [order] [threads]");
    dir = argv[1];
    const int order = argc > 2 ? atoi(argv[2]) : 0, threads = argc > 3 ? atoi(argv[3]) : 1;
    const char *e;
    const csi maxsteps = (e = getenv("TR_STEPS")) && atol(e) > 0 ? atol(e) : 10;
    const int ratio_mode = (e = getenv("TR_CH")) && !strcmp(e, "ratio");
    const double tol = (e = getenv("TR_TOL")) ? atof(e) : 1e-9, DT = (e = getenv("DT")) ? atof(e) : 0;
    const int rounds = (e = getenv("PG_ROUNDS")) && atoi(e) > 0 ? atoi(e) : 3;
    const int fuse_sel = (e = getenv("PG_FUSE")) ? atoi(e) : 1;
    const double rtol = (e = getenv("TR_RTOL")) ? atof(e) : 1e-9;     /* b/x the same in every step: within this */
    if ((e = getenv("PG_BLOCK"))) vsdlss_reduce_block = atol(e);       /* small cases: blocks and a fusable tail */
    if ((e = getenv("PG_TAILMIN"))) vsdlss_reduce_tail_min = atol(e);
    vsdlss_set_num_threads(threads);
    mkdir(pathf(".trcache", 0), 0755);

    double t0 = now();
    vsdlss *A = read_matrix();
    if (!A) die("cannot read diag.txt / data.txt");
    const csi n = A->n, nz = A->p[n];
    /* steps present */
    csi N = 0;
    while (N < maxsteps && (mtime(pathf("%lldb_vector.txt", N + 1)) >= 0 || mtime(pathf(".trcache/b%lld.bin", N + 1)) >= 0) &&
           (mtime(pathf("%lldx_vector.txt", N + 1)) >= 0 || mtime(pathf(".trcache/x%lld.bin", N + 1)) >= 0)) N++;
    if (!N) die("no 1b_vector.txt / 1x_vector.txt");
    xsim = malloc((size_t)(N + 1) * sizeof(double *));
    double **bsim = malloc((size_t)(N + 1) * sizeof(double *));
    xsim[0] = read_vec(pathf("x_vector.txt", 0), pathf(".trcache/x0.bin", 0), n);
    if (!xsim[0]) die("no x_vector.txt (step 0)");
    bsim[0] = NULL;
    for (csi k = 1; k <= N; k++) {
        bsim[k] = read_vec(pathf("%lldb_vector.txt", k), pathf(".trcache/b%lld.bin", k), n);
        xsim[k] = read_vec(pathf("%lldx_vector.txt", k), pathf(".trcache/x%lld.bin", k), n);
        if (!bsim[k] || !xsim[k]) die("missing step vector");
    }
    printf("# %s: n %lld, nnz(upper) %lld, %lld steps; read %.1f s\n", dir, (long long)n, (long long)nz, (long long)N, now() - t0);

    /* ---- Ch ---- */
    double *dg = calloc((size_t)n, 8), *offs = calloc((size_t)n, 8), *ex = malloc((size_t)n * 8), *c = malloc((size_t)n * 8);
    for (csi j = 0; j < n; j++) for (csi p = A->p[j]; p < A->p[j + 1]; p++) {
        csi i = A->i[p]; if (i == j) dg[j] += A->x[p]; else { offs[i] += fabs(A->x[p]); offs[j] += fabs(A->x[p]); } }
    csi neg = 0;
    for (csi v = 0; v < n; v++) { ex[v] = dg[v] - offs[v]; if (ex[v] < 0) { neg++; ex[v] = 0; } }
    /* step-independent ratios b_{k+1}/x_k */
    unsigned char *steady = malloc((size_t)n); double *rat = malloc((size_t)n * 8); csi nsteady = 0;
    for (csi v = 0; v < n; v++) {
        int ok = xsim[0][v] != 0; double r0 = ok ? bsim[1][v] / xsim[0][v] : 0;
        for (csi k = 1; ok && k < N; k++) { if (xsim[k][v] == 0) { ok = 0; break; } double r = bsim[k + 1][v] / xsim[k][v]; ok = fabs(r - r0) <= rtol * fabs(r0); }
        if (N < 2) ok = 0;            /* one step cannot tell a source from a capacitor */
        steady[v] = (unsigned char)ok; rat[v] = r0; nsteady += ok;
    }
    const char *src_name = "excess (diagonal minus off-diagonal sum)";
    double *cfile = read_vec_text(pathf("cdiag.txt", 0), n);
    for (csi v = 0; v < n; v++) c[v] = cfile ? cfile[v] : (ratio_mode && steady[v]) ? rat[v] : ex[v];
    if (cfile) src_name = "cdiag.txt"; else if (ratio_mode) src_name = "ratio b/x where steady, excess elsewhere";
    if ((e = getenv("TR_CHECK_CDIAG"))) {          /* the true C/dt (gen_tr_dump's cdiag_true.txt) */
        double *tru = read_vec_text(e, n), mxs = 0; csi off6 = 0;
        if (!tru) die("cannot read TR_CHECK_CDIAG");
        for (csi v = 0; v < n; v++) if (tru[v] > 0) { double d = fabs(c[v] - tru[v]) / tru[v]; off6 += d > 1e-6; if (steady[v] && d > mxs) mxs = d; }
        printf("# Ch against %s: max relative error at nodes without source %.2e; nodes off by more than 1e-6: %lld "
               "(pads and other ground conductances: the excess includes them, the sources absorb them)\n", e, mxs, (long long)off6);
        free(tru);
    }
    /* cross-check: steady ratios against the excess */
    { double *rd = malloc((size_t)(nsteady + 1) * 8); csi m = 0;
      for (csi v = 0; v < n; v++) if (steady[v] && rat[v] > 0) rd[m++] = fabs(ex[v] - rat[v]) / rat[v];
      double p50 = pctl(rd, m, 0.5), p99 = pctl(rd, m, 0.99), mx = m ? rd[m - 1] : NAN;
      printf("# Ch from %s; excess < 0 clamped at %lld nodes\n", src_name, (long long)neg);
      printf("# cross-check: %lld nodes (%.1f%%) have a step-independent b/x ratio; |excess - ratio| / ratio: median %.2e, 99%% %.2e, max %.2e%s\n",
             (long long)nsteady, 100.0 * (double)nsteady / (double)n, p50, p99, mx,
             (m && p50 > 1e-6) ? "  <- the excess is not reliable (few digits in the text?): try TR_CH=ratio or cdiag.txt" : "");
      if (!m && N >= 2)
          printf("# warning: no node has a step-independent b/x ratio -- every node has a load current, or the text has few digits (raise TR_RTOL, e.g. 1e-5 for 7 digits)\n");
      if (DT > 0 && m) {
          csi q = 0; for (csi v = 0; v < n; v++) if (steady[v] && rat[v] > 0) rd[q++] = rat[v] * DT;
          printf("# capacitance at those nodes (DT = %g s): 1%% %.3g F, median %.3g F, 99%% %.3g F\n", DT, pctl(rd, q, 0.01), pctl(rd, q, 0.5), pctl(rd, q, 0.99));
      }
      free(rd); }
    /* Ch matrix: diagonal c (+ cpair), off-diagonal -cpair */
    csi npair = 0, cappair = 0; csi *pa = NULL, *pb = NULL; double *pv = NULL;
    { FILE *f = fopen(pathf("cpair.txt", 0), "r");
      if (f) { char *line = NULL; size_t cp = 0;
          while (text_line(f, &line, &cp)) { long long a, b; double v; if (sscanf(line, "%lld %lld %lf", &a, &b, &v) != 3 || a < 0 || b < 0 || a >= n || b >= n || a == b) die("bad cpair.txt line");
              if (npair == cappair) { cappair = cappair ? 2 * cappair : 1024; pa = realloc(pa, (size_t)cappair * sizeof(csi)); pb = realloc(pb, (size_t)cappair * sizeof(csi)); pv = realloc(pv, (size_t)cappair * 8); }
              pa[npair] = a < b ? a : b; pb[npair] = a < b ? b : a; pv[npair++] = v; }
          fclose(f); free(line); } }
    vsdlss *Ch;
    { csi *ea = malloc((size_t)(n + npair) * sizeof(csi)), *eb = malloc((size_t)(n + npair) * sizeof(csi)); double *exv = malloc((size_t)(n + npair) * 8); csi m = 0;
      double *cd = malloc((size_t)n * 8); memcpy(cd, c, (size_t)n * 8);
      for (csi q = 0; q < npair; q++) { cd[pa[q]] += pv[q]; cd[pb[q]] += pv[q]; ea[m] = pa[q]; eb[m] = pb[q]; exv[m++] = -pv[q]; }
      for (csi v = 0; v < n; v++) { ea[m] = v; eb[m] = v; exv[m++] = cd[v]; }
      /* upper CSC by column, rows ascending */
      csi *cnt = calloc((size_t)n + 1, sizeof(csi)); for (csi q = 0; q < m; q++) cnt[eb[q] + 1]++;
      for (csi j = 0; j < n; j++) cnt[j + 1] += cnt[j];
      Ch = vsdlss_spalloc(n, n, m, 1, 0); csi *cur = malloc((size_t)n * sizeof(csi)); memcpy(cur, cnt, (size_t)n * sizeof(csi));
      for (csi q = 0; q < m; q++) { csi j = eb[q]; Ch->i[cur[j]] = ea[q]; Ch->x[cur[j]++] = exv[q]; }
      for (csi j = 0; j < n; j++) { Ch->p[j] = cnt[j];
          for (csi p = cnt[j] + 1; p < cnt[j + 1]; p++) for (csi r = p; r > cnt[j] && Ch->i[r] < Ch->i[r - 1]; r--) { csi ti = Ch->i[r]; double tx = Ch->x[r]; Ch->i[r] = Ch->i[r - 1]; Ch->x[r] = Ch->x[r - 1]; Ch->i[r - 1] = ti; Ch->x[r - 1] = tx; } }
      Ch->p[n] = m;
      free(ea); free(eb); free(exv); free(cd); free(cnt); free(cur); }

    /* ---- sources: s_{k+1} = b_{k+1} - Ch x_k (dumped x), zero below tol ---- */
    double **sk = malloc((size_t)(N + 1) * sizeof(double *)); unsigned char *has = calloc((size_t)n, 1);
    double *chx = malloc((size_t)n * 8); csi zero_total = 0;
    for (csi k = 0; k < N; k++) {
        const double *x = xsim[k], *b = bsim[k + 1];
        vsdlss_spmv_sym_upper(Ch, x, chx);
        double *s = malloc((size_t)n * 8); csi zeros = 0;
        for (csi v = 0; v < n; v++) { double r = b[v] - chx[v]; if (fabs(r) <= tol * (fabs(b[v]) + fabs(chx[v]))) { r = 0; zeros++; } else has[v] = 1; s[v] = r; }
        sk[k + 1] = s; zero_total += zeros;
        if (k < 3 || k == N - 1) printf("# step %lld: nodes with zero source %lld (%.1f%%)\n", (long long)(k + 1), (long long)zeros, 100.0 * (double)zeros / (double)n);
    }
    csi nsrc = 0; for (csi v = 0; v < n; v++) nsrc += has[v];
    csi *src = malloc((size_t)(nsrc ? nsrc : 1) * sizeof(csi)); nsrc = 0;
    for (csi v = 0; v < n; v++) if (has[v]) src[nsrc++] = v;
    double *sv = malloc((size_t)(nsrc > 0 ? N * nsrc : 1) * 8);
    for (csi k = 0; k < N; k++) for (csi j = 0; j < nsrc; j++) sv[k * nsrc + j] = sk[k + 1][src[j]];
    for (csi k = 1; k <= N; k++) free(sk[k]);
    printf("# sources: %lld nodes (%.1f%%) carry a nonzero value in some step; %lld two-terminal capacitors (cpair.txt)\n",
           (long long)nsrc, 100.0 * (double)nsrc / (double)n, (long long)npair);
    if ((e = getenv("TR_SAVE")) && *e) save_inputs(e, n, N, c, npair, pa, pb, pv, nsrc, src, sv, xsim[0], src_name, tol);

    t0 = now();
    vsdlss_m3_factor *F = NULL;
    if (vsdlss_factorize_m3(A, order, &F) != VSDLSS_OK) die("factorization failed");
    printf("# factorize (order %d, %d threads): %.2f s\n", order, threads, now() - t0);
    csi *slot = malloc((size_t)n * sizeof(csi)); vsdlss_m3_export_packed_slots(F, slot, n);
    double *dpk = calloc((size_t)n, 8);
    for (csi j = 0; j < n; j++) for (csi p = Ch->p[j]; p < Ch->p[j + 1]; p++) if (Ch->i[p] == j) dpk[slot[j]] += Ch->x[p];
    /* off-diagonal partners in packed order (the interface's formula) */
    csi *pc = calloc((size_t)n + 1, sizeof(csi));
    for (csi q = 0; q < npair; q++) { pc[slot[pa[q]] + 1]++; pc[slot[pb[q]] + 1]++; }
    for (csi p = 0; p < n; p++) pc[p + 1] += pc[p];
    csi *pu = malloc((size_t)(pc[n] + 1) * sizeof(csi)), *fl = calloc((size_t)n, sizeof(csi)); double *pw = malloc((size_t)(pc[n] + 1) * 8);
    for (csi j = 0; j < n; j++) for (csi p = Ch->p[j]; p < Ch->p[j + 1]; p++) if (Ch->i[p] != j) {
        csi a = slot[Ch->i[p]], b = slot[j]; pu[pc[a] + fl[a]] = b; pw[pc[a] + fl[a]++] = Ch->x[p]; pu[pc[b] + fl[b]] = a; pw[pc[b] + fl[b]++] = Ch->x[p]; }
    for (csi p = 0; p < n; p++) for (csi a = pc[p] + 1; a < pc[p + 1]; a++) { csi u = pu[a]; double v = pw[a]; csi t = a; while (t > pc[p] && pu[t - 1] > u) { pu[t] = pu[t - 1]; pw[t] = pw[t - 1]; t--; } pu[t] = u; pw[t] = v; }

    /* ---- the four ways ---- */
    const char *name[4] = {"orig", "manual", "tr", "tr+fuse"};
    uint64_t *hash[4]; double *err[4], *xmx[4], best[4] = {1e300, 1e300, 1e300, 1e300};
    double *xu = malloc((size_t)n * 8), *xa = malloc((size_t)n * 8), *xb = malloc((size_t)n * 8), *s = calloc((size_t)n, 8);
    for (int m = 0; m < 4; m++) { hash[m] = calloc((size_t)N + 1, 8); err[m] = calloc((size_t)N + 1, 8); xmx[m] = calloc((size_t)N + 1, 8); }
    for (int r = -1; r < rounds; r++) for (int i = 0; i < 4; i++) {
        const int m = r < 0 ? i : (i + r) % 4;
        vctx cx = {slot, n, N, xu, hash[m], err[m], xmx[m], r < 0};
        double tt = 0;
        if (m == 0) {                          /* the dumped b, caller order */
            memcpy(xa, xsim[0], (size_t)n * 8);
            for (csi k = 1; k <= N; k++) {
                double t1 = now();
                if (vsdlss_m3_solve(F, bsim[k], xa) != VSDLSS_OK) die("solve failed");
                tt += now() - t1;
                if (r < 0) { hash[m][k] = fnv(xa, n); double e2 = 0, mx = 0; for (csi v = 0; v < n; v++) { double d = fabs(xa[v] - xsim[k][v]); if (d > e2) e2 = d; if (fabs(xsim[k][v]) > mx) mx = fabs(xsim[k][v]); } err[m][k] = e2; xmx[m][k] = mx; }
            }
        } else if (m == 1) {                   /* packed b by the formula */
            for (csi v = 0; v < n; v++) xa[slot[v]] = xsim[0][v];
            memset(s, 0, (size_t)n * 8);
            double t1 = now();
            if (r < 0) on_step(&cx, 0, xa, NULL);
            for (csi k = 0; k < N; k++) {
                for (csi j = 0; j < nsrc; j++) s[slot[src[j]]] = 0.0;
                for (csi j = 0; j < nsrc; j++) s[slot[src[j]]] += sv[k * nsrc + j];
                for (csi p = 0; p < n; p++) { double v = dpk[p] * xa[p]; for (csi a = pc[p]; a < pc[p + 1]; a++) v += pw[a] * xa[pu[a]]; xb[p] = v + s[p]; }
                if (vsdlss_m3_solve_packed_inplace(F, xb) != VSDLSS_OK) die("packed solve failed");
                double *t = xa; xa = xb; xb = t;
                if (r < 0) { double t2 = now(); on_step(&cx, k + 1, xa, NULL); t1 += now() - t2; }
            }
            tt = now() - t1;
        } else {
            vsdlss_m3_transient_fuse = m == 3 ? fuse_sel : 0;
            vsdlss_m3_transient *T = NULL;
            if (vsdlss_m3_transient_create(F, Ch, nsrc, src, 0, NULL, &T) != VSDLSS_OK) die("transient create failed");
            if (r < 0) { csi fc, u, l; vsdlss_m3_transient_info(T, &fc, &u, &l);
                printf("# %-8s fused components %lld, tail chunks %lld, late units %lld\n", name[m], (long long)fc, (long long)u, (long long)l); }
            vsdlss_m3_transient_start(T, xsim[0]);
            double t1 = now();
            if (vsdlss_m3_transient_run(T, N, sv, on_step, &cx) != VSDLSS_OK) die("transient run failed");
            tt = now() - t1;
            vsdlss_m3_transient_free(T);
        }
        if (r >= 0 && tt < best[m]) best[m] = tt;
    }
    printf("validation (every step against <k>x_vector.txt; max |x - x_dump| / max |x_dump| over the steps):\n");
    for (int m = 0; m < 4; m++) {
        double rel = 0; for (csi k = 1; k <= N; k++) if (xmx[m][k] > 0 && err[m][k] / xmx[m][k] > rel) rel = err[m][k] / xmx[m][k];
        int same = 1; if (m >= 2) for (csi k = 0; k <= N; k++) same &= hash[m][k] == hash[1][k];
        printf("  %-8s %.3e%s\n", name[m], rel, m >= 2 ? (same ? "  bitwise = manual" : "  DIFFERS from manual") : "");
    }
    printf("timing (best of %d rounds, order rotated), per step:\n", rounds);
    for (int m = 0; m < 4; m++) printf("  %-8s %8.2f ms%s\n", name[m], 1e3 * best[m] / (double)N, m == 0 ? "  (solve only: the dumped b is given)" : "");
    vsdlss_m3_factor_free(F);
    return 0;
}
