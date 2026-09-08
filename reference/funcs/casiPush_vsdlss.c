/* VSDLSS function (decompiled by Ghidra) */
/* name: casiPush_vsdlss  addr: 00a2f730  size: 13 */
#include "vsdlss_ref.h"

/* signature: void casiPush_vsdlss(undefined8 param_1,long param_2); */

void casiPush_vsdlss(undefined8 param_1,long param_2)

{
  undefined8 uVar1;
  
  uVar1 = newCEdgelist_vsdlss(param_1,*(undefined8 *)(param_2 + 8));
  *(undefined8 *)(param_2 + 8) = uVar1;
  return;
}


