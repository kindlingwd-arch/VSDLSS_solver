/* VSDLSS function (decompiled by Ghidra) */
/* name: copyArrayToCEdgelist_vsdlss  addr: 009e2280  size: 4d */
#include "vsdlss_ref.h"

/* signature: undefined8 copyArrayToCEdgelist_vsdlss(long param_1,int param_2); */

undefined8 copyArrayToCEdgelist_vsdlss(long param_1,int param_2)

{
  undefined4 uVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  
  if (0 < param_2) {
    puVar3 = (undefined4 *)(param_1 + (long)param_2 * 4);
    uVar2 = 0;
    do {
      uVar1 = *puVar3;
      puVar3 = puVar3 + -1;
      uVar2 = newCEdgelist_vsdlss(uVar1,uVar2);
    } while (puVar3 != (undefined4 *)(param_1 + -4 + (long)param_2 * 4 + (ulong)(param_2 - 1) * -4))
    ;
    return uVar2;
  }
  return 0;
}


