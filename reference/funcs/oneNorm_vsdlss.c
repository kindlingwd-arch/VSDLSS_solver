/* VSDLSS function (decompiled by Ghidra) */
/* name: oneNorm_vsdlss  addr: 009d7d80  size: 4f */
#include "vsdlss_ref.h"

/* signature: double oneNorm_vsdlss(long param_1,int param_2); */

double oneNorm_vsdlss(long param_1,int param_2)

{
  double dVar1;
  ulong uVar2;
  ulong uVar3;
  double dVar4;
  
  uVar3 = (ulong)param_2;
  if (uVar3 == 0) {
    return 0.0;
  }
  uVar2 = 1;
  dVar4 = 0.0;
  do {
    while (dVar1 = *(double *)(param_1 + uVar2 * 8), dVar1 <= 0.0) {
      uVar2 = uVar2 + 1;
      dVar4 = dVar4 + (double)((ulong)dVar1 ^ DAT_00abd200);
      if (uVar3 < uVar2) {
        return dVar4;
      }
    }
    uVar2 = uVar2 + 1;
    dVar4 = dVar4 + dVar1;
  } while (uVar2 <= uVar3);
  return dVar4;
}


