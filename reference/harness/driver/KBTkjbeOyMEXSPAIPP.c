/* driver: KBTkjbeOyMEXSPAIPP addr=009965d0 size=a6 */
#include "vsdlss_ref.h"


undefined8 * KBTkjbeOyMEXSPAIPP(undefined8 param_1,undefined8 param_2,long param_3)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x90);
  puVar1[1] = param_1;
  puVar1[2] = param_2;
  puVar1[3] = param_3;
  *puVar1 = 0xffffffffffffffff;
  uVar2 = KBTgtB_QLOXSPAIPP(param_2,1,0);
  puVar1[10] = uVar2;
  puVar1[0xb] = 0;
  uVar2 = KBTgntB_QLOXSPAIPP(param_2,1,0);
  puVar1[0xc] = uVar2;
  uVar2 = KBTgtB_QLOXSPAIPP(param_3 * 2,0,0);
  puVar1[0xe] = 0;
  puVar1[0xd] = uVar2;
  puVar1[0xf] = 0;
  puVar1[0x10] = 0;
  puVar1[0x11] = 0;
  return puVar1;
}


