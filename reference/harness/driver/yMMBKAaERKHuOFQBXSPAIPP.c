/* driver: yMMBKAaERKHuOFQBXSPAIPP addr=00975060 size=65 */
#include "vsdlss_ref.h"


void yMMBKAaERKHuOFQBXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  *(undefined8 *)(_JRdOLJlLABtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = param_2;
  *(undefined8 *)(_JRrLlLABtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = param_3;
  *(undefined8 *)(_JRmCCbFyDtB_uOFQB + _JRaROOBKQcKQOVuOFQB * 8) = param_1;
  _JRaROOBKQcKQOVuOFQB = _JRaROOBKQcKQOVuOFQB + 1;
  if (_JRaROOBKQcKQOVuOFQB <= _JRaERKHqFWBuOFQB) {
    return;
  }
  TOFQBaERKHuOFQBXSPAIPP();
  _JRaROOBKQcKQOVuOFQB = 1;
  return;
}


