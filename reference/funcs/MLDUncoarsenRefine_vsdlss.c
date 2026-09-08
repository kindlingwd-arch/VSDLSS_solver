/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDUncoarsenRefine_vsdlss  addr: 00a00d00  size: 43 */
#include "vsdlss_ref.h"

/* signature: void MLDUncoarsenRefine_vsdlss(undefined8 param_1,int param_2,undefined8 param_3,undefined8 param_4); */

void MLDUncoarsenRefine_vsdlss(undefined8 param_1,int param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_2;
  if (1 < param_2) {
    do {
      iVar2 = iVar1 + -1;
      MLDUncoarsenRefineOneLevel_vsdlss(param_1,iVar1,param_2,param_3,param_4);
      iVar1 = iVar2;
    } while (iVar2 != 1);
  }
  return;
}


