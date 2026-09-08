/* driver: DBQlBUQdOLJrLXSPAIPP addr=00987ba0 size=a0 */
#include "vsdlss_ref.h"


undefined8 DBQlBUQdOLJrLXSPAIPP(undefined8 *param_1,undefined8 *param_2)

{
  long lVar1;
  
  if (0 < CQOTlRJgQBJPjBCQ) {
    CQOTaROOBKQcKQOVpByA = CQOTaROOBKQcKQOVpByA + 1;
    if (CQOTaERKHqFWBpByA < CQOTaROOBKQcKQOVpByA) {
      OByAdOLJrL();
      CQOTaROOBKQcKQOVpByA = 1;
      lVar1 = 8;
    }
    else {
      lVar1 = CQOTaROOBKQcKQOVpByA * 8;
    }
    *param_1 = *(undefined8 *)(CQOTdOLJlLABtB_pByA + lVar1);
    *param_2 = *(undefined8 *)(CQOTrLlLABtB_pByA + CQOTaROOBKQcKQOVpByA * 8);
    CQOTlRJgQBJPjBCQ = CQOTlRJgQBJPjBCQ + -1;
    return 0;
  }
  *param_1 = 0xffffffffffffffff;
  *param_2 = 0xffffffffffffffff;
  return 1;
}


