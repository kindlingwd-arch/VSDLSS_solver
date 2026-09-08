/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewMLDGraphNoEdges_vsdlss  addr: 009fc7a0  size: b9 */
#include "vsdlss_ref.h"

/* signature: undefined4 * tagNewMLDGraphNoEdges_vsdlss(ulong param_1,undefined4 param_2,int param_3); */

undefined4 * tagNewMLDGraphNoEdges_vsdlss(ulong param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  long lVar3;
  
  lVar3 = (long)param_3;
  puVar1 = (undefined4 *)newBlock_vsdlss(param_1,0x68);
  *puVar1 = (int)param_1;
  puVar1[1] = param_2;
  puVar1[2] = param_3;
  puVar1[3] = 0;
  uVar2 = tagNewIVector_vsdlss(param_1 & 0xffffffff,lVar3,1,0);
  *(undefined8 *)(puVar1 + 10) = uVar2;
  *(undefined8 *)(puVar1 + 0xc) = 0;
  uVar2 = tagNewIPVector_vsdlss(param_1 & 0xffffffff,lVar3,1,0);
  *(undefined8 *)(puVar1 + 0xe) = uVar2;
  *(undefined8 *)(puVar1 + 0x10) = 0;
  uVar2 = tagNewIPVector_vsdlss(param_1 & 0xffffffff,lVar3,1,0);
  *(undefined8 *)(puVar1 + 0x12) = uVar2;
  *(undefined8 *)(puVar1 + 0x14) = 0;
  uVar2 = tagNewIVector_vsdlss(param_1 & 0xffffffff,lVar3,0,0);
  *(undefined8 *)(puVar1 + 0x16) = uVar2;
  uVar2 = tagNewIVector_vsdlss(param_1 & 0xffffffff,lVar3,0,0);
  *(undefined8 *)(puVar1 + 0x18) = uVar2;
  return puVar1;
}


