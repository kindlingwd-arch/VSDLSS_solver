/* Selective inversion of diagonal blocks (vsdlss_m3_selinv):
 * results agree with the triangular solve to rounding for several width
 * ranges, on a 2D grid with dangling nodes (narrow panels) and a 3D grid
 * (wide separators, external rows > 1024); caller-order, packed, in-place
 * and multi-RHS solves; 1 and 4 threads; argument checks and refusals. */
#include "../src/vsdlss_m3_internal.h"
#include "../src/vsdlss_parallel.h"
#include <stdio.h>
#include <string.h>
static int fails;
#define CHECK(c,msg,...) do{ if(!(c)){ fails++; fprintf(stderr,"FAIL: " msg "\n",__VA_ARGS__); } }while(0)

static vsdlss *grid2(void)
{
    csi R=120,C=130,n=R*C,nz=0; vsdlss *A=vsdlss_spalloc(n,n,3*n,1,0);
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
static double rel(const double *a,const double *b,csi n)
{ double num=0,den=0; for(csi i=0;i<n;i++){ num=fmax(num,fabs(a[i]-b[i])); den=fmax(den,fabs(b[i])); } return num/den; }

static void run(vsdlss *A,const char *name,long long lo,long long hi,int threads)
{
    csi n=A->n;
    vsdlss_set_num_threads(threads);
    vsdlss_m3_factor *f=NULL;
    CHECK(vsdlss_factorize_m3(A,5,&f)==VSDLSS_OK,"%s factor",name);
    double *b=malloc(n*8*2),*x0=malloc(n*8*2),*x1=malloc(n*8*2),*p0=malloc(n*8),*p1=malloc(n*8);
    for(csi i=0;i<2*n;i++) b[i]=sin(0.37*(double)i)+0.1;
    CHECK(vsdlss_m3_solve(f,b,x0)==VSDLSS_OK,"%s ref",name);
    CHECK(vsdlss_m3_solve(f,b+n,x0+n)==VSDLSS_OK,"%s ref2",name);
    CHECK(vsdlss_m3_solve_packed(f,b,p0)==VSDLSS_OK,"%s refp",name);
    csi maxw=0; for(csi c=0;c<f->count;c++){ const vsdlss_sn_factor *s=f->component[c].numeric; if(s) for(csi q=0;q<s->count;q++){ csi w=s->column_start[q+1]-s->column_start[q]; if(w>maxw)maxw=w; } }
    CHECK(vsdlss_m3_selinv(f,lo,hi)==VSDLSS_OK,"%s selinv %lld:%lld",name,lo,hi);
    CHECK(vsdlss_m3_selinv(f,lo,hi)==VSDLSS_ERR_INVALID,"%s selinv twice",name);
    CHECK(vsdlss_m3_solve(f,b,x1)==VSDLSS_OK,"%s solve",name);
    double r=rel(x1,x0,n), eta=1;
    CHECK(r<1e-12,"%s %lld:%lld t%d rel %.2e",name,lo,hi,threads,r);
    double eta0=1; vsdlss_backward_error(A,x0,b,&eta0);
    vsdlss_backward_error(A,x1,b,&eta);
    CHECK(eta<=4*eta0+1e-16,"%s berr %.2e (triangular %.2e)",name,eta,eta0);
    CHECK(vsdlss_m3_solve_packed(f,b,p1)==VSDLSS_OK,"%s packed",name);
    CHECK(rel(p1,p0,n)<1e-12,"%s packed rel %.2e",name,rel(p1,p0,n));
    { double *pb=calloc(n,8), *pr=malloc(n*8); csi *pp=malloc(n*sizeof(csi));
      if(!pb||!pr||!pp){ fails++; return; }
      vsdlss_m3_export_packed_permutation(f,pp,n);
      for(csi q=0;q<n;q++) pb[q]=b[pp[q]];
      memcpy(pr,pb,n*8);
      CHECK(vsdlss_m3_solve_packed_inplace(f,pr)==VSDLSS_OK,"%s inplace",name);
      for(csi q=0;q<n;q++) p1[q]=x1[pp[q]];
      CHECK(!memcmp(pr,p1,n*8),"%s inplace vs caller-order (same kernels)",name);
      free(pb);free(pr);free(pp); }
    CHECK(vsdlss_m3_solve_many(f,2,b,n,x1,n)==VSDLSS_OK,"%s batch",name);
    CHECK(rel(x1+n,x0+n,n)<1e-12,"%s batch rel",name);
    memcpy(x1,b,n*8); x1[n/2]=1.0/0.0;
    CHECK(vsdlss_m3_solve(f,x1,p1)==VSDLSS_ERR_NONFINITE,"%s nonfinite",name);
    vsdlss *L=NULL;
    int any=0; for(csi c=0;c<f->count;c++){ vsdlss_sn_factor *s=f->component[c].numeric; if(s&&s->selinv_hi){ any=1; CHECK(vsdlss_sn_export_L(s,&L)==VSDLSS_ERR_INVALID&&!L,"%s export refused",name); } }
    CHECK(any,"%s no numeric component",name);
    printf("  %s selinv %lld:%lld threads %d max_width %lld: rel %.1e berr %.1e (triangular %.1e)\n",name,lo,hi,threads,(long long)maxw,r,eta,eta0);
    vsdlss_m3_factor_free(f); free(b);free(x0);free(x1);free(p0);free(p1);
}

int main(void)
{
    vsdlss *A=grid2(), *B=grid3(28);
    const long long R[][2]={{7,31},{1,6},{1,1000000},{32,127},{128,1000000}};
    for(unsigned k=0;k<sizeof R/sizeof*R;k++){ run(A,"grid2",R[k][0],R[k][1],1); run(B,"grid3",R[k][0],R[k][1],1); }
    run(B,"grid3",1,1000000,4);
    vsdlss_set_num_threads(1);
    vsdlss_m3_factor *f=NULL; vsdlss_factorize_m3(A,5,&f);
    CHECK(vsdlss_m3_selinv(NULL,1,2)==VSDLSS_ERR_INVALID,"%s","null");
    CHECK(vsdlss_m3_selinv(f,0,2)==VSDLSS_ERR_INVALID,"%s","wmin 0");
    CHECK(vsdlss_m3_selinv(f,5,4)==VSDLSS_ERR_INVALID,"%s","wmax<wmin");
    vsdlss_m3_factor_free(f); vsdlss_spfree(A); vsdlss_spfree(B);
    if(fails){ printf("test_selinv: %d failures\n",fails); return 1; }
    puts("test_selinv: selective inversion within rounding of the triangular solve: passed");
    return 0;
}
