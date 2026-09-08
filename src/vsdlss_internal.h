#ifndef VSDLSS_INTERNAL_H
#define VSDLSS_INTERNAL_H

#include "vsdlss.h"

typedef struct vsdlss_graph vsdlss_graph;
typedef struct vsdlss_mld_level vsdlss_mld_level;

vsdlss_status vsdlss_graph_build(const vsdlss *A, vsdlss_graph **out);
void vsdlss_graph_free(vsdlss_graph *graph);
csi vsdlss_graph_degree(const vsdlss_graph *graph, csi vertex);
int vsdlss_graph_has_edge(const vsdlss_graph *graph, csi a, csi b);
vsdlss_status vsdlss_graph_eliminate(vsdlss_graph *graph, csi vertex,
                                     csi **neighbors, csi *count);
csi vsdlss_graph_fill_edges(const vsdlss_graph *graph);
vsdlss_status vsdlss_graph_neighbors(const vsdlss_graph *graph, csi vertex,
                                     const csi **neighbors, csi *count);

vsdlss_status vsdlss_min_degree_order(const vsdlss *A, csi **q,
                                      vsdlss_order_stats *stats);
vsdlss_status vsdlss_min_degree_subset(const vsdlss *A, const csi *vertices,
                                       csi count, csi *order);
vsdlss_status vsdlss_mld_order(const vsdlss *A, csi **q,
                               vsdlss_order_stats *stats);
vsdlss_status vsdlss_mld_top_partition(const vsdlss *A, signed char *side,
                                        csi *left, csi *right, csi *separator);

vsdlss_status vsdlss_mld_level_build(const vsdlss *A, const csi *vertices,
                                      csi count, vsdlss_mld_level **out);
void vsdlss_mld_level_free(vsdlss_mld_level *level);
csi vsdlss_mld_level_vertices(const vsdlss_mld_level *level);
csi vsdlss_mld_level_edges(const vsdlss_mld_level *level);
csi vsdlss_mld_level_vertex_weight(const vsdlss_mld_level *level, csi v);
csi vsdlss_mld_level_edge_weight(const vsdlss_mld_level *level, csi a, csi b);
const csi *vsdlss_mld_level_original(const vsdlss_mld_level *level);
vsdlss_status vsdlss_mld_level_neighbors(const vsdlss_mld_level *level,
                                          csi v, const csi **neighbors,
                                          const csi **weights, csi *count);
vsdlss_status vsdlss_mld_coarsen_one(vsdlss_mld_level *fine,
                                      vsdlss_mld_level **coarse);
const csi *vsdlss_mld_level_map(const vsdlss_mld_level *fine);
vsdlss_status vsdlss_mld_partition_level(const vsdlss_mld_level *level,
                                          signed char *part,
                                          csi *initial_cut, csi *final_cut);
vsdlss_status vsdlss_mld_project_refine(const vsdlss_mld_level *fine,
                                         const signed char *coarse_part,
                                         signed char *fine_part);
vsdlss_status vsdlss_mld_node_separator(const vsdlss_mld_level *level,
                                         signed char *part, csi *left,
                                         csi *right, csi *separator);
vsdlss_status vsdlss_mld_hierarchy_analyze(const vsdlss *A, csi *levels);

vsdlss_status vsdlss_numeric_factorize(const vsdlss *A, int order,
                                       vsdlss_num **out);

#endif
