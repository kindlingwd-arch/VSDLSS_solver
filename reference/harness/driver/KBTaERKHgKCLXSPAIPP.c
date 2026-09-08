/* driver: KBTaERKHgKCLXSPAIPP addr=00974380 size=16f */
#include "vsdlss_ref.h"


undefined1 * KBTaERKHgKCLXSPAIPP(undefined8 param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  undefined1 *puVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  
  puVar2 = (undefined1 *)_yPFkyIIL_XSPAIPP(0x80);
  *(undefined8 *)(puVar2 + 8) = param_1;
  *(long *)(puVar2 + 0x20) = param_4;
  *puVar2 = 0;
  *(long *)(puVar2 + 0x10) = param_2;
  *(long *)(puVar2 + 0x18) = param_3;
  lVar5 = 1;
  lVar3 = KBTantB_QLOXSPAIPP(param_3,1,0);
  *(long *)(puVar2 + 0x28) = lVar3;
  if (0 < param_3) {
    while( true ) {
      lVar1 = lVar5 * 8;
      lVar5 = lVar5 + 1;
      uVar4 = KBTatB_QLOXSPAIPP(param_4 + 1,1,0);
      *(undefined8 *)(lVar3 + lVar1) = uVar4;
      if (param_3 < lVar5) break;
      lVar3 = *(long *)(puVar2 + 0x28);
    }
  }
  uVar4 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x30) = uVar4;
  uVar4 = KBTdFIBnQOtB_QLOXSPAIPP(param_2,1,0);
  *(undefined8 *)(puVar2 + 0x38) = uVar4;
  uVar4 = KBTdFIBnQOtB_QLOXSPAIPP(param_2,1,0);
  *(undefined8 *)(puVar2 + 0x40) = uVar4;
  uVar4 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x48) = uVar4;
  uVar4 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x50) = uVar4;
  uVar4 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x58) = uVar4;
  uVar4 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x60) = uVar4;
  uVar4 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x68) = uVar4;
  uVar4 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x70) = uVar4;
  uVar4 = KBTjgtB_QLOXSPAIPP(param_2,0,0);
  *(undefined8 *)(puVar2 + 0x78) = uVar4;
  if (0 < param_2) {
    lVar3 = *(long *)(puVar2 + 0x30);
    lVar5 = 0;
    do {
      *(long *)(lVar3 + 8 + lVar5 * 8) = lVar5 % param_3 + 1;
      lVar5 = lVar5 + 1;
    } while (lVar5 != param_2);
  }
  *puVar2 = 1;
  return puVar2;
}


