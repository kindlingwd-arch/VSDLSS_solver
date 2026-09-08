/* driver: ABIBQBkyQOFUXSPAIPP addr=00978010 size=50 */
#include "vsdlss_ref.h"


undefined8 ABIBQBkyQOFUXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  
  if (param_1 != 0) {
    lVar1 = 1;
    if (0 < param_2) {
      do {
        if (*(long *)(param_1 + lVar1 * 8) != 0) {
          _yPFdOBBXSPAIPP();
        }
        *(undefined8 *)(param_1 + lVar1 * 8) = 0;
        lVar1 = lVar1 + 1;
      } while (lVar1 <= param_2);
    }
    _yPFdOBBXSPAIPP(param_1);
  }
  return 0;
}


