#ifndef VSDLSS_PARALLEL_H
#define VSDLSS_PARALLEL_H
#include "vsdlss.h"
#ifdef _OPENMP
#include <omp.h>
#define VSDLSS_OMP(x) _Pragma(#x)
#else
#define VSDLSS_OMP(x)
#endif
int vsdlss_parallel_width(double work);
void vsdlss_parallel_observe(void); /* master thread only */
#endif
