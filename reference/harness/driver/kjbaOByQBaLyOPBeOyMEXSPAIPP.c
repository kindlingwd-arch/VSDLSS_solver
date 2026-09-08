/* driver: kjbaOByQBaLyOPBeOyMEXSPAIPP addr=00998390 size=5d1 */
#include "vsdlss_ref.h"


void kjbaOByQBaLyOPBeOyMEXSPAIPP
               (undefined8 param_1,long param_2,long param_3,long param_4,long param_5,
               undefined8 param_6,long param_7)

{
  ulong *puVar1;
  long *plVar2;
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
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  ulong uVar18;
  long lVar19;
  long *plVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  long lVar24;
  long lVar25;
  long lVar26;
  long *plVar27;
  long *plVar28;
  long *plVar29;
  long *plVar30;
  long *plVar31;
  long lVar32;
  ulong uVar33;
  long local_e0;
  
  lVar26 = *(long *)(param_3 + 0x50);
  lVar22 = *(long *)(param_3 + 0x10);
  lVar4 = *(long *)(param_3 + 0x70);
  lVar5 = *(long *)(param_3 + 0x58);
  lVar3 = (param_2 - lVar22) + *(long *)(param_3 + 0x18);
  lVar6 = *(long *)(param_3 + 0x60);
  lVar7 = *(long *)(param_3 + 0x80);
  lVar8 = *(long *)(param_3 + 0x88);
  if ((lVar4 == 0) || (lVar7 == 0)) {
    KOBOOLOXSPAIPP(1,"mldorder.c",0x643);
  }
  lVar19 = QyDlBTkjbeOyMEXSPAIPP(param_1,*(long *)(param_3 + 8) + 1,param_2,lVar3);
  lVar9 = *(long *)(lVar19 + 0x50);
  lVar10 = *(long *)(lVar19 + 0x68);
  lVar11 = *(long *)(lVar19 + 0x78);
  lVar12 = *(long *)(lVar19 + 0x58);
  lVar13 = *(long *)(lVar19 + 0x80);
  lVar14 = *(long *)(lVar19 + 0x60);
  lVar15 = *(long *)(lVar19 + 0x70);
  FKFQgtB_QLOXSPAIPP(param_7,param_2,0);
  if (lVar22 < 1) {
    lVar26 = 0;
    uVar33 = 0;
    lVar22 = 0;
LAB_009986fe:
    if (((long)uVar33 <= lVar3 * 2) && (lVar22 == param_2)) goto LAB_00998728;
  }
  else {
    plVar2 = (long *)(param_5 + 8 + lVar22 * 8);
    plVar27 = (long *)(param_5 + 8);
    lVar32 = 1;
    uVar33 = 0;
    lVar22 = 0;
    do {
      while( true ) {
        lVar16 = *plVar27;
        lVar17 = *(long *)(lVar8 + lVar16 * 8);
        if (lVar17 != lVar22 + 1) break;
        local_e0 = 0;
        *(ulong *)(lVar14 + lVar17 * 8) = lVar10 + uVar33 * 8;
        *(ulong *)(lVar15 + lVar17 * 8) = uVar33 * 8 + lVar11;
        lVar22 = *(long *)(param_4 + lVar16 * 8);
        if (lVar16 != lVar22) {
          local_e0 = *(long *)(lVar26 + lVar22 * 8);
        }
        lVar24 = *(long *)(lVar26 + lVar16 * 8);
        lVar25 = *(long *)(lVar6 + lVar16 * 8);
        plVar31 = *(long **)(lVar4 + lVar16 * 8);
        plVar28 = (long *)(lVar13 + lVar17 * 8);
        plVar20 = (long *)(lVar17 * 8 + lVar12);
        *plVar28 = *(long *)(lVar7 + lVar16 * 8);
        *plVar20 = *(long *)(lVar5 + lVar16 * 8);
        if (local_e0 + lVar24 < 0x10) {
          if (0 < lVar24) {
            plVar29 = (long *)(lVar25 + 8);
            plVar30 = plVar29 + lVar24;
            do {
              plVar31 = plVar31 + 1;
              lVar24 = *(long *)(lVar8 + *plVar29 * 8);
              if (lVar17 != lVar24) {
                if ((long)uVar33 < lVar32) {
LAB_009987ea:
                  uVar33 = uVar33 + 1;
                  *(long *)(lVar10 + uVar33 * 8) = lVar24;
                  *(long *)(lVar11 + uVar33 * 8) = *plVar31;
                }
                else {
                  lVar25 = *(long *)(lVar10 + lVar32 * 8);
                  lVar21 = lVar32 * 8;
                  lVar23 = lVar32;
                  while (lVar25 != lVar24) {
                    lVar21 = lVar21 + 8;
                    lVar23 = lVar23 + 1;
                    if ((long)uVar33 < lVar23) goto LAB_009987ea;
                    lVar25 = *(long *)(lVar10 + lVar21);
                  }
                  *(long *)(lVar11 + lVar21) = *(long *)(lVar11 + lVar21) + *plVar31;
                }
              }
              plVar29 = plVar29 + 1;
            } while (plVar29 != plVar30);
          }
          if (lVar16 != lVar22) {
            lVar16 = *(long *)(lVar6 + lVar22 * 8);
            plVar31 = *(long **)(lVar4 + lVar22 * 8);
            *plVar28 = *plVar28 + *(long *)(lVar7 + lVar22 * 8);
            *plVar20 = *plVar20 + *(long *)(lVar5 + lVar22 * 8);
            if (0 < local_e0) {
              plVar30 = (long *)(lVar16 + 8);
              plVar28 = plVar30 + local_e0;
              do {
                plVar31 = plVar31 + 1;
                lVar22 = *(long *)(lVar8 + *plVar30 * 8);
                if (lVar17 == lVar22) {
                  lVar22 = *plVar20;
                  lVar16 = *plVar31;
                  *plVar20 = lVar22 - lVar16;
                  *plVar20 = (lVar22 - lVar16) - *plVar31;
                }
                else if ((long)uVar33 < lVar32) {
LAB_009988de:
                  uVar33 = uVar33 + 1;
                  *(long *)(lVar10 + uVar33 * 8) = lVar22;
                  *(long *)(lVar11 + uVar33 * 8) = *plVar31;
                }
                else {
                  lVar16 = *(long *)(lVar10 + lVar32 * 8);
                  lVar24 = lVar32 * 8;
                  lVar25 = lVar32;
                  while (lVar16 != lVar22) {
                    lVar24 = lVar24 + 8;
                    lVar25 = lVar25 + 1;
                    if ((long)uVar33 < lVar25) goto LAB_009988de;
                    lVar16 = *(long *)(lVar10 + lVar24);
                  }
                  *(long *)(lVar11 + lVar24) = *(long *)(lVar11 + lVar24) + *plVar31;
                }
                plVar30 = plVar30 + 1;
              } while (plVar30 != plVar28);
            }
          }
        }
        else {
          lVar21 = 1;
          if (0 < lVar24) {
LAB_009985d5:
            do {
              lVar23 = *(long *)(lVar8 + *(long *)(lVar25 + lVar21 * 8) * 8);
              if (lVar17 != lVar23) {
                puVar1 = (ulong *)(param_7 + lVar23 * 8);
                uVar18 = *puVar1;
                if (lVar32 <= (long)uVar18) {
                  plVar30 = plVar31 + lVar21;
                  lVar21 = lVar21 + 1;
                  plVar29 = (long *)(lVar11 + uVar18 * 8);
                  *plVar29 = *plVar29 + *plVar30;
                  if (lVar21 == lVar24 + 1) break;
                  goto LAB_009985d5;
                }
                uVar33 = uVar33 + 1;
                *puVar1 = uVar33;
                *(long *)(lVar10 + uVar33 * 8) = lVar23;
                *(long *)(lVar11 + uVar33 * 8) = plVar31[lVar21];
              }
              lVar21 = lVar21 + 1;
            } while (lVar21 != lVar24 + 1);
          }
          if (lVar16 != lVar22) {
            lVar16 = *(long *)(lVar6 + lVar22 * 8);
            lVar24 = *(long *)(lVar4 + lVar22 * 8);
            *plVar28 = *plVar28 + *(long *)(lVar7 + lVar22 * 8);
            *plVar20 = *plVar20 + *(long *)(lVar5 + lVar22 * 8);
            if (0 < local_e0) {
              lVar22 = 1;
LAB_00998695:
              do {
                lVar25 = *(long *)(lVar8 + *(long *)(lVar16 + lVar22 * 8) * 8);
                if (lVar17 == lVar25) {
                  lVar25 = *plVar20 - *(long *)(lVar24 + lVar22 * 8);
                  *plVar20 = lVar25;
                  *plVar20 = lVar25 - *(long *)(lVar24 + lVar22 * 8);
                }
                else {
                  puVar1 = (ulong *)(param_7 + lVar25 * 8);
                  uVar18 = *puVar1;
                  if (lVar32 <= (long)uVar18) {
                    lVar25 = lVar22 * 8;
                    lVar22 = lVar22 + 1;
                    plVar31 = (long *)(lVar11 + uVar18 * 8);
                    *plVar31 = *plVar31 + *(long *)(lVar24 + lVar25);
                    if (lVar22 == local_e0 + 1) break;
                    goto LAB_00998695;
                  }
                  uVar33 = uVar33 + 1;
                  *puVar1 = uVar33;
                  *(long *)(lVar10 + uVar33 * 8) = lVar25;
                  *(undefined8 *)(lVar11 + uVar33 * 8) = *(undefined8 *)(lVar24 + lVar22 * 8);
                }
                lVar22 = lVar22 + 1;
              } while (lVar22 != local_e0 + 1);
            }
          }
        }
        plVar27 = plVar27 + 1;
        lVar22 = uVar33 - lVar32;
        lVar32 = uVar33 + 1;
        *(long *)(lVar9 + lVar17 * 8) = lVar22 + 1;
        lVar22 = lVar17;
        if (plVar27 == plVar2) goto LAB_009986f2;
      }
      plVar27 = plVar27 + 1;
    } while (plVar27 != plVar2);
LAB_009986f2:
    lVar26 = (long)uVar33 >> 1;
    if ((uVar33 & 1) == 0) goto LAB_009986fe;
  }
  KOBOOLOXSPAIPP(1,"mldorder.c",0x6ce);
LAB_00998728:
  *(long *)(lVar19 + 0x18) = lVar26;
  return;
}


