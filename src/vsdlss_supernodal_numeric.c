#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int bytes_ok(csi n, size_t z) { return n>=0 && (uint64_t)n<=SIZE_MAX/z; }
static void *copy_array(const void *p,csi n,size_t z)
{
    void *q;
    if(!bytes_ok(n,z)) return NULL;
    if(!n) return NULL;
    q=malloc((size_t)n*z); if(q) memcpy(q,p,(size_t)n*z); return q;
}
static csi find_entry(const vsdlss_sn_factor *f,csi col,csi row)
{
    csi lo=f->l_col_ptr[col],hi=f->l_col_ptr[col+1];
    while(lo<hi){csi m=lo+(hi-lo)/2;if(f->l_row_index[m]<row)lo=m+1;else hi=m;}
    return lo<f->l_col_ptr[col+1]&&f->l_row_index[lo]==row?lo:-1;
}

void vsdlss_sn_factor_free(vsdlss_sn_factor *f)
{
    if(!f)return;
    free(f->l_col_ptr);free(f->l_row_index);free(f->l_panel_slot);
    free(f->column_start);free(f->row_ptr);free(f->row_index);
    free(f->panel_offset);free(f->panel);free(f);
}

/* Iterators in a depend clause are OpenMP 5.0.  GCC and clang accept them
 * well before they raise _OPENMP to 201811, so select on the compiler.  The
 * right-looking alternative (one task per source/destination block pair) was
 * measured slower than the serial path at every size tried, so without
 * iterator support the DAG request falls back to the ordinary path instead. */
#if defined(_OPENMP) && \
    ((defined(__clang_major__) && __clang_major__ >= 12) || \
     (!defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 9))
#define VSDLSS_DAG_TASKS 1
#endif

#ifdef VSDLSS_DAG_TASKS
/* Left-looking task graph: exactly one task per panel.  The task applies
 * every contribution that targets the panel, in increasing source order, and
 * then factors it, so the sequence of updates seen by each panel slot is the
 * same one the serial loop produces and results stay bitwise identical.
 *
 * The previous right-looking form created one task per (source, destination)
 * block, i.e. one per nonzero block of the assembly graph, each holding two
 * dependences.  Task creation and dependence resolution both take the team
 * task lock, so those thousands of microscopic tasks cost far more than the
 * arithmetic they carried; it ran 3-20x slower than the serial loop.  Panel
 * granularity keeps the same dependence edges but only `count` task objects,
 * and a panel's whole left-looking update is one unit of stealable work. */
static vsdlss_status factor_dag(vsdlss_sn_factor *f,const vsdlss_sn_symbolic *s,int nt)
{
    csi count=f->count,blocks=0;
    csi *owner=malloc((size_t)f->n*sizeof(*owner));
    csi *src_ptr=calloc((size_t)count+1,sizeof(*src_ptr));
    csi *src_of=NULL,*src_first=NULL,*src_end=NULL,*cursor=NULL;
    char *token=calloc((size_t)count,1);
    vsdlss_status *state=calloc((size_t)count,sizeof(*state));
    vsdlss_status result=VSDLSS_OK;
    if(!owner||!src_ptr||!token||!state)goto oom;
    for(csi sn=0;sn<count;sn++)
        for(csi j=f->column_start[sn];j<f->column_start[sn+1];j++)owner[j]=sn;
    /* Transpose the source-major block structure into destination-major
     * lists.  Appending while sn ascends keeps each list in source order. */
    for(csi sn=0;sn<count;sn++){
        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn];
        for(csi first=0;first<ext;){
            csi dest=owner[f->row_index[f->row_ptr[sn]+first]],end=first+1;
            while(end<ext&&owner[f->row_index[f->row_ptr[sn]+end]]==dest)end++;
            src_ptr[dest+1]++;blocks++;first=end;
        }
    }
    for(csi d=0;d<count;d++)src_ptr[d+1]+=src_ptr[d];
    src_of=malloc((size_t)(blocks?blocks:1)*sizeof(*src_of));
    src_first=malloc((size_t)(blocks?blocks:1)*sizeof(*src_first));
    src_end=malloc((size_t)(blocks?blocks:1)*sizeof(*src_end));
    cursor=malloc((size_t)count*sizeof(*cursor));
    if(!src_of||!src_first||!src_end||!cursor)goto oom;
    for(csi d=0;d<count;d++)cursor[d]=src_ptr[d];
    for(csi sn=0;sn<count;sn++){
        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn];
        for(csi first=0;first<ext;){
            csi dest=owner[f->row_index[f->row_ptr[sn]+first]],end=first+1;
            while(end<ext&&owner[f->row_index[f->row_ptr[sn]+end]]==dest)end++;
            csi at=cursor[dest]++;
            src_of[at]=sn;src_first[at]=first;src_end[at]=end;first=end;
        }
    }
    (void)nt;(void)token;
    VSDLSS_OMP(omp parallel num_threads(nt))
    {
        VSDLSS_OMP(omp master)
        {
            vsdlss_parallel_observe();
            for(csi d=0;d<count;d++){
                csi off=src_ptr[d],ns=src_ptr[d+1]-off;
                VSDLSS_OMP(omp task firstprivate(d,off,ns)
                           depend(iterator(csi i=0:ns),in:token[src_of[off+i]])
                           depend(inout:token[d]))
                {
                    vsdlss_status st=VSDLSS_OK;
                    for(csi t=off;st==VSDLSS_OK&&t<off+ns;t++){
                        csi sn=src_of[t];
                        if(state[sn]!=VSDLSS_OK){st=state[sn];break;}
                        csi w=f->column_start[sn+1]-f->column_start[sn];
                        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn];
                        int b=vsdlss_panel_update_range(f->panel+f->panel_offset[sn],
                                w+ext,w,ext,src_first[t],src_end[t],
                                s->update_target+s->update_ptr[sn],f->panel,
                                f->panel_offset[d],f->panel_offset[d+1]);
                        if(b&1)st=VSDLSS_ERR_INVALID;
                        else if(b&2)st=VSDLSS_ERR_NONFINITE;
                    }
                    if(st==VSDLSS_OK){
                        csi w=f->column_start[d+1]-f->column_start[d];
                        csi ext=f->row_ptr[d+1]-f->row_ptr[d];
                        st=vsdlss_panel_factor(f->panel+f->panel_offset[d],w+ext,w);
                    }
                    state[d]=st;
                }
            }
            VSDLSS_OMP(omp taskwait)
        }
    }
    for(csi d=0;d<count;d++)if(state[d]!=VSDLSS_OK){result=state[d];break;}
    goto done;
oom:
    result=VSDLSS_ERR_OOM;
done:
    free(owner);free(src_ptr);free(src_of);free(src_first);free(src_end);
    free(cursor);free(token);free(state);return result;
}
#endif

vsdlss_status vsdlss_sn_factorize(const vsdlss *A,const vsdlss_sn_symbolic *s,
                                  vsdlss_sn_factor **out)
{
    vsdlss_sn_factor *f=NULL; vsdlss_status st; csi k,j,sn;
    if(!out)return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(!A||A->m<1||A->n<1||A->m!=A->n)return VSDLSS_ERR_INVALID;
    /* validate_upper_csc and the retained-layout copies use dimension + 1. */
    if(A->n==INT64_MAX)return VSDLSS_ERR_OOM;
    st=vsdlss_validate_upper_csc(A); if(st!=VSDLSS_OK)return st;
    if(!s||s->n!=A->n||s->count<1||s->l_nnz<1)return VSDLSS_ERR_INVALID;
    if(s->n==INT64_MAX||s->count==INT64_MAX)return VSDLSS_ERR_OOM;
    if(
       !s->l_col_ptr||!s->l_row_index||!s->l_panel_slot||!s->column_start||
       !s->row_ptr||!s->panel_offset||!s->update_ptr||
       (s->row_ptr[s->count]&&!s->row_index)||
       (s->update_ptr[s->count]&&!s->update_target))return VSDLSS_ERR_INVALID;
    if(!bytes_ok(s->n+1,sizeof(csi))||!bytes_ok(s->count+1,sizeof(csi))||
       !bytes_ok(s->l_nnz,sizeof(csi))||
       !bytes_ok(s->panel_offset[s->count],sizeof(double))||
       !bytes_ok(s->row_ptr[s->count],sizeof(csi)))return VSDLSS_ERR_OOM;
    f=calloc(1,sizeof(*f));if(!f)return VSDLSS_ERR_OOM;
    f->n=s->n;f->count=s->count;f->l_nnz=s->l_nnz;
#define CPY(field,n_) do{f->field=copy_array(s->field,(n_),sizeof(*f->field));if((n_)&&!f->field){st=VSDLSS_ERR_OOM;goto fail;}}while(0)
    CPY(l_col_ptr,s->n+1);CPY(l_row_index,s->l_nnz);CPY(l_panel_slot,s->l_nnz);
    CPY(column_start,s->count+1);CPY(row_ptr,s->count+1);
    CPY(row_index,s->row_ptr[s->count]);CPY(panel_offset,s->count+1);
#undef CPY
    f->panel=calloc((size_t)s->panel_offset[s->count],sizeof(double));
    if(!f->panel){st=VSDLSS_ERR_OOM;goto fail;}
    for(j=0;j<A->n;j++)for(k=A->p[j];k<A->p[j+1];k++){
        csi e=find_entry(f,A->i[k],j),slot;
        if(e<0){st=VSDLSS_ERR_INVALID;goto fail;} slot=f->l_panel_slot[e];
        if(slot<0||slot>=f->panel_offset[f->count]){st=VSDLSS_ERR_INVALID;goto fail;}
        f->panel[slot]+=A->x[k];if(!isfinite(f->panel[slot])){st=VSDLSS_ERR_NONFINITE;goto fail;}
    }
#ifdef VSDLSS_DAG_TASKS
    int dag_threads=vsdlss_parallel_width((double)f->n*256);
    if(vsdlss_get_dag_enabled() && dag_threads>1 && f->count>1) {
        st=factor_dag(f,s,dag_threads);
        if(st!=VSDLSS_OK)goto fail;
        *out=f;return VSDLSS_OK;
    }
#endif
    for(sn=0;sn<f->count;sn++){
        csi begin=f->column_start[sn],w=f->column_start[sn+1]-begin;
        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn],rows=w+ext,base=f->panel_offset[sn];

        if(w<1||ext<0||rows<1){st=VSDLSS_ERR_INVALID;goto fail;}
        st=vsdlss_panel_factor(f->panel+base,rows,w);
        if(st!=VSDLSS_OK)goto fail;
        /* Chunk the external columns; the kernel blocks inside each chunk.
         * Column chunks own disjoint target slots, so no accumulation races
         * and no ordering dependence between chunks. */
        const csi chunk=VSDLSS_PANEL_UPDATE_CB;
        csi chunks=ext/chunk+(ext%chunk!=0);
        int nt=vsdlss_parallel_width((double)ext*ext*w),bad=0;
        if(nt>chunks)nt=(int)chunks;
        (void)nt;
        VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
        {
            VSDLSS_OMP(omp master)
            vsdlss_parallel_observe();
            VSDLSS_OMP(omp for schedule(dynamic,1))
            for(csi c=0;c<chunks;c++){
                csi c0=c*chunk,c1=c0+chunk;
                if(c1>ext)c1=ext;
                bad|=vsdlss_panel_update_range(f->panel+base,rows,w,ext,c0,c1,
                        s->update_target+s->update_ptr[sn],f->panel,
                        0,f->panel_offset[f->count]);
            }
        }
        if(bad){st=(bad&1)?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE;goto fail;}
    }
    *out=f;return VSDLSS_OK;
fail:vsdlss_sn_factor_free(f);return st;
}

vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *f,const double *rhs,double *out)
{
    double *x; csi sn,j;
    if(!f||!rhs||!out||f->n<1||!bytes_ok(f->n,sizeof(double)))return VSDLSS_ERR_INVALID;
    x=malloc((size_t)f->n*sizeof(*x));if(!x)return VSDLSS_ERR_OOM;
    for(j=0;j<f->n;j++){if(!isfinite(rhs[j])){free(x);return VSDLSS_ERR_NONFINITE;}x[j]=rhs[j];}
    for(int back=0;back<2;back++)for(csi t=0;t<f->count;t++){
        sn=back?f->count-1-t:t;
        csi b=f->column_start[sn],w=f->column_start[sn+1]-b;
        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn];
        vsdlss_status st=vsdlss_panel_solve(f->panel+f->panel_offset[sn],b,w,ext,
            ext?f->row_index+f->row_ptr[sn]:NULL,x,back);
        if(st!=VSDLSS_OK){free(x);return st;}
    }
    for(j=0;j<f->n;j++)if(!isfinite(x[j])){free(x);return VSDLSS_ERR_NONFINITE;}
    memcpy(out,x,(size_t)f->n*sizeof(*x));free(x);return VSDLSS_OK;
}

vsdlss_status vsdlss_sn_export_L(const vsdlss_sn_factor *f,vsdlss **out)
{
    vsdlss *L;csi k;
    if(!out)return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(!f||f->n<1||f->l_nnz<1)return VSDLSS_ERR_INVALID;
    L=vsdlss_spalloc(f->n,f->n,f->l_nnz,1,0);if(!L)return VSDLSS_ERR_OOM;
    memcpy(L->p,f->l_col_ptr,(size_t)(f->n+1)*sizeof(csi));
    memcpy(L->i,f->l_row_index,(size_t)f->l_nnz*sizeof(csi));
    for(k=0;k<f->l_nnz;k++)L->x[k]=f->panel[f->l_panel_slot[k]];
    *out=L;return VSDLSS_OK;
}
