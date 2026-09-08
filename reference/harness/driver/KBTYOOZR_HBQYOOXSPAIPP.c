/* driver: KBTYOOZR_HBQYOOXSPAIPP addr=009b17f0 size=a2 */
#include "vsdlss_ref.h"


undefined8 * KBTYOOZR_HBQYOOXSPAIPP(long param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  
  uVar1 = _OByQBryDXSPAIPP();
  puVar2 = (undefined8 *)KBTZIL_HXSPAIPP(uVar1,0x28);
  *puVar2 = uVar1;
  puVar2[1] = param_1;
  lVar3 = KBTZIL_HXSPAIPP(uVar1,(param_1 + 1) * 8);
  puVar2[2] = lVar3;
  lVar5 = 1;
  if (0 < param_1) {
    while( true ) {
      *(undefined8 *)(lVar3 + lVar5 * 8) = 0;
      lVar5 = lVar5 + 1;
      if (lVar5 == param_1 + 1) break;
      lVar3 = puVar2[2];
    }
  }
  uVar4 = QyDlBTgtB_QLOXSPAIPP(uVar1,param_1,1,0);
  puVar2[3] = uVar4;
  uVar1 = QyDlBTgtB_QLOXSPAIPP(uVar1,param_1,1,0);
  puVar2[4] = uVar1;
  return puVar2;
}


