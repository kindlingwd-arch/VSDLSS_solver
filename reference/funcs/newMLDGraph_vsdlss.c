/* VSDLSS function (decompiled by Ghidra) */
/* name: newMLDGraph_vsdlss  addr: 009fc3a0  size: 9e */
#include "vsdlss_ref.h"

/* signature: undefined4 * newMLDGraph_vsdlss(undefined4 param_1,int param_2,int param_3); */

undefined4 * newMLDGraph_vsdlss(undefined4 param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x68);
  puVar1[1] = param_1;
  puVar1[2] = param_2;
  puVar1[3] = param_3;
  *puVar1 = 0xffffffff;
  uVar2 = newIVector_vsdlss((long)param_2,1,0);
  *(undefined8 *)(puVar1 + 10) = uVar2;
  *(undefined8 *)(puVar1 + 0xc) = 0;
  uVar2 = newIPVector_vsdlss((long)param_2,1,0);
  *(undefined8 *)(puVar1 + 0xe) = uVar2;
  uVar2 = newIVector_vsdlss((long)(param_3 * 2),0,0);
  *(undefined8 *)(puVar1 + 0x12) = 0;
  *(undefined8 *)(puVar1 + 0x10) = uVar2;
  *(undefined8 *)(puVar1 + 0x14) = 0;
  *(undefined8 *)(puVar1 + 0x16) = 0;
  *(undefined8 *)(puVar1 + 0x18) = 0;
  return puVar1;
}


