/* driver: MNgK_OByPBAiBVgQBOXSPAIPP addr=009a6a70 size=bc */
#include "vsdlss_ref.h"


void MNgK_OByPBAiBVgQBOXSPAIPP(long param_1,long param_2)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  long lVar11;
  long lVar12;
  
  lVar9 = param_1 * 2;
  lVar2 = *(long *)(param_2 + 0x10);
  lVar3 = *(long *)(param_2 + 0x20);
  lVar4 = *(long *)(param_2 + 0x28);
  lVar5 = *(long *)(param_2 + 0x18);
  if ((lVar9 <= lVar2) && (lVar9 != 0)) {
    while( true ) {
      lVar8 = lVar9;
      if (lVar9 < lVar2) {
        plVar10 = (long *)(lVar3 + lVar9 * 8);
        plVar1 = (long *)(lVar3 + 8 + lVar9 * 8);
        lVar11 = *plVar10;
        lVar6 = *plVar1;
        lVar12 = *(long *)(lVar4 + lVar11 * 8);
        lVar7 = *(long *)(lVar4 + lVar6 * 8);
        if (lVar7 < lVar12) {
          lVar8 = lVar9 + 1;
          plVar10 = plVar1;
          lVar11 = lVar6;
          lVar12 = lVar7;
        }
      }
      else {
        plVar10 = (long *)(lVar3 + lVar9 * 8);
        lVar11 = *plVar10;
        lVar12 = *(long *)(lVar4 + lVar11 * 8);
      }
      plVar1 = (long *)(lVar3 + param_1 * 8);
      lVar9 = *plVar1;
      if ((*(long *)(lVar4 + lVar9 * 8) < lVar12) &&
         (plVar10 = plVar1, lVar11 = lVar9, lVar8 != param_1)) break;
      *plVar1 = lVar11;
      *plVar10 = lVar9;
      *(long *)(lVar5 + *plVar1 * 8) = param_1;
      *(long *)(lVar5 + *plVar10 * 8) = lVar8;
      lVar9 = lVar8 * 2;
      if (lVar2 < lVar9) {
        return;
      }
      param_1 = lVar8;
      if (lVar9 == 0) {
        return;
      }
    }
  }
  return;
}


