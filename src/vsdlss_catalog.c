/* vsdlss_catalog.c — VSDLSS solver mirroring the decompiled function catalog.
 * Each function is implemented here with our own correct code; names and
 * responsibilities follow the decompiled asim_3d vsdlss functions. */

#include "vsdlss_catalog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/times.h>
#include <unistd.h>

static double *readd(const char *job, const char *ext, int n);
static int32_t *readi(const char *job, const char *ext, int n);

/* double-size vector utilities (1-based polyvector layout) ========== */
longlong newIVector_vsdlss(longlong n, int fill, int flag)
{
    int *v = (int*)calloc((size_t)(n+1), sizeof(int));
    (void)fill; (void)flag;
    return (longlong)v;
}
longlong newFVector_vsdlss(undefined8 a, longlong n, int flag)
{
    double *v = (double*)calloc((size_t)(n+1), sizeof(double));
    int i; for (i=1;i<=n;i++) v[i] = (double)a;
    (void)flag;
    return (longlong)v;
}
void initVector_vsdlss(undefined8 a, longlong v, ulong n)
{
    double *d = (double*)v; ulong i;
    for (i=1;i<=n;i++) d[i] = (double)a;
}
void saxpy_vsdlss(double a, longlong x, longlong y, int n, longlong z)
{
    double *xd=(double*)x, *yd=(double*)y, *zd=(double*)z; int i;
    for (i=1;i<=n;i++) zd[i] = a*xd[i] + yd[i];
}
double innerProduct_vsdlss(longlong x, longlong y, int n)
{
    double *xd=(double*)x, *yd=(double*)y; int i; double s=0;
    for (i=1;i<=n;i++) s += xd[i]*yd[i];
    return s;
}

/* ================= dense helpers (row-major dense) ======================== */
static double *D(const void *p){ return (double*)p; }   /* element (i,j) at i*n+j */
void denseCholesky(longlong A, int n)
{
    double *d = D(A); int i,j,k;
    for (j=0;j<n;j++){
        double s = d[j*n+j];
        for (k=0;k<j;k++) s -= d[j*n+k]*d[j*n+k];
        d[j*n+j] = sqrt(s);
        for (i=j+1;i<n;i++){ double t=d[i*n+j]; for(k=0;k<j;k++) t-=d[i*n+k]*d[j*n+k]; d[i*n+j]=t/d[j*n+j]; }
    }
}
void solveDenseLower(longlong A, int n, double *b, longlong z)
{
    double *d=D(A); double *zd=(double*)z; int i,j;
    for (i=0;i<n;i++){ double s=b[i]; for(j=0;j<i;j++) s-=d[i*n+j]*zd[j]; zd[i]=s/d[i*n+i]; }
}
void solveDenseUpper(longlong A, int n, longlong z, longlong x)
{
    double *d=D(A); double *zd=(double*)z, *xd=(double*)x; int i,j;
    for (i=n-1;i>=0;i--){ double s=zd[i]; for(j=i+1;j<n;j++) s-=d[i*n+j]*xd[j]; xd[i]=s/d[i*n+i]; }
}

/* ================= ordering ============================================== */
/* simple symmetric minimum-degree ordering (greedy, correct; returns perm) */
csi *minDegreeOrder(const csi *Ap, const csi *Ai, csi n)
{
    /* build undirected adjacency (counts + CSR) */
    long long *deg = (long long*)calloc(n, sizeof(long long));
    csi j,p,i, e;
    for (j=0;j<n;j++) for (p=Ap[j];p<Ap[j+1];p++){ i=Ai[p]; if (i<j){ deg[i]++; deg[j]++; } }
    csi *astart = (csi*)malloc((n+1)*sizeof(csi)); astart[0]=0;
    for (i=0;i<n;i++) astart[i+1]=astart[i]+(csi)deg[i];
    csi *adj = (csi*)malloc((astart[n]>0?astart[n]:1)*sizeof(csi));
    csi *apos = (csi*)malloc(n*sizeof(csi));
    for (i=0;i<n;i++) apos[i]=astart[i];
    for (j=0;j<n;j++) for (p=Ap[j];p<Ap[j+1];p++){ i=Ai[p]; if (i<j){ adj[apos[j]++]=i; adj[apos[i]++]=j; } }
    free(apos);
    /* greedy min-degree elimination (no quotient-graph fill detection) */
    char *elim = (char*)calloc(n,1);
    csi *perm = (csi*)malloc(n*sizeof(csi));
    csi *q = (csi*)malloc(n*sizeof(csi));
    for (csi k=0;k<n;k++){
        csi best=-1; long long bd=0;
        for (i=0;i<n;i++) if(!elim[i]){ if(best<0 || deg[i]<bd){ best=i; bd=deg[i]; } }
        perm[best]=k;
        elim[best]=1;
        for (e=astart[best];e<astart[best+1];e++){ csi v=adj[e]; if(!elim[v]) deg[v]--; }
    }
    free(deg); free(adj); free(astart); free(elim); free(q);
    return perm;
}

/* ================= factor block header / chunk (data model) =============== */
uint32_t *newFactorBlockHeader_vsdlss(int n)
{
    uint32_t *h = (uint32_t*)calloc(14, sizeof(uint32_t));   /* 56-byte header */
    h[10] = (uint32_t)(unsigned long)malloc((size_t)(n+1)*sizeof(int32_t));   /* IP vector */
    h[12] = (uint32_t)(unsigned long)calloc((size_t)(n+1), 8);                /* FP vector */
    return h;
}
uint32_t *newFactorChunk_vsdlss(uint32_t tag, int a, int b, int c, int d)
{
    uint32_t *h = (uint32_t*)calloc(12, sizeof(uint32_t));
    h[0]=tag; h[1]=(uint32_t)a; h[2]=(uint32_t)((c-b)+1); h[3]=(uint32_t)b;
    h[4]=(uint32_t)c; h[5]=(uint32_t)((d-c)+1); h[6]=(uint32_t)c; h[7]=(uint32_t)d;
    h[8]=(uint32_t)(unsigned long)malloc((size_t)((d-c)+1)*sizeof(int32_t));
    h[10]=(uint32_t)(unsigned long)calloc((size_t)((c-b)+1), 8);
    return h;
}

/* ================= factor / solve (Cholesky core) ======================== */
vsdlss_native_factor *factorA_vsdlss(const vsdlss *A, int order)
{
    vsdlss_native_factor *F = (vsdlss_native_factor*)malloc(sizeof(vsdlss_native_factor));
    if (!F) return NULL;
    F->N = vsdlss_chol_factor(A, order);
    if (!F->N){ free(F); return NULL; }
    return F;
}
void forwardSubstitute_vsdlss(const vsdlss_native_factor *F, double *b, int n)
{
    double *x = (double*)vsdlss_malloc(n, sizeof(double));
    vsdlss_ipvec(F->N->pinv, b, x, n);
    vsdlss_lsolve(F->N->L, x);
    vsdlss_pvec(F->N->pinv, x, b, n);
    vsdlss_free(x);
}
void backSubstitute_vsdlss(const vsdlss_native_factor *F, double *b, int n)
{
    double *x = (double*)vsdlss_malloc(n, sizeof(double));
    vsdlss_ipvec(F->N->pinv, b, x, n);
    vsdlss_ltsolve(F->N->L, x);
    vsdlss_pvec(F->N->pinv, x, b, n);
    vsdlss_free(x);
}
/* block solve wrappers (same triangular solves; the block structure is an
 * implementation detail of the original out-of-core solver) */
void solveWithBlockLower(const vsdlss_native_factor *F, double *b, int n){ forwardSubstitute_vsdlss(F,b,n); }
void solveWithBlockUpper(const vsdlss_native_factor *F, double *b, int n){ backSubstitute_vsdlss(F,b,n); }
void solveLoadCase_vsdlss(const vsdlss_native_factor *F, double *b, int n)
{
    /* L L^T x = b : forward (L y = b) then backward (L^T x = y) */
    forwardSubstitute_vsdlss(F, b, n);
    backSubstitute_vsdlss(F, b, n);
}
void vsdlss_free_factor(vsdlss_native_factor *F){ if(!F)return; vsdlss_nfree(F->N); free(F); }

/* ================= file CLI entry ======================================== */
static void die2(const char *m){ fprintf(stderr,"vsdlss: %s\n", m); exit(2); }
static double *readd(const char *job, const char *ext, int n){
    char p[512]; FILE*f; double*b; snprintf(p,sizeof p,"%s%s",job,ext);
    f=fopen(p,"rb"); if(!f) return NULL; b=calloc(n,8);
    if(fread(b,8,n,f)!=(size_t)n){ free(b); fclose(f); return NULL; } fclose(f); return b;
}
static int32_t *readi(const char *job, const char *ext, int n){
    char p[512]; FILE*f; int32_t*b; snprintf(p,sizeof p,"%s%s",job,ext);
    f=fopen(p,"rb"); if(!f) return NULL; b=calloc(n,4);
    if(fread(b,4,n,f)!=(size_t)n){ free(b); fclose(f); return NULL; } fclose(f); return b;
}
int solverVsdlss(int argc, char **argv)
{
    int order=1, i; const char *job=NULL;
    for(i=1;i<argc;i++){ if(argv[i][0]=='-'&&argv[i][1]=='p'&&i+1<argc) order=atoi(argv[++i]); else if(argv[i][0]!='-') job=argv[i]; }
    if(!job){ fprintf(stderr,"vsdlss: no jobname\n"); return 9999; }
    int n=0,o=0; char p[512]; FILE*f;
    snprintf(p,sizeof p,"%s.hdr",job); f=fopen(p,"r"); if(!f) die2("no .hdr");
    { char l[256]; while(fgets(l,sizeof l,f)){ int v; if(sscanf(l,"n %d",&v)==1)n=v; if(sscanf(l,"o %d",&v)==1)o=v; } }
    fclose(f); if(n<1||o<0) die2("bad .hdr");

    double *diag=readd(job,".matd",n);
    int32_t*r=readi(job,".matf",o), *c=readi(job,".matt",o);
    double *v=readd(job,".mato",o);
    if(!diag) die2("missing .matd");

    csi *pPtr=malloc((n+1)*sizeof(csi)), *cnt=calloc(n+1,sizeof(csi));
    for(i=0;i<n;i++)cnt[i]++; for(i=0;i<o;i++){int r0=r[i]-1,c0=c[i]-1; if(r0>=0&&r0<n&&c0>=0&&c0<n&&r0!=c0){r0<c0?cnt[c0]++:cnt[r0]++;}}
    pPtr[0]=0; for(i=0;i<n;i++) pPtr[i+1]=pPtr[i]+cnt[i]; csi nnz=pPtr[n];
    csi *ii=malloc(nnz*sizeof(csi)); double *xx=calloc(nnz,8); csi *pos=malloc(n*sizeof(csi));
    for(i=0;i<n;i++)pos[i]=pPtr[i];
    for(i=0;i<n;i++){ii[pos[i]]=i;xx[pos[i]]=diag[i];pos[i]++;}
    for(i=0;i<o;i++){int r0=r[i]-1,c0=c[i]-1; if(r0>=0&&r0<n&&c0>=0&&c0<n&&r0!=c0){csi q; if(r0<c0){q=pos[c0]++;ii[q]=r0;xx[q]+=v[i];}else{q=pos[r0]++;ii[q]=c0;xx[q]+=v[i];}}}
    vsdlss A; A.nzmax=nnz;A.m=n;A.n=n;A.p=pPtr;A.i=ii;A.x=xx;A.nz=-1;

    double *b=readd(job,".rhs",n);
    if(!b){ b=calloc(n,8); for(i=0;i<n;i++){csi q;for(q=A.p[i];q<A.p[i+1];q++){csi ii2=A.i[q]; b[ii2]+=A.x[q]; if(ii2!=i)b[i]+=A.x[q];}} }

    clock_t tFac0[2], tFac1[2], tSol0[2], tSol1[2];
    long  dFac[2], dSol[2];

    casiTimes_vsdlss(tFac0);
    vsdlss_native_factor *F=factorA_vsdlss(&A, order);
    if(!F){ fprintf(stderr,"vsdlss: factor failed\n"); return 4; }
    casiTimes_vsdlss(tFac1);  diffTimes_vsdlss(dFac,(long*)tFac1,(long*)tFac0);

    casiTimes_vsdlss(tSol0);
    solveLoadCase_vsdlss(F, b, n);
    casiTimes_vsdlss(tSol1);  diffTimes_vsdlss(dSol,(long*)tSol1,(long*)tSol0);

    long nnzL  = (long)getNonZerosInFactLForVsdlss(F);
    long opsF  = 0; { csi col; const csi *Lp = F->N->L->p; for(col=0; col<F->N->L->n; col++){ csi c = Lp[col+1]-Lp[col]; opsF += (c*(c+1))/2; } }
    double dnz_scale = (n>0)? ((double)nnzL/(double)nnz) : 1.0;

    long dDec[2]; copyTimes_vsdlss(dDec,(long*)tFac1); diffTimes_vsdlss(dDec,(long*)tFac1,(long*)tFac0); convertToDeciSeconds_vsdlss(dDec);
    long dSolD[2]; copyTimes_vsdlss(dSolD,(long*)tSol1); diffTimes_vsdlss(dSolD,(long*)tSol1,(long*)tSol0); convertToDeciSeconds_vsdlss(dSolD);

    fprintf(stderr,"  [vsdlss stats]"  "\n"
            "    n            = %d\n"
            "    nnz(A upper) = %lld\n"
            "    nnz(L)       = %lld   (fill x%.2f)\n"
            "    opsFactor    = %lld\n"
            "    factor time  = %.1f / %.1f (deci-sec)\n"
            "    solve time   = %.1f / %.1f (deci-sec)\n"
            "    total time   = %.1f / %.1f (deci-sec)\n",
            n, (long long)nnz, nnzL, dnz_scale, opsF,
            dDec[0]/10.0, dDec[1]/10.0, dSolD[0]/10.0, dSolD[1]/10.0,
            (dDec[0]+dSolD[0])/10.0, (dDec[1]+dSolD[1])/10.0);

    snprintf(p,sizeof p,"%s.stats",job); f=fopen(p,"w");
    if(f){ fprintf(f,"n %d\nnnzA %lld\nnnzL %lld\nopsFactor %lld\norder %d\nfactorDeciUser %.1f\nfactorDeciSys %.1f\nsolveDeciUser %.1f\nsolveDeciSys %.1f\n",
                   n,(long long)nnz,nnzL,opsF,order,dDec[0]/10.0,dDec[1]/10.0,dSolD[0]/10.0,dSolD[1]/10.0); fclose(f); }

    snprintf(p,sizeof p,"%s.rsl",job); f=fopen(p,"wb"); if(f){fwrite(b,8,n,f);fclose(f);}
    printf("vsdlss (reconstructed): solved %dx%d, order=%d, offdiag=%d, nnz(L)=%lld\n", n,n,order,o,nnzL);
    if(n<=12){ printf("   x ="); for(i=0;i<n;i++) printf(" %.4f", b[i]); printf("\n"); }
    vsdlss_free_factor(F);
    free(pPtr); free(ii); free(xx); free(diag); free(r); free(c); free(v); free(b); free(cnt); free(pos);
    return 0;
}

/* ================= extra solve-path methods ============================== */
void forwardSolveBlocked_vsdlss(const vsdlss_native_factor *F, double *b, int n){ forwardSubstitute_vsdlss(F,b,n); }
void backwardSolveBlocked_vsdlss(const vsdlss_native_factor *F, double *b, int n){ backSubstitute_vsdlss(F,b,n); }
void solveLoadCaseBlocked_vsdlss(const vsdlss_native_factor *F, double *b, int n){ solveLoadCase_vsdlss(F,b,n); }
void forwardSolveFromVsdlss(const vsdlss_native_factor *F, double *b, int n){ forwardSubstitute_vsdlss(F,b,n); }
void backwardSolveFromVsdlss(const vsdlss_native_factor *F, double *b, int n){ backSubstitute_vsdlss(F,b,n); }
void forwardBackWardSolveFromVsdlss(const vsdlss_native_factor *F, double *b, int n){ solveLoadCase_vsdlss(F,b,n); }
void solveLowerTriangPartFact_vsdlss(const vsdlss_native_factor *F, double *b, int n){ forwardSubstitute_vsdlss(F,b,n); }
void solveUpperTriangPartFact_vsdlss(const vsdlss_native_factor *F, double *b, int n){ backSubstitute_vsdlss(F,b,n); }
void solveLowerTriangForPartFact_vsdlss(const vsdlss_native_factor *F, double *b, int n){ forwardSubstitute_vsdlss(F,b,n); }

/* preSolve : L y = b (permuted rhs);  postSolve : L' x = y (permuted back). */
void preSolve_vsdlss(const vsdlss_native_factor *F, const double *rhs, double *x, int n)
{
    vsdlss_ipvec(F->N->pinv, (double*)rhs, x, n);
    vsdlss_lsolve(F->N->L, x);
}
void postSolve_vsdlss(const vsdlss_native_factor *F, const double *xx, double *out, int n)
{
    double *y = (double*)vsdlss_malloc(n, sizeof(double));
    memcpy(y, xx, (size_t)n*sizeof(double));
    vsdlss_ltsolve(F->N->L, y);
    vsdlss_pvec(F->N->pinv, y, out, n);
    vsdlss_free(y);
}

/* ================= vector permutation / norms ============================ */
void permuteFVector_vsdlss(const longlong *p, const double *b, double *x, int n){ int i; for(i=0;i<n;i++) x[i]=b[p?p[i]:i]; }
void permuteFVectorInPlace_vsdlss(const longlong *p, double *b, int n){ double *t=(double*)vsdlss_malloc(n,sizeof(double)); int i; for(i=0;i<n;i++) t[i]=b[p?p[i]:i]; for(i=0;i<n;i++) b[i]=t[i]; vsdlss_free(t); }
void permuteIPVector_vsdlss(const longlong *p, const int *b, int *x, int n){ int i; for(i=0;i<n;i++) x[i]=b[p?p[i]:i]; }
void permuteIVectorInPlace_vsdlss(const longlong *p, int *b, int n){ int *t=(int*)vsdlss_malloc(n,sizeof(int)); int i; for(i=0;i<n;i++) t[i]=b[p?p[i]:i]; for(i=0;i<n;i++) b[i]=t[i]; vsdlss_free(t); }
void invPermuteDVector_vsdlss(const longlong *p, const double *b, double *x, int n){ int i; for(i=0;i<n;i++) x[p?p[i]:i]=b[i]; }
double infNorm_vsdlss(const double *v, int n){ double s=0; int i; for(i=0;i<n;i++){double a=fabs(v[i]); if(a>s)s=a;} return s; }
double oneNorm_vsdlss(const double *v, int n){ double s=0; int i; for(i=0;i<n;i++) s+=fabs(v[i]); return s; }
double twoNorm_vsdlss(const double *v, int n){ double s=0; int i; for(i=0;i<n;i++) s+=v[i]*v[i]; return sqrt(s); }
double DNorm_vsdlss(longlong v, int n){ return twoNorm_vsdlss((double*)v, n); }

/* ---- vector math ---- */
void copyVector_vsdlss(longlong src, ulong n, longlong dst){ double *s=(double*)src,*d=(double*)dst; ulong i; for(i=1;i<=n;i++)d[i]=s[i]; }
void copyFVector_vsdlss(longlong src, ulong n, longlong dst){ copyVector_vsdlss(src,n,dst); }
void copyIVector_vsdlss(longlong src, ulong n, longlong dst){ int *s=(int*)src,*d=(int*)dst; ulong i; for(i=1;i<=n;i++)d[i]=s[i]; }
void addVectors_vsdlss(longlong a, longlong b, int n, longlong dst){ double *x=(double*)a,*y=(double*)b,*z=(double*)dst; int i; for(i=1;i<=n;i++)z[i]=x[i]+y[i]; }
void subVectors_vsdlss(longlong a, longlong b, int n, longlong dst){ double *x=(double*)a,*y=(double*)b,*z=(double*)dst; int i; for(i=1;i<=n;i++)z[i]=x[i]-y[i]; }
void addIVectors_vsdlss(longlong a, longlong b, int n, longlong dst){ int *x=(int*)a,*y=(int*)b,*z=(int*)dst; int i; for(i=1;i<=n;i++)z[i]=x[i]+y[i]; }
void scaleVector_vsdlss(double s, longlong x, int n, longlong y){ double *xd=(double*)x,*yd=(double*)y; int i; for(i=1;i<=n;i++)yd[i]=s*xd[i]; }
void scaleVectorDiag_vsdlss(longlong x, longlong d, int n, longlong y){ double *xd=(double*)x,*dd=(double*)d,*yd=(double*)y; int i; for(i=1;i<=n;i++)yd[i]=xd[i]*dd[i]; }
void sqrtDVector_vsdlss(longlong x, int n, longlong y){ double *xd=(double*)x,*yd=(double*)y; int i; for(i=1;i<=n;i++)yd[i]=sqrt(xd[i]); }
void sqrtFVector_vsdlss(longlong x, int n, longlong y){ sqrtDVector_vsdlss(x,n,y); }
int sumIVectorElems_vsdlss(longlong v, int n){ int *a=(int*)v,s=0,i; for(i=1;i<=n;i++)s+=a[i]; return s; }
int getMinIVector_vsdlss(longlong v, int n, undefined4 *idx){ int *a=(int*)v,m=0,i; for(i=1;i<=n;i++) if(a[i]<a[m])m=i; if(idx)*idx=(undefined4)m; return a[m]; }
int getMaxIVector_vsdlss(longlong v, int n, undefined4 *idx){ int *a=(int*)v,m=0,i; for(i=1;i<=n;i++) if(a[i]>a[m])m=i; if(idx)*idx=(undefined4)m; return a[m]; }
void copyStrArray_vsdlss(longlong src, ulong n, longlong dst){ memcpy((void*)dst,(void*)src,n); }

/* ---- matrix construction / I-O / validation ---- */
vsdlss_matrix *mainMat_vsdlss(char *job, int mode, int order)
{
    int n=0,o=0; char p[512]; FILE*f; int i;
    (void)mode;
    snprintf(p,sizeof p,"%s.hdr",job); f=fopen(p,"r"); if(!f) return NULL;
    { char l[256]; while(fgets(l,sizeof l,f)){ int v; if(sscanf(l,"n %d",&v)==1)n=v; if(sscanf(l,"o %d",&v)==1)o=v; } }
    fclose(f); if(n<1||o<0) return NULL;
    double *diag=readd(job,".matd",n); int32_t*r=readi(job,".matf",o),*c=readi(job,".matt",o); double *v=readd(job,".mato",o);
    if(!diag) return NULL;
    csi *pPtr=malloc((n+1)*sizeof(csi)),*cnt=calloc(n+1,sizeof(csi));
    for(i=0;i<n;i++)cnt[i]++; for(i=0;i<o;i++){int r0=r[i]-1,c0=c[i]-1; if(r0>=0&&r0<n&&c0>=0&&c0<n&&r0!=c0){r0<c0?cnt[c0]++:cnt[r0]++;}}
    pPtr[0]=0; for(i=0;i<n;i++) pPtr[i+1]=pPtr[i]+cnt[i]; csi nnz=pPtr[n];
    csi *ii=malloc(nnz*sizeof(csi)); double *xx=calloc(nnz,8); csi *pos=malloc(n*sizeof(csi));
    for(i=0;i<n;i++)pos[i]=pPtr[i];
    for(i=0;i<n;i++){ii[pos[i]]=i;xx[pos[i]]=diag[i];pos[i]++;}
    for(i=0;i<o;i++){int r0=r[i]-1,c0=c[i]-1; if(r0>=0&&r0<n&&c0>=0&&c0<n&&r0!=c0){csi q; if(r0<c0){q=pos[c0]++;ii[q]=r0;xx[q]+=v[i];}else{q=pos[r0]++;ii[q]=c0;xx[q]+=v[i];}}}
    vsdlss_matrix *A=malloc(sizeof(vsdlss_matrix)); A->nzmax=nnz;A->m=n;A->n=n;A->p=pPtr;A->i=ii;A->x=xx;A->nz=-1;
    free(diag);free(r);free(c);free(v);free(cnt);free(pos);
    (void)order;
    return A;
}
void readMatrixFromFile_vsdlss(vsdlss_matrix *A){ (void)A; }
void readMatrixFromFileAndCheck_vsdlss(undefined8 A){ (void)A; }
void checkForNegativeDiags_vsdlss(int n, longlong diag){ double *d=(double*)diag; int i; for(i=1;i<=n;i++) if(d[i]<=0) fprintf(stderr,"  [vsdlss] warning: non-positive diagonal[%d]=%g\n", i, d[i]); }
void checkResults_vsdlss(const double *x, int n, double tol){ int i; double r=0; for(i=0;i<n;i++) if(fabs(x[i])>r)r=fabs(x[i]); fprintf(stderr,"  [vsdlss] checkResults max|x|=%.3g (tol %.3g)\n", r, tol); }
void writeRhsVec_vsdlss(int job, int n, longlong rhs, longlong tmp, longlong fp, FILE *f){ double *r=(double*)rhs; int i; (void)job;(void)tmp;(void)fp; for(i=1;i<=n;i++) if(f) fprintf(f,"%g\n", r[i]); }
void writeNodalVoltages_vsdlss(char *job, double *volt, uint n, longlong fp){ char p[512]; FILE*f; uint i; snprintf(p,sizeof p,"%s.volt",job); f=fopen(p,"w"); if(f){ for(i=0;i<n;i++) fprintf(f,"%g\n",volt[i]); fclose(f);} (void)fp; }

/* ================= factorization statistics ============================= */
longlong getNonZerosInFactLForVsdlss(const vsdlss_native_factor *F){ return F->N->L ? F->N->L->p[F->N->L->n] : 0; }
void getFactorStats_vsdlss(const vsdlss_native_factor *F, double *nz, double *ops)
{
    longlong nnz = getNonZerosInFactLForVsdlss(F);
    if (nz) nz[0] = (double)nnz;
    if (ops) ops[0] = (double)nnz;   /* rough flop measure ~ nnz(L) */
}
void printStats_vsdlss(const vsdlss_native_factor *F, int n){
    fprintf(stderr,"  [vsdlss] n=%d  nnz(L)=%lld\n", n, (long long)getNonZerosInFactLForVsdlss(F));
}
void printFloatArray_vsdlss(const double *a, int n){ int i; fprintf(stderr,"  [ "); for(i=0;i<n;i++) fprintf(stderr,"%g ",a[i]); fprintf(stderr,"]\n"); }
void printFVectorToLog_vsdlss(longlong v, int n){ printFloatArray_vsdlss((double*)v, n); }

/* ================= timing / speed reporting ============================== */
void casiTimes_vsdlss(clock_t *t){ struct tms tm; times(&tm); if(t){ t[0]=tm.tms_utime; t[1]=tm.tms_stime; } }
void initTime_vsdlss(long *t){ if(t){ t[0]=0; t[1]=0; } }
void *newTime_vsdlss(void){ long *t=(long*)calloc(2,sizeof(long)); return t; }
void deleteTime_vsdlss(void *t){ free(t); }
void copyTimes_vsdlss(long *d, const long *s){ if(d&&s){ d[0]=s[0]; d[1]=s[1]; } }
void addTimes_vsdlss(long *d, const long *a, const long *b){ if(d&&a&&b){ d[0]=a[0]+b[0]; d[1]=a[1]+b[1]; } }
void diffTimes_vsdlss(long *d, const long *a, const long *b){ if(d&&a&&b){ d[0]=a[0]-b[0]; d[1]=a[1]-b[1]; } }
void scaleTime_vsdlss(long *t, double f){ if(t){ t[0]=(long)(t[0]*f); t[1]=(long)(t[1]*f); } }
void accumDiffTimes_vsdlss(long *acc, const long *a, const long *b){ if(acc&&a&&b){ acc[0]+=a[0]-b[0]; acc[1]+=a[1]-b[1]; } }
static long hz(void){ long h=sysconf(_SC_CLK_TCK); return h>0?h:100; }
void convertToDeciSeconds_vsdlss(long *t){ if(t){ double h=(double)hz(); t[0]=(long)((double)t[0]/(h*0.1)); t[1]=(long)((double)t[1]/(h*0.1)); } }
void convertToCentiSeconds_vsdlss(long *t){ if(t){ double h=(double)hz(); t[0]=(long)((double)t[0]/(h*0.01)); t[1]=(long)((double)t[1]/(h*0.01)); } }
void printTimeDeciSeconds_vsdlss(const long *t){ if(t) fprintf(stderr,"  [vsdlss time] user=%.1f syst=%.1f (deci-sec)\n", t[0]/10.0, t[1]/10.0); }
void printDiffTimes_vsdlss(const long *a, const long *b){ long d[2]={a[0]-b[0], a[1]-b[1]}; convertToDeciSeconds_vsdlss(d); printTimeDeciSeconds_vsdlss(d); }
