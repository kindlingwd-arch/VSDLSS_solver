/* VSDLSS function (decompiled by Ghidra) */
/* name: newSparseFactor_vsdlss  addr: 00a08320  size: b6 */
#include "vsdlss_ref.h"

/* signature: undefined4 * newSparseFactor_vsdlss(int param_1,char param_2); */

undefined4 * newSparseFactor_vsdlss(int param_1,char param_2)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  
  uVar1 = createTag_vsdlss();
  puVar2 = (undefined4 *)newBlock_vsdlss(uVar1,0x48);
  *puVar2 = uVar1;
  *(undefined8 *)(puVar2 + 2) = 0;
  puVar2[4] = param_1;
  *(char *)(puVar2 + 7) = param_2;
  *(undefined1 *)(puVar2 + 0xe) = 0;
  *(undefined1 *)((long)puVar2 + 0x1d) = 0;
  uVar3 = tagNewIVector_vsdlss(uVar1,(long)param_1,0,0);
  *(undefined8 *)(puVar2 + 0xc) = uVar3;
  uVar3 = newBlock_vsdlss(uVar1,(long)(param_1 + 1) << 3);
  *(undefined8 *)(puVar2 + 0x10) = uVar3;
  *(undefined8 *)(puVar2 + 8) = 0;
  if ((1 < (byte)(param_2 - 2U)) && (param_2 != '\0')) {
    *(undefined8 *)(puVar2 + 10) = 0;
    return puVar2;
  }
  uVar3 = tagNewFVector_vsdlss(0,uVar1,(long)param_1,1);
  *(undefined8 *)(puVar2 + 10) = uVar3;
  return puVar2;
}


