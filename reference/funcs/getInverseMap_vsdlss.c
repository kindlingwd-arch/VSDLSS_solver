/* VSDLSS function (decompiled by Ghidra) */
/* name: getInverseMap_vsdlss  addr: 009d8b80  size: 2c */
#include "vsdlss_ref.h"

/* signature: void getInverseMap_vsdlss(int *param_1,int param_2,long param_3); */

void getInverseMap_vsdlss(int *param_1,int param_2,long param_3)

{
  int iVar1;
  
  iVar1 = 1;
  if (0 < param_2) {
    do {
      param_1 = param_1 + 1;
      *(int *)(param_3 + (long)*param_1 * 4) = iVar1;
      iVar1 = iVar1 + 1;
    } while (iVar1 != param_2 + 1);
  }
  return;
}


