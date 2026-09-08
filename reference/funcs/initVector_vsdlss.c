/* VSDLSS function (decompiled by Ghidra) */
/* name: initVector_vsdlss  addr: 009d7b10  size: 20 */
#include "vsdlss_ref.h"

/* signature: void initVector_vsdlss(undefined8 param_1,long param_2,ulong param_3); */

void initVector_vsdlss(undefined8 param_1,long param_2,ulong param_3)

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


