/* driver: KBToXSPAIPP addr=009a7ba0 size=43 */
#include "vsdlss_ref.h"


undefined8 * KBToXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x20);
  puVar1[2] = param_1;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,0,0);
  *puVar1 = 0;
  puVar1[3] = uVar2;
  puVar1[1] = 0;
  return puVar1;
}


