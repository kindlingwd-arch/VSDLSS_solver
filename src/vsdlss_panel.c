#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

vsdlss_status vsdlss_panel_factor(double *a, csi rows, csi width)
{
    if (!a || width<1 || rows<width) return VSDLSS_ERR_INVALID;
    /* Factor the diagonal block first. External rows only read this block,
       so each row tile can complete all columns without column barriers.
       Keep the original increasing-k subtraction order for each element. */
    for(csi j=0;j<width;j++) {
        double d=a[j*rows+j];
        for(csi k=0;k<j;k++)d-=a[k*rows+j]*a[k*rows+j];
        if(!isfinite(d))return VSDLSS_ERR_NONFINITE;
        if(d<=0)return VSDLSS_ERR_NOT_POSDEF;
        d=sqrt(d);a[j*rows+j]=d;
        for(csi k=0;k<j;k++) {
            double coeff=a[k*rows+j];
            for(csi i=j+1;i<width;i++)a[j*rows+i]-=a[k*rows+i]*coeff;
        }
        for(csi i=j+1;i<width;i++) {
            a[j*rows+i]/=d;
            if(!isfinite(a[j*rows+i]))return VSDLSS_ERR_NONFINITE;
        }
    }
    const csi tile=32,ext=rows-width,tiles=ext/tile+(ext%tile!=0);
    int nt=vsdlss_parallel_width((double)ext*width*width),bad=0;
    if(tiles==0)return VSDLSS_OK;
    if(nt>tiles)nt=(int)tiles;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(static))
        for(csi t=0;t<tiles;t++) {
            csi first=width+t*tile,n=rows-first;
            if(n>tile)n=tile;
            double v[32];
            for(csi j=0;j<width;j++) {
                for(csi i=0;i<n;i++)v[i]=a[j*rows+first+i];
                for(csi k=0;k<j;k++) {
                    double coeff=a[k*rows+j];
                    for(csi i=0;i<n;i++)v[i]-=a[k*rows+first+i]*coeff;
                }
                for(csi i=0;i<n;i++) {
                    v[i]/=a[j*rows+j];
                    if(!isfinite(v[i]))bad=1;
                    a[j*rows+first+i]=v[i];
                }
            }
        }
    }
    return bad?VSDLSS_ERR_NONFINITE:VSDLSS_OK;
}

double vsdlss_panel_dot(const double *a,csi rows,csi width,csi i,csi j)
{
    double v=0;
    /* Preserve left-to-right accumulation, including the initial +0. */
#define DOT(K) v+=a[(K)*rows+i]*a[(K)*rows+j]
    if(width>=1 && width<=6) {
        DOT(0); if(width>=2){DOT(1);} if(width>=3){DOT(2);}
        if(width>=4){DOT(3);} if(width>=5){DOT(4);} if(width>=6){DOT(5);}
    } else for(csi k=0;k<width;k++) v+=a[k*rows+i]*a[k*rows+j];
#undef DOT
    return v;
}

vsdlss_status vsdlss_panel_solve_generic(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
    csi rows=width+ext;
    int nt=vsdlss_parallel_width((double)width*ext),bad=0;(void)nt;
    /* External rows have no dependency on J until the triangular solve ends.
       Reverse solve applies the external contribution before solving J^T. */
    if(!back)for(csi j=0;j<width;j++){
        double d=a[j*rows+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        x[begin+j]/=d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
        for(csi r=j+1;r<width;r++)x[begin+r]-=a[j*rows+r]*x[begin+j];
    }
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(static))
        for(csi t=0;t<(back?width:ext);t++){
            csi dest=back?begin+t:index[t];double v=x[dest];
            if(back)for(csi r=0;r<ext;r++)v-=a[t*rows+width+r]*x[index[r]];
            else for(csi j=0;j<width;j++)v-=a[j*rows+width+t]*x[begin+j];
            x[dest]=v;if(!isfinite(v))bad=1;
        }
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    if(back)for(csi j=width;j-- >0;){
        double d=a[j*rows+j],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=a[j*rows+r]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

#include "vsdlss_small_solve.inc"

vsdlss_status vsdlss_panel_solve(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
    /* Keep large external-row work on the existing parallel path. */
    if(vsdlss_parallel_width((double)width*ext)>1)
        return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,back);
    switch(width) {
#define CASE(N) case N: return solve_##N(a,begin,ext,index,x,back)
        CASE(1); CASE(2); CASE(3); CASE(4); CASE(5); CASE(6);
#undef CASE
        default: return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,back);
    }
}
