/* driver: KBTeOyMEXSPAIPP addr=009a21f0 size=43 */
#include "vsdlss_ref.h"


void KBTeOyMEXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x38);
  *puVar1 = param_1;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0;
  puVar1[5] = 0;
  puVar1[6] = 0;
  return;
}


