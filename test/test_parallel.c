#include "../src/vsdlss_m3_internal.h"
#include "vsdlss.h"
#include <string.h>
#define CHECK(e) do{if(!(e)){fprintf(stderr,"FAIL parallel %d: %s\n",__LINE__,#e);return 1;}}while(0)
static int kernels(void)
{
    CHECK(vsdlss_get_num_threads()==1);
    CHECK(vsdlss_set_num_threads(0)==VSDLSS_ERR_INVALID);
    if(!vsdlss_parallel_enabled()){
        CHECK(vsdlss_set_num_threads(2)==VSDLSS_ERR_UNSUPPORTED);
        puts("parallel: serial build correctly rejects threads>1");return 0;
    }
    csi rows=512,w=96;size_t bytes=(size_t)rows*w*sizeof(double);
    double *a=calloc((size_t)rows*w,sizeof(double)),*b=malloc(bytes);CHECK(a&&b);
    for(csi j=0;j<w;j++)for(csi i=j;i<rows;i++)a[j*rows+i]=i==j?200:0.01/(1+i-j);
    memcpy(b,a,bytes);CHECK(vsdlss_panel_factor(a,rows,w)==VSDLSS_OK);
    CHECK(vsdlss_set_num_threads(4)==VSDLSS_OK);
    CHECK(vsdlss_panel_factor(b,rows,w)==VSDLSS_OK);
    CHECK(vsdlss_parallel_last_team_size()>1);
    CHECK(memcmp(a,b,bytes)==0);
    memset(b,0,bytes);b[0]=-1;
    CHECK(vsdlss_panel_factor(b,rows,w)==VSDLSS_ERR_NOT_POSDEF);
    free(a);free(b);return 0;
}

static vsdlss *matrix(csi n,csi components)
{
    vsdlss *A=vsdlss_spalloc(n,n,n*(n+1)/2,1,0);if(!A)return NULL;
    csi p=0;
    for(csi j=0;j<n;j++){
        A->p[j]=p;
        for(csi i=0;i<=j;i++)if(i%components==j%components){
            A->i[p]=i;A->x[p++]=i==j?200:0.01/(1+j-i);
        }
    }
    A->p[n]=p;return A;
}
static int components_and_rhs(void)
{
    const csi n=512,ld=515;vsdlss *A=matrix(n,4);CHECK(A);
    double *truth=malloc(n*8),*rhs=malloc(ld*4*8),*out=malloc(ld*4*8),*ref=malloc(ld*4*8);CHECK(truth&&rhs&&out&&ref);
    for(int r=0;r<4;r++){
        for(csi i=0;i<n;i++)truth[i]=(i%2?-1:1)*(0.5+i*0.01+r);
        CHECK(vsdlss_spmv_sym_upper(A,truth,rhs+r*ld)==VSDLSS_OK);
    }
    vsdlss_m3_factor *f=NULL;
    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    CHECK(vsdlss_factorize_m3(A,2,&f)==VSDLSS_OK);
    CHECK(vsdlss_m3_solve_many(f,4,rhs,ld,ref,ld)==VSDLSS_OK);
    vsdlss_m3_factor_free(f);f=NULL;
    for(int threads=2;threads<=4;threads*=2){
        CHECK(vsdlss_set_num_threads(threads)==VSDLSS_OK);
        CHECK(vsdlss_factorize_m3(A,2,&f)==VSDLSS_OK);
        CHECK(vsdlss_parallel_last_team_size()>1);
        CHECK(vsdlss_m3_solve_many(f,4,rhs,ld,out,ld)==VSDLSS_OK);
        for(int r=0;r<4;r++){
            CHECK(memcmp(ref+r*ld,out+r*ld,n*8)==0);
            double eta;CHECK(vsdlss_backward_error(A,out+r*ld,rhs+r*ld,&eta)==VSDLSS_OK&&eta<1e-12);
        }
        memcpy(out,rhs,(size_t)ld*4*8);
        CHECK(vsdlss_m3_solve_many(f,4,out,ld,out,ld)==VSDLSS_OK);
        for(int r=0;r<4;r++)CHECK(memcmp(out+r*ld,ref+r*ld,n*8)==0);
        CHECK(vsdlss_m3_solve_many(f,4,rhs,n-1,out,ld)==VSDLSS_ERR_INVALID);
        for(csi i=0;i<4*ld;i++)out[i]=91;
        rhs[ld+4]=NAN;
        CHECK(vsdlss_m3_solve_many(f,4,rhs,ld,out,ld)==VSDLSS_ERR_NONFINITE);
        for(csi i=0;i<4*ld;i++)CHECK(out[i]==91);
        rhs[ld+4]=0;
        vsdlss_m3_factor_free(f);f=NULL;
        /* Restore the known second RHS before the next comparison. */
        for(csi i=0;i<n;i++)truth[i]=(i%2?-1:1)*(1.5+i*0.01);
        CHECK(vsdlss_spmv_sym_upper(A,truth,rhs+ld)==VSDLSS_OK);
    }
    A->x[A->p[2+1]-1]=-1;
    CHECK(vsdlss_factorize_m3(A,2,&f)==VSDLSS_ERR_NOT_POSDEF&&f==NULL);
    free(truth);free(rhs);free(out);free(ref);vsdlss_spfree(A);return 0;
}
static int disk(void)
{
    const csi n=256;vsdlss *A=matrix(n,1);CHECK(A);
    double truth[256],rhs[256],ref[256],out[256];
    for(csi i=0;i<n;i++)truth[i]=(i%2?-1:1)*(0.5+i*0.01);
    CHECK(vsdlss_spmv_sym_upper(A,truth,rhs)==VSDLSS_OK);
    size_t memory=0;
    for(int threads=1;threads<=4;threads*=2){
        vsdlss_m4_factor *f=NULL;CHECK(vsdlss_set_num_threads(threads)==VSDLSS_OK);
        CHECK(vsdlss_factorize_m4_ex(A,2,1024*1024,NULL,64,&f)==VSDLSS_OK);
        if(threads>1)CHECK(vsdlss_parallel_last_team_size()>1);
        if(threads==1)memory=vsdlss_m4_workspace_bytes(f);
        CHECK(vsdlss_m4_workspace_bytes(f)==memory);
        CHECK(vsdlss_m4_solve(f,rhs,out)==VSDLSS_OK);
        for(csi i=0;i<n;i++)CHECK(isfinite(out[i])&&fabs(out[i]-truth[i])<1e-12);
        if(threads==1)memcpy(ref,out,sizeof(ref));else CHECK(memcmp(ref,out,sizeof(ref))==0);
        vsdlss_m4_factor_free(f);
    }
    vsdlss_spfree(A);return 0;
}
static int solve_kernel(void)
{
    const csi rows=1024,width=256,ext=rows-width;
    double *a=calloc((size_t)rows*width,8),*x=malloc(rows*8),*ref=malloc(rows*8);
    csi *index=malloc(ext*8);CHECK(a&&x&&ref&&index);
    for(csi j=0;j<width;j++)for(csi i=j;i<rows;i++)a[j*rows+i]=i==j?2:0.0001;
    for(csi i=0;i<ext;i++)index[i]=width+i;
    for(int back=0;back<2;back++){
        for(csi i=0;i<rows;i++)x[i]=ref[i]=1+i*0.01;
        CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
        CHECK(vsdlss_panel_solve(a,0,width,ext,index,ref,back)==VSDLSS_OK);
        CHECK(vsdlss_set_num_threads(4)==VSDLSS_OK);
        CHECK(vsdlss_panel_solve(a,0,width,ext,index,x,back)==VSDLSS_OK);
        CHECK(vsdlss_parallel_last_team_size()>1&&memcmp(x,ref,rows*8)==0);
    }
    free(a);free(x);free(ref);free(index);return 0;
}
int main(void)
{
    CHECK(kernels()==0);
    if(vsdlss_parallel_enabled()){
        CHECK(components_and_rhs()==0);CHECK(disk()==0);CHECK(solve_kernel()==0);
    }
    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    puts("test_parallel: ALL OK");return 0;
}
