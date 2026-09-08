/* VSDLSS function (decompiled by Ghidra) */
/* name: removeFromPqi_vsdlss  addr: 00a0d490  size: 94 */
#include "vsdlss_ref.h"

/* signature: void removeFromPqi_vsdlss(int param_1,long param_2); */

void removeFromPqi_vsdlss(int param_1,long param_2)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  long lVar6;
  
  lVar6 = (long)param_1;
  iVar2 = *(int *)(*(long *)(param_2 + 0x10) + lVar6 * 4);
  if (-1 < iVar2) {
    iVar5 = *(int *)(param_2 + 8);
    if (iVar5 < 1) {
      nrerror_vsdlss(5,"priorityqutils_i.c",0x297);
      iVar2 = *(int *)(*(long *)(param_2 + 0x10) + lVar6 * 4);
      iVar5 = *(int *)(param_2 + 8);
    }
    iVar3 = *(int *)(*(long *)(param_2 + 0x18) + (long)iVar5 * 4);
    puVar1 = (undefined4 *)(*(long *)(param_2 + 0x20) + (long)iVar3 * 4);
    uVar4 = *puVar1;
    *puVar1 = *(undefined4 *)(*(long *)(param_2 + 0x20) + lVar6 * 4);
    pqiSwap_vsdlss(iVar2,iVar5,param_2);
    *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + -1;
    updatePqi_vsdlss(iVar3,uVar4,param_2);
    *(undefined4 *)(*(long *)(param_2 + 0x10) + lVar6 * 4) = 0xffffffff;
  }
  return;
}


