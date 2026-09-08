/* VSDLSS function (decompiled by Ghidra) */
/* name: newDeg3Pf_vsdlss  addr: 009e3580  size: e0 */
#include "vsdlss_ref.h"

/* signature: undefined4 * newDeg3Pf_vsdlss(undefined4 param_1,int param_2); */

undefined4 * newDeg3Pf_vsdlss(undefined4 param_1,int param_2)

{
  undefined4 *puVar1;
  undefined8 *puVar2;
  undefined4 *puVar3;
  long lVar4;
  
  lVar4 = (long)param_2;
  puVar1 = (undefined4 *)casiMalloc_vsdlss(0x40);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  puVar2 = (undefined8 *)newFVector_vsdlss(0,lVar4,1);
  *(undefined8 **)(puVar1 + 2) = puVar2;
  *puVar2 = 0;
  puVar3 = (undefined4 *)newIVector_vsdlss(lVar4,1,0);
  *(undefined4 **)(puVar1 + 4) = puVar3;
  *puVar3 = 0;
  puVar2 = (undefined8 *)newFVector_vsdlss(0,lVar4,1);
  *(undefined8 **)(puVar1 + 6) = puVar2;
  *puVar2 = 0;
  puVar3 = (undefined4 *)newIVector_vsdlss(lVar4,1,0);
  *(undefined4 **)(puVar1 + 8) = puVar3;
  *puVar3 = 0;
  puVar2 = (undefined8 *)newFVector_vsdlss(0,lVar4,1);
  *(undefined8 **)(puVar1 + 10) = puVar2;
  *puVar2 = 0;
  puVar3 = (undefined4 *)newIVector_vsdlss(lVar4,1,0);
  *(undefined4 **)(puVar1 + 0xc) = puVar3;
  *puVar3 = 0;
  puVar2 = (undefined8 *)newFVector_vsdlss(0,lVar4,1);
  *(undefined8 **)(puVar1 + 0xe) = puVar2;
  *puVar2 = 0;
  return puVar1;
}


