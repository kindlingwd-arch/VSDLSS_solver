/* VSDLSS function (decompiled by Ghidra) */
/* name: newQ_vsdlss  addr: 00a0d560  size: 3f */
#include "vsdlss_ref.h"

/* signature: undefined4 * newQ_vsdlss(int param_1); */

undefined4 * newQ_vsdlss(int param_1)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x18);
  puVar1[2] = param_1;
  uVar2 = newIVector_vsdlss((long)param_1,0,0);
  *puVar1 = 0;
  *(undefined8 *)(puVar1 + 4) = uVar2;
  puVar1[1] = 0;
  return puVar1;
}


