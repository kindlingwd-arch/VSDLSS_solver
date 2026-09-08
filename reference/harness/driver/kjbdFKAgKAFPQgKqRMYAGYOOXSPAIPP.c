/* driver: kjbdFKAgKAFPQgKqRMYAGYOOXSPAIPP addr=009b59b0 size=520 */
#include "vsdlss_ref.h"


void kjbdFKAgKAFPQgKqRMYAGYOOXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long *param_6,
               long *param_7)

{
  long *plVar1;
  long lVar2;
  ulong uVar3;
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
  bool bVar16;
  long lVar17;
  long *plVar18;
  long *plVar19;
  long lVar20;
  long lVar21;
  long *plVar22;
  long lVar23;
  ulong uVar24;
  long lVar25;
  long lVar26;
  long lVar27;
  long *plVar28;
  
  uVar3 = param_6[2];
  lVar4 = param_6[6];
  lVar5 = *(long *)(param_3 + 0x50);
  lVar6 = *(long *)(param_3 + 0x60);
  lVar7 = *param_6;
  lVar8 = param_7[2];
  lVar9 = param_6[1];
  lVar10 = param_6[3];
  lVar11 = param_6[4];
  lVar2 = param_1;
  do {
    if (lVar2 < 1) {
      return;
    }
    lVar17 = lVar7;
    if (lVar2 <= lVar7) {
      lVar17 = lVar2;
    }
    if (0 < lVar17) {
      lVar26 = 1;
      do {
        while( true ) {
          lVar21 = *(long *)(param_2 + lVar26 * 8);
          lVar27 = *(long *)(lVar5 + lVar21 * 8);
          if (((0 < *(long *)(param_4 + lVar21 * 8)) || (param_1 < lVar27)) ||
             (*(long *)(param_5 + lVar21 * 8) == -0x7ffffff5)) break;
          if (lVar27 < 1) {
            lVar21 = 8;
            bVar16 = false;
            lVar27 = 1;
          }
          else {
            plVar18 = (long *)(*(long *)(lVar6 + lVar21 * 8) + 8);
            uVar24 = 0;
            plVar22 = plVar18 + lVar27;
            do {
              uVar24 = uVar24 + *plVar18;
              if (lVar9 <= (long)uVar24) {
                uVar24 = uVar24 & uVar3;
              }
              plVar18 = plVar18 + 1;
            } while (plVar18 != plVar22);
            lVar27 = uVar24 + 1;
            lVar21 = lVar27 * 8;
            bVar16 = lVar27 < 1;
          }
          if ((lVar9 < lVar27) || (bVar16)) {
            KOBOOLOXSPAIPP(1,"mldmmd.c",0x67c);
          }
          *(long *)(lVar4 + lVar26 * 8) = lVar27;
          lVar26 = lVar26 + 1;
          *(long *)(lVar10 + lVar21) = *(long *)(lVar10 + lVar21) + 1;
          if (lVar17 < lVar26) goto LAB_009b5b47;
        }
        lVar26 = lVar26 + 1;
      } while (lVar26 <= lVar17);
LAB_009b5b47:
      lVar21 = 1;
      lVar26 = param_6[5];
      do {
        lVar27 = *(long *)(param_2 + lVar21 * 8);
        if (((*(long *)(param_4 + lVar27 * 8) < 1) && (*(long *)(lVar5 + lVar27 * 8) <= param_1)) &&
           (*(long *)(param_5 + lVar27 * 8) != -0x7ffffff5)) {
          lVar27 = *(long *)(lVar4 + lVar21 * 8);
          plVar22 = (long *)(lVar10 + lVar27 * 8);
          lVar23 = *plVar22;
          if (lVar23 != 0) {
            *(long *)(lVar11 + lVar27 * 8) = lVar26;
            lVar26 = lVar26 + lVar23 * 8;
            *plVar22 = 0;
          }
        }
        lVar21 = lVar21 + 1;
      } while (lVar21 != lVar17 + 1);
      lVar26 = 1;
      do {
        lVar21 = *(long *)(param_2 + lVar26 * 8);
        if (((*(long *)(param_4 + lVar21 * 8) < 1) && (*(long *)(lVar5 + lVar21 * 8) <= param_1)) &&
           (*(long *)(param_5 + lVar21 * 8) != -0x7ffffff5)) {
          lVar27 = *(long *)(lVar4 + lVar26 * 8);
          plVar22 = (long *)(lVar10 + lVar27 * 8);
          lVar27 = *(long *)(lVar11 + lVar27 * 8);
          lVar23 = *plVar22 + 1;
          *plVar22 = lVar23;
          *(long *)(lVar27 + lVar23 * 8) = lVar21;
        }
        lVar26 = lVar26 + 1;
      } while (lVar17 + 1 != lVar26);
      lVar26 = 1;
      do {
        while( true ) {
          lVar21 = *(long *)(param_2 + lVar26 * 8);
          if (((*(long *)(param_4 + lVar21 * 8) < 1) && (*(long *)(lVar5 + lVar21 * 8) <= param_1))
             && (*(long *)(param_5 + lVar21 * 8) != -0x7ffffff5)) break;
LAB_009b5bf0:
          lVar26 = lVar26 + 1;
          if (lVar17 < lVar26) goto LAB_009b5dd5;
        }
        lVar27 = *(long *)(lVar4 + lVar26 * 8);
        lVar21 = lVar27 * 8;
        plVar22 = (long *)(lVar10 + lVar21);
        lVar23 = *plVar22;
        if (lVar23 < 2) {
          *plVar22 = 0;
          *(undefined8 *)(lVar11 + lVar27 * 8) = 0;
          goto LAB_009b5bf0;
        }
        plVar18 = (long *)(lVar21 + lVar11);
        lVar21 = *plVar18;
        lVar27 = 2;
        do {
          lVar12 = *(long *)(lVar21 + -8 + lVar27 * 8);
          plVar1 = (long *)(param_5 + lVar12 * 8);
          if (*plVar1 != -0x7ffffff5) {
            lVar13 = *(long *)(lVar5 + lVar12 * 8);
            plVar19 = (long *)(*(long *)(lVar6 + lVar12 * 8) + 8);
            lVar12 = *param_7;
            plVar28 = plVar19 + lVar13;
            if (0 < lVar13) {
              do {
                lVar25 = *plVar19;
                plVar19 = plVar19 + 1;
                *(long *)(lVar8 + lVar25 * 8) = lVar12;
              } while (plVar19 != plVar28);
            }
            lVar25 = lVar27;
            if (lVar27 <= lVar23) {
              do {
                while( true ) {
                  lVar14 = *(long *)(lVar21 + lVar25 * 8);
                  if (lVar13 == *(long *)(lVar5 + lVar14 * 8)) break;
LAB_009b5cf8:
                  lVar25 = lVar25 + 1;
                  if (lVar25 == lVar23 + 1) goto LAB_009b5d80;
                }
                plVar28 = (long *)(lVar14 * 8 + param_5);
                lVar15 = *plVar28;
                if (lVar15 == -0x7ffffff5) goto LAB_009b5cf8;
                lVar14 = *(long *)(lVar6 + lVar14 * 8);
                if (0 < lVar13) {
                  if (lVar12 <= *(long *)(lVar8 + *(long *)(lVar14 + 8) * 8)) {
                    lVar20 = 1;
                    do {
                      lVar20 = lVar20 + 1;
                      if (lVar13 < lVar20) goto LAB_009b5d60;
                    } while (lVar12 <= *(long *)(lVar8 + *(long *)(lVar14 + lVar20 * 8) * 8));
                  }
                  goto LAB_009b5cf8;
                }
LAB_009b5d60:
                lVar25 = lVar25 + 1;
                *plVar1 = *plVar1 + lVar15 + 0x7ffffff5;
                *plVar28 = -0x7ffffff5;
              } while (lVar25 != lVar23 + 1);
            }
LAB_009b5d80:
            RKJyOHaLRKQFKDkyOHXSPAIPP(param_7);
          }
          bVar16 = lVar27 <= lVar23;
          lVar27 = lVar27 + 1;
        } while (bVar16);
        lVar26 = lVar26 + 1;
        *plVar22 = 0;
        *plVar18 = 0;
      } while (lVar26 <= lVar17);
    }
LAB_009b5dd5:
    lVar2 = lVar2 - lVar17;
    param_2 = param_2 + lVar17 * 8;
  } while( true );
}


