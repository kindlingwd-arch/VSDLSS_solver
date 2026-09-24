#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { csi vertex; double value; } numeric_edge;

/* Adjacency lists are addressed by (arena, offset), so growth never
 * invalidates other lists.  Arena 0 holds the initial lists and never grows;
 * a list that outgrows its slot moves to the calling phase's growth arena
 * (one per parallel block, one for the sequential phase), so concurrent
 * blocks never reallocate shared storage.  Lists are kept sorted by vertex. */
typedef struct { int64_t slot; int32_t count, capacity; } numeric_list;  /* 16 bytes */
/* slot = arena id in the top 16 bits, offset in the low 48 bits. */
#define SLOT_OFF(s)   ((csi)((s) & ((INT64_C(1)<<48)-1)))
#define SLOT_ARENA(s) ((csi)((uint64_t)(s) >> 48))
#define MAKE_SLOT(a,o) ((int64_t)(((uint64_t)(a) << 48) | (uint64_t)(o)))
#define MAX_ARENAS 65535
#define MAX_LIST INT32_MAX
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

static numeric_edge *list_edges(edge_arena *ar, const numeric_list *l)
{ return ar[SLOT_ARENA(l->slot)].edge+SLOT_OFF(l->slot); }

static vsdlss_status list_reserve(edge_arena *ar, csi grow_id, numeric_list *list, csi needed)
{
    edge_arena *g=ar+grow_id; csi capacity, need_arena;
    if(needed<=list->capacity) return VSDLSS_OK;
    capacity=list->capacity?list->capacity:4;
    while(capacity<needed) { if(capacity>MAX_LIST/2) return VSDLSS_ERR_OOM; capacity*=2; }
    if(g->used>(INT64_C(1)<<48)-1-capacity) return VSDLSS_ERR_OOM;
    need_arena=g->used+capacity;
    if(need_arena>g->capacity) {
        csi grow=g->capacity?g->capacity:64; numeric_edge *edge;
        while(grow<need_arena) { if(grow>INT64_MAX/2) return VSDLSS_ERR_OOM; grow*=2; }
        if(!checked_count(grow,sizeof(*edge))) return VSDLSS_ERR_OOM;
        edge=(numeric_edge *)realloc(g->edge,(size_t)grow*sizeof(*edge));
        if(!edge) return VSDLSS_ERR_OOM;
        g->edge=edge; g->capacity=grow;
    }
    if(list->count)
        memcpy(g->edge+g->used,list_edges(ar,list),(size_t)list->count*sizeof(numeric_edge));
    list->slot=MAKE_SLOT(grow_id,g->used); list->capacity=(int32_t)capacity; g->used+=capacity;
    return VSDLSS_OK;
}

static vsdlss_status edge_set(edge_arena *ar, csi grow_id, numeric_list *adj,
                              csi a, csi b, double value)
{
    numeric_edge *ea=list_edges(ar,adj+a), *eb;
    csi pa=edge_lower_bound(ea,adj[a].count,b), pb;
    if(pa<adj[a].count && ea[pa].vertex==b) {
        double updated=ea[pa].value+value;
        eb=list_edges(ar,adj+b); pb=edge_lower_bound(eb,adj[b].count,a);
        if(!isfinite(updated)) return VSDLSS_ERR_NONFINITE;
        ea[pa].value=eb[pb].value=updated; return VSDLSS_OK;
    }
    if(adj[a].count>=MAX_LIST-1 || adj[b].count>=MAX_LIST-1) return VSDLSS_ERR_OOM;
    if(list_reserve(ar,grow_id,adj+a,adj[a].count+1)!=VSDLSS_OK ||
       list_reserve(ar,grow_id,adj+b,adj[b].count+1)!=VSDLSS_OK) return VSDLSS_ERR_OOM;
    ea=list_edges(ar,adj+a); eb=list_edges(ar,adj+b);
    pb=edge_lower_bound(eb,adj[b].count,a);
    memmove(ea+pa+1,ea+pa,(size_t)(adj[a].count-pa)*sizeof(*ea));
    ea[pa].vertex=b; ea[pa].value=value; adj[a].count++;
    memmove(eb+pb+1,eb+pb,(size_t)(adj[b].count-pb)*sizeof(*eb));
    eb[pb].vertex=a; eb[pb].value=value; adj[b].count++;
    return VSDLSS_OK;
}

static void edge_remove(edge_arena *ar, numeric_list *list, csi vertex)
{
    numeric_edge *e=list_edges(ar,list);
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
    free(reduction->records); free(reduction->core_vertices); free(reduction->block_ptr);
    if(reduction->pk) for(csi q=0;q<reduction->pk_count;q++) {
        free(reduction->pk[q].head); free(reduction->pk[q].nb); free(reduction->pk[q].val);
    }
    free(reduction->pk);
    vsdlss_spfree(reduction->core); free(reduction);
}

static int valid_reduction_shape(const vsdlss_reduction *r)
{
    return r && r->n>=0 && r->count>=0 && r->core_n>=0 &&
           r->count<=r->n && r->core_n==r->n-r->count &&
           (!r->count || r->records || (r->pk && r->pk_count>0)) &&
           (!r->core_n || r->core_vertices);
}

#define PK_VMASK 0x3fffffffu

/* Sequential access to record k in struct form: direct for unpacked
 * reductions, otherwise decoded through a cursor over the packed segments
 * (next: increasing k from a zeroed cursor; prev: decreasing k from
 * pk_cursor_end). */
typedef struct { csi s, i, o; } pk_cursor;

static void decode(const vsdlss_pk_seg *g, csi i, csi o, vsdlss_elim_record *t)
{
    uint32_t h=g->head[i]; csi d=(csi)(h>>30);
    t->vertex=(csi)(h&PK_VMASK); t->degree=d; t->pivot=g->val[i+o];
    for(csi j=0;j<d;j++) { t->neighbor[j]=g->nb[o+j]; t->multiplier[j]=g->val[i+o+1+j]; }
}

static const vsdlss_elim_record *next_record(const vsdlss_reduction *r, csi k, pk_cursor *c,
                                             vsdlss_elim_record *t)
{
    if(r->records) return r->records+k;
    while(c->i>=r->pk[c->s].count) { c->s++; c->i=0; c->o=0; }
    decode(r->pk+c->s,c->i,c->o,t);
    c->o+=t->degree; c->i++;
    return t;
}

static pk_cursor pk_cursor_end(const vsdlss_reduction *r)
{
    pk_cursor c={0,0,0};
    if(!r->records && r->pk_count) { c.s=r->pk_count-1; c.i=r->pk[c.s].count; c.o=r->pk[c.s].nbn; }
    return c;
}

static const vsdlss_elim_record *prev_record(const vsdlss_reduction *r, csi k, pk_cursor *c,
                                             vsdlss_elim_record *t)
{
    const vsdlss_pk_seg *g;
    if(r->records) return r->records+k;
    while(c->i==0) { c->s--; c->i=r->pk[c->s].count; c->o=r->pk[c->s].nbn; }
    g=r->pk+c->s; c->i--; c->o-=(csi)(g->head[c->i]>>30);
    decode(g,c->i,c->o,t);
    return t;
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
    pk_cursor cur={0,0,0}; vsdlss_elim_record tmp;
    for(k=0;k<r->count;k++) {
        const vsdlss_elim_record *record=next_record(r,k,&cur,&tmp);
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
    pk_cursor cur=pk_cursor_end(r); vsdlss_elim_record tmp;
    for(k=r->count;k>0;k--) {
        const vsdlss_elim_record *record=prev_record(r,k-1,&cur,&tmp); double value;
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

/* One forward step (record k) with the arithmetic of vsdlss_reduce_rhs. */
static int forward_record(const vsdlss_reduction *r, csi k, double *work, double *saved)
{
    const vsdlss_elim_record *record=r->records+k; double s; csi j;
    if(record->vertex<0 || record->vertex>=r->n || record->degree<0 ||
       record->degree>3) return 1;
    s=saved[k]=work[record->vertex];
    for(j=0;j<record->degree;j++) {
        csi neighbor=record->neighbor[j]; double update;
        if(neighbor<0 || neighbor>=r->n) return 1;
        update=record->multiplier[j]*s;
        if(!isfinite(record->multiplier[j]) || !isfinite(update) ||
           !isfinite(work[neighbor]-update)) return 2;
        work[neighbor]-=update;
    }
    return 0;
}

/* One backward step with the arithmetic of vsdlss_reduce_recover. */
static int backward_record(const vsdlss_reduction *r, csi k, const double *saved, double *x)
{
    const vsdlss_elim_record *record=r->records+k; double value; csi j;
    if(record->vertex<0 || record->vertex>=r->n || record->degree<0 ||
       record->degree>3) return 1;
    if(!isfinite(saved[k]) || !isfinite(record->pivot)) return 2;
    if(record->pivot==0.0) return 1;
    value=saved[k]/record->pivot;
    if(!isfinite(value)) return 2;
    for(j=0;j<record->degree;j++) {
        csi neighbor=record->neighbor[j]; double update;
        if(neighbor<0 || neighbor>=r->n) return 1;
        update=record->multiplier[j]*x[neighbor];
        if(!isfinite(record->multiplier[j]) || !isfinite(update) ||
           !isfinite(value-update)) return 2;
        value-=update;
    }
    x[record->vertex]=value;
    return 0;
}

static int blocks_valid(const vsdlss_reduction *r)
{
    if(r->blocks<1 || !r->block_ptr || r->block_ptr[0]!=0 || r->block_ptr[r->blocks]>r->count) return 0;
    for(csi b=0;b<r->blocks;b++) if(r->block_ptr[b+1]<r->block_ptr[b]) return 0;
    return 1;
}

static vsdlss_status status_of(int bad)
{ return bad==0?VSDLSS_OK:(bad&1)?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE; }

/* Packed replay of one segment (saved points at the segment's first
 * record).  Same operations, in the same order, as forward_record /
 * backward_record. */
static void forward_seg(const vsdlss_pk_seg *g, double *restrict work, double *restrict saved)
{
    const uint32_t *restrict head=g->head, *restrict nb=g->nb;
    const double *restrict val=g->val;
    csi o=0;
    for(csi i=0;i<g->count;i++) {
        uint32_t h=head[i]; csi d=(csi)(h>>30);
        const double *m=val+i+o+1; const uint32_t *w=nb+o;
        double s=work[h&PK_VMASK];
        if(saved) saved[i]=s;
        for(csi j=0;j<d;j++) { double update=m[j]*s; work[w[j]]-=update; }
        o+=d;
    }
}

/* Returns 0 when a recovered value is not finite. */
static int backward_seg(const vsdlss_pk_seg *g, const double *restrict saved, double *restrict x)
{
    const uint32_t *restrict head=g->head, *restrict nb=g->nb;
    const double *restrict val=g->val;
    csi o=g->nbn; int ok=1;
    for(csi i=g->count;i>0;) {
        i--;
        uint32_t h=head[i]; csi d=(csi)(h>>30);
        o-=d;
        const double *v=val+i+o; const uint32_t *w=nb+o;
        double value=(saved?saved[i]:x[h&PK_VMASK])/v[0];
        for(csi j=0;j<d;j++) { double update=v[1+j]*x[w[j]]; value-=update; }
        ok&=isfinite(value)!=0;
        x[h&PK_VMASK]=value;
    }
    return ok;
}

/* Blocked packed layout: one segment per block plus the sequential tail. */
static int pk_blocked(const vsdlss_reduction *r)
{ return r->blocks>=1 && r->pk_count==r->blocks+1 && blocks_valid(r); }

/* In-place forms used by the M3 solve, whose buffers are private: work
 * holds the component RHS and is updated exactly as vsdlss_reduce_rhs updates
 * its internal copy; afterwards work[core_vertices[k]] is the core RHS.
 * Records of the parallel block pass touch disjoint vertices per block, so
 * those blocks are replayed concurrently; every entry sees the same
 * operations as in the serial replay, for any thread count.
 * saved may be NULL for a packed reduction: a record's saved value is then
 * left in work at its vertex, which no later record updates and the core
 * write-back does not touch, and the backward replay reads it from there. */
#define SV(saved,g) ((saved)?(saved)+(g).k0:NULL)
vsdlss_status vsdlss_reduce_forward_inplace(const vsdlss_reduction *r, double *work,
                                            double *saved)
{
    csi k, first=0; int bad=0, nt;
    if(!valid_reduction_shape(r) || (!work && r->n) || (!saved && r->count && r->records)) return VSDLSS_ERR_INVALID;
    nt=vsdlss_parallel_width((double)r->n*4);
    (void)nt;
    if(!r->records && r->count) {
        const vsdlss_pk_seg *pk=r->pk; csi q0=0;
        if(pk_blocked(r)) {
            int bt=nt; if(bt>r->blocks) bt=(int)r->blocks;
            (void)bt;
            VSDLSS_OMP(omp parallel for num_threads(bt) if(bt>1) schedule(dynamic,4))
            for(csi b=0;b<r->blocks;b++) forward_seg(pk+b,work,SV(saved,pk[b]));
            q0=r->blocks;
        }
        for(csi q=q0;q<r->pk_count;q++) forward_seg(pk+q,work,SV(saved,pk[q]));
        return VSDLSS_OK;
    }
    if(blocks_valid(r)) {
        int bt=nt; if(bt>r->blocks) bt=(int)r->blocks;
        (void)bt;
        VSDLSS_OMP(omp parallel for num_threads(bt) if(bt>1) schedule(dynamic,4) reduction(|:bad))
        for(csi b=0;b<r->blocks;b++)
            for(csi q=r->block_ptr[b];q<r->block_ptr[b+1]&&!bad;q++) bad|=forward_record(r,q,work,saved);
        if(bad) return status_of(bad);
        first=r->block_ptr[r->blocks];
    }
    for(k=first;k<r->count;k++) { bad=forward_record(r,k,work,saved); if(bad) return status_of(bad); }
    return VSDLSS_OK;
}

/* x holds the core solution at x[core_vertices[k]]; the eliminated entries
 * are recovered in reverse order with the arithmetic of reduce_recover. */
vsdlss_status vsdlss_reduce_backward_inplace(const vsdlss_reduction *r, const double *saved,
                                             double *x)
{
    csi k, stop=0; int bad=0, nt;
    if(!valid_reduction_shape(r) || (!x && r->n) || (!saved && r->count && r->records)) return VSDLSS_ERR_INVALID;
    for(k=0;k<r->core_n;k++) {
        csi vertex=r->core_vertices[k];
        if(vertex<0 || vertex>=r->n) return VSDLSS_ERR_INVALID;
        if(!isfinite(x[vertex])) return VSDLSS_ERR_NONFINITE;
    }
    nt=vsdlss_parallel_width((double)r->n*4);
    (void)nt;
    if(!r->records && r->count) {
        const vsdlss_pk_seg *pk=r->pk; int ok=1; csi q0=pk_blocked(r)?r->blocks:0;
        for(csi q=r->pk_count;q>q0;q--) ok&=backward_seg(pk+q-1,SV(saved,pk[q-1]),x);
        if(q0) {
            int bt=nt; if(bt>r->blocks) bt=(int)r->blocks;
            (void)bt;
            VSDLSS_OMP(omp parallel for num_threads(bt) if(bt>1) schedule(dynamic,4) reduction(&:ok))
            for(csi b=0;b<r->blocks;b++) ok&=backward_seg(pk+b,SV(saved,pk[b]),x);
        }
        return ok?VSDLSS_OK:VSDLSS_ERR_NONFINITE;
    }
    int blocked=blocks_valid(r);
    if(blocked) stop=r->block_ptr[r->blocks];
    for(k=r->count;k>stop;k--) { bad=backward_record(r,k-1,saved,x); if(bad) return status_of(bad); }
    if(blocked) {
        int bt=nt; if(bt>r->blocks) bt=(int)r->blocks;
        (void)bt;
        VSDLSS_OMP(omp parallel for num_threads(bt) if(bt>1) schedule(dynamic,4) reduction(|:bad))
        for(csi b=0;b<r->blocks;b++)
            for(csi q=r->block_ptr[b+1];q>r->block_ptr[b]&&!bad;q--) bad|=backward_record(r,q-1,saved,x);
        if(bad) return status_of(bad);
    }
    return VSDLSS_OK;
}

static int record_valid(const vsdlss_reduction *r, const vsdlss_elim_record *e)
{
    if(e->vertex<0 || e->vertex>=r->n || e->degree<0 || e->degree>3) return 0;
    if(!isfinite(e->pivot) || e->pivot==0.0) return 0;
    for(csi j=0;j<e->degree;j++)
        if(e->neighbor[j]<0 || e->neighbor[j]>=r->n || !isfinite(e->multiplier[j])) return 0;
    return 1;
}

/* Packs rec[0..cnt) into *g (first record index k0).  INVALID when a
 * record fails validation. */
static vsdlss_status pack_segment(const vsdlss_reduction *r, const vsdlss_elim_record *rec,
                                  csi cnt, csi k0, vsdlss_pk_seg *g)
{
    csi nbn=0;
    memset(g,0,sizeof(*g)); g->k0=k0;
    for(csi i=0;i<cnt;i++) { if(!record_valid(r,rec+i)) return VSDLSS_ERR_INVALID; nbn+=rec[i].degree; }
    if(!cnt) return VSDLSS_OK;
    uint32_t *head=(uint32_t *)malloc((size_t)cnt*sizeof(uint32_t));
    uint32_t *nb=(uint32_t *)malloc((size_t)(nbn?nbn:1)*sizeof(uint32_t));
    double *val=(double *)malloc((size_t)(cnt+nbn)*sizeof(double));
    if(!head||!nb||!val) { free(head); free(nb); free(val); return VSDLSS_ERR_OOM; }
    for(csi i=0,o=0;i<cnt;i++) {
        const vsdlss_elim_record *e=rec+i;
        head[i]=(uint32_t)e->vertex|((uint32_t)e->degree<<30);
        val[i+o]=e->pivot;
        for(csi j=0;j<e->degree;j++) { nb[o+j]=(uint32_t)e->neighbor[j]; val[i+o+1+j]=e->multiplier[j]; }
        o+=e->degree;
    }
    g->count=cnt; g->nbn=nbn; g->head=head; g->nb=nb; g->val=val;
    return VSDLSS_OK;
}

static void free_segments(vsdlss_reduction *r)
{
    if(r->pk) for(csi q=0;q<r->pk_count;q++) { free(r->pk[q].head); free(r->pk[q].nb); free(r->pk[q].val); }
    free(r->pk); r->pk=NULL; r->pk_count=0;
}

vsdlss_status vsdlss_reduce_pack(vsdlss_reduction *r)
{
    if(!r) return VSDLSS_ERR_INVALID;
    if(!r->records || !r->count || r->n>(csi)PK_VMASK+1) return VSDLSS_OK;
    const int blocked=blocks_valid(r);
    const csi segs=blocked?r->blocks+1:1;       /* last segment: sequential tail */
    vsdlss_status st=VSDLSS_OK;
    r->pk=(vsdlss_pk_seg *)calloc((size_t)segs,sizeof(*r->pk));
    if(!r->pk) return VSDLSS_ERR_OOM;
    r->pk_count=segs;
    int nt=vsdlss_parallel_width((double)r->count*8); (void)nt;
    int bad=0;       /* bit 0: invalid, bit 1: out of memory */
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(dynamic,8) reduction(|:bad))
    for(csi q=0;q<segs;q++) {
        csi lo=blocked?r->block_ptr[q]:0;
        csi hi=blocked?(q<r->blocks?r->block_ptr[q+1]:r->count):r->count;
        vsdlss_status e=pack_segment(r,r->records+lo,hi-lo,lo,r->pk+q);
        if(e==VSDLSS_ERR_INVALID) bad|=1; else if(e!=VSDLSS_OK) bad|=2;
    }
    if(bad) { free_segments(r); st=(bad&2)?VSDLSS_ERR_OOM:VSDLSS_OK; return st; }
    free(r->records); r->records=NULL;
    return VSDLSS_OK;
}

/* Shared state of one reduction. */
typedef struct {
    numeric_list *adj; double *diag; unsigned char *active;
    edge_arena *ar; vsdlss_elim_record *records;
} reduce_state;

/* Eliminate v (degree <= 3) into *record; grow lists into arena grow_id.
 * old[] receives the neighbours' degrees before the update. */
static vsdlss_status eliminate(reduce_state *z, csi grow_id, csi v,
                               vsdlss_elim_record *record, csi *old)
{
    numeric_list *adj=z->adj; double *diag=z->diag; csi a,b;
    const numeric_edge *e;
    if(!isfinite(diag[v])) return VSDLSS_ERR_NONFINITE;
    if(diag[v]<=0.0) return VSDLSS_ERR_NOT_POSDEF;
    record->vertex=v; record->degree=adj[v].count; record->pivot=diag[v];
    e=list_edges(z->ar,adj+v);
    for(a=0;a<record->degree;a++) {
        csi w=e[a].vertex; double avw=e[a].value;
        old[a]=adj[w].count;
        record->neighbor[a]=w; record->multiplier[a]=avw/diag[v];
        if(!isfinite(record->multiplier[a])) return VSDLSS_ERR_NONFINITE;
        diag[w]-=record->multiplier[a]*avw;
        if(!isfinite(diag[w])) return VSDLSS_ERR_NONFINITE;
    }
    for(a=0;a<record->degree;a++) for(b=a+1;b<record->degree;b++) {
        /* v's own list is not modified here, but arenas may move. */
        double vb=list_edges(z->ar,adj+v)[b].value, updated=-record->multiplier[a]*vb;
        vsdlss_status st;
        if(!isfinite(updated)) return VSDLSS_ERR_NONFINITE;
        st=edge_set(z->ar,grow_id,adj,record->neighbor[a],record->neighbor[b],updated);
        if(st!=VSDLSS_OK) return st;
    }
    for(a=0;a<record->degree;a++) edge_remove(z->ar,adj+record->neighbor[a],v);
    z->active[v]=0;
    return VSDLSS_OK;
}

/* Minimum-degree elimination of vertices of degree <= 3 inside [lo,hi):
 * smallest current degree first, ties to the smallest index.  With
 * block != 0 only vertices whose neighbours all lie inside the range are
 * eliminated (the others are left for the sequential pass), so disjoint
 * ranges touch disjoint vertices and can run concurrently.  Records are
 * written to z->records[rec0 ..]; *count receives how many. */
static vsdlss_status eliminate_range(reduce_state *z, csi grow_id, csi lo, csi hi,
                                     int block, csi rec0, csi *count)
{
    min_bitset bucket[4]; int made=0; vsdlss_status st=VSDLSS_OK; csi v, done=0;
    for(made=0;made<4;made++) { st=bs_init(&bucket[made],hi-lo); if(st!=VSDLSS_OK) goto out; }
    for(v=lo;v<hi;v++) if(z->active[v]&&z->adj[v].count<=3) bs_set(&bucket[z->adj[v].count],v-lo);
    for(;;) {
        csi d, old[3], k; vsdlss_elim_record *record;
        for(d=0;d<4;d++) { v=bs_min(&bucket[d]); if(v>=0) break; }
        if(d==4) break;
        bs_clear(&bucket[d],v); v+=lo;
        if(block) {
            const numeric_edge *e=list_edges(z->ar,z->adj+v); int inside=1;
            for(k=0;k<z->adj[v].count;k++) if(e[k].vertex<lo||e[k].vertex>=hi) { inside=0; break; }
            if(!inside) continue;   /* stays for the sequential pass */
        }
        record=z->records+rec0+done;
        st=eliminate(z,grow_id,v,record,old);
        if(st!=VSDLSS_OK) goto out;
        done++;
        for(k=0;k<record->degree;k++) {
            csi w=record->neighbor[k], now=z->adj[w].count;
            if(now==old[k]) continue;
            if(old[k]<=3) bs_clear(&bucket[old[k]],w-lo);
            if(now<=3) bs_set(&bucket[now],w-lo);
        }
    }
out:
    for(int i=0;i<made;i++) bs_free(&bucket[i]);
    *count=done;
    return st;
}

/* Blocks for the parallel pass.  The partition depends only on n, never on
 * the thread count, so the records and every floating-point result are the
 * same for 1..N threads. */
csi vsdlss_reduce_block = (csi)1<<16;
#define REDUCE_BLOCK vsdlss_reduce_block

/* Prepared input of the elimination: sorted symmetric adjacency in arena 0
 * plus the diagonal.  Built from an upper CSC matrix or, without any CSC
 * intermediate, from a component of a weighted graph in a new numbering. */
struct vsdlss_reduce_input {
    csi n;
    numeric_list *adj;
    edge_arena base;
    double *diag;
};

void vsdlss_reduce_input_free(vsdlss_reduce_input *in)
{
    if(!in) return;
    free(in->adj); free(in->base.edge); free(in->diag); free(in);
}

static vsdlss_status input_alloc(csi n, vsdlss_reduce_input **out)
{
    vsdlss_reduce_input *in;
    *out=NULL;
    if(n<1) return VSDLSS_ERR_INVALID;
    if(n==INT64_MAX || !checked_count(n,sizeof(numeric_list)) ||
       !checked_count(n,sizeof(double)) || !checked_count(n+1,sizeof(csi)))
        return VSDLSS_ERR_OOM;
    in=(vsdlss_reduce_input *)calloc(1,sizeof(*in));
    if(!in) return VSDLSS_ERR_OOM;
    in->n=n;
    in->adj=(numeric_list *)vsdlss_big_calloc((size_t)n,sizeof(numeric_list));
    in->diag=(double *)vsdlss_big_calloc((size_t)n,sizeof(double));
    if(!in->adj||!in->diag) { vsdlss_reduce_input_free(in); return VSDLSS_ERR_OOM; }
    *out=in; return VSDLSS_OK;
}

/* Place each vertex's list at a prefix-sum offset of its capacity. */
static vsdlss_status input_place(vsdlss_reduce_input *in)
{
    csi v, used=0;
    for(v=0;v<in->n;v++) {
        if(used>(INT64_C(1)<<48)-1-in->adj[v].capacity) return VSDLSS_ERR_OOM;
        in->adj[v].slot=MAKE_SLOT(0,used); used+=in->adj[v].capacity;
    }
    in->base.used=used; in->base.capacity=used>0?used:1;
    if(!checked_count(in->base.capacity,sizeof(numeric_edge))) return VSDLSS_ERR_OOM;
    in->base.edge=(numeric_edge *)vsdlss_big_malloc((size_t)in->base.capacity*sizeof(numeric_edge));
    return in->base.edge?VSDLSS_OK:VSDLSS_ERR_OOM;
}

vsdlss_status vsdlss_reduce_prepare_csc(const vsdlss *A, vsdlss_reduce_input **out)
{
    vsdlss_reduce_input *in=NULL; numeric_list *adj; double *diag;
    csi n,col,k,v; vsdlss_status status;
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(!A || A->n<1) return VSDLSS_ERR_INVALID;
    if(A->n==INT64_MAX || !checked_count(A->n,sizeof(numeric_list)) ||
       !checked_count(A->n+1,sizeof(csi))) return VSDLSS_ERR_OOM;
    status=vsdlss_validate_upper_csc(A); if(status!=VSDLSS_OK) return status;
    status=input_alloc(A->n,&in); if(status!=VSDLSS_OK) return status;
    n=A->n; adj=in->adj; diag=in->diag;
    /* Symmetric adjacency in one pass: count, place (rows i<v of column v come
     * first, then columns j>v in increasing order, so lists are sorted), and
     * only if the input had unsorted or repeated rows sort and merge. */
    for(col=0;col<n;col++) for(k=A->p[col];k<A->p[col+1];k++) {
        csi row=A->i[k];
        if(row==col) { diag[col]+=A->x[k]; if(!isfinite(diag[col])) {status=VSDLSS_ERR_NONFINITE;goto fail;} }
        else {
            if(adj[row].capacity>=MAX_LIST-1 || adj[col].capacity>=MAX_LIST-1) {status=VSDLSS_ERR_OOM;goto fail;}
            adj[row].capacity++; adj[col].capacity++;
        }
    }
    status=input_place(in); if(status!=VSDLSS_OK) goto fail;
    for(col=0;col<n;col++) for(k=A->p[col];k<A->p[col+1];k++) {
        csi row=A->i[k];
        if(row==col) continue;
        in->base.edge[SLOT_OFF(adj[col].slot)+adj[col].count++]=(numeric_edge){row,A->x[k]};
    }
    for(col=0;col<n;col++) for(k=A->p[col];k<A->p[col+1];k++) {
        csi row=A->i[k];
        if(row==col) continue;
        in->base.edge[SLOT_OFF(adj[row].slot)+adj[row].count++]=(numeric_edge){col,A->x[k]};
    }
    for(v=0;v<n;v++) {
        numeric_edge *e=in->base.edge+SLOT_OFF(adj[v].slot); csi c=adj[v].count, u=0, j;
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
        adj[v].count=(int32_t)u;
    }
    *out=in; return VSDLSS_OK;
fail:
    vsdlss_reduce_input_free(in); return status;
}

vsdlss_status vsdlss_reduce_prepare_graph(const csi *ptr, const csi *idx, const double *val,
                                          const double *diag, const csi *map, csi n,
                                          const csi *newidx, vsdlss_reduce_input **out)
{
    vsdlss_reduce_input *in=NULL; vsdlss_status status; int bad=0, nt;
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    if(!ptr||!idx||!val||!diag||!map||!newidx) return VSDLSS_ERR_INVALID;
    status=input_alloc(n,&in); if(status!=VSDLSS_OK) return status;
    for(csi k=0;k<n;k++) {
        csi d=ptr[map[k]+1]-ptr[map[k]];
        if(d<0||d>=MAX_LIST-1) { vsdlss_reduce_input_free(in); return VSDLSS_ERR_INVALID; }
        in->adj[k].capacity=(int32_t)d;
    }
    status=input_place(in);
    if(status!=VSDLSS_OK) { vsdlss_reduce_input_free(in); return status; }
    /* Each new vertex writes only its own slot: parallel and deterministic.
     * Lists are then sorted by new index (short lists: insertion sort). */
    nt=vsdlss_parallel_width((double)n*16);
    (void)nt;
    VSDLSS_OMP(omp parallel for num_threads(nt) if(nt>1) schedule(static) reduction(|:bad))
    for(csi k=0;k<n;k++) {
        csi g=map[k], c=0, j;
        numeric_edge *e=in->base.edge+SLOT_OFF(in->adj[k].slot);
        in->diag[k]=diag[g];
        if(!isfinite(diag[g])) bad|=2;
        for(csi p=ptr[g];p<ptr[g+1];p++) {
            csi u=newidx[idx[p]];
            if(u<0||u>=n||u==k) { bad|=1; continue; }
            numeric_edge x={u,val[p]};
            for(j=c;j>0&&e[j-1].vertex>u;j--) e[j]=e[j-1];
            e[j]=x; c++;
        }
        for(j=1;j<c;j++) if(e[j].vertex==e[j-1].vertex) bad|=1;
        in->adj[k].count=(int32_t)c;
    }
    if(bad) { vsdlss_reduce_input_free(in); return (bad&1)?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE; }
    *out=in; return VSDLSS_OK;
}

static vsdlss_status reduce_run_impl(vsdlss_reduce_input *in, vsdlss_reduction **out, int pack,
                                     vsdlss_reduce_ws *ws);
vsdlss_status vsdlss_reduce_run(vsdlss_reduce_input *in, vsdlss_reduction **out)
{ return reduce_run_impl(in,out,0,NULL); }
vsdlss_status vsdlss_reduce_run_packed(vsdlss_reduce_input *in, vsdlss_reduction **out,
                                       vsdlss_reduce_ws *ws)
{ return reduce_run_impl(in,out,1,ws); }

/* pack: blocked reductions build the packed segments straight from each
 * block's records (a per-thread scratch of one block) and from the tail, so
 * the n-record array is never allocated; the result equals vsdlss_reduce_run
 * followed by vsdlss_reduce_pack. */
static vsdlss_status reduce_run_impl(vsdlss_reduce_input *in, vsdlss_reduction **out, int pack,
                                     vsdlss_reduce_ws *ws)
{
    vsdlss_reduction *r=NULL; numeric_list *adj; double *diag;
    unsigned char *active=NULL; csi *local=NULL; vsdlss_status status=VSDLSS_OK;
    edge_arena *ar=NULL; csi blocks=0, narenas=0;
    csi *made=NULL; vsdlss_elim_record *tail=NULL; int direct=0;
    csi n,col,k,v,core_n,nnz,at;
    if(ws) memset(ws,0,sizeof(*ws));
    if(!out) { vsdlss_reduce_input_free(in); return VSDLSS_ERR_INVALID; }
    *out=NULL;
    if(!in) return VSDLSS_ERR_INVALID;
    n=in->n; adj=in->adj; diag=in->diag;
    blocks=n>=2*REDUCE_BLOCK ? (n+REDUCE_BLOCK-1)/REDUCE_BLOCK : 0;
    if(blocks+2>MAX_ARENAS) { status=VSDLSS_ERR_OOM; goto fail; }   /* > 4e9 vertices */
    narenas=blocks+2;      /* 0 = initial lists, 1..blocks, blocks+1 = sequential */
    direct=pack && blocks && n<=(csi)PK_VMASK+1;
    r=(vsdlss_reduction *)calloc(1,sizeof(*r));
    active=(unsigned char *)vsdlss_big_malloc((size_t)n);
    ar=(edge_arena *)calloc((size_t)narenas,sizeof(*ar));
    if(!r||!active||!ar) { status=VSDLSS_ERR_OOM; goto fail; }
    ar[0]=in->base; in->base.edge=NULL;      /* arena 0 now owned here */
    r->n=n; memset(active,1,(size_t)n);
    if ((uint64_t)n > SIZE_MAX / sizeof(*r->records)) {status=VSDLSS_ERR_OOM;goto fail;}
    if(direct) {
        r->pk=(vsdlss_pk_seg *)calloc((size_t)(blocks+1),sizeof(*r->pk));
        if(!r->pk) { status=VSDLSS_ERR_OOM; goto fail; }
        r->pk_count=blocks+1;
    } else {
        r->records=(vsdlss_elim_record *)vsdlss_big_malloc((size_t)n*sizeof(*r->records));
        if(!r->records) { status=VSDLSS_ERR_OOM; goto fail; }
    }
    r->block_ptr=(csi *)malloc((size_t)(blocks+1)*sizeof(csi));
    if(!r->block_ptr) { status=VSDLSS_ERR_OOM; goto fail; }
    r->blocks=blocks;

    {
        reduce_state z={adj,diag,active,ar,r->records};
        csi total=0;
        r->block_ptr[0]=0;
        if(blocks) {
            /* Parallel pass: block b eliminates only vertices whose
             * neighbours all lie in the block, so blocks touch disjoint
             * vertices (the solve replays them concurrently).  Records go to
             * records[b*BLOCK ..], compacted in block order afterwards, or
             * (direct) to a per-thread scratch packed into segment b. */
            vsdlss_status *res=(vsdlss_status *)calloc((size_t)blocks,sizeof(*res));
            int nt=vsdlss_parallel_width((double)n*64), scratch_failed=0;
            made=(csi *)calloc((size_t)blocks,sizeof(csi));
            if(!made||!res) { free(res); status=VSDLSS_ERR_OOM; goto fail; }
            if(nt>blocks) nt=(int)blocks;
            (void)nt;
            VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:scratch_failed))
            {
                reduce_state zl=z; vsdlss_elim_record *scratch=NULL;
                if(direct) {
                    scratch=(vsdlss_elim_record *)malloc((size_t)REDUCE_BLOCK*sizeof(*scratch));
                    if(!scratch) scratch_failed=1;
                    zl.records=scratch;
                }
                VSDLSS_OMP(omp for schedule(dynamic,1))
                for(csi bl=0;bl<blocks;bl++) {
                    csi lo=bl*REDUCE_BLOCK, hi=lo+REDUCE_BLOCK<n?lo+REDUCE_BLOCK:n;
                    if(direct && !scratch) { res[bl]=VSDLSS_ERR_OOM; continue; }
                    res[bl]=eliminate_range(&zl,bl+1,lo,hi,1,direct?0:lo,&made[bl]);
                    if(direct && res[bl]==VSDLSS_OK) res[bl]=pack_segment(r,scratch,made[bl],0,r->pk+bl);
                }
                free(scratch);
            }
            for(csi bl=0;bl<blocks;bl++) if(res[bl]!=VSDLSS_OK) { status=res[bl]; break; }
            free(res);
            if(status==VSDLSS_OK && scratch_failed) status=VSDLSS_ERR_OOM;
            if(status!=VSDLSS_OK) goto fail;
            for(csi bl=0;bl<blocks;bl++) {
                if(direct) r->pk[bl].k0=total;
                else if(total!=bl*REDUCE_BLOCK)
                    memmove(r->records+total,r->records+bl*REDUCE_BLOCK,(size_t)made[bl]*sizeof(*r->records));
                total+=made[bl];
                r->block_ptr[bl+1]=total;
            }
        }
        /* Sequential pass over everything that is left (direct: into a
         * scratch of the remaining size, packed as the last segment). */
        {
            csi more=0;
            if(direct) {
                tail=(vsdlss_elim_record *)vsdlss_big_malloc((size_t)(n-total>0?n-total:1)*sizeof(*tail));
                if(!tail) {status=VSDLSS_ERR_OOM;goto fail;}
                z.records=tail;
            }
            status=eliminate_range(&z,narenas-1,0,n,0,direct?0:total,&more);
            if(status!=VSDLSS_OK) goto fail;
            if(direct) {
                status=pack_segment(r,tail,more,total,r->pk+blocks);
                if(status!=VSDLSS_OK) goto fail;
                free(tail); tail=NULL;
            }
            total+=more;
        }
        r->count=total;
    }
    r->core_n=core_n=n-r->count;
    if(core_n) {
        r->core_vertices=(csi *)vsdlss_big_malloc((size_t)core_n*sizeof(csi));
        local=(csi *)vsdlss_big_malloc((size_t)n*sizeof(csi));
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
            const numeric_edge *e=list_edges(ar,adj+original);
            for(j=0;j<adj[original].count;j++) {
                csi neighbor=e[j].vertex;
                if(neighbor>=original) continue;
                r->core->i[at]=local[neighbor]; r->core->x[at++]=e[j].value;
            }
            r->core->i[at]=col; r->core->x[at++]=diag[original]; r->core->p[col+1]=at;
        }
    }
    /* Trim the record array to what was used. */
    if(r->records && r->count<n) {
        vsdlss_elim_record *t=(vsdlss_elim_record *)realloc(r->records,(size_t)(r->count?r->count:1)*sizeof(*r->records));
        if(!t) {status=VSDLSS_ERR_OOM;goto fail;}   /* keeps allocation failures uniform */
        r->records=t;
    }
    if(!r->count) { free(r->records); r->records=NULL; }
    if(ws) {
        /* Hand the solve its buffers from arrays this reduction already
         * touched (the diagonal, the list headers, the core index map):
         * fresh allocations would page-fault on the first solve. */
        ws->local=in->diag; in->diag=NULL;                         /* n doubles */
        if(r->count) {                              /* 16n bytes >= 8 count */
            double *t=(double *)realloc(in->adj,(size_t)r->count*sizeof(double));
            if(!t) { status=VSDLSS_ERR_OOM; goto ws_fail; }
            ws->saved=t; in->adj=NULL;
        }
        if(core_n) {                                 /* local: n csi >= core_n */
            double *t=(double *)realloc(local,(size_t)core_n*sizeof(double));
            if(!t) { status=VSDLSS_ERR_OOM; goto ws_fail; }
            ws->core=t; local=NULL;
        }
    }
    for(k=0;k<narenas;k++) free(ar[k].edge);
    free(ar); free(active); free(local); free(made); vsdlss_reduce_input_free(in);
    if(pack && !direct) {
        status=vsdlss_reduce_pack(r);
        if(status!=VSDLSS_OK) {
            vsdlss_reduction_free(r);
            if(ws) { free(ws->local); free(ws->saved); free(ws->core); memset(ws,0,sizeof(*ws)); }
            return status;
        }
    }
    if(ws && !r->records) { free(ws->saved); ws->saved=NULL; }   /* packed: replay keeps it in place */
    *out=r; return VSDLSS_OK;
ws_fail:
    free(ws->local); free(ws->saved); free(ws->core); memset(ws,0,sizeof(*ws));
fail:
    if(ar) for(k=0;k<narenas;k++) free(ar[k].edge);
    free(ar); free(active); free(local); free(made); free(tail); vsdlss_reduce_input_free(in);
    vsdlss_reduction_free(r); return status;
}

vsdlss_status vsdlss_reduce(const vsdlss *A, vsdlss_reduction **out)
{
    vsdlss_reduce_input *in=NULL; vsdlss_status st;
    if(!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    st=vsdlss_reduce_prepare_csc(A,&in);
    if(st!=VSDLSS_OK) return st;
    return vsdlss_reduce_run(in,out);
}

vsdlss_status vsdlss_reduce_consume(vsdlss **A, vsdlss_reduction **out)
{
    vsdlss_reduce_input *in=NULL; vsdlss_status st;
    if(!A) { if(out) *out=NULL; return VSDLSS_ERR_INVALID; }
    if(out) *out=NULL;
    st=out?vsdlss_reduce_prepare_csc(*A,&in):VSDLSS_ERR_INVALID;
    vsdlss_spfree(*A); *A=NULL;       /* the adjacency now holds everything */
    if(st!=VSDLSS_OK) return st;
    return vsdlss_reduce_run(in,out);
}
