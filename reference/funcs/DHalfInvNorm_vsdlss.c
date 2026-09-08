/* VSDLSS function (decompiled by Ghidra) */
/* name: DHalfInvNorm_vsdlss  addr: 009d8e60  size: 66 */
#include "vsdlss_ref.h"

/* signature: void DHalfInvNorm_vsdlss(long param_1,long param_2,int param_3); */

void DHalfInvNorm_vsdlss(long param_1,long param_2,int param_3)

{
  double dVar1;
  double dVar2;
  long lVar3;
  double __x;
  
  __x = 0.0;
  if (0 < param_3) {
    lVar3 = 0;
    __x = 0.0;
    do {
      dVar1 = *(double *)(param_1 + 8 + lVar3);
      if (0.0 < dVar1) {
        dVar2 = *(double *)(param_2 + 8 + lVar3);
        __x = __x + (dVar2 * dVar2) / dVar1;
      }
      lVar3 = lVar3 + 8;
    } while (lVar3 != (ulong)(param_3 - 1) * 8 + 8);
  }
  if (NAN(SQRT(__x))) {
    sqrt(__x);
  }
  return;
}


