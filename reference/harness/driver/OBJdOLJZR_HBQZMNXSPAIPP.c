/* driver: OBJdOLJZR_HBQZMNXSPAIPP addr=009b2650 size=5d */
#include "vsdlss_ref.h"


void OBJdOLJZR_HBQZMNXSPAIPP(long param_1,long *param_2)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long *plVar7;
  
  lVar2 = param_2[7];
  lVar3 = *param_2;
  lVar4 = param_2[4];
  plVar1 = (long *)(lVar2 + param_1 * 8);
  lVar5 = *plVar1;
  if ((0 < lVar5) && (lVar5 <= lVar3 * 2)) {
    plVar7 = (long *)(param_1 * 8 + param_2[8]);
    lVar6 = *plVar7;
    if (lVar3 < lVar5) {
      *(long *)(param_2[6] + (lVar5 - lVar3) * 8) = lVar6;
    }
    else {
      *(long *)(param_2[8] + lVar5 * 8) = lVar6;
    }
    if (lVar6 != lVar4) {
      *(long *)(lVar2 + lVar6 * 8) = lVar5;
    }
    param_2[2] = param_2[2] + -1;
    *plVar1 = lVar4;
    *plVar7 = lVar4;
  }
  return;
}


