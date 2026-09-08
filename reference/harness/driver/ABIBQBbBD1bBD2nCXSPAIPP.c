/* driver: ABIBQBbBD1bBD2nCXSPAIPP addr=00975830 size=57 */
#include "vsdlss_ref.h"


undefined8 ABIBQBbBD1bBD2nCXSPAIPP(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 0x18) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x18) = 0;
    if (*(long *)(param_1 + 0x20) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x20) = 0;
    if (*(long *)(param_1 + 0x28) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x28) = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


