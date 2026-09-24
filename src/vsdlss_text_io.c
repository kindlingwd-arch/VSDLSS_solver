#define _POSIX_C_SOURCE 200809L
#include "vsdlss_text_io.h"

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

static int path_for(char *out, size_t cap, const char *dir, const char *name)
{
    int n = snprintf(out, cap, "%s/%s", dir, name);
    return n >= 0 && (size_t)n < cap;
}

static int next_record(FILE *f, char **line, size_t *cap, size_t *number)
{
    ssize_t length;
    while ((length = getline(line, cap, f)) >= 0) {
        char *p = *line;
        (void)length;
        ++*number;
        while (isspace((unsigned char)*p)) ++p;
        if (*p && *p != '#') return 1;
    }
    return ferror(f) ? -1 : 0;
}

static int parse_integer(char **cursor, int64_t *out)
{
    char *end;
    errno = 0;
    *out = strtoll(*cursor, &end, 10);
    if (errno || end == *cursor) return 0;
    *cursor = end;
    return 1;
}

static int parse_double(char **cursor, double *out)
{
    char *end;
    errno = 0;
    *out = strtod(*cursor, &end);
    if (errno || end == *cursor || !isfinite(*out)) return 0;
    *cursor = end;
    return 1;
}

static int end_record(char *p)
{
    while (isspace((unsigned char)*p)) ++p;
    return *p == '\0';
}

static int diagonal_record(char *p, csi index, double *value)
{
    char *end;
    int64_t row;
    errno = 0;
    /* A single floating point value is allowed; a pair is index then value. */
    *value = strtod(p, &end);
    if (errno || end == p || !isfinite(*value)) return 0;
    if (end_record(end)) return 1;
    if (!parse_integer(&p, &row) || row != index ||
        !parse_double(&p, value) || !end_record(p)) return 0;
    return 1;
}

static vsdlss_status read_diagonal(const char *path, double **values, csi *n)
{
    FILE *f = fopen(path, "r");
    char *line = NULL;
    size_t cap = 0, number = 0, allocated = 0;
    int found;
    vsdlss_status status = VSDLSS_OK;
    if (!f) { fprintf(stderr, "%s: cannot open\n", path); return VSDLSS_ERR_IO; }
    while ((found = next_record(f, &line, &cap, &number)) == 1) {
        double value;
        if (*n == INT64_MAX || !diagonal_record(line, *n, &value)) {
            fprintf(stderr, "%s:%zu: invalid diagonal/index\n", path, number);
            status = VSDLSS_ERR_INVALID; break;
        }
        if ((size_t)*n == allocated) {
            size_t next = allocated ? allocated * 2 : 16;
            double *grown;
            if (next <= allocated || next > SIZE_MAX / sizeof(double)) {
                status = VSDLSS_ERR_OOM; break;
            }
            grown = realloc(*values, next * sizeof(double));
            if (!grown) { status = VSDLSS_ERR_OOM; break; }
            *values = grown; allocated = next;
        }
        (*values)[(*n)++] = value;
    }
    if (found < 0) status = VSDLSS_ERR_IO;
    if (status == VSDLSS_OK && !*n) status = VSDLSS_ERR_INVALID;
    free(line); fclose(f);
    return status;
}

static int edge_record(char *p, csi n, csi *row, csi *col, double *value)
{
    int64_t r, c;
    if (!parse_integer(&p, &r) || !parse_integer(&p, &c) ||
        !parse_double(&p, value) || !end_record(p) ||
        r < 0 || c <= r || c >= n) return 0;
    *row = r; *col = c;
    return 1;
}

static vsdlss_status load_edges(const char *path, vsdlss *A, const double *diagonal)
{
    FILE *f;
    char *line = NULL;
    size_t cap = 0, number = 0;
    csi row, col, edges = 0, *next = NULL;
    double value;
    int found, pass;
    vsdlss_status status = VSDLSS_OK;
    for (pass = 0; pass < 2; ++pass) {
        number = 0;
        f = fopen(path, "r");
        if (!f) { fprintf(stderr, "%s: cannot open\n", path); status = VSDLSS_ERR_IO; break; }
        while ((found = next_record(f, &line, &cap, &number)) == 1) {
            csi at;
            if (!edge_record(line, A->n, &row, &col, &value)) {
                fprintf(stderr, "%s:%zu: expected unique upper entry row col value\n", path, number);
                status = VSDLSS_ERR_INVALID; break;
            }
            if (!pass) {
                if (edges >= INT64_MAX - A->n || A->p[col + 1] == INT64_MAX) {
                    status = VSDLSS_ERR_INVALID; break;
                }
                ++edges; ++A->p[col + 1];
            } else {
                if (next[col] >= A->p[col + 1]) {
                    fprintf(stderr, "%s: data changed during loading\n", path);
                    status = VSDLSS_ERR_IO; break;
                }
                at = next[col]++;
                A->i[at] = row; A->x[at] = value;
            }
        }
        if (found < 0) status = VSDLSS_ERR_IO;
        fclose(f);
        if (status != VSDLSS_OK) break;
        if (pass) {
            csi j;
            for (j = 0; j < A->n; ++j)
                if (next[j] != A->p[j + 1]) {
                    fprintf(stderr, "%s: data changed during loading\n", path);
                    status = VSDLSS_ERR_IO; break;
                }
            if (status != VSDLSS_OK) break;
        }
        if (!pass) {
            csi col_index;
            for (col_index = 0; col_index < A->n; ++col_index)
                A->p[col_index + 1] += A->p[col_index] + 1;
            if (!vsdlss_sprealloc(A, A->n + edges)) { status = VSDLSS_ERR_OOM; break; }
            next = malloc((size_t)A->n * sizeof(csi));
            if (!next) { status = VSDLSS_ERR_OOM; break; }
            for (col_index = 0; col_index < A->n; ++col_index) {
                A->i[A->p[col_index]] = col_index;
                A->x[A->p[col_index]] = diagonal[col_index];
                next[col_index] = A->p[col_index] + 1;
            }
        }
    }
    free(next); free(line);
    return status;
}

static vsdlss_status read_vector(const char *path, csi n, double **out)
{
    FILE *f = fopen(path, "r");
    char *line = NULL, *p;
    size_t cap = 0, number = 0;
    csi k = 0;
    int found;
    vsdlss_status status = VSDLSS_OK;
    if (!f) { fprintf(stderr, "%s: cannot open\n", path); return VSDLSS_ERR_IO; }
    *out = malloc((size_t)n * sizeof(double));
    if (!*out) { fclose(f); return VSDLSS_ERR_OOM; }
    while ((found = next_record(f, &line, &cap, &number)) == 1) {
        int64_t idx;
        p = line;
        if (k >= n || !parse_integer(&p, &idx) || idx != k ||
            !parse_double(&p, &(*out)[k]) || !end_record(p)) {
            fprintf(stderr, "%s:%zu: expected sequential index value\n", path, number);
            status = VSDLSS_ERR_INVALID; break;
        }
        ++k;
    }
    if (found < 0) status = VSDLSS_ERR_IO;
    if (status == VSDLSS_OK && k != n) {
        fprintf(stderr, "%s: expected %" PRId64 " entries, got %" PRId64 "\n", path, n, k);
        status = VSDLSS_ERR_INVALID;
    }
    free(line); fclose(f);
    return status;
}

vsdlss_status vsdlss_load_text_dump(const char *directory, vsdlss **A,
                                    double **rhs, double **reference)
{
    char path[4096];
    double *diagonal = NULL, *b = NULL, *x = NULL;
    csi n = 0;
    vsdlss *raw = NULL, *normalized = NULL;
    vsdlss_status status = VSDLSS_ERR_INVALID;
    if (!directory || !*directory || !A || !rhs || !reference) return status;
    *A = NULL; *rhs = NULL; *reference = NULL;
    if (!path_for(path, sizeof path, directory, "diag.txt")) goto done;
    status = read_diagonal(path, &diagonal, &n);
    if (status != VSDLSS_OK) goto done;
    raw = vsdlss_spalloc(n, n, n, 1, 0);
    if (!raw) { status = VSDLSS_ERR_OOM; goto done; }
    /* spalloc does not initialize column pointers; first pass counts edges. */
    memset(raw->p, 0, ((size_t)n + 1) * sizeof(csi));
    if (!path_for(path, sizeof path, directory, "data.txt")) { status = VSDLSS_ERR_INVALID; goto done; }
    status = load_edges(path, raw, diagonal);
    if (status != VSDLSS_OK) goto done;
    status = vsdlss_normalize_upper(raw, &normalized);
    if (status != VSDLSS_OK) goto done;
    if (normalized->p[n] != raw->p[n]) {
        fprintf(stderr, "%s: duplicate matrix entry\n", path);
        status = VSDLSS_ERR_INVALID; goto done;
    }
    if (!path_for(path, sizeof path, directory, "b_vector.txt")) { status = VSDLSS_ERR_INVALID; goto done; }
    status = read_vector(path, n, &b);
    if (status != VSDLSS_OK) goto done;
    if (!path_for(path, sizeof path, directory, "x_vector.txt")) { status = VSDLSS_ERR_INVALID; goto done; }
    status = read_vector(path, n, &x);
    if (status != VSDLSS_OK) goto done;
    *A = normalized; normalized = NULL;
    *rhs = b; b = NULL;
    *reference = x; x = NULL;
done:
    free(diagonal); free(b); free(x);
    vsdlss_spfree(raw); vsdlss_spfree(normalized);
    return status;
}
