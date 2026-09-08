/* driver: DBQkFKYKAkyUgtB_QLOXSPAIPP addr=00961300 size=89 */
#include "vsdlss_ref.h"


void DBQkFKYKAkyUgtB_QLOXSPAIPP
               (long param_1,long param_2,long *param_3,long *param_4,long *param_5,long *param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  
  if (param_2 < 1) {
    lVar9 = 0;
    lVar6 = 0;
    lVar4 = 0;
    lVar8 = 0;
  }
  else {
    lVar4 = *(long *)(param_1 + 8);
    lVar8 = lVar4;
    if (param_2 == 1) {
      lVar9 = 1;
      lVar6 = 1;
    }
    else {
      lVar2 = 2;
      lVar5 = 1;
      lVar9 = 1;
      do {
        while ((lVar3 = lVar2, lVar1 = *(long *)(param_1 + lVar3 * 8), lVar6 = lVar3, lVar7 = lVar1,
               lVar1 < lVar8 || (lVar6 = lVar5, lVar7 = lVar8, lVar1 <= lVar4))) {
          lVar2 = lVar3 + 1;
          lVar5 = lVar6;
          lVar8 = lVar7;
          if (lVar3 + 1 == param_2 + 1) goto LAB_00961360;
        }
        lVar2 = lVar3 + 1;
        lVar4 = lVar1;
        lVar9 = lVar3;
      } while (lVar3 + 1 != param_2 + 1);
    }
  }
LAB_00961360:
  *param_3 = lVar8;
  *param_4 = lVar4;
  *param_5 = lVar6;
  *param_6 = lVar9;
  return;
}


