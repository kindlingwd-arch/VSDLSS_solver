#include "vsdlss.h"
#include <string.h>
int main(int argc,char **argv)
{
    /* A = [4 -1 0; -1 4 -1; 0 -1 3], x = [1 2 3], b = [2 4 7]. */
    csi p[]={0,1,3,5},i[]={0,0,1,1,2};
    double values[]={4,-1,4,-1,3},b[]={2,4,7},x[3]={0};
    vsdlss a={5,3,3,p,i,values,-1};
    vsdlss_status st=VSDLSS_ERR_UNSUPPORTED;
    const char *mode=argc>1?argv[1]:"m3";
    if(!strcmp(mode,"m1")){
        vsdlss_factor *f=NULL;st=vsdlss_factorize(&a,0,&f);
        if(st==VSDLSS_OK)st=vsdlss_factor_solve(f,b,x);
        vsdlss_factor_free(f);
    }else if(!strcmp(mode,"m3")){
        vsdlss_m3_factor *f=NULL;st=vsdlss_factorize_m3(&a,0,&f);
        if(st==VSDLSS_OK)st=vsdlss_m3_solve(f,b,x);
        vsdlss_m3_factor_free(f);
    }else if(!strcmp(mode,"m4")){
        vsdlss_m4_factor *f=NULL;st=vsdlss_factorize_m4(&a,0,65536,NULL,&f);
        if(st==VSDLSS_OK)st=vsdlss_m4_solve(f,b,x);
        vsdlss_m4_factor_free(f);
    }else if(!strcmp(mode,"m4-reduced")){
        vsdlss_m4_reduced_factor *f=NULL;
        st=vsdlss_factorize_m4_reduced(&a,0,65536,NULL,&f);
        if(st==VSDLSS_OK)st=vsdlss_m4_reduced_solve(f,b,x);
        vsdlss_m4_reduced_free(f);
    }
    if(st!=VSDLSS_OK){fprintf(stderr,"%s: %s\n",mode,vsdlss_status_string(st));return 1;}
    double eta;
    st=vsdlss_backward_error(&a,x,b,&eta);
    if(st!=VSDLSS_OK||!isfinite(eta)||eta>1e-12)return 2;
    for(int k=0;k<3;k++)if(fabs(x[k]-(k+1))>1e-12)return 3;
    printf("%s: x=[%.12g %.12g %.12g] backward_error=%.3e PASS\n",mode,x[0],x[1],x[2],eta);
    return 0;
}
