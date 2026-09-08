/* VSDLSS function (decompiled by Ghidra) */
/* name: newOneFactorChunkHeader_vsdlss  addr: 009f2320  size: 8e */
#include "vsdlss_ref.h"

/* signature: void newOneFactorChunkHeader_vsdlss(undefined4 param_1); */

void newOneFactorChunkHeader_vsdlss(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  
  uVar1 = createTag_vsdlss();
  puVar2 = (undefined4 *)newBlock_vsdlss(uVar1,0x70);
  *puVar2 = uVar1;
  puVar2[1] = 1;
  puVar2[3] = 1;
  puVar2[4] = param_1;
  puVar2[2] = param_1;
  puVar2[6] = 1;
  puVar2[7] = param_1;
  puVar2[5] = param_1;
  *(undefined8 *)(puVar2 + 8) = 0;
  *(undefined8 *)(puVar2 + 10) = 0;
  *(undefined8 *)(puVar2 + 0xc) = 0;
  *(undefined8 *)(puVar2 + 0xe) = 0;
  puVar2[0x10] = 0;
  *(undefined8 *)(puVar2 + 0x12) = 0;
  *(undefined8 *)(puVar2 + 0x14) = 0;
  puVar2[0x16] = 0;
  *(undefined8 *)(puVar2 + 0x18) = 0;
  puVar2[0x1a] = param_1;
  return;
}


