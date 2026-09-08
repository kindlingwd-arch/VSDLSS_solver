/* driver: kjbcIFJgKAFPQlLABPYOOqQRzXSPAIPP addr=009b4db0 size=464 */
#include "vsdlss_ref.h"


void kjbcIFJgKAFPQlLABPYOOqQRzXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6,
               long param_7,long *param_8,long *param_9)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  bool bVar9;
  long *plVar10;
  long lVar11;
  long lVar12;
  long *plVar13;
  long *plVar14;
  long lVar15;
  long lVar16;
  long *plVar17;
  long local_90;
  long local_78;
  long local_50;
  
  lVar3 = *(long *)(param_6 + 0x60);
  local_50 = *param_8;
  lVar4 = param_9[2];
  lVar5 = *(long *)(param_6 + 0x50);
  lVar6 = *param_9;
  local_90 = param_1;
  if (param_2 != 1) {
    local_90 = *(long *)(param_4 + -8 + param_2 * 8);
  }
  *(long *)(lVar4 + param_1 * 8) = lVar6;
  *(long *)(param_7 + param_1 * 8) = local_90;
  plVar1 = (long *)(lVar5 + param_1 * 8);
  plVar17 = (long *)(param_1 * 8 + lVar3);
  plVar10 = (long *)(*plVar17 + 8);
  plVar14 = plVar10 + *plVar1;
  if (0 < *plVar1) {
    do {
      lVar15 = *plVar10;
      lVar11 = lVar15 * 8;
      plVar2 = (long *)(param_5 + lVar11);
      if ((*plVar2 + 0x3ffffff9U < 0x3ffffff9) &&
         (plVar13 = (long *)(lVar11 + lVar4), *plVar13 < lVar6)) {
        *plVar13 = lVar6;
        *(long *)(param_7 + lVar15 * 8) = local_90;
        *plVar2 = -*plVar2;
      }
      plVar10 = plVar10 + 1;
    } while (plVar10 != plVar14);
  }
  if (param_2 < 2) {
    *(undefined8 *)(param_7 + local_90 * 8) = 0;
    local_50 = local_50 + 1;
    *(long *)(param_5 + param_1 * 8) = local_50;
    *plVar17 = param_4;
    *plVar1 = param_3;
  }
  else {
    plVar14 = (long *)(param_4 + 8);
    plVar10 = plVar14;
    do {
      lVar11 = *plVar10;
      plVar10 = plVar10 + 1;
      *(long *)(lVar4 + lVar11 * 8) = lVar6;
      *(long *)(param_7 + lVar11 * 8) = local_90;
    } while (plVar10 != (long *)(param_4 + param_2 * 8));
    *(undefined8 *)(param_7 + local_90 * 8) = 0;
    lVar11 = local_50 + 1;
    *(long *)(param_5 + param_1 * 8) = lVar11;
    *plVar17 = param_4;
    *plVar1 = param_3;
    lVar15 = param_3;
    do {
      lVar15 = lVar15 + -1;
      lVar12 = *plVar14;
      lVar11 = lVar11 + 1;
      *(long *)(param_5 + lVar12 * 8) = lVar11;
      *(long **)(lVar3 + lVar12 * 8) = plVar14;
      plVar14 = plVar14 + 1;
      *(long *)(lVar5 + lVar12 * 8) = lVar15;
    } while (lVar11 != param_2 + local_50);
    local_50 = local_50 + param_2;
  }
  local_78 = local_90 * 8;
  *(long *)(param_5 + local_78) = -*(long *)(param_5 + local_78);
  if (param_3 < param_2) {
    RKJyOHaLRKQFKDkyOHXSPAIPP(param_9);
  }
  else {
    lVar11 = param_2;
    do {
      lVar15 = lVar11 * 8;
      lVar11 = lVar11 + 1;
      *(long *)(lVar4 + *(long *)(param_4 + lVar15) * 8) = lVar6;
      lVar15 = param_2;
    } while (lVar11 != param_3 + 1);
    do {
      while( true ) {
        lVar12 = *(long *)(param_4 + lVar15 * 8);
        lVar11 = lVar12 * 8;
        lVar12 = *(long *)(param_5 + lVar12 * 8);
        plVar1 = (long *)(lVar5 + lVar11);
        plVar14 = (long *)(param_7 + lVar11);
        lVar16 = *plVar14;
        lVar7 = *plVar1;
        if (lVar16 < 0) {
          KOBOOLOXSPAIPP(1,"mldmmd.c",0x4c6);
          lVar16 = *plVar14;
        }
        if (((lVar7 < 0x81) || ((double)lVar7 <= (double)(lVar16 + 1 + param_3))) ||
           (lVar12 != -0x7ffffff4)) break;
        *plVar14 = lVar16 + param_3 + 1;
LAB_009b4fa7:
        lVar15 = lVar15 + 1;
        if (param_3 < lVar15) goto LAB_009b50d8;
      }
      if (0 < lVar16) {
        *plVar14 = -0x7ffffff5;
        goto LAB_009b4fa7;
      }
      *plVar14 = 0;
      lVar11 = *(long *)(lVar3 + lVar11);
      if (lVar7 < 1) {
        *plVar1 = 0;
      }
      else {
        lVar16 = 0;
        lVar12 = 1;
        bVar9 = true;
        do {
          while (lVar8 = *(long *)(lVar11 + lVar12 * 8), lVar6 <= *(long *)(lVar4 + lVar8 * 8)) {
            if (bVar9) {
              lVar16 = lVar16 + 1;
              bVar9 = false;
              *(long *)(lVar11 + lVar16 * 8) = local_90;
            }
            lVar12 = lVar12 + 1;
            if (lVar12 == lVar7 + 1) goto LAB_009b5084;
          }
          lVar12 = lVar12 + 1;
          lVar16 = lVar16 + 1;
          *(long *)(lVar11 + lVar16 * 8) = lVar8;
        } while (lVar12 != lVar7 + 1);
LAB_009b5084:
        *plVar1 = lVar16;
        if (!bVar9) goto LAB_009b4fa7;
      }
      lVar15 = lVar15 + 1;
      KOBOOLOXSPAIPP(1,"mldmmd.c",0x4ef);
    } while (lVar15 <= param_3);
LAB_009b50d8:
    RKJyOHaLRKQFKDkyOHXSPAIPP(param_9);
    do {
      while (lVar3 = *(long *)(param_4 + param_2 * 8), *(long *)(param_7 + lVar3 * 8) != -0x7ffffff5
            ) {
        param_2 = param_2 + 1;
        if (param_3 < param_2) goto LAB_009b5148;
      }
      param_2 = param_2 + 1;
      kjbsMAyQBlLABYAGYOOyVXSPAIPP(lVar3,param_5,param_6,param_7,param_9);
    } while (param_2 <= param_3);
  }
LAB_009b5148:
  *param_8 = local_50;
  return;
}


