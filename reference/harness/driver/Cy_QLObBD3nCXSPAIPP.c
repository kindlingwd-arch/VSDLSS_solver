/* driver: Cy_QLObBD3nCXSPAIPP addr=009777d0 size=57a */
#include "vsdlss_ref.h"


void Cy_QLObBD3nCXSPAIPP(long param_1,long param_2,long *param_3)

{
  double *pdVar1;
  double __x;
  double dVar2;
  double dVar3;
  double dVar4;
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
  long lVar18;
  long lVar19;
  long lVar20;
  double dVar21;
  ulong uVar22;
  
  lVar20 = 1;
  lVar5 = *(long *)(param_1 + 0x18);
  lVar6 = *(long *)(param_1 + 8);
  lVar7 = *(long *)(param_1 + 0x20);
  lVar8 = *(long *)(param_1 + 0x28);
  lVar9 = *(long *)(param_1 + 0x30);
  lVar10 = *(long *)(param_1 + 0x38);
  lVar11 = *(long *)(param_1 + 0x40);
  lVar12 = *(long *)(param_1 + 0x10);
  if (0 < lVar6) {
    do {
      __x = *(double *)(param_2 + lVar20 * 8);
      if (__x <= 0.0) {
        KOBOOLOXSPAIPP(__x,0x19,"deg123pfutils.c",0x378,lVar20);
      }
      dVar2 = *(double *)(lVar7 + lVar20 * 8);
      lVar17 = *(long *)(lVar5 + lVar20 * 8);
      lVar19 = *(long *)(lVar8 + lVar20 * 8);
      dVar3 = *(double *)(lVar9 + lVar20 * 8);
      lVar13 = *(long *)(lVar10 + lVar20 * 8);
      dVar4 = *(double *)(lVar11 + lVar20 * 8);
      if (((lVar17 == 0) && (lVar13 != 0 || lVar19 != 0)) || ((lVar13 != 0 && (lVar19 == 0)))) {
        KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x387);
      }
      dVar21 = SQRT(__x);
      if (NAN(dVar21)) {
        dVar21 = sqrt(__x);
      }
      *(double *)(lVar12 + lVar20 * 8) = dVar21;
      if (lVar17 != 0) {
        if (lVar17 <= lVar6) {
          KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x391);
        }
        pdVar1 = (double *)(param_2 + lVar17 * 8);
        *pdVar1 = *pdVar1 - (dVar2 * dVar2) / __x;
        *(double *)(lVar7 + lVar20 * 8) = *(double *)(lVar7 + lVar20 * 8) / dVar21;
        if (lVar19 != 0) {
          if (lVar19 <= lVar6) {
            KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x39a);
          }
          lVar15 = CQOTdOLJlLABtB_uOFQB;
          pdVar1 = (double *)(param_2 + lVar19 * 8);
          *pdVar1 = *pdVar1 - (dVar3 * dVar3) / __x;
          *(double *)(lVar9 + lVar20 * 8) = *(double *)(lVar9 + lVar20 * 8) / dVar21;
          *param_3 = *param_3 + 1;
          lVar16 = lVar17;
          if (lVar19 <= lVar17) {
            lVar16 = lVar19;
          }
          *(long *)(lVar15 + CQOTaROOBKQcKQOVuOFQB * 8) = lVar16 - lVar6;
          lVar15 = lVar17;
          if (lVar17 <= lVar19) {
            lVar15 = lVar19;
          }
          *(long *)(CQOTrLlLABtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = lVar15 - lVar6;
          *(ulong *)(CQOTmCCbFyDtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) =
               (ulong)((dVar3 * dVar2) / __x) ^ DAT_00abd200;
          CQOTaROOBKQcKQOVuOFQB = CQOTaROOBKQcKQOVuOFQB + 1;
          if (CQOTaERKHqFWBuOFQB < CQOTaROOBKQcKQOVuOFQB) {
            TOFQBdOLJrLmCCbFyDXSPAIPP();
            CQOTaROOBKQcKQOVuOFQB = 1;
          }
          if (lVar13 != 0) {
            if (lVar13 <= lVar6) {
              KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x3ae);
            }
            lVar15 = CQOTdOLJlLABtB_uOFQB;
            pdVar1 = (double *)(param_2 + lVar13 * 8);
            uVar22 = (ulong)((dVar2 * dVar4) / __x) ^ DAT_00abd200;
            *pdVar1 = *pdVar1 - (dVar4 * dVar4) / __x;
            *(double *)(lVar11 + lVar20 * 8) = *(double *)(lVar11 + lVar20 * 8) / dVar21;
            *param_3 = *param_3 + 1;
            lVar16 = lVar17;
            if (lVar13 <= lVar17) {
              lVar16 = lVar13;
            }
            if (lVar17 <= lVar13) {
              lVar17 = lVar13;
            }
            *(long *)(lVar15 + CQOTaROOBKQcKQOVuOFQB * 8) = lVar16 - lVar6;
            lVar16 = CQOTrLlLABtB_uOFQB;
            *(long *)(CQOTrLlLABtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = lVar17 - lVar6;
            lVar14 = CQOTaERKHqFWBuOFQB;
            lVar18 = CQOTmCCbFyDtB_uOFQB;
            lVar17 = CQOTaROOBKQcKQOVuOFQB + 1;
            *(ulong *)(CQOTmCCbFyDtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = uVar22;
            CQOTaROOBKQcKQOVuOFQB = lVar17;
            if (lVar14 < lVar17) {
              TOFQBdOLJrLmCCbFyDXSPAIPP();
              CQOTaROOBKQcKQOVuOFQB = 1;
              lVar18 = CQOTmCCbFyDtB_uOFQB;
              lVar15 = CQOTdOLJlLABtB_uOFQB;
              lVar16 = CQOTrLlLABtB_uOFQB;
            }
            *param_3 = *param_3 + 1;
            lVar17 = lVar19;
            if (lVar13 <= lVar19) {
              lVar17 = lVar13;
            }
            if (lVar19 <= lVar13) {
              lVar19 = lVar13;
            }
            *(long *)(lVar15 + CQOTaROOBKQcKQOVuOFQB * 8) = lVar17 - lVar6;
            *(long *)(lVar16 + CQOTaROOBKQcKQOVuOFQB * 8) = lVar19 - lVar6;
            lVar19 = CQOTaERKHqFWBuOFQB;
            lVar17 = CQOTaROOBKQcKQOVuOFQB + 1;
            *(ulong *)(lVar18 + CQOTaROOBKQcKQOVuOFQB * 8) =
                 (ulong)((dVar3 * dVar4) / __x) ^ DAT_00abd200;
            CQOTaROOBKQcKQOVuOFQB = lVar17;
            if (lVar19 < lVar17) {
              TOFQBdOLJrLmCCbFyDXSPAIPP();
              CQOTaROOBKQcKQOVuOFQB = 1;
            }
          }
        }
      }
      lVar20 = lVar20 + 1;
    } while (lVar20 <= lVar6);
  }
  return;
}


