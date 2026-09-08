/* VSDLSS function (decompiled by Ghidra) */
/* name: innerProduct_vsdlss  addr: 009d7d40  size: 36 */
#include "vsdlss_ref.h"

/* signature: double innerProduct_vsdlss(long param_1,long param_2,int param_3); */

double innerProduct_vsdlss(long param_1,long param_2,int param_3)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  double dVar4;
  
  if ((long)param_3 != 0) {
    dVar4 = 0.0;
    uVar3 = 1;
    do {
      lVar1 = uVar3 * 8;
      lVar2 = uVar3 * 8;
      uVar3 = uVar3 + 1;
      dVar4 = dVar4 + *(double *)(param_1 + lVar1) * *(double *)(param_2 + lVar2);
    } while (uVar3 <= (ulong)(long)param_3);
    return dVar4;
  }
  return 0.0;
}


