/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewUNSIGNEDINTVector_vsdlss  addr: 009d8760  size: 41 */
#include "vsdlss_ref.h"

/* signature: void tagNewUNSIGNEDINTVector_vsdlss(undefined8 param_1,ulong param_2,int param_3,undefined4 param_4); */

void tagNewUNSIGNEDINTVector_vsdlss(undefined8 param_1,ulong param_2,int param_3,undefined4 param_4)

{
  long lVar1;
  ulong uVar2;
  
  lVar1 = newBlock_vsdlss(param_1,param_2 * 4 + 4);
  if ((param_3 != 0) && (param_2 != 0)) {
    uVar2 = 1;
    do {
      *(undefined4 *)(lVar1 + uVar2 * 4) = param_4;
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_2);
  }
  return;
}


