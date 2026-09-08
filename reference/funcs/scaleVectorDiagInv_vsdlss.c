/* VSDLSS function (decompiled by Ghidra) */
/* name: scaleVectorDiagInv_vsdlss  addr: 009d8000  size: 2a */
#include "vsdlss_ref.h"

/* signature: void scaleVectorDiagInv_vsdlss(long param_1,long param_2,int param_3,long param_4); */

void scaleVectorDiagInv_vsdlss(long param_1,long param_2,int param_3,long param_4)

{
  ulong uVar1;
  
  uVar1 = 1;
  if ((long)param_3 != 0) {
    do {
      *(double *)(param_4 + uVar1 * 8) =
           *(double *)(param_2 + uVar1 * 8) / *(double *)(param_1 + uVar1 * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= (ulong)(long)param_3);
  }
  return;
}


