/* VSDLSS function (decompiled by Ghidra) */
/* name: newAStack_vsdlss  addr: 00a1e700  size: 5a */
#include "vsdlss_ref.h"

/* signature: undefined4 * newAStack_vsdlss(int param_1); */

undefined4 * newAStack_vsdlss(int param_1)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  int iVar3;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x18);
  *puVar1 = 0xffffffff;
  iVar3 = 0x20;
  if (0x83f < param_1) {
    iVar3 = param_1 >> 6;
  }
  puVar1[3] = param_1;
  if (param_1 < iVar3) {
    iVar3 = param_1;
  }
  puVar1[2] = iVar3;
  uVar2 = newIVector_vsdlss((long)iVar3,0,0);
  puVar1[1] = 0;
  *(undefined8 *)(puVar1 + 4) = uVar2;
  return puVar1;
}


