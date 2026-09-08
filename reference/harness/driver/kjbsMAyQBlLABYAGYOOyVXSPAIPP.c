/* driver: kjbsMAyQBlLABYAGYOOyVXSPAIPP addr=009b3460 size=2b3 */
#include "vsdlss_ref.h"


long kjbsMAyQBlLABYAGYOOyVXSPAIPP(long param_1,long param_2,long param_3,long param_4,long *param_5)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long *plVar8;
  long *plVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long *plVar14;
  long lVar15;
  
  lVar13 = *(long *)(param_3 + 0x60);
  lVar11 = *(long *)(param_3 + 0x50);
  lVar2 = *(long *)(lVar13 + param_1 * 8);
  lVar3 = param_5[2];
  if (-0x3ffffffa < *(long *)(param_2 + param_1 * 8)) {
    KOBOOLOXSPAIPP(1,"mldmmd.c",0x9b);
  }
  plVar9 = (long *)(param_1 * 8 + lVar11);
  lVar4 = *param_5;
  *(long *)(lVar3 + param_1 * 8) = lVar4;
  lVar5 = *plVar9;
  if (lVar5 < 1) {
    lVar13 = 0;
    lVar15 = 0;
  }
  else {
    lVar10 = 1;
    lVar12 = 0;
    lVar15 = 0;
    do {
      while (lVar7 = *(long *)(lVar2 + lVar10 * 8), *(long *)(param_2 + lVar7 * 8) < -0x3ffffff9) {
        lVar12 = lVar12 + 1;
        *(long *)(lVar2 + lVar12 * 8) = lVar7;
LAB_009b34f8:
        lVar10 = lVar10 + 1;
        if (lVar5 < lVar10) goto LAB_009b35c0;
      }
      lVar7 = kjbdFKAuFQEnyQEaLJMOBPPFLKXSPAIPP(lVar7,param_4);
      if (0x3ffffff8 < *(long *)(param_2 + lVar7 * 8) + 0x3ffffff9U) {
        KOBOOLOXSPAIPP(1,"mldmmd.c",0xb0);
      }
      lVar6 = *(long *)(lVar11 + lVar7 * 8);
      if ((lVar6 < 2) || (plVar14 = (long *)(lVar7 * 8 + lVar3), lVar4 <= *plVar14))
      goto LAB_009b34f8;
      lVar12 = lVar12 + 1;
      *plVar14 = lVar4;
      *(long *)(lVar2 + lVar12 * 8) = lVar7;
      plVar8 = (long *)(*(long *)(lVar13 + lVar7 * 8) + 8);
      plVar14 = plVar8 + lVar6;
      do {
        plVar1 = (long *)(lVar3 + *plVar8 * 8);
        if (*plVar1 < lVar4) {
          *plVar1 = lVar4;
          lVar15 = lVar15 + 1;
        }
        plVar8 = plVar8 + 1;
      } while (plVar8 != plVar14);
      lVar10 = lVar10 + 1;
    } while (lVar10 <= lVar5);
LAB_009b35c0:
    *plVar9 = lVar12;
    if (lVar12 == 0) {
      lVar13 = 0;
    }
    else {
      lVar13 = 0;
      lVar11 = 1;
      do {
        while( true ) {
          lVar5 = *(long *)(lVar2 + lVar11 * 8);
          lVar10 = *(long *)(param_2 + lVar5 * 8);
          if (-0x3ffffffa < lVar10) break;
          if (*(long *)(lVar3 + lVar5 * 8) < lVar4) {
            lVar13 = lVar13 + 1;
            lVar15 = lVar15 + 1;
            *(long *)(lVar2 + lVar13 * 8) = lVar5;
          }
LAB_009b360a:
          lVar11 = lVar11 + 1;
          if (lVar12 < lVar11) goto LAB_009b364b;
        }
        if (0x3ffffff8 < lVar10 + 0x3ffffff9U) {
          KOBOOLOXSPAIPP(1,"mldmmd.c",0xdb);
          goto LAB_009b360a;
        }
        lVar11 = lVar11 + 1;
        lVar13 = lVar13 + 1;
        *(long *)(lVar2 + lVar13 * 8) = lVar5;
      } while (lVar11 <= lVar12);
    }
  }
LAB_009b364b:
  *plVar9 = lVar13;
  *(undefined8 *)(param_4 + param_1 * 8) = 0;
  RKJyOHaLRKQFKDkyOHXSPAIPP(param_5);
  return lVar15;
}


