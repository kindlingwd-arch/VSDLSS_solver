/* driver: ABIBQBnNFXSPAIPP addr=009a7230 size=69 */
#include "vsdlss_ref.h"


undefined8 ABIBQBnNFXSPAIPP(long *param_1)

{
  if (param_1 != (long *)0x0) {
    if (*param_1 == -1) {
      if (param_1[3] != 0) {
        _yPFdOBBXSPAIPP();
      }
      param_1[3] = 0;
      if (param_1[4] != 0) {
        _yPFdOBBXSPAIPP();
      }
      param_1[4] = 0;
      if (param_1[5] != 0) {
        _yPFdOBBXSPAIPP();
      }
      param_1[5] = 0;
      _yPFdOBBXSPAIPP(param_1);
      return 0;
    }
    ABIBQBryDXSPAIPP();
  }
  return 0;
}


