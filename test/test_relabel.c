/* Solve-order relabel (VSDLSS_RELABEL / vsdlss_m3_relabel): solutions are
 * bitwise identical with and without it (caller order, many RHS, 1 and 4
 * threads); large components get the contiguous core; the packed order
 * stays a permutation and packed / in-place solves agree with the
 * caller-order solve; non-finite RHS is still reported. */
#include "../src/vsdlss_m3_internal.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
extern int vsdlss_m3_relabel;
static int fails;
#define CHECK(c,msg,...) do{ if(!(c)){ fails++; fprintf(stderr,"FAIL: " msg "\n",__VA_ARGS__); } }while(0)

static vsdlss *grid2(csi R,csi C)
{
    csi n=R*C,nz=0; vsdlss *A=vsdlss_spalloc(n,n,3*n,1,0);
    for(csi j=0;j<n;j++){ A->p[j]=nz; csi r=j/C,c=j%C;
        if(c>0&&(j%7)){A->i[nz]=j-1;A->x[nz++]=-1;} if(r>0&&(j%5)){A->i[nz]=j-C;A->x[nz++]=-1.3;}
        A->i[nz]=j;A->x[nz++]=4.5+(j%3); }
    A->p[n]=nz; return A;
}
static vsdlss *grid3(csi m)
{
    csi n=m*m*m,nz=0; vsdlss *A=vsdlss_spalloc(n,n,4*n,1,0);
    for(csi j=0;j<n;j++){ A->p[j]=nz; csi x=j%m,y=(j/m)%m,z=j/(m*m);
        if(x>0){A->i[nz]=j-1;A->x[nz++]=-1;} if(y>0){A->i[nz]=j-m;A->x[nz++]=-1;} if(z>0){A->i[nz]=j-m*m;A->x[nz++]=-1;}
        A->i[nz]=j;A->x[nz++]=6.01; }
    A->p[n]=nz; return A;
}

/* A relabelled reduction keeps 1-byte degrees instead of head words.  The
 * same records in head form (vertex k0+i) must replay bitwise identically:
 * in-place forward/backward and the cursor paths of reduce_rhs/recover. */
static void compact_form(const vsdlss_m3_factor *f,const char *name)
{
    for(csi c=0;c<f->count;c++){
        const vsdlss_m3_component_factor *cf=f->component+c;
        const vsdlss_reduction *r=cf->reduction;
        if(!cf->core_contig || !r->count) continue;
        int compact=1; for(csi q=0;q<r->pk_count;q++) if(r->pk[q].count && (r->pk[q].head || !r->pk[q].deg)) compact=0;
        CHECK(compact,"%s relabelled component %lld not in 1-byte degree form",name,(long long)c);
        if(!compact) continue;
        const csi n=r->n,cnt=r->count,core=r->core_n;
        vsdlss_pk_seg *seg=malloc((size_t)r->pk_count*sizeof(*seg));
        csi *cv=malloc((size_t)(core?core:1)*sizeof(csi));
        for(csi k=0;k<core;k++) cv[k]=cnt+k;
        for(csi q=0;q<r->pk_count;q++){
            seg[q]=r->pk[q]; seg[q].deg=NULL;
            seg[q].head=malloc((size_t)(seg[q].count?seg[q].count:1)*4);
            for(csi i=0;i<seg[q].count;i++) seg[q].head[i]=(uint32_t)(seg[q].k0+i)|((uint32_t)r->pk[q].deg[i]<<30);
        }
        vsdlss_reduction a=*r,h=*r; a.core_vertices=cv; h.core_vertices=cv; h.pk=seg;
        double *b=malloc(n*8),*w1=malloc(n*8),*w2=malloc(n*8),*c1=malloc((core+1)*8),*c2=malloc((core+1)*8);
        double *s1=malloc(cnt*8),*s2=malloc(cnt*8);
        for(csi i=0;i<n;i++) b[i]=cos(0.29*(double)i)+0.4;
        memcpy(w1,b,n*8); memcpy(w2,b,n*8);
        CHECK(vsdlss_reduce_forward_inplace(&a,w1,NULL)==VSDLSS_OK && vsdlss_reduce_forward_inplace(&h,w2,NULL)==VSDLSS_OK,"%s fwd",name);
        CHECK(!memcmp(w1,w2,n*8),"%s compact forward differs",name);
        CHECK(vsdlss_reduce_backward_inplace(&a,NULL,w1)==VSDLSS_OK && vsdlss_reduce_backward_inplace(&h,NULL,w2)==VSDLSS_OK,"%s bwd",name);
        CHECK(!memcmp(w1,w2,n*8),"%s compact backward differs",name);
        CHECK(vsdlss_reduce_rhs(&a,b,c1,s1)==VSDLSS_OK && vsdlss_reduce_rhs(&h,b,c2,s2)==VSDLSS_OK,"%s rhs",name);
        CHECK(!memcmp(c1,c2,core*8) && !memcmp(s1,s2,cnt*8),"%s compact cursor rhs differs",name);
        CHECK(vsdlss_reduce_recover(&a,s1,c1,w1)==VSDLSS_OK && vsdlss_reduce_recover(&h,s2,c2,w2)==VSDLSS_OK,"%s recover",name);
        CHECK(!memcmp(w1,w2,n*8),"%s compact cursor recover differs",name);
        for(csi q=0;q<r->pk_count;q++) free(seg[q].head);
        free(seg);free(cv);free(b);free(w1);free(w2);free(c1);free(c2);free(s1);free(s2);
    }
}

static void run(vsdlss *A,const char *name,int threads)
{
    csi n=A->n;
    vsdlss_set_num_threads(threads);
    double *b=malloc(2*n*8),*x0=malloc(2*n*8),*x1=malloc(2*n*8),*pb=calloc(n,8),*px=malloc(n*8);
    csi *pp=malloc(n*sizeof(csi)); unsigned char *seen=calloc(n,1);
    for(csi i=0;i<2*n;i++) b[i]=sin(0.37*(double)i)+0.1;
    vsdlss_m3_factor *f0=NULL,*f1=NULL;
    vsdlss_m3_relabel=0; CHECK(vsdlss_factorize_m3(A,5,&f0)==VSDLSS_OK,"%s factor0",name);
    vsdlss_m3_relabel=1; CHECK(vsdlss_factorize_m3(A,5,&f1)==VSDLSS_OK,"%s factor1",name);
    int contig=0; for(csi c=0;c<f1->count;c++) contig+=f1->component[c].core_contig;
    for(csi c=0;c<f0->count;c++) CHECK(!f0->component[c].core_contig,"%s relabel off but contiguous",name);
    CHECK(contig>0,"%s no relabelled component",name);
    for(csi c=0;c<f0->count;c++) for(csi q=0;q<f0->component[c].reduction->pk_count;q++)
        CHECK(!f0->component[c].reduction->pk[q].deg,"%s relabel off but compact records",name);
    compact_form(f1,name);
    CHECK(vsdlss_m3_solve(f0,b,x0)==VSDLSS_OK && vsdlss_m3_solve(f1,b,x1)==VSDLSS_OK,"%s solve",name);
    CHECK(!memcmp(x0,x1,n*8),"%s relabel changed the solution",name);
    CHECK(vsdlss_m3_solve_many(f0,2,b,n,x0,n)==VSDLSS_OK && vsdlss_m3_solve_many(f1,2,b,n,x1,n)==VSDLSS_OK,"%s many",name);
    CHECK(!memcmp(x0,x1,2*n*8),"%s relabel changed solve_many",name);
    CHECK(vsdlss_m3_export_packed_permutation(f1,pp,n)==VSDLSS_OK,"%s export",name);
    int perm_ok=1; for(csi q=0;q<n;q++){ if(pp[q]<0||pp[q]>=n||seen[pp[q]]) perm_ok=0; else seen[pp[q]]=1; }
    CHECK(perm_ok,"%s packed order not a permutation",name);
    if(perm_ok){
        for(csi q=0;q<n;q++) pb[q]=b[pp[q]];
        CHECK(vsdlss_m3_solve_packed(f1,pb,px)==VSDLSS_OK,"%s packed",name);
        int eq=1; for(csi q=0;q<n;q++) if(memcmp(&px[q],&x1[pp[q]],8)) eq=0;
        CHECK(eq,"%s packed differs from caller order",name);
        CHECK(vsdlss_m3_solve_packed_inplace(f1,pb)==VSDLSS_OK && !memcmp(pb,px,n*8),"%s in-place",name);
    }
    memcpy(x1,b,n*8); x1[n/3]=0.0/0.0;
    CHECK(vsdlss_m3_solve(f1,x1,x0)==VSDLSS_ERR_NONFINITE,"%s nonfinite",name);
    printf("  %s threads %d: %d of %lld components relabelled, bitwise equal\n",name,threads,contig,(long long)f1->count);
    vsdlss_m3_factor_free(f0); vsdlss_m3_factor_free(f1);
    free(b);free(x0);free(x1);free(pb);free(px);free(pp);free(seen);
}

int main(void)
{
    vsdlss *A=grid2(120,130), *B=grid3(22);
    run(A,"grid2",1); run(B,"grid3",1); run(A,"grid2",4); run(B,"grid3",4);
    vsdlss_spfree(A); vsdlss_spfree(B);
    vsdlss_m3_relabel=-1;
    if(fails){ printf("test_relabel: %d failures\n",fails); return 1; }
    puts("test_relabel: solve-order relabel bitwise identical: passed");
    return 0;
}
