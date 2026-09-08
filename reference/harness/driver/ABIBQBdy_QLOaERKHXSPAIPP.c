/* driver: ABIBQBdy_QLOaERKHXSPAIPP addr=00985cd0 size=2b */
#include "vsdlss_ref.h"


undefined8 ABIBQBdy_QLOaERKHXSPAIPP(undefined8 *param_1)

{
  if (param_1 != (undefined8 *)0x0) {
    if (param_1[8] != 0) {
      _yPFdOBBXSPAIPP();
    }
    param_1[8] = 0;
    ABIBQBryDXSPAIPP(*param_1);
  }
  return 0;
}


