/* driver: ABIBQBoXSPAIPP addr=009a7ce0 size=2b */
#include "vsdlss_ref.h"


undefined8 ABIBQBoXSPAIPP(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 0x18) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x18) = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


