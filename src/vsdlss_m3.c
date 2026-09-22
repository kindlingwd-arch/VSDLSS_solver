#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Components smaller than this keep the ascending numbering. */
csi vsdlss_reorder_min = 4096;
#define VSDLSS_REORDER_MIN vsdlss_reorder_min

static int count_fits(csi n, size_t width)
{
    return n >= 0 && (uint64_t)n <= SIZE_MAX / width;
}

void vsdlss_m3_factor_free(vsdlss_m3_factor *factor)
{
    csi k;
    if(!factor) return;
    if(factor->component) for(k=0;k<factor->count;k++) {
        vsdlss_m4_factor_free(factor->component[k].disk);
        vsdlss_reduction_free(factor->component[k].reduction);
        free(factor->component[k].gather);
        free(factor->component[k].q);
        vsdlss_sn_factor_free(factor->component[k].numeric);
    }
    free(factor->component);
    vsdlss_components_free(factor->components);
    free(factor);
}

static vsdlss_status factor_component(vsdlss_m3_factor *factor,const vsdlss *normalized,
                                      csi component,int order)
{
    vsdlss_status status=VSDLSS_OK;

        vsdlss_m3_component_factor *cf=factor->component+component;
        vsdlss *local=NULL,*permuted=NULL; csi *pinv=NULL;
        vsdlss_sn_symbolic *symbolic=NULL;
        cf->n=factor->components->offset[component+1]-factor->components->offset[component];
        if(factor->components->order && cf->n>=VSDLSS_REORDER_MIN) {
            /* BFS numbering: the low-degree reduction walks vertices in index
             * order, so neighbours with nearby indices keep its adjacency and
             * diagonal accesses in cache (2-3x faster on scattered input). */
            const csi *perm=factor->components->order+factor->components->offset[component];
            const csi begin=factor->components->offset[component];
            cf->gather=(csi*)malloc((size_t)cf->n*sizeof(csi));
            if(!cf->gather){status=VSDLSS_ERR_OOM;goto component_fail;}
            for(csi k=0;k<cf->n;k++)cf->gather[k]=factor->components->vertices[begin+perm[k]];
            status=vsdlss_component_extract_permuted(normalized,factor->components,component,perm,&local);
        } else
            status=vsdlss_component_extract_normalized(normalized,factor->components,component,&local);
        if(status!=VSDLSS_OK) goto component_fail;
        status=vsdlss_reduce(local,&cf->reduction);
        if(status!=VSDLSS_OK) goto component_fail;
        if(cf->reduction->core_n && !factor->disk_mode) {
            status=vsdlss_order(cf->reduction->core,order,&cf->q,&pinv);
            if(status!=VSDLSS_OK) goto component_fail;
            status=vsdlss_postorder_permutation(cf->reduction->core,cf->q,pinv);
            if(status!=VSDLSS_OK) goto component_fail;
            permuted=vsdlss_symperm(cf->reduction->core,pinv,1);
            if(!permuted) {status=VSDLSS_ERR_OOM;goto component_fail;}
            status=vsdlss_sn_analyze_relaxed(permuted,&symbolic);
            if(status==VSDLSS_OK) status=vsdlss_sn_factorize(permuted,symbolic,&cf->numeric);
        }
component_fail:
        free(pinv); vsdlss_sn_symbolic_free(symbolic); vsdlss_spfree(permuted); vsdlss_spfree(local);
        return status;
}

static vsdlss_status factorize_shared(const vsdlss *A, int order,
                                  vsdlss_m3_factor **out,int disk_mode,size_t budget,const char *directory)
{
    vsdlss_m3_factor *factor=NULL; vsdlss *normalized=NULL;
    vsdlss_status status; csi component;
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(order<0 || order>5) return VSDLSS_ERR_UNSUPPORTED;
    if(!A) return VSDLSS_ERR_INVALID;
    if(A->n==INT64_MAX || A->n<1 || !count_fits(A->n+1,sizeof(csi)) ||
       !count_fits(A->n,sizeof(double))) return A->n<1?VSDLSS_ERR_INVALID:VSDLSS_ERR_OOM;
    status=vsdlss_normalize_upper(A,&normalized);
    if(status!=VSDLSS_OK) return status;
    factor=(vsdlss_m3_factor*)calloc(1,sizeof(*factor));
    if(!factor) {status=VSDLSS_ERR_OOM;goto fail;}
    factor->n=normalized->n; factor->disk_mode=disk_mode;
    status=vsdlss_components_build_normalized(normalized,&factor->components);
    if(status!=VSDLSS_OK) goto fail;
    factor->count=factor->components->count;
    if(!count_fits(factor->count,sizeof(*factor->component))) {status=VSDLSS_ERR_OOM;goto fail;}
    factor->component=(vsdlss_m3_component_factor*)calloc((size_t)factor->count,sizeof(*factor->component));
    if(!factor->component) {status=VSDLSS_ERR_OOM;goto fail;}
    vsdlss_status *results=calloc((size_t)factor->count,sizeof(*results));
    if(!results){status=VSDLSS_ERR_OOM;goto fail;}
    int nt=factor->disk_mode?1:vsdlss_parallel_width((double)factor->n*256);
    if(nt>factor->count)nt=(int)factor->count;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(dynamic,1))
        for(component=0;component<factor->count;component++)
            results[component]=factor_component(factor,normalized,component,order);
    }
    for(component=0;component<factor->count;component++)if(results[component]!=VSDLSS_OK){
        status=results[component];free(results);goto fail;
    }
    free(results);
    if(disk_mode) {
        csi cores=0;
        for(component=0;component<factor->count;component++)
            if(factor->component[component].reduction->core_n)cores++;
        size_t remaining=budget;
        for(component=0;component<factor->count;component++) {
            vsdlss_m3_component_factor *cf=&factor->component[component];
            if(!cf->reduction->core_n)continue;
            status=vsdlss_factorize_m4(cf->reduction->core,order,remaining/(size_t)cores,directory,&cf->disk);
            if(status!=VSDLSS_OK)goto fail;
            size_t used=vsdlss_m4_workspace_bytes(cf->disk);
            if(used>remaining){status=VSDLSS_ERR_OOM;goto fail;}
            remaining-=used;cores--;
        }
    }
    vsdlss_spfree(normalized); *out=factor; return VSDLSS_OK;
fail:
    vsdlss_spfree(normalized); vsdlss_m3_factor_free(factor); return status;
}

/* One RHS through one component: gather, eliminate the low-degree
 * vertices in place, solve the core, recover in place, scatter.  All buffers
 * are private, so the in-place reduction forms are used (same arithmetic as
 * vsdlss_reduce_rhs / vsdlss_reduce_recover, fewer large temporaries). */
static vsdlss_status solve_component(const vsdlss_m3_factor *factor,const double *rhs,
                                     double *global,csi component)
{
    const vsdlss_m3_component_factor *cf=factor->component+component;
    const vsdlss_reduction *r=cf->reduction;
    const csi begin=factor->components->offset[component], core=r->core_n;
    const csi *map=cf->gather?cf->gather:factor->components->vertices+begin;
    double *local=NULL,*saved=NULL,*core_b=NULL,*core_x=NULL;
    vsdlss_status status=VSDLSS_OK; csi k;
    if(!count_fits(cf->n,sizeof(double)) || !count_fits(core,sizeof(double)) ||
       !count_fits(r->count,sizeof(double))) return VSDLSS_ERR_OOM;
    local=(double*)malloc((size_t)cf->n*sizeof(double));
    if(r->count) saved=(double*)malloc((size_t)r->count*sizeof(double));
    if(core) core_b=(double*)malloc((size_t)core*sizeof(double));
    if(core&&cf->disk) core_x=(double*)malloc((size_t)core*sizeof(double));
    if(!local||(r->count&&!saved)||(core&&!core_b)||(core&&cf->disk&&!core_x))
        {status=VSDLSS_ERR_OOM;goto done;}
    for(k=0;k<cf->n;k++) local[k]=rhs[map[k]];
    status=vsdlss_reduce_forward_inplace(r,local,saved);
    if(status!=VSDLSS_OK) goto done;
    if(core) {
        if(cf->disk) {
            for(k=0;k<core;k++) core_b[k]=local[r->core_vertices[k]];
            status=vsdlss_m4_solve(cf->disk,core_b,core_x);
            if(status!=VSDLSS_OK) goto done;
            for(k=0;k<core;k++) local[r->core_vertices[k]]=core_x[k];
        } else {
            for(k=0;k<core;k++) core_b[k]=local[r->core_vertices[cf->q[k]]];
            status=vsdlss_sn_solve(cf->numeric,core_b,core_b);
            if(status!=VSDLSS_OK) goto done;
            for(k=0;k<core;k++) local[r->core_vertices[cf->q[k]]]=core_b[k];
        }
    }
    status=vsdlss_reduce_backward_inplace(r,saved,local);
    if(status==VSDLSS_OK) for(k=0;k<cf->n;k++) global[map[k]]=local[k];
done:
    free(local);free(saved);free(core_b);free(core_x);
    return status;
}

/* Writes every component straight into `dest`, which must not alias the
 * caller's output unless a partial write is acceptable.  vsdlss_m3_solve adds
 * the transactional staging buffer; vsdlss_m3_solve_many already owns one. */
static vsdlss_status solve_into(const vsdlss_m3_factor *factor,
                                const double *rhs, double *dest)
{
    csi component; vsdlss_status status=VSDLSS_OK;
    vsdlss_status *results=calloc((size_t)factor->count,sizeof(*results));
    if(!results)return VSDLSS_ERR_OOM;
    int nt=factor->disk_mode?1:vsdlss_parallel_width((double)factor->n*256);
    if(nt>factor->count)nt=(int)factor->count;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(dynamic,1))
        for(component=0;component<factor->count;component++)
            results[component]=solve_component(factor,rhs,dest,component);
    }
    for(component=0;component<factor->count;component++)if(results[component]!=VSDLSS_OK){
        status=results[component];break;
    }
    free(results); return status;
}

vsdlss_status vsdlss_m3_solve(const vsdlss_m3_factor *factor,
                              const double *rhs, double *solution)
{
    double *global=NULL; vsdlss_status status;
    if(!factor || !factor->components || !factor->component || !rhs || !solution)
        return VSDLSS_ERR_INVALID;
    if(!count_fits(factor->n,sizeof(*global))) return VSDLSS_ERR_OOM;
    global=(double*)malloc((size_t)factor->n*sizeof(*global));
    if(!global) return VSDLSS_ERR_OOM;
    status=solve_into(factor,rhs,global);
    if(status==VSDLSS_OK) memcpy(solution,global,(size_t)factor->n*sizeof(*solution));
    free(global); return status;
}

/* A batch of right-hand sides through every component: gather/reduce each
 * RHS, run one batched supernodal solve per component, then recover.  The
 * per-RHS arithmetic is that of solve_component, so results are identical. */
/* A batch of right-hand sides through every component: per RHS gather and
 * in-place forward elimination, one batched supernodal solve per component,
 * then per RHS in-place recovery.  Per-RHS arithmetic equals solve_component. */
static vsdlss_status solve_batch(const vsdlss_m3_factor *f,csi nrhs,const double *rhs,
                                 csi ldrhs,double *dest,csi lddest)
{
    vsdlss_status st=VSDLSS_OK;
    for(csi c=0;c<f->count&&st==VSDLSS_OK;c++){
        const vsdlss_m3_component_factor *cf=f->component+c;
        const vsdlss_reduction *r=cf->reduction;
        const csi begin=f->components->offset[c],cn=cf->n,core=r->core_n,cnt=r->count;
        const csi *map=cf->gather?cf->gather:f->components->vertices+begin;
        double *local=malloc((size_t)cn*sizeof(double));
        double *perm=core?malloc((size_t)core*(size_t)nrhs*sizeof(double)):NULL;
        double *saved=cnt?malloc((size_t)cnt*(size_t)nrhs*sizeof(double)):NULL;
        if(!local||(core&&!perm)||(cnt&&!saved)){st=VSDLSS_ERR_OOM;goto next;}
        for(csi r0=0;r0<nrhs&&st==VSDLSS_OK;r0++){
            const double *b=rhs+(size_t)r0*ldrhs;
            for(csi k=0;k<cn;k++)local[k]=b[map[k]];
            st=vsdlss_reduce_forward_inplace(r,local,cnt?saved+(size_t)r0*cnt:NULL);
            if(st==VSDLSS_OK)for(csi k=0;k<core;k++)perm[(size_t)r0*core+k]=local[r->core_vertices[cf->q[k]]];
        }
        if(st==VSDLSS_OK&&core)st=vsdlss_sn_solve_batch(cf->numeric,nrhs,perm,core);
        for(csi r0=0;r0<nrhs&&st==VSDLSS_OK;r0++){
            double *x=dest+(size_t)r0*lddest;
            for(csi k=0;k<core;k++)local[r->core_vertices[cf->q[k]]]=perm[(size_t)r0*core+k];
            st=vsdlss_reduce_backward_inplace(r,cnt?saved+(size_t)r0*cnt:NULL,local);
            if(st==VSDLSS_OK)for(csi k=0;k<cn;k++)x[map[k]]=local[k];
        }
next:
        free(local);free(perm);free(saved);
    }
    return st;
}

vsdlss_status vsdlss_m3_solve_many(const vsdlss_m3_factor *f,csi nrhs,
    const double *rhs,csi ldrhs,double *solution,csi ldsolution)
{
    if(f && f->disk_mode)return VSDLSS_ERR_UNSUPPORTED;
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
    /* Contiguous groups of right-hand sides, one batched solve per group;
     * `work` is private per RHS, so no extra staging is needed. */
    csi groups=nt>1?nt:1;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(static,1))
        for(csi g=0;g<groups;g++){
            csi r0=nrhs*g/groups,r1=nrhs*(g+1)/groups;
            if(r1>r0)results[r0]=solve_batch(f,r1-r0,rhs+(size_t)r0*ldrhs,ldrhs,
                                             work+(size_t)r0*f->n,f->n);
        }
    }
    for(csi r=0;r<nrhs;r++)if(results[r]!=VSDLSS_OK){status=results[r];break;}
    if(status==VSDLSS_OK)for(csi r=0;r<nrhs;r++)
        memcpy(solution+(size_t)r*ldsolution,work+(size_t)r*f->n,(size_t)f->n*sizeof(double));
    free(work);free(results);return status;
}

vsdlss_status vsdlss_factorize_m3(const vsdlss *a,int order,vsdlss_m3_factor **out)
{ return factorize_shared(a,order,out,0,0,NULL); }
vsdlss_status vsdlss_factorize_m4_reduced(const vsdlss *a,int order,size_t budget,const char *directory,vsdlss_m4_reduced_factor **out)
{ return factorize_shared(a,order,out,1,budget,directory); }
vsdlss_status vsdlss_m4_reduced_solve(vsdlss_m4_reduced_factor *f,const double *b,double *x)
{ if(!f||!f->disk_mode)return VSDLSS_ERR_INVALID;return vsdlss_m3_solve(f,b,x); }
void vsdlss_m4_reduced_free(vsdlss_m4_reduced_factor *f)
{ vsdlss_m3_factor_free(f); }
