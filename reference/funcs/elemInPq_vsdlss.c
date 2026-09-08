/* VSDLSS function (decompiled by Ghidra) */
/* name: elemInPq_vsdlss  addr: 00a2ccb0  size: 5e */
#include "vsdlss_ref.h"

/* signature: undefined8 elemInPq_vsdlss(long param_1,int param_2,undefined4 *param_3); */

undefined8 elemInPq_vsdlss(long param_1,int param_2,undefined4 *param_3)

{
  int iVar1;
  long lVar2;
  
  lVar2 = (long)param_2;
  if ((param_2 < 1) || (*(int *)(param_1 + 4) < param_2)) {
    nrerror_vsdlss(1,"priorityqutils.c",0x274);
    iVar1 = *(int *)(*(long *)(param_1 + 0x10) + lVar2 * 4);
  }
  else {
    iVar1 = *(int *)(*(long *)(param_1 + 0x10) + lVar2 * 4);
  }
  if (-1 < iVar1) {
    *param_3 = *(undefined4 *)(*(long *)(param_1 + 0x20) + lVar2 * 4);
    return 1;
  }
  return 0;
}


