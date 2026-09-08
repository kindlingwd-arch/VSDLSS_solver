/* driver: KBTYqQy_HXSPAIPP addr=009b1d40 size=62 */
#include "vsdlss_ref.h"


undefined8 * KBTYqQy_HXSPAIPP(long param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  long lVar3;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x28);
  *puVar1 = 0xffffffffffffffff;
  lVar3 = 0x20;
  if (0x83f < param_1) {
    lVar3 = param_1 >> 6;
  }
  puVar1[3] = param_1;
  if (param_1 < lVar3) {
    lVar3 = param_1;
  }
  puVar1[2] = lVar3;
  uVar2 = KBTgtB_QLOXSPAIPP(lVar3,0,0);
  puVar1[1] = 0;
  puVar1[4] = uVar2;
  return puVar1;
}


