/* VSDLSS function (decompiled by Ghidra) */
/* name: removeFromPq_vsdlss  addr: 00a2cbe0  size: b8 */
#include "vsdlss_ref.h"

/* signature: void removeFromPq_vsdlss(int param_1,long param_2); */

void removeFromPq_vsdlss(int param_1,long param_2)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  long lVar6;
  
  lVar6 = (long)param_1;
  if ((param_1 < 1) || (*(int *)(param_2 + 4) < param_1)) {
    nrerror_vsdlss(1,"priorityqutils.c",0x24a);
  }
  iVar2 = *(int *)(*(long *)(param_2 + 0x10) + lVar6 * 4);
  if (-1 < iVar2) {
    iVar5 = *(int *)(param_2 + 8);
    if (iVar5 < 1) {
      nrerror_vsdlss(0x26,"priorityqutils.c",0x251);
      iVar2 = *(int *)(*(long *)(param_2 + 0x10) + lVar6 * 4);
      iVar5 = *(int *)(param_2 + 8);
    }
    iVar3 = *(int *)(*(long *)(param_2 + 0x18) + (long)iVar5 * 4);
    puVar1 = (undefined4 *)(*(long *)(param_2 + 0x20) + (long)iVar3 * 4);
    uVar4 = *puVar1;
    *puVar1 = *(undefined4 *)(*(long *)(param_2 + 0x20) + lVar6 * 4);
    pqSwap_vsdlss(iVar2,iVar5,param_2);
    *(int *)(param_2 + 8) = *(int *)(param_2 + 8) + -1;
    updatePq_vsdlss(iVar3,uVar4,param_2);
    *(undefined4 *)(*(long *)(param_2 + 0x10) + lVar6 * 4) = 0xffffffff;
  }
  return;
}


