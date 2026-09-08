/* driver: PRz3pLTaLKQOFzdOLJpLTpyKDBXSPAIPP addr=00982c80 size=5a3 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz3pLTaLKQOFzdOLJpLTpyKDBXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  double dVar6;
  ulong uVar7;
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
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  
  lVar4 = *(long *)(param_4 + 0x50);
  lVar5 = *(long *)(param_4 + 0x48);
  lVar11 = *(long *)(param_4 + 0x58);
  lVar17 = *(long *)(lVar4 + param_1 * 8);
  lVar10 = *(long *)(lVar11 + param_1 * 8);
  lVar14 = *(long *)(lVar11 + 8 + param_1 * 8);
  lVar11 = *(long *)(lVar11 + 0x10 + param_1 * 8);
  lVar12 = *(long *)(lVar5 + param_1 * 8);
  if (param_2 <= *(long *)(lVar17 + lVar12 * 8)) {
    dVar23 = *(double *)(lVar10 + 8);
    if (dVar23 <= 0.0) {
      KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",0x1d4,param_1);
      dVar23 = *(double *)(lVar10 + 8);
    }
    dVar6 = _DAT_00ab67f8;
    dVar23 = _DAT_00ab67f8 / dVar23;
    if ((param_2 <= param_1 + 1) &&
       (dVar18 = (double)(*(ulong *)(lVar10 + 0x10) ^ DAT_00abd200), 1 < lVar12)) {
      lVar8 = 1;
      do {
        *(double *)(lVar14 + lVar8 * 8) =
             *(double *)(lVar10 + 8 + lVar8 * 8) * dVar18 * dVar23 + *(double *)(lVar14 + lVar8 * 8)
        ;
        lVar8 = lVar8 + 1;
      } while (lVar8 != lVar12);
    }
    dVar18 = *(double *)(lVar14 + 8);
    if (dVar18 <= 0.0) {
      KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",0x1e3);
      dVar18 = *(double *)(lVar14 + 8);
    }
    lVar8 = lVar10 + 0x10;
    dVar18 = dVar6 / dVar18;
    lVar16 = lVar12 + -2;
    if (param_2 <= param_1 + 2) {
      dVar20 = (double)(*(ulong *)(lVar10 + 0x18) ^ DAT_00abd200);
      dVar19 = (double)(*(ulong *)(lVar14 + 0x10) ^ DAT_00abd200);
      if (0 < lVar16) {
        lVar10 = 1;
        do {
          *(double *)(lVar11 + lVar10 * 8) =
               *(double *)(lVar8 + lVar10 * 8) * dVar20 * dVar23 +
               *(double *)(lVar14 + 8 + lVar10 * 8) * dVar19 * dVar18 +
               *(double *)(lVar11 + lVar10 * 8);
          lVar10 = lVar10 + 1;
        } while (lVar10 != lVar12 + -1);
      }
    }
    dVar20 = *(double *)(lVar11 + 8);
    if (dVar20 <= 0.0) {
      KOBOOLOXSPAIPP(0x23,"factorapar_ls.c",500);
      dVar20 = *(double *)(lVar11 + 8);
    }
    uVar7 = DAT_00abd200;
    if (1 < lVar16) {
      lVar10 = *(long *)(lVar17 + 0x20);
      lVar17 = lVar17 + 0x10;
      while (lVar14 = lVar14 + 8, lVar10 < param_2) {
        lVar16 = lVar16 + -1;
        lVar8 = lVar8 + 8;
        lVar11 = lVar11 + 8;
        if (lVar16 == 1) {
          return;
        }
        lVar10 = *(long *)(lVar17 + 0x18);
        lVar17 = lVar17 + 8;
      }
      if (lVar10 <= param_3) {
        do {
          lVar13 = lVar8 + 8;
          lVar16 = lVar16 + -1;
          lVar15 = lVar11 + 8;
          lVar12 = lVar14 + 8;
          dVar19 = (double)(*(ulong *)(lVar8 + 0x10) ^ uVar7) * dVar23;
          dVar21 = (double)(*(ulong *)(lVar14 + 0x10) ^ uVar7) * dVar18;
          dVar22 = (double)(*(ulong *)(lVar11 + 0x10) ^ uVar7) * (dVar6 / dVar20);
          if ((*(long *)(param_4 + 0x80) == 0) || (lVar10 <= *(long *)(param_4 + 0x88))) {
            lVar14 = *(long *)(lVar5 + lVar10 * 8);
            lVar11 = *(long *)(*(long *)(param_4 + 0x58) + lVar10 * 8);
            if (lVar16 == lVar14) goto LAB_0098301d;
LAB_00982f27:
            if (lVar16 < lVar14) {
              lVar10 = *(long *)(lVar4 + lVar10 * 8);
              if (0 < lVar14) {
                lVar9 = 1;
                lVar8 = 1;
                do {
                  if (*(long *)(lVar17 + 8 + lVar8 * 8) == *(long *)(lVar10 + lVar9 * 8)) {
                    lVar3 = lVar8 * 8;
                    lVar1 = lVar8 * 8;
                    lVar2 = lVar8 * 8;
                    lVar8 = lVar8 + 1;
                    *(double *)(lVar11 + lVar9 * 8) =
                         *(double *)(lVar13 + lVar3) * dVar19 + *(double *)(lVar12 + lVar1) * dVar21
                         + *(double *)(lVar15 + lVar2) * dVar22 + *(double *)(lVar11 + lVar9 * 8);
                  }
                } while ((lVar8 <= lVar16) && (lVar9 = lVar9 + 1, lVar9 <= lVar14));
                if (lVar16 < lVar8) goto LAB_00982fb1;
              }
              KOBOOLOXSPAIPP(0x25,"factorapar_ls.c",0x225);
            }
            else {
              KOBOOLOXSPAIPP(1,"factorapar_ls.c",0x22a);
            }
          }
          else {
            lVar11 = *(long *)(*(long *)(param_4 + 0x80) + (lVar10 - *(long *)(param_4 + 0x88)) * 8)
            ;
            lVar14 = *(long *)(lVar5 + lVar10 * 8);
            if (lVar16 != lVar14) goto LAB_00982f27;
LAB_0098301d:
            lVar10 = 1;
            do {
              *(double *)(lVar11 + lVar10 * 8) =
                   *(double *)(lVar13 + lVar10 * 8) * dVar19 +
                   *(double *)(lVar12 + lVar10 * 8) * dVar21 +
                   *(double *)(lVar15 + lVar10 * 8) * dVar22 + *(double *)(lVar11 + lVar10 * 8);
              lVar10 = lVar10 + 1;
            } while (lVar10 <= lVar16);
          }
LAB_00982fb1:
        } while ((lVar16 != 1) &&
                (lVar10 = *(long *)(lVar17 + 0x18), lVar8 = lVar13, lVar14 = lVar12, lVar11 = lVar15
                , lVar17 = lVar17 + 8, lVar10 <= param_3));
      }
    }
  }
  return;
}


