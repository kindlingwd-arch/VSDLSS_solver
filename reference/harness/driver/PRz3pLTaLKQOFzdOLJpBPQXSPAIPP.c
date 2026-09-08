/* driver: PRz3pLTaLKQOFzdOLJpBPQXSPAIPP addr=0096b430 size=52b */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz3pLTaLKQOFzdOLJpBPQXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  double dVar8;
  ulong uVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  double dVar24;
  double dVar25;
  
  lVar6 = *(long *)(param_2 + 0x50);
  lVar7 = *(long *)(param_2 + 0x48);
  lVar17 = *(long *)(param_2 + 0x58);
  lVar15 = *(long *)(lVar6 + param_1 * 8);
  lVar18 = *(long *)(lVar7 + param_1 * 8);
  lVar19 = *(long *)(lVar17 + param_1 * 8);
  lVar13 = *(long *)(lVar17 + 8 + param_1 * 8);
  lVar17 = *(long *)(lVar17 + 0x10 + param_1 * 8);
  dVar25 = *(double *)(lVar19 + 8);
  if (dVar25 <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"vsdlss1memutils.c",0x558,param_1);
    dVar25 = *(double *)(lVar19 + 8);
  }
  uVar9 = DAT_00abd200;
  dVar8 = _DAT_00ab67f8;
  lVar10 = 1;
  dVar25 = _DAT_00ab67f8 / dVar25;
  dVar21 = (double)(*(ulong *)(lVar19 + 0x10) ^ DAT_00abd200);
  if (1 < lVar18) {
    do {
      *(double *)(lVar13 + lVar10 * 8) =
           *(double *)(lVar19 + 8 + lVar10 * 8) * dVar21 * dVar25 + *(double *)(lVar13 + lVar10 * 8)
      ;
      lVar10 = lVar10 + 1;
    } while (lVar10 != lVar18);
  }
  dVar21 = *(double *)(lVar13 + 8);
  if (dVar21 <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"vsdlss1memutils.c",0x565,param_1 + 1);
    dVar21 = *(double *)(lVar13 + 8);
  }
  uVar4 = *(ulong *)(lVar19 + 0x18);
  uVar5 = *(ulong *)(lVar13 + 0x10);
  lVar10 = lVar18 + -2;
  dVar21 = dVar8 / dVar21;
  lVar11 = 1;
  if (0 < lVar10) {
    do {
      *(double *)(lVar17 + lVar11 * 8) =
           *(double *)(lVar19 + 0x10 + lVar11 * 8) * (double)(uVar4 ^ uVar9) * dVar25 +
           *(double *)(lVar13 + 8 + lVar11 * 8) * (double)(uVar5 ^ uVar9) * dVar21 +
           *(double *)(lVar17 + lVar11 * 8);
      lVar11 = lVar11 + 1;
    } while (lVar11 != lVar18 + -1);
  }
  dVar20 = *(double *)(lVar17 + 8);
  if (dVar20 <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"vsdlss1memutils.c",0x574,param_1 + 2);
    dVar20 = *(double *)(lVar17 + 8);
  }
  lVar15 = lVar15 + 0x10;
  lVar18 = lVar19 + 0x10;
  lVar19 = lVar13 + 8;
  if (1 < lVar10) {
    do {
      lVar10 = lVar10 + -1;
      lVar13 = *(long *)(lVar15 + 0x10);
      dVar22 = (double)(*(ulong *)(lVar18 + 0x10) ^ uVar9) * dVar25;
      dVar23 = (double)(*(ulong *)(lVar19 + 0x10) ^ uVar9) * dVar21;
      dVar24 = (double)(*(ulong *)(lVar17 + 0x10) ^ uVar9) * (dVar8 / dVar20);
      if ((*(long *)(param_2 + 0x80) == 0) || (lVar13 <= *(long *)(param_2 + 0x88))) {
        lVar11 = *(long *)(*(long *)(param_2 + 0x58) + lVar13 * 8);
        lVar16 = *(long *)(lVar7 + lVar13 * 8);
        lVar14 = lVar10 - lVar16;
        if (lVar14 == 0) goto LAB_0096b6e9;
LAB_0096b616:
        if (SBORROW8(lVar10,lVar16) == lVar14 < 0) {
          KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x59f);
        }
        else {
          lVar13 = *(long *)(lVar6 + lVar13 * 8);
          if ((lVar16 < 1) || (lVar10 < 1)) {
            lVar14 = 1;
          }
          else {
            lVar12 = 1;
            lVar14 = 1;
            do {
              if (*(long *)(lVar15 + 8 + lVar14 * 8) == *(long *)(lVar13 + lVar12 * 8)) {
                lVar1 = lVar14 * 8;
                lVar2 = lVar14 * 8;
                lVar3 = lVar14 * 8;
                lVar14 = lVar14 + 1;
                *(double *)(lVar11 + lVar12 * 8) =
                     *(double *)(lVar18 + 8 + lVar1) * dVar22 +
                     *(double *)(lVar19 + 8 + lVar2) * dVar23 +
                     *(double *)(lVar17 + 8 + lVar3) * dVar24 + *(double *)(lVar11 + lVar12 * 8);
              }
            } while ((lVar14 <= lVar10) && (lVar12 = lVar12 + 1, lVar12 <= lVar16));
          }
          if (lVar14 <= lVar10) {
            KOBOOLOXSPAIPP(0x25,"vsdlss1memutils.c",0x59a);
          }
        }
      }
      else {
        lVar11 = *(long *)(*(long *)(param_2 + 0x80) + (lVar13 - *(long *)(param_2 + 0x88)) * 8);
        lVar16 = *(long *)(lVar7 + lVar13 * 8);
        lVar14 = lVar10 - lVar16;
        if (lVar10 != lVar16) goto LAB_0096b616;
LAB_0096b6e9:
        lVar13 = 1;
        do {
          *(double *)(lVar11 + lVar13 * 8) =
               *(double *)(lVar18 + 8 + lVar13 * 8) * dVar22 +
               *(double *)(lVar19 + 8 + lVar13 * 8) * dVar23 +
               *(double *)(lVar17 + 8 + lVar13 * 8) * dVar24 + *(double *)(lVar11 + lVar13 * 8);
          lVar13 = lVar13 + 1;
        } while (lVar13 <= lVar10);
      }
      lVar15 = lVar15 + 8;
      lVar17 = lVar17 + 8;
      lVar18 = lVar18 + 8;
      lVar19 = lVar19 + 8;
    } while (lVar10 != 1);
  }
  return;
}


