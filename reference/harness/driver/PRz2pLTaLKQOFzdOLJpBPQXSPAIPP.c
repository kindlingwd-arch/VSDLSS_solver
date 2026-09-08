/* driver: PRz2pLTaLKQOFzdOLJpBPQXSPAIPP addr=0096b090 size=377 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz2pLTaLKQOFzdOLJpBPQXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  double dVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  
  lVar3 = *(long *)(param_2 + 0x50);
  lVar4 = *(long *)(param_2 + 0x48);
  lVar12 = *(long *)(lVar3 + param_1 * 8);
  lVar14 = *(long *)(lVar4 + param_1 * 8);
  lVar9 = *(long *)(*(long *)(param_2 + 0x58) + param_1 * 8);
  lVar15 = *(long *)(*(long *)(param_2 + 0x58) + 8 + param_1 * 8);
  dVar19 = *(double *)(lVar9 + 8);
  if (dVar19 <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"vsdlss1memutils.c",0x4fb,param_1);
    dVar19 = *(double *)(lVar9 + 8);
  }
  uVar6 = DAT_00abd200;
  dVar5 = _DAT_00ab67f8;
  lVar11 = lVar14 + -1;
  lVar7 = 1;
  dVar19 = _DAT_00ab67f8 / dVar19;
  dVar16 = (double)(*(ulong *)(lVar9 + 0x10) ^ DAT_00abd200);
  if (0 < lVar11) {
    do {
      *(double *)(lVar15 + lVar7 * 8) =
           *(double *)(lVar9 + 8 + lVar7 * 8) * dVar16 * dVar19 + *(double *)(lVar15 + lVar7 * 8);
      lVar7 = lVar7 + 1;
    } while (lVar7 != lVar14);
  }
  dVar16 = *(double *)(lVar15 + 8);
  if (dVar16 <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"vsdlss1memutils.c",0x508,param_1 + 1);
    dVar16 = *(double *)(lVar15 + 8);
  }
  lVar12 = lVar12 + 8;
  lVar14 = lVar9 + 8;
  if (1 < lVar11) {
    do {
      lVar11 = lVar11 + -1;
      lVar9 = *(long *)(lVar12 + 0x10);
      dVar17 = (double)(*(ulong *)(lVar14 + 0x10) ^ uVar6) * dVar19;
      dVar18 = (double)(*(ulong *)(lVar15 + 0x10) ^ uVar6) * (dVar5 / dVar16);
      if ((*(long *)(param_2 + 0x80) == 0) || (lVar9 <= *(long *)(param_2 + 0x88))) {
        lVar13 = *(long *)(lVar4 + lVar9 * 8);
        lVar7 = *(long *)(*(long *)(param_2 + 0x58) + lVar9 * 8);
        lVar10 = lVar11 - lVar13;
        if (lVar10 == 0) goto LAB_0096b28d;
LAB_0096b1ce:
        if (SBORROW8(lVar11,lVar13) == lVar10 < 0) {
          KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x52f);
        }
        else {
          lVar9 = *(long *)(lVar3 + lVar9 * 8);
          if ((lVar13 < 1) || (lVar11 < 1)) {
            lVar10 = 1;
          }
          else {
            lVar8 = 1;
            lVar10 = 1;
            do {
              if (*(long *)(lVar12 + 8 + lVar10 * 8) == *(long *)(lVar9 + lVar8 * 8)) {
                lVar1 = lVar10 * 8;
                lVar2 = lVar10 * 8;
                lVar10 = lVar10 + 1;
                *(double *)(lVar7 + lVar8 * 8) =
                     *(double *)(lVar14 + 8 + lVar1) * dVar17 +
                     *(double *)(lVar15 + 8 + lVar2) * dVar18 + *(double *)(lVar7 + lVar8 * 8);
              }
            } while ((lVar10 <= lVar11) && (lVar8 = lVar8 + 1, lVar8 <= lVar13));
          }
          if (lVar10 <= lVar11) {
            KOBOOLOXSPAIPP(0x25,"vsdlss1memutils.c",0x52a);
          }
        }
      }
      else {
        lVar7 = *(long *)(*(long *)(param_2 + 0x80) + (lVar9 - *(long *)(param_2 + 0x88)) * 8);
        lVar13 = *(long *)(lVar4 + lVar9 * 8);
        lVar10 = lVar11 - lVar13;
        if (lVar11 != lVar13) goto LAB_0096b1ce;
LAB_0096b28d:
        lVar9 = 1;
        do {
          *(double *)(lVar7 + lVar9 * 8) =
               *(double *)(lVar14 + 8 + lVar9 * 8) * dVar17 +
               *(double *)(lVar15 + 8 + lVar9 * 8) * dVar18 + *(double *)(lVar7 + lVar9 * 8);
          lVar9 = lVar9 + 1;
        } while (lVar9 <= lVar11);
      }
      lVar12 = lVar12 + 8;
      lVar14 = lVar14 + 8;
      lVar15 = lVar15 + 8;
    } while (lVar11 != 1);
  }
  return;
}


