/* driver: zR_HBQlLABPZVbBDOBBXSPAIPP addr=00997bd0 size=cb */
#include "vsdlss_ref.h"


void zR_HBQlLABPZVbBDOBBXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long *plVar4;
  long lVar5;
  
  FKFQgtB_QLOXSPAIPP(param_6,param_1,0);
  plVar2 = (long *)(param_4 + 8);
  plVar4 = plVar2 + param_2;
  if (0 < param_2) {
    do {
      lVar5 = *plVar2 + 1;
      if (param_1 < *plVar2 + 1) {
        lVar5 = param_1;
      }
      plVar2 = plVar2 + 1;
      plVar1 = (long *)(param_6 + lVar5 * 8);
      *plVar1 = *plVar1 + 1;
    } while (plVar2 != plVar4);
  }
  if (0 < param_1) {
    plVar2 = (long *)(param_6 + 8);
    lVar5 = 1;
    plVar4 = plVar2 + param_1;
    do {
      lVar3 = *plVar2;
      *plVar2 = lVar5;
      plVar2 = plVar2 + 1;
      lVar5 = lVar5 + lVar3;
    } while (plVar2 != plVar4);
  }
  plVar4 = (long *)(param_3 + 8);
  plVar2 = plVar4 + param_2;
  if (0 < param_2) {
    do {
      lVar5 = *plVar4;
      lVar3 = *(long *)(param_4 + lVar5 * 8) + 1;
      if (param_1 < lVar3) {
        lVar3 = param_1;
      }
      plVar4 = plVar4 + 1;
      plVar1 = (long *)(param_6 + lVar3 * 8);
      *(long *)(param_5 + *plVar1 * 8) = lVar5;
      *plVar1 = *plVar1 + 1;
    } while (plVar4 != plVar2);
  }
  return;
}


