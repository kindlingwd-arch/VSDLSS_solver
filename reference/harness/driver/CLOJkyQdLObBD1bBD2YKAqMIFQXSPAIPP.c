/* driver: CLOJkyQdLObBD1bBD2YKAqMIFQXSPAIPP addr=00976980 size=484 */
#include "vsdlss_ref.h"


long CLOJkyQdLObBD1bBD2YKAqMIFQXSPAIPP
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,long param_4,
               undefined8 param_5,long param_6,long param_7,long *param_8,long *param_9)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long *plVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  undefined8 uVar11;
  long local_48;
  
  lVar4 = BKNqLISBgKkBJXSPAIPP();
  if (lVar4 == 0) {
    local_48 = KBTbBD1bBD2nClLbFyDtB_XSPAIPP(param_2,param_4,param_5);
  }
  else {
    local_48 = QyDlBTbBD1bBD2nClLbFyDtB_XSPAIPP(param_1,param_2,param_4,param_5);
  }
  lVar4 = *(long *)(local_48 + 0x40);
  lVar1 = *(long *)(local_48 + 0x30);
  lVar10 = *(long *)(local_48 + 8) + *(long *)(local_48 + 0x10);
  lVar2 = *(long *)(local_48 + 0x38);
  lVar3 = *(long *)(local_48 + 0x48);
  FKFQpByAdOLJrLmCCbFyDXSPAIPP(0x1b,0x1c,0x1d,param_3);
  lVar5 = CQOTlRJgQBJPjBCQ;
  if (CQOTlRJgQBJPjBCQ < 1) {
LAB_00976b70:
    CQOTlRJgQBJPjBCQ = lVar5;
    param_4 = param_4 + 1;
    CFKFPEpByAdOLJrLmCCbFyDXSPAIPP();
    if (param_4 <= lVar10) {
      do {
        while ((lVar5 = *(long *)(lVar1 + param_4 * 8), lVar5 != 0 &&
               (lVar5 == *(long *)(lVar4 + param_4 * 8)))) {
          *(double *)(lVar2 + param_4 * 8) =
               *(double *)(lVar2 + param_4 * 8) + *(double *)(lVar3 + param_4 * 8);
          *(undefined8 *)(lVar4 + param_4 * 8) = 0;
          *(undefined8 *)(lVar3 + param_4 * 8) = 0;
          param_4 = param_4 + 1;
          if (param_4 == lVar10 + 1) {
            return local_48;
          }
        }
        param_4 = param_4 + 1;
      } while (param_4 != lVar10 + 1);
    }
    return local_48;
  }
  CQOTaROOBKQcKQOVpByA = CQOTaROOBKQcKQOVpByA + 1;
  if (CQOTaERKHqFWBpByA < CQOTaROOBKQcKQOVpByA) {
    OByAdOLJrLmCCbFyDXSPAIPP();
    CQOTaROOBKQcKQOVpByA = 1;
    lVar5 = 8;
  }
  else {
    lVar5 = CQOTaROOBKQcKQOVpByA * 8;
  }
  lVar8 = *(long *)(CQOTrLlLABtB_pByA + lVar5);
  lVar7 = *(long *)(CQOTdOLJlLABtB_pByA + lVar5);
  uVar11 = *(undefined8 *)(CQOTmCCbFyDtB_pByA + lVar5);
  param_6 = param_6 + lVar10;
  do {
    CQOTlRJgQBJPjBCQ = CQOTlRJgQBJPjBCQ + -1;
    lVar5 = *(long *)(param_7 + lVar7 * 8);
    lVar8 = *(long *)(param_7 + lVar8 * 8);
    if (lVar5 == lVar8) {
      KOBOOLOXSPAIPP(0x2c,"deg123pfutils.c",0x213);
LAB_00976c1c:
      lVar7 = lVar5;
      lVar5 = lVar8;
    }
    else {
      lVar7 = lVar8;
      if (lVar5 <= lVar8) goto LAB_00976c1c;
    }
    if (param_4 < lVar7) {
      if (lVar10 < lVar7) {
        if (param_6 < lVar7) {
          *param_9 = *param_9 + 1;
          *(long *)(CQOTdOLJlLABtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = lVar7 - param_6;
          *(long *)(CQOTrLlLABtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = lVar5 - param_6;
          *(undefined8 *)(CQOTmCCbFyDtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = uVar11;
          CQOTaROOBKQcKQOVuOFQB = CQOTaROOBKQcKQOVuOFQB + 1;
          lVar8 = CQOTlRJgQBJPjBCQ;
          lVar5 = CQOTlRJgQBJPjBCQ;
          if (CQOTaERKHqFWBuOFQB < CQOTaROOBKQcKQOVuOFQB) {
            TOFQBdOLJrLmCCbFyDXSPAIPP();
            CQOTaROOBKQcKQOVuOFQB = 1;
            lVar8 = CQOTlRJgQBJPjBCQ;
            lVar5 = CQOTlRJgQBJPjBCQ;
          }
        }
        else {
          *param_8 = *param_8 + 1;
          *(long *)(_JRdOLJlLABtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = lVar7 - lVar10;
          *(long *)(_JRrLlLABtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = lVar5 - lVar10;
          *(undefined8 *)(_JRmCCbFyDtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = uVar11;
          _JRaROOBKQcKQOVuOFQB = _JRaROOBKQcKQOVuOFQB + 1;
          lVar8 = CQOTlRJgQBJPjBCQ;
          lVar5 = CQOTlRJgQBJPjBCQ;
          if (_JRaERKHqFWBuOFQB < _JRaROOBKQcKQOVuOFQB) {
            TOFQBaERKHuOFQBXSPAIPP();
            _JRaROOBKQcKQOVuOFQB = 1;
            lVar8 = CQOTlRJgQBJPjBCQ;
            lVar5 = CQOTlRJgQBJPjBCQ;
          }
        }
      }
      else {
        lVar9 = lVar7 * 8;
        plVar6 = (long *)(lVar1 + lVar9);
        if ((*plVar6 == 0) ||
           ((*(long *)(lVar4 + lVar9) != 0 &&
            (KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x225), *plVar6 == 0)))) goto LAB_00976a92;
        *(long *)(lVar4 + lVar9) = lVar5;
        lVar8 = CQOTlRJgQBJPjBCQ;
        *(undefined8 *)(lVar3 + lVar9) = uVar11;
        lVar5 = CQOTlRJgQBJPjBCQ;
      }
    }
    else {
      plVar6 = (long *)(lVar1 + lVar7 * 8);
      if (*plVar6 != 0) {
        KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x21d);
      }
LAB_00976a92:
      *plVar6 = lVar5;
      *(undefined8 *)(lVar2 + lVar7 * 8) = uVar11;
      lVar8 = CQOTlRJgQBJPjBCQ;
      lVar5 = CQOTlRJgQBJPjBCQ;
    }
    CQOTlRJgQBJPjBCQ = lVar8;
    if (CQOTlRJgQBJPjBCQ < 1) goto LAB_00976b70;
    CQOTaROOBKQcKQOVpByA = CQOTaROOBKQcKQOVpByA + 1;
    if (CQOTaERKHqFWBpByA < CQOTaROOBKQcKQOVpByA) {
      CQOTlRJgQBJPjBCQ = lVar5;
      OByAdOLJrLmCCbFyDXSPAIPP();
      CQOTaROOBKQcKQOVpByA = 1;
      lVar5 = 8;
    }
    else {
      lVar5 = CQOTaROOBKQcKQOVpByA * 8;
    }
    lVar8 = *(long *)(CQOTrLlLABtB_pByA + lVar5);
    lVar7 = *(long *)(CQOTdOLJlLABtB_pByA + lVar5);
    uVar11 = *(undefined8 *)(CQOTmCCbFyDtB_pByA + lVar5);
  } while( true );
}


