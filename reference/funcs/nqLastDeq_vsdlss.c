/* VSDLSS function (decompiled by Ghidra) */
/* name: nqLastDeq_vsdlss  addr: 00a2f650  size: 2d */
#include "vsdlss_ref.h"

/* signature: void nqLastDeq_vsdlss(undefined8 param_1,long param_2); */

void nqLastDeq_vsdlss(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *(long *)(param_2 + 0x10);
  uVar2 = newCEdgelist_vsdlss(param_1,*(undefined8 *)(lVar1 + 8));
  *(undefined8 *)(lVar1 + 8) = uVar2;
  *(undefined8 *)(param_2 + 0x10) = *(undefined8 *)(*(long *)(param_2 + 0x10) + 8);
  return;
}


