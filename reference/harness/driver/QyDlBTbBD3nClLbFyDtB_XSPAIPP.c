/* driver: QyDlBTbBD3nClLbFyDtB_XSPAIPP addr=00975a90 size=f1 */
#include "vsdlss_ref.h"


undefined8 * QyDlBTbBD3nClLbFyDtB_XSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  
  puVar1 = (undefined8 *)KBTZIL_HXSPAIPP(param_1,0x48);
  *puVar1 = param_2;
  puVar1[1] = param_3;
  puVar1[2] = 0;
  puVar2 = (undefined8 *)QyDlBTgtB_QLOXSPAIPP(param_1,param_3,1,0);
  puVar1[3] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)QyDlBTdtB_QLOXSPAIPP(0,param_1,param_3,1);
  puVar1[4] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)QyDlBTgtB_QLOXSPAIPP(param_1,param_3,1,0);
  puVar1[5] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)QyDlBTdtB_QLOXSPAIPP(0,param_1,param_3,1);
  puVar1[6] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)QyDlBTgtB_QLOXSPAIPP(param_1,param_3,1,0);
  puVar1[7] = puVar2;
  *puVar2 = 0;
  puVar2 = (undefined8 *)QyDlBTdtB_QLOXSPAIPP(0,param_1,param_3,1);
  puVar1[8] = puVar2;
  *puVar2 = 0;
  return puVar1;
}


