/* driver: FKSnBOJRQBbtB_QLOXSPAIPP addr=00961b40 size=2a */
#include "vsdlss_ref.h"


void FKSnBOJRQBbtB_QLOXSPAIPP(long param_1,ulong param_2,long param_3,long param_4)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined8 *)(param_4 + uVar1 * 8) =
           *(undefined8 *)(param_3 + *(long *)(param_1 + uVar1 * 8) * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


