#ifndef VSDLSS_INTERNAL_H
#define VSDLSS_INTERNAL_H

#include "vsdlss.h"

typedef struct vsdlss_graph vsdlss_graph;

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

vsdlss_status vsdlss_numeric_factorize(const vsdlss *A, int order,
                                       vsdlss_num **out);

#endif
