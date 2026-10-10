#ifndef REPLAY_FIXTURE_H
#define REPLAY_FIXTURE_H
#include "../src/vsdlss_m3_internal.h"
#include <string.h>
/* A tail of wide layers, including shared update targets. No factorization
 * time is included in the replay microbenchmark. Actual M3 is tested too. */
static vsdlss_reduction *replay_fixture(csi width,csi depth,int relabel,int overlap)
{
    vsdlss_reduction *r=calloc(1,sizeof(*r));
    if(!r) return NULL;
    r->n=width*(depth+1); r->count=width*depth; r->core_n=width;
    r->pk_count=1; r->pk=calloc(1,sizeof(*r->pk));
    if(!r->pk) {vsdlss_reduction_free(r);return NULL;}
    vsdlss_pk_seg *g=r->pk; g->count=r->count;
    g->nbn=g->count*(overlap==2?3:overlap?2:1);
    if(relabel) g->deg=malloc((size_t)g->count+1);
    else g->head=malloc((size_t)(g->count+1)*4);
    g->nb=malloc((size_t)g->nbn*4); g->val=malloc((size_t)g->nbn*8); g->piv=malloc((size_t)(g->count+1)*8);
    if((!g->deg&&!g->head)||!g->nb||!g->val||!g->piv) {vsdlss_reduction_free(r);return NULL;}
    csi o=0;
    for(csi i=0;i<g->count;i++) {
        csi d=overlap==2?i%4:overlap?2:1, base=(i/width+1)*width, lane=i%width;
        if(g->head) g->head[i]=(uint32_t)i|((uint32_t)d<<30); else g->deg[i]=(uint8_t)d;
        g->piv[i]=2.0+(i%13)*0.03125;
        if(d) {g->nb[o]=(uint32_t)(base+lane); g->val[o++]=-0.125;}
        if(d>1) {g->nb[o]=(uint32_t)(base+(lane^1));g->val[o++]=0.0625;}
        if(d>2) {g->nb[o]=(uint32_t)(base+(lane+2)%width);g->val[o++]=-0.03125;}
    }
    g->nbn=o;
    if(overlap==2) { /* A one-record final level exercises the single path. */
        if(g->head) g->head[g->count]=(uint32_t)g->count; else g->deg[g->count]=0;
        g->piv[g->count]=2;g->count++;r->count++;r->core_n--;
    }
    return r;
}
static inline int replay_prepend_block(vsdlss_reduction *r)
{
    vsdlss_pk_seg *pk=calloc(2,sizeof(*pk));csi *bp=calloc(2,sizeof(*bp));
    if(!pk||!bp) {free(pk);free(bp);return 0;}
    pk[0].count=2;pk[0].deg=calloc(2,1);pk[0].piv=malloc(2*sizeof(double));
    pk[0].nb=malloc(sizeof(*pk[0].nb));pk[0].val=malloc(sizeof(*pk[0].val));
    if(!pk[0].deg||!pk[0].piv||!pk[0].nb||!pk[0].val) {
        free(pk[0].deg);free(pk[0].piv);free(pk[0].nb);free(pk[0].val);free(pk);free(bp);return 0;
    }
    pk[0].piv[0]=pk[0].piv[1]=2;pk[1]=r->pk[0];pk[1].k0=2;
    for(csi i=0;i<pk[1].count;i++) if(pk[1].head) pk[1].head[i]+=2;
    for(csi i=0;i<pk[1].nbn;i++) pk[1].nb[i]+=2;
    free(r->pk);r->pk=pk;r->pk_count=2;r->blocks=1;r->block_ptr=bp;bp[1]=2;r->n+=2;r->count+=2;
    return 1;
}
#endif
