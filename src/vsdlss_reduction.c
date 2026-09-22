#include "vsdlss_m3_internal.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { csi vertex; double value; } numeric_edge;

/* Adjacency lists live in one arena and are addressed by offset, so growth
 * never invalidates other lists; a list that outgrows its slot moves to the
 * arena end with doubled capacity.  Lists are kept sorted by vertex. */
typedef struct { csi off, count, capacity; } numeric_list;
typedef struct { numeric_edge *edge; csi used, capacity; } edge_arena;

/* Hierarchical bitmap: set/clear O(levels), minimum O(levels).  One per
 * degree 0..3 replaces the former binary heap with lazy deletion; popping the
 * lowest non-empty degree's minimum vertex yields exactly the heap's order
 * (smallest current degree, ties to the smallest vertex). */
#define BS_LEVELS 8
typedef struct { int levels; uint64_t *word[BS_LEVELS]; } min_bitset;

static int checked_count(csi n, size_t width)
{ return n >= 0 && (uint64_t)n <= SIZE_MAX / width; }

static int ctz64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(x);
#else
    int k=0; while(!(x&1)){x>>=1;k++;} return k;
#endif
}

static void bs_free(min_bitset *s)
{ for(int l=0;l<BS_LEVELS;l++) { free(s->word[l]); s->word[l]=NULL; } s->levels=0; }

static vsdlss_status bs_init(min_bitset *s, csi n)
{
    csi size=n;
    memset(s,0,sizeof(*s));
    do {
        size=(size+63)/64;
        if(s->levels==BS_LEVELS) return VSDLSS_ERR_OOM;
        s->word[s->levels]=(uint64_t *)calloc((size_t)size,sizeof(uint64_t));
        if(!s->word[s->levels]) { bs_free(s); return VSDLSS_ERR_OOM; }
        s->levels++;
    } while(size>1);
    return VSDLSS_OK;
}

static void bs_set(min_bitset *s, csi v)
{
    for(int l=0;l<s->levels;l++) {
        uint64_t *w=s->word[l]+(v>>6); int had=*w!=0;
        *w|=(uint64_t)1<<(v&63);
        if(had) return;
        v>>=6;
    }
}

static void bs_clear(min_bitset *s, csi v)
{
    for(int l=0;l<s->levels;l++) {
        uint64_t *w=s->word[l]+(v>>6);
        *w&=~((uint64_t)1<<(v&63));
        if(*w) return;
        v>>=6;
    }
}

static csi bs_min(const min_bitset *s)
{
    csi idx=0;
    if(!s->word[s->levels-1][0]) return -1;
    for(int l=s->levels-1;l>=0;l--) idx=idx*64+ctz64(s->word[l][idx]);
    return idx;
}

static csi edge_lower_bound(const numeric_edge *e, csi count, csi vertex)
{
    csi lo=0, hi=count;
    while(lo<hi) { csi mid=lo+(hi-lo)/2; if(e[mid].vertex<vertex) lo=mid+1; else hi=mid; }
    return lo;
}

static vsdlss_status list_reserve(edge_arena *arena, numeric_list *list, csi needed)
{
    csi capacity, need_arena;
    if(needed<=list->capacity) return VSDLSS_OK;
    capacity=list->capacity?list->capacity:4;
    while(capacity<needed) { if(capacity>INT64_MAX/2) return VSDLSS_ERR_OOM; capacity*=2; }
    if(arena->used>INT64_MAX-capacity) return VSDLSS_ERR_OOM;
    need_arena=arena->used+capacity;
    if(need_arena>arena->capacity) {
        csi grow=arena->capacity?arena->capacity:16; numeric_edge *edge;
        while(grow<need_arena) { if(grow>INT64_MAX/2) return VSDLSS_ERR_OOM; grow*=2; }
        if(!checked_count(grow,sizeof(*edge))) return VSDLSS_ERR_OOM;
        edge=(numeric_edge *)realloc(arena->edge,(size_t)grow*sizeof(*edge));
        if(!edge) return VSDLSS_ERR_OOM;
        arena->edge=edge; arena->capacity=grow;
    }
    if(list->count)
        memcpy(arena->edge+arena->used,arena->edge+list->off,(size_t)list->count*sizeof(numeric_edge));
    list->off=arena->used; list->capacity=capacity; arena->used+=capacity;
    return VSDLSS_OK;
}

static vsdlss_status edge_set(edge_arena *arena, numeric_list *adj, csi a, csi b, double value)
{
    numeric_edge *ea=arena->edge+adj[a].off, *eb;
    csi pa=edge_lower_bound(ea,adj[a].count,b), pb;
    if(pa<adj[a].count && ea[pa].vertex==b) {
        double updated=ea[pa].value+value;
        eb=arena->edge+adj[b].off; pb=edge_lower_bound(eb,adj[b].count,a);
        if(!isfinite(updated)) return VSDLSS_ERR_NONFINITE;
        ea[pa].value=eb[pb].value=updated; return VSDLSS_OK;
    }
    if(adj[a].count==INT64_MAX || adj[b].count==INT64_MAX) return VSDLSS_ERR_OOM;
    if(list_reserve(arena,adj+a,adj[a].count+1)!=VSDLSS_OK ||
       list_reserve(arena,adj+b,adj[b].count+1)!=VSDLSS_OK) return VSDLSS_ERR_OOM;
    ea=arena->edge+adj[a].off; eb=arena->edge+adj[b].off;
    pb=edge_lower_bound(eb,adj[b].count,a);
    memmove(ea+pa+1,ea+pa,(size_t)(adj[a].count-pa)*sizeof(*ea));
    ea[pa].vertex=b; ea[pa].value=value; adj[a].count++;
    memmove(eb+pb+1,eb+pb,(size_t)(adj[b].count-pb)*sizeof(*eb));
    eb[pb].vertex=a; eb[pb].value=value; adj[b].count++;
    return VSDLSS_OK;
}

static void edge_remove(edge_arena *arena, numeric_list *list, csi vertex)
{
    numeric_edge *e=arena->edge+list->off;
    csi at=edge_lower_bound(e,list->count,vertex);
    if(at<list->count && e[at].vertex==vertex) {
        memmove(e+at,e+at+1,(size_t)(list->count-at-1)*sizeof(*e));
        list->count--;
    }
}

static int cmp_edge(const void *x, const void *y)
{
    csi a=((const numeric_edge *)x)->vertex, b=((const numeric_edge *)y)->vertex;
    return (a>b)-(a<b);
}

void vsdlss_reduction_free(vsdlss_reduction *reduction)
{
    if(!reduction) return;
    free(reduction->records); free(reduction->core_vertices);
    vsdlss_spfree(reduction->core); free(reduction);
}

static int valid_reduction_shape(const vsdlss_reduction *r)
{
    return r && r->n>=0 && r->count>=0 && r->core_n>=0 &&
           r->count<=r->n && r->core_n==r->n-r->count &&
           (!r->count || r->records) && (!r->core_n || r->core_vertices);
}

vsdlss_status vsdlss_reduce_rhs(const vsdlss_reduction *r, const double *b,
                                double *core_rhs, double *saved)
{
    double *work=NULL, *next_core=NULL, *next_saved=NULL;
    csi k,j; vsdlss_status status=VSDLSS_OK;
    if(!valid_reduction_shape(r) || (!b && r->n) || (!core_rhs && r->core_n) ||
       (!saved && r->count)) return VSDLSS_ERR_INVALID;
    if(!checked_count(r->n,sizeof(*work)) ||
       !checked_count(r->core_n,sizeof(*next_core)) ||
       !checked_count(r->count,sizeof(*next_saved))) return VSDLSS_ERR_OOM;
    if(r->n) work=(double *)malloc((size_t)r->n*sizeof(*work));
    if(r->core_n) next_core=(double *)malloc((size_t)r->core_n*sizeof(*next_core));
    if(r->count) next_saved=(double *)malloc((size_t)r->count*sizeof(*next_saved));
    if((r->n&&!work)||(r->core_n&&!next_core)||(r->count&&!next_saved)) {
        status=VSDLSS_ERR_OOM; goto done;
    }
    for(k=0;k<r->n;k++) {
        if(!isfinite(b[k])) {status=VSDLSS_ERR_NONFINITE;goto done;}
        work[k]=b[k];
    }
    for(k=0;k<r->count;k++) {
        const vsdlss_elim_record *record=r->records+k;
        if(record->vertex<0 || record->vertex>=r->n || record->degree<0 ||
           record->degree>3) {status=VSDLSS_ERR_INVALID;goto done;}
        next_saved[k]=work[record->vertex];
        if(!isfinite(next_saved[k])) {status=VSDLSS_ERR_NONFINITE;goto done;}
        for(j=0;j<record->degree;j++) {
            csi neighbor=record->neighbor[j]; double update;
            if(neighbor<0 || neighbor>=r->n) {status=VSDLSS_ERR_INVALID;goto done;}
            update=record->multiplier[j]*next_saved[k];
            if(!isfinite(record->multiplier[j]) || !isfinite(update) ||
               !isfinite(work[neighbor]-update)) {status=VSDLSS_ERR_NONFINITE;goto done;}
            work[neighbor]-=update;
        }
    }
    for(k=0;k<r->core_n;k++) {
        csi vertex=r->core_vertices[k];
        if(vertex<0 || vertex>=r->n) {status=VSDLSS_ERR_INVALID;goto done;}
        next_core[k]=work[vertex];
        if(!isfinite(next_core[k])) {status=VSDLSS_ERR_NONFINITE;goto done;}
    }
    if(r->core_n) memcpy(core_rhs,next_core,(size_t)r->core_n*sizeof(*core_rhs));
    if(r->count) memcpy(saved,next_saved,(size_t)r->count*sizeof(*saved));
done:
    free(work); free(next_core); free(next_saved); return status;
}

vsdlss_status vsdlss_reduce_recover(const vsdlss_reduction *r,
                                    const double *saved,
                                    const double *core_solution, double *x)
{
    double *next_x=NULL, *saved_copy=NULL, *core_copy=NULL;
    csi k,j; vsdlss_status status=VSDLSS_OK;
    if(!valid_reduction_shape(r) || (!x && r->n) || (!saved && r->count) ||
       (!core_solution && r->core_n)) return VSDLSS_ERR_INVALID;
    if(!checked_count(r->n,sizeof(*next_x)) ||
       !checked_count(r->count,sizeof(*saved_copy)) ||
       !checked_count(r->core_n,sizeof(*core_copy))) return VSDLSS_ERR_OOM;
    if(r->n) next_x=(double *)calloc((size_t)r->n,sizeof(*next_x));
    if(r->count) saved_copy=(double *)malloc((size_t)r->count*sizeof(*saved_copy));
    if(r->core_n) core_copy=(double *)malloc((size_t)r->core_n*sizeof(*core_copy));
    if((r->n&&!next_x)||(r->count&&!saved_copy)||(r->core_n&&!core_copy)) {
        status=VSDLSS_ERR_OOM; goto done;
    }
    for(k=0;k<r->count;k++) {
        if(!isfinite(saved[k])) {status=VSDLSS_ERR_NONFINITE;goto done;}
        saved_copy[k]=saved[k];
    }
    for(k=0;k<r->core_n;k++) {
        csi vertex=r->core_vertices[k];
        if(vertex<0 || vertex>=r->n) {status=VSDLSS_ERR_INVALID;goto done;}
        if(!isfinite(core_solution[k])) {status=VSDLSS_ERR_NONFINITE;goto done;}
        core_copy[k]=core_solution[k]; next_x[vertex]=core_copy[k];
    }
    for(k=r->count;k>0;k--) {
        const vsdlss_elim_record *record=r->records+(k-1); double value;
        if(record->vertex<0 || record->vertex>=r->n || record->degree<0 ||
           record->degree>3) {status=VSDLSS_ERR_INVALID;goto done;}
        if(!isfinite(record->pivot)) {status=VSDLSS_ERR_NONFINITE;goto done;}
        if(record->pivot==0.0) {status=VSDLSS_ERR_INVALID;goto done;}
        value=saved_copy[k-1]/record->pivot;
        if(!isfinite(value)) {status=VSDLSS_ERR_NONFINITE;goto done;}
        for(j=0;j<record->degree;j++) {
            csi neighbor=record->neighbor[j]; double update;
            if(neighbor<0 || neighbor>=r->n) {status=VSDLSS_ERR_INVALID;goto done;}
            update=record->multiplier[j]*next_x[neighbor];
            if(!isfinite(record->multiplier[j]) || !isfinite(update) ||
               !isfinite(value-update)) {status=VSDLSS_ERR_NONFINITE;goto done;}
            value-=update;
        }
        next_x[record->vertex]=value;
    }
    if(r->n) memcpy(x,next_x,(size_t)r->n*sizeof(*x));
done:
    free(next_x); free(saved_copy); free(core_copy); return status;
}

vsdlss_status vsdlss_reduce(const vsdlss *A, vsdlss_reduction **out)
{
    vsdlss_reduction *r=NULL; numeric_list *adj=NULL; double *diag=NULL;
    unsigned char *active=NULL; csi *local=NULL; vsdlss_status status;
    edge_arena arena={0}; min_bitset bucket[4]; int buckets=0;
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
    if ((uint64_t)n > SIZE_MAX / sizeof(*r->records)) {status=VSDLSS_ERR_OOM;goto fail;}
    r->records=(vsdlss_elim_record *)calloc((size_t)n,sizeof(*r->records));
    if(!r->records) { status=VSDLSS_ERR_OOM; goto fail; }

    /* Symmetric adjacency in one pass: count, place (rows i<v of column v come
     * first, then columns j>v in increasing order, so lists are sorted), and
     * only if the input had unsorted or repeated rows sort and merge. */
    for(col=0;col<n;col++) for(k=A->p[col];k<A->p[col+1];k++) {
        csi row=A->i[k];
        if(row==col) { diag[col]+=A->x[k]; if(!isfinite(diag[col])) {status=VSDLSS_ERR_NONFINITE;goto fail;} }
        else { adj[row].capacity++; adj[col].capacity++; }
    }
    for(v=0;v<n;v++) {
        if(arena.used>INT64_MAX-adj[v].capacity) {status=VSDLSS_ERR_OOM;goto fail;}
        adj[v].off=arena.used; arena.used+=adj[v].capacity;
    }
    arena.capacity=arena.used+arena.used/4+16;
    if(!checked_count(arena.capacity,sizeof(numeric_edge))) {status=VSDLSS_ERR_OOM;goto fail;}
    arena.edge=(numeric_edge *)malloc((size_t)arena.capacity*sizeof(numeric_edge));
    if(!arena.edge) {status=VSDLSS_ERR_OOM;goto fail;}
    for(col=0;col<n;col++) for(k=A->p[col];k<A->p[col+1];k++) {
        csi row=A->i[k];
        if(row==col) continue;
        arena.edge[adj[col].off+adj[col].count++]=(numeric_edge){row,A->x[k]};
    }
    for(col=0;col<n;col++) for(k=A->p[col];k<A->p[col+1];k++) {
        csi row=A->i[k];
        if(row==col) continue;
        arena.edge[adj[row].off+adj[row].count++]=(numeric_edge){col,A->x[k]};
    }
    for(v=0;v<n;v++) {
        numeric_edge *e=arena.edge+adj[v].off; csi c=adj[v].count, u=0, j;
        int sorted=1;
        for(j=1;j<c;j++) if(e[j].vertex<=e[j-1].vertex) { sorted=0; break; }
        if(sorted) continue;
        qsort(e,(size_t)c,sizeof(*e),cmp_edge);
        for(j=0;j<c;j++) {
            if(u && e[u-1].vertex==e[j].vertex) {
                e[u-1].value+=e[j].value;
                if(!isfinite(e[u-1].value)) {status=VSDLSS_ERR_NONFINITE;goto fail;}
            } else e[u++]=e[j];
        }
        adj[v].count=u;
    }

    for(buckets=0;buckets<4;buckets++) {
        status=bs_init(&bucket[buckets],n);
        if(status!=VSDLSS_OK) goto fail;
    }
    for(v=0;v<n;v++) if(adj[v].count<=3) bs_set(&bucket[adj[v].count],v);
    remaining=n;
    for(;;) {
        numeric_list *list; numeric_edge *e; vsdlss_elim_record *record; csi a,b,d,old[3];
        for(d=0;d<4;d++) { v=bs_min(&bucket[d]); if(v>=0) break; }
        if(d==4) break;
        bs_clear(&bucket[d],v);
        if(!isfinite(diag[v])) {status=VSDLSS_ERR_NONFINITE;goto fail;}
        if(diag[v]<=0.0) {status=VSDLSS_ERR_NOT_POSDEF;goto fail;}
        list=adj+v; record=r->records+r->count; record->vertex=v;
        record->degree=list->count; record->pivot=diag[v];
        e=arena.edge+list->off;
        for(a=0;a<list->count;a++) {
            csi w=e[a].vertex; double avw=e[a].value;
            old[a]=adj[w].count;
            record->neighbor[a]=w; record->multiplier[a]=avw/diag[v];
            if(!isfinite(record->multiplier[a])) {status=VSDLSS_ERR_NONFINITE;goto fail;}
            diag[w]-=record->multiplier[a]*avw;
            if(!isfinite(diag[w])) {status=VSDLSS_ERR_NONFINITE;goto fail;}
        }
        for(a=0;a<record->degree;a++) for(b=a+1;b<record->degree;b++) {
            /* v's own list is not modified here, but the arena may move. */
            double vb=arena.edge[adj[v].off+b].value;
            double updated=-record->multiplier[a]*vb;
            if(!isfinite(updated)) {status=VSDLSS_ERR_NONFINITE;goto fail;}
            status=edge_set(&arena,adj,record->neighbor[a],record->neighbor[b],updated);
            if(status!=VSDLSS_OK) goto fail;
        }
        for(a=0;a<record->degree;a++) edge_remove(&arena,adj+record->neighbor[a],v);
        active[v]=0; r->count++; remaining--;
        for(a=0;a<record->degree;a++) {
            csi w=record->neighbor[a], now=adj[w].count;
            if(now==old[a]) continue;
            if(old[a]<=3) bs_clear(&bucket[old[a]],w);
            if(now<=3) bs_set(&bucket[now],w);
        }
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
            csi original=r->core_vertices[col], j;
            const numeric_edge *e=arena.edge+adj[original].off;
            for(j=0;j<adj[original].count;j++) {
                csi neighbor=e[j].vertex;
                if(neighbor>=original) continue;
                r->core->i[at]=local[neighbor]; r->core->x[at++]=e[j].value;
            }
            r->core->i[at]=col; r->core->x[at++]=diag[original]; r->core->p[col+1]=at;
        }
    }
    free(arena.edge); free(adj); free(diag); free(active); free(local);
    for(k=0;k<buckets;k++) bs_free(&bucket[k]);
    *out=r; return VSDLSS_OK;
fail:
    free(arena.edge); free(adj); free(diag); free(active); free(local);
    for(k=0;k<buckets;k++) bs_free(&bucket[k]);
    vsdlss_reduction_free(r); return status;
}
