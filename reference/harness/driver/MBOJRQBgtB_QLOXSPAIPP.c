/* driver: MBOJRQBgtB_QLOXSPAIPP addr=00961710 size=27 */
#include "vsdlss_ref.h"


void MBOJRQBgtB_QLOXSPAIPP(long param_1,long param_2,ulong param_3,long param_4)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  
  uVar3 = 1;
  if (param_3 != 0) {
    do {
      lVar1 = uVar3 * 8;
      lVar2 = uVar3 * 8;
      uVar3 = uVar3 + 1;
      *(undefined8 *)(param_4 + *(long *)(param_1 + lVar2) * 8) = *(undefined8 *)(param_2 + lVar1);
    } while (uVar3 <= param_3);
  }
  return;
}


