#include "vsdlss_m3_internal.h"

vsdlss_status vsdlss_panel_factor(double *a, csi rows, csi width)
{
    if (!a || width<1 || rows<width) return VSDLSS_ERR_INVALID;
    for (csi j=0;j<width;j++) {
        double d=a[j*rows+j];
        for (csi k=0;k<j;k++) d-=a[k*rows+j]*a[k*rows+j];
        if (!isfinite(d)) return VSDLSS_ERR_NONFINITE;
        if (d<=0) return VSDLSS_ERR_NOT_POSDEF;
        d=sqrt(d); a[j*rows+j]=d;
        for (csi i=j+1;i<rows;i++) {
            double v=a[j*rows+i];
            for (csi k=0;k<j;k++) v-=a[k*rows+i]*a[k*rows+j];
            v/=d;
            if (!isfinite(v)) return VSDLSS_ERR_NONFINITE;
            a[j*rows+i]=v;
        }
    }
    return VSDLSS_OK;
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
    for(csi t=0;t<width;t++) {
        csi j=back?width-1-t:t;
        double d=a[j*rows+j],v=x[begin+j];
        if(!isfinite(d)||d<=0) return VSDLSS_ERR_INVALID;
        if(back) {
            for(csi r=j+1;r<width;r++) v-=a[j*rows+r]*x[begin+r];
            for(csi r=0;r<ext;r++) v-=a[j*rows+width+r]*x[index[r]];
            x[begin+j]=v/d;
        } else {
            x[begin+j]=v/d;
            for(csi r=j+1;r<width;r++) x[begin+r]-=a[j*rows+r]*x[begin+j];
            for(csi r=0;r<ext;r++) x[index[r]]-=a[j*rows+width+r]*x[begin+j];
        }
        if(!isfinite(x[begin+j])) return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}
