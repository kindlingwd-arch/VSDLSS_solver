/* driver: DBQlBUQaERKHpByAXSPAIPP addr=00975370 size=f3 */
#include "vsdlss_ref.h"


undefined8 DBQlBUQaERKHpByAXSPAIPP(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3)

{
  _JRaROOBKQcKQOVpByA = _JRaROOBKQcKQOVpByA + 1;
  if (_JRaERKHqFWBpByA < _JRaROOBKQcKQOVpByA) {
    _JRaROOBKQcKQOVpByA = 1;
    _JRlRJgKaROOBKQaERKH = OByAaERKHpByAXSPAIPP();
    if (_JRaERKHqFWBpByA < _JRlRJgKaROOBKQaERKH) {
      KOBOOLOXSPAIPP(1,"chunkmatrixutils.c",0x113);
    }
    if (_JRlRJgKaROOBKQaERKH == 0) goto LAB_0097540d;
  }
  else if (_JRlRJgKaROOBKQaERKH < _JRaROOBKQcKQOVpByA) {
LAB_0097540d:
    *param_1 = 0xffffffffffffffff;
    *param_2 = 0xffffffffffffffff;
    _JRaROOBKQcKQOVpByA = _JRlRJgKaROOBKQaERKH;
    *param_3 = 0;
    return 1;
  }
  *param_1 = *(undefined8 *)(_JRdOLJlLABtB_pByA + _JRaROOBKQcKQOVpByA * 8);
  *param_2 = *(undefined8 *)(_JRrLlLABtB_pByA + _JRaROOBKQcKQOVpByA * 8);
  *param_3 = *(undefined8 *)(_JRmCCbFyDtB_pByA + _JRaROOBKQcKQOVpByA * 8);
  return 0;
}


