/* VSDLSS function (decompiled by Ghidra) */
/* name: newPqiNoNewVecs_vsdlss  addr: 00a0ca60  size: 6d */
#include "vsdlss_ref.h"

/* signature: undefined4 *newPqiNoNewVecs_vsdlss(int param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4); */

undefined4 *
newPqiNoNewVecs_vsdlss(int param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  
  uVar1 = createTag_vsdlss();
  puVar2 = (undefined4 *)newBlock_vsdlss(uVar1,0x28);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  *(undefined8 *)(puVar2 + 4) = param_2;
  *(undefined8 *)(puVar2 + 6) = param_3;
  *(undefined8 *)(puVar2 + 8) = param_4;
  puVar2[2] = 0;
  initIVector_vsdlss(param_2,(long)param_1,0xffffffff);
  return puVar2;
}


