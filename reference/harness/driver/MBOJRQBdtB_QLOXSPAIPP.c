/* driver: MBOJRQBdtB_QLOXSPAIPP addr=00961a10 size=2a */
#include "vsdlss_ref.h"


void MBOJRQBdtB_QLOXSPAIPP(long param_1,ulong param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  uVar3 = 1;
  if (param_2 != 0) {
    do {
      lVar1 = uVar3 * 8;
      lVar2 = uVar3 * 8;
      uVar3 = uVar3 + 1;
      *(undefined8 *)(param_4 + *(long *)(param_1 + lVar1) * 8) = *(undefined8 *)(param_3 + lVar2);
    } while (uVar3 <= param_2);
  }
  return;
}


