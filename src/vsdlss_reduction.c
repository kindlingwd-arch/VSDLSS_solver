#include "vsdlss_m3_internal.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { csi vertex; double value; } numeric_edge;
typedef struct { numeric_edge *edge; csi count, capacity; } numeric_list;
typedef struct { csi degree, vertex; } heap_entry;
typedef struct { heap_entry *entry; csi count, capacity; } min_heap;

static int checked_count(csi n, size_t width)
{ return n >= 0 && (uint64_t)n <= SIZE_MAX / width; }

static csi edge_lower_bound(const numeric_list *list, csi vertex)
{
    csi lo=0, hi=list->count;
    while(lo<hi) { csi mid=lo+(hi-lo)/2; if(list->edge[mid].vertex<vertex) lo=mid+1; else hi=mid; }
    return lo;
}

static vsdlss_status list_reserve(numeric_list *list, csi needed)
{
    csi capacity; numeric_edge *edge;
    if(needed<=list->capacity) return VSDLSS_OK;
    capacity=list->capacity?list->capacity:4;
    while(capacity<needed) { if(capacity>INT64_MAX/2) return VSDLSS_ERR_OOM; capacity*=2; }
    if(!checked_count(capacity,sizeof(*edge))) return VSDLSS_ERR_OOM;
    edge=(numeric_edge *)realloc(list->edge,(size_t)capacity*sizeof(*edge));
    if(!edge) return VSDLSS_ERR_OOM;
    list->edge=edge; list->capacity=capacity; return VSDLSS_OK;
}

static void list_insert_reserved(numeric_list *list, csi at, csi vertex, double value)
{
    memmove(list->edge+at+1,list->edge+at,(size_t)(list->count-at)*sizeof(*list->edge));
    list->edge[at].vertex=vertex; list->edge[at].value=value; list->count++;
}

static vsdlss_status edge_set(numeric_list *adj, csi a, csi b, double value, int add)
{
    csi pa=edge_lower_bound(adj+a,b), pb=edge_lower_bound(adj+b,a);
    int has=pa<adj[a].count && adj[a].edge[pa].vertex==b;
    if(has) {
        double updated=add?adj[a].edge[pa].value+value:value;
        if(!isfinite(updated)) return VSDLSS_ERR_NONFINITE;
        adj[a].edge[pa].value=adj[b].edge[pb].value=updated; return VSDLSS_OK;
    }
    if(adj[a].count==INT64_MAX || adj[b].count==INT64_MAX) return VSDLSS_ERR_OOM;
    if(list_reserve(adj+a,adj[a].count+1)!=VSDLSS_OK ||
       list_reserve(adj+b,adj[b].count+1)!=VSDLSS_OK) return VSDLSS_ERR_OOM;
    list_insert_reserved(adj+a,pa,b,value); list_insert_reserved(adj+b,pb,a,value);
    return VSDLSS_OK;
}

static void edge_remove(numeric_list *list, csi vertex)
{
    csi at=edge_lower_bound(list,vertex);
    if(at<list->count && list->edge[at].vertex==vertex) {
        memmove(list->edge+at,list->edge+at+1,(size_t)(list->count-at-1)*sizeof(*list->edge));
        list->count--;
    }
}

static int heap_less(heap_entry a, heap_entry b)
{ return a.degree<b.degree || (a.degree==b.degree && a.vertex<b.vertex); }

static vsdlss_status heap_push(min_heap *heap, csi degree, csi vertex)
{
    csi at; heap_entry value={degree,vertex};
    if(degree>3) return VSDLSS_OK;
    if(heap->count==heap->capacity) {
        csi capacity; heap_entry *entry;
        if(heap->capacity>INT64_MAX/2) return VSDLSS_ERR_OOM;
        capacity=heap->capacity?heap->capacity*2:16;
        if(!checked_count(capacity,sizeof(*entry))) return VSDLSS_ERR_OOM;
        entry=(heap_entry *)realloc(heap->entry,(size_t)capacity*sizeof(*entry));
        if(!entry) return VSDLSS_ERR_OOM;
        heap->entry=entry; heap->capacity=capacity;
    }
    at=heap->count++;
    while(at>0) { csi parent=(at-1)/2; if(!heap_less(value,heap->entry[parent])) break;
        heap->entry[at]=heap->entry[parent]; at=parent; }
    heap->entry[at]=value; return VSDLSS_OK;
}

static heap_entry heap_pop(min_heap *heap)
{
    heap_entry result=heap->entry[0], last=heap->entry[--heap->count]; csi at=0;
    while(at<heap->count) { csi child=2*at+1; if(child>=heap->count) break;
        if(child+1<heap->count && heap_less(heap->entry[child+1],heap->entry[child])) child++;
        if(!heap_less(heap->entry[child],last)) break;
        heap->entry[at]=heap->entry[child]; at=child;
    }
    if(heap->count) heap->entry[at]=last;
    return result;
}

void vsdlss_reduction_free(vsdlss_reduction *reduction)
{
    if(!reduction) return;
    free(reduction->records); free(reduction->core_vertices);
    vsdlss_spfree(reduction->core); free(reduction);
}

vsdlss_status vsdlss_reduce(const vsdlss *A, vsdlss_reduction **out)
{
    vsdlss_reduction *r=NULL; numeric_list *adj=NULL; double *diag=NULL;
    unsigned char *active=NULL; csi *local=NULL; min_heap heap={0}; vsdlss_status status;
    csi n,col,k,v,remaining,core_n,nnz,at;
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(!A || A->n<1) return VSDLSS_ERR_INVALID;
    if(A->n==INT64_MAX || !checked_count(A->n,sizeof(*adj)) ||
       !checked_count(A->n,sizeof(*diag)) || !checked_count(A->n+1,sizeof(csi)))
        return VSDLSS_ERR_OOM;
    status=vsdlss_validate_upper_csc(A); if(status!=VSDLSS_OK) return status;
    r=(vsdlss_reduction *)calloc(1,sizeof(*r));
    adj=(numeric_list *)calloc((size_t)A->n,sizeof(*adj));
    diag=(double *)calloc((size_t)A->n,sizeof(*diag)); active=(unsigned char *)malloc((size_t)A->n);
    if(!r||!adj||!diag||!active) { status=VSDLSS_ERR_OOM; goto fail; }
    r->n=n=A->n; memset(active,1,(size_t)n);
    r->records=(vsdlss_elim_record *)calloc((size_t)n,sizeof(*r->records));
    if(!r->records) { status=VSDLSS_ERR_OOM; goto fail; }
    for(col=0;col<n;col++) for(k=A->p[col];k<A->p[col+1];k++) {
        csi row=A->i[k];
        if(row==col) { diag[col]+=A->x[k]; if(!isfinite(diag[col])) {status=VSDLSS_ERR_NONFINITE;goto fail;} }
        else { status=edge_set(adj,row,col,A->x[k],1); if(status!=VSDLSS_OK) goto fail; }
    }
    for(v=0;v<n;v++) { status=heap_push(&heap,adj[v].count,v); if(status!=VSDLSS_OK) goto fail; }
    remaining=n;
    while(heap.count) {
        heap_entry h=heap_pop(&heap); numeric_list *list; vsdlss_elim_record *record; csi a,b;
        v=h.vertex; if(!active[v] || adj[v].count!=h.degree || h.degree>3) continue;
        if(!isfinite(diag[v])) {status=VSDLSS_ERR_NONFINITE;goto fail;}
        if(diag[v]<=0.0) {status=VSDLSS_ERR_NOT_POSDEF;goto fail;}
        list=adj+v; record=r->records+r->count; record->vertex=v;
        record->degree=list->count; record->pivot=diag[v];
        for(a=0;a<list->count;a++) {
            csi w=list->edge[a].vertex; double avw=list->edge[a].value;
            record->neighbor[a]=w; record->multiplier[a]=avw/diag[v];
            if(!isfinite(record->multiplier[a])) {status=VSDLSS_ERR_NONFINITE;goto fail;}
            diag[w]-=record->multiplier[a]*avw;
            if(!isfinite(diag[w])) {status=VSDLSS_ERR_NONFINITE;goto fail;}
        }
        for(a=0;a<list->count;a++) for(b=a+1;b<list->count;b++) {
            csi wa=list->edge[a].vertex, wb=list->edge[b].vertex;
            double updated=-record->multiplier[a]*list->edge[b].value;
            if(!isfinite(updated)) {status=VSDLSS_ERR_NONFINITE;goto fail;}
            status=edge_set(adj,wa,wb,updated,1); if(status!=VSDLSS_OK) goto fail;
        }
        for(a=0;a<list->count;a++) edge_remove(adj+list->edge[a].vertex,v);
        active[v]=0; r->count++; remaining--;
        for(a=0;a<list->count;a++) { csi w=list->edge[a].vertex;
            status=heap_push(&heap,adj[w].count,w); if(status!=VSDLSS_OK) goto fail; }
    }
    r->core_n=core_n=remaining;
    if(core_n) {
        r->core_vertices=(csi *)malloc((size_t)core_n*sizeof(csi));
        local=(csi *)malloc((size_t)n*sizeof(csi));
        if(!r->core_vertices || !local) {status=VSDLSS_ERR_OOM;goto fail;}
        nnz=core_n;
        for(v=0;v<n;v++) if(active[v]) {
            if(adj[v].count>INT64_MAX-nnz) {status=VSDLSS_ERR_OOM;goto fail;} nnz+=adj[v].count;
        }
        /* Each undirected edge was counted twice above. */
        nnz=core_n+(nnz-core_n)/2;
        r->core=vsdlss_spalloc(core_n,core_n,nnz,1,0);
        if(!r->core) {status=VSDLSS_ERR_OOM;goto fail;}
        at=0; for(v=0;v<n;v++) if(active[v]) { r->core_vertices[at]=v; local[v]=at++; }
        at=0; r->core->p[0]=0;
        for(col=0;col<core_n;col++) {
            csi original=r->core_vertices[col], e;
            for(e=0;e<adj[original].count;e++) {
                csi neighbor=adj[original].edge[e].vertex;
                if(neighbor>=original) continue;
                r->core->i[at]=local[neighbor]; r->core->x[at++]=adj[original].edge[e].value;
            }
            r->core->i[at]=col; r->core->x[at++]=diag[original]; r->core->p[col+1]=at;
        }
    }
    for(v=0;v<n;v++) free(adj[v].edge);
    free(adj); free(diag); free(active); free(local); free(heap.entry);
    *out=r; return VSDLSS_OK;
fail:
    if(adj) for(v=0;v<A->n;v++) free(adj[v].edge);
    free(adj); free(diag); free(active); free(local); free(heap.entry);
    vsdlss_reduction_free(r); return status;
}
