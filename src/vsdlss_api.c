/* vsdlss_api.c — reproduction of the VSDLSS solver interface.
 *
 * Function names / roles mirror the decompiled solver
 * (solverVsdlss, vsdlss, factorA_vsdlss, forwardSubstitute_vsdlss,
 *  backSubstitute_vsdlss, solveLoadCase_vsdlss).  Bodies are a clean, correct
 * implementation of the same sparse-direct (LL^T) algorithm, built on the
 * reconstructed sparse-Cholesky core in vsdlss.c.
 */

#include "vsdlss_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ======================= factor / solve core ============================ */

vsdlss_factor *factorA_vsdlss(const vsdlss_matrix *A, int order)
{
    return vsdlss_chol_factor(A, order);
}

/* raw forward solve on the (already permuted) factor:  L y = b -> b=y */
void forwardSubstitute_vsdlss(const vsdlss_factor *F, double *b, int n)
{
    vsdlss_lsolve(F->L, b);
}

/* raw backward solve:  L' x = y -> b=x */
void backSubstitute_vsdlss(const vsdlss_factor *F, double *b, int n)
{
    vsdlss_ltsolve(F->L, b);
}

/* full load-case solve:  A x = b  (handles the fill-reducing permutation) */
void solveLoadCase_vsdlss(const vsdlss_factor *F, double *b, int n)
{
    double *x = (double *)vsdlss_malloc(n, sizeof(double));
    vsdlss_ipvec(F->pinv, b, x, n);   /* x = b in the permuted order */
    vsdlss_lsolve(F->L, x);
    vsdlss_ltsolve(F->L, x);
    vsdlss_pvec(F->pinv, x, b, n);    /* scatter back to original order */
    vsdlss_free(x);
}

int vsdlss_solve(int order, const vsdlss_matrix *A, double *b)
{
    vsdlss_factor *F = factorA_vsdlss(A, order);
    if (!F) return 0;
    solveLoadCase_vsdlss(F, b, (int)A->n);
    vsdlss_nfree(F);
    return 1;
}

/* ======================= RedHawk file-format loader ====================== */

static void die(const char *m){ fprintf(stderr,"vsdlss: %s\n", m); exit(2); }

vsdlss_matrix *vsdlss_load_matrix(const char *job, int *n, int *o)
{
    char path[512]; FILE *f;
    int nn = 0, oo = 0;
    snprintf(path, sizeof path, "%s.hdr", job);
    f = fopen(path, "r"); if (!f) die("cannot open .hdr");
    { char line[256]; while (fgets(line, sizeof line, f)) {
        int v; if (sscanf(line, "n %d", &v) == 1) nn = v;
               if (sscanf(line, "o %d", &v) == 1) oo = v;
    } }
    fclose(f);
    if (nn < 1 || oo < 0) die("bad .hdr (n/o)");
    *n = nn; *o = oo;

    double *diag = (double*)calloc(nn, 8);
    int32_t *r = (int32_t*)calloc(oo ? oo : 1, 4);
    int32_t *c = (int32_t*)calloc(oo ? oo : 1, 4);
    double  *v = (double*)calloc(oo ? oo : 1, 8);
    FILE *fd = fopen(job, "rb");
    if (fd) fclose(fd);
    snprintf(path, sizeof path, "%s.matd", job); f=fopen(path,"rb");
    if (!f || fread(diag, 8, nn, f) != (size_t)nn) die("missing/too-small .matd"); fclose(f);
    snprintf(path, sizeof path, "%s.matf", job); f=fopen(path,"rb");
    if (oo>0){ if(!f||fread(r,4,oo,f)!=(size_t)oo) die("missing .matf"); fclose(f); }
    snprintf(path, sizeof path, "%s.matt", job); f=fopen(path,"rb");
    if (oo>0){ if(!f||fread(c,4,oo,f)!=(size_t)oo) die("missing .matt"); fclose(f); }
    snprintf(path, sizeof path, "%s.mato", job); f=fopen(path,"rb");
    if (oo>0){ if(!f||fread(v,8,oo,f)!=(size_t)oo) die("missing .mato"); fclose(f); }

    csi *p = (csi*)malloc((nn+1)*sizeof(csi));
    csi *cnt = (csi*)calloc(nn+1, sizeof(csi));
    int k;
    for (k=0;k<nn;k++) cnt[k]++;
    for (k=0;k<oo;k++){ int r0=r[k]-1,c0=c[k]-1;
        if(r0>=0&&r0<nn&&c0>=0&&c0<nn&&r0!=c0){ r0<c0?cnt[c0]++:cnt[r0]++; } }
    p[0]=0; for(k=0;k<nn;k++) p[k+1]=p[k]+cnt[k];
    csi nnz=p[nn];
    csi *i = (csi*)malloc(nnz*sizeof(csi));
    double *x = (double*)calloc(nnz, sizeof(double));
    csi *pos = (csi*)malloc(nn*sizeof(csi));
    for(k=0;k<nn;k++) pos[k]=p[k];
    for(k=0;k<nn;k++){ i[pos[k]]=k; x[pos[k]]=diag[k]; pos[k]++; }
    for(k=0;k<oo;k++){ int r0=r[k]-1,c0=c[k]-1;
        if(r0>=0&&r0<nn&&c0>=0&&c0<nn&&r0!=c0){
            csi q; if(r0<c0){ q=pos[c0]++; i[q]=r0; x[q]+=v[k]; } else { q=pos[r0]++; i[q]=c0; x[q]+=v[k]; } } }

    vsdlss_matrix *A = (vsdlss_matrix*)malloc(sizeof(vsdlss_matrix));
    A->nzmax=nnz; A->m=nn; A->n=nn; A->p=p; A->i=i; A->x=x; A->nz=-1;
    free(diag); free(r); free(c); free(v); free(cnt); free(pos);
    return A;
}

/* ======================= CLI / file entry ================================ */

/* solverVsdlss(argc, argv): parse argv, read files, solve, write .rsl.
 * argv = { "vsdlss", ["-p" n] ["-S"], <jobname> , NULL }  (mirrors decompiled). */
int solverVsdlss(int argc, char **argv)
{
    int order = 1;          /* RCM ordering */
    const char *job = NULL;
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0]=='-' && argv[i][1]=='p' && i+1<argc) { order = atoi(argv[++i]); }
        else if (argv[i][0]=='-') { /* -S silent etc. */ }
        else job = argv[i];
    }
    if (!job) { fprintf(stderr,"vsdlss: no jobname\n"); return 9999; }

    int n, o;
    vsdlss_matrix *A = vsdlss_load_matrix(job, &n, &o);
    double *b = (double*)calloc(n, 8);
    char path[512]; FILE *f;
    snprintf(path, sizeof path, "%s.rhs", job); f=fopen(path,"rb");
    if (f && fread(b,8,n,f)==(size_t)n) { fclose(f); }
    else { /* b = A*ones */
        for (i=0;i<n;i++) b[i]=0;
        for (i=0;i<n;i++){ csi q; for(q=A->p[i];q<A->p[i+1];q++){ csi ii=A->i[q]; b[ii]+=A->x[q]; if(ii!=i) b[i]+=A->x[q]; } }
    }

    vsdlss_factor *F = factorA_vsdlss(A, order);
    if (!F) { fprintf(stderr,"vsdlss: factor failed\n"); return 4; }
    solveLoadCase_vsdlss(F, b, n);
    vsdlss_nfree(F);

    /* write <job>.rsl (binary doubles) */
    snprintf(path, sizeof path, "%s.rsl", job); f=fopen(path,"wb");
    if (f){ fwrite(b, 8, n, f); fclose(f); }

    printf("vsdlss (reconstructed): solved %dx%d, order=%d, o=%d\n", n, n, order, o);
    if (n <= 12){ printf("   x ="); for(i=0;i<n;i++) printf(" %.4f", b[i]); printf("\n"); }

    vsdlss_spfree(A); free(b);
    return 0;
}
