/* driver: bfyICgKSlLOJXSPAIPP addr=00961670 size=64 */
#include "vsdlss_ref.h"


void bfyICgKSlLOJXSPAIPP(long param_1,long param_2,long param_3)

{
  double dVar1;
  double dVar2;
  long lVar3;
  double __x;
  
  __x = 0.0;
  if (0 < param_3) {
    lVar3 = 1;
    __x = 0.0;
    do {
      dVar1 = *(double *)(param_1 + lVar3 * 8);
      if (0.0 < dVar1) {
        dVar2 = *(double *)(param_2 + lVar3 * 8);
        __x = __x + (dVar2 * dVar2) / dVar1;
      }
      lVar3 = lVar3 + 1;
    } while (lVar3 != param_3 + 1);
  }
  if (NAN(SQRT(__x))) {
    sqrt(__x);
  }
  return;
}


