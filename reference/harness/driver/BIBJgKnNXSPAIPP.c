/* driver: BIBJgKnNXSPAIPP addr=009a6fd0 size=61 */
#include "vsdlss_ref.h"


undefined8 BIBJgKnNXSPAIPP(long param_1,long param_2,undefined8 *param_3)

{
  long lVar1;
  
  if ((param_2 < 1) || (*(long *)(param_1 + 8) < param_2)) {
    KOBOOLOXSPAIPP(1,"priorityqutils.c",0x274);
    lVar1 = *(long *)(*(long *)(param_1 + 0x18) + param_2 * 8);
  }
  else {
    lVar1 = *(long *)(*(long *)(param_1 + 0x18) + param_2 * 8);
  }
  if (-1 < lVar1) {
    *param_3 = *(undefined8 *)(*(long *)(param_1 + 0x28) + param_2 * 8);
    return 1;
  }
  return 0;
}


