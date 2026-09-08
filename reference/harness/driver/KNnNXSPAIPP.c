/* driver: KNnNXSPAIPP addr=009a6dc0 size=86 */
#include "vsdlss_ref.h"


void KNnNXSPAIPP(long param_1,undefined8 param_2,long param_3)

{
  long lVar1;
  long lVar2;
  
  if ((param_1 < 1) || (lVar1 = *(long *)(param_3 + 8), lVar1 < param_1)) {
    KOBOOLOXSPAIPP(1,"priorityqutils.c",0x1eb);
    lVar1 = *(long *)(param_3 + 8);
  }
  lVar2 = *(long *)(param_3 + 0x10);
  if (lVar1 <= lVar2) {
    KOBOOLOXSPAIPP(0x27,"priorityqutils.c",0x1ee);
    lVar2 = *(long *)(param_3 + 0x10);
  }
  lVar2 = lVar2 + 1;
  *(long *)(param_3 + 0x10) = lVar2;
  *(long *)(*(long *)(param_3 + 0x20) + lVar2 * 8) = param_1;
  *(undefined8 *)(*(long *)(param_3 + 0x28) + param_1 * 8) = param_2;
  *(long *)(*(long *)(param_3 + 0x18) + param_1 * 8) = lVar2;
  MNbB_OByPBAiBVgQBOXSPAIPP(lVar2,param_3);
  return;
}


