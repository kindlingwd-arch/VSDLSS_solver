/* driver: kjbZRFIAZMNYOOsPBky_OLXSPAIPP addr=009b38c0 size=19e */
#include "vsdlss_ref.h"


void kjbZRFIAZMNYOOsPBky_OLXSPAIPP
               (long param_1,long param_2,long param_3,long *param_4,undefined8 param_5)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long *plVar8;
  long lVar9;
  
  lVar3 = *(long *)(param_1 + 0x10);
  lVar9 = *(long *)(param_1 + 0x50);
  if (0 < lVar3) {
    if (*(long *)(param_2 + 8) == -0x7ffffff4) {
      lVar5 = 1;
      do {
        lVar5 = lVar5 + 1;
        if (lVar3 < lVar5) {
          lVar5 = param_4[8];
          lVar7 = param_4[6];
          lVar4 = param_4[7];
          lVar6 = 1;
          do {
            lVar1 = *(long *)(lVar9 + lVar6 * 8) + 1;
            plVar2 = (long *)(lVar7 + lVar1 * 8);
            *(long *)(lVar5 + lVar6 * 8) = *plVar2;
            *(long *)(lVar4 + lVar6 * 8) = lVar1 + *param_4;
            *plVar2 = lVar6;
            lVar1 = *(long *)(lVar5 + lVar6 * 8);
            if (param_4[4] != lVar1) {
              *(long *)(lVar4 + lVar1 * 8) = lVar6;
            }
            lVar6 = lVar6 + 1;
            param_4[2] = param_4[2] + 1;
          } while (lVar6 != lVar3 + 1);
          return;
        }
      } while (*(long *)(param_2 + lVar5 * 8) == -0x7ffffff4);
    }
    lVar9 = 1;
    do {
      lVar5 = *(long *)(param_2 + lVar9 * 8);
      if ((lVar5 < -0x3ffffff9) && (lVar5 != -0x7ffffff5)) {
        if (*(long *)(param_3 + lVar9 * 8) < 1) {
          lVar7 = kjbdFKAbBDgKcIFJeOyMEYOOXSPAIPP(lVar9,param_2,param_1,param_5);
          lVar7 = lVar7 + (-0x7ffffff4 - lVar5);
        }
        else {
          lVar7 = kjbsMAyQBlLABYAGYOOyVXSPAIPP(lVar9,param_2,param_1,param_3,param_5);
          lVar7 = lVar7 + (-0x7ffffff4 - lVar5);
        }
        plVar8 = (long *)(lVar9 * 8 + param_4[8]);
        plVar2 = (long *)(param_4[6] + (lVar7 + 1) * 8);
        *plVar8 = *plVar2;
        lVar5 = param_4[7];
        *(long *)(lVar5 + lVar9 * 8) = lVar7 + 1 + *param_4;
        *plVar2 = lVar9;
        lVar7 = *plVar8;
        if (param_4[4] != lVar7) {
          *(long *)(lVar5 + lVar7 * 8) = lVar9;
        }
        param_4[2] = param_4[2] + 1;
      }
      lVar9 = lVar9 + 1;
    } while (lVar9 <= lVar3);
  }
  return;
}


