/* VSDLSS function (decompiled by Ghidra) */
/* name: dqPq_vsdlss  addr: 00a2ca20  size: 7f */
#include "vsdlss_ref.h"

/* signature: int dqPq_vsdlss(long param_1,undefined4 *param_2); */

int dqPq_vsdlss(long param_1,undefined4 *param_2)

{
  int iVar1;
  
  if (*(int *)(param_1 + 8) < 1) {
    nrerror_vsdlss(0x26,"priorityqutils.c",0x1cd);
  }
  iVar1 = *(int *)(*(long *)(param_1 + 0x18) + 4);
  *param_2 = *(undefined4 *)(*(long *)(param_1 + 0x20) + (long)iVar1 * 4);
  pqSwap_vsdlss(1,*(undefined4 *)(param_1 + 8),param_1);
  *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + -1;
  pqIncreasedKeyIter_vsdlss(1,param_1);
  *(undefined4 *)(*(long *)(param_1 + 0x10) + (long)iVar1 * 4) = 0xffffffff;
  return iVar1;
}


