/* driver: ABIBQBfyPEryzIBXSPAIPP addr=009b2940 size=59 */
#include "vsdlss_ref.h"


undefined8 ABIBQBfyPEryzIBXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  if (param_1 != (long *)0x0) {
    lVar1 = *param_1;
    lVar2 = 1;
    if (0 < lVar1) {
      do {
        lVar3 = lVar2 + 1;
        ABIBQBZR_HBQdOLJfQXSPAIPP(lVar2,param_1);
        lVar2 = lVar3;
      } while (lVar3 <= lVar1);
    }
    if (param_1[1] != 0) {
      _yPFdOBBXSPAIPP();
    }
    param_1[1] = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


