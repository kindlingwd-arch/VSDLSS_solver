/* VSDLSS function (decompiled by Ghidra) */
/* name: scaleVector_vsdlss  addr: 009d7fa0  size: 29 */
#include "vsdlss_ref.h"

/* signature: void scaleVector_vsdlss(double param_1,long param_2,int param_3,long param_4); */

void scaleVector_vsdlss(double param_1,long param_2,int param_3,long param_4)

{
  ulong uVar1;
  
  uVar1 = 1;
  if ((long)param_3 != 0) {
    do {
      *(double *)(param_4 + uVar1 * 8) = *(double *)(param_2 + uVar1 * 8) * param_1;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= (ulong)(long)param_3);
  }
  return;
}


