/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDBalance_vsdlss  addr: 009ffb20  size: 79 */
#include "vsdlss_ref.h"

/* signature: void MLDBalance_vsdlss(long param_1,long param_2,undefined8 param_3); */

void MLDBalance_vsdlss(long param_1,long param_2,undefined8 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  iVar1 = *(int *)(param_1 + 4);
  iVar2 = *(int *)(param_2 + 0x1c);
  iVar3 = *(int *)(param_2 + 0x20);
  uVar4 = iVar1 - iVar2 >> 0x1f;
  if (((((iVar2 + iVar3) * 3) / *(int *)(param_2 + 8) <= (int)((iVar1 - iVar2 ^ uVar4) - uVar4)) &&
      ((iVar2 <= iVar1 || ((int)((double)iVar1 * DAT_00ec5d98) <= iVar2)))) &&
     ((iVar3 <= *(int *)(param_1 + 8) ||
      ((int)((double)*(int *)(param_1 + 8) * DAT_00ec5d98) <= iVar3)))) {
    MLDBalanceBoundaryNodes_h(param_1,param_2,param_3);
    return;
  }
  return;
}


