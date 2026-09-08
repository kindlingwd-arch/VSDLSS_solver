/* driver: ABIBQBZMNXSPAIPP addr=009b20a0 size=57 */
#include "vsdlss_ref.h"


undefined8 ABIBQBZMNXSPAIPP(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 0x30) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x30) = 0;
    if (*(long *)(param_1 + 0x38) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    if (*(long *)(param_1 + 0x40) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x40) = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


