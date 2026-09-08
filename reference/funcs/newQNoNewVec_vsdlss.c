/* VSDLSS function (decompiled by Ghidra) */
/* name: newQNoNewVec_vsdlss  addr: 00a0d5d0  size: 30 */
#include "vsdlss_ref.h"

/* signature: void newQNoNewVec_vsdlss(undefined4 param_1,undefined8 param_2); */

void newQNoNewVec_vsdlss(undefined4 param_1,undefined8 param_2)

{
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x18);
  puVar1[2] = param_1;
  *(undefined8 *)(puVar1 + 4) = param_2;
  *puVar1 = 0;
  puVar1[1] = 0;
  return;
}


