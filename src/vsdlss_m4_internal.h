#ifndef VSDLSS_M4_INTERNAL_H
#define VSDLSS_M4_INTERNAL_H
#include "vsdlss.h"
struct vsdlss_m4_factor {
    FILE *file;
    csi n;
    csi *q;
    uint64_t *offset, *count;
    size_t workspace_bytes;
};
#endif
