/* driver: kjbdFKApBy_EYOOXSPAIPP addr=009b3a70 size=1ce */
#include "vsdlss_ref.h"


void kjbdFKApBy_EYOOXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long *param_6)

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
  long *plVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  
  lVar13 = 0;
  lVar1 = *(long *)(param_3 + 0x60);
  lVar2 = param_6[2];
  lVar3 = *param_6;
  lVar4 = *(long *)(lVar1 + param_1 * 8);
  lVar5 = *(long *)(param_3 + 0x50);
  lVar6 = *(long *)(lVar5 + param_1 * 8);
  *(long *)(lVar2 + param_1 * 8) = lVar3;
  if (0 < lVar6) {
    lVar12 = 1;
LAB_009b3b07:
    do {
      lVar7 = *(long *)(lVar4 + lVar12 * 8);
      lVar8 = *(long *)(param_2 + lVar7 * 8);
      if (lVar8 < -0x3ffffff9) {
        plVar10 = (long *)(lVar7 * 8 + lVar2);
        if (*plVar10 < lVar3) {
          *plVar10 = lVar3;
          lVar13 = lVar13 + 1;
          *(long *)(param_5 + lVar13 * 8) = lVar7;
        }
      }
      else if (lVar8 + 0x3ffffff9U < 0x3ffffff9) {
        lVar8 = *(long *)(lVar5 + lVar7 * 8);
        lVar7 = *(long *)(lVar1 + lVar7 * 8);
        if (0 < lVar8) {
          lVar11 = 1;
          do {
            while( true ) {
              lVar9 = *(long *)(lVar7 + lVar11 * 8);
              if (*(long *)(param_2 + lVar9 * 8) < -0x3ffffff9) break;
              lVar11 = lVar11 + 1;
              KOBOOLOXSPAIPP(1,"mldmmd.c",0x1aa);
              if (lVar8 < lVar11) goto LAB_009b3bd0;
            }
            plVar10 = (long *)(lVar9 * 8 + lVar2);
            if (*plVar10 < lVar3) {
              lVar13 = lVar13 + 1;
              *plVar10 = lVar3;
              *(long *)(param_5 + lVar13 * 8) = lVar9;
            }
            lVar11 = lVar11 + 1;
          } while (lVar11 <= lVar8);
LAB_009b3bd0:
          lVar12 = lVar12 + 1;
          if (lVar6 < lVar12) break;
          goto LAB_009b3b07;
        }
      }
      else {
        KOBOOLOXSPAIPP(1,"mldmmd.c",0x1b1);
      }
      lVar12 = lVar12 + 1;
    } while (lVar12 <= lVar6);
  }
  RKJyOHaLRKQFKDkyOHXSPAIPP(param_6);
  if (lVar13 != param_4) {
    KOBOOLOXSPAIPP(1,"mldmmd.c",0x1ba);
    return;
  }
  return;
}


