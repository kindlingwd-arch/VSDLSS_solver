/* VSDLSS function (decompiled by Ghidra) */
/* name: copyIVector_vsdlss  addr: 009d7c20  size: 21 */
#include "vsdlss_ref.h"

/* signature: void copyIVector_vsdlss(long param_1,ulong param_2,long param_3); */

void copyIVector_vsdlss(long param_1,ulong param_2,long param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined4 *)(param_3 + uVar1 * 4) = *(undefined4 *)(param_1 + uVar1 * 4);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


