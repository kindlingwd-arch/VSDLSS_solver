/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewFVector_vsdlss  addr: 009d87b0  size: 4d */
#include "vsdlss_ref.h"

/* signature: void tagNewFVector_vsdlss(undefined8 param_1,undefined8 param_2,ulong param_3,int param_4); */

void tagNewFVector_vsdlss(undefined8 param_1,undefined8 param_2,ulong param_3,int param_4)

{
  long lVar1;
  ulong uVar2;
  
  lVar1 = newBlock_vsdlss(param_2,param_3 * 8 + 8);
  if ((param_4 != 0) && (param_3 != 0)) {
    uVar2 = 1;
    do {
      *(undefined8 *)(lVar1 + uVar2 * 8) = param_1;
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_3);
  }
  return;
}


