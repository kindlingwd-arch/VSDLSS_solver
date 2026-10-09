#ifndef VSDLSS_M3_INTERNAL_H
#define VSDLSS_M3_INTERNAL_H

#include "vsdlss.h"
#include <stdatomic.h>

typedef struct vsdlss_components {
    csi n, count;
    csi *offset;
    csi *vertices;
    csi *component_of, *local_of;
    /* Optional (may be NULL): per component, local indices in BFS discovery
     * order, laid out like `vertices`.  Neighbours get nearby positions, which
     * the low-degree reduction uses as a cache-friendly numbering. */
    csi *order;
} vsdlss_components;

typedef struct vsdlss_elim_record {
    csi vertex, degree, neighbor[3];
    double pivot, multiplier[3];
} vsdlss_elim_record;

typedef struct vsdlss_pk_seg {
    csi k0, count, nbn;
    uint32_t *head, *nb;
    double *val;
} vsdlss_pk_seg;

typedef struct vsdlss_reduction {
    csi n, count, core_n;
    vsdlss_elim_record *records;
    csi *core_vertices;
    vsdlss *core;
    /* Optional: records [block_ptr[b], block_ptr[b+1]) for b < blocks came
     * from the parallel block pass and touch only that block's vertices, so
     * blocks can be replayed concurrently; the rest follow sequentially. */
    csi blocks;
    csi *block_ptr;
    /* Packed replay form (records is then NULL): segments pk[0..pk_count)
     * cover records [pk[s].k0, pk[s].k0 + pk[s].count) in order.  When the
     * reduction is blocked, segment b < blocks is block b and the last one
     * holds the sequential tail.  Within a segment, record i has head[i] =
     * vertex | degree << 30, neighbours nb[o..o+d) and pivot, multipliers
     * val[i+o], val[i+o+1..i+o+d] (o = sum of the degrees before i).  Packed
     * records were validated (indices in range, pivots and multipliers
     * finite, pivots nonzero), so replay needs no per-entry checks. */
    csi pk_count;
    struct vsdlss_pk_seg *pk;
} vsdlss_reduction;

/* Supernodal symbolic layout.
 *
 * Panel s owns columns [column_start[s], column_start[s+1]) and stores the
 * rows J_s = those columns followed by the sorted external rows
 * R_s = row_index[row_ptr[s]..row_ptr[s+1]).  panel_offset addresses a dense
 * column-major (width + |R_s|)-by-width panel; the strictly upper part of the
 * leading width-by-width block is kept zero.
 *
 * Strict analysis yields maximal (fundamental) supernodes, so every panel
 * slot on or below the diagonal is a structural entry of L.  Relaxed analysis
 * additionally amalgamates a child into its parent when the explicit zeros it
 * introduces stay under the usual size-dependent thresholds; l_nnz then counts
 * those stored zeros too (relaxed_zeros of them).
 *
 * The left-looking numeric phase needs, for every target panel d, the source
 * blocks that update it: block b = (blk_src[b], blk_first[b], blk_end[b]) for
 * b in [blk_ptr[d], blk_ptr[d+1]) says that rows R_src[first..end) are
 * columns of d, and R_src[first..|R_src|) are the rows to update.  Blocks of
 * one target are sorted by source, which fixes the accumulation order.
 * Storage is O(n + sum |R_s|); no per-entry map of L is kept. */
typedef struct vsdlss_sn_symbolic {
    csi n, count, l_nnz, relaxed_zeros, max_rows;
    csi *sn_parent;       /* count, -1 for roots */
    csi *column_start;    /* count+1 */
    csi *row_ptr;         /* count+1 */
    csi *row_index;       /* row_ptr[count] */
    csi *panel_offset;    /* count+1, scalar slots */
    csi *blk_ptr;         /* count+1 */
    csi *blk_src, *blk_first, *blk_end; /* blk_ptr[count] each */
} vsdlss_sn_symbolic;
/* Supernodal numeric factor.  Indices below n or count are 32-bit
 * (vsdlss_sni; the factorization rejects larger cores), offsets 64-bit.
 *
 * Panels are packed: panel s (width w, e = |R_s| external rows) holds the
 * lower triangle of its diagonal block by columns, column j (rows j..w-1)
 * at VSDLSS_SN_DCOL(j, w), followed by the external rows column-major with
 * leading dimension e: w(w+1)/2 + w e values, exactly the stored entries
 * counted by l_nnz, so panel_offset[count] == l_nnz. */
typedef int32_t vsdlss_sni;
#define VSDLSS_SN_DCOL(j,w) ((j)*(w)-(j)*((j)-1)/2)
typedef struct vsdlss_sn_factor {
    csi n, count, l_nnz;
    vsdlss_sni *column_start;  /* count+1 */
    csi *row_ptr;              /* count+1 */
    vsdlss_sni *row_index;     /* row_ptr[count] */
    csi *panel_offset;         /* count+1 */
    double *panel;        /* panel_offset[count] values */
    void *panel_block;    /* allocation owning `panel` (may be aligned inside) */
    /* Tree and source blocks (copied from the symbolic layout) for the
     * tree-parallel triangular solves. */
    vsdlss_sni *sn_parent;     /* count */
    csi *blk_ptr;              /* count+1 */
    vsdlss_sni *blk_src, *blk_first, *blk_end;
    /* Solve-tree postorder and per-thread-count subtree splits, built once
     * so repeated tree-parallel solves do not rebuild them (NULL: built per
     * solve). */
    struct vsdlss_sn_solve_tree *solve_tree;
} vsdlss_sn_factor;

typedef struct vsdlss_m3_component_factor {
    csi n;
    int renumbered;              /* local numbering is the BFS order */
    /* Global vertex of each local index.  map32 when every global index
     * fits 32 bits (then the 64-bit tables are freed once factored);
     * otherwise gather, or NULL for components->vertices in ascending
     * order.  During the factorization gather holds the BFS numbering. */
    uint32_t *map32;
    csi *gather;
    vsdlss_m4_factor *disk;
    vsdlss_reduction *reduction; /* owns local reduction and core matrix */
    /* Local vertex of each (permuted) core unknown: core_map32 when local
     * indices fit 32 bits, else core_map. */
    uint32_t *core_map32;
    csi *core_map;
    vsdlss_sn_factor *numeric;   /* owns supernodal numeric layout */
    int core_contig;             /* core_map[k] == reduction->count + k (relabelled) */
    /* Solve workspace allocated with the factor, so repeated solves do not
     * allocate (and fault in) large buffers.  A solve takes it with an
     * atomic flag; a concurrent solve on the same factor falls back to
     * private buffers. */
    double *ws_local, *ws_saved, *ws_core;
    atomic_int ws_busy;
} vsdlss_m3_component_factor;

struct vsdlss_m3_factor {
    csi n, count;
    int disk_mode;
    vsdlss_components *components;       /* owns global/local maps */
    vsdlss_m3_component_factor *component; /* count owned entries */
    /* Global vertex -> (component << inv_shift) | local index, so the solve
     * writes the caller's solution sequentially (reading the component
     * buffers at random) instead of scattering into it.  32-bit codes when
     * they fit, else 64-bit; both NULL means the per-component scatter. */
    uint32_t *inv32;
    uint64_t *inv64;
    int inv_shift;
    /* Blocked two-pass gather / write-back of original-order solves
     * (vsdlss_m3.c): plans built on first use per thread count, and a
     * scratch area (bucket buffer, write-combining lines, staging blocks;
     * pbuf_len doubles, 64-byte aligned) taken with an atomic flag. */
#define VSDLSS_PERM_PLAN_SLOTS 65
    _Atomic(struct vsdlss_perm_plan *) pplan[VSDLSS_PERM_PLAN_SLOTS];
    double *pbuf;
    size_t pbuf_len;
    atomic_int pbuf_busy;
};

/* Symmetric weighted adjacency (no diagonal in idx/val; diag separate). */
typedef struct vsdlss_wgraph {
    csi n;
    csi *ptr;        /* n+1 */
    csi *idx;        /* ptr[n] neighbours, ascending per vertex */
    double *val;     /* ptr[n] off-diagonal values */
    double *diag;    /* n */
} vsdlss_wgraph;
void vsdlss_wgraph_free(vsdlss_wgraph *);
/* For a validated upper CSC: 1 if every column is strictly increasing. */
int vsdlss_is_normalized_upper(const vsdlss *A);
vsdlss_status vsdlss_components_build(const vsdlss *, vsdlss_components **);
/* Normalized input; also returns the weighted adjacency built on the way. */
vsdlss_status vsdlss_components_build_graph(const vsdlss *, vsdlss_components **,
                                            vsdlss_wgraph **);
vsdlss_status vsdlss_components_build_normalized(const vsdlss *,
                                                 vsdlss_components **);
vsdlss_status vsdlss_component_extract(const vsdlss *, const vsdlss_components *,
                                       csi, vsdlss **);
vsdlss_status vsdlss_component_extract_normalized(const vsdlss *,
                                                  const vsdlss_components *,
                                                  csi, vsdlss **);
void vsdlss_components_free(vsdlss_components *);
/* Extract component `c` of a normalized matrix numbered by perm (new local
 * index -> old local index, i.e. position in components->vertices); the
 * result is normalized upper CSC. */
vsdlss_status vsdlss_component_extract_permuted(const vsdlss *, const vsdlss_components *,
                                                csi c, const csi *perm, vsdlss **);
vsdlss_status vsdlss_reduce(const vsdlss *, vsdlss_reduction **);
/* Same, but takes ownership of *A and frees it as soon as the adjacency is
 * built (always freed; *A is NULL on return). */
vsdlss_status vsdlss_reduce_consume(vsdlss **A, vsdlss_reduction **);
/* Two-step form: prepare the adjacency (from CSC, or from a component of a
 * weighted graph renumbered by map: new -> graph vertex, newidx: graph vertex
 * -> new), then run the elimination, which consumes the input. */
typedef struct vsdlss_reduce_input vsdlss_reduce_input;
vsdlss_status vsdlss_reduce_prepare_csc(const vsdlss *A, vsdlss_reduce_input **);
vsdlss_status vsdlss_reduce_prepare_graph(const csi *ptr, const csi *idx, const double *val,
                                          const double *diag, const csi *map, csi n,
                                          const csi *newidx, vsdlss_reduce_input **);
vsdlss_status vsdlss_reduce_run(vsdlss_reduce_input *, vsdlss_reduction **);
void vsdlss_reduce_input_free(vsdlss_reduce_input *);
vsdlss_status vsdlss_reduce_rhs(const vsdlss_reduction *, const double *,
                                double *, double *);
vsdlss_status vsdlss_reduce_recover(const vsdlss_reduction *, const double *,
                                    const double *, double *);
void vsdlss_reduction_free(vsdlss_reduction *);
/* Size thresholds (internal; tests lower them to reach the large-input
 * paths on small matrices).  Results depend on them, never on threads. */
extern csi vsdlss_reduce_block;   /* block of the parallel reduction pass */
extern csi vsdlss_reduce_tomb_min; /* tests: shortest adjacency list removed lazily (tombstones) */
extern csi vsdlss_reorder_min;    /* min component size for BFS renumbering */
extern int vsdlss_m3_inverse_force64; /* tests: 64-bit inverse-map codes */
extern csi vsdlss_perm2_min;         /* tests: smallest n for the two-pass gather/write-back */
extern csi vsdlss_perm2_nt_min;      /* tests: smallest n for its non-temporal stores */
/* Benchmarks (bench_core): called with each component's ordered core
 * matrix (upper CSC, postordered) before its supernodal analysis; may run
 * concurrently for different components.  NULL in normal use. */
extern void (*vsdlss_m3_core_hook)(csi component, const vsdlss *core);
extern int vsdlss_fwd_top_team;        /* 1: one team for the whole forward tree top (0: a team per large target) */
/* Return free heap memory to the system when VSDLSS_TRIM >= level (glibc
 * malloc_trim; no-op elsewhere).  Levels: 1 end of factorization, 2 between
 * factorization phases. */
void vsdlss_release_free_memory(int level);
/* Non-transactional in-place variants for callers with private buffers. */
/* Replaces records by the packed form (about half the bytes; the solve's
 * reduction replay is bandwidth bound).  Keeps records, returning OK, when
 * they cannot be packed (n > 2^30, invalid entries); OOM leaves r as is. */
vsdlss_status vsdlss_reduce_pack(vsdlss_reduction *);
/* vsdlss_reduce_run producing the packed form directly (same result as
 * vsdlss_reduce_run followed by vsdlss_reduce_pack, without compacting). */
typedef struct { double *local, *saved, *core; } vsdlss_reduce_ws;
/* ws (optional) receives solve buffers of n, count and core_n doubles taken
 * over from the reduction's own arrays (NULL when a size is 0; saved is also
 * NULL when the result is packed, whose replays need no saved array). */
vsdlss_status vsdlss_reduce_run_packed(vsdlss_reduce_input *, vsdlss_reduction **,
                                       vsdlss_reduce_ws *ws);
/* work must be finite on entry (callers check while gathering); overflow in
 * the replay shows up as a non-finite core RHS or in the backward pass.
 * saved may be NULL when r is packed (records NULL); both replays must then
 * use the same work/x vector, untouched between them except at core vertices. */
vsdlss_status vsdlss_reduce_forward_inplace(const vsdlss_reduction *, double *work, double *saved);
vsdlss_status vsdlss_reduce_backward_inplace(const vsdlss_reduction *, const double *saved, double *x);
vsdlss_status vsdlss_sn_analyze(const vsdlss *, vsdlss_sn_symbolic **);
vsdlss_status vsdlss_sn_analyze_relaxed(const vsdlss *, vsdlss_sn_symbolic **);
/* Compose an elimination-tree postorder into (q, pinv) for matrix A (the
 * matrix before permutation).  Fill is unchanged; supernodes get larger. */
vsdlss_status vsdlss_postorder_permutation(const vsdlss *A, csi *q, csi *pinv);
void vsdlss_sn_symbolic_free(vsdlss_sn_symbolic *);
/* Column order inside each supernode of s that makes descendants' row sets
 * contiguous (vsdlss_sn_reorder.c): *perm (new -> old, n entries) keeps
 * every column within its supernode. */
vsdlss_status vsdlss_sn_reorder_within(const vsdlss_sn_symbolic *s, csi **perm);
vsdlss_status vsdlss_sn_factorize(const vsdlss *, const vsdlss_sn_symbolic *,
                                  vsdlss_sn_factor **);
/* Same, but takes ownership of *A and *s and frees them as soon as the
 * factor has copied what it needs, before L is allocated (always freed;
 * both NULL on return), then returns free heap memory to the system
 * (vsdlss_release_free_memory(2)). */
vsdlss_status vsdlss_sn_factorize_consume(vsdlss **A, vsdlss_sn_symbolic **s,
                                          vsdlss_sn_factor **);
vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *, const double *, double *);
/* In place on x (length n), no allocation on the serial path and no input or
 * output finiteness scans: the caller checks x afterwards (a non-finite input
 * or intermediate always leaves a non-finite entry in the result).  Bitwise
 * the same result as vsdlss_sn_solve.  On failure x is partially updated. */
vsdlss_status vsdlss_sn_solve_inplace(const vsdlss_sn_factor *, double *x);
vsdlss_status vsdlss_sn_solve_batch(const vsdlss_sn_factor *, csi nrhs, double *x, csi ldx);
vsdlss_status vsdlss_sn_export_L(const vsdlss_sn_factor *, vsdlss **);
void vsdlss_sn_factor_free(vsdlss_sn_factor *);

/* Shared in-memory/disk panel kernels. Layout is column major. */
vsdlss_status vsdlss_panel_factor(double *, csi rows, csi width);
vsdlss_status vsdlss_panel_solve(const double *, csi begin, csi width,
                                csi ext, const csi *index, double *, int back);
#ifdef VSDLSS_BLAS
/* 1 when wide panels are solved with BLAS (VSDLSS_BLAS_SOLVE_MIN > 0). */
int vsdlss_panel_solve_uses_blas(void);
#endif
/* Internal reference path for microkernel validation. */
vsdlss_status vsdlss_panel_solve_generic(const double *,csi,csi,csi,const csi *,double *,int);
/* The same solves for one packed panel of a vsdlss_sn_factor (same
 * operations and results as the full-layout kernels on the same values). */
/* 1 unless VSDLSS_SOLVE_UPD16=0: 16-row forward block updates (vsdlss_panel.c). */
int vsdlss_solve_upd16(void);
vsdlss_status vsdlss_sn_panel_solve(const double *, csi begin, csi width,
                                    csi ext, const vsdlss_sni *index, double *, int back);
vsdlss_status vsdlss_sn_panel_solve_generic(const double *,csi,csi,csi,const vsdlss_sni *,double *,int);

#endif
