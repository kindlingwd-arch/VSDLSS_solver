/* VSDLSS function (decompiled by Ghidra) */
/* name: initFVector_vsdlss  addr: 009d7b90  size: 20 */
#include "vsdlss_ref.h"

/* signature: void initFVector_vsdlss(undefined8 param_1,long param_2,ulong param_3); */

void initFVector_vsdlss(undefined8 param_1,long param_2,ulong param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if (param_3 != 0) {
    do {
      *(undefined8 *)(param_2 + uVar1 * 8) = param_1;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= param_3);
  }
  return;
}


