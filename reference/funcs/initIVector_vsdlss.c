/* VSDLSS function (decompiled by Ghidra) */
/* name: initIVector_vsdlss  addr: 009d7b50  size: 1e */
#include "vsdlss_ref.h"

/* signature: void initIVector_vsdlss(long param_1,ulong param_2,undefined4 param_3); */

void initIVector_vsdlss(long param_1,ulong param_2,undefined4 param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined4 *)(param_1 + uVar1 * 4) = param_3;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


