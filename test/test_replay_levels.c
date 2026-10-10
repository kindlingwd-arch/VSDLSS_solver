#define _POSIX_C_SOURCE 200809L
#include "replay_fixture.h"
#include "../src/vsdlss_parallel.h"
#include <stdio.h>
#include "m3_test_alloc.h"
#define CHECK(c) do {if(!(c)){fprintf(stderr,"replay levels line %d: %s\n",__LINE__,#c);return 1;}}while(0)
static int integration(int degree)
{
    const csi width=4096,depth=16,leaves=width*depth,n=leaves+width;
    vsdlss *T=vsdlss_spalloc(n,n,5*n,1,1);double *diag=calloc((size_t)n,sizeof(double));CHECK(T&&diag);
    /* Four-regular core with independent leaves. Core degree stays >=4,
     * making a genuinely wide tail in the normal factorization path. */
    for(csi prev=0;prev<n;prev++) for(int d=1;d<=(prev<leaves?degree:2);d++) {
        csi next=prev<leaves?leaves+(prev%(width/degree))*degree+d-1:leaves+(prev-leaves+d)%width;
        T->i[T->nz]=prev<next?prev:next;T->p[T->nz]=prev<next?next:prev;T->x[T->nz++]=-1;
        diag[prev]++;diag[next]++;
    }
    for(csi j=0;j<n;j++) {T->i[T->nz]=j;T->p[T->nz]=j;T->x[T->nz++]=diag[j]+0.1;}
    vsdlss *A=vsdlss_spalloc(n,n,T->nz,1,0);csi *pos=calloc((size_t)n,sizeof(csi));CHECK(A&&pos);
    for(csi k=0;k<T->nz;k++)pos[T->p[k]]++;
    A->p[0]=0;for(csi j=0;j<n;j++) {A->p[j+1]=A->p[j]+pos[j];pos[j]=A->p[j];}
    for(csi k=0;k<T->nz;k++) {csi p=pos[T->p[k]]++;A->i[p]=T->i[k];A->x[p]=T->x[k];}
    vsdlss_spfree(T);free(diag);free(pos);
    double *b=malloc((size_t)n*8),*x=malloc((size_t)n*8),*ref=malloc((size_t)n*8);CHECK(b&&x&&ref);
    for(csi j=0;j<n;j++) b[j]=sin(j*0.11)+0.2;
    vsdlss_m3_factor *F=NULL,*legacy=NULL;vsdlss_set_num_threads(1);
    setenv("VSDLSS_REPLAY_LEVELS","0",1);CHECK(vsdlss_factorize_m3(A,5,&legacy)==VSDLSS_OK);
    setenv("VSDLSS_REPLAY_LEVELS","1",1);
    CHECK(vsdlss_factorize_m3(A,5,&F)==VSDLSS_OK);
    csi retained=0;
    for(csi c=0;c<F->count;c++) if(F->component[c].reduction) {
        const vsdlss_reduction *r=F->component[c].reduction;
        for(csi q=0;q<r->pk_count;q++) if(r->pk[q].levels)retained+=r->pk[q].count;
    }
    CHECK(!vsdlss_parallel_enabled()||retained>32768);
    for(int rhs=0;rhs<3;rhs++) {
        for(csi j=0;j<n;j++) b[j]+=0.01;
        setenv("VSDLSS_REPLAY_LEVELS","0",1);CHECK(vsdlss_m3_solve(legacy,b,ref)==VSDLSS_OK);
        setenv("VSDLSS_REPLAY_LEVELS","1",1);vsdlss_set_num_threads(4);
        CHECK(vsdlss_m3_solve(F,b,x)==VSDLSS_OK);CHECK(!memcmp(ref,x,(size_t)n*8));
        double eta=1;CHECK(vsdlss_backward_error(A,x,b,&eta)==VSDLSS_OK && eta<1e-12);
    }
    vsdlss_m3_factor_free(legacy);vsdlss_m3_factor_free(F);vsdlss_spfree(A);free(b);free(x);free(ref);return 0;
}
int main(void)
{
    setenv("VSDLSS_REPLAY_LEVELS","1",1);
    for(int pack=0;pack<2;pack++) for(int blocked=0;blocked<2;blocked++) for(int relabel=0;relabel<2;relabel++) for(int overlap=0;overlap<3;overlap++) {
        vsdlss_reduction *r=replay_fixture(4096,12,relabel,overlap); CHECK(r);
        if(blocked) CHECK(replay_prepend_block(r));
        size_t bytes=(size_t)r->n*8, sb=(size_t)r->count*8;
        double *b=malloc(bytes),*ref=malloc(bytes),*w=malloc(bytes),*fw=malloc(bytes),*s=malloc(sb),*sr=malloc(sb);
        CHECK(b&&ref&&w&&fw&&s&&sr);
        for(csi i=0;i<r->n;i++) b[i]=0.2+sin(i*0.13);
        vsdlss_set_num_threads(1); memcpy(ref,b,bytes);
        CHECK(vsdlss_reduce_forward_inplace(r,ref,sr)==VSDLSS_OK); memcpy(fw,ref,bytes);
        CHECK(vsdlss_reduce_backward_inplace(r,sr,ref)==VSDLSS_OK);
        CHECK(vsdlss_reduce_build_levels(r)==VSDLSS_OK);
        CHECK(!vsdlss_parallel_enabled() || r->pk[blocked].levels);
        if(pack) {
            CHECK(vsdlss_reduce_pack_levels(r)==VSDLSS_OK);
            /* saved[] follows the reordered records; the physical vector
             * and final answer must still match the original serial path. */
            memcpy(w,b,bytes);CHECK(vsdlss_reduce_forward_inplace(r,w,sr)==VSDLSS_OK);
            CHECK(!memcmp(w,fw,bytes));
        }
        for(int nt=1;nt<=4;nt*=2) for(int saved=0;saved<2;saved++) {
            if(nt>1&&!vsdlss_parallel_enabled())continue;
            CHECK(vsdlss_set_num_threads(nt)==VSDLSS_OK); memcpy(w,b,bytes);
            CHECK(vsdlss_reduce_forward_inplace(r,w,saved?s:NULL)==VSDLSS_OK);
            CHECK(memcmp(w,fw,bytes)==0);
            if(saved) CHECK(memcmp(s,sr,sb)==0);
            CHECK(vsdlss_reduce_backward_inplace(r,saved?s:NULL,w)==VSDLSS_OK);
            CHECK(memcmp(w,ref,bytes)==0);
            if(nt>1) CHECK(vsdlss_parallel_last_team_size()>1);
        }
        /* The fast path must still report nonfinite recovered values. */
        memcpy(w,fw,bytes); w[r->n-1]=INFINITY;
        CHECK(vsdlss_reduce_backward_inplace(r,NULL,w)==VSDLSS_ERR_NONFINITE);
        free(b);free(ref);free(w);free(fw);free(s);free(sr);vsdlss_reduction_free(r);
    }
    /* A long narrow chain must retain no schedule. */
    vsdlss_reduction *chain=replay_fixture(1,32768,1,0);CHECK(chain);
    CHECK(vsdlss_reduce_build_levels(chain)==VSDLSS_OK && !chain->pk[0].levels);
    vsdlss_reduction_free(chain);
    if(vsdlss_parallel_enabled()) {
        size_t base=m3_alloc_live();
        vsdlss_reduction *r=replay_fixture(4096,12,1,0);CHECK(r);
        m3_alloc_reset();CHECK(vsdlss_reduce_build_levels(r)==VSDLSS_OK);
        size_t calls=m3_alloc_calls();CHECK(calls>0);vsdlss_reduction_free(r);
        CHECK(m3_alloc_live()==base);
        for(size_t k=1;k<=calls;k++) {
            m3_alloc_reset();r=replay_fixture(4096,12,1,0);CHECK(r);
            size_t retained=m3_alloc_live();m3_alloc_fail_at(k);
            CHECK(vsdlss_reduce_build_levels(r)==VSDLSS_ERR_OOM);
            CHECK(!r->pk[0].levels && m3_alloc_live()==retained);
            m3_alloc_reset();CHECK(vsdlss_reduce_build_levels(r)==VSDLSS_OK);
            vsdlss_reduction_free(r);CHECK(m3_alloc_live()==base);
        }
        m3_alloc_reset();r=replay_fixture(4096,12,1,1);CHECK(r);
        CHECK(vsdlss_reduce_build_levels(r)==VSDLSS_OK);m3_alloc_reset();
        CHECK(vsdlss_reduce_pack_levels(r)==VSDLSS_OK);calls=m3_alloc_calls();CHECK(calls>0);
        vsdlss_reduction_free(r);CHECK(m3_alloc_live()==base);
        for(size_t k=1;k<=calls;k++) {
            m3_alloc_reset();r=replay_fixture(4096,12,1,1);CHECK(r);
            CHECK(vsdlss_reduce_build_levels(r)==VSDLSS_OK);size_t retained=m3_alloc_live();
            m3_alloc_fail_at(k);CHECK(vsdlss_reduce_pack_levels(r)==VSDLSS_ERR_OOM);
            CHECK(r->pk[0].levels->ref && !r->pk[0].levels->tile && m3_alloc_live()==retained);
            m3_alloc_reset();CHECK(vsdlss_reduce_pack_levels(r)==VSDLSS_OK);
            vsdlss_reduction_free(r);CHECK(m3_alloc_live()==base);
        }
    }
    for(int degree=1;degree<=3;degree++) CHECK(integration(degree)==0);
    puts("replay levels: packed layouts, blocked tails, degree 0..3, narrow levels, OOM, M3 integration, 1/2/4 threads bitwise identical: PASS");
    return 0;
}
