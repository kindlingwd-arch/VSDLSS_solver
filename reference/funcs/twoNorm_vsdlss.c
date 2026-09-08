/* VSDLSS function (decompiled by Ghidra) */
/* name: twoNorm_vsdlss  addr: 009d7dd0  size: 4d */
#include "vsdlss_ref.h"

/* signature: void twoNorm_vsdlss(long param_1,int param_2); */

void twoNorm_vsdlss(long param_1,int param_2)

{
  double dVar1;
  ulong uVar2;
  double __x;
  
  __x = 0.0;
  if ((long)param_2 != 0) {
    __x = 0.0;
    uVar2 = 1;
    do {
      dVar1 = *(double *)(param_1 + uVar2 * 8);
      uVar2 = uVar2 + 1;
      __x = __x + dVar1 * dVar1;
    } while (uVar2 <= (ulong)(long)param_2);
  }
  if (NAN(SQRT(__x))) {
    sqrt(__x);
  }
  return;
}


