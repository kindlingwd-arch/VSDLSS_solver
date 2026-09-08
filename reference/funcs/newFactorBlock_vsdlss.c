/* VSDLSS function (decompiled by Ghidra) */
/* name: newFactorBlock_vsdlss  addr: 009f21f0  size: 9a */
#include "vsdlss_ref.h"

/* signature: undefined4 * newFactorBlock_vsdlss(int param_1,int param_2,int param_3); */

undefined4 * newFactorBlock_vsdlss(int param_1,int param_2,int param_3)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x38);
  puVar1[4] = param_2;
  puVar1[5] = param_3;
  *puVar1 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  uVar2 = newIVector_vsdlss((long)param_2,0,0);
  *(undefined8 *)(puVar1 + 6) = uVar2;
  uVar2 = newFVector_vsdlss(0,(long)param_3,0);
  *(undefined8 *)(puVar1 + 8) = uVar2;
  uVar2 = newIPVector_vsdlss((long)param_1,1,0);
  *(undefined8 *)(puVar1 + 10) = uVar2;
  uVar2 = newFPVector_vsdlss((long)param_1,1,0);
  *(undefined8 *)(puVar1 + 0xc) = uVar2;
  return puVar1;
}


