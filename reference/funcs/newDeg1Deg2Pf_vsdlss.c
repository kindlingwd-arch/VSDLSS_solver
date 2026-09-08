/* VSDLSS function (decompiled by Ghidra) */
/* name: newDeg1Deg2Pf_vsdlss  addr: 009e34b0  size: 29 */
#include "vsdlss_ref.h"

/* signature: long newDeg1Deg2Pf_vsdlss(void); */

long newDeg1Deg2Pf_vsdlss(void)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = newDeg1Deg2PfNoDiagVec_vsdlss();
  uVar2 = newFVector_vsdlss(0,(long)(*(int *)(lVar1 + 4) + *(int *)(lVar1 + 8)),1);
  *(undefined8 *)(lVar1 + 0x10) = uVar2;
  return lVar1;
}


