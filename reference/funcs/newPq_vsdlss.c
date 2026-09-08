/* VSDLSS function (decompiled by Ghidra) */
/* name: newPq_vsdlss  addr: 00a2c590  size: 6e */
#include "vsdlss_ref.h"

/* signature: undefined4 * newPq_vsdlss(int param_1); */

undefined4 * newPq_vsdlss(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  long lVar4;
  
  lVar4 = (long)param_1;
  uVar1 = createTag_vsdlss();
  puVar2 = (undefined4 *)newBlock_vsdlss(uVar1,0x28);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  puVar2[2] = 0;
  uVar3 = tagNewIVector_vsdlss(uVar1,lVar4,1,0xffffffff);
  *(undefined8 *)(puVar2 + 4) = uVar3;
  uVar3 = tagNewIVector_vsdlss(uVar1,lVar4,0,0);
  *(undefined8 *)(puVar2 + 6) = uVar3;
  uVar3 = tagNewIVector_vsdlss(uVar1,lVar4,0,0);
  *(undefined8 *)(puVar2 + 8) = uVar3;
  return puVar2;
}


