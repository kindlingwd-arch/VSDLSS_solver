/* driver: KBTnyOQdy_QXSPAIPP addr=009a2610 size=85 */
#include "vsdlss_ref.h"


undefined8 * KBTnyOQdy_QXSPAIPP(undefined8 param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  
  uVar1 = _OByQBryDXSPAIPP();
  puVar2 = (undefined8 *)KBTZIL_HXSPAIPP(uVar1,0x28);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  puVar3 = (undefined8 *)QyDlBTgtB_QLOXSPAIPP(uVar1,param_1,1,0);
  puVar2[2] = puVar3;
  *puVar3 = 0;
  puVar3 = (undefined8 *)QyDlBTgntB_QLOXSPAIPP(uVar1,param_1,1,0);
  puVar2[3] = puVar3;
  *puVar3 = 0;
  puVar3 = (undefined8 *)QyDlBTbntB_QLOXSPAIPP(uVar1,param_1,1,0);
  puVar2[4] = puVar3;
  *puVar3 = 0;
  return puVar2;
}


