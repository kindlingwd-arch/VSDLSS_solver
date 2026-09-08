/* driver: KBTdy_QLOaERKHXSPAIPP addr=00985bc0 size=106 */
#include "vsdlss_ref.h"


undefined8 *
KBTdy_QLOaERKHXSPAIPP(undefined8 param_1,long param_2,long param_3,long param_4,long param_5)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  
  uVar2 = _OByQBryDXSPAIPP();
  puVar3 = (undefined8 *)KBTZIL_HXSPAIPP(uVar2,0x90);
  puVar3[7] = param_5;
  lVar1 = (param_5 - param_4) + 1;
  puVar3[4] = param_3;
  *puVar3 = uVar2;
  puVar3[1] = param_1;
  puVar3[3] = param_2;
  puVar3[2] = (param_3 - param_2) + 1;
  puVar3[6] = param_4;
  puVar3[5] = lVar1;
  uVar4 = KBTgtB_QLOXSPAIPP(lVar1,1,0);
  puVar3[8] = uVar4;
  uVar4 = QyDlBTgtB_QLOXSPAIPP(uVar2,puVar3[2],1,0);
  puVar3[9] = uVar4;
  uVar4 = QyDlBTgntB_QLOXSPAIPP(uVar2,puVar3[2],1,0);
  puVar3[10] = uVar4;
  uVar2 = QyDlBTdntB_QLOXSPAIPP(uVar2,puVar3[2],1,0);
  puVar3[0xb] = uVar2;
  puVar3[0xc] = 0;
  puVar3[0xd] = 0;
  puVar3[0xe] = 0;
  puVar3[0xf] = 0;
  puVar3[0x11] = puVar3[4];
  puVar3[0x10] = 0;
  return puVar3;
}


