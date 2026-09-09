#ifndef VSDLSS_M4_INTERNAL_H
#define VSDLSS_M4_INTERNAL_H
#include "vsdlss.h"
typedef struct vsdlss_disk_panel {
    uint64_t begin, width, rows, offset, checksum;
} vsdlss_disk_panel;
struct vsdlss_m4_factor {
    FILE *file;
    csi n;
    csi *q;
    uint64_t *offset, *count;
    size_t workspace_bytes;
    int format_version;
    csi block_count, width_cap;
    size_t max_payload;
    vsdlss_disk_panel *blocks;
    vsdlss_m4_stats stats;
};
vsdlss_status vsdlss_factorize_m4_scalar(const vsdlss *,int,size_t,const char *,vsdlss_m4_factor **);
vsdlss_status vsdlss_m4_panel_solve(vsdlss_m4_factor *,const double *,double *);
#endif
