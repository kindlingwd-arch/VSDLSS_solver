/* driver: PNOQdtB_QLOXSPAIPP addr=00961470 size=59 */
#include "vsdlss_ref.h"


void PNOQdtB_QLOXSPAIPP(long param_1,ulong param_2,long param_3)

{
  ulong uVar1;
  double dVar2;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      dVar2 = SQRT(*(double *)(param_1 + uVar1 * 8));
      if (NAN(dVar2)) {
        dVar2 = sqrt(*(double *)(param_1 + uVar1 * 8));
      }
      *(double *)(param_3 + uVar1 * 8) = dVar2;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


