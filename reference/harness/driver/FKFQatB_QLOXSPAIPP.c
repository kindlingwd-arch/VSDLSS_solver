/* driver: FKFQatB_QLOXSPAIPP addr=00960340 size=1e */
#include "vsdlss_ref.h"


void FKFQatB_QLOXSPAIPP(long param_1,ulong param_2,undefined1 param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined1 *)(param_1 + uVar1) = param_3;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


