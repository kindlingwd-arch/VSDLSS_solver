/* driver: ABIBQBYEQXSPAIPP addr=009af650 size=6d */
#include "vsdlss_ref.h"


undefined8 ABIBQBYEQXSPAIPP(long param_1)

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
    if (*(long *)(param_1 + 0x30) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x30) = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


