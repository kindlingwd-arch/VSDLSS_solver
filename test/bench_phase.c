/* Solve-phase micro-benchmark: times the M3 solve phases of one component in
 * isolation, so a change to one phase is measured without the rest of the
 * solve (and without a second factorization per variant).
 *
 *   ./bench_phase dump.bin            order threads_list [reps]
 *   ./bench_phase grid:SIDE[:CHAIN]   order threads_list [reps]
 *   ./bench_phase text:DIR            order threads_list [reps]   (DIR/diag.txt, DIR/data.txt)
 *   build: make bench_phase METIS=1 (order 6), or without METIS for order 5
 *
 * dump.bin is a PG_DUMP system (int64 n, nnz, upper CSC p, i, x, b).
 * grid:SIDE builds a SIDE x SIDE grid whose edges are chains of CHAIN
 * (default 3) intermediate nodes, a power-grid-like mix of degree 2 and 4;
 * small sides keep the working set in cache ("hot"), which removes the
 * memory-bandwidth ceiling of a laptop and exposes scheduling and serial
 * costs that dominate on a high-bandwidth server.
 *
 * The largest component is factored once with `order`; then for each thread
 * count in threads_list ("1 2 4 8"), the median over reps of:
 *   replay fwd / bwd       vsdlss_reduce_{forward,backward}_inplace, all records
 *   blocks fwd / bwd       the parallel block segments only
 *   tail fwd / bwd         the sequential tail segment only
 *   core fwd+bwd           vsdlss_sn_solve_inplace on the core
 * with the bytes each phase streams (records or L) per millisecond.  The
 * vector is reset (outside the timing) before every repetition.  Results
 * are bitwise compared across thread counts (bwd after fwd, core). */
#define _GNU_SOURCE   /* getline, syscall */
#include "../src/vsdlss_m3_internal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#if defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#endif

/* VSDLSS_INTERLEAVE=1: interleave the process's memory over the online NUMA
 * nodes (as numactl --interleave=all), which also keeps the kernel's
 * automatic NUMA balancing from scanning it (see examples/transient_driver). */
static void interleave_memory(void)
{
    const char *e = getenv("VSDLSS_INTERLEAVE");
    if (!e || atoi(e) == 0) { puts("# numa: off (VSDLSS_INTERLEAVE=1 to interleave)"); return; }
#if defined(__linux__) && defined(SYS_set_mempolicy)
    FILE *f = fopen("/sys/devices/system/node/online", "r"); char list[256] = "";
    if (!f || !fgets(list, sizeof list, f)) { if (f) fclose(f); puts("# numa: no NUMA information"); return; }
    fclose(f);
    unsigned long mask[16] = {0}; int maxnode = 0, nodes = 0;
    for (char *s = list; *s && *s != '\n';) {
        char *end; long a = strtol(s, &end, 10), b = a;
        if (end == s) break;
        if (*end == '-') { s = end + 1; b = strtol(s, &end, 10); }
        for (long nd = a; nd <= b && nd < 1024; nd++) { mask[nd / 64] |= 1UL << (nd % 64); nodes++; if (nd + 1 > maxnode) maxnode = (int)nd + 1; }
        s = *end == ',' ? end + 1 : end;
    }
    if (nodes < 2) { puts("# numa: single NUMA node, nothing to interleave"); return; }
    if (syscall(SYS_set_mempolicy, 3 /* MPOL_INTERLEAVE */, mask, (unsigned long)maxnode + 1) != 0)
        printf("# numa: set_mempolicy failed: %s\n", strerror(errno));
    else printf("# numa: interleaved over nodes %s", list);
#else
    puts("# numa: not supported on this platform");
#endif
}

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static int cmpd(const void *a, const void *b) { double x = *(const double *)a, y = *(const double *)b; return (x > y) - (x < y); }

typedef struct { csi a, b; double w; } gedge;
static int cmp_edge(const void *x, const void *y)
{
    const gedge *p = x, *q = y;
    if (p->b != q->b) return (p->b > q->b) - (p->b < q->b);
    return (p->a > q->a) - (p->a < q->a);
}

/* Upper CSC of the grid-of-chains Laplacian, slightly grounded. */
static vsdlss *grid_chains(csi side, csi chain)
{
    csi n = side * side, e = 0, maxe = 2 * side * (side - 1) * (chain + 1);
    gedge *ed = malloc((size_t)maxe * sizeof(*ed));
    if (!ed) return NULL;
    for (csi y = 0; y < side; y++) for (csi x = 0; x < side; x++) for (int dir = 0; dir < 2; dir++) {
        csi x2 = x + (dir == 0), y2 = y + (dir == 1), prev = y * side + x;
        if (x2 >= side || y2 >= side) continue;
        for (csi c = 0; c < chain; c++) { csi node = n++; ed[e++] = (gedge){prev, node, 1.0 + 0.001 * (double)(e % 97)}; prev = node; }
        ed[e++] = (gedge){prev, y2 * side + x2, 0.5 + 0.002 * (double)(e % 89)};
    }
    for (csi k = 0; k < e; k++) if (ed[k].a > ed[k].b) { csi t = ed[k].a; ed[k].a = ed[k].b; ed[k].b = t; }
    qsort(ed, (size_t)e, sizeof(*ed), cmp_edge);
    double *diag = calloc((size_t)n, sizeof(double));
    vsdlss *A = vsdlss_spalloc(n, n, n + e, 1, 0);
    if (!diag || !A) { free(ed); free(diag); return NULL; }
    for (csi k = 0; k < e; k++) { diag[ed[k].a] += ed[k].w; diag[ed[k].b] += ed[k].w; }
    csi nz = 0, k = 0;
    for (csi col = 0; col < n; col++) {
        A->p[col] = nz;
        for (; k < e && ed[k].b == col; k++) { A->i[nz] = ed[k].a; A->x[nz++] = -ed[k].w; }
        A->i[nz] = col; A->x[nz++] = diag[col] + 1e-3;
    }
    A->p[n] = nz;
    free(ed); free(diag);
    return A;
}

static vsdlss *read_dump(const char *path)
{
    FILE *fp = fopen(path, "rb"); int64_t n, nz;
    if (!fp || fread(&n, 8, 1, fp) != 1 || fread(&nz, 8, 1, fp) != 1) { if (fp) fclose(fp); return NULL; }
    vsdlss *A = vsdlss_spalloc(n, n, nz, 1, 0);
    if (!A || fread(A->p, 8, (size_t)n + 1, fp) != (size_t)n + 1 || fread(A->i, 8, (size_t)nz, fp) != (size_t)nz ||
        fread(A->x, 8, (size_t)nz, fp) != (size_t)nz) { fclose(fp); return NULL; }
    fclose(fp);
    return A;
}

/* text:DIR -- the files of examples/transient_driver: DIR/diag.txt ("value" or
 * "i value" per line) and DIR/data.txt ("row col value", strict upper,
 * zero-based).  Blank lines and lines starting with '#' are skipped. */
static int text_line(FILE *f, char **buf, size_t *cap)
{
    for (;;) {
        if (getline(buf, cap, f) < 0) return 0;
        char *s = *buf; while (*s == ' ' || *s == '\t') s++;
        if (*s && *s != '\n' && *s != '\r' && *s != '#') return 1;
    }
}
static vsdlss *read_text(const char *dir)
{
    char path[4096]; char *line = NULL; size_t cap = 0;
    snprintf(path, sizeof path, "%s/diag.txt", dir);
    FILE *f = fopen(path, "r");
    if (!f) { perror(path); return NULL; }
    size_t nd = 0, capd = 1 << 20; double *diag = malloc(capd * sizeof(double));
    while (diag && text_line(f, &line, &cap)) {
        char *e; double a = strtod(line, &e), b; char *e2;
        b = strtod(e, &e2);
        if (e2 != e) a = b;                          /* "i value" */
        if (nd == capd) { capd *= 2; diag = realloc(diag, capd * sizeof(double)); if (!diag) break; }
        diag[nd++] = a;
    }
    fclose(f);
    snprintf(path, sizeof path, "%s/data.txt", dir);
    f = fopen(path, "r");
    if (!f || !diag) { perror(path); free(diag); free(line); return NULL; }
    size_t ne = 0, cape = 1 << 20; int64_t *ri = malloc(cape * 8), *ci = malloc(cape * 8); double *vv = malloc(cape * 8);
    while (ri && ci && vv && text_line(f, &line, &cap)) {
        char *e; int64_t r = strtoll(line, &e, 10), c = strtoll(e, &e, 10); double v = strtod(e, NULL);
        if (r < 0 || c <= r || (size_t)c >= nd) { fprintf(stderr, "bad entry %lld %lld\n", (long long)r, (long long)c); return NULL; }
        if (ne == cape) {
            cape *= 2; ri = realloc(ri, cape * 8); ci = realloc(ci, cape * 8); vv = realloc(vv, cape * 8);
            if (!ri || !ci || !vv) break;
        }
        ri[ne] = r; ci[ne] = c; vv[ne++] = v;
    }
    fclose(f); free(line);
    if (!ri || !ci || !vv) { puts("out of memory reading data.txt"); return NULL; }
    const csi n = (csi)nd;
    vsdlss *A = vsdlss_spalloc(n, n, n + (csi)ne, 1, 0);
    size_t *byrow = malloc(ne * sizeof(size_t)), *rp = calloc(nd + 1, sizeof(size_t));
    if (!A || !byrow || !rp) { puts("out of memory building CSC"); return NULL; }
    for (size_t k = 0; k < ne; k++) rp[ri[k] + 1]++;                   /* entries by row ... */
    for (size_t i = 0; i < nd; i++) rp[i + 1] += rp[i];
    for (size_t k = 0; k < ne; k++) byrow[rp[ri[k]]++] = k;
    for (csi j = 0; j <= n; j++) A->p[j] = 0;
    for (size_t k = 0; k < ne; k++) A->p[ci[k] + 1]++;
    for (csi j = 0; j < n; j++) A->p[j + 1] += A->p[j] + 1;              /* + the diagonal */
    csi *pos = malloc((size_t)n * sizeof(csi));
    if (!pos) return NULL;
    for (csi j = 0; j < n; j++) pos[j] = A->p[j];
    for (size_t q = 0; q < ne; q++) {                                  /* ... so each column gets ascending rows */
        const size_t k = byrow[q]; const csi p = pos[ci[k]]++;
        A->i[p] = ri[k]; A->x[p] = vv[k];
    }
    for (csi j = 0; j < n; j++) { A->i[A->p[j + 1] - 1] = j; A->x[A->p[j + 1] - 1] = diag[j]; }
    free(pos); free(byrow); free(rp); free(ri); free(ci); free(vv); free(diag);
    return A;
}

/* Level structure of the sequential tail (segments q >= blocks): a record's
 * level is one more than the last level touching its pivot or a neighbour, so
 * the records of one level touch disjoint vertices and could be replayed
 * concurrently.  Prints the depth and how parallel the levels are. */
static void tail_levels(const vsdlss_reduction *r)
{
    double tail = 0;
    for (csi q = r->blocks; q < r->pk_count; q++) tail += (double)r->pk[q].count;
    if (tail < 1) { puts("# tail: empty"); return; }
    int *last = calloc((size_t)r->n, sizeof(int)), *lv = malloc((size_t)tail * sizeof(int));
    if (!last || !lv) { free(last); free(lv); puts("# tail levels: out of memory"); return; }
    csi k = 0; int maxl = 0;
    for (csi q = r->blocks; q < r->pk_count; q++) {
        const vsdlss_pk_seg *g = r->pk + q; csi o = 0;
        for (csi i = 0; i < g->count; i++) {
            const csi v = vsdlss_pk_vertex(g, i), d = vsdlss_pk_degree(g, i);
            int l = last[v];
            for (csi j = 0; j < d; j++) if (last[g->nb[o + j]] > l) l = last[g->nb[o + j]];
            l++; last[v] = l; for (csi j = 0; j < d; j++) last[g->nb[o + j]] = l;
            lv[k++] = l; if (l > maxl) maxl = l; o += d;
        }
    }
    long *sz = calloc((size_t)maxl + 1, sizeof(long));
    if (!sz) { free(last); free(lv); return; }
    for (csi i = 0; i < k; i++) sz[lv[i]]++;
    double big = 0; static const int P[4] = {8, 16, 32, 64}; double steps[4] = {0};
    for (int l = 1; l <= maxl; l++) {
        if (sz[l] >= 256) big += (double)sz[l];
        for (int p = 0; p < 4; p++) steps[p] += (double)((sz[l] + P[p] - 1) / P[p]);
    }
    printf("# tail: %.0f records in %d levels (%.0f per level on average), %.1f%% of them in levels of >= 256 | "
           "ideal speedup of a level-parallel tail: 8 thr %.1fx, 16 thr %.1fx, 32 thr %.1fx, 64 thr %.1fx\n",
           tail, maxl, tail / maxl, 100 * big / tail, tail / steps[0], tail / steps[1], tail / steps[2], tail / steps[3]);
    free(last); free(lv); free(sz);
}

/* A view of the reduction with only the block segments (part 1) or only the
 * tail (part 2) non-empty; the shape checks still pass. */
static vsdlss_reduction view(const vsdlss_reduction *r, int part, vsdlss_pk_seg *seg, csi *bp)
{
    vsdlss_reduction v = *r;
    for (csi q = 0; q < r->pk_count; q++) {
        int keep = part == 0 || (part == 1 && q < r->blocks) || (part == 2 && q >= r->blocks);
        seg[q] = r->pk[q];
        if (!keep) { seg[q].count = 0; seg[q].nbn = 0; }
    }
    for (csi b = 0; b <= r->blocks; b++) bp[b] = part == 2 ? 0 : r->block_ptr[b];
    if (part == 1 && r->blocks) bp[r->blocks] = r->block_ptr[r->blocks];
    v.pk = seg; v.block_ptr = bp;
    return v;
}

#define MAXT 16
int main(int argc, char **argv)
{
    if (argc < 4) { fprintf(stderr, "usage: bench_phase dump.bin|grid:SIDE[:CHAIN]|text:DIR order \"threads\" [reps]\n"); return 1; }
    const int order = atoi(argv[2]), reps = argc > 4 ? atoi(argv[4]) : 15;
    interleave_memory();                     /* before the large allocations */
    vsdlss *A = NULL;
    if (!strncmp(argv[1], "grid:", 5)) {
        csi side = atol(argv[1] + 5), chain = 3; const char *c = strchr(argv[1] + 5, ':');
        if (c) chain = atol(c + 1);
        A = grid_chains(side, chain);
    } else if (!strncmp(argv[1], "text:", 5)) {
        double t = now(); A = read_text(argv[1] + 5);
        if (A) printf("# read %s in %.1f s: n %lld, nnz %lld\n", argv[1] + 5, now() - t, (long long)A->n, (long long)A->p[A->n]);
    } else A = read_dump(argv[1]);
    if (!A) { puts("input failed"); return 1; }
    int threads[MAXT], nth = 0; char buf[256]; strncpy(buf, argv[3], 255); buf[255] = 0;
    for (char *t = strtok(buf, " ,"); t && nth < MAXT; t = strtok(NULL, " ,")) threads[nth++] = atoi(t);
    int tmax = 1; for (int i = 0; i < nth; i++) if (threads[i] > tmax) tmax = threads[i];
    vsdlss_set_num_threads(tmax);
    vsdlss_m3_factor *F = NULL; double t0 = now();
    if (vsdlss_factorize_m3(A, order, &F) != VSDLSS_OK) { puts("factor failed"); return 1; }
    const double tf = now() - t0;
    csi best = -1;
    for (csi c = 0; c < F->count; c++) if (F->component[c].reduction && (best < 0 || F->component[c].n > F->component[best].n)) best = c;
    if (best < 0) { puts("no reduced component"); return 1; }
    const vsdlss_m3_component_factor *cf = F->component + best;
    const vsdlss_reduction *r = cf->reduction; const vsdlss_sn_factor *sf = cf->numeric;
    if (!r->pk || r->records) { puts("reduction not packed"); return 1; }
    const csi n = r->n;
    double tail_rec = 0, rec_bytes[3] = {0};
    for (csi q = 0; q < r->pk_count; q++) {
        double b = (double)r->pk[q].count * (r->pk[q].head ? 12 : 9) + (double)r->pk[q].nbn * 12;
        rec_bytes[0] += b; rec_bytes[q < r->blocks ? 1 : 2] += b;
        if (q >= r->blocks) tail_rec += (double)r->pk[q].count;
    }
    double lbytes = sf ? (double)sf->panel_offset[sf->count] * 8 * 2 : 0;
    printf("# %s: n %lld (component %lld of %lld, n %lld), factor %.1f s | records %lld, blocks %lld, tail %.0f (%.1f%%) | core %lld, L %.0f MB\n",
           argv[1], (long long)A->n, (long long)best, (long long)F->count, (long long)n, tf, (long long)r->count,
           (long long)r->blocks, tail_rec, 100 * tail_rec / (double)(r->count ? r->count : 1),
           (long long)(sf ? sf->n : 0), sf ? (double)sf->panel_offset[sf->count] * 8 / 1e6 : 0);
    for (csi c = 0; c < F->count; c++)        /* every large component, not just the measured one */
        if (F->component[c].reduction && F->component[c].n >= 100000)
            printf("# component %lld: n %lld, records %lld (%.1f%% of n), core %lld\n", (long long)c,
                   (long long)F->component[c].n, (long long)F->component[c].reduction->count,
                   100.0 * (double)F->component[c].reduction->count / (double)F->component[c].n,
                   (long long)F->component[c].reduction->core_n);
    tail_levels(r);
    fflush(stdout);

    vsdlss_pk_seg *seg = malloc((size_t)r->pk_count * sizeof(*seg)); csi *bp = malloc((size_t)(r->blocks + 1) * sizeof(csi));
    double *w0 = malloc((size_t)n * 8), *w = malloc((size_t)n * 8), *ref = malloc((size_t)n * 8);
    const csi cn = sf ? sf->n : 0;
    double *c0 = malloc((size_t)(cn ? cn : 1) * 8), *cx = malloc((size_t)(cn ? cn : 1) * 8), *cref = malloc((size_t)(cn ? cn : 1) * 8);
    double ts[64];
    if (!seg || !bp || !w0 || !w || !ref || !c0 || !cx || !cref) { puts("alloc failed"); return 1; }
    for (csi i = 0; i < n; i++) w0[i] = sin(0.37 * (double)i) + 0.2;
    for (csi i = 0; i < cn; i++) c0[i] = cos(0.11 * (double)i) + 0.3;
    static const char *name[3] = {"replay", "blocks", "tail"};
    int mismatch = 0;
    for (int ti = 0; ti < nth; ti++) {
        vsdlss_set_num_threads(threads[ti]);
        printf("T=%-2d", threads[ti]);
        for (int part = 0; part < 3; part++) {
            vsdlss_reduction v = view(r, part, seg, bp);
            if (part && !r->blocks) continue;
            double med[2];
            for (int dir = 0; dir < 2; dir++) {
                int R = reps < 64 ? reps : 64;
                for (int k = -1; k < R; k++) {
                    memcpy(w, w0, (size_t)n * 8);
                    if (dir) vsdlss_reduce_forward_inplace(&v, w, NULL);
                    double s = now();
                    if (dir) vsdlss_reduce_backward_inplace(&v, NULL, w);
                    else vsdlss_reduce_forward_inplace(&v, w, NULL);
                    if (k >= 0) ts[k] = now() - s;
                }
                qsort(ts, (size_t)R, 8, cmpd); med[dir] = ts[R / 2];
            }
            if (part == 0) {     /* full replay result, compared across thread counts */
                if (ti == 0) memcpy(ref, w, (size_t)n * 8); else mismatch |= memcmp(ref, w, (size_t)n * 8) != 0;
            }
            printf(" | %s fwd %7.2f bwd %7.2f ms (%4.1f GB/s)", name[part], 1e3 * med[0], 1e3 * med[1],
                   rec_bytes[part] * 2 / (med[0] + med[1]) / 1e9);
        }
        if (sf) {
            int R = reps < 64 ? reps : 64;
            for (int k = -1; k < R; k++) {
                memcpy(cx, c0, (size_t)cn * 8);
                double s = now(); vsdlss_sn_solve_inplace(sf, cx); if (k >= 0) ts[k] = now() - s;
            }
            qsort(ts, (size_t)R, 8, cmpd);
            if (ti == 0) memcpy(cref, cx, (size_t)cn * 8); else mismatch |= memcmp(cref, cx, (size_t)cn * 8) != 0;
            printf(" | core fwd+bwd %7.2f ms (%4.1f GB/s)", 1e3 * ts[R / 2], lbytes / ts[R / 2] / 1e9);
        }
        printf("\n");
    }
    printf("# results across thread counts: %s\n", mismatch ? "DIFFERENT" : "bitwise identical");
    vsdlss_m3_factor_free(F); vsdlss_spfree(A);
    free(seg); free(bp); free(w0); free(w); free(ref); free(c0); free(cx); free(cref);
    return mismatch;
}
