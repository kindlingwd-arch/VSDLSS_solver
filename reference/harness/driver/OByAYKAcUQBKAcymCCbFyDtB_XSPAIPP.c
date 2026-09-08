/* driver: OByAYKAcUQBKAcymCCbFyDtB_XSPAIPP addr=009ac580 size=ec */
#include "vsdlss_ref.h"


void OByAYKAcUQBKAcymCCbFyDtB_XSPAIPP(long param_1,long param_2,long param_3,long param_4)

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
  
  lVar1 = *(long *)(param_3 + 0x10);
  lVar2 = *(long *)(param_3 + 0x40);
  lVar6 = KBTdtB_QLOXSPAIPP(0,param_1,0);
  *(long *)(param_4 + 0x40) = lVar6;
  OByAdtB_QLOdlRJXSPAIPP(0x1d,param_1 - param_2,lVar6);
  if (lVar1 < 1) {
    lVar9 = 0;
  }
  else {
    lVar3 = *(long *)(param_3 + 0x58);
    lVar9 = 0;
    lVar8 = 1;
    do {
      lVar4 = *(long *)(lVar2 + lVar8 * 8);
      lVar5 = *(long *)(*(long *)(lVar3 + lVar8 * 8) + 8);
      if (0 < lVar4) {
        lVar7 = 1;
        do {
          *(undefined8 *)(lVar6 + (param_1 - param_2) * 8 + lVar9 * 8 + lVar7 * 8) =
               *(undefined8 *)(lVar5 + lVar7 * 8);
          lVar7 = lVar7 + 1;
        } while (lVar7 != lVar4 + 1);
        lVar9 = lVar9 + lVar4;
      }
      lVar8 = lVar8 + 1;
    } while (lVar8 != lVar1 + 1);
  }
  if (lVar9 != param_2) {
    KOBOOLOXSPAIPP(1,"reduceproblem.c",0x573);
    return;
  }
  return;
}


