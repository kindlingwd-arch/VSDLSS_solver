/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewMLDGraphHeaderOnly_vsdlss  addr: 009fc860  size: 66 */
#include "vsdlss_ref.h"

/* signature: void tagNewMLDGraphHeaderOnly_vsdlss(undefined4 param_1); */

void tagNewMLDGraphHeaderOnly_vsdlss(undefined4 param_1)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)newBlock_vsdlss(param_1,0x68);
  *puVar1 = param_1;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  *(undefined8 *)(puVar1 + 10) = 0;
  *(undefined8 *)(puVar1 + 0xc) = 0;
  *(undefined8 *)(puVar1 + 0xe) = 0;
  *(undefined8 *)(puVar1 + 0x10) = 0;
  *(undefined8 *)(puVar1 + 0x12) = 0;
  *(undefined8 *)(puVar1 + 0x14) = 0;
  *(undefined8 *)(puVar1 + 0x16) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  return;
}


