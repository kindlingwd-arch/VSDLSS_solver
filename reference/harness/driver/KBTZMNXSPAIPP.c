/* driver: KBTZMNXSPAIPP addr=009b2010 size=87 */
#include "vsdlss_ref.h"


undefined8 * KBTZMNXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x48);
  *puVar1 = param_1;
  puVar1[1] = param_1;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0x7ffffff5;
  puVar1[5] = 0x7ffffff5;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0x7ffffff5);
  puVar1[6] = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,puVar1[4]);
  puVar1[7] = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,puVar1[4]);
  puVar1[8] = uVar2;
  return puVar1;
}


