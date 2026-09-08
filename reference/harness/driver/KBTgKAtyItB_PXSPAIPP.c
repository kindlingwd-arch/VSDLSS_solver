/* driver: KBTgKAtyItB_PXSPAIPP addr=009a2440 size=41 */
#include "vsdlss_ref.h"


undefined8 * KBTgKAtyItB_PXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x10);
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,0,0);
  *puVar1 = uVar2;
  uVar2 = KBTtB_QLOXSPAIPP(0,param_1,0);
  puVar1[1] = uVar2;
  return puVar1;
}


