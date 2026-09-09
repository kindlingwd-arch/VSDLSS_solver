#include "vsdlss_m3_internal.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int count_fits(csi n, size_t width)
{
    return n >= 0 && (uint64_t)n <= SIZE_MAX / width;
}

void vsdlss_m3_factor_free(vsdlss_m3_factor *factor)
{
    csi k;
    if(!factor) return;
    if(factor->component) for(k=0;k<factor->count;k++) {
        vsdlss_reduction_free(factor->component[k].reduction);
        free(factor->component[k].q);
        vsdlss_sn_factor_free(factor->component[k].numeric);
    }
    free(factor->component);
    vsdlss_components_free(factor->components);
    free(factor);
}

vsdlss_status vsdlss_factorize_m3(const vsdlss *A, int order,
                                  vsdlss_m3_factor **out)
{
    vsdlss_m3_factor *factor=NULL; vsdlss *normalized=NULL;
    vsdlss_status status; csi component;
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(order<0 || order>4) return VSDLSS_ERR_UNSUPPORTED;
    if(!A) return VSDLSS_ERR_INVALID;
    if(A->n==INT64_MAX || A->n<1 || !count_fits(A->n+1,sizeof(csi)) ||
       !count_fits(A->n,sizeof(double))) return A->n<1?VSDLSS_ERR_INVALID:VSDLSS_ERR_OOM;
    status=vsdlss_normalize_upper(A,&normalized);
    if(status!=VSDLSS_OK) return status;
    factor=(vsdlss_m3_factor*)calloc(1,sizeof(*factor));
    if(!factor) {status=VSDLSS_ERR_OOM;goto fail;}
    factor->n=normalized->n;
    status=vsdlss_components_build_normalized(normalized,&factor->components);
    if(status!=VSDLSS_OK) goto fail;
    factor->count=factor->components->count;
    if(!count_fits(factor->count,sizeof(*factor->component))) {status=VSDLSS_ERR_OOM;goto fail;}
    factor->component=(vsdlss_m3_component_factor*)calloc((size_t)factor->count,sizeof(*factor->component));
    if(!factor->component) {status=VSDLSS_ERR_OOM;goto fail;}
    for(component=0;component<factor->count;component++) {
        vsdlss_m3_component_factor *cf=factor->component+component;
        vsdlss *local=NULL,*permuted=NULL; csi *pinv=NULL;
        vsdlss_sn_symbolic *symbolic=NULL;
        cf->n=factor->components->offset[component+1]-factor->components->offset[component];
        status=vsdlss_component_extract_normalized(normalized,factor->components,component,&local);
        if(status!=VSDLSS_OK) goto component_fail;
        status=vsdlss_reduce(local,&cf->reduction);
        if(status!=VSDLSS_OK) goto component_fail;
        if(cf->reduction->core_n) {
            status=vsdlss_order(cf->reduction->core,order,&cf->q,&pinv);
            if(status!=VSDLSS_OK) goto component_fail;
            permuted=vsdlss_symperm(cf->reduction->core,pinv,1);
            if(!permuted) {status=VSDLSS_ERR_OOM;goto component_fail;}
            status=vsdlss_sn_analyze(permuted,&symbolic);
            if(status==VSDLSS_OK) status=vsdlss_sn_factorize(permuted,symbolic,&cf->numeric);
        }
component_fail:
        free(pinv); vsdlss_sn_symbolic_free(symbolic); vsdlss_spfree(permuted); vsdlss_spfree(local);
        if(status!=VSDLSS_OK) goto fail;
    }
    vsdlss_spfree(normalized); *out=factor; return VSDLSS_OK;
fail:
    vsdlss_spfree(normalized); vsdlss_m3_factor_free(factor); return status;
}

vsdlss_status vsdlss_m3_solve(const vsdlss_m3_factor *factor,
                              const double *rhs, double *solution)
{
    double *global=NULL; csi component; vsdlss_status status=VSDLSS_OK;
    if(!factor || !factor->components || !factor->component || !rhs || !solution)
        return VSDLSS_ERR_INVALID;
    if(!count_fits(factor->n,sizeof(*global))) return VSDLSS_ERR_OOM;
    global=(double*)malloc((size_t)factor->n*sizeof(*global));
    if(!global) return VSDLSS_ERR_OOM;
    for(component=0;component<factor->count;component++) {
        const vsdlss_m3_component_factor *cf=factor->component+component;
        const csi begin=factor->components->offset[component]; csi k;
        double *local=NULL,*core_rhs=NULL,*saved=NULL,*permuted=NULL,*core_x=NULL,*local_x=NULL;
        if(!count_fits(cf->n,sizeof(double)) ||
           !count_fits(cf->reduction->core_n,sizeof(double)) ||
           !count_fits(cf->reduction->count,sizeof(double))) {status=VSDLSS_ERR_OOM;goto component_done;}
        local=(double*)malloc((size_t)cf->n*sizeof(double));
        local_x=(double*)malloc((size_t)cf->n*sizeof(double));
        if(cf->reduction->core_n) {
            core_rhs=(double*)malloc((size_t)cf->reduction->core_n*sizeof(double));
            permuted=(double*)malloc((size_t)cf->reduction->core_n*sizeof(double));
            core_x=(double*)malloc((size_t)cf->reduction->core_n*sizeof(double));
        }
        if(cf->reduction->count) saved=(double*)malloc((size_t)cf->reduction->count*sizeof(double));
        if(!local||!local_x||(cf->reduction->core_n&&(!core_rhs||!permuted||!core_x))||
           (cf->reduction->count&&!saved)) {status=VSDLSS_ERR_OOM;goto component_done;}
        for(k=0;k<cf->n;k++) local[k]=rhs[factor->components->vertices[begin+k]];
        status=vsdlss_reduce_rhs(cf->reduction,local,core_rhs,saved);
        if(status!=VSDLSS_OK) goto component_done;
        if(cf->reduction->core_n) {
            for(k=0;k<cf->reduction->core_n;k++) permuted[k]=core_rhs[cf->q[k]];
            status=vsdlss_sn_solve(cf->numeric,permuted,permuted);
            if(status!=VSDLSS_OK) goto component_done;
            for(k=0;k<cf->reduction->core_n;k++) core_x[cf->q[k]]=permuted[k];
        }
        status=vsdlss_reduce_recover(cf->reduction,saved,core_x,local_x);
        if(status==VSDLSS_OK) for(k=0;k<cf->n;k++)
            global[factor->components->vertices[begin+k]]=local_x[k];
component_done:
        free(local);free(core_rhs);free(saved);free(permuted);free(core_x);free(local_x);
        if(status!=VSDLSS_OK) break;
    }
    if(status==VSDLSS_OK) memcpy(solution,global,(size_t)factor->n*sizeof(*solution));
    free(global); return status;
}
