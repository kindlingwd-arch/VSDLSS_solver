/* driver: yMMBKAdOLJrLmCCbFyDXSPAIPP addr=009875e0 size=76 */
#include "vsdlss_ref.h"


void yMMBKAdOLJrLmCCbFyDXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  long lVar1;
  
  *(undefined8 *)(CQOTdOLJlLABtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = param_2;
  *(undefined8 *)(CQOTrLlLABtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = param_3;
  *(undefined8 *)(CQOTmCCbFyDtB_uOFQB + CQOTaROOBKQcKQOVuOFQB * 8) = param_1;
  lVar1 = CQOTaROOBKQcKQOVuOFQB + 1;
  if (CQOTaERKHqFWBuOFQB < lVar1) {
    if (CQOTaROOBKQcKQOVuOFQB < 1) {
      CQOTaROOBKQcKQOVuOFQB = 1;
      return;
    }
    CQOTaROOBKQcKQOVuOFQB = lVar1;
    FUN_00987320();
    CQOTaROOBKQcKQOVuOFQB = 1;
    lVar1 = CQOTaROOBKQcKQOVuOFQB;
  }
  CQOTaROOBKQcKQOVuOFQB = lVar1;
  return;
}


