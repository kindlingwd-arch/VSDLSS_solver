/* Compare packed panels with the unchanged full-layout kernels.  The BLAS
 * wrapper also checks dispatch boundaries: numerical agreement alone would
 * not detect an option that never selected the optimized path. */
#include "../src/vsdlss_m3_internal.h"
#include <stdio.h>
#include <string.h>

#ifdef VSDLSS_BLAS
static int packed_calls;
void __real_dtpsv_(const char *, const char *, const char *, const int *,
                  const double *, double *, const int *);
void __wrap_dtpsv_(const char *u, const char *t, const char *d, const int *n,
                  const double *a, double *x, const int *inc)
{
    packed_calls++;
    __real_dtpsv_(u,t,d,n,a,x,inc);
}
#endif

static unsigned seed = 9981;
static double rnd(void)
{
    seed = 1664525u * seed + 1013904223u;
    return (seed >> 8) / 16777216.0 - 0.5;
}

int main(void)
{
    const int widths[] = {1,6,7,8,15,16,17,31,32,63,64,127,128,255,512};
    const int exts[] = {0,1,15,16,63,64,255,1024,1025};
    int count = 0;
    for (unsigned wi=0; wi<sizeof(widths)/sizeof(*widths); wi++)
    for (unsigned ei=0; ei<sizeof(exts)/sizeof(*exts); ei++)
    for (int back=0; back<2; back++)
    for (int inject=0; inject<4; inject++) {
        csi w=widths[wi], e=exts[ei], begin=3, rows=w+e;
        csi n=begin+w+3*e+8, len=w*(w+1)/2+w*e;
        double *a=malloc(len*sizeof(*a)), *full=calloc(rows*w,sizeof(*full));
        double *initial=malloc(n*sizeof(*initial));
        double *x=malloc(n*sizeof(*x)), *y=malloc(n*sizeof(*y));
        vsdlss_sni *ix=malloc((e+1)*sizeof(*ix));
        csi *ix64=malloc((e+1)*sizeof(*ix64));
        if (!a || !full || !initial || !x || !y || !ix || !ix64) return 2;
        for (csi j=0; j<w; j++) {
            for (csi r=j; r<w; r++)
                a[VSDLSS_SN_DCOL(j,w)+r-j] = r==j ? 2.0 : rnd()/(w+1);
            for (csi r=0; r<e; r++) a[w*(w+1)/2+j*e+r]=rnd()/(w+1);
        }
        for (csi j=0; j<n; j++) initial[j]=rnd();
        for (csi r=0; r<e; r++) ix64[r]=ix[r]=(vsdlss_sni)(begin+w+3*r);
        if (inject==1) a[VSDLSS_SN_DCOL(w/2,w)]=0;
        if (inject==2) a[VSDLSS_SN_DCOL(w/2,w)]=NAN;
        if (inject==3) initial[begin+w/2]=NAN;
        for (csi j=0; j<w; j++) {
            for (csi r=j; r<w; r++) full[j*rows+r]=a[VSDLSS_SN_DCOL(j,w)+r-j];
            for (csi r=0; r<e; r++) full[j*rows+w+r]=a[w*(w+1)/2+j*e+r];
        }
        memcpy(x,initial,n*sizeof(*x));
        memcpy(y,initial,n*sizeof(*y));
        vsdlss_status s=vsdlss_panel_solve(full,begin,w,e,ix64,x,back);
#ifdef VSDLSS_BLAS
        packed_calls=0;
#endif
        vsdlss_status t=vsdlss_sn_panel_solve(a,begin,w,e,ix,y,back);
        if (s!=t || (inject==0 && t!=VSDLSS_OK) ||
            ((inject==1 || inject==2) && t!=VSDLSS_ERR_INVALID) ||
            (inject==3 && t!=VSDLSS_ERR_NONFINITE)) goto fail;
#ifdef VSDLSS_BLAS
        const char *option=getenv("VSDLSS_BLAS_BACKWARD");
        int expected=option && strcmp(option,"1")==0 && back &&
            (w>=16 || (w>=7 && e>=64)) && inject!=1 && inject!=2;
        if (packed_calls!=expected) goto fail;
#endif
        if (s==VSDLSS_OK) {
            double delta=0, norm=0;
            for (csi j=0; j<n; j++) {
                if (!isfinite(y[j])) goto fail;
                delta=fmax(delta,fabs(x[j]-y[j])); norm=fmax(norm,fabs(x[j]));
            }
            if (delta>1e-12*fmax(norm,1e-100)) goto fail;
        }
        if (memcmp(y,initial,begin*sizeof(*y))) goto fail;
        if (back && memcmp(y+begin+w,initial+begin+w,(n-begin-w)*sizeof(*y))) goto fail;
        free(a); free(full); free(initial); free(x); free(y); free(ix); free(ix64);
        count++;
        continue;
fail:
        fprintf(stderr,"backward BLAS: w=%lld ext=%lld back=%d inject=%d status=%d/%d\n",
                (long long)w,(long long)e,back,inject,s,t);
        return 3;
    }
    printf("test_backward_blas: %d shape/direction/status cases passed\n",count);
    return 0;
}
