#include "vsdlss_internal.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

struct vsdlss_mld_level {
    csi n, nz;
    csi *offset, *neighbor, *edge_weight, *vertex_weight, *original, *map;
};

typedef struct edge_record { csi a,b,w; } edge_record;

static int edge_compare(const void *x,const void *y)
{
    const edge_record *a=x,*b=y;
    if (a->a!=b->a) return (a->a>b->a)-(a->a<b->a);
    return (a->b>b->b)-(a->b<b->b);
}

static void level_destroy(vsdlss_mld_level *g)
{
    if (!g) return;
    free(g->offset); free(g->neighbor); free(g->edge_weight);
    free(g->vertex_weight); free(g->original); free(g->map); free(g);
}

void vsdlss_mld_level_free(vsdlss_mld_level *g) { level_destroy(g); }
csi vsdlss_mld_level_vertices(const vsdlss_mld_level *g) { return g?g->n:0; }
csi vsdlss_mld_level_edges(const vsdlss_mld_level *g) { return g?g->nz/2:0; }
csi vsdlss_mld_level_vertex_weight(const vsdlss_mld_level *g,csi v)
{ return (!g||v<0||v>=g->n)?0:g->vertex_weight[v]; }
const csi *vsdlss_mld_level_original(const vsdlss_mld_level *g) { return g?g->original:NULL; }
const csi *vsdlss_mld_level_map(const vsdlss_mld_level *g) { return g?g->map:NULL; }
vsdlss_status vsdlss_mld_level_neighbors(const vsdlss_mld_level *g,csi v,
                                          const csi **nb,const csi **wt,csi *count)
{
    if(!g||v<0||v>=g->n||!nb||!wt||!count) return VSDLSS_ERR_INVALID;
    *nb=g->neighbor+g->offset[v]; *wt=g->edge_weight+g->offset[v];
    *count=g->offset[v+1]-g->offset[v]; return VSDLSS_OK;
}
csi vsdlss_mld_level_edge_weight(const vsdlss_mld_level *g,csi a,csi b)
{
    csi p;
    if (!g||a<0||b<0||a>=g->n||b>=g->n) return 0;
    for (p=g->offset[a];p<g->offset[a+1];p++) if (g->neighbor[p]==b) return g->edge_weight[p];
    return 0;
}

static vsdlss_status make_level(csi n,const csi *vw,const csi *orig,
                                edge_record *e,csi m,vsdlss_mld_level **out)
{
    vsdlss_mld_level *g=NULL; csi k,u=0,*cursor=NULL;
    qsort(e,(size_t)m,sizeof(*e),edge_compare);
    for (k=0;k<m;k++) {
        if (e[k].a==e[k].b) continue;
        if (u && e[u-1].a==e[k].a && e[u-1].b==e[k].b) e[u-1].w+=e[k].w;
        else e[u++]=e[k];
    }
    g=calloc(1,sizeof(*g));
    if (!g) return VSDLSS_ERR_OOM;
    g->n=n; g->nz=2*u;
    g->offset=calloc((size_t)n+1,sizeof(csi));
    g->neighbor=malloc((size_t)g->nz*sizeof(csi));
    g->edge_weight=malloc((size_t)g->nz*sizeof(csi));
    g->vertex_weight=malloc((size_t)n*sizeof(csi));
    g->original=malloc((size_t)n*sizeof(csi));
    cursor=malloc((size_t)n*sizeof(csi));
    if (!g->offset||(!g->neighbor&&g->nz)||(!g->edge_weight&&g->nz)||
       (!g->vertex_weight&&n)||(!g->original&&n)||(!cursor&&n)) { free(cursor); level_destroy(g); return VSDLSS_ERR_OOM; }
    for (k=0;k<u;k++) { g->offset[e[k].a+1]++; g->offset[e[k].b+1]++; }
    for (k=0;k<n;k++) { g->offset[k+1]+=g->offset[k]; cursor[k]=g->offset[k]; g->vertex_weight[k]=vw[k]; g->original[k]=orig[k]; }
    for (k=0;k<u;k++) {
        csi p=cursor[e[k].a]++; g->neighbor[p]=e[k].b; g->edge_weight[p]=e[k].w;
        p=cursor[e[k].b]++; g->neighbor[p]=e[k].a; g->edge_weight[p]=e[k].w;
    }
    free(cursor); *out=g; return VSDLSS_OK;
}

vsdlss_status vsdlss_mld_level_build(const vsdlss *A,const csi *vertices,csi count,
                                      vsdlss_mld_level **out)
{
    csi *local=NULL,*vw=NULL,*orig=NULL,col,p,m=0; edge_record *e=NULL; vsdlss_status s;
    if (!A||!out||count<0||count>A->n) return VSDLSS_ERR_INVALID;
    *out=NULL;
    local=malloc((size_t)A->n*sizeof(csi)); vw=malloc((size_t)count*sizeof(csi)); orig=malloc((size_t)count*sizeof(csi));
    e=malloc((size_t)A->p[A->n]*sizeof(*e));
    if ((!local&&A->n)||(!vw&&count)||(!orig&&count)||(!e&&A->p[A->n]>0)) { s=VSDLSS_ERR_OOM; goto done; }
    for (col=0;col<A->n;col++) local[col]=-1;
    for (col=0;col<count;col++) { csi v=vertices?vertices[col]:col; if(v<0||v>=A->n||local[v]>=0){s=VSDLSS_ERR_INVALID;goto done;} local[v]=col;vw[col]=1;orig[col]=v; }
    for (col=0;col<count;col++) { csi old=orig[col]; for(p=A->p[old];p<A->p[old+1];p++){csi row=A->i[p]; if(row!=old&&local[row]>=0){csi a=col,b=local[row];if(a>b){csi t=a;a=b;b=t;} e[m++]=(edge_record){a,b,1};}} }
    s=make_level(count,vw,orig,e,m,out);
done: free(local);free(vw);free(orig);free(e);return s;
}

vsdlss_status vsdlss_mld_coarsen_one(vsdlss_mld_level *f,vsdlss_mld_level **out)
{
    csi *order=NULL,*mate=NULL,*vw=NULL,*orig=NULL,k,j,nc=0,m=0,total=0,cap,target; edge_record *e=NULL; vsdlss_status s=VSDLSS_OK;
    if(!f||!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    order=malloc((size_t)f->n*sizeof(csi)); mate=malloc((size_t)f->n*sizeof(csi));
    if((!order&&f->n)||(!mate&&f->n)){s=VSDLSS_ERR_OOM;goto done;}
    for(k=0;k<f->n;k++){order[k]=k;mate[k]=-1;}
    for(k=0;k<f->n;k++)total+=f->vertex_weight[k];
    target=f->n/2; if(target<1)target=1;
    cap=(3*total+target-1)/target; if(cap<2)cap=2;
    for(k=1;k<f->n;k++){csi v=order[k],d=f->offset[v+1]-f->offset[v],x=k;while(x&&((f->offset[order[x-1]+1]-f->offset[order[x-1]]>d)||((f->offset[order[x-1]+1]-f->offset[order[x-1]]==d)&&f->original[order[x-1]]>f->original[v]))){order[x]=order[x-1];x--;}order[x]=v;}
    for(k=0;k<f->n;k++){csi v=order[k],best=-1,bw=-1;if(mate[v]>=0)continue;for(j=f->offset[v];j<f->offset[v+1];j++){csi w=f->neighbor[j];if(mate[w]<0&&f->vertex_weight[v]+f->vertex_weight[w]<=cap&&(f->edge_weight[j]>bw||(f->edge_weight[j]==bw&&f->original[w]<(best<0?INT64_MAX:f->original[best])))){best=w;bw=f->edge_weight[j];}}mate[v]=v;if(best>=0)mate[best]=v;}
    f->map=malloc((size_t)f->n*sizeof(csi)); if(!f->map&&f->n){s=VSDLSS_ERR_OOM;goto done;}
    for(k=0;k<f->n;k++)if(mate[k]==k){f->map[k]=nc;for(j=0;j<f->n;j++)if(j!=k&&mate[j]==k)f->map[j]=nc;nc++;}
    vw=calloc((size_t)nc,sizeof(csi));orig=malloc((size_t)nc*sizeof(csi));e=malloc((size_t)(f->nz/2)*sizeof(*e));if((!vw&&nc)||(!orig&&nc)||(!e&&f->nz)){s=VSDLSS_ERR_OOM;goto done;}
    for(k=0;k<nc;k++)orig[k]=INT64_MAX;
    for(k=0;k<f->n;k++){csi c=f->map[k];vw[c]+=f->vertex_weight[k];if(f->original[k]<orig[c])orig[c]=f->original[k];}
    for(k=0;k<f->n;k++)for(j=f->offset[k];j<f->offset[k+1];j++)if(k<f->neighbor[j]){csi a=f->map[k],b=f->map[f->neighbor[j]];if(a!=b){if(a>b){csi t=a;a=b;b=t;}e[m++]=(edge_record){a,b,f->edge_weight[j]};}}
    s=make_level(nc,vw,orig,e,m,out);
done:free(order);free(mate);free(vw);free(orig);free(e);return s;
}
