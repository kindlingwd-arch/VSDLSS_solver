/* driver: _LMVqRMlLABtyIPrLjLKDtB_XSPAIPP addr=009807f0 size=84 */
#include "vsdlss_ref.h"


void _LMVqRMlLABtyIPrLjLKDtB_XSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5)

{
  undefined8 *puVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  
  param_2 = param_2 - param_1;
  lVar2 = *(long *)(param_4 + 0x58);
  lVar3 = *(long *)(*(long *)(param_4 + 0x48) + param_1 * 8);
  if (param_3 <= lVar3) {
    lVar6 = param_3 * 8;
    lVar7 = 0;
    do {
      if (-1 < param_2) {
        lVar4 = 1;
        lVar5 = lVar6;
        do {
          puVar1 = (undefined8 *)(*(long *)(lVar2 + param_1 * 8 + -8 + lVar4 * 8) + lVar5);
          lVar5 = lVar5 + -8;
          *(undefined8 *)(param_5 + lVar7 * 8 + lVar4 * 8) = *puVar1;
          lVar4 = lVar4 + 1;
        } while (lVar4 != param_2 + 2);
        lVar7 = param_2 + 1 + lVar7;
      }
      param_3 = param_3 + 1;
      lVar6 = lVar6 + 8;
    } while (param_3 != lVar3 + 1);
  }
  return;
}


