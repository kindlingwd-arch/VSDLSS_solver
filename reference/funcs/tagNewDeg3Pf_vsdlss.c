/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewDeg3Pf_vsdlss  addr: 009e3660  size: fd */
#include "vsdlss_ref.h"

/* signature: undefined4 * tagNewDeg3Pf_vsdlss(ulong param_1,undefined4 param_2,int param_3); */

undefined4 * tagNewDeg3Pf_vsdlss(ulong param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  undefined8 *puVar2;
  undefined4 *puVar3;
  long lVar4;
  
  lVar4 = (long)param_3;
  puVar1 = (undefined4 *)newBlock_vsdlss(param_1,0x40);
  *puVar1 = param_2;
  puVar1[1] = param_3;
  puVar2 = (undefined8 *)tagNewFVector_vsdlss(0,param_1 & 0xffffffff,lVar4,1);
  *(undefined8 **)(puVar1 + 2) = puVar2;
  *puVar2 = 0;
  puVar3 = (undefined4 *)tagNewIVector_vsdlss(param_1 & 0xffffffff,lVar4,1,0);
  *(undefined4 **)(puVar1 + 4) = puVar3;
  *puVar3 = 0;
  puVar2 = (undefined8 *)tagNewFVector_vsdlss(0,param_1 & 0xffffffff,lVar4,1);
  *(undefined8 **)(puVar1 + 6) = puVar2;
  *puVar2 = 0;
  puVar3 = (undefined4 *)tagNewIVector_vsdlss(param_1 & 0xffffffff,lVar4,1,0);
  *(undefined4 **)(puVar1 + 8) = puVar3;
  *puVar3 = 0;
  puVar2 = (undefined8 *)tagNewFVector_vsdlss(0,param_1 & 0xffffffff,lVar4,1);
  *(undefined8 **)(puVar1 + 10) = puVar2;
  *puVar2 = 0;
  puVar3 = (undefined4 *)tagNewIVector_vsdlss(param_1 & 0xffffffff,lVar4,1,0);
  *(undefined4 **)(puVar1 + 0xc) = puVar3;
  *puVar3 = 0;
  puVar2 = (undefined8 *)tagNewFVector_vsdlss(0,param_1 & 0xffffffff,lVar4,1);
  *(undefined8 **)(puVar1 + 0xe) = puVar2;
  *puVar2 = 0;
  return puVar1;
}


