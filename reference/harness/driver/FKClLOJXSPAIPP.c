/* driver: FKClLOJXSPAIPP addr=009606a0 size=8c */
#include "vsdlss_ref.h"


double FKClLOJXSPAIPP(long param_1,long param_2)

{
  double dVar1;
  long lVar2;
  double dVar3;
  double dVar4;
  
  if (param_2 < 1) {
    dVar3 = 0.0;
  }
  else {
    dVar3 = *(double *)(param_1 + 8);
    if (dVar3 <= 0.0) {
      dVar3 = (double)((ulong)dVar3 ^ DAT_00abd200);
    }
    if (param_2 != 1) {
      lVar2 = 2;
      do {
        while( true ) {
          dVar1 = *(double *)(param_1 + lVar2 * 8);
          dVar4 = dVar1;
          if (dVar1 <= 0.0) {
            dVar4 = (double)((ulong)dVar1 ^ DAT_00abd200);
          }
          if ((dVar4 <= dVar3) || (dVar3 = dVar1, 0.0 < dVar1)) break;
          lVar2 = lVar2 + 1;
          dVar3 = (double)((ulong)dVar1 ^ DAT_00abd200);
          if (lVar2 == param_2 + 1) {
            return dVar3;
          }
        }
        lVar2 = lVar2 + 1;
      } while (lVar2 != param_2 + 1);
      return dVar3;
    }
  }
  return dVar3;
}


