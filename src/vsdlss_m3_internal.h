#ifndef VSDLSS_M3_INTERNAL_H
#define VSDLSS_M3_INTERNAL_H

#include "vsdlss.h"

typedef struct vsdlss_components {
    csi n, count;
    csi *offset;
    csi *vertices;
    csi *component_of, *local_of;
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
} vsdlss_reduction;

/* Strict-supernode symbolic layout.  Each panel s owns columns
 * [column_start[s],column_start[s+1]) and logical rows J followed by the
 * sorted external rows row_index[row_ptr[s]..row_ptr[s+1]).  panel_offset
 * addresses a dense column-major (width + external_count)-by-width panel.
 * l_panel_slot maps every CSC symbolic-L entry to exactly one absolute panel
 * slot.  update_target stores, for each source panel, target panel slots for
 * the lower triangle of R-by-R in column-major triangular order:
 * (R[0],R[0]), (R[1],R[0]), ... then (R[1],R[1]), ... .  Thus its
 * retained size is sum_s |R_s|(|R_s|+1)/2 csi slots; this can exceed nnz(L).
 */
typedef struct vsdlss_sn_symbolic {
    csi n, count, l_nnz;
    csi *parent;          /* n */
    csi *l_col_ptr;       /* n+1 */
    csi *l_row_index;     /* l_nnz */
    csi *column_start;    /* allocated n+1; first count+1 entries are used */
    csi *row_ptr;         /* count+1 */
    csi *row_index;       /* row_ptr[count] */
    csi *panel_offset;    /* count+1, scalar slots */
    csi *l_panel_slot;    /* l_nnz, absolute panel slots */
    csi *update_ptr;      /* count+1 */
    csi *update_target;   /* update_ptr[count], absolute panel slots */
} vsdlss_sn_symbolic;
typedef struct vsdlss_sn_factor {
    csi n, count, l_nnz;
    csi *l_col_ptr, *l_row_index, *l_panel_slot;
    csi *column_start, *row_ptr, *row_index, *panel_offset;
    double *panel;
} vsdlss_sn_factor;

typedef struct vsdlss_m3_component_factor {
    csi n;
    vsdlss_reduction *reduction; /* owns local reduction and core matrix */
    csi *q;                      /* q[new] = old for the reduced core */
    vsdlss_sn_factor *numeric;   /* owns supernodal numeric layout */
} vsdlss_m3_component_factor;

struct vsdlss_m3_factor {
    csi n, count;
    vsdlss_components *components;       /* owns global/local maps */
    vsdlss_m3_component_factor *component; /* count owned entries */
};

vsdlss_status vsdlss_components_build(const vsdlss *, vsdlss_components **);
vsdlss_status vsdlss_components_build_normalized(const vsdlss *,
                                                 vsdlss_components **);
vsdlss_status vsdlss_component_extract(const vsdlss *, const vsdlss_components *,
                                       csi, vsdlss **);
vsdlss_status vsdlss_component_extract_normalized(const vsdlss *,
                                                  const vsdlss_components *,
                                                  csi, vsdlss **);
void vsdlss_components_free(vsdlss_components *);
vsdlss_status vsdlss_reduce(const vsdlss *, vsdlss_reduction **);
vsdlss_status vsdlss_reduce_rhs(const vsdlss_reduction *, const double *,
                                double *, double *);
vsdlss_status vsdlss_reduce_recover(const vsdlss_reduction *, const double *,
                                    const double *, double *);
void vsdlss_reduction_free(vsdlss_reduction *);
vsdlss_status vsdlss_sn_analyze(const vsdlss *, vsdlss_sn_symbolic **);
void vsdlss_sn_symbolic_free(vsdlss_sn_symbolic *);
vsdlss_status vsdlss_sn_factorize(const vsdlss *, const vsdlss_sn_symbolic *,
                                  vsdlss_sn_factor **);
vsdlss_status vsdlss_sn_solve(const vsdlss_sn_factor *, const double *, double *);
vsdlss_status vsdlss_sn_export_L(const vsdlss_sn_factor *, vsdlss **);
void vsdlss_sn_factor_free(vsdlss_sn_factor *);

/* Shared in-memory/disk panel kernels. Layout is column major. */
vsdlss_status vsdlss_panel_factor(double *, csi rows, csi width);
double vsdlss_panel_dot(const double *, csi rows, csi width, csi i, csi j);
vsdlss_status vsdlss_panel_solve(const double *, csi begin, csi width,
                                csi ext, const csi *index, double *, int back);
vsdlss_status vsdlss_sn_analyze_compact(const vsdlss *, vsdlss_sn_symbolic **);
#endif
