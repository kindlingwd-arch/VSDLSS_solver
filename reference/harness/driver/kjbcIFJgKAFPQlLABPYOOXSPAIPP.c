/* driver: kjbcIFJgKAFPQlLABPYOOXSPAIPP addr=009b41c0 size=615 */
#include "vsdlss_ref.h"


void kjbcIFJgKAFPQlLABPYOOXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6,
               long param_7,long *param_8,long *param_9,long *param_10)

{
  long *plVar1;
  ulong *puVar2;
  long *plVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  bool bVar9;
  long *plVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long *plVar15;
  long lVar16;
  long lVar17;
  long *plVar18;
  long *plVar19;
  long local_90;
  long local_78;
  long local_48;
  
  lVar13 = *(long *)(param_6 + 0x60);
  local_48 = *param_9;
  lVar4 = param_10[2];
  lVar14 = *param_10;
  lVar5 = *(long *)(param_6 + 0x50);
  local_90 = param_1;
  if (param_2 != 1) {
    local_90 = *(long *)(param_4 + -8 + param_2 * 8);
  }
  *(long *)(lVar4 + param_1 * 8) = lVar14;
  *(long *)(param_7 + param_1 * 8) = local_90;
  plVar1 = (long *)(lVar5 + param_1 * 8);
  plVar18 = (long *)(param_1 * 8 + lVar13);
  plVar10 = (long *)(*plVar18 + 8);
  plVar19 = plVar10 + *plVar1;
  if (0 < *plVar1) {
    do {
      lVar16 = *plVar10;
      lVar11 = lVar16 * 8;
      plVar3 = (long *)(param_5 + lVar11);
      if ((*plVar3 + 0x3ffffff9U < 0x3ffffff9) &&
         (plVar15 = (long *)(lVar11 + lVar4), *plVar15 < lVar14)) {
        *plVar15 = lVar14;
        *(long *)(param_7 + lVar16 * 8) = local_90;
        *plVar3 = -*plVar3;
      }
      plVar10 = plVar10 + 1;
    } while (plVar10 != plVar19);
  }
  if (param_2 < 2) {
    *(undefined8 *)(param_7 + local_90 * 8) = 0;
    local_48 = local_48 + 1;
    *(long *)(param_5 + param_1 * 8) = local_48;
    *plVar18 = param_4;
    *plVar1 = param_3;
  }
  else {
    plVar19 = (long *)(param_4 + 8);
    plVar10 = plVar19;
    do {
      lVar11 = *plVar10;
      plVar10 = plVar10 + 1;
      *(long *)(lVar4 + lVar11 * 8) = lVar14;
      *(long *)(param_7 + lVar11 * 8) = local_90;
    } while (plVar10 != (long *)(param_4 + param_2 * 8));
    *(undefined8 *)(param_7 + local_90 * 8) = 0;
    lVar11 = local_48 + 1;
    *(long *)(param_5 + param_1 * 8) = lVar11;
    *plVar18 = param_4;
    *plVar1 = param_3;
    lVar16 = param_3;
    do {
      lVar16 = lVar16 + -1;
      lVar12 = *plVar19;
      lVar11 = lVar11 + 1;
      *(long *)(param_5 + lVar12 * 8) = lVar11;
      *(long **)(lVar13 + lVar12 * 8) = plVar19;
      plVar19 = plVar19 + 1;
      *(long *)(lVar5 + lVar12 * 8) = lVar16;
    } while (lVar11 != param_2 + local_48);
    local_48 = local_48 + param_2;
  }
  local_78 = local_90 * 8;
  *(long *)(param_5 + local_78) = -*(long *)(param_5 + local_78);
  lVar11 = param_2;
  if (param_3 < param_2) {
    RKJyOHaLRKQFKDkyOHXSPAIPP(param_10);
  }
  else {
    do {
      while( true ) {
        puVar2 = (ulong *)(lVar4 + *(long *)(param_4 + lVar11 * 8) * 8);
        if ((long)*puVar2 < lVar14) break;
        lVar11 = lVar11 + 1;
        KOBOOLOXSPAIPP(1,"mldmmd.c",0x341);
        *puVar2 = ~*puVar2;
        if (param_3 < lVar11) goto LAB_009b43df;
      }
      lVar11 = lVar11 + 1;
      *puVar2 = ~*puVar2;
    } while (lVar11 <= param_3);
LAB_009b43df:
    lVar11 = param_2;
    do {
      while( true ) {
        lVar12 = *(long *)(param_4 + lVar11 * 8);
        lVar16 = lVar12 * 8;
        lVar12 = *(long *)(param_5 + lVar12 * 8);
        plVar1 = (long *)(param_7 + lVar16);
        plVar19 = (long *)(lVar5 + lVar16);
        lVar17 = *plVar1;
        lVar6 = *plVar19;
        if (lVar17 < 0) {
          KOBOOLOXSPAIPP(1,"mldmmd.c",0x34d);
          lVar17 = *plVar1;
        }
        if (((lVar6 < 0xccccccd) || ((double)lVar6 * 0.0 <= (double)(lVar17 + 1 + param_3))) ||
           (lVar12 != -0x7ffffff4)) break;
        *plVar1 = lVar17 + param_3 + 1;
LAB_009b4428:
        lVar11 = lVar11 + 1;
        lVar16 = param_2;
        if (param_3 < lVar11) goto LAB_009b4590;
      }
      if (0 < lVar17) {
        *plVar1 = -0x7ffffff5;
        goto LAB_009b4428;
      }
      *plVar1 = 0;
      lVar16 = *(long *)(lVar13 + lVar16);
      if (lVar6 < 1) {
        *plVar19 = 0;
      }
      else {
        lVar17 = 0;
        lVar12 = 1;
        bVar9 = true;
        do {
          while( true ) {
            lVar7 = *(long *)(lVar16 + lVar12 * 8);
            lVar8 = *(long *)(lVar4 + lVar7 * 8);
            if (lVar8 < lVar14) break;
            if (bVar9) {
              lVar17 = lVar17 + 1;
              bVar9 = false;
              *(long *)(lVar16 + lVar17 * 8) = local_90;
            }
LAB_009b44ef:
            lVar12 = lVar12 + 1;
            if (lVar12 == lVar6 + 1) goto LAB_009b451c;
          }
          if (lVar8 < 0) goto LAB_009b44ef;
          lVar12 = lVar12 + 1;
          lVar17 = lVar17 + 1;
          *(long *)(lVar16 + lVar17 * 8) = lVar7;
        } while (lVar12 != lVar6 + 1);
LAB_009b451c:
        *plVar19 = lVar17;
        if (!bVar9) goto LAB_009b4428;
      }
      lVar11 = lVar11 + 1;
      KOBOOLOXSPAIPP(1,"mldmmd.c",0x378);
      lVar16 = param_2;
    } while (lVar11 <= param_3);
LAB_009b4590:
    do {
      lVar13 = lVar16 + 1;
      puVar2 = (ulong *)(lVar4 + *(long *)(param_4 + lVar16 * 8) * 8);
      *puVar2 = ~*puVar2;
      lVar16 = lVar13;
    } while (lVar13 != param_3 + 1);
    RKJyOHaLRKQFKDkyOHXSPAIPP(param_10);
    do {
      while( true ) {
        lVar13 = *(long *)(param_4 + param_2 * 8);
        lVar4 = *(long *)(param_5 + lVar13 * 8);
        if (*(long *)(param_7 + lVar13 * 8) == -0x7ffffff5) break;
LAB_009b45d0:
        param_2 = param_2 + 1;
        if (param_3 < param_2) goto LAB_009b46a0;
      }
      lVar14 = kjbsMAyQBlLABYAGYOOyVXSPAIPP(lVar13,param_5,param_6,param_7,param_10);
      lVar5 = *(long *)(param_8[7] + lVar13 * 8);
      if ((0 < lVar5) && (lVar5 <= *param_8 * 2)) {
        lVar14 = lVar14 + (-0x7ffffff3 - lVar4);
        OBJdOLJZR_HBQZMNXSPAIPP(lVar13,param_8);
        plVar19 = (long *)(lVar13 * 8 + param_8[8]);
        plVar1 = (long *)(param_8[6] + lVar14 * 8);
        *plVar19 = *plVar1;
        lVar4 = param_8[7];
        *(long *)(lVar4 + lVar13 * 8) = lVar14 + *param_8;
        *plVar1 = lVar13;
        lVar14 = *plVar19;
        if (param_8[4] != lVar14) {
          *(long *)(lVar4 + lVar14 * 8) = lVar13;
        }
        param_8[2] = param_8[2] + 1;
        goto LAB_009b45d0;
      }
      if (lVar5 != param_8[4]) goto LAB_009b45d0;
      param_2 = param_2 + 1;
      KOBOOLOXSPAIPP(1,"mldmmd.c",0x392);
    } while (param_2 <= param_3);
  }
LAB_009b46a0:
  *param_9 = local_48;
  return;
}


