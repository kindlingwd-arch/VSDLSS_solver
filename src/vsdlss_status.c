#include "vsdlss.h"

const char *vsdlss_status_string(vsdlss_status status)
{
    switch (status) {
    case VSDLSS_OK: return "success";
    case VSDLSS_ERR_INVALID: return "invalid input";
    case VSDLSS_ERR_OOM: return "out of memory";
    case VSDLSS_ERR_NOT_POSDEF: return "matrix is not positive definite";
    case VSDLSS_ERR_NONFINITE: return "non-finite numeric value";
    case VSDLSS_ERR_IO: return "I/O error";
    case VSDLSS_ERR_UNSUPPORTED: return "unsupported operation";
    default: return "unknown error";
    }
}
