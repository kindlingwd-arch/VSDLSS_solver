/* driver: KBTgKAFPQlLABqBQPXSPAIPP addr=009893f0 size=36 */
#include "vsdlss_ref.h"


undefined8 * KBTgKAFPQlLABqBQPXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x10);
  *puVar1 = param_1;
  uVar2 = KBTacADBtB_QLOXSPAIPP(param_1,1,0);
  puVar1[1] = uVar2;
  return puVar1;
}


