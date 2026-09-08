/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewMLDGraph_vsdlss  addr: 009fc700  size: a0 */
#include "vsdlss_ref.h"

/* signature: void tagNewMLDGraph_vsdlss(undefined4 param_1,undefined4 param_2,int param_3,int param_4); */

void tagNewMLDGraph_vsdlss(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  long lVar1;
  long lVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  long lVar5;
  
  lVar2 = (long)(param_3 * 2 + 2) * 2;
  puVar3 = (undefined4 *)newBlock_vsdlss(param_1,(((param_4 + param_3) * 4 + 6) + lVar2) * 4 + 0x70)
  ;
  puVar4 = puVar3 + 0x1c;
  puVar3[3] = param_4;
  *puVar3 = param_1;
  puVar3[1] = param_2;
  *(undefined4 **)(puVar3 + 0xe) = puVar4;
  puVar4 = puVar4 + (long)param_3 * 2 + 2;
  *(undefined4 **)(puVar3 + 0x12) = puVar4;
  lVar1 = (long)(param_4 * 2) * 4 + 4;
  *(undefined4 **)(puVar3 + 10) = puVar4 + (long)param_3 * 2 + 2;
  lVar5 = (long)(puVar4 + (long)param_3 * 2 + 2) + lVar2;
  puVar3[2] = param_3;
  *(long *)(puVar3 + 0xc) = lVar5;
  lVar5 = lVar5 + lVar2;
  *(long *)(puVar3 + 0x10) = lVar5;
  lVar5 = lVar5 + lVar1;
  *(long *)(puVar3 + 0x14) = lVar5;
  lVar5 = lVar5 + lVar1;
  *(long *)(puVar3 + 0x16) = lVar5;
  *(long *)(puVar3 + 0x18) = lVar2 + lVar5;
  return;
}


