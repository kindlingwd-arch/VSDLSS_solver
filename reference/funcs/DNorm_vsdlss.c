/* VSDLSS function (decompiled by Ghidra) */
/* name: DNorm_vsdlss  addr: 009d8e00  size: 5c */
#include "vsdlss_ref.h"

/* signature: void DNorm_vsdlss(long param_1,long param_2,int param_3); */

void DNorm_vsdlss(long param_1,long param_2,int param_3)

{
  long lVar1;
  double dVar2;
  double __x;
  
  __x = 0.0;
  if (0 < param_3) {
    __x = 0.0;
    lVar1 = 0;
    do {
      dVar2 = *(double *)(param_1 + 8 + lVar1) * *(double *)(param_2 + 8 + lVar1);
      lVar1 = lVar1 + 8;
      __x = __x + dVar2 * dVar2;
    } while (lVar1 != (ulong)(param_3 - 1) * 8 + 8);
  }
  if (NAN(SQRT(__x))) {
    sqrt(__x);
  }
  return;
}


