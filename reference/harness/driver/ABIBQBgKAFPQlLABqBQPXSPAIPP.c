/* driver: ABIBQBgKAFPQlLABqBQPXSPAIPP addr=00989430 size=6c */
#include "vsdlss_ref.h"


undefined8 ABIBQBgKAFPQlLABqBQPXSPAIPP(long *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  
  if (param_1 != (long *)0x0) {
    lVar2 = *param_1;
    lVar4 = 1;
    if (0 < lVar2) {
      do {
        puVar1 = (undefined8 *)(param_1[1] + lVar4 * 8);
        lVar4 = lVar4 + 1;
        uVar3 = ABIBQBcKQFOBacADBIFPQXSPAIPP(*puVar1);
        *puVar1 = uVar3;
      } while (lVar4 <= lVar2);
    }
    if (param_1[1] != 0) {
      _yPFdOBBXSPAIPP();
    }
    param_1[1] = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


