/* VSDLSS function (decompiled by Ghidra) */
/* name: popAStack_vsdlss  addr: 00a1e900  size: 1e */
#include "vsdlss_ref.h"

/* signature: undefined4 popAStack_vsdlss(long param_1); */

undefined4 popAStack_vsdlss(long param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = *(int *)(param_1 + 4);
  if (iVar1 != 0) {
    uVar2 = *(undefined4 *)(*(long *)(param_1 + 0x10) + (long)iVar1 * 4);
    *(int *)(param_1 + 4) = iVar1 + -1;
    return uVar2;
  }
  return 0xffffffff;
}


