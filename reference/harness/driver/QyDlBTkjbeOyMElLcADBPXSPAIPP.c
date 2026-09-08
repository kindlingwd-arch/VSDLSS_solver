/* driver: QyDlBTkjbeOyMElLcADBPXSPAIPP addr=00996a00 size=c7 */
#include "vsdlss_ref.h"


undefined8 * QyDlBTkjbeOyMElLcADBPXSPAIPP(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)KBTZIL_HXSPAIPP(param_1,0x90);
  *puVar1 = param_1;
  puVar1[1] = param_2;
  puVar1[2] = param_3;
  puVar1[3] = 0;
  uVar2 = QyDlBTgtB_QLOXSPAIPP(param_1,param_3,1,0);
  puVar1[10] = uVar2;
  puVar1[0xb] = 0;
  uVar2 = QyDlBTgntB_QLOXSPAIPP(param_1,param_3,1,0);
  puVar1[0xc] = uVar2;
  puVar1[0xd] = 0;
  uVar2 = QyDlBTgntB_QLOXSPAIPP(param_1,param_3,1,0);
  puVar1[0xe] = uVar2;
  puVar1[0xf] = 0;
  uVar2 = QyDlBTgtB_QLOXSPAIPP(param_1,param_3,0,0);
  puVar1[0x10] = uVar2;
  uVar2 = QyDlBTgtB_QLOXSPAIPP(param_1,param_3,0,0);
  puVar1[0x11] = uVar2;
  return puVar1;
}


