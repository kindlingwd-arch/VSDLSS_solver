/* driver: MBOJRQBeOyMEXSPAIPP addr=00989170 size=101 */
#include "vsdlss_ref.h"


void MBOJRQBeOyMEXSPAIPP(long *param_1,long param_2)

{
  long lVar1;
  long lVar2;
  ulong *puVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  undefined8 uVar7;
  ulong uVar8;
  ulong uVar9;
  
  puVar3 = (ulong *)*param_1;
  uVar4 = *puVar3;
  lVar5 = KBTeOyMEXSPAIPP(uVar4);
  if (puVar3[2] == 0) {
    lVar6 = KBTacADBtB_QLOXSPAIPP(uVar4,0,0);
    *(long *)(lVar5 + 0x20) = lVar6;
    if (uVar4 == 0) goto LAB_0098922d;
  }
  else {
    lVar6 = KBTgtB_QLOXSPAIPP();
    *(long *)(lVar5 + 0x10) = lVar6;
    if (uVar4 == 0) {
      uVar7 = KBTacADBtB_QLOXSPAIPP(0,0,0);
      *(undefined8 *)(lVar5 + 0x20) = uVar7;
      goto LAB_0098922d;
    }
    uVar9 = puVar3[2];
    uVar8 = 1;
    do {
      lVar1 = uVar8 * 8;
      lVar2 = uVar8 * 8;
      uVar8 = uVar8 + 1;
      *(undefined8 *)(lVar6 + *(long *)(param_2 + lVar2) * 8) = *(undefined8 *)(uVar9 + lVar1);
    } while (uVar8 <= uVar4);
    lVar6 = KBTacADBtB_QLOXSPAIPP(uVar4,0,0);
    *(long *)(lVar5 + 0x20) = lVar6;
  }
  uVar9 = 1;
  while( true ) {
    lVar2 = uVar9 * 8;
    lVar1 = uVar9 * 8;
    uVar9 = uVar9 + 1;
    *(undefined8 *)(lVar6 + *(long *)(param_2 + lVar2) * 8) = *(undefined8 *)(puVar3[4] + lVar1);
    if (uVar4 < uVar9) break;
    lVar6 = *(long *)(lVar5 + 0x20);
  }
  uVar9 = 1;
  do {
    *(undefined8 *)(puVar3[4] + uVar9 * 8) = 0;
    uVar9 = uVar9 + 1;
  } while (uVar9 <= uVar4);
LAB_0098922d:
  ABIBQBeOyMEXSPAIPP(puVar3);
  OBKyJBlLABPgKYAGjFPQmCeOyMEXSPAIPP(lVar5,param_2);
  *param_1 = lVar5;
  return;
}


