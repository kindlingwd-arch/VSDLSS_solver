/* driver: ABIBQBgKAtyItB_PXSPAIPP addr=009a24e0 size=3f */
#include "vsdlss_ref.h"


undefined8 ABIBQBgKAtyItB_PXSPAIPP(long *param_1)

{
  if (param_1 != (long *)0x0) {
    if (*param_1 != 0) {
      _yPFdOBBXSPAIPP();
    }
    *param_1 = 0;
    if (param_1[1] != 0) {
      _yPFdOBBXSPAIPP();
    }
    param_1[1] = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


