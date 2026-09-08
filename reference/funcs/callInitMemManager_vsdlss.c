/* VSDLSS function (decompiled by Ghidra) */
/* name: callInitMemManager_vsdlss  addr: 009dce70  size: 54 */
#include "vsdlss_ref.h"

/* signature: void callInitMemManager_vsdlss(int param_1); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callInitMemManager_vsdlss(int param_1)

{
  ulong uVar1;
  ulong uVar2;
  double dVar3;
  
  dVar3 = (double)(param_1 + 2) * INIT_ALLOC_MULT_vsdlss + _DAT_00b1a140;
  if (DAT_00b126e8 <= dVar3) {
    uVar1 = (long)(dVar3 - DAT_00b126e8) ^ 0x8000000000000000;
  }
  else {
    uVar1 = (ulong)dVar3;
  }
  uVar2 = 0x20;
  if (0x1f < uVar1) {
    uVar2 = uVar1;
  }
  initMemManager_vsdlss(uVar2);
  return;
}


