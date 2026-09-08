/* driver: LKBlLOJXSPAIPP addr=00960570 size=4c */
#include "vsdlss_ref.h"


double LKBlLOJXSPAIPP(long param_1,ulong param_2)

{
  double dVar1;
  ulong uVar2;
  double dVar3;
  
  if (param_2 == 0) {
    return 0.0;
  }
  uVar2 = 1;
  dVar3 = 0.0;
  do {
    while (dVar1 = *(double *)(param_1 + uVar2 * 8), dVar1 <= 0.0) {
      uVar2 = uVar2 + 1;
      dVar3 = dVar3 + (double)((ulong)dVar1 ^ DAT_00abd200);
      if (param_2 < uVar2) {
        return dVar3;
      }
    }
    uVar2 = uVar2 + 1;
    dVar3 = dVar3 + dVar1;
  } while (uVar2 <= param_2);
  return dVar3;
}


