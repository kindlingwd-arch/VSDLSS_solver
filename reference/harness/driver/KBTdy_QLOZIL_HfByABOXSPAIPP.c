/* driver: KBTdy_QLOZIL_HfByABOXSPAIPP addr=00985940 size=85 */
#include "vsdlss_ref.h"


undefined8 * KBTdy_QLOZIL_HfByABOXSPAIPP(undefined8 param_1)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x50);
  *puVar1 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0;
  puVar1[5] = 0;
  puVar1[6] = 0;
  puVar1[7] = 0;
  uVar2 = KBTgntB_QLOXSPAIPP(param_1,1,0);
  puVar1[8] = uVar2;
  uVar2 = KBTdntB_QLOXSPAIPP(param_1,1,0);
  puVar1[9] = uVar2;
  return puVar1;
}


