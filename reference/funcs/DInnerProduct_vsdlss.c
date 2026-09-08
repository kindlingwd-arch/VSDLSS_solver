/* VSDLSS function (decompiled by Ghidra) */
/* name: DInnerProduct_vsdlss  addr: 009d7e20  size: 46 */
#include "vsdlss_ref.h"

/* signature: double DInnerProduct_vsdlss(long param_1,long param_2,long param_3,int param_4); */

double DInnerProduct_vsdlss(long param_1,long param_2,long param_3,int param_4)

{
  double *pdVar1;
  double *pdVar2;
  double dVar3;
  long lVar4;
  double dVar5;
  
  if (0 < param_4) {
    dVar5 = 0.0;
    lVar4 = 0;
    do {
      dVar3 = *(double *)(param_1 + 8 + lVar4);
      pdVar1 = (double *)(param_2 + 8 + lVar4);
      pdVar2 = (double *)(param_3 + 8 + lVar4);
      lVar4 = lVar4 + 8;
      dVar5 = dVar5 + *pdVar1 * dVar3 * dVar3 * *pdVar2;
    } while (lVar4 != (ulong)(param_4 - 1) * 8 + 8);
    return dVar5;
  }
  return 0.0;
}


