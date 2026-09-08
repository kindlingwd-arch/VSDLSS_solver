/* driver: _LMVtB_QLOXSPAIPP addr=009603c0 size=25 */
#include "vsdlss_ref.h"


void _LMVtB_QLOXSPAIPP(long param_1,ulong param_2,long param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined8 *)(param_3 + uVar1 * 8) = *(undefined8 *)(param_1 + uVar1 * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


