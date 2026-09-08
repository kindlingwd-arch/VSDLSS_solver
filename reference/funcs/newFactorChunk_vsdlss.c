/* VSDLSS function (decompiled by Ghidra) */
/* name: newFactorChunk_vsdlss  addr: 009f23b0  size: f8 */
#include "vsdlss_ref.h"

/* signature: undefined4 *newFactorChunk_vsdlss(undefined4 param_1,int param_2,int param_3,int param_4,int param_5); */

undefined4 *
newFactorChunk_vsdlss(undefined4 param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  int iVar4;
  
  uVar1 = createTag_vsdlss();
  puVar2 = (undefined4 *)newBlock_vsdlss(uVar1,0x70);
  puVar2[4] = param_3;
  puVar2[7] = param_5;
  iVar4 = (param_5 - param_4) + 1;
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  puVar2[3] = param_2;
  puVar2[2] = (param_3 - param_2) + 1;
  puVar2[6] = param_4;
  puVar2[5] = iVar4;
  uVar3 = newIVector_vsdlss((long)iVar4,1,0);
  *(undefined8 *)(puVar2 + 8) = uVar3;
  uVar3 = tagNewIVector_vsdlss(uVar1,(long)(int)puVar2[2],1,0);
  *(undefined8 *)(puVar2 + 10) = uVar3;
  uVar3 = tagNewIPVector_vsdlss(uVar1,(long)(int)puVar2[2],1,0);
  *(undefined8 *)(puVar2 + 0xc) = uVar3;
  uVar3 = tagNewFPVector_vsdlss(uVar1,(long)(int)puVar2[2],1,0);
  *(undefined8 *)(puVar2 + 0xe) = uVar3;
  puVar2[0x10] = 0;
  *(undefined8 *)(puVar2 + 0x12) = 0;
  *(undefined8 *)(puVar2 + 0x14) = 0;
  puVar2[0x16] = 0;
  puVar2[0x1a] = puVar2[4];
  *(undefined8 *)(puVar2 + 0x18) = 0;
  return puVar2;
}


