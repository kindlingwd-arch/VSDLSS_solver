/* VSDLSS function (decompiled by Ghidra) */
/* name: writeInvMapAsPerm_vsdlss  addr: 00a1a770  size: 4a */
#include "vsdlss_ref.h"

/* signature: void writeInvMapAsPerm_vsdlss(int param_1,undefined8 param_2); */

void writeInvMapAsPerm_vsdlss(int param_1,undefined8 param_2)

{
  long lVar1;
  
  lVar1 = newIVector_vsdlss((long)param_1,0,0);
  getInverseMap_vsdlss(param_2,param_1,lVar1);
  writeIVectorFNum_vsdlss(0x26,param_1,lVar1);
  if (lVar1 != 0) {
    casiFree_vsdlss(lVar1);
    return;
  }
  return;
}


