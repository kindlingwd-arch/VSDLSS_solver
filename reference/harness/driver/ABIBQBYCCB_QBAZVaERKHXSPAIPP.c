/* driver: ABIBQBYCCB_QBAZVaERKHXSPAIPP addr=0096eec0 size=41 */
#include "vsdlss_ref.h"


undefined8 ABIBQBYCCB_QBAZVaERKHXSPAIPP(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 8) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 8) = 0;
    if (*(long *)(param_1 + 0x10) != 0) {
      _yPFdOBBXSPAIPP();
    }
    *(undefined8 *)(param_1 + 0x10) = 0;
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


