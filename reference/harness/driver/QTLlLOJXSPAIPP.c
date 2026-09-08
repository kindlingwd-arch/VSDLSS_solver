/* driver: QTLlLOJXSPAIPP addr=009605c0 size=4d */
#include "vsdlss_ref.h"


void QTLlLOJXSPAIPP(long param_1,ulong param_2)

{
  double dVar1;
  ulong uVar2;
  double __x;
  
  __x = 0.0;
  if (param_2 != 0) {
    __x = 0.0;
    uVar2 = 1;
    do {
      dVar1 = *(double *)(param_1 + uVar2 * 8);
      uVar2 = uVar2 + 1;
      __x = __x + dVar1 * dVar1;
    } while (uVar2 <= param_2);
  }
  if (NAN(SQRT(__x))) {
    sqrt(__x);
  }
  return;
}


