/* driver: kjbdFKAbBDgKcIFJeOyMEYOOXSPAIPP addr=009b3730 size=187 */
#include "vsdlss_ref.h"


long kjbdFKAbBDgKcIFJeOyMEYOOXSPAIPP(long param_1,long param_2,long param_3,long *param_4)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  
  lVar3 = param_4[2];
  lVar4 = *param_4;
  lVar5 = *(long *)(param_3 + 0x50);
  lVar6 = *(long *)(lVar5 + param_1 * 8);
  lVar7 = *(long *)(param_3 + 0x60);
  lVar8 = *(long *)(lVar7 + param_1 * 8);
  *(long *)(lVar3 + param_1 * 8) = lVar4;
  if (lVar6 < 1) {
    lVar13 = 0;
  }
  else {
    lVar12 = 1;
    lVar13 = 0;
LAB_009b37ae:
    do {
      lVar9 = *(long *)(lVar8 + lVar12 * 8);
      lVar2 = lVar9 * 8;
      lVar9 = *(long *)(param_2 + lVar9 * 8);
      if (lVar9 < -0x3ffffff9) {
        if (*(long *)(lVar2 + lVar3) < lVar4) {
          *(long *)(lVar2 + lVar3) = lVar4;
          lVar13 = lVar13 + 1;
        }
      }
      else if (lVar9 + 0x3ffffff9U < 0x3ffffff9) {
        lVar9 = *(long *)(lVar7 + lVar2);
        lVar2 = *(long *)(lVar5 + lVar2);
        if (0 < lVar2) {
          lVar11 = 1;
          do {
            while (lVar10 = *(long *)(lVar9 + lVar11 * 8),
                  -0x3ffffffa < *(long *)(param_2 + lVar10 * 8)) {
              lVar11 = lVar11 + 1;
              KOBOOLOXSPAIPP(1,"mldmmd.c",0x121);
              if (lVar2 < lVar11) goto LAB_009b3868;
            }
            plVar1 = (long *)(lVar3 + lVar10 * 8);
            if (*plVar1 < lVar4) {
              *plVar1 = lVar4;
              lVar13 = lVar13 + 1;
            }
            lVar11 = lVar11 + 1;
          } while (lVar11 <= lVar2);
LAB_009b3868:
          lVar12 = lVar12 + 1;
          if (lVar6 < lVar12) break;
          goto LAB_009b37ae;
        }
      }
      else {
        KOBOOLOXSPAIPP(1,"mldmmd.c",0x128);
      }
      lVar12 = lVar12 + 1;
    } while (lVar12 <= lVar6);
  }
  RKJyOHaLRKQFKDkyOHXSPAIPP(param_4);
  return lVar13;
}


