/* driver: QyDlBTgKAtyItB_PXSPAIPP addr=009a2490 size=46 */
#include "vsdlss_ref.h"


undefined8 * QyDlBTgKAtyItB_PXSPAIPP(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)KBTZIL_HXSPAIPP(param_1,0x10);
  uVar2 = QyDlBTgtB_QLOXSPAIPP(param_1,param_2,0,0);
  *puVar1 = uVar2;
  uVar2 = QyDlBTdtB_QLOXSPAIPP(0,param_1,param_2,0);
  puVar1[1] = uVar2;
  return puVar1;
}


