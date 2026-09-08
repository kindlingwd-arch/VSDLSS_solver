#include "../src/vsdlss_internal.h"

#include <stdio.h>
#include <stdlib.h>

static int failures;
#define CHECK(e) do { if (!(e)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #e); failures++; } } while (0)

static vsdlss *make_weighted_path(void)
{
    csi p[] = {0,1,3,5,7};
    csi i[] = {0,0,1,1,2,2,3};
    double x[] = {5,-1,5,-9,5,-1,5};
    vsdlss *A = vsdlss_spalloc(4,4,7,1,0);
    csi k;
    if (!A) return NULL;
    for (k=0;k<5;k++) A->p[k]=p[k];
    for (k=0;k<7;k++) { A->i[k]=i[k]; A->x[k]=x[k]; }
    return A;
}

static vsdlss *make_grid(csi side)
{
    csi n=side*side,nz=n+2*side*(side-1),col,at=0;
    vsdlss *A=vsdlss_spalloc(n,n,nz,1,0); if(!A)return NULL;
    for(col=0;col<n;col++){A->p[col]=at;if(col>=side){A->i[at]=col-side;A->x[at++]=-1;}if(col%side){A->i[at]=col-1;A->x[at++]=-1;}A->i[at]=col;A->x[at++]=5;}
    A->p[n]=at;return A;
}

/* Catches selecting the first neighbor instead of the strongest legal edge. */
static void test_shem_and_coarse_aggregation(void)
{
    vsdlss *A=make_weighted_path();
    vsdlss_mld_level *fine=NULL,*coarse=NULL;
    const csi *map;
    CHECK(A != NULL);
    CHECK(vsdlss_mld_level_build(A,NULL,4,&fine)==VSDLSS_OK);
    CHECK(vsdlss_mld_coarsen_one(fine,&coarse)==VSDLSS_OK);
    map=vsdlss_mld_level_map(fine);
    CHECK(map && map[0]==0 && map[1]==0 && map[2]==1 && map[3]==1);
    CHECK(vsdlss_mld_level_vertices(coarse)==2);
    CHECK(vsdlss_mld_level_vertex_weight(coarse,0)==2);
    CHECK(vsdlss_mld_level_vertex_weight(coarse,1)==2);
    CHECK(vsdlss_mld_level_edge_weight(coarse,0,1)==1);
    vsdlss_mld_level_free(coarse); vsdlss_mld_level_free(fine); vsdlss_spfree(A);
}

/* Catches a refiner that increases cut or creates an empty/unbalanced side. */
static void test_partition_refines_without_increasing_cut(void)
{
    vsdlss *A=make_weighted_path(); vsdlss_mld_level *g=NULL;
    signed char part[4],again[4]; csi initial=-1,final=-1,i;
    CHECK(vsdlss_mld_level_build(A,NULL,4,&g)==VSDLSS_OK);
    CHECK(vsdlss_mld_partition_level(g,part,&initial,&final)==VSDLSS_OK);
    CHECK(final<=initial);
    CHECK(vsdlss_mld_partition_level(g,again,NULL,NULL)==VSDLSS_OK);
    for(i=0;i<4;i++) CHECK(part[i]==again[i]);
    CHECK((part[0]==0||part[0]==1) && (part[3]==0||part[3]==1));
    CHECK(part[0]!=part[3]);
    vsdlss_mld_level_free(g); vsdlss_spfree(A);
}

/* Catches separator conversion that leaves a cross-side edge. */
static void test_node_separator_removes_every_cross_edge(void)
{
    vsdlss *A=make_weighted_path(); vsdlss_mld_level *g=NULL;
    signed char part[4]; csi l,r,s,v,k;
    CHECK(vsdlss_mld_level_build(A,NULL,4,&g)==VSDLSS_OK);
    CHECK(vsdlss_mld_partition_level(g,part,NULL,NULL)==VSDLSS_OK);
    CHECK(vsdlss_mld_node_separator(g,part,&l,&r,&s)==VSDLSS_OK);
    CHECK(l>0 && r>0 && s>0 && l+r+s==4);
    for(v=0;v<4;v++) { const csi *nb,*wt; csi n;
        CHECK(vsdlss_mld_level_neighbors(g,v,&nb,&wt,&n)==VSDLSS_OK); (void)wt;
        for(k=0;k<n;k++) CHECK(!((part[v]==0&&part[nb[k]]==1)||(part[v]==1&&part[nb[k]]==0)));
    }
    vsdlss_mld_level_free(g); vsdlss_spfree(A);
}

/* Catches integration that still dispatches every large graph to BFS-only MLD. */
static void test_large_grid_builds_multiple_levels(void)
{
    vsdlss *A=make_grid(30);csi levels=0,*q=NULL,*pinv=NULL;
    CHECK(A!=NULL);
    CHECK(vsdlss_mld_hierarchy_analyze(A,&levels)==VSDLSS_OK);
    CHECK(levels>=2);
    CHECK(vsdlss_order(A,4,&q,&pinv)==VSDLSS_OK);
    if(q&&pinv)CHECK(vsdlss_validate_permutation(q,pinv,A->n)==VSDLSS_OK);
    free(q);free(pinv);vsdlss_spfree(A);
}

int main(void)
{
    test_shem_and_coarse_aggregation();
    test_partition_refines_without_increasing_cut();
    test_node_separator_removes_every_cross_edge();
    test_large_grid_builds_multiple_levels();
    if (failures) return 1;
    puts("test_mld: ALL OK");
    return 0;
}
