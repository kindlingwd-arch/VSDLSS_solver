/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewOneFactorChunkHeader_vsdlss  addr: 009f3460  size: 87 */
#include "vsdlss_ref.h"

/* signature: void tagNewOneFactorChunkHeader_vsdlss(undefined4 param_1,undefined4 param_2); */

void tagNewOneFactorChunkHeader_vsdlss(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)newBlock_vsdlss(param_1,0x70);
  *puVar1 = param_1;
  puVar1[1] = 1;
  puVar1[3] = 1;
  puVar1[4] = param_2;
  puVar1[2] = param_2;
  puVar1[6] = 1;
  puVar1[7] = param_2;
  puVar1[5] = param_2;
  *(undefined8 *)(puVar1 + 8) = 0;
  *(undefined8 *)(puVar1 + 10) = 0;
  *(undefined8 *)(puVar1 + 0xc) = 0;
  *(undefined8 *)(puVar1 + 0xe) = 0;
  puVar1[0x10] = 0;
  *(undefined8 *)(puVar1 + 0x12) = 0;
  *(undefined8 *)(puVar1 + 0x14) = 0;
  puVar1[0x16] = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  puVar1[0x1a] = param_2;
  return;
}


