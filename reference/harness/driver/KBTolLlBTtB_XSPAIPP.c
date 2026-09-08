/* driver: KBTolLlBTtB_XSPAIPP addr=009a7c20 size=34 */
#include "vsdlss_ref.h"


void KBTolLlBTtB_XSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x20);
  puVar1[2] = param_1;
  puVar1[3] = param_2;
  *puVar1 = 0;
  puVar1[1] = 0;
  return;
}


