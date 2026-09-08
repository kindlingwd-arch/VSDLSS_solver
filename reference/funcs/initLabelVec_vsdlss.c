/* VSDLSS function (decompiled by Ghidra) */
/* name: initLabelVec_vsdlss  addr: 009d8c30  size: 1f */
#include "vsdlss_ref.h"

/* signature: void initLabelVec_vsdlss(int *param_1,int param_2); */

void initLabelVec_vsdlss(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = 1;
  if (0 < param_2) {
    do {
      param_1 = param_1 + 1;
      *param_1 = iVar1;
      iVar1 = iVar1 + 1;
    } while (iVar1 != param_2 + 1);
  }
  return;
}


