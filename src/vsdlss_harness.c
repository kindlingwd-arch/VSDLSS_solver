/* vsdlss_harness.c
 *
 * Reads a VSDLSS matrix in the RedHawk file format (the same files that the
 * decompiled mainMat_vsdlss reads), builds the symmetric matrix, and solves
 * A x = b with the reconstructed vsdlss sparse Cholesky solver.
 *
 * Files (jobname = <job>, all under the same dir):
 *   <job>.hdr   : text, KEY VALUE lines: n(rows) o(offdiag) m a k z r c x u ...
 *   <job>.matd  : binary, n doubles (diagonal)
 *   <job>.matf  : binary, o int32 (row index of off-diagonals, 1-based)
 *   <job>.matt  : binary, o int32 (col index of off-diagonals, 1-based)
 *   <job>.mato  : binary, o doubles (off-diagonal values)
 *   <job>.rhs   : binary, n doubles (right-hand side), optional
 *
 * Usage: ./vsdlss_harness <job> [order]
 */

#include "vsdlss.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

static void die(const char *m){ fprintf(stderr,"harness: %s\n", m); exit(2); }

static void read_hdr(const char *job, int *n, int *o)
{
    char path[512]; FILE *f;
    snprintf(path, sizeof path, "%s.hdr", job);
    f = fopen(path, "r"); if(!f) die("cannot open .hdr");
    char line[256];
    while (fgets(line, sizeof line, f)) {
        char k; int v; char c;
        if (sscanf(line, "n %d", &v) == 1) *n = v;
        if (sscanf(line, "o %d", &v) == 1) *o = v;
    }
    fclose(f);
    if (*n < 1 || *o < 0) die("bad .hdr (n/o)");
}

static double *read_bin(const char *job, const char *ext, size_t cnt, size_t sz)
{
    char path[512]; FILE *f; double *buf;
    snprintf(path, sizeof path, "%s%s", job, ext);
    f = fopen(path, "rb"); if(!f) return NULL;
    buf = calloc(cnt ? cnt : 1, sz);
    if (fread(buf, sz, cnt, f) != cnt) { free(buf); fclose(f); return NULL; }
    fclose(f);
    return buf;
}

int main(int argc, char **argv)
{
    if (argc < 2) { fprintf(stderr,"usage: %s <job> [order]\n", argv[0]); return 1; }
    const char *job = argv[1];
    int order = argc > 2 ? atoi(argv[2]) : 1;

    int n = 0, o = 0;
    read_hdr(job, &n, &o);

    double *diag = read_bin(job, ".matd", n, 8);
    int32_t *r   = (int32_t*)read_bin(job, ".matf", o, 4);
    int32_t *c   = (int32_t*)read_bin(job, ".matt", o, 4);
    double  *v   = read_bin(job, ".mato", o, 8);
    if (!diag) die("missing/too-small .matd");
    if (o>0 && (!r || !c || !v)) die("missing off-diagonal file");

    /* Build upper-CSC symmetric matrix A.
       Column j holds rows i<=j.  Add diag + mirrored off-diagonals. */
    csi *p    = malloc((n+1)*sizeof(csi));
    /* first count per column */
    csi *cnt  = calloc(n+1, sizeof(csi));
    int k;
    for (k=0;k<n;k++) cnt[k]++;                    /* diagonal */
    for (k=0;k<o;k++){
        int r0 = r[k]-1, c0 = c[k]-1;              /* 0-based */
        if (r0>=0 && r0<n && c0>=0 && c0<n && r0!=c0){
            if (r0 < c0) cnt[c0]++;                 /* (r0,c0) upper */
            else         cnt[r0]++;                 /* (c0,r0) upper */
        }
    }
    p[0]=0; for(k=0;k<n;k++) p[k+1]=p[k]+cnt[k];
    csi nnz = p[n];
    csi *i   = malloc(nnz*sizeof(csi));
    double *x= calloc(nnz, sizeof(double));
    csi *pos = malloc(n*sizeof(csi));
    for(k=0;k<n;k++) pos[k]=p[k];
    for(k=0;k<n;k++){ i[pos[k]]=k; x[pos[k]]=diag[k]; pos[k]++; }
    for(k=0;k<o;k++){
        int r0=r[k]-1, c0=c[k]-1;
        if(r0>=0&&r0<n&&c0>=0&&c0<n&&r0!=c0){
            if(r0<c0){ csi q=pos[c0]++; i[q]=r0; x[q]+=v[k]; }
            else     { csi q=pos[r0]++; i[q]=c0; x[q]+=v[k]; }
        }
    }
    for(k=0;k<n;k++) if (pos[k]!=p[k+1]) { fprintf(stderr,"internal fill mismatch\n"); return 3; }

    vsdlss A;
    A.nzmax=nnz; A.m=n; A.n=n; A.p=p; A.i=i; A.x=x; A.nz=-1;

    printf("vsdlss (reconstructed) solve: n=%d  nnz(upper)=%lld  offdiag=%d\n",
           n, (long long)nnz, o);

    /* RHS: use .rhs if present (n doubles), else b = A*ones (=> x = ones) */
    double *b = read_bin(job, ".rhs", n, 8);
    if (!b) {
        b = calloc(n,8);
        csi j, q;
        for(j=0;j<n;j++){ double s=0; for(q=A.p[j];q<A.p[j+1];q++){ csi ii=A.i[q]; s+=A.x[q]; if(ii!=j)s+=A.x[q]; } b[j]=s; }
        /* multiply correctly: y = A*ones */
        for(j=0;j<n;j++) b[j]=0;
        for(j=0;j<n;j++) for(q=A.p[j];q<A.p[j+1];q++){ csi ii=A.i[q]; b[ii]+=A.x[q]; if(ii!=j) b[j]+=A.x[q]; }
    }

    int ok = vsdlss_cholsolve(order, &A, b);   /* b -> x in place */

    /* residual ||A x - b_orig||_inf */
    double *b0 = read_bin(job,".rhs",n,8);
    if(!b0){ b0=calloc(n,8); for(int j=0;j<n;j++){ double s=0; for(csi q=A.p[j];q<A.p[j+1];q++){ csi ii=A.i[q]; s+=A.x[q]; } b0[j]=s; } }
    double *Ax = calloc(n,8);
    for(int j=0;j<n;j++) for(csi q=A.p[j];q<A.p[j+1];q++){ csi ii=A.i[q]; Ax[ii]+=A.x[q]*b[j]; if(ii!=j) Ax[j]+=A.x[q]*b[j]; }
    double resid=0, maxx=0; for(int j=0;j<n;j++){ double e=fabs(Ax[j]-b0[j]); if(e>resid)resid=e; if(fabs(b[j])>maxx)maxx=fabs(b[j]); }

    printf("solve ok=%d  ||A x - b||_inf = %.3e   max|x| = %.3e\n", ok, resid, maxx);
    if (n <= 12) { printf("   x ="); for(int j=0;j<n;j++) printf(" %.4f", b[j]); printf("\n"); }

    free(diag); free(r); free(c); free(v); free(p); free(i); free(x); free(cnt); free(pos); free(Ax); free(b0);
    return 0;
}
