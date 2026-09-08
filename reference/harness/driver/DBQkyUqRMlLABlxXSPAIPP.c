/* driver: DBQkyUqRMlLABlxXSPAIPP addr=0096ca50 size=67 */
#include "vsdlss_ref.h"


long DBQkyUqRMlLABlxXSPAIPP(long param_1)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long *plVar5;
  long lVar6;
  
  if (0 < *(long *)(param_1 + 0x28)) {
    plVar5 = (long *)(*(long *)(param_1 + 0x40) + 8);
    lVar6 = 1;
    plVar1 = plVar5 + *(long *)(param_1 + 0x28);
    lVar3 = 0;
    do {
      lVar2 = *plVar5;
      lVar4 = (*(long *)(*(long *)(param_1 + 0x48) + lVar6 * 8) + 1) * lVar2 -
              ((lVar2 + -1) * lVar2) / 2;
      if (lVar3 < lVar4) {
        lVar3 = lVar4;
      }
      plVar5 = plVar5 + 1;
      lVar6 = lVar6 + lVar2;
    } while (plVar5 != plVar1);
    return lVar3;
  }
  return 0;
}


