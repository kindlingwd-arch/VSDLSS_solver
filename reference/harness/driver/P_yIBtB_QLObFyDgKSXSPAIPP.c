/* driver: P_yIBtB_QLObFyDgKSXSPAIPP addr=00960810 size=2a */
#include "vsdlss_ref.h"


void P_yIBtB_QLObFyDgKSXSPAIPP(long param_1,long param_2,ulong param_3,long param_4)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_3 != 0) {
    do {
      *(double *)(param_4 + uVar1 * 8) =
           *(double *)(param_2 + uVar1 * 8) / *(double *)(param_1 + uVar1 * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_3);
  }
  return;
}


