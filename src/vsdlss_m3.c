#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define M3_TASK_MIN 4096 /* smallest component factored as a task of its own */

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

/* vsdlss_order for a reduced core; MLD (orders 0 and 4) through the M3
   implementation, which yields the same permutation. */
static vsdlss_status core_order(const vsdlss *core,int order,csi **q,csi **pinv)
{
    csi *lq=NULL,*lp,k; vsdlss_status status;
    if(order!=0&&order!=4) return vsdlss_order(core,order,q,pinv);
    *q=NULL; *pinv=NULL;
    status=vsdlss_validate_upper_csc(core);
    if(status==VSDLSS_OK) status=vsdlss_m3_mld_order(core,&lq);
    if(status!=VSDLSS_OK) return status;
    lp=(csi *)malloc((size_t)core->n*sizeof(csi));
    if(!lp) {free(lq);return VSDLSS_ERR_OOM;}
    for(k=0;k<core->n;++k) lp[k]=-1;
    for(k=0;k<core->n;++k) {
        if(lq[k]<0||lq[k]>=core->n||lp[lq[k]]>=0) {free(lq);free(lp);return VSDLSS_ERR_INVALID;}
        lp[lq[k]]=k;
    }
    *q=lq; *pinv=lp;
    return VSDLSS_OK;
}

static vsdlss_status factor_component(vsdlss_m3_factor *factor,const vsdlss *normalized,
                                      csi component,int order)
{
    vsdlss_status status=VSDLSS_OK;

        vsdlss_m3_component_factor *cf=factor->component+component;
        vsdlss *local=NULL,*permuted=NULL; csi *pinv=NULL;
        cf->n=factor->components->offset[component+1]-factor->components->offset[component];
        status=vsdlss_component_extract_normalized(normalized,factor->components,component,&local);
        if(status!=VSDLSS_OK) goto component_fail;
        status=vsdlss_reduce(local,&cf->reduction);
        if(status!=VSDLSS_OK) goto component_fail;
        if(cf->reduction->core_n) {
            status=core_order(cf->reduction->core,order,&cf->q,&pinv);
            if(status!=VSDLSS_OK) goto component_fail;
            permuted=vsdlss_symperm(cf->reduction->core,pinv,1);
            if(!permuted) {status=VSDLSS_ERR_OOM;goto component_fail;}
            vsdlss_spfree(local); local=NULL;
            status=vsdlss_m3_sn_factor(permuted,&cf->numeric);
        }
component_fail:
        free(pinv); vsdlss_spfree(permuted); vsdlss_spfree(local);
        return status;
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
    vsdlss_status *results=calloc((size_t)factor->count,sizeof(*results));
    if(!results){status=VSDLSS_ERR_OOM;goto fail;}
    /* One team for everything: a large component is a task, and its ordering
       hands independent subsets to idle threads as further tasks (results do
       not depend on the schedule); the small components share a task loop. */
    int nt=vsdlss_parallel_width((double)factor->n*256);
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp single)
        {
            for(component=0;component<factor->count;component++)
                if(factor->components->offset[component+1]-factor->components->offset[component]>=M3_TASK_MIN) {
                    VSDLSS_OMP(omp task firstprivate(component))
                    results[component]=factor_component(factor,normalized,component,order);
                }
            VSDLSS_OMP(omp taskloop grainsize(64))
            for(csi small=0;small<factor->count;small++)
                if(factor->components->offset[small+1]-factor->components->offset[small]<M3_TASK_MIN)
                    results[small]=factor_component(factor,normalized,small,order);
        }
    }
    for(component=0;component<factor->count;component++)if(results[component]!=VSDLSS_OK){
        status=results[component];free(results);goto fail;
    }
    free(results);
    vsdlss_spfree(normalized); *out=factor; return VSDLSS_OK;
fail:
    vsdlss_spfree(normalized); vsdlss_m3_factor_free(factor); return status;
}

/* One component of vsdlss_m3_solve: vsdlss_reduce_rhs, the core solve in
   the core order, vsdlss_reduce_recover -- the same operations and tests --
   on three arrays, gathering and scattering through the maps directly. */
static vsdlss_status solve_component(const vsdlss_m3_factor *factor,const double *rhs,
                                     double *global,csi component)
{
    const vsdlss_m3_component_factor *cf=factor->component+component;
    const vsdlss_reduction *r=cf->reduction;
    const csi n=cf->n,*vertex=factor->components->vertices+factor->components->offset[component];
    double *work=NULL,*saved=NULL,*core=NULL; csi k,j; vsdlss_status status=VSDLSS_OK;
    if(!count_fits(n,sizeof(double))||!count_fits(r->core_n,sizeof(double))||
       !count_fits(r->count,sizeof(double))) return VSDLSS_ERR_OOM;
    work=(double*)malloc((size_t)(n?n:1)*sizeof(double));
    saved=(double*)malloc((size_t)(r->count?r->count:1)*sizeof(double));
    core=(double*)malloc((size_t)(r->core_n?r->core_n:1)*sizeof(double));
    if(!work||!saved||!core) {status=VSDLSS_ERR_OOM;goto done;}
    for(k=0;k<n;k++) {
        work[k]=rhs[vertex[k]];
        if(!isfinite(work[k])) {status=VSDLSS_ERR_NONFINITE;goto done;}
    }
    for(k=0;k<r->count;k++) {
        const vsdlss_elim_record *record=r->records+k; double s;
        if(record->vertex<0||record->vertex>=n||record->degree<0||record->degree>3) {status=VSDLSS_ERR_INVALID;goto done;}
        s=saved[k]=work[record->vertex];
        if(!isfinite(s)) {status=VSDLSS_ERR_NONFINITE;goto done;}
        for(j=0;j<record->degree;j++) {
            const csi neighbor=record->neighbor[j]; double update;
            if(neighbor<0||neighbor>=n) {status=VSDLSS_ERR_INVALID;goto done;}
            update=record->multiplier[j]*s;
            if(!isfinite(record->multiplier[j])||!isfinite(update)||
               !isfinite(work[neighbor]-update)) {status=VSDLSS_ERR_NONFINITE;goto done;}
            work[neighbor]-=update;
        }
    }
    for(k=0;k<r->core_n;k++) {
        const csi v=r->core_vertices[k];
        if(v<0||v>=n) {status=VSDLSS_ERR_INVALID;goto done;}
        if(!isfinite(work[v])) {status=VSDLSS_ERR_NONFINITE;goto done;}
    }
    if(r->core_n) {
        for(k=0;k<r->core_n;k++) core[k]=work[r->core_vertices[cf->q[k]]];
        status=vsdlss_m3_sn_solve(cf->numeric,core);
        if(status!=VSDLSS_OK) goto done;
    }
    memset(work,0,(size_t)n*sizeof(double));     /* now the local solution */
    for(k=0;k<r->core_n;k++) work[r->core_vertices[cf->q[k]]]=core[k];
    for(k=r->count;k>0;k--) {
        const vsdlss_elim_record *record=r->records+(k-1); double value;
        if(!isfinite(record->pivot)) {status=VSDLSS_ERR_NONFINITE;goto done;}
        if(record->pivot==0.0) {status=VSDLSS_ERR_INVALID;goto done;}
        value=saved[k-1]/record->pivot;
        if(!isfinite(value)) {status=VSDLSS_ERR_NONFINITE;goto done;}
        for(j=0;j<record->degree;j++) {
            const csi neighbor=record->neighbor[j];
            const double update=record->multiplier[j]*work[neighbor];
            if(!isfinite(update)||!isfinite(value-update)) {status=VSDLSS_ERR_NONFINITE;goto done;}
            value-=update;
        }
        work[record->vertex]=value;
    }
    for(k=0;k<n;k++) global[vertex[k]]=work[k];
done:
    free(work);free(saved);free(core);
    return status;
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
    vsdlss_status *results=calloc((size_t)factor->count,sizeof(*results));
    if(!results){free(global);return VSDLSS_ERR_OOM;}
    int nt=vsdlss_parallel_width((double)factor->n*256);
    if(nt>factor->count)nt=(int)factor->count;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(dynamic,1))
        for(component=0;component<factor->count;component++)
            results[component]=solve_component(factor,rhs,global,component);
    }
    for(component=0;component<factor->count;component++)if(results[component]!=VSDLSS_OK){
        status=results[component];break;
    }
    free(results);
    if(status==VSDLSS_OK) memcpy(solution,global,(size_t)factor->n*sizeof(*solution));
    free(global); return status;
}

vsdlss_status vsdlss_m3_solve_many(const vsdlss_m3_factor *f,csi nrhs,
    const double *rhs,csi ldrhs,double *solution,csi ldsolution)
{
    if(!f||!rhs||!solution||nrhs<1||ldrhs<f->n||ldsolution<f->n)return VSDLSS_ERR_INVALID;
    if(f->n<1||(uint64_t)nrhs>SIZE_MAX/sizeof(double)/(uint64_t)f->n||
        (uint64_t)nrhs>SIZE_MAX/sizeof(vsdlss_status)||
        (uint64_t)(nrhs-1)>(SIZE_MAX/sizeof(double)-(uint64_t)f->n)/(uint64_t)ldrhs||
        (uint64_t)(nrhs-1)>(SIZE_MAX/sizeof(double)-(uint64_t)f->n)/(uint64_t)ldsolution)
        return VSDLSS_ERR_OOM;
    double *work=malloc((size_t)nrhs*(size_t)f->n*sizeof(double));
    vsdlss_status *results=calloc((size_t)nrhs,sizeof(*results)),status=VSDLSS_OK;
    if(!work||!results){free(work);free(results);return VSDLSS_ERR_OOM;}
    int nt=vsdlss_parallel_width((double)f->n*nrhs*256);
    if(nt>nrhs)nt=(int)nrhs;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(dynamic,1))
        for(csi r=0;r<nrhs;r++)results[r]=vsdlss_m3_solve(f,rhs+(size_t)r*ldrhs,work+(size_t)r*f->n);
    }
    for(csi r=0;r<nrhs;r++)if(results[r]!=VSDLSS_OK){status=results[r];break;}
    if(status==VSDLSS_OK)for(csi r=0;r<nrhs;r++)
        memcpy(solution+(size_t)r*ldsolution,work+(size_t)r*f->n,(size_t)f->n*sizeof(double));
    free(work);free(results);return status;
}
