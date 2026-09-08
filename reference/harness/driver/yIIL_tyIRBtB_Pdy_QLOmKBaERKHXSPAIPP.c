/* driver: yIIL_tyIRBtB_Pdy_QLOmKBaERKHXSPAIPP addr=009698e0 size=106 */
#include "vsdlss_ref.h"


void yIIL_tyIRBtB_Pdy_QLOmKBaERKHXSPAIPP(undefined8 *param_1)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  long local_40;
  
  lVar1 = param_1[2];
  uVar2 = *param_1;
  lVar3 = param_1[9];
  if (lVar1 < 0x400) {
    lVar5 = QyDlBTdntB_QLOXSPAIPP(uVar2,lVar1,1,0);
    param_1[0xb] = lVar5;
    local_40 = 1;
    if (lVar1 < 1) {
      return;
    }
  }
  else {
    local_40 = lVar1 >> 10;
    lVar5 = QyDlBTdntB_QLOXSPAIPP(uVar2,lVar1,1,0);
    param_1[0xb] = lVar5;
  }
  lVar10 = 1;
  lVar9 = 0;
  lVar6 = 0;
  do {
    uVar4 = *(ulong *)(lVar3 + lVar10 * 8);
    lVar7 = lVar6;
    if (lVar9 < (long)uVar4) {
      lVar9 = uVar4 + 1;
      if ((long)uVar4 < local_40) {
        lVar9 = local_40;
      }
      lVar7 = KBTZIL_HXSPAIPP(uVar2,lVar9 * 8);
      lVar9 = lVar9 + -1;
    }
    lVar9 = lVar9 - uVar4;
    lVar6 = lVar7 + uVar4 * 8;
    uVar8 = 1;
    if (uVar4 != 0) {
      do {
        *(undefined8 *)(lVar7 + uVar8 * 8) = 0;
        uVar8 = uVar8 + 1;
      } while (uVar8 <= uVar4);
    }
    *(long *)(lVar5 + lVar10 * 8) = lVar7;
    lVar10 = lVar10 + 1;
  } while (lVar10 <= lVar1);
  return;
}


