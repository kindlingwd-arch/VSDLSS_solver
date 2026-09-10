#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

vsdlss_status vsdlss_panel_factor(double *a, csi rows, csi width)
{
    if (!a || width<1 || rows<width) return VSDLSS_ERR_INVALID;
    int nt=vsdlss_parallel_width((double)rows*width*width),bad=0;
    vsdlss_status status=VSDLSS_OK;(void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) shared(status,bad))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        for(csi j=0;j<width;j++) {
            VSDLSS_OMP(omp single)
            {
                bad=0;
                if(status==VSDLSS_OK){
                    double d=a[j*rows+j];
                    for(csi k=0;k<j;k++)d-=a[k*rows+j]*a[k*rows+j];
                    if(!isfinite(d))status=VSDLSS_ERR_NONFINITE;
                    else if(d<=0)status=VSDLSS_ERR_NOT_POSDEF;
                    else a[j*rows+j]=sqrt(d);
                }
            }
            VSDLSS_OMP(omp for schedule(static) reduction(|:bad))
            for(csi i=j+1;i<rows;i++)if(status==VSDLSS_OK){
                double v=a[j*rows+i];
                for(csi k=0;k<j;k++)v-=a[k*rows+i]*a[k*rows+j];
                v/=a[j*rows+j];
                if(!isfinite(v))bad=1;
                a[j*rows+i]=v;
            }
            VSDLSS_OMP(omp single)
            { if(bad)status=VSDLSS_ERR_NONFINITE; }
        }
    }
    return status;
}

double vsdlss_panel_dot(const double *a,csi rows,csi width,csi i,csi j)
{
    double v=0;
    for(csi k=0;k<width;k++) v+=a[k*rows+i]*a[k*rows+j];
    return v;
}

vsdlss_status vsdlss_panel_solve(const double *a,csi begin,csi width,
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
