/* driver: PBQdy_QLOZIL_HnQOPXSPAIPP addr=00986b90 size=8c */
#include "vsdlss_ref.h"


void PBQdy_QLOZIL_HnQOPXSPAIPP(long param_1,long param_2,long param_3,long *param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  
  lVar1 = *(long *)(param_3 + 0x50);
  lVar2 = *(long *)(param_3 + 0x58);
  lVar3 = param_4[9];
  lVar8 = *(long *)(*(long *)(param_2 + 0x10) + param_1 * 8);
  lVar4 = *(long *)(*(long *)(param_2 + 0x18) + param_1 * 8);
  lVar5 = *(long *)(param_3 + 0x18);
  lVar6 = *(long *)(param_2 + 0x20);
  param_4[1] = lVar8;
  lVar7 = param_4[8];
  lVar9 = lVar8 + lVar4 + -1;
  param_4[2] = lVar9;
  lVar9 = lVar9 - lVar8;
  lVar4 = lVar9 + 1;
  *param_4 = lVar4;
  param_4[3] = *(long *)(lVar6 + param_1 * 8);
  if (0 < lVar4) {
    lVar4 = (lVar8 - lVar5) * 8;
    lVar8 = 1;
    do {
      *(undefined8 *)(lVar7 + lVar8 * 8) = *(undefined8 *)(lVar1 + lVar4 + lVar8 * 8);
      *(undefined8 *)(lVar3 + lVar8 * 8) = *(undefined8 *)(lVar4 + lVar2 + lVar8 * 8);
      lVar8 = lVar8 + 1;
    } while (lVar8 != lVar9 + 2);
  }
  return;
}


