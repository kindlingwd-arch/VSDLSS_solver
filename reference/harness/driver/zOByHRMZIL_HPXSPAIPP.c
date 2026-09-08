/* driver: zOByHRMZIL_HPXSPAIPP addr=0096e3a0 size=7f */
#include "vsdlss_ref.h"


long zOByHRMZIL_HPXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long *plVar5;
  long lVar6;
  long lVar7;
  
  if (param_2 < 1) {
    return 0;
  }
  plVar5 = (long *)(param_1 + 8);
  plVar1 = plVar5 + param_2;
  lVar3 = 0;
  do {
    while (lVar2 = *plVar5, lVar2 <= param_3) {
      plVar5 = plVar5 + 1;
      lVar3 = lVar3 + 1;
      *(long *)(param_4 + lVar3 * 8) = lVar2;
      if (plVar5 == plVar1) {
        return lVar3;
      }
    }
    lVar4 = lVar2 + -1;
    lVar6 = 1;
    if (0 < lVar2) {
      do {
        lVar7 = lVar4 + 1;
        if (param_3 <= lVar4) {
          lVar7 = param_3;
        }
        lVar6 = lVar6 + param_3;
        lVar3 = lVar3 + 1;
        lVar4 = lVar4 - param_3;
        *(long *)(param_4 + lVar3 * 8) = lVar7;
      } while (lVar6 <= lVar2);
    }
    plVar5 = plVar5 + 1;
  } while (plVar5 != plVar1);
  return lVar3;
}


