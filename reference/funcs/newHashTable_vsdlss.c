/* VSDLSS function (decompiled by Ghidra) */
/* name: newHashTable_vsdlss  addr: 00a2f7b0  size: 33 */
#include "vsdlss_ref.h"

/* signature: undefined4 * newHashTable_vsdlss(undefined4 param_1); */

undefined4 * newHashTable_vsdlss(undefined4 param_1)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x10);
  *puVar1 = param_1;
  uVar2 = newCEdgeVector_vsdlss(param_1,1,0);
  *(undefined8 *)(puVar1 + 2) = uVar2;
  return puVar1;
}


