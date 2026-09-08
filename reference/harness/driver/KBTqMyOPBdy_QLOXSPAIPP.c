/* driver: KBTqMyOPBdy_QLOXSPAIPP addr=009a2520 size=bb */
#include "vsdlss_ref.h"


undefined8 * KBTqMyOPBdy_QLOXSPAIPP(long param_1,char param_2)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  
  uVar1 = _OByQBryDXSPAIPP();
  puVar2 = (undefined8 *)KBTZIL_HXSPAIPP(uVar1,0x60);
  *puVar2 = uVar1;
  puVar2[1] = 0;
  puVar2[2] = param_1;
  *(char *)(puVar2 + 5) = param_2;
  *(undefined1 *)(puVar2 + 9) = 0;
  *(undefined1 *)((long)puVar2 + 0x29) = 0;
  uVar3 = QyDlBTgtB_QLOXSPAIPP(uVar1,param_1,0,0);
  puVar2[8] = uVar3;
  uVar3 = KBTZIL_HXSPAIPP(uVar1,param_1 * 8 + 8);
  puVar2[0xb] = uVar3;
  puVar2[6] = 0;
  if ((1 < (byte)(param_2 - 2U)) && (param_2 != '\0')) {
    puVar2[7] = 0;
    return puVar2;
  }
  uVar1 = QyDlBTdtB_QLOXSPAIPP(0,uVar1,param_1,1);
  puVar2[7] = uVar1;
  return puVar2;
}


