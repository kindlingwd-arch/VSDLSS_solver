/* VSDLSS function (decompiled by Ghidra) */
/* name: updatePq_vsdlss  addr: 00a2c950  size: 79 */
#include "vsdlss_ref.h"

/* signature: void updatePq_vsdlss(int param_1,int param_2,long param_3); */

void updatePq_vsdlss(int param_1,int param_2,long param_3)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  long lVar5;
  
  lVar5 = (long)param_1;
  if ((param_1 < 1) || (*(int *)(param_3 + 4) < param_1)) {
    nrerror_vsdlss(1,"priorityqutils.c",0x181);
    piVar1 = (int *)(*(long *)(param_3 + 0x20) + lVar5 * 4);
    iVar2 = *piVar1;
    iVar4 = iVar2 - param_2;
  }
  else {
    piVar1 = (int *)(*(long *)(param_3 + 0x20) + lVar5 * 4);
    iVar2 = *piVar1;
    iVar4 = iVar2 - param_2;
  }
  if (iVar2 <= param_2) {
    if (SBORROW4(iVar2,param_2) != iVar4 < 0) {
      uVar3 = *(undefined4 *)(*(long *)(param_3 + 0x10) + lVar5 * 4);
      *piVar1 = param_2;
      pqIncreasedKeyIter_vsdlss(uVar3,param_3);
      return;
    }
    return;
  }
  uVar3 = *(undefined4 *)(*(long *)(param_3 + 0x10) + lVar5 * 4);
  *piVar1 = param_2;
  pqDecreasedKeyIter_vsdlss(uVar3,param_3);
  return;
}


