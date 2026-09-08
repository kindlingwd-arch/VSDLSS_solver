/* VSDLSS function (decompiled by Ghidra) */
/* name: initIPVector_vsdlss  addr: 009d7b70  size: 1f */
#include "vsdlss_ref.h"

/* signature: void initIPVector_vsdlss(long param_1,ulong param_2,undefined8 param_3); */

void initIPVector_vsdlss(long param_1,ulong param_2,undefined8 param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined8 *)(param_1 + uVar1 * 8) = param_3;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


