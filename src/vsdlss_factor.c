#include "vsdlss.h"
#include "vsdlss_internal.h"

#include <string.h>

struct vsdlss_factor {
    vsdlss *normalized;
    vsdlss_num *numeric;
    csi *q;
    csi *pinv;
    csi n;
};

vsdlss_status vsdlss_factorize(const vsdlss *A, int order, vsdlss_factor **out)
{
    vsdlss_factor *factor = NULL;
    vsdlss *normalized = NULL;
    vsdlss_num *numeric = NULL;
    csi *q = NULL, *pinv = NULL, k;
    vsdlss_status status;
    if (!out) return VSDLSS_ERR_INVALID;
    *out = NULL;
    if (order != 0 && order != 1 && order != 2 && order != 3 && order != 4)
        return VSDLSS_ERR_UNSUPPORTED;
    status = vsdlss_normalize_upper(A, &normalized);
    if (status != VSDLSS_OK) return status;
    status = vsdlss_numeric_factorize(normalized, order, &numeric);
    if (status != VSDLSS_OK) { vsdlss_spfree(normalized); return status; }
    pinv = numeric->pinv;
    numeric->pinv = NULL;
    q = (csi *)malloc((size_t)normalized->n * sizeof(csi));
    factor = (vsdlss_factor *)calloc(1, sizeof(*factor));
    if (!q || !factor) {
        free(q); free(pinv); vsdlss_nfree(numeric); vsdlss_spfree(normalized);
        return VSDLSS_ERR_OOM;
    }
    for (k = 0; k < normalized->n; ++k) q[pinv[k]] = k;
    status = vsdlss_validate_permutation(q, pinv, normalized->n);
    if (status != VSDLSS_OK) {
        free(q); free(pinv); free(factor); vsdlss_nfree(numeric); vsdlss_spfree(normalized);
        return status;
    }
    factor->normalized = normalized;
    factor->numeric = numeric;
    factor->q = q;
    factor->pinv = pinv;
    factor->n = normalized->n;
    *out = factor;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_factor_solve(const vsdlss_factor *factor,
                                  const double *rhs, double *solution)
{
    double *work;
    csi k;
    if (!factor || !factor->numeric || !factor->numeric->L || !rhs || !solution)
        return VSDLSS_ERR_INVALID;
    for (k = 0; k < factor->n; ++k) if (!isfinite(rhs[k])) return VSDLSS_ERR_NONFINITE;
    work = (double *)malloc((size_t)factor->n * sizeof(double));
    if (!work) return VSDLSS_ERR_OOM;
    if (!vsdlss_ipvec(factor->pinv, rhs, work, factor->n) ||
        !vsdlss_lsolve(factor->numeric->L, work) ||
        !vsdlss_ltsolve(factor->numeric->L, work)) {
        free(work);
        return VSDLSS_ERR_INVALID;
    }
    for (k = 0; k < factor->n; ++k) {
        if (!isfinite(work[k])) { free(work); return VSDLSS_ERR_NONFINITE; }
    }
    for (k = 0; k < factor->n; ++k) solution[k] = work[factor->pinv[k]];
    free(work);
    return VSDLSS_OK;
}

void vsdlss_factor_free(vsdlss_factor *factor)
{
    if (!factor) return;
    vsdlss_spfree(factor->normalized);
    vsdlss_nfree(factor->numeric);
    free(factor->q);
    free(factor->pinv);
    free(factor);
}

const vsdlss *vsdlss_factor_L(const vsdlss_factor *factor)
{
    return factor && factor->numeric ? factor->numeric->L : NULL;
}

const csi *vsdlss_factor_q(const vsdlss_factor *factor)
{
    return factor ? factor->q : NULL;
}

const csi *vsdlss_factor_pinv(const vsdlss_factor *factor)
{
    return factor ? factor->pinv : NULL;
}

csi vsdlss_factor_dimension(const vsdlss_factor *factor)
{
    return factor ? factor->n : 0;
}
