#include "vsdlss_internal.h"

#include <stdlib.h>

static csi total_weight(const vsdlss_mld_level *g)
{ csi n=vsdlss_mld_level_vertices(g),v,s=0; for(v=0;v<n;v++)s+=vsdlss_mld_level_vertex_weight(g,v); return s; }

static csi cut_weight(const vsdlss_mld_level *g,const signed char *part)
{
    csi n=vsdlss_mld_level_vertices(g),v,k,sum=0;
    for(v=0;v<n;v++){const csi *nb,*wt; csi d;vsdlss_mld_level_neighbors(g,v,&nb,&wt,&d);for(k=0;k<d;k++)if(v<nb[k]&&part[v]!=part[nb[k]])sum+=wt[k];}
    return sum;
}

static void side_weights(const vsdlss_mld_level *g,const signed char *part,csi *a,csi *b)
{ csi n=vsdlss_mld_level_vertices(g),v;*a=*b=0;for(v=0;v<n;v++)if(part[v]==0)*a+=vsdlss_mld_level_vertex_weight(g,v);else if(part[v]==1)*b+=vsdlss_mld_level_vertex_weight(g,v); }

static int balanced(csi a,csi b)
{ csi t=a+b,limit=t-t/8-(t%8!=0);return a>0&&b>0&&a<=limit&&b<=limit; }

static void refine(const vsdlss_mld_level *g,signed char *part)
{
    csi n=vsdlss_mld_level_vertices(g),pass;
    for(pass=0;pass<n;pass++){
        csi a,b,v,best=-1,best_gain=0; side_weights(g,part,&a,&b);
        for(v=0;v<n;v++){const csi *nb,*wt; csi d,k,in=0,out=0,w=vsdlss_mld_level_vertex_weight(g,v),na=a,nbw=b;
            vsdlss_mld_level_neighbors(g,v,&nb,&wt,&d);for(k=0;k<d;k++)if(part[nb[k]]==part[v])in+=wt[k];else out+=wt[k];
            if(part[v]==0){na-=w;nbw+=w;}else{nbw-=w;na+=w;}
            if(out-in>best_gain&&balanced(na,nbw)){best=v;best_gain=out-in;}
        }
        if(best<0)break;
        part[best]=(signed char)(1-part[best]);
    }
}

vsdlss_status vsdlss_mld_partition_level(const vsdlss_mld_level *g,signed char *part,csi *initial,csi *final)
{
    csi n,v,total,seed_index,seeds[5],best_cut=-1,best_initial=0; const csi *orig;
    signed char *candidate=NULL;
    if(!g||!part||(n=vsdlss_mld_level_vertices(g))<2)return VSDLSS_ERR_INVALID;
    total=total_weight(g);orig=vsdlss_mld_level_original(g);
    candidate=malloc((size_t)n); if(!candidate)return VSDLSS_ERR_OOM;
    seeds[0]=0;seeds[1]=n/4;seeds[2]=n/2;seeds[3]=(3*n)/4;seeds[4]=n-1;
    for(seed_index=0;seed_index<5;seed_index++){
        csi used=0,chosen=seeds[seed_index],candidate_initial,candidate_cut;
        for(v=0;v<n;v++)candidate[v]=-1;
        while(used<total/2){csi best=-1,best_link=-1,k;
            if(chosen>=0&&candidate[chosen]<0)best=chosen;
            else for(v=0;v<n;v++)if(candidate[v]<0){const csi *nb,*wt;csi d,link=0;vsdlss_mld_level_neighbors(g,v,&nb,&wt,&d);for(k=0;k<d;k++)if(candidate[nb[k]]==0)link+=wt[k];if(link>best_link||(link==best_link&&(best<0||orig[v]<orig[best]))){best=v;best_link=link;}}
            if(best<0)break;
            candidate[best]=0;used+=vsdlss_mld_level_vertex_weight(g,best);chosen=-1;
        }
        for(v=0;v<n;v++)if(candidate[v]<0)candidate[v]=1;
        candidate_initial=cut_weight(g,candidate);refine(g,candidate);candidate_cut=cut_weight(g,candidate);
        if(best_cut<0||candidate_cut<best_cut){for(v=0;v<n;v++)part[v]=candidate[v];best_cut=candidate_cut;best_initial=candidate_initial;}
    }
    free(candidate);
    if(initial)*initial=best_initial;
    if(final)*final=best_cut;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_mld_project_refine(const vsdlss_mld_level *fine,const signed char *coarse_part,signed char *fine_part)
{
    const csi *map; csi n,v;if(!fine||!coarse_part||!fine_part||(map=vsdlss_mld_level_map(fine))==NULL)return VSDLSS_ERR_INVALID;
    n=vsdlss_mld_level_vertices(fine);for(v=0;v<n;v++)fine_part[v]=coarse_part[map[v]];refine(fine,fine_part);return VSDLSS_OK;
}

static int augment(const vsdlss_mld_level *g,const signed char *part,csi u,csi *mr,unsigned char *seen)
{
    const csi *nb,*wt;csi d,k;(void)wt;vsdlss_mld_level_neighbors(g,u,&nb,&wt,&d);
    for(k=0;k<d;k++){csi v=nb[k];if(part[v]!=1||seen[v])continue;seen[v]=1;if(mr[v]<0||augment(g,part,mr[v],mr,seen)){mr[v]=u;return 1;}}
    return 0;
}

vsdlss_status vsdlss_mld_node_separator(const vsdlss_mld_level *g,signed char *part,csi *left,csi *right,csi *separator)
{
    csi n,v,k,*mr=NULL,*ml=NULL,*queue=NULL,head=0,tail=0;unsigned char *seen=NULL,*zl=NULL,*zr=NULL;
    if(!g||!part||!left||!right||!separator)return VSDLSS_ERR_INVALID;
    n=vsdlss_mld_level_vertices(g);
    mr=malloc((size_t)n*sizeof(csi));ml=malloc((size_t)n*sizeof(csi));queue=malloc((size_t)n*sizeof(csi));seen=malloc((size_t)n);zl=calloc((size_t)n,1);zr=calloc((size_t)n,1);
    if((!mr&&n)||(!ml&&n)||(!queue&&n)||(!seen&&n)||(!zl&&n)||(!zr&&n)){free(mr);free(ml);free(queue);free(seen);free(zl);free(zr);return VSDLSS_ERR_OOM;}
    for(v=0;v<n;v++)mr[v]=ml[v]=-1;
    for(v=0;v<n;v++)if(part[v]==0){for(k=0;k<n;k++)seen[k]=0;if(augment(g,part,v,mr,seen))for(k=0;k<n;k++)if(mr[k]==v){ml[v]=k;break;}}
    for(v=0;v<n;v++)if(part[v]==0&&ml[v]<0){zl[v]=1;queue[tail++]=v;}
    while(head<tail){csi u=queue[head++];const csi *nb,*wt;csi d;(void)wt;vsdlss_mld_level_neighbors(g,u,&nb,&wt,&d);for(k=0;k<d;k++){csi r=nb[k];if(part[r]!=1||ml[u]==r||zr[r])continue;zr[r]=1;if(mr[r]>=0&&!zl[mr[r]]){zl[mr[r]]=1;queue[tail++]=mr[r];}}}
    *left=*right=*separator=0;
    for(v=0;v<n;v++){if((part[v]==0&&!zl[v])||(part[v]==1&&zr[v]))part[v]=2;if(part[v]==0)(*left)++;else if(part[v]==1)(*right)++;else(*separator)++;}
    free(mr);free(ml);free(queue);free(seen);free(zl);free(zr);
    return VSDLSS_OK;
}
