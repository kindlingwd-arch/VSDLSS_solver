/* driver: yAAgtB_QLOPXSPAIPP addr=009604e0 size=27 */
#include "vsdlss_ref.h"


void yAAgtB_QLOPXSPAIPP(long param_1,long param_2,ulong param_3,long param_4)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_3 != 0) {
    do {
      *(long *)(param_4 + uVar1 * 8) =
           *(long *)(param_1 + uVar1 * 8) + *(long *)(param_2 + uVar1 * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_3);
  }
  return;
}


