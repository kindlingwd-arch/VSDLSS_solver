/* driver: PRz1pLTaLKQOFzdOLJpBPQXSPAIPP addr=0096ae30 size=23a */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PRz1pLTaLKQOFzdOLJpBPQXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  double dVar13;
  double dVar14;
  
  lVar2 = *(long *)(param_2 + 0x50);
  lVar3 = *(long *)(param_2 + 0x48);
  lVar11 = *(long *)(*(long *)(param_2 + 0x58) + param_1 * 8);
  lVar12 = *(long *)(lVar2 + param_1 * 8);
  lVar9 = *(long *)(lVar3 + param_1 * 8);
  dVar14 = *(double *)(lVar11 + 8);
  if (dVar14 <= 0.0) {
    KOBOOLOXSPAIPP(0x23,"vsdlss1memutils.c",0x4b0,param_1);
    dVar14 = *(double *)(lVar11 + 8);
  }
  uVar4 = DAT_00abd200;
  dVar14 = _DAT_00ab67f8 / dVar14;
  if (1 < lVar9) {
    do {
      lVar9 = lVar9 + -1;
      lVar6 = *(long *)(lVar12 + 0x10);
      dVar13 = (double)(*(ulong *)(lVar11 + 0x10) ^ uVar4) * dVar14;
      if ((*(long *)(param_2 + 0x80) == 0) || (lVar6 <= *(long *)(param_2 + 0x88))) {
        lVar10 = *(long *)(lVar3 + lVar6 * 8);
        lVar7 = *(long *)(*(long *)(param_2 + 0x58) + lVar6 * 8);
        lVar8 = lVar9 - lVar10;
        if (lVar8 == 0) goto LAB_0096af9d;
LAB_0096aeee:
        if (SBORROW8(lVar9,lVar10) == lVar8 < 0) {
          KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x4d6);
        }
        else {
          lVar6 = *(long *)(lVar2 + lVar6 * 8);
          if ((lVar10 < 1) || (lVar9 < 1)) {
            lVar8 = 1;
          }
          else {
            lVar5 = 1;
            lVar8 = 1;
            do {
              if (*(long *)(lVar12 + 8 + lVar8 * 8) == *(long *)(lVar6 + lVar5 * 8)) {
                lVar1 = lVar8 * 8;
                lVar8 = lVar8 + 1;
                *(double *)(lVar7 + lVar5 * 8) =
                     *(double *)(lVar11 + 8 + lVar1) * dVar13 + *(double *)(lVar7 + lVar5 * 8);
              }
            } while ((lVar8 <= lVar9) && (lVar5 = lVar5 + 1, lVar5 <= lVar10));
          }
          if (lVar8 <= lVar9) {
            KOBOOLOXSPAIPP(0x25,"vsdlss1memutils.c",0x4d1);
          }
        }
      }
      else {
        lVar7 = *(long *)(*(long *)(param_2 + 0x80) + (lVar6 - *(long *)(param_2 + 0x88)) * 8);
        lVar10 = *(long *)(lVar3 + lVar6 * 8);
        lVar8 = lVar9 - lVar10;
        if (lVar9 != lVar10) goto LAB_0096aeee;
LAB_0096af9d:
        lVar6 = 1;
        do {
          *(double *)(lVar7 + lVar6 * 8) =
               *(double *)(lVar11 + 8 + lVar6 * 8) * dVar13 + *(double *)(lVar7 + lVar6 * 8);
          lVar6 = lVar6 + 1;
        } while (lVar6 <= lVar9);
      }
      lVar11 = lVar11 + 8;
      lVar12 = lVar12 + 8;
    } while (lVar9 != 1);
  }
  return;
}


