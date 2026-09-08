/* driver: KBTqQy_HXSPAIPP addr=009b27f0 size=5 */
#include "vsdlss_ref.h"


undefined8 * KBTqQy_HXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x20);
  *puVar1 = param_1;
  uVar2 = KBTacADBIFPQXSPAIPP(param_1,0);
  puVar1[1] = uVar2;
  puVar1[2] = uVar2;
  puVar1[3] = uVar2;
  return puVar1;
}


