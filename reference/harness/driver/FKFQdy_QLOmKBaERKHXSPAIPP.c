/* driver: FKFQdy_QLOmKBaERKHXSPAIPP addr=00969ec0 size=2e5 */
#include "vsdlss_ref.h"


void FKFQdy_QLOmKBaERKHXSPAIPP(long param_1,long param_2,long param_3,long param_4)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  double *pdVar13;
  long lVar14;
  double dVar15;
  
  uVar1 = *(undefined8 *)(param_2 + 0x10);
  uVar2 = *(undefined8 *)(param_2 + 0x18);
  uVar3 = *(undefined8 *)(param_2 + 0x20);
  lVar4 = *(long *)(param_4 + 0x50);
  lVar5 = *(long *)(param_4 + 0x58);
  lVar6 = *(long *)(param_4 + 0x48);
  OByAbFyDtB_dlRJnBOJRQBgKQLd_(param_3,*(undefined8 *)(param_2 + 8));
  lVar14 = 0;
  FKFQpByAdOLJrLmCCbFyDXSPAIPP(uVar1,uVar2,uVar3,param_1);
  lVar7 = CQOTlRJgQBJPjBCQ;
  if (0 < CQOTlRJgQBJPjBCQ) {
    CQOTaROOBKQcKQOVpByA = CQOTaROOBKQcKQOVpByA + 1;
    if (CQOTaERKHqFWBpByA < CQOTaROOBKQcKQOVpByA) {
      OByAdOLJrLmCCbFyDXSPAIPP();
      CQOTaROOBKQcKQOVpByA = 1;
      lVar7 = 8;
    }
    else {
      lVar7 = CQOTaROOBKQcKQOVpByA * 8;
    }
    lVar14 = 0;
    lVar11 = *(long *)(CQOTdOLJlLABtB_pByA + lVar7);
    dVar15 = *(double *)(CQOTmCCbFyDtB_pByA + lVar7);
    lVar7 = *(long *)(CQOTrLlLABtB_pByA + lVar7);
    do {
      CQOTlRJgQBJPjBCQ = CQOTlRJgQBJPjBCQ + -1;
      lVar11 = *(long *)(param_3 + lVar11 * 8);
      lVar7 = *(long *)(param_3 + lVar7 * 8);
      lVar14 = lVar14 + 1;
      lVar8 = lVar7;
      if (lVar11 <= lVar7) {
        lVar8 = lVar11;
        lVar11 = lVar7;
      }
      lVar10 = *(long *)(lVar6 + lVar8 * 8);
      lVar7 = *(long *)(lVar4 + lVar8 * 8);
      if (lVar10 < 0x21) {
        if (0 < lVar10) {
          lVar9 = 1;
          if (*(long *)(lVar7 + 8) == lVar11) {
            lVar12 = 8;
            lVar9 = 1;
          }
          else {
            do {
              lVar9 = lVar9 + 1;
              if (lVar9 == lVar10 + 1) goto LAB_00969ffb;
              lVar12 = lVar9 * 8;
            } while (*(long *)(lVar7 + lVar9 * 8) != lVar11);
          }
          pdVar13 = (double *)(lVar12 + *(long *)(lVar5 + lVar8 * 8));
          *pdVar13 = dVar15 + *pdVar13;
          lVar7 = CQOTlRJgQBJPjBCQ;
          if (lVar9 <= lVar10) goto joined_r0x0096a0fe;
        }
LAB_00969ffb:
        KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x310);
        lVar7 = CQOTlRJgQBJPjBCQ;
      }
      else {
        lVar9 = 1;
        do {
          while (lVar12 = (lVar9 + lVar10) / 2, *(long *)(lVar7 + lVar12 * 8) < lVar11) {
            lVar9 = lVar12 + 1;
            if (lVar10 <= lVar9) goto LAB_0096a0ab;
          }
          lVar10 = lVar12;
        } while (lVar9 < lVar12);
LAB_0096a0ab:
        if ((lVar9 != lVar10) || (*(long *)(lVar7 + lVar9 * 8) != lVar11)) {
          KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x31d);
        }
        lVar11 = CQOTlRJgQBJPjBCQ;
        pdVar13 = (double *)(lVar9 * 8 + *(long *)(lVar5 + lVar8 * 8));
        *pdVar13 = dVar15 + *pdVar13;
        lVar7 = CQOTlRJgQBJPjBCQ;
        CQOTlRJgQBJPjBCQ = lVar11;
      }
joined_r0x0096a0fe:
      if (CQOTlRJgQBJPjBCQ < 1) break;
      CQOTaROOBKQcKQOVpByA = CQOTaROOBKQcKQOVpByA + 1;
      if (CQOTaERKHqFWBpByA < CQOTaROOBKQcKQOVpByA) {
        CQOTlRJgQBJPjBCQ = lVar7;
        OByAdOLJrLmCCbFyDXSPAIPP();
        CQOTaROOBKQcKQOVpByA = 1;
        lVar7 = 8;
      }
      else {
        lVar7 = CQOTaROOBKQcKQOVpByA * 8;
      }
      lVar11 = *(long *)(CQOTdOLJlLABtB_pByA + lVar7);
      dVar15 = *(double *)(CQOTmCCbFyDtB_pByA + lVar7);
      lVar7 = *(long *)(CQOTrLlLABtB_pByA + lVar7);
    } while( true );
  }
  CQOTlRJgQBJPjBCQ = lVar7;
  if (param_1 != lVar14) {
    KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x328);
  }
  CFKFPEpByAdOLJrLmCCbFyDXSPAIPP();
  return;
}


