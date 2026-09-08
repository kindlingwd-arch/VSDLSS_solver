/* driver: QyDlBTgttuFQEnQOPXSPAIPP addr=009a2410 size=24 */
#include "vsdlss_ref.h"


void QyDlBTgttuFQEnQOPXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)KBTZIL_HXSPAIPP(param_1,0x10);
  *puVar1 = param_2;
  puVar1[1] = param_3;
  return;
}


