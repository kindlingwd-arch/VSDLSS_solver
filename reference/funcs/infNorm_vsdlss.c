/* VSDLSS function (decompiled by Ghidra) */
/* name: infNorm_vsdlss  addr: 009d7ec0  size: 83 */
#include "vsdlss_ref.h"

/* signature: double infNorm_vsdlss(long param_1,int param_2); */

double infNorm_vsdlss(long param_1,int param_2)

{
  double dVar1;
  ulong uVar2;
  ulong uVar3;
  double dVar4;
  double dVar5;
  
  if (param_2 < 1) {
    dVar4 = 0.0;
  }
  else {
    dVar4 = *(double *)(param_1 + 8);
    if (dVar4 <= 0.0) {
      dVar4 = (double)((ulong)dVar4 ^ DAT_00abd200);
    }
    uVar3 = (ulong)param_2;
    if (uVar3 != 1) {
      uVar2 = 2;
      do {
        while( true ) {
          dVar1 = *(double *)(param_1 + uVar2 * 8);
          dVar5 = dVar1;
          if (dVar1 <= 0.0) {
            dVar5 = (double)((ulong)dVar1 ^ DAT_00abd200);
          }
          if ((dVar5 <= dVar4) || (dVar4 = dVar1, 0.0 < dVar1)) break;
          uVar2 = uVar2 + 1;
          dVar4 = (double)((ulong)dVar1 ^ DAT_00abd200);
          if (uVar3 < uVar2) {
            return dVar4;
          }
        }
        uVar2 = uVar2 + 1;
      } while (uVar2 <= uVar3);
      return dVar4;
    }
  }
  return dVar4;
}


