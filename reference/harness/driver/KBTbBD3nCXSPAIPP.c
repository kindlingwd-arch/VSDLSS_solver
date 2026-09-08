/* driver: KBTbBD3nCXSPAIPP addr=00975890 size=e4 */
#include "vsdlss_ref.h"


undefined8 * KBTbBD3nCXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  
  puVar1 = (undefined8 *)_yPFkyIIL_XSPAIPP(0x48);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  puVar2 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,param_2,1);
  puVar1[2] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)KBTgtB_QLOXSPAIPP(param_2,1,0);
  puVar1[3] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,param_2,1);
  puVar1[4] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)KBTgtB_QLOXSPAIPP(param_2,1,0);
  puVar1[5] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,param_2,1);
  puVar1[6] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)KBTgtB_QLOXSPAIPP(param_2,1,0);
  puVar1[7] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,param_2,1);
  puVar1[8] = puVar2;
  *puVar2 = 0;
  return puVar1;
}


