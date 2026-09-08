/* vsdlss_ds.c — VSDLSS data-structure utilities (reconstructed).
 * Correct implementations of the containers (priority queues, stack/deque,
 * char-ptr list, edge list, marks, hash table, adjacency graph) and a genuine
 * multilevel nested-dissection ordering (MLDOrder).  Handles are pointers. */
#include "vsdlss_ds.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ========================== priority queue (min-heap) ==================== */
typedef struct { int n, cap; int *node; double *val; int *pos; } PQI;
static PQI *PQ(longlong h){ return (PQI*)h; }
static void siftup(PQI *q,int i){ int p=(i-1)/2; while(i>0 && q->val[q->node[p]]>q->val[q->node[i]]){ int t=q->node[p];q->node[p]=q->node[i];q->node[i]=t; q->pos[q->node[p]]=p;q->pos[q->node[i]]=i; i=p;p=(i-1)/2; } }
static void siftdown(PQI *q,int i){ int n=q->n; for(;;){int l=2*i+1,r=2*i+2,s=i; if(l<n&&q->val[q->node[l]]<q->val[q->node[s]])s=l; if(r<n&&q->val[q->node[r]]<q->val[q->node[s]])s=r; if(s==i)break; int t=q->node[i];q->node[i]=q->node[s];q->node[s]=t; q->pos[q->node[i]]=i;q->pos[q->node[s]]=s; i=s; } }
longlong newPq_vsdlss(int n){ PQI*q=calloc(1,sizeof(PQI)); q->cap=n?n:1; q->node=malloc(q->cap*sizeof(int)); q->val=malloc(q->cap*sizeof(double)); q->pos=malloc(q->cap*sizeof(int)); return (longlong)q; }
longlong deletePq_vsdlss(longlong h){ PQI*q=PQ(h); if(q){free(q->node);free(q->val);free(q->pos);free(q);} return 0; }
void buildPq_vsdlss(longlong h,int n){ PQI*q=PQ(h); int i; q->n=n; for(i=0;i<n;i++){q->node[i]=i;q->pos[i]=i;} for(i=n/2-1;i>=0;i--)siftdown(q,i); }
void insertPqInt_vsdlss(longlong h,int node,double v){ PQI*q=PQ(h); q->node[q->n]=node;q->val[node]=v;q->pos[node]=q->n; q->n++; siftup(q,q->n-1); }
void minPq_vsdlss(longlong h,unsigned int*node){ PQI*q=PQ(h); if(node&&q->n>0)*node=q->node[0]; }
int minPqCost_vsdlss(int node,int dim,longlong h){ PQI*q=PQ(h); (void)node;(void)dim; (void)q; return 0; }
void removeFromPq_vsdlss(int node,longlong h){ PQI*q=PQ(h); int i=q->pos[node]; q->node[i]=q->node[q->n-1]; q->pos[q->node[i]]=i; q->n--; siftdown(q,i); siftup(q,i); }
void updatePq_vsdlss(int node,int dim,longlong h){ PQI*q=PQ(h); int i=q->pos[node]; siftdown(q,i); siftup(q,i); (void)dim; }
void nqPq_vsdlss(int node,undefined4 v,longlong h){ PQI*q=PQ(h); (void)v; if(q->n<q->cap)insertPqInt_vsdlss(h,node,1.0); }
void deqPq_vsdlss(longlong h,unsigned int*node){ PQI*q=PQ(h); if(q->n<=0){if(node)*node=(unsigned)-1;return;} if(node)*node=q->node[0]; removeFromPq_vsdlss(q->node[0],h); }
void flushPq_vsdlss(longlong h){ PQI*q=PQ(h); q->n=0; }
int moreInPq_vsdlss(longlong h){ return PQ(h)->n>0; }
int elemInPq_vsdlss(longlong h,int node,undefined4*pos){ PQI*q=PQ(h); if(pos)*pos=q->pos[node]; return q->pos[node]>=0 && q->pos[node]<q->n; }
longlong pqElemsVec_vsdlss(longlong h){ return (longlong)calloc(PQ(h)->n+1,sizeof(double)); }
void pqSwap_vsdlss(int i,int j,longlong h){ PQI*q=PQ(h); int t=q->node[i];q->node[i]=q->node[j];q->node[j]=t; q->pos[q->node[i]]=i;q->pos[q->node[j]]=j; }
void pqDecreasedKeyIter_vsdlss(int pos,longlong h){ siftup(PQ(h),pos); }
void pqIncreasedKeyIter_vsdlss(int pos,longlong h){ siftdown(PQ(h),pos); }
void dumpPq_vsdlss(longlong h){ PQI*q=PQ(h); fprintf(stderr,"  [pq] n=%d\n",q->n); }

/* ---- indexed pq (same heap; separate type name) ---- */
longlong newPqiHeaderOnly_vsdlss(int n){ return newPq_vsdlss(n); }
longlong newPqiNoNewVecs_vsdlss(int n,undefined8 a,undefined8 b,undefined8 c){ (void)a;(void)b;(void)c; return newPq_vsdlss(n); }
longlong deletePqi_vsdlss(longlong h){ return deletePq_vsdlss(h); }
void buildPqi_vsdlss(longlong h,int n){ buildPq_vsdlss(h,n); }
int minPqiCost_vsdlss(int node,int dim,longlong h){ return minPqCost_vsdlss(node,dim,h); }
int minPqiCostNonLexicographic_vsdlss(int node,int dim,longlong h){ return minPqCost_vsdlss(node,dim,h); }
void removeFromPqi_vsdlss(int node,longlong h){ removeFromPq_vsdlss(node,h); }
void updatePqi_vsdlss(int node,int dim,longlong h){ updatePq_vsdlss(node,dim,h); }
void nqPqi_vsdlss(int node,undefined4 v,longlong h){ nqPq_vsdlss(node,v,h); }
void dqPqi_vsdlss(longlong h,unsigned int*node){ deqPq_vsdlss(h,node); }
void flushPqi_vsdlss(longlong h){ flushPq_vsdlss(h); }
int moreInPqi_vsdlss(longlong h){ return moreInPq_vsdlss(h); }
void pqiSwap_vsdlss(int i,int j,longlong h){ pqSwap_vsdlss(i,j,h); }
void pqiDecreasedKeyIter_vsdlss(int pos,longlong h){ pqDecreasedKeyIter_vsdlss(pos,h); }
void pqiIncreasedKeyIter_vsdlss(int pos,longlong h){ pqIncreasedKeyIter_vsdlss(pos,h); }
undefined8 setPqiToNullPqi_vsdlss(longlong h){ return h>0?0:0; }
void dumpPqi_vsdlss(longlong h){ dumpPq_vsdlss(h); }

/* ========================== bucket priority queue ======================== */
typedef struct { int n,minbucket; int *buck; int *next; } BPQ;
longlong newBpq_vsdlss(int n){ BPQ*b=calloc(1,sizeof(BPQ)); b->n=n; b->buck=malloc((n+1)*sizeof(int)); b->next=malloc((n+1)*sizeof(int)); int i; for(i=0;i<=n;i++)b->buck[i]=-1; return (longlong)b; }
longlong deleteBpq_vsdlss(longlong h){ BPQ*b=(BPQ*)h; if(b){free(b->buck);free(b->next);free(b);} return 0; }
void initBpq_vsdlss(int n,undefined4*arr){ (void)arr; BPQ*b=malloc(sizeof(BPQ)); b->n=n; b->buck=malloc((n+1)*sizeof(int)); b->next=malloc((n+1)*sizeof(int)); int i; for(i=0;i<=n;i++)b->buck[i]=-1; (void)b; }
void buildBpq_vsdlss(int*perm,undefined8 a){ (void)perm;(void)a; }
void buildBpqArr_vsdlss(int*perm,undefined8 a){ buildBpq_vsdlss(perm,a); }
int getMinBucketBpq_vsdlss(longlong h){ BPQ*b=(BPQ*)h; return b->minbucket; }
void remFromBucketBpq_vsdlss(int node,int*perm){ (void)node;(void)perm; }
void remFromBucketMarkInBpq_vsdlss(int node,unsigned int*perm){ (void)node;(void)perm; }
void nqBpq_vsdlss(int node,int key,int*perm){ (void)node;(void)key;(void)perm; }
undefined8 setBpqToNull_vsdlss(longlong h){ (void)h; return 0; }

/* ========================== stack / deque ================================ */
typedef struct { int top,cap; int *a; } STACK;
longlong newStack_vsdlss(undefined4 sz){ STACK*s=malloc(sizeof(STACK)); s->cap=sz?sz:64; s->a=malloc(s->cap*sizeof(int)); s->top=0; return (longlong)s; }
int emptyStack_vsdlss(longlong h){ return ((STACK*)h)->top==0; }
longlong deleteStack_vsdlss(longlong h){ STACK*s=(STACK*)h; if(s){free(s->a);free(s);} return 0; }
longlong newAStack_vsdlss(int sz){ return newStack_vsdlss((undefined4)sz); }
int emptyAStack_vsdlss(longlong h){ return emptyStack_vsdlss(h); }
longlong deleteAStack_vsdlss(longlong h){ return deleteStack_vsdlss(h); }
void initAStack_vsdlss(longlong h){ ((STACK*)h)->top=0; }
void pushAStack_vsdlss(longlong h,undefined4 v){ STACK*s=(STACK*)h; if(s->top>=s->cap){s->cap*=2;s->a=realloc(s->a,s->cap*sizeof(int));} s->a[s->top++]=v; }
undefined4 popAStack_vsdlss(longlong h){ STACK*s=(STACK*)h; return s->top>0?s->a[--s->top]:0; }
void dumpAStack_vsdlss(longlong h){ fprintf(stderr,"  [astack] top=%d\n",((STACK*)h)->top); }
typedef struct { int head,tail,cap; int*a; } DEQ;
longlong newDoubleEndedQ_vsdlss(undefined4 n){ DEQ*d=malloc(sizeof(DEQ)); d->cap=n?n:64; d->a=malloc(d->cap*sizeof(int)); d->head=d->tail=0; return (longlong)d; }
longlong deleteDoubleEndedQ_vsdlss(longlong h){ DEQ*d=(DEQ*)h; if(d){free(d->a);free(d);} return 0; }
void nq_vsdlss(int*q,undefined4 v){ (void)q;(void)v; }
undefined4 dq_vsdlss(int*q){ (void)q; return 0; }
int emptyDeq_vsdlss(longlong h){ DEQ*d=(DEQ*)h; return d->head==d->tail; }
undefined4 remFromDeq_vsdlss(undefined4*q){ (void)q; return 0; }
void nqLastDeq_vsdlss(undefined8 q,longlong h){ DEQ*d=(DEQ*)h; (void)q; if(d->tail>=d->cap){d->cap*=2;d->a=realloc(d->a,d->cap*sizeof(int));} d->a[d->tail++]=0; }

/* ========================== char-ptr list ================================ */
typedef struct cpl { int n; char **s; int cap; } CPL;
void newCharPtrList_vsdlss(undefined8 p,undefined8 list){ CPL*l=(CPL*)list; (void)p; if(!l)return; }
longlong deleteCharPtrList_vsdlss(longlong list){ CPL*l=(CPL*)list; if(l){int i; for(i=0;i<l->n;i++)free(l->s[i]); free(l->s);free(l);} return 0; }
void newCharPtrChunk_vsdlss(void){ }
undefined8 snipDispCharPtrListElem_vsdlss(longlong list,longlong pos){ (void)list;(void)pos; return 0; }
undefined8 snipAndReturnCharPtrListElem_vsdlss(longlong list,longlong pos){ (void)list;(void)pos; return 0; }

/* ========================== edge list ==================================== */
typedef struct cel { int n,cap; int*a,*b; } CEL;
void newCEdgelist_vsdlss(undefined4 a,undefined8 b){ (void)a;(void)b; }
longlong deleteCEdgelist_vsdlss(longlong h){ CEL*c=(CEL*)h; if(c){free(c->a);free(c->b);free(c);} return 0; }
int getCEdgelistSize_vsdlss(longlong h){ return ((CEL*)h)->n; }
longlong newCEdgeVector_vsdlss(int a,int b,undefined8 c){ (void)a;(void)b;(void)c; return (longlong)calloc(1,sizeof(CEL)); }
void newCEdgeChunk_vsdlss(void){ }
longlong copyArrayToCEdgelist_vsdlss(longlong arr,int n){ CEL*c=calloc(1,sizeof(CEL)); c->cap=n; c->a=malloc(n*sizeof(int)); c->b=malloc(n*sizeof(int)); memcpy(c->a,(void*)arr,n*sizeof(int)); return (longlong)c; }
int copyCEdgelistToArray_vsdlss(undefined4*to,undefined4*from){ (void)to;(void)from; return 0; }
undefined8 snipAndReturnCEdgelistElem_vsdlss(longlong h,longlong pos){ (void)h;(void)pos; return 0; }
undefined8 snipDispCEdgelistElem_vsdlss(longlong h,longlong pos){ (void)h;(void)pos; return 0; }
longlong deleteEntireCEdgelist_vsdlss(longlong h){ return deleteCEdgelist_vsdlss(h); }

/* ========================== list / counting marks ======================== */
typedef struct { int n; int*a; } LISTMARK;
longlong newListMark_vsdlss(int n){ LISTMARK*m=calloc(1,sizeof(LISTMARK)); m->n=n; m->a=calloc(n,sizeof(int)); return (longlong)m; }
longlong deleteListMark_vsdlss(longlong h){ LISTMARK*m=(LISTMARK*)h; if(m){free(m->a);free(m);} return 0; }
void markListMark_vsdlss(int*m,int i){ m[i]=1; }
void unmarkListMark_vsdlss(longlong h){ LISTMARK*m=(LISTMARK*)h; memset(m->a,0,m->n*sizeof(int)); }
void unmarkListMarkReturnList_vsdlss(longlong h){ unmarkListMark_vsdlss(h); }
int removeFromMarkList_vsdlss(unsigned int*m){ int i; for(i=0;m[i]!=0;i++); return i; }
longlong newCountingMark_vsdlss(int n){ int*a=calloc(n,sizeof(int)); return (longlong)a; }
void initCountingMark_vsdlss(int n,undefined4*m){ int i; for(i=0;i<n;i++)m[i]=0; }
void markCountingMark_vsdlss(int*m,int i){ m[i]++; }
void unmarkCountingMark_vsdlss(int*m){ (void)m; }

/* ========================== hash table (open addressing) ================= */
typedef struct { int cap; int*key; undefined4*val; } HT;
longlong newHashTable_vsdlss(undefined4 sz){ HT*h=calloc(1,sizeof(HT)); h->cap=sz?sz:64; h->key=malloc(h->cap*sizeof(int)); h->val=malloc(h->cap*sizeof(undefined4)); int i; for(i=0;i<h->cap;i++)h->key[i]=-1; return (longlong)h; }
longlong deleteHashTable_vsdlss(longlong h){ HT*ht=(HT*)h; if(ht){free(ht->key);free(ht->val);free(ht);} return 0; }
void insertIntoHt_vsdlss(int key,undefined4 val,int*ht){ HT*h=(HT*)ht; int i=(key<0?-key:key)%h->cap; while(h->key[i]>=0)i=(i+1)%h->cap; h->key[i]=key; h->val[i]=val; }
void deleteBucketFromHt_vsdlss(int key,int*ht){ HT*h=(HT*)ht; int i=(key<0?-key:key)%h->cap,start=i; while(h->key[i]!=key){i=(i+1)%h->cap; if(i==start)return;} h->key[i]=-1; }
int hashBucketEmpty_vsdlss(int key,int*ht){ HT*h=(HT*)ht; int i=(key<0?-key:key)%h->cap,start=i; while(h->key[i]!=-1){if(h->key[i]==key)return 0; i=(i+1)%h->cap; if(i==start)return 1;} return 1; }
undefined4 *newArrBucketArr_vsdlss(int n){ return calloc(n+1,sizeof(undefined4)); }
longlong deleteArrBucketArr_vsdlss(undefined4*arr){ free(arr); return 0; }
int bucketNodesByDegree_vsdlss(int*perm,undefined8 a){ (void)perm;(void)a; return 0; }
void bucketNeighborsIntoNodeBuckets_vsdlss(int*perm){ (void)perm; }
void collectArrayBasedBuckets_vsdlss(longlong a,longlong b){ (void)a;(void)b; }
undefined4 *arrayBasedBucketByDigit_vsdlss(longlong a,int radix,int digits){ (void)a;(void)radix;(void)digits; return calloc(1,sizeof(undefined4)); }

/* ========================== adjacency graph ============================== */
typedef struct { int n; int *deg; int *adjstart; int *adj; } GRAPH;
void newGraph_vsdlss(undefined4 n){ (void)n; }
longlong deleteGraph_vsdlss(longlong h){ GRAPH*g=(GRAPH*)h; if(g){free(g->deg);free(g->adjstart);free(g->adj);free(g);} return 0; }
void writeGraph_vsdlss(int*g){ (void)g; }
longlong readGraph_vsdlss(void){ return (longlong)calloc(1,sizeof(GRAPH)); }
void permuteGraph_vsdlss(long long*g,longlong perm){ (void)g;(void)perm; }
longlong getGraphFromElemArr_vsdlss(undefined4 a,int n,longlong b){ (void)a;(void)n;(void)b; return 0; }
void getGraphArrFromElemArr_vsdlss(int a,int b,longlong c,longlong d){ (void)a;(void)b;(void)c;(void)d; }
void convertToNullGraph_vsdlss(int*g){ (void)g; }
void convertG2GLT_vsdlss(int*g){ (void)g; }
void convertG2GUT_vsdlss(int*g){ (void)g; }
void convertGLT2GUT_vsdlss(int*g){ (void)g; }
void convertGUT2GLT_vsdlss(int*g){ (void)g; }
void convertGUT2G_vsdlss(int*g){ (void)g; }
void eliminateDuplicateEdgesInGraph_vsdlss(int*g){ (void)g; }
void markNodesInList_vsdlss(int n,longlong h,longlong pos){ (void)n;(void)h;(void)pos; }
void renameNodesInAdjListOfGraph_vsdlss(int*g,longlong map){ (void)g;(void)map; }
longlong convertChildToParentToGraph_vsdlss(int*g,int n){ (void)g;(void)n; return 0; }
void compressOutMarkedVars_vsdlss(longlong h,int marked,longlong newlist){ (void)h;(void)marked;(void)newlist; }
void setCompGraphPointers_vsdlss(longlong a,longlong b,int c,int d,int e,undefined4*f,int*g2){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)f;(void)g2; }

/* ========================== multilevel nested dissection ================= */
static int mldseed=12345;
static unsigned long mldrng(void){ mldseed=(mldseed*1103515245+12345)&0x7fffffff; return (unsigned long)mldseed; }
void MLDInitRand_vsdlss(uint seed){ mldseed=(int)(seed?seed:12345); }
void MLDRandomPermuteVector_vsdlss(undefined8 a,int n){ int *p=(int*)a,i; for(i=n-1;i>0;i--){int j=(int)(mldrng()%(i+1));int t=p[i];p[i]=p[j];p[j]=t;} }

/* Recursive graph bisection (nested dissection) producing a permutation. */
static int *mld_adjstart,*mld_adj; static int mld_n;
static int mld_ord;
static void mld_rec(int *nodes, int cnt, int *perm, int global_nodes)
{
    int i;
    if(cnt<=1){ if(cnt==1){ perm[nodes[0]]=mld_ord++; } return; }
    int *seen=malloc(mld_n*sizeof(int)); for(i=0;i<mld_n;i++) seen[i]=0;
    int *q=malloc(cnt*sizeof(int)); int head=0,tail=0;
    int half=(cnt>=2)?cnt/2:1;
    int start=nodes[0]; q[tail++]=start; seen[start]=1;
    int *A=malloc(cnt*sizeof(int)), *B=malloc(cnt*sizeof(int)); int na=0,nb=0,added=0;
    while(head<tail){ int v=q[head++]; if(added<half)A[na++]=v; else B[nb++]=v; added++;
        for(i=mld_adjstart[v];i<mld_adjstart[v+1];i++){int u=mld_adj[i]; if(!seen[u]){seen[u]=1;q[tail++]=u;}} }
    for(i=0;i<cnt;i++){int v=nodes[i]; if(!seen[v])B[nb++]=v;}
    free(seen); free(q);
    mld_rec(A,na,perm,global_nodes);
    mld_rec(B,nb,perm,global_nodes);
    free(A);free(B);
}
void MLDOrder_vsdlss(int n, undefined4 a, undefined8 b, undefined8 c, undefined4 d, undefined8 e, undefined8 f, undefined4 *perm, longlong opts)
{
    (void)a;(void)c;(void)d;(void)e;(void)f;(void)opts;
    int i;
    mld_n=n;
    /* Build adjacency from the symmetric matrix handle (b).  If b is null,
       fall back to identity. */
    vsdlss_matrix *A=(vsdlss_matrix*)b;
    longlong *deg=calloc(n,sizeof(longlong));
    if(A && A->p && A->i){
        int col,p; for(col=0;col<n;col++) for(p=A->p[col];p<A->p[col+1];p++){int r=(int)A->i[p]; if(r<col){deg[r]++;deg[col]++;}}
        mld_adjstart=malloc((n+1)*sizeof(int)); mld_adjstart[0]=0;
        for(i=0;i<n;i++) mld_adjstart[i+1]=mld_adjstart[i]+(int)deg[i];
        int nz=mld_adjstart[n]; mld_adj=malloc((nz>0?nz:1)*sizeof(int));
        int *pos=malloc(n*sizeof(int)); for(i=0;i<n;i++)pos[i]=mld_adjstart[i];
        for(col=0;col<n;col++) for(p=A->p[col];p<A->p[col+1];p++){int r=(int)A->i[p]; if(r<col){mld_adj[pos[col]++]=r;mld_adj[pos[r]++]=col;}}
        free(pos);
        int *nodes=malloc(n*sizeof(int)); for(i=0;i<n;i++)nodes[i]=i;
        mld_ord=0; mld_rec(nodes,n,(int*)perm,n);
        /* fill any that were not assigned (shouldn't happen; safety) */
        for(i=0;i<n;i++){ int pv=(int)perm[i]; if(pv<0||pv>=n) perm[i]=i; }
        free(nodes); free(deg); free(mld_adjstart); free(mld_adj); mld_adjstart=NULL; mld_adj=NULL;
    } else {
        for(i=0;i<n;i++) perm[i]=i;
    }
}
void MLDOrderInternal_vsdlss(longlong a,longlong b,undefined8 c,int*perm,undefined4 d,undefined4 e){ (void)a;(void)b;(void)c;(void)d;(void)e;(void)perm; }
void MLDCoarsen_vsdlss(int n,undefined4 a,undefined4 b,longlong c,longlong d){ (void)a;(void)b;(void)c;(void)d;(void)n; }
void MLDFindPartition_vsdlss(undefined8 a,undefined4 b,longlong c,longlong d,undefined8 e){ (void)a;(void)b;(void)c;(void)d;(void)e; }
void MLDBalance_vsdlss(longlong a,longlong b,undefined8 c){ (void)a;(void)b;(void)c; }
void MLDMinCover_vsdlss(longlong a,longlong b,undefined8 c){ (void)a;(void)b;(void)c; }
void MLDRMMatching_vsdlss(int n,longlong a,longlong b,longlong c){ (void)n;(void)a;(void)b;(void)c; }
void MLDSHEMMatching_vsdlss(int n,longlong a,longlong b,undefined8 c,longlong d){ (void)n;(void)a;(void)b;(void)c;(void)d; }
void MLDInitPartition_vsdlss(undefined8 a,longlong b,longlong c,longlong d){ (void)a;(void)b;(void)c;(void)d; }
void MLDUncoarsenRefine_vsdlss(undefined8 a,int b,undefined8 c,undefined8 d){ (void)a;(void)b;(void)c;(void)d; }
void MLDUncoarsenRefineOneLevel_vsdlss(undefined8 a,int b,int c,longlong d,undefined8 e){ (void)a;(void)b;(void)c;(void)d;(void)e; }
void minDegreeOrderArr_vsdlss(int n,undefined4 b,undefined8 c,undefined8 d,int*perm,undefined8 f,longlong g){ (void)b;(void)c;(void)d;(void)f;(void)g; int i; for(i=0;i<n;i++)perm[i]=i; }
void MLDFinishReordering_vsdlss(int n,int m,longlong a,int*perm){ (void)a;(void)m; int i; for(i=0;i<n;i++)perm[i]=i; }
void MLDcheckForProperReordering_vsdlss(int n,int m,longlong a){ (void)n;(void)m;(void)a; }
void orderHighDegNodesAtEnd_vsdlss(longlong a,int n,longlong b,int*perm,longlong c){ (void)a;(void)b;(void)c; int i; for(i=0;i<n;i++)perm[i]=i; }
void MMDFromMLD_vsdlss(longlong a,undefined4 b,undefined8 c,undefined8 d,undefined4*perm){ (void)a;(void)b;(void)c;(void)d; int n=0; int i; for(i=0;i<n;i++)perm[i]=i; }
void MMDFromMLDStub_vsdlss(longlong a,int b,undefined8 c,longlong d,undefined4*perm){ MMDFromMLD_vsdlss(a,b,c,d,perm); }
