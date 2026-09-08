/* VSDLSS function (decompiled by Ghidra) */
/* name: newCountingMark_vsdlss  addr: 00a1f1d0  size: 3b */
#include "vsdlss_ref.h"

/* signature: undefined4 * newCountingMark_vsdlss(int param_1); */

undefined4 * newCountingMark_vsdlss(int param_1)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x10);
  puVar1[1] = param_1;
  *puVar1 = 1;
  uVar2 = newIVector_vsdlss((long)param_1,1,0);
  *(undefined8 *)(puVar1 + 2) = uVar2;
  return puVar1;
}


