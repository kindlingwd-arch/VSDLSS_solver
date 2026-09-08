/* VSDLSS function (decompiled by Ghidra) */
/* name: newDoubleEndedQ_vsdlss  addr: 00a2f5d0  size: 36 */
#include "vsdlss_ref.h"

/* signature: undefined4 * newDoubleEndedQ_vsdlss(undefined4 param_1); */

undefined4 * newDoubleEndedQ_vsdlss(undefined4 param_1)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x20);
  *puVar1 = param_1;
  uVar2 = newCEdgelist_vsdlss(param_1,0);
  *(undefined8 *)(puVar1 + 2) = uVar2;
  *(undefined8 *)(puVar1 + 4) = uVar2;
  *(undefined8 *)(puVar1 + 6) = uVar2;
  return puVar1;
}


