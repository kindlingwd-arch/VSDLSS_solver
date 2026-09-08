/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewIPVector_vsdlss  addr: 009d8800  size: 42 */
#include "vsdlss_ref.h"

/* signature: void tagNewIPVector_vsdlss(undefined8 param_1,ulong param_2,int param_3,undefined8 param_4); */

void tagNewIPVector_vsdlss(undefined8 param_1,ulong param_2,int param_3,undefined8 param_4)

{
  long lVar1;
  ulong uVar2;
  
  lVar1 = newBlock_vsdlss(param_1,param_2 * 8 + 8);
  if ((param_3 != 0) && (param_2 != 0)) {
    uVar2 = 1;
    do {
      *(undefined8 *)(lVar1 + uVar2 * 8) = param_4;
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_2);
  }
  return;
}


