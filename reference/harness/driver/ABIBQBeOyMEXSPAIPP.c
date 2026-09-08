/* driver: ABIBQBeOyMEXSPAIPP addr=009a2240 size=c4 */
#include "vsdlss_ref.h"


undefined8 ABIBQBeOyMEXSPAIPP(long *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  
  if (param_1 == (long *)0x0) {
    return 0;
  }
  if (param_1[2] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[2] = 0;
  if (param_1[3] != 0) {
    _yPFdOBBXSPAIPP();
  }
  lVar5 = param_1[4];
  param_1[3] = 0;
  if (lVar5 != 0) {
    if (0 < *param_1) {
      lVar4 = 1;
      do {
        puVar1 = (undefined8 *)(lVar5 + lVar4 * 8);
        lVar4 = lVar4 + 1;
        uVar3 = ABIBQBcKQFOBacADBIFPQXSPAIPP(*puVar1);
        lVar2 = *param_1;
        *puVar1 = uVar3;
        lVar5 = param_1[4];
      } while (lVar4 <= lVar2);
      if (lVar5 == 0) goto LAB_009a22c1;
    }
    _yPFdOBBXSPAIPP();
  }
LAB_009a22c1:
  param_1[4] = 0;
  if (param_1[5] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[5] = 0;
  if (param_1[6] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[6] = 0;
  _yPFdOBBXSPAIPP(param_1);
  return 0;
}


