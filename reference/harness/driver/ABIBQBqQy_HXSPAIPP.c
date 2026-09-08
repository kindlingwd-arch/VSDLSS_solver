/* driver: ABIBQBqQy_HXSPAIPP addr=009b2800 size=5 */
#include "vsdlss_ref.h"


undefined8 ABIBQBqQy_HXSPAIPP(long param_1)

{
  if (param_1 != 0) {
    ABIBQBcKQFOBacADBIFPQXSPAIPP(*(undefined8 *)(param_1 + 8));
    *(undefined8 *)(param_1 + 8) = 0;
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


