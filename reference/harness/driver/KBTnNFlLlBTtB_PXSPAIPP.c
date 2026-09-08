/* driver: KBTnNFlLlBTtB_PXSPAIPP addr=009a70e0 size=71 */
#include "vsdlss_ref.h"


undefined8 *
KBTnNFlLlBTtB_PXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  
  uVar1 = _OByQBryDXSPAIPP();
  puVar2 = (undefined8 *)KBTZIL_HXSPAIPP(uVar1,0x30);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  puVar2[3] = param_2;
  puVar2[4] = param_3;
  puVar2[5] = param_4;
  puVar2[2] = 0;
  FKFQgtB_QLOXSPAIPP(param_2,param_1,0xffffffffffffffff);
  return puVar2;
}


