/* driver: blLOJXSPAIPP addr=00961610 size=5a */
#include "vsdlss_ref.h"


void blLOJXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  double dVar2;
  double __x;
  
  __x = 0.0;
  if (0 < param_3) {
    __x = 0.0;
    lVar1 = 1;
    do {
      dVar2 = *(double *)(param_1 + lVar1 * 8) * *(double *)(param_2 + lVar1 * 8);
      lVar1 = lVar1 + 1;
      __x = __x + dVar2 * dVar2;
    } while (lVar1 != param_3 + 1);
  }
  if (NAN(SQRT(__x))) {
    sqrt(__x);
  }
  return;
}


