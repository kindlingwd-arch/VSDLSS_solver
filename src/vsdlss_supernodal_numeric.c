#include "vsdlss_m3_internal.h"

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
    for(sn=0;sn<f->count;sn++){
        csi begin=f->column_start[sn],w=f->column_start[sn+1]-begin;
        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn],rows=w+ext,base=f->panel_offset[sn];
        csi col,row,t,u=0;
        if(w<1||ext<0||rows<1){st=VSDLSS_ERR_INVALID;goto fail;}
        for(col=0;col<w;col++){
            double d=f->panel[base+col*rows+col];
            for(t=0;t<col;t++){double v=f->panel[base+t*rows+col];d-=v*v;}
            if(!isfinite(d)){st=VSDLSS_ERR_NONFINITE;goto fail;}
            if(d<=0){st=VSDLSS_ERR_NOT_POSDEF;goto fail;}
            d=sqrt(d);f->panel[base+col*rows+col]=d;
            for(row=col+1;row<rows;row++){
                double v=f->panel[base+col*rows+row];
                for(t=0;t<col;t++)v-=f->panel[base+t*rows+row]*f->panel[base+t*rows+col];
                v/=d;if(!isfinite(v)){st=VSDLSS_ERR_NONFINITE;goto fail;}
                f->panel[base+col*rows+row]=v;
            }
        }
        for(col=0;col<ext;col++)for(row=col;row<ext;row++){
            csi target=s->update_target[s->update_ptr[sn]+u++]; double v=0;
            if(target<0||target>=f->panel_offset[f->count]){st=VSDLSS_ERR_INVALID;goto fail;}
            for(t=0;t<w;t++)v+=f->panel[base+t*rows+w+row]*f->panel[base+t*rows+w+col];
            f->panel[target]-=v;if(!isfinite(f->panel[target])){st=VSDLSS_ERR_NONFINITE;goto fail;}
        }
    }
    *out=f;return VSDLSS_OK;
fail:vsdlss_sn_factor_free(f);return st;
}

vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *f,const double *rhs,double *out)
{
    double *x; csi sn,j,r;
    if(!f||!rhs||!out||f->n<1||!bytes_ok(f->n,sizeof(double)))return VSDLSS_ERR_INVALID;
    x=malloc((size_t)f->n*sizeof(*x));if(!x)return VSDLSS_ERR_OOM;
    for(j=0;j<f->n;j++){if(!isfinite(rhs[j])){free(x);return VSDLSS_ERR_NONFINITE;}x[j]=rhs[j];}
    for(sn=0;sn<f->count;sn++){
        csi b=f->column_start[sn],w=f->column_start[sn+1]-b;
        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn],rows=w+ext,base=f->panel_offset[sn];
        for(j=0;j<w;j++){
            csi gj=b+j;x[gj]/=f->panel[base+j*rows+j];
            for(r=j+1;r<w;r++)x[b+r]-=f->panel[base+j*rows+r]*x[gj];
            for(r=0;r<ext;r++)x[f->row_index[f->row_ptr[sn]+r]]-=f->panel[base+j*rows+w+r]*x[gj];
        }
    }
    for(sn=f->count;sn-- >0;){
        csi b=f->column_start[sn],w=f->column_start[sn+1]-b;
        csi ext=f->row_ptr[sn+1]-f->row_ptr[sn],rows=w+ext,base=f->panel_offset[sn];
        for(j=w;j-- >0;){double v=x[b+j];
            for(r=j+1;r<w;r++)v-=f->panel[base+j*rows+r]*x[b+r];
            for(r=0;r<ext;r++)v-=f->panel[base+j*rows+w+r]*x[f->row_index[f->row_ptr[sn]+r]];
            x[b+j]=v/f->panel[base+j*rows+j];
        }
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
