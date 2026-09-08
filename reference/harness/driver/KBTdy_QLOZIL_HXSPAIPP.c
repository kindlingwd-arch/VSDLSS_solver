/* driver: KBTdy_QLOZIL_HXSPAIPP addr=009859d0 size=9d */
#include "vsdlss_ref.h"


undefined8 * KBTdy_QLOZIL_HXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x50);
  puVar1[4] = param_2;
  puVar1[5] = param_3;
  *puVar1 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  uVar2 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  puVar1[6] = uVar2;
  uVar2 = KBTdtB_QLOXSPAIPP(0,param_3,0);
  puVar1[7] = uVar2;
  uVar2 = KBTgntB_QLOXSPAIPP(param_1,1,0);
  puVar1[8] = uVar2;
  uVar2 = KBTdntB_QLOXSPAIPP(param_1,1,0);
  puVar1[9] = uVar2;
  return puVar1;
}


