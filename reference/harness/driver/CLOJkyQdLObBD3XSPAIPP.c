/* driver: CLOJkyQdLObBD3XSPAIPP addr=00976e30 size=354 */
#include "vsdlss_ref.h"


long CLOJkyQdLObBD3XSPAIPP(undefined8 param_1,undefined8 param_2,long param_3,long param_4)

{
  long lVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  double *pdVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  double dVar15;
  
  lVar10 = BKNqLISBgKkBJXSPAIPP();
  if (lVar10 == 0) {
    lVar10 = KBTbBD3nCXSPAIPP(param_2,param_3);
  }
  else {
    lVar10 = QyDlBTbBD3nCXSPAIPP(param_1,param_2,param_3);
  }
  lVar3 = *(long *)(lVar10 + 0x28);
  lVar4 = *(long *)(lVar10 + 0x18);
  lVar5 = *(long *)(lVar10 + 0x20);
  lVar6 = *(long *)(lVar10 + 0x30);
  lVar7 = *(long *)(lVar10 + 0x38);
  lVar8 = *(long *)(lVar10 + 0x40);
  FKFQaERKHpByAXSPAIPP(0xd);
  _JRaROOBKQcKQOVpByA = _JRaROOBKQcKQOVpByA + 1;
  if (_JRaERKHqFWBpByA < _JRaROOBKQcKQOVpByA) {
    _JRaROOBKQcKQOVpByA = 1;
    _JRlRJgKaROOBKQaERKH = OByAaERKHpByAXSPAIPP();
    if (_JRaERKHqFWBpByA < _JRlRJgKaROOBKQaERKH) {
      KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x285);
    }
    if (_JRlRJgKaROOBKQaERKH == 0) goto LAB_00976fed;
  }
  else if (_JRlRJgKaROOBKQaERKH < _JRaROOBKQcKQOVpByA) {
LAB_00976fed:
    lVar12 = 0;
    lVar14 = _JRlRJgKaROOBKQaERKH;
LAB_00976f82:
    _JRaROOBKQcKQOVpByA = lVar14;
    _JRlRJgKaROOBKQaERKH = _JRaROOBKQcKQOVpByA;
    CFKFPEaERKHpByAXSPAIPP();
    if (lVar12 != param_4) {
      KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x2ae);
    }
    return lVar10;
  }
  lVar12 = 1;
  lVar14 = *(long *)(_JRdOLJlLABtB_pByA + _JRaROOBKQcKQOVpByA * 8);
  lVar13 = *(long *)(_JRrLlLABtB_pByA + _JRaROOBKQcKQOVpByA * 8);
  dVar15 = *(double *)(_JRmCCbFyDtB_pByA + _JRaROOBKQcKQOVpByA * 8);
  if (param_3 < lVar14) goto LAB_00977063;
  do {
    if (lVar13 <= lVar14) goto LAB_00977063;
    while( true ) {
      lVar1 = lVar14 * 8;
      plVar2 = (long *)(lVar4 + lVar1);
      lVar9 = *plVar2;
      if (lVar9 == 0) {
        *plVar2 = lVar13;
        *(double *)(lVar5 + lVar14 * 8) = dVar15;
      }
      else if (lVar13 == lVar9) {
        pdVar11 = (double *)(lVar1 + lVar5);
        *pdVar11 = dVar15 + *pdVar11;
      }
      else {
        plVar2 = (long *)(lVar3 + lVar1);
        lVar14 = *plVar2;
        if (lVar14 == 0) {
          *plVar2 = lVar13;
          *(double *)(lVar6 + lVar1) = dVar15;
        }
        else if (lVar13 == lVar14) {
          pdVar11 = (double *)(lVar1 + lVar6);
          *pdVar11 = dVar15 + *pdVar11;
        }
        else {
          plVar2 = (long *)(lVar7 + lVar1);
          lVar14 = *plVar2;
          if (lVar14 == 0) {
            *plVar2 = lVar13;
            *(double *)(lVar8 + lVar1) = dVar15;
          }
          else if (lVar13 == lVar14) {
            pdVar11 = (double *)(lVar1 + lVar8);
            *pdVar11 = dVar15 + *pdVar11;
          }
          else {
            KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x2a2);
          }
        }
      }
      _JRaROOBKQcKQOVpByA = _JRaROOBKQcKQOVpByA + 1;
      if (_JRaERKHqFWBpByA < _JRaROOBKQcKQOVpByA) {
        _JRaROOBKQcKQOVpByA = 1;
        _JRlRJgKaROOBKQaERKH = OByAaERKHpByAXSPAIPP();
        if (_JRaERKHqFWBpByA < _JRlRJgKaROOBKQaERKH) {
          KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x2a9);
        }
        lVar14 = _JRlRJgKaROOBKQaERKH;
        if (_JRlRJgKaROOBKQaERKH == 0) goto LAB_00976f82;
      }
      else {
        lVar14 = _JRlRJgKaROOBKQaERKH;
        if (_JRlRJgKaROOBKQaERKH < _JRaROOBKQcKQOVpByA) goto LAB_00976f82;
      }
      lVar12 = lVar12 + 1;
      lVar14 = *(long *)(_JRdOLJlLABtB_pByA + _JRaROOBKQcKQOVpByA * 8);
      lVar13 = *(long *)(_JRrLlLABtB_pByA + _JRaROOBKQcKQOVpByA * 8);
      dVar15 = *(double *)(_JRmCCbFyDtB_pByA + _JRaROOBKQcKQOVpByA * 8);
      if (lVar14 <= param_3) break;
LAB_00977063:
      KOBOOLOXSPAIPP(1,"deg123pfutils.c",0x289);
    }
  } while( true );
}


