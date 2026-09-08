/* driver: KBTjFPQkyOHXSPAIPP addr=0098b640 size=46 */
#include "vsdlss_ref.h"


undefined8 * KBTjFPQkyOHXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x20);
  *puVar1 = param_1;
  uVar2 = KBTatB_QLOXSPAIPP(param_1,1,0);
  puVar1[2] = 0;
  puVar1[1] = uVar2;
  puVar1[3] = 0;
  return puVar1;
}


