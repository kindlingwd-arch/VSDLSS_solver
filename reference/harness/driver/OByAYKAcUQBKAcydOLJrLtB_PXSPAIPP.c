/* driver: OByAYKAcUQBKAcydOLJrLtB_PXSPAIPP addr=009ac410 size=167 */
#include "vsdlss_ref.h"


void OByAYKAcUQBKAcydOLJrLtB_PXSPAIPP
               (long param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

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
  long lVar10;
  long lVar11;
  
  lVar11 = param_2 - param_4;
  lVar1 = *(long *)(param_5 + 0x10);
  lVar2 = *(long *)(param_5 + 0x40);
  lVar6 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(long *)(param_6 + 0x28) = lVar6;
  lVar7 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  *(long *)(param_6 + 0x38) = lVar7;
  OByAgtB_QLOdlRJXSPAIPP(0x1b,lVar11,lVar6);
  OByAgtB_QLOdlRJXSPAIPP(0x1c,lVar11,lVar7);
  _EB_HdOLJYKArLlLABPXSPAIPP(param_1 - param_3,lVar11,lVar6,lVar7);
  if (lVar1 < 1) {
    lVar8 = 0;
  }
  else {
    lVar10 = 1;
    lVar8 = 0;
    lVar3 = *(long *)(param_5 + 0x58);
    do {
      lVar4 = *(long *)(lVar2 + lVar10 * 8);
      lVar5 = **(long **)(lVar3 + lVar10 * 8);
      if (0 < lVar4) {
        lVar9 = 1;
        do {
          *(undefined8 *)(lVar6 + lVar11 * 8 + lVar8 * 8 + lVar9 * 8) =
               *(undefined8 *)(lVar5 + lVar9 * 8);
          *(long *)(lVar8 * 8 + lVar7 + lVar11 * 8 + lVar9 * 8) = (param_1 - param_3) + lVar10;
          lVar9 = lVar9 + 1;
        } while (lVar9 != lVar4 + 1);
        lVar8 = lVar8 + lVar4;
      }
      lVar10 = lVar10 + 1;
    } while (lVar10 != lVar1 + 1);
  }
  if ((lVar8 == param_4) && (lVar1 == param_3)) {
    return;
  }
  KOBOOLOXSPAIPP(1,"reduceproblem.c",0x551);
  return;
}


