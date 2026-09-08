/* driver: RMAyQBnNXSPAIPP addr=009a6c70 size=7d */
#include "vsdlss_ref.h"


void RMAyQBnNXSPAIPP(long param_1,long param_2,long param_3)

{
  long *plVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  
  if ((param_1 < 1) || (*(long *)(param_3 + 8) < param_1)) {
    KOBOOLOXSPAIPP(1,"priorityqutils.c",0x181);
    plVar1 = (long *)(*(long *)(param_3 + 0x28) + param_1 * 8);
    lVar2 = *plVar1;
    lVar4 = lVar2 - param_2;
  }
  else {
    plVar1 = (long *)(*(long *)(param_3 + 0x28) + param_1 * 8);
    lVar2 = *plVar1;
    lVar4 = lVar2 - param_2;
  }
  if (lVar2 <= param_2) {
    if (SBORROW8(lVar2,param_2) != lVar4 < 0) {
      uVar3 = *(undefined8 *)(*(long *)(param_3 + 0x18) + param_1 * 8);
      *plVar1 = param_2;
      MNgK_OByPBAiBVgQBOXSPAIPP(uVar3,param_3);
      return;
    }
    return;
  }
  uVar3 = *(undefined8 *)(*(long *)(param_3 + 0x18) + param_1 * 8);
  *plVar1 = param_2;
  MNbB_OByPBAiBVgQBOXSPAIPP(uVar3,param_3);
  return;
}


