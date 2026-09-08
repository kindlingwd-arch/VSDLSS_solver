/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDUncoarsenRefineOneLevel_vsdlss  addr: 00a00c70  size: 85 */
#include "vsdlss_ref.h"

/* signature: void MLDUncoarsenRefineOneLevel_vsdlss(undefined8 param_1,int param_2,int param_3,long param_4,undefined8 param_5); */

void MLDUncoarsenRefineOneLevel_vsdlss
               (undefined8 param_1,int param_2,int param_3,long param_4,undefined8 param_5)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *(long *)(param_4 + -8 + (long)param_2 * 8);
  uVar2 = *(undefined8 *)(param_4 + (long)param_2 * 8);
  MLDUncoarsenPartIdVec_vsdlss(*(undefined4 *)(lVar1 + 8),*(undefined8 *)(lVar1 + 0x60),param_5);
  if (param_2 == param_3) {
    computeInterVecsForBalanceRefine_vsdlss(param_1,lVar1,param_5);
  }
  else {
    computeInterVecsForBalanceRefineFast(param_1,lVar1,uVar2,param_5);
  }
  MLDBalance_vsdlss(param_1,lVar1,param_5);
  MLDFMRefine_h(param_1,lVar1,param_5);
  return;
}


