/* driver: MLMYqQy_HXSPAIPP addr=009b1f60 size=22 */
#include "vsdlss_ref.h"


undefined8 MLMYqQy_HXSPAIPP(long param_1)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *(long *)(param_1 + 8);
  if (lVar1 != 0) {
    uVar2 = *(undefined8 *)(*(long *)(param_1 + 0x20) + lVar1 * 8);
    *(long *)(param_1 + 8) = lVar1 + -1;
    return uVar2;
  }
  return 0xffffffffffffffff;
}


