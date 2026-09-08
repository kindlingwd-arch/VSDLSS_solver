/* VSDLSS function (decompiled by Ghidra) */
/* name: newFactorBlockHeader_vsdlss  addr: 009f2170  size: 7f */
#include "vsdlss_ref.h"

/* signature: undefined4 * newFactorBlockHeader_vsdlss(int param_1); */

undefined4 * newFactorBlockHeader_vsdlss(int param_1)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x38);
  *puVar1 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0;
  puVar1[5] = 0;
  *(undefined8 *)(puVar1 + 6) = 0;
  *(undefined8 *)(puVar1 + 8) = 0;
  uVar2 = newIPVector_vsdlss((long)param_1,1,0);
  *(undefined8 *)(puVar1 + 10) = uVar2;
  uVar2 = newFPVector_vsdlss((long)param_1,1,0);
  *(undefined8 *)(puVar1 + 0xc) = uVar2;
  return puVar1;
}


