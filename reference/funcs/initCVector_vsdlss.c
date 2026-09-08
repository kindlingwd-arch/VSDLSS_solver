/* VSDLSS function (decompiled by Ghidra) */
/* name: initCVector_vsdlss  addr: 009d7b30  size: 1e */
#include "vsdlss_ref.h"

/* signature: void initCVector_vsdlss(long param_1,ulong param_2,undefined1 param_3); */

void initCVector_vsdlss(long param_1,ulong param_2,undefined1 param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined1 *)(param_1 + uVar1) = param_3;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


