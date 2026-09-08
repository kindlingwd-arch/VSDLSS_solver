/* VSDLSS function (decompiled by Ghidra) */
/* name: sqrtFVector_vsdlss  addr: 009d8c50  size: 56 */
#include "vsdlss_ref.h"

/* signature: void sqrtFVector_vsdlss(long param_1,int param_2,long param_3); */

void sqrtFVector_vsdlss(long param_1,int param_2,long param_3)

{
  ulong uVar1;
  double dVar2;
  
  uVar1 = 1;
  if ((long)param_2 != 0) {
    do {
      dVar2 = SQRT(*(double *)(param_1 + uVar1 * 8));
      if (NAN(dVar2)) {
        dVar2 = sqrt(*(double *)(param_1 + uVar1 * 8));
      }
      *(double *)(param_3 + uVar1 * 8) = dVar2;
      uVar1 = uVar1 + 1;
    } while (uVar1 <= (ulong)(long)param_2);
  }
  return;
}


