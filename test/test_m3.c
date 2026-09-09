#include "../src/vsdlss_m3_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define CHECK(e) do { if (!(e)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #e); return 1; } } while (0)

static int check_csc(const vsdlss *A, csi n, const csi *p, const csi *i,
                     const double *x, csi nz)
{
    csi k;
    if (!A || A->m != n || A->n != n || A->nz != -1 || A->p[n] != nz) return 0;
    for (k = 0; k <= n; ++k) if (A->p[k] != p[k]) return 0;
    for (k = 0; k < nz; ++k) if (A->i[k] != i[k] || A->x[k] != x[k]) return 0;
    return 1;
}

static int test_interleaved_components_and_extract(void)
{
    csi p[] = {0,1,2,4,6}, i[] = {0,1,0,2,1,3};
    double x[] = {4,5,-1,4,-2,5};
    vsdlss A = {6,4,4,p,i,x,-1}, *B = NULL;
    vsdlss_components *c = NULL;
    const csi ep[] = {0,1,3}, ei0[] = {0,0,1}, ei1[] = {0,0,1};
    const double ex0[] = {4,-1,4}, ex1[] = {5,-2,5};
    CHECK(vsdlss_components_build(&A,&c)==VSDLSS_OK);
    CHECK(c->count==2 && c->offset[0]==0 && c->offset[1]==2 && c->offset[2]==4);
    CHECK(c->vertices[0]==0 && c->vertices[1]==2);
    CHECK(c->vertices[2]==1 && c->vertices[3]==3);
    CHECK(c->component_of[0]==0 && c->local_of[0]==0);
    CHECK(c->component_of[3]==1 && c->local_of[3]==1);
    CHECK(vsdlss_component_extract(&A,c,0,&B)==VSDLSS_OK);
    CHECK(check_csc(B,2,ep,ei0,ex0,3)); vsdlss_spfree(B); B=NULL;
    CHECK(vsdlss_component_extract(&A,c,1,&B)==VSDLSS_OK);
    CHECK(check_csc(B,2,ep,ei1,ex1,3)); vsdlss_spfree(B);
    vsdlss_components_free(c);
    return 0;
}

static int test_isolate_single_component_and_zero_edge(void)
{
    csi p[] = {0,1,3,5}, i[] = {0,0,1,1,2};
    double x[] = {2,0,2,1,2};
    vsdlss A = {5,3,3,p,i,x,-1};
    vsdlss_components *c = NULL;
    CHECK(vsdlss_components_build(&A,&c)==VSDLSS_OK);
    CHECK(c->count==1); /* explicit structural zero 0--1 connects */
    CHECK(c->vertices[0]==0 && c->vertices[1]==1 && c->vertices[2]==2);
    vsdlss_components_free(c);
    {
        csi qp[] = {0,1,2,3}, qi[] = {0,1,2}; double qx[] = {1,2,3};
        vsdlss Q = {3,3,3,qp,qi,qx,-1};
        CHECK(vsdlss_components_build(&Q,&c)==VSDLSS_OK);
        CHECK(c->count==3 && c->offset[1]==1 && c->offset[2]==2);
        vsdlss_components_free(c);
    }
    return 0;
}

static int test_invalid_inputs_leave_outputs_null(void)
{
    csi p[] = {0,1,1}, i[] = {1}; double x[] = {1};
    vsdlss bad = {1,2,2,p,i,x,-1}, *B = (vsdlss *)1;
    vsdlss_components *c = (vsdlss_components *)1;
    CHECK(vsdlss_components_build(&bad,&c)==VSDLSS_ERR_INVALID && c==NULL);
    CHECK(vsdlss_components_build(NULL,&c)==VSDLSS_ERR_INVALID && c==NULL);
    CHECK(vsdlss_components_build(&bad,NULL)==VSDLSS_ERR_INVALID);
    CHECK(vsdlss_component_extract(&bad,NULL,0,&B)==VSDLSS_ERR_INVALID && B==NULL);
    {
        csi dp[] = {0,1,0}, di[] = {0}; double dx[] = {1};
        vsdlss decreasing = {1,2,2,dp,di,dx,-1};
        CHECK(vsdlss_components_build(&decreasing,&c)==VSDLSS_ERR_INVALID && c==NULL);
    }
    {
        csi hp[] = {0}; csi hi[] = {0}; double hx[] = {0};
        vsdlss huge = {0,INT64_MAX,INT64_MAX,hp,hi,hx,-1};
        csi offset[] = {0,1}, vertex[] = {0}, map[] = {0};
        vsdlss_components fake = {INT64_MAX,1,offset,vertex,map,map};
        CHECK(vsdlss_components_build(&huge,&c)==VSDLSS_ERR_OOM && c==NULL);
        CHECK(vsdlss_component_extract(&huge,&fake,0,&B)==VSDLSS_ERR_OOM && B==NULL);
    }
    return 0;
}

static int test_large_sparse_diagonal(void)
{
    const csi n = 100000;
    csi *p = malloc((size_t)(n + 1) * sizeof(*p));
    csi *i = malloc((size_t)n * sizeof(*i));
    double *x = malloc((size_t)n * sizeof(*x));
    vsdlss_components *c = NULL;
    csi k;
    CHECK(p && i && x);
    for (k = 0; k < n; ++k) { p[k] = k; i[k] = k; x[k] = 1.0; }
    p[n] = n;
    {
        vsdlss A = {n,n,n,p,i,x,-1};
        CHECK(vsdlss_components_build(&A,&c)==VSDLSS_OK);
        CHECK(c->count==n && c->offset[n]==n);
        CHECK(c->vertices[n-1]==n-1 && c->local_of[n-1]==0);
    }
    vsdlss_components_free(c); free(p); free(i); free(x);
    return 0;
}

static int test_large_sparse_connected_path(void)
{
    const csi n = 100000, nz = 2 * n - 1;
    csi *p = malloc((size_t)(n + 1) * sizeof(*p));
    csi *i = malloc((size_t)nz * sizeof(*i));
    double *x = malloc((size_t)nz * sizeof(*x));
    vsdlss_components *c = NULL;
    csi col, at = 0;
    CHECK(p && i && x);
    p[0] = 0;
    for (col = 0; col < n; ++col) {
        if (col > 0) { i[at] = col - 1; x[at++] = -1.0; }
        i[at] = col; x[at++] = 2.0; p[col + 1] = at;
    }
    {
        vsdlss A = {nz,n,n,p,i,x,-1};
        CHECK(vsdlss_components_build(&A,&c)==VSDLSS_OK);
        CHECK(c->count==1 && c->offset[1]==n);
        CHECK(c->vertices[n-1]==n-1 && c->local_of[n-1]==n-1);
    }
    vsdlss_components_free(c); free(p); free(i); free(x);
    return 0;
}

static int test_reduction_hand_fixture(void)
{
    csi p[]={0,1,3,5}, i[]={0,0,1,1,2};
    double x[]={4,2,5,1,3};
    vsdlss A={5,3,3,p,i,x,-1}; vsdlss_reduction *r=NULL;
    CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK);
    CHECK(r->count==3 && r->core_n==0);
    CHECK(r->records[0].vertex==0 && r->records[0].degree==1);
    CHECK(r->records[0].pivot==4 && r->records[0].multiplier[0]==0.5);
    CHECK(r->records[1].vertex==1 && r->records[1].pivot==4);
    CHECK(r->records[2].vertex==2 && r->records[2].pivot==2.75);
    vsdlss_reduction_free(r); return 0;
}

static int test_reduction_dynamic_structure(void)
{
    /* A four-cycle: eliminating 0 fills 1--3, so vertex 1 stays degree two. */
    csi p[]={0,1,3,5,8}, i[]={0,0,1,1,2,0,2,3};
    double x[]={4,1,4,1,4,1,1,4};
    vsdlss A={8,4,4,p,i,x,-1}; vsdlss_reduction *r=NULL;
    CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK);
    CHECK(r->count==4 && r->records[0].vertex==0 && r->records[0].degree==2);
    CHECK(r->records[1].vertex==1 && r->records[1].degree==2);
    vsdlss_reduction_free(r);
    /* Exact cancellation must leave the pre-existing structural edge. */
    { csi q[]={0,1,3,6}, j[]={0,0,1,0,1,2};
      double y[]={1,1,3,1,1,3}; vsdlss B={6,3,3,q,j,y,-1};
      CHECK(vsdlss_reduce(&B,&r)==VSDLSS_OK);
      CHECK(r->records[1].degree==1); vsdlss_reduction_free(r); }
    return 0;
}

static int test_reduction_core_and_degree_three(void)
{
    csi p[7], i[22], at=0, col,row; double x[22];
    /* K5 plus leaf 5 attached to 0: leaf is removed and K5 is the core. */
    p[0]=0;
    for(col=0;col<6;col++) { for(row=0;row<=col;row++) {
        if ((col<5) || (col==5 && row==0) || row==col) {
            i[at]=row; x[at++]=(row==col?10.0:1.0);
        }
    } p[col+1]=at; }
    { vsdlss A={at,6,6,p,i,x,-1}; vsdlss_reduction *r=NULL;
      CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK);
      CHECK(r->count==1 && r->records[0].vertex==5 && r->core_n==5);
      CHECK(r->core && r->core->p[5]==15);
      CHECK(r->core->x[0]==9.9 && r->core->x[1]==1.0);
      for(col=0;col<5;col++) CHECK(r->core_vertices[col]==col);
      vsdlss_reduction_free(r); }
    /* The 3D cube is cubic and exercises a degree-three Schur update. */
    { csi q[9], j[20], nz=0, c, rr; double y[20]; vsdlss_reduction *r=NULL;
      q[0]=0; for(c=0;c<8;c++) { for(rr=0;rr<c;rr++) {
          csi d=c^rr; if(d==1 || d==2 || d==4) { j[nz]=rr; y[nz++]=1.0; }
        } j[nz]=c; y[nz++]=6.0; q[c+1]=nz; }
      { vsdlss B={nz,8,8,q,j,y,-1}; CHECK(vsdlss_reduce(&B,&r)==VSDLSS_OK); }
      CHECK(r->count==8 && r->records[0].vertex==0 && r->records[0].degree==3);
      CHECK(r->records[0].neighbor[0]==1 && r->records[0].neighbor[1]==2 &&
            r->records[0].neighbor[2]==4); vsdlss_reduction_free(r); }
    /* Pure K5 has no eligible vertex. */
    { vsdlss A={15,5,5,p,i,x,-1}; vsdlss_reduction *r=NULL;
      CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK && r->count==0 && r->core_n==5);
      vsdlss_reduction_free(r); }
    return 0;
}

static int test_reduction_failures_are_transactional(void)
{
    vsdlss_reduction *r=(vsdlss_reduction *)1;
    csi p[]={0,1,3}, i[]={0,0,1}; double x[]={0,1,1};
    vsdlss A={3,2,2,p,i,x,-1};
    CHECK(vsdlss_reduce(&A,&r)==VSDLSS_ERR_NOT_POSDEF && r==NULL);
    x[0]=1e-300; x[1]=DBL_MAX; x[2]=DBL_MAX;
    CHECK(vsdlss_reduce(&A,&r)==VSDLSS_ERR_NONFINITE && r==NULL);
    { csi bp[]={0,2,1}, bi[]={0,0}; double bx[]={1,1};
      vsdlss bad={2,2,2,bp,bi,bx,-1};
      CHECK(vsdlss_reduce(&bad,&r)==VSDLSS_ERR_INVALID && r==NULL); }
    { csi hp[]={0}, hi[]={0}; double hx[]={1};
      vsdlss huge={0,INT64_MAX,INT64_MAX,hp,hi,hx,-1};
      CHECK(vsdlss_reduce(&huge,&r)==VSDLSS_ERR_OOM && r==NULL); }
    CHECK(vsdlss_reduce(NULL,&r)==VSDLSS_ERR_INVALID && r==NULL);
    CHECK(vsdlss_reduce(&A,NULL)==VSDLSS_ERR_INVALID);
    return 0;
}

static int test_reduction_large_sparse_path(void)
{
    const csi n=20000, nz=2*n-1; csi *p=malloc((size_t)(n+1)*sizeof(*p));
    csi *i=malloc((size_t)nz*sizeof(*i)), col, at=0; double *x=malloc((size_t)nz*sizeof(*x));
    vsdlss_reduction *r=NULL; CHECK(p&&i&&x); p[0]=0;
    for(col=0;col<n;col++) { if(col) {i[at]=col-1;x[at++]=-1;} i[at]=col;x[at++]=2;p[col+1]=at; }
    { vsdlss A={nz,n,n,p,i,x,-1}; CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK);
      CHECK(r->count==n && r->core_n==0 && r->records[0].vertex==0); }
    vsdlss_reduction_free(r); free(p); free(i); free(x); return 0;
}

int main(void)
{
    CHECK(test_interleaved_components_and_extract()==0);
    CHECK(test_isolate_single_component_and_zero_edge()==0);
    CHECK(test_invalid_inputs_leave_outputs_null()==0);
    CHECK(test_large_sparse_diagonal()==0);
    CHECK(test_large_sparse_connected_path()==0);
    CHECK(test_reduction_hand_fixture()==0);
    CHECK(test_reduction_dynamic_structure()==0);
    CHECK(test_reduction_core_and_degree_three()==0);
    CHECK(test_reduction_failures_are_transactional()==0);
    CHECK(test_reduction_large_sparse_path()==0);
    puts("m3 component and reduction tests passed");
    return 0;
}
