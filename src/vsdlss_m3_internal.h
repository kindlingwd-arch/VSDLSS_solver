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

typedef struct vsdlss_sn_symbolic vsdlss_sn_symbolic;
typedef struct vsdlss_sn_factor vsdlss_sn_factor;

vsdlss_status vsdlss_components_build(const vsdlss *, vsdlss_components **);
vsdlss_status vsdlss_component_extract(const vsdlss *, const vsdlss_components *,
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

#endif
