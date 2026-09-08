/* VSDLSS function (decompiled by Ghidra) */
/* name: newDeg1Deg2PfNoDiagVec_vsdlss  addr: 009e3350  size: a7 */
#include "vsdlss_ref.h"

/* signature: undefined4 * newDeg1Deg2PfNoDiagVec_vsdlss(undefined4 param_1,int param_2,int param_3); */

undefined4 * newDeg1Deg2PfNoDiagVec_vsdlss(undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  long lVar6;
  long lVar7;
  
  puVar3 = (undefined4 *)casiMalloc_vsdlss(0x48);
  puVar3[1] = param_2;
  *puVar3 = param_1;
  puVar3[2] = param_3;
  lVar7 = (long)(param_2 + param_3 * 2);
  *(undefined8 *)(puVar3 + 4) = 0;
  puVar4 = (undefined4 *)newIVector_vsdlss(lVar7,1,0);
  *(undefined4 **)(puVar3 + 6) = puVar4;
  *puVar4 = 0;
  puVar5 = (undefined8 *)newFVector_vsdlss(0,lVar7,1);
  iVar1 = puVar3[1];
  iVar2 = puVar3[2];
  lVar7 = *(long *)(puVar3 + 6);
  *(undefined8 **)(puVar3 + 8) = puVar5;
  *puVar5 = 0;
  *(undefined8 **)(puVar3 + 0xc) = puVar5;
  *(long *)(puVar3 + 10) = lVar7;
  lVar6 = (long)(iVar2 + iVar1) - (long)iVar1;
  *(undefined8 **)(puVar3 + 0x10) = puVar5 + lVar6;
  *(long *)(puVar3 + 0xe) = lVar7 + lVar6 * 4;
  return puVar3;
}


