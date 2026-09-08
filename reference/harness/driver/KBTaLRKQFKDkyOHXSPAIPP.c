/* driver: KBTaLRKQFKDkyOHXSPAIPP addr=009754c0 size=3e */
#include "vsdlss_ref.h"


undefined8 * KBTaLRKQFKDkyOHXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x18);
  puVar1[1] = param_1;
  *puVar1 = 1;
  uVar2 = KBTgtB_QLOXSPAIPP(param_1,1,0);
  puVar1[2] = uVar2;
  return puVar1;
}


