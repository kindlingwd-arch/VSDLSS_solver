/* driver: bgKKBOnOLAR_QXSPAIPP addr=00960610 size=43 */
#include "vsdlss_ref.h"


double bgKKBOnOLAR_QXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  double dVar3;
  long lVar4;
  double dVar5;
  
  if (0 < param_4) {
    dVar5 = 0.0;
    lVar4 = 1;
    do {
      dVar3 = *(double *)(param_1 + lVar4 * 8);
      lVar1 = lVar4 * 8;
      lVar2 = lVar4 * 8;
      lVar4 = lVar4 + 1;
      dVar5 = dVar5 + *(double *)(param_2 + lVar1) * dVar3 * dVar3 * *(double *)(param_3 + lVar2);
    } while (lVar4 != param_4 + 1);
    return dVar5;
  }
  return 0.0;
}


