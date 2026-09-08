/* driver: ANXSPAIPP addr=009a7e00 size=38 */
#include "vsdlss_ref.h"


undefined8 ANXSPAIPP(long *param_1)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *param_1;
  if (param_1[1] == lVar1) {
    uVar2 = 0xffffffffffffffff;
  }
  else {
    uVar2 = *(undefined8 *)(param_1[3] + lVar1 * 8);
    *param_1 = lVar1 + 1;
    if (param_1[2] < lVar1 + 1) {
      *param_1 = 0;
      return uVar2;
    }
  }
  return uVar2;
}


