/* driver: yAArLbBPQtB_PZVaLIPXSPAIPP addr=0097ea00 size=b7 */
#include "vsdlss_ref.h"


void yAArLbBPQtB_PZVaLIPXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  long lVar1;
  long *plVar2;
  double *pdVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  
  if (param_1 == param_3) {
    if (0 < param_4) {
      plVar6 = (long *)(param_6 + 8);
      plVar2 = plVar6 + param_4;
      do {
        lVar4 = *plVar6;
        if (0 < param_1) {
          lVar5 = 1;
          do {
            *(double *)(lVar4 + lVar5 * 8) =
                 *(double *)(lVar4 + lVar5 * 8) + *(double *)(param_5 + lVar5 * 8);
            lVar5 = lVar5 + 1;
          } while (lVar5 != param_1 + 1);
          param_5 = param_5 + param_1 * 8;
        }
        plVar6 = plVar6 + 1;
      } while (plVar6 != plVar2);
      return;
    }
  }
  else {
    if (param_4 < 1) {
      return;
    }
    plVar6 = (long *)(param_6 + 8);
    plVar2 = plVar6 + param_4;
    do {
      lVar4 = *plVar6;
      if (0 < param_1) {
        lVar5 = 1;
        do {
          pdVar3 = (double *)(lVar4 + *(long *)(param_2 + lVar5 * 8) * 8);
          lVar1 = lVar5 * 8;
          lVar5 = lVar5 + 1;
          *pdVar3 = *pdVar3 + *(double *)(param_5 + lVar1);
        } while (lVar5 != param_1 + 1);
        param_5 = param_5 + param_1 * 8;
      }
      plVar6 = plVar6 + 1;
    } while (plVar6 != plVar2);
  }
  return;
}


