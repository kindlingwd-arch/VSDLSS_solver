/* VSDLSS function (decompiled by Ghidra) */
/* name: tagNewDeg1Deg2Pf_vsdlss  addr: 009e34e0  size: 37 */
#include "vsdlss_ref.h"

/* signature: long tagNewDeg1Deg2Pf_vsdlss(undefined4 param_1); */

long tagNewDeg1Deg2Pf_vsdlss(undefined4 param_1)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = tagNewDeg1Deg2PfNoDiagVec_vsdlss();
  uVar2 = tagNewFVector_vsdlss(0,param_1,(long)(*(int *)(lVar1 + 4) + *(int *)(lVar1 + 8)),1);
  *(undefined8 *)(lVar1 + 0x10) = uVar2;
  return lVar1;
}


