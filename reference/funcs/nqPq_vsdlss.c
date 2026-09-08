/* VSDLSS function (decompiled by Ghidra) */
/* name: nqPq_vsdlss  addr: 00a2cab0  size: 7e */
#include "vsdlss_ref.h"

/* signature: void nqPq_vsdlss(int param_1,undefined4 param_2,long param_3); */

void nqPq_vsdlss(int param_1,undefined4 param_2,long param_3)

{
  int iVar1;
  int iVar2;
  
  if ((param_1 < 1) || (iVar2 = *(int *)(param_3 + 4), iVar2 < param_1)) {
    nrerror_vsdlss(1,"priorityqutils.c",0x1eb);
    iVar2 = *(int *)(param_3 + 4);
  }
  iVar1 = *(int *)(param_3 + 8);
  if (iVar2 <= iVar1) {
    nrerror_vsdlss(0x27,"priorityqutils.c",0x1ee);
    iVar1 = *(int *)(param_3 + 8);
  }
  iVar1 = iVar1 + 1;
  *(int *)(param_3 + 8) = iVar1;
  *(int *)(*(long *)(param_3 + 0x18) + (long)iVar1 * 4) = param_1;
  *(undefined4 *)(*(long *)(param_3 + 0x20) + (long)param_1 * 4) = param_2;
  *(int *)(*(long *)(param_3 + 0x10) + (long)param_1 * 4) = iVar1;
  pqDecreasedKeyIter_vsdlss(iVar1,param_3);
  return;
}


