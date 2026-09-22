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
typedef struct vsdlss_sn_factor {
    csi n, count, l_nnz;
    csi *column_start, *row_ptr, *row_index, *panel_offset;
    double *panel;        /* panel_offset[count] values */
    void *panel_block;    /* allocation owning `panel` (may be aligned inside) */
    /* Tree and source blocks (copied from the symbolic layout) for the
     * tree-parallel triangular solves. */
    csi *sn_parent, *blk_ptr, *blk_src, *blk_first, *blk_end;
} vsdlss_sn_factor;

typedef struct vsdlss_m3_component_factor {
    csi n;
    csi *gather;                 /* NULL, or global vertex of each local index */
    vsdlss_m4_factor *disk;
    vsdlss_reduction *reduction; /* owns local reduction and core matrix */
    csi *q;                      /* q[new] = old for the reduced core */
    vsdlss_sn_factor *numeric;   /* owns supernodal numeric layout */
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
    double *ws_global;                   /* staging buffer for vsdlss_m3_solve */
    atomic_int ws_busy;
    vsdlss_components *components;       /* owns global/local maps */
    vsdlss_m3_component_factor *component; /* count owned entries */
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
extern csi vsdlss_reorder_min;    /* min component size for BFS renumbering */
/* Non-transactional in-place variants for callers with private buffers. */
vsdlss_status vsdlss_reduce_forward_inplace(const vsdlss_reduction *, double *work, double *saved);
vsdlss_status vsdlss_reduce_backward_inplace(const vsdlss_reduction *, const double *saved, double *x);
vsdlss_status vsdlss_sn_analyze(const vsdlss *, vsdlss_sn_symbolic **);
vsdlss_status vsdlss_sn_analyze_relaxed(const vsdlss *, vsdlss_sn_symbolic **);
/* Compose an elimination-tree postorder into (q, pinv) for matrix A (the
 * matrix before permutation).  Fill is unchanged; supernodes get larger. */
vsdlss_status vsdlss_postorder_permutation(const vsdlss *A, csi *q, csi *pinv);
void vsdlss_sn_symbolic_free(vsdlss_sn_symbolic *);
vsdlss_status vsdlss_sn_factorize(const vsdlss *, const vsdlss_sn_symbolic *,
                                  vsdlss_sn_factor **);
vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *, const double *, double *);
vsdlss_status vsdlss_sn_solve_batch(const vsdlss_sn_factor *, csi nrhs, double *x, csi ldx);
vsdlss_status vsdlss_sn_export_L(const vsdlss_sn_factor *, vsdlss **);
void vsdlss_sn_factor_free(vsdlss_sn_factor *);

/* Shared in-memory/disk panel kernels. Layout is column major. */
vsdlss_status vsdlss_panel_factor(double *, csi rows, csi width);
vsdlss_status vsdlss_panel_solve(const double *, csi begin, csi width,
                                csi ext, const csi *index, double *, int back);
/* Internal reference path for microkernel validation. */
vsdlss_status vsdlss_panel_solve_generic(const double *,csi,csi,csi,const csi *,double *,int);

#endif
