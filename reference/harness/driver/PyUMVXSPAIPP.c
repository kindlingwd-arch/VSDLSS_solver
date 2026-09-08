/* driver: PyUMVXSPAIPP addr=00960840 size=2e */
#include "vsdlss_ref.h"


void PyUMVXSPAIPP(double param_1,long param_2,long param_3,ulong param_4,long param_5)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_4 != 0) {
    do {
      *(double *)(param_5 + uVar1 * 8) =
           *(double *)(param_2 + uVar1 * 8) * param_1 + *(double *)(param_3 + uVar1 * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_4);
  }
  return;
}


