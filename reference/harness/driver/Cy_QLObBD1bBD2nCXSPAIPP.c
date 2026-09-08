/* driver: Cy_QLObBD1bBD2nCXSPAIPP addr=009771d0 size=5c4 */
#include "vsdlss_ref.h"


void Cy_QLObBD1bBD2nCXSPAIPP(long param_1,long param_2,long param_3,long *param_4,long *param_5)

{
  double *pdVar1;
  double dVar2;
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
  double dVar17;
  ulong uVar18;
  
  lVar14 = 1;
  lVar3 = *(long *)(param_2 + 8);
  lVar4 = *(long *)(param_2 + 0x30);
  lVar5 = *(long *)(param_2 + 0x38);
  lVar6 = *(long *)(param_2 + 0x40);
  lVar15 = lVar3 + *(long *)(param_2 + 0x10);
  lVar7 = *(long *)(param_2 + 0x48);
  lVar8 = *(long *)(param_2 + 0x18);
  param_1 = param_1 + lVar15;
  if (0 < lVar3) {
    do {
      if (*(double *)(param_3 + lVar14 * 8) <= 0.0) {
        KOBOOLOXSPAIPP(0x19,"deg123pfutils.c",0x2e1,lVar14);
      }
      lVar12 = *(long *)(lVar4 + lVar14 * 8);
      dVar17 = *(double *)(lVar5 + lVar14 * 8);
      if (lVar12 != 0) {
        pdVar1 = (double *)(param_3 + lVar12 * 8);
        *pdVar1 = *pdVar1 - (dVar17 * dVar17) / *(double *)(param_3 + lVar14 * 8);
      }
      lVar14 = lVar14 + 1;
    } while (lVar14 <= lVar3);
  }
  lVar12 = lVar3 + 1;
  lVar14 = lVar12;
  do {
    while( true ) {
      if (lVar15 < lVar14) {
        lVar14 = 1;
        if (0 < lVar15) {
          do {
            dVar17 = SQRT(*(double *)(param_3 + lVar14 * 8));
            if (NAN(dVar17)) {
              dVar17 = sqrt(*(double *)(param_3 + lVar14 * 8));
            }
            *(double *)(lVar8 + lVar14 * 8) = dVar17;
            lVar14 = lVar14 + 1;
          } while (lVar14 <= lVar15);
        }
        lVar14 = 1;
        if (0 < lVar3) {
          do {
            if (0 < *(long *)(lVar4 + lVar14 * 8)) {
              *(double *)(lVar5 + lVar14 * 8) =
                   *(double *)(lVar5 + lVar14 * 8) / *(double *)(lVar8 + lVar14 * 8);
            }
            lVar14 = lVar14 + 1;
          } while (lVar14 != lVar12);
        }
        if (lVar12 <= lVar15) {
          do {
            if (0 < *(long *)(lVar4 + lVar12 * 8)) {
              *(double *)(lVar5 + lVar12 * 8) =
                   *(double *)(lVar5 + lVar12 * 8) / *(double *)(lVar8 + lVar12 * 8);
            }
            if (0 < *(long *)(lVar6 + lVar12 * 8)) {
              *(double *)(lVar7 + lVar12 * 8) =
                   *(double *)(lVar7 + lVar12 * 8) / *(double *)(lVar8 + lVar12 * 8);
            }
            lVar12 = lVar12 + 1;
          } while (lVar12 != lVar15 + 1);
        }
        return;
      }
      if (*(double *)(param_3 + lVar14 * 8) <= 0.0) {
        KOBOOLOXSPAIPP(0x19,"deg123pfutils.c",0x2fa,lVar14);
      }
      lVar16 = *(long *)(lVar4 + lVar14 * 8);
      dVar17 = *(double *)(lVar5 + lVar14 * 8);
      lVar9 = *(long *)(lVar6 + lVar14 * 8);
      dVar2 = *(double *)(lVar7 + lVar14 * 8);
      if (lVar16 != 0) break;
      if (lVar9 != 0) {
        KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x306);
      }
LAB_00977319:
      lVar14 = lVar14 + 1;
    }
    pdVar1 = (double *)(param_3 + lVar16 * 8);
    *pdVar1 = *pdVar1 - (dVar17 * dVar17) / *(double *)(param_3 + lVar14 * 8);
    if (lVar9 == 0) goto LAB_00977319;
    pdVar1 = (double *)(param_3 + lVar9 * 8);
    *pdVar1 = *pdVar1 - (dVar2 * dVar2) / *(double *)(param_3 + lVar14 * 8);
    if (lVar16 == lVar9) {
      KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x313);
    }
    lVar11 = CQOTdOLJlLABtB_uOFQB;
    lVar10 = _JRdOLJlLABtB_uOFQB;
    lVar13 = lVar16;
    if (lVar9 <= lVar16) {
      lVar13 = lVar9;
    }
    if (lVar16 <= lVar9) {
      lVar16 = lVar9;
    }
    dVar17 = (dVar17 * dVar2) / *(double *)(param_3 + lVar14 * 8);
    uVar18 = (ulong)dVar17 ^ DAT_00abd200;
    if (lVar13 <= param_1) {
      if (lVar15 < lVar13) {
        *param_4 = *param_4 + 1;
        *(long *)(lVar10 + _JRaROOBKQcKQOVuOFQB * 8) = lVar13 - lVar15;
        *(long *)(_JRrLlLABtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = lVar16 - lVar15;
        *(ulong *)(_JRmCCbFyDtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = uVar18;
        _JRaROOBKQcKQOVuOFQB = _JRaROOBKQcKQOVuOFQB + 1;
        if (_JRaERKHqFWBuOFQB < _JRaROOBKQcKQOVuOFQB) {
          TOFQBaERKHuOFQBXSPAIPP();
          _JRaROOBKQcKQOVuOFQB = 1;
        }
      }
      else {
        lVar9 = lVar13 * 8;
        lVar10 = *(long *)(lVar4 + lVar9);
        if (lVar10 == 0) {
          *(long *)(lVar4 + lVar9) = lVar16;
          *(ulong *)(lVar5 + lVar13 * 8) = uVar18;
        }
        else if (lVar16 == lVar10) {
          *(double *)(lVar9 + lVar5) = *(double *)(lVar9 + lVar5) - dVar17;
        }
        else {
          lVar10 = *(long *)(lVar6 + lVar9);
          if (lVar10 == 0) {
            *(long *)(lVar6 + lVar9) = lVar16;
            *(ulong *)(lVar7 + lVar13 * 8) = uVar18;
          }
          else if (lVar16 == lVar10) {
            *(double *)(lVar9 + lVar7) = *(double *)(lVar9 + lVar7) - dVar17;
          }
          else {
            KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x338);
          }
        }
      }
      goto LAB_00977319;
    }
    *param_5 = *param_5 + 1;
    *(long *)(lVar11 + CQOTaROOBKQcKQOVuOFQB * 8) = lVar13 - param_1;
    *(long *)(CQOTrLlLABtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = lVar16 - param_1;
    *(ulong *)(CQOTmCCbFyDtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = uVar18;
    CQOTaROOBKQcKQOVuOFQB = CQOTaROOBKQcKQOVuOFQB + 1;
    if (CQOTaROOBKQcKQOVuOFQB <= CQOTaERKHqFWBuOFQB) goto LAB_00977319;
    lVar14 = lVar14 + 1;
    TOFQBdOLJrLmCCbFyDXSPAIPP();
    CQOTaROOBKQcKQOVuOFQB = 1;
  } while( true );
}


