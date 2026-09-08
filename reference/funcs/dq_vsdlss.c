/* VSDLSS function (decompiled by Ghidra) */
/* name: dq_vsdlss  addr: 00a0d7a0  size: 2c */
#include "vsdlss_ref.h"

/* signature: undefined4 dq_vsdlss(int *param_1); */

undefined4 dq_vsdlss(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = *param_1;
  if (param_1[1] == iVar1) {
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = *(undefined4 *)(*(long *)(param_1 + 4) + (long)iVar1 * 4);
    *param_1 = iVar1 + 1;
    if (param_1[2] < iVar1 + 1) {
      *param_1 = 0;
      return uVar2;
    }
  }
  return uVar2;
}


