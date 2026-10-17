/* Shared input of the solve-phase benchmarks (bench_phase, bench_overlap):
 * the system from a PG_DUMP file, a grid of chains, or the text files of
 * examples/transient_driver, and optional NUMA interleaving.  Include after
 * the system headers (needs _GNU_SOURCE for getline). */
#ifndef VSDLSS_BENCH_INPUT_H
#define VSDLSS_BENCH_INPUT_H

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

/* "dump.bin" | "grid:SIDE[:CHAIN]" | "text:DIR" */
static vsdlss *read_input(const char *spec)
{
    if (!strncmp(spec, "grid:", 5)) {
        csi side = atol(spec + 5), chain = 3; const char *c = strchr(spec + 5, ':');
        if (c) chain = atol(c + 1);
        return grid_chains(side, chain);
    }
    if (!strncmp(spec, "text:", 5)) {
        double t = now(); vsdlss *A = read_text(spec + 5);
        if (A) printf("# read %s in %.1f s: n %lld, nnz %lld\n", spec + 5, now() - t, (long long)A->n, (long long)A->p[A->n]);
        return A;
    }
    return read_dump(spec);
}

#endif
