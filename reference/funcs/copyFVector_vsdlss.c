/* VSDLSS function (decompiled by Ghidra) */
/* name: copyFVector_vsdlss  addr: 009d7c80  size: 25 */
#include "vsdlss_ref.h"

/* signature: void copyFVector_vsdlss(long param_1,ulong param_2,long param_3); */

void copyFVector_vsdlss(long param_1,ulong param_2,long param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_2 != 0) {
    do {
      *(undefined8 *)(param_3 + uVar1 * 8) = *(undefined8 *)(param_1 + uVar1 * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_2);
  }
  return;
}


