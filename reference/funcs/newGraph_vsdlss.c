/* VSDLSS function (decompiled by Ghidra) */
/* name: newGraph_vsdlss  addr: 00a07fe0  size: 40 */
#include "vsdlss_ref.h"

/* signature: void newGraph_vsdlss(undefined4 param_1); */

void newGraph_vsdlss(undefined4 param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x30);
  *puVar1 = param_1;
  puVar1[1] = 0;
  *(undefined8 *)(puVar1 + 2) = 0;
  *(undefined8 *)(puVar1 + 4) = 0;
  *(undefined8 *)(puVar1 + 6) = 0;
  *(undefined8 *)(puVar1 + 8) = 0;
  *(undefined8 *)(puVar1 + 10) = 0;
  return;
}


