/* driver: yAArLbBPQtB_PXSPAIPP addr=0097eac0 size=c4 */
#include "vsdlss_ref.h"


void yAArLbBPQtB_PXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  double *pdVar4;
  long *plVar5;
  long lVar6;
  
  if (param_1 == param_3) {
    if (0 < param_1) {
      lVar6 = 1;
      do {
        if (0 < param_4) {
          lVar3 = 1;
          do {
            pdVar4 = (double *)(lVar6 * 8 + *(long *)(param_6 + lVar3 * 8));
            lVar1 = lVar3 * 8;
            lVar3 = lVar3 + 1;
            *pdVar4 = *pdVar4 + *(double *)(param_5 + lVar1);
          } while (lVar3 != param_4 + 1);
          param_5 = param_5 + param_4 * 8;
        }
        lVar6 = lVar6 + 1;
      } while (lVar6 != param_1 + 1);
      return;
    }
  }
  else if (0 < param_1) {
    plVar5 = (long *)(param_2 + 8);
    plVar2 = plVar5 + param_1;
    do {
      lVar6 = *plVar5;
      if (0 < param_4) {
        lVar3 = 1;
        do {
          pdVar4 = (double *)(lVar6 * 8 + *(long *)(param_6 + lVar3 * 8));
          lVar1 = lVar3 * 8;
          lVar3 = lVar3 + 1;
          *pdVar4 = *pdVar4 + *(double *)(param_5 + lVar1);
        } while (lVar3 != param_4 + 1);
        param_5 = param_5 + param_4 * 8;
      }
      plVar5 = plVar5 + 1;
    } while (plVar5 != plVar2);
  }
  return;
}


