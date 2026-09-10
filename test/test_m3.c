#include "../src/vsdlss_m3_internal.h"
#include "m3_test_alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>

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

static int test_reduction_rhs_hand_fixture_and_aliases(void)
{
    csi p[]={0,1,3,5}, i[]={0,0,1,1,2};
    double a[]={4,2,5,1,3}, b[]={0,-5,7}, saved[3], solution[3];
    vsdlss A={5,3,3,p,i,a,-1}; vsdlss_reduction *r=NULL;
    vsdlss_elim_record before[3];
    CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK);
    memcpy(before,r->records,sizeof(before));
    CHECK(vsdlss_reduce_rhs(r,b,NULL,saved)==VSDLSS_OK);
    CHECK(saved[0]==0 && saved[1]==-5 && saved[2]==8.25);
    CHECK(vsdlss_reduce_recover(r,saved,NULL,solution)==VSDLSS_OK);
    CHECK(fabs(solution[0]-1)<1e-12 && fabs(solution[1]+2)<1e-12 &&
          fabs(solution[2]-3)<1e-12);
    CHECK(memcmp(before,r->records,sizeof(before))==0);

    b[0]=10; b[1]=8; b[2]=-2;
    CHECK(vsdlss_reduce_rhs(r,b,NULL,b)==VSDLSS_OK);
    CHECK(vsdlss_reduce_recover(r,b,NULL,b)==VSDLSS_OK);
    CHECK(fabs(b[0]-2)<1e-12 && fabs(b[1]-1)<1e-12 &&
          fabs(b[2]+1)<1e-12);
    CHECK(vsdlss_reduce_rhs(r,b,NULL,NULL)==VSDLSS_ERR_INVALID);
    CHECK(vsdlss_reduce_recover(r,NULL,NULL,solution)==VSDLSS_ERR_INVALID);
    vsdlss_reduction_free(r); return 0;
}

static int test_reduction_rhs_nonempty_core_and_transactions(void)
{
    csi p[7], i[22], at=0, col,row; double a[22];
    double b[]={15,14,14,14,14,11}, core[5], saved[1], solution[6];
    double aliased[]={15,14,14,14,14,11};
    vsdlss_reduction *r=NULL; vsdlss_elim_record before;
    p[0]=0;
    for(col=0;col<6;col++) { for(row=0;row<=col;row++) {
        if ((col<5) || (col==5 && row==0) || row==col) {
            i[at]=row; a[at++]=(row==col?10.0:1.0);
        }
    } p[col+1]=at; }
    { vsdlss A={at,6,6,p,i,a,-1}; CHECK(vsdlss_reduce(&A,&r)==VSDLSS_OK); }
    before=r->records[0];
    CHECK(vsdlss_reduce_rhs(r,b,core,saved)==VSDLSS_OK);
    CHECK(saved[0]==11 && fabs(core[0]-13.9)<1e-12);
    for(col=1;col<5;col++) CHECK(core[col]==14);
    for(col=0;col<5;col++) core[col]=1;
    CHECK(vsdlss_reduce_recover(r,saved,core,solution)==VSDLSS_OK);
    for(col=0;col<6;col++) CHECK(fabs(solution[col]-1)<1e-12);
    CHECK(memcmp(&before,r->records,sizeof(before))==0);

    for(col=0;col<6;col++) aliased[col]=1;
    CHECK(vsdlss_reduce_recover(r,saved,aliased,aliased)==VSDLSS_OK);
    for(col=0;col<6;col++) CHECK(fabs(aliased[col]-1)<1e-12);

    memcpy(aliased,b,sizeof(b));
    CHECK(vsdlss_reduce_rhs(r,aliased,aliased,aliased+5)==VSDLSS_OK);
    CHECK(fabs(aliased[0]-13.9)<1e-12 && aliased[5]==11);
    CHECK(vsdlss_reduce_rhs(r,b,NULL,saved)==VSDLSS_ERR_INVALID);
    CHECK(vsdlss_reduce_recover(r,saved,NULL,solution)==VSDLSS_ERR_INVALID);
    CHECK(vsdlss_reduce_rhs(r,NULL,core,saved)==VSDLSS_ERR_INVALID);

    { double badb[]={15,14,14,14,14,INFINITY}, outc[]={2,2,2,2,2}, outs[]={3};
      CHECK(vsdlss_reduce_rhs(r,badb,outc,outs)==VSDLSS_ERR_NONFINITE);
      for(col=0;col<5;col++) CHECK(outc[col]==2);
      CHECK(outs[0]==3); }
    { double badsaved[]={INFINITY}, coresol[]={1,1,1,1,1};
      double out[]={7,7,7,7,7,7};
      CHECK(vsdlss_reduce_recover(r,badsaved,coresol,out)==VSDLSS_ERR_NONFINITE);
      for(col=0;col<6;col++) CHECK(out[col]==7); }
    { double goodsaved[]={11}, badcore[]={1,1,NAN,1,1};
      double out[]={7,7,7,7,7,7};
      CHECK(vsdlss_reduce_recover(r,goodsaved,badcore,out)==VSDLSS_ERR_NONFINITE);
      for(col=0;col<6;col++) CHECK(out[col]==7); }
    vsdlss_reduction_free(r);

    { vsdlss_elim_record record={0,1,{1,0,0},1,{2,0,0}};
      vsdlss_reduction fake={2,1,1,&record,(csi[]){1},NULL};
      double huge[]={DBL_MAX,DBL_MAX}, outcore[]={8}, outsaved[]={9};
      CHECK(vsdlss_reduce_rhs(&fake,huge,outcore,outsaved)==VSDLSS_ERR_NONFINITE);
      CHECK(outcore[0]==8 && outsaved[0]==9); }
    { vsdlss_reduction no_elimination={2,0,2,NULL,(csi[]){0,1},NULL};
      double rhs[]={3,4}, core_only[2], recovered[2];
      CHECK(vsdlss_reduce_rhs(&no_elimination,rhs,core_only,NULL)==VSDLSS_OK);
      CHECK(core_only[0]==3 && core_only[1]==4);
      CHECK(vsdlss_reduce_recover(&no_elimination,NULL,core_only,recovered)==VSDLSS_OK);
      CHECK(recovered[0]==3 && recovered[1]==4); }
    { vsdlss_reduction empty={0,0,0,NULL,NULL,NULL};
      CHECK(vsdlss_reduce_rhs(&empty,NULL,NULL,NULL)==VSDLSS_OK);
      CHECK(vsdlss_reduce_recover(&empty,NULL,NULL,NULL)==VSDLSS_OK); }
    return 0;
}

static int check_symbolic_mapping(const vsdlss_sn_symbolic *s)
{
    csi k, sn, width, rows, slot, local_col, local_row;
    unsigned char *seen;
    CHECK(s && s->l_nnz >= 0);
    seen = calloc((size_t)(s->panel_offset[s->count] ?
                          s->panel_offset[s->count] : 1), 1);
    CHECK(seen);
    for (k = 0; k < s->l_nnz; ++k) {
        slot = s->l_panel_slot[k];
        CHECK(slot >= 0 && slot < s->panel_offset[s->count]);
        CHECK(!seen[slot]); seen[slot] = 1;
        for (sn = 0; sn + 1 < s->count &&
             slot >= s->panel_offset[sn + 1]; ++sn) {}
        width = s->column_start[sn + 1] - s->column_start[sn];
        rows = width + s->row_ptr[sn + 1] - s->row_ptr[sn];
        local_col = (slot - s->panel_offset[sn]) / rows;
        local_row = (slot - s->panel_offset[sn]) % rows;
        CHECK(local_col >= 0 && local_col < width && local_row >= local_col);
        CHECK(s->column_start[sn]+local_col >= 0);
        if (local_row < width)
            CHECK(s->l_row_index[k] == s->column_start[sn] + local_row);
        else
            CHECK(s->l_row_index[k] ==
                  s->row_index[s->row_ptr[sn] + local_row - width]);
    }
    free(seen); return 0;
}

static int test_symbolic_dense4_and_tridiagonal4(void)
{
    csi dp[]={0,1,3,6,10}, di[]={0,0,1,0,1,2,0,1,2,3};
    double dx[]={4,1,4,1,1,4,1,1,1,4};
    vsdlss D={10,4,4,dp,di,dx,-1}; vsdlss_sn_symbolic *s=NULL;
    CHECK(vsdlss_sn_analyze(&D,&s)==VSDLSS_OK);
    CHECK(s->count==1 && s->column_start[0]==0 && s->column_start[1]==4);
    CHECK(s->row_ptr[1]==0 && s->panel_offset[1]==16 && s->l_nnz==10);
    CHECK(check_symbolic_mapping(s)==0); vsdlss_sn_symbolic_free(s); s=NULL;
    { csi p[]={0,1,3,5,7}, i[]={0,0,1,1,2,2,3};
      double x[]={4,1,4,1,4,1,4}; vsdlss A={7,4,4,p,i,x,-1};
      CHECK(vsdlss_sn_analyze(&A,&s)==VSDLSS_OK);
      CHECK(s->count==3 && s->column_start[0]==0 && s->column_start[1]==1 &&
            s->column_start[2]==2 && s->column_start[3]==4);
      CHECK(s->row_ptr[0]==0 && s->row_ptr[1]==1 && s->row_ptr[2]==2 &&
            s->row_ptr[3]==2);
      CHECK(check_symbolic_mapping(s)==0); vsdlss_sn_symbolic_free(s); }
    return 0;
}

static int test_symbolic_single_shared_updates_and_failures(void)
{
    csi p[]={0,1,3,5,8,11}, i[]={0,0,1,0,2,1,2,3,1,3,4};
    double x[]={8,1,8,1,8,1,1,8,1,1,8};
    vsdlss A={11,5,5,p,i,x,-1}; vsdlss_sn_symbolic *s=NULL;
    CHECK(vsdlss_sn_analyze(&A,&s)==VSDLSS_OK);
    { const csi cs[]={0,1,5}, lp[]={0,3,7,10,12,13};
      const csi li[]={0,1,2,1,2,3,4,2,3,4,3,4,4};
      const csi rp[]={0,2,2}, ri[]={1,2}, po[]={0,3,19};
      const csi lm[]={0,1,2,3,4,5,6,8,9,10,13,14,18};
      const csi up[]={0,3,3}, ut[]={3,4,8}; csi k;
      CHECK(s->count==2 && s->l_nnz==13);
      for(k=0;k<3;k++) CHECK(s->column_start[k]==cs[k] &&
                             s->row_ptr[k]==rp[k] &&
                             s->panel_offset[k]==po[k] &&
                             s->update_ptr[k]==up[k]);
      for(k=0;k<6;k++) CHECK(s->l_col_ptr[k]==lp[k]);
      for(k=0;k<13;k++) CHECK(s->l_row_index[k]==li[k] &&
                              s->l_panel_slot[k]==lm[k]);
      for(k=0;k<2;k++) CHECK(s->row_index[k]==ri[k]);
      for(k=0;k<3;k++) CHECK(s->update_target[k]==ut[k]); }
    CHECK(check_symbolic_mapping(s)==0);
    { csi k; for(k=0;k<s->update_ptr[s->count];++k)
        CHECK(s->update_target[k]>=0 &&
              s->update_target[k]<s->panel_offset[s->count]); }
    vsdlss_sn_symbolic_free(s); s=(vsdlss_sn_symbolic *)1;
    /* Two distinct source panels both update the later column-4 panel. */
    { csi gp[]={0,1,2,4,5,7,8}, gi[]={0,1,0,2,3,0,4,5};
      double gx[]={8,8,1,8,8,1,8,8}; vsdlss G={8,6,6,gp,gi,gx,-1};
      const csi cs[]={0,1,2,3,4,5,6}, rp[]={0,2,2,3,3,3,3}; csi k;
      CHECK(vsdlss_sn_analyze(&G,&s)==VSDLSS_OK && s->count==6);
      for(k=0;k<7;k++) CHECK(s->column_start[k]==cs[k] && s->row_ptr[k]==rp[k]);
      CHECK(s->row_index[0]==2 && s->row_index[1]==4 && s->row_index[2]==4);
      CHECK(s->update_ptr[1]==3 && s->update_ptr[2]==3 &&
            s->update_ptr[3]==4);
      CHECK(s->update_target[2]==s->update_target[3]);
      CHECK(check_symbolic_mapping(s)==0); vsdlss_sn_symbolic_free(s); }
    s=(vsdlss_sn_symbolic *)1;
    { csi q[]={0,1}, j[]={0}; double y[]={1}; vsdlss one={1,1,1,q,j,y,-1};
      CHECK(vsdlss_sn_analyze(&one,&s)==VSDLSS_OK && s->count==1 &&
            s->column_start[1]==1); vsdlss_sn_symbolic_free(s); }
    s=(vsdlss_sn_symbolic *)1;
    CHECK(vsdlss_sn_analyze(NULL,&s)==VSDLSS_ERR_INVALID && s==NULL);
    CHECK(vsdlss_sn_analyze(&A,NULL)==VSDLSS_ERR_INVALID);
    { csi hp[]={0}; csi hi[]={0}; double hx[]={1};
      vsdlss huge={0,INT64_MAX,INT64_MAX,hp,hi,hx,-1};
      CHECK(vsdlss_sn_analyze(&huge,&s)==VSDLSS_ERR_OOM && s==NULL); }
    return 0;
}

static int dense_from_csc(const vsdlss *L, double *d)
{
    csi j,k; memset(d,0,(size_t)L->n*(size_t)L->n*sizeof(*d));
    for(j=0;j<L->n;j++) for(k=L->p[j];k<L->p[j+1];k++)
        d[L->i[k]*L->n+j]=L->x[k];
    return 0;
}

static int test_numeric_hand_factor_ownership_and_solve(void)
{
    csi p[]={0,1,3,5}, i[]={0,0,1,1,2};
    double a[]={4,2,5,2,5}, expected[]={2,0,0,1,2,0,0,1,2};
    double rhs[]={8,18,19}, alias[]={8,18,19}, x[3], dense[9];
    vsdlss A={5,3,3,p,i,a,-1}, *L=NULL; vsdlss_sn_symbolic *s=NULL;
    vsdlss_sn_factor *f=NULL; double before[5]; csi k;
    memcpy(before,a,sizeof(a));
    CHECK(vsdlss_sn_analyze(&A,&s)==VSDLSS_OK);
    CHECK(vsdlss_sn_factorize(&A,s,&f)==VSDLSS_OK && f);
    vsdlss_sn_symbolic_free(s); /* factor owns every retained layout array */
    CHECK(memcmp(before,a,sizeof(a))==0);
    CHECK(vsdlss_sn_export_L(f,&L)==VSDLSS_OK);
    dense_from_csc(L,dense);
    for(k=0;k<9;k++) CHECK(fabs(dense[k]-expected[k])<1e-12);
    CHECK(vsdlss_sn_solve(f,rhs,x)==VSDLSS_OK);
    CHECK(fabs(x[0]-1)<1e-12&&fabs(x[1]-2)<1e-12&&fabs(x[2]-3)<1e-12);
    CHECK(vsdlss_sn_solve(f,alias,alias)==VSDLSS_OK);
    for(k=0;k<3;k++) CHECK(fabs(alias[k]-(double)(k+1))<1e-12);
    rhs[0]=2;rhs[1]=4;rhs[2]=6;
    CHECK(vsdlss_sn_solve(f,rhs,x)==VSDLSS_OK); /* reusable immutable factor */
    { double bad[]={1,NAN,3}, unchanged[]={7,8,9};
      CHECK(vsdlss_sn_solve(f,bad,unchanged)==VSDLSS_ERR_NONFINITE);
      CHECK(unchanged[0]==7&&unchanged[1]==8&&unchanged[2]==9); }
    vsdlss_spfree(L); vsdlss_sn_factor_free(f); return 0;
}

static int test_numeric_heterogeneous_shared_updates_and_reconstruction(void)
{
    csi p[]={0,1,3,5,8,11}, i[]={0,0,1,0,2,1,2,3,1,3,4};
    double a[]={8,1,8,1,8,1,1,8,1,1,8}, dense[25], sum, want;
    vsdlss A={11,5,5,p,i,a,-1}, *L=NULL; vsdlss_sn_symbolic *s=NULL;
    vsdlss_sn_factor *f=NULL; vsdlss_factor *m1=NULL; csi r,c,t,k;
    CHECK(vsdlss_sn_analyze(&A,&s)==VSDLSS_OK && s->count==2);
    CHECK(vsdlss_sn_factorize(&A,s,&f)==VSDLSS_OK);
    CHECK(vsdlss_sn_export_L(f,&L)==VSDLSS_OK); dense_from_csc(L,dense);
    CHECK(vsdlss_factorize(&A,2,&m1)==VSDLSS_OK);
    { const vsdlss *M=vsdlss_factor_L(m1); CHECK(M&&M->p[5]==L->p[5]);
      for(k=0;k<=5;k++) CHECK(M->p[k]==L->p[k]);
      for(k=0;k<L->p[5];k++) CHECK(M->i[k]==L->i[k]&&fabs(M->x[k]-L->x[k])<1e-12); }
    for(r=0;r<5;r++) for(c=0;c<5;c++) {
        sum=0; for(t=0;t<5;t++) sum+=dense[r*5+t]*dense[c*5+t];
        want=0; for(k=p[c];k<p[c+1];k++) if(i[k]==r) want=a[k];
        if(r>c) { for(k=p[r];k<p[r+1];k++) if(i[k]==c) want=a[k]; }
        CHECK(fabs(sum-want)<1e-11);
    }
    vsdlss_factor_free(m1); vsdlss_spfree(L); vsdlss_sn_factor_free(f); vsdlss_sn_symbolic_free(s);
    return 0;
}

static csi panel_owner(const vsdlss_sn_symbolic *s, csi slot)
{
    csi sn;
    for(sn=0;sn<s->count;sn++)
        if(slot>=s->panel_offset[sn]&&slot<s->panel_offset[sn+1]) return sn;
    return -1;
}

static int test_numeric_multiple_destinations_and_shared_target(void)
{
    csi p[]={0,1,2,4,5,7,8}, i[]={0,1,0,2,3,0,4,5};
    double a[]={8,8,1,8,8,1,8,8}, dense[36], sum, want;
    vsdlss A={8,6,6,p,i,a,-1}, *L=NULL; vsdlss_sn_symbolic *s=NULL;
    vsdlss_sn_factor *f=NULL; vsdlss_factor *m1=NULL; csi r,c,t,k;
    CHECK(vsdlss_sn_analyze(&A,&s)==VSDLSS_OK && s->count>2);
    /* Source panel 0 scatters to panels 2 and 4.  Panels 0 and 2 both
       contribute to the diagonal slot in the later panel 4. */
    CHECK(panel_owner(s,s->update_target[0])==2);
    CHECK(panel_owner(s,s->update_target[2])==4);
    CHECK(s->update_target[2]==s->update_target[s->update_ptr[2]]);
    CHECK(vsdlss_sn_factorize(&A,s,&f)==VSDLSS_OK);
    CHECK(vsdlss_sn_export_L(f,&L)==VSDLSS_OK); dense_from_csc(L,dense);
    CHECK(vsdlss_factorize(&A,2,&m1)==VSDLSS_OK);
    { const vsdlss *M=vsdlss_factor_L(m1); CHECK(M&&M->p[6]==L->p[6]);
      for(k=0;k<=6;k++) CHECK(M->p[k]==L->p[k]);
      for(k=0;k<L->p[6];k++) CHECK(M->i[k]==L->i[k]&&fabs(M->x[k]-L->x[k])<1e-12); }
    for(r=0;r<6;r++) for(c=0;c<6;c++) {
        sum=0; for(t=0;t<6;t++) sum+=dense[r*6+t]*dense[c*6+t];
        want=0; for(k=p[c];k<p[c+1];k++) if(i[k]==r) want=a[k];
        if(r>c) for(k=p[r];k<p[r+1];k++) if(i[k]==c) want=a[k];
        CHECK(fabs(sum-want)<1e-11);
    }
    vsdlss_factor_free(m1); vsdlss_spfree(L); vsdlss_sn_factor_free(f);
    vsdlss_sn_symbolic_free(s); return 0;
}

static int test_numeric_errors_are_transactional(void)
{
    csi p[]={0,1,3}, i[]={0,0,1}; double a[]={1,2,1};
    vsdlss A={3,2,2,p,i,a,-1}; vsdlss_sn_symbolic *s=NULL;
    vsdlss_sn_factor *f=(vsdlss_sn_factor*)1; vsdlss *L=(vsdlss*)1;
    double rhs[]={1,NAN}, out[]={7,8};
    CHECK(vsdlss_sn_analyze(&A,&s)==VSDLSS_OK);
    CHECK(vsdlss_sn_factorize(&A,s,&f)==VSDLSS_ERR_NOT_POSDEF&&f==NULL);
    a[2]=INFINITY; f=(vsdlss_sn_factor*)1;
    CHECK(vsdlss_sn_factorize(&A,s,&f)==VSDLSS_ERR_NONFINITE&&f==NULL);
    a[2]=1;
    CHECK(vsdlss_sn_factorize(NULL,s,&f)==VSDLSS_ERR_INVALID&&f==NULL);
    CHECK(vsdlss_sn_factorize(&A,NULL,&f)==VSDLSS_ERR_INVALID&&f==NULL);
    CHECK(vsdlss_sn_factorize(&A,s,NULL)==VSDLSS_ERR_INVALID);
    CHECK(vsdlss_sn_solve(NULL,rhs,out)==VSDLSS_ERR_INVALID);
    CHECK(out[0]==7&&out[1]==8);
    CHECK(vsdlss_sn_export_L(NULL,&L)==VSDLSS_ERR_INVALID&&L==NULL);
    { csi tiny_p[]={0}, tiny_i[]={0}; double tiny_x[]={1};
      vsdlss huge={INT64_MAX,INT64_MAX,INT64_MAX,tiny_p,tiny_i,tiny_x,-1};
      vsdlss_sn_symbolic huge_s={0};
      huge_s.n=INT64_MAX; huge_s.count=1; huge_s.l_nnz=1;
      f=(vsdlss_sn_factor*)1;
      CHECK(vsdlss_sn_factorize(&huge,&huge_s,&f)==VSDLSS_ERR_OOM&&f==NULL); }
    { csi one_p[]={0,1}, one_i[]={0}; double one_x[]={1};
      csi one[]={0}; vsdlss one_A={1,1,1,one_p,one_i,one_x,-1};
      vsdlss_sn_symbolic huge_count={0};
      huge_count.n=1; huge_count.count=INT64_MAX; huge_count.l_nnz=1;
      huge_count.l_col_ptr=one; huge_count.l_row_index=one;
      huge_count.l_panel_slot=one; huge_count.column_start=one;
      huge_count.row_ptr=one; huge_count.panel_offset=one;
      huge_count.update_ptr=one; f=(vsdlss_sn_factor*)1;
      CHECK(vsdlss_sn_factorize(&one_A,&huge_count,&f)==VSDLSS_ERR_OOM&&f==NULL); }
    { vsdlss_sn_symbolic malformed={0}; f=(vsdlss_sn_factor*)1;
      malformed.n=2; malformed.count=1; malformed.l_nnz=1;
      CHECK(vsdlss_sn_factorize(&A,&malformed,&f)==VSDLSS_ERR_INVALID&&f==NULL); }
    vsdlss_sn_symbolic_free(s); return 0;
}

static vsdlss *make_facade_fixture(csi n, int mixed)
{
    csi col,row,nz=0,capacity=mixed?n*(n+1)/2:n;
    vsdlss *A=vsdlss_spalloc(n,n,capacity,1,0);
    if(!A) return NULL;
    A->p[0]=0;
    for(col=0;col<n;col++) {
        for(row=0;row<=col;row++) {
            int same=(row%2)==(col%2), edge=0;
            if(row==col) edge=1;
            else if(mixed && same && (col%2==0 || col-row==2)) edge=1;
            if(edge) { A->i[nz]=row; A->x[nz++]=(row==col?(col%2?4.0:10.0):1.0); }
        }
        A->p[col+1]=nz;
    }
    A->nzmax=nz?nz:1; return A;
}

static int test_m3_facade_mixed_components_orders_reuse_and_alias(void)
{
    vsdlss *A=make_facade_fixture(10,1); double want[10],rhs[10],out[10];
    vsdlss_m3_factor *f=NULL; int order,pass; csi k;
    CHECK(A);
    for(order=0;order<=4;order++) {
        CHECK(vsdlss_factorize_m3(A,order,&f)==VSDLSS_OK && f);
        for(pass=0;pass<2;pass++) {
            for(k=0;k<10;k++) want[k]=(pass?-.25:1.0)+(double)(k+1)*(pass?0.3:0.2);
            CHECK(vsdlss_spmv_sym_upper(A,want,rhs)==VSDLSS_OK);
            CHECK(vsdlss_m3_solve(f,rhs,out)==VSDLSS_OK);
            for(k=0;k<10;k++) CHECK(fabs(out[k]-want[k])<1e-10);
        }
        CHECK(vsdlss_spmv_sym_upper(A,want,rhs)==VSDLSS_OK);
        CHECK(vsdlss_m3_solve(f,rhs,rhs)==VSDLSS_OK);
        for(k=0;k<10;k++) CHECK(fabs(rhs[k]-want[k])<1e-10);
        vsdlss_m3_factor_free(f); f=NULL;
    }
    vsdlss_spfree(A); return 0;
}

static vsdlss *make_dense33_spd(void)
{
    const csi n=33, nz=n*(n+1)/2;
    vsdlss *A=vsdlss_spalloc(n,n,nz,1,0); csi col,row,at=0;
    if(!A) return NULL;
    A->p[0]=0;
    for(col=0;col<n;col++) {
        for(row=0;row<col;row++) { A->i[at]=row; A->x[at++]=-1.0; }
        A->i[at]=col; A->x[at++]=34.0; A->p[col+1]=at;
    }
    A->nzmax=at; return A;
}

static int test_dense33_degenerate_separator_m1_m3(void)
{
    vsdlss *A=make_dense33_spd(); double want[33],rhs[33],got[33];
    const int orders[]={0,4}; csi k; size_t oi;
    CHECK(A);
    for(k=0;k<33;k++) want[k]=(k%3==0?-.5:.25)+(double)(k+1)/19.0;
    CHECK(vsdlss_spmv_sym_upper(A,want,rhs)==VSDLSS_OK);
    for(oi=0;oi<sizeof(orders)/sizeof(orders[0]);oi++) {
        vsdlss_factor *m1=NULL; vsdlss_m3_factor *m3=NULL;
        CHECK(vsdlss_factorize(A,orders[oi],&m1)==VSDLSS_OK && m1);
        CHECK(vsdlss_factor_solve(m1,rhs,got)==VSDLSS_OK);
        for(k=0;k<33;k++) CHECK(fabs(got[k]-want[k])<2e-11);
        CHECK(vsdlss_factorize_m3(A,orders[oi],&m3)==VSDLSS_OK && m3);
        CHECK(vsdlss_m3_solve(m3,rhs,got)==VSDLSS_OK);
        for(k=0;k<33;k++) CHECK(fabs(got[k]-want[k])<2e-11);
        vsdlss_factor_free(m1); vsdlss_m3_factor_free(m3);
    }
    vsdlss_spfree(A); return 0;
}

static int test_m3_facade_errors_transaction_and_many_isolates(void)
{
    csi p[]={0,1,2},i[]={0,1}; double a[]={2,-1},rhs[]={1,NAN},out[]={7,8};
    vsdlss A={2,2,2,p,i,a,-1}; vsdlss_m3_factor *f=(vsdlss_m3_factor*)1;
    CHECK(vsdlss_factorize_m3(&A,2,&f)==VSDLSS_ERR_NOT_POSDEF && f==NULL);
    CHECK(vsdlss_factorize_m3(&A,9,&f)==VSDLSS_ERR_UNSUPPORTED && f==NULL);
    a[1]=3; CHECK(vsdlss_factorize_m3(&A,2,&f)==VSDLSS_OK);
    CHECK(vsdlss_m3_solve(f,rhs,out)==VSDLSS_ERR_NONFINITE && out[0]==7 && out[1]==8);
    CHECK(vsdlss_m3_solve(NULL,rhs,out)==VSDLSS_ERR_INVALID);
    vsdlss_m3_factor_free(f); vsdlss_m3_factor_free(NULL);
    { const csi n=12000; vsdlss *D=make_facade_fixture(n,0); double *b,*x; csi k;
      CHECK(D); b=malloc((size_t)n*sizeof(*b)); x=malloc((size_t)n*sizeof(*x)); CHECK(b&&x);
      CHECK(vsdlss_factorize_m3(D,4,&f)==VSDLSS_OK);
      for(k=0;k<n;k++) b[k]=2.0*(k%7-3);
      CHECK(vsdlss_m3_solve(f,b,x)==VSDLSS_OK);
      for(k=0;k<n;k++) CHECK(x[k]==b[k]/D->x[D->p[k]]);
      free(b);free(x);vsdlss_m3_factor_free(f);vsdlss_spfree(D); }
    { csi hp[]={0}; csi hi[]={0}; double hx[]={1};
      vsdlss huge={0,INT64_MAX,INT64_MAX,hp,hi,hx,-1}; f=(vsdlss_m3_factor*)1;
      CHECK(vsdlss_factorize_m3(&huge,2,&f)==VSDLSS_ERR_OOM && f==NULL); }
    return 0;
}

static int test_m3_partial_factor_cleanup_without_component_array(void)
{
    vsdlss_m3_factor *f=(vsdlss_m3_factor*)calloc(1,sizeof(*f));
    CHECK(f);
    f->count=1;
    f->component=NULL;
    vsdlss_m3_factor_free(f);
    return 0;
}

typedef struct { csi a,b; double w; } test_edge;

static vsdlss *make_laplacian(csi n, const test_edge *edge, csi edges)
{
    csi col,k,nz=0; vsdlss *A=vsdlss_spalloc(n,n,n+edges,1,0);
    if(!A) return NULL;
    A->p[0]=0;
    for(col=0;col<n;col++) {
        double diagonal=1.0;
        for(k=0;k<edges;k++) if(edge[k].a==col||edge[k].b==col) diagonal+=edge[k].w;
        A->i[nz]=col; A->x[nz++]=diagonal;
        for(k=0;k<edges;k++) {
            csi lo=edge[k].a<edge[k].b?edge[k].a:edge[k].b;
            csi hi=edge[k].a<edge[k].b?edge[k].b:edge[k].a;
            if(hi==col) { A->i[nz]=lo; A->x[nz++]=-edge[k].w; }
        }
        /* Keep each upper column sorted for validation. */
        { csi p,q; for(p=A->p[col]+1;p<nz;p++) for(q=p;q>A->p[col]&&A->i[q]<A->i[q-1];q--) {
            csi ti=A->i[q]; double tx=A->x[q]; A->i[q]=A->i[q-1]; A->x[q]=A->x[q-1]; A->i[q-1]=ti; A->x[q-1]=tx;
        }}
        A->p[col+1]=nz;
    }
    A->nzmax=nz; return A;
}

static int dense_oracle(const vsdlss *A,const double *b,double *x)
{
    csi n=A->n,i,j,k,p; double d[128*128],y[128];
    if(n>128) return 0;
    memset(d,0,(size_t)n*(size_t)n*sizeof(double));
    for(j=0;j<n;j++) for(p=A->p[j];p<A->p[j+1];p++) {
        i=A->i[p]; d[i*n+j]=A->x[p]; d[j*n+i]=A->x[p];
    }
    for(j=0;j<n;j++) {
        for(i=j;i<n;i++) { double s=d[i*n+j]; for(k=0;k<j;k++) s-=d[i*n+k]*d[j*n+k];
            if(i==j) {if(!(s>0)) return 0; d[j*n+j]=sqrt(s);} else d[i*n+j]=s/d[j*n+j]; }
    }
    for(i=0;i<n;i++){double s=b[i];for(k=0;k<i;k++)s-=d[i*n+k]*y[k];y[i]=s/d[i*n+i];}
    for(i=n;i-->0;){double s=y[i];for(k=i+1;k<n;k++)s-=d[k*n+i]*x[k];x[i]=s/d[i*n+i];}
    return 1;
}

static int compare_graph(const vsdlss *A,int expected_layout,int require_degree3)
{
    double b[128],xm1[128],xm3[128],xo[128],eta1,eta3; csi k,d3=0;
    vsdlss_factor *m1=NULL; vsdlss_m3_factor *m3=NULL; vsdlss_reduction *r=NULL;
    vsdlss_sn_symbolic *s=NULL; vsdlss_sn_factor *sn=NULL;
    CHECK(A->n<=128); for(k=0;k<A->n;k++) b[k]=sin((double)(k+1)*.37)+.1*k;
    CHECK(dense_oracle(A,b,xo)); CHECK(vsdlss_factorize(A,2,&m1)==VSDLSS_OK);
    CHECK(vsdlss_factor_solve(m1,b,xm1)==VSDLSS_OK);
    CHECK(vsdlss_factorize_m3(A,2,&m3)==VSDLSS_OK); CHECK(vsdlss_m3_solve(m3,b,xm3)==VSDLSS_OK);
    CHECK(vsdlss_reduce(A,&r)==VSDLSS_OK); for(k=0;k<r->count;k++) if(r->records[k].degree==3)d3++;
    CHECK(!expected_layout||r->core_n>0); CHECK(!require_degree3||d3>0);
    if(expected_layout){CHECK(vsdlss_sn_analyze(r->core,&s)==VSDLSS_OK);CHECK(vsdlss_sn_factorize(r->core,s,&sn)==VSDLSS_OK);
        if(expected_layout==1){CHECK(s->count==1);CHECK(s->row_ptr[s->count]==0);}
        else {CHECK(s->count>1);CHECK(s->row_ptr[s->count]>0);CHECK(s->update_ptr[s->count]>0);CHECK(sn->count==s->count);CHECK(sn->row_ptr[sn->count]>0);}}
    for(k=0;k<A->n;k++){CHECK(fabs(xm1[k]-xo[k])<2e-11);CHECK(fabs(xm3[k]-xo[k])<2e-11);}
    CHECK(vsdlss_backward_error(A,xm1,b,&eta1)==VSDLSS_OK&&eta1<=1e-12);
    CHECK(vsdlss_backward_error(A,xm3,b,&eta3)==VSDLSS_OK&&eta3<=1e-12);
    vsdlss_sn_factor_free(sn);vsdlss_sn_symbolic_free(s);vsdlss_reduction_free(r);vsdlss_factor_free(m1);vsdlss_m3_factor_free(m3);return 0;
}

static int test_deterministic_graph_oracles(void)
{
    test_edge cube[12],k5[10],grid[144]; csi e=0,x,y,z;
    for(x=0;x<8;x++) for(y=0;y<3;y++) if(!(x&(1<<y))) {cube[e++]=(test_edge){x,x+(1<<y),1.0+.03*e};}
    {vsdlss*A=make_laplacian(8,cube,12);CHECK(A);CHECK(compare_graph(A,0,1)==0);vsdlss_spfree(A);}
    e=0;for(x=0;x<5;x++)for(y=x+1;y<5;y++)k5[e++]=(test_edge){x,y,.7+.02*e};
    {vsdlss*A=make_laplacian(5,k5,10);CHECK(A);CHECK(compare_graph(A,1,0)==0);vsdlss_spfree(A);}
    e=0;for(z=0;z<4;z++)for(y=0;y<4;y++)for(x=0;x<4;x++){
        csi v=x+4*y+16*z;if(x<3)grid[e++]=(test_edge){v,v+1,1.01};if(y<3)grid[e++]=(test_edge){v,v+4,.93};if(z<3)grid[e++]=(test_edge){v,v+16,1.07};}
    {vsdlss*A=make_laplacian(64,grid,e);CHECK(A);CHECK(compare_graph(A,2,0)==0);vsdlss_spfree(A);} return 0;
}

static int test_m3_allocation_failures(void)
{
    test_edge edge[144]; csi a,x,y,z,e=0; vsdlss *A; vsdlss_m3_factor *f=NULL; size_t n,k,base,factor_n; double rhs[64],out[64];
    for(z=0;z<4;z++)for(y=0;y<4;y++)for(x=0;x<4;x++){csi v=x+4*y+16*z;if(x<3)edge[e++]=(test_edge){v,v+1,1};if(y<3)edge[e++]=(test_edge){v,v+4,1};if(z<3)edge[e++]=(test_edge){v,v+16,1};}
    A=make_laplacian(64,edge,e);CHECK(A);base=m3_alloc_live();
    m3_alloc_reset();CHECK(vsdlss_factorize_m3(A,0,&f)==VSDLSS_OK);n=m3_alloc_calls();factor_n=n;vsdlss_m3_factor_free(f);f=NULL;CHECK(m3_alloc_live()==base);
    for(k=1;k<=n;k++){m3_alloc_fail_at(k);CHECK(vsdlss_factorize_m3(A,0,&f)==VSDLSS_ERR_OOM);CHECK(f==NULL);CHECK(m3_alloc_live()==base);}
    m3_alloc_reset();CHECK(vsdlss_factorize_m3(A,0,&f)==VSDLSS_OK);for(a=0;a<64;a++)rhs[a]=a+1;
    base=m3_alloc_live();m3_alloc_reset();CHECK(vsdlss_m3_solve(f,rhs,out)==VSDLSS_OK);n=m3_alloc_calls();CHECK(m3_alloc_live()==base);
    for(k=1;k<=n;k++){for(a=0;a<64;a++)out[a]=99; m3_alloc_fail_at(k);CHECK(vsdlss_m3_solve(f,rhs,out)==VSDLSS_ERR_OOM);for(a=0;a<64;a++)CHECK(out[a]==99);CHECK(m3_alloc_live()==base);}
    printf("allocation failure sweeps: factorize=%zu solve=%zu\n",factor_n,n);
    m3_alloc_reset();vsdlss_m3_factor_free(f);vsdlss_spfree(A);return 0;
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
    CHECK(test_reduction_rhs_hand_fixture_and_aliases()==0);
    CHECK(test_reduction_rhs_nonempty_core_and_transactions()==0);
    CHECK(test_symbolic_dense4_and_tridiagonal4()==0);
    CHECK(test_symbolic_single_shared_updates_and_failures()==0);
    CHECK(test_numeric_hand_factor_ownership_and_solve()==0);
    CHECK(test_numeric_heterogeneous_shared_updates_and_reconstruction()==0);
    CHECK(test_numeric_multiple_destinations_and_shared_target()==0);
    CHECK(test_numeric_errors_are_transactional()==0);
    CHECK(test_m3_facade_mixed_components_orders_reuse_and_alias()==0);
    CHECK(test_dense33_degenerate_separator_m1_m3()==0);
    CHECK(test_m3_facade_errors_transaction_and_many_isolates()==0);
    CHECK(test_m3_partial_factor_cleanup_without_component_array()==0);
    CHECK(test_deterministic_graph_oracles()==0);
    CHECK(test_m3_allocation_failures()==0);
    puts("m3 component, reduction, RHS, symbolic, and numeric tests passed");
    return 0;
}
