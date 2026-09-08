/* driver: MNFgK_OByPBAiBVgQBOXSPAIPP addr=009a73b0 size=f0 */
#include "vsdlss_ref.h"


void MNFgK_OByPBAiBVgQBOXSPAIPP(long param_1,long param_2)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long *plVar12;
  long lVar13;
  
  lVar11 = *(long *)(param_2 + 0x10);
  lVar10 = param_1 * 2;
  if ((lVar10 <= lVar11) && (lVar10 != 0)) {
    lVar3 = *(long *)(param_2 + 0x28);
    lVar4 = *(long *)(param_2 + 0x20);
    while( true ) {
      lVar9 = lVar10;
      if (lVar10 < lVar11) {
        plVar1 = (long *)(lVar4 + lVar10 * 8);
        plVar2 = (long *)(lVar4 + 8 + lVar10 * 8);
        lVar5 = *plVar1;
        lVar6 = *plVar2;
        lVar7 = *(long *)(lVar3 + lVar5 * 8);
        lVar8 = *(long *)(lVar3 + lVar6 * 8);
        plVar12 = plVar1;
        lVar11 = lVar5;
        lVar13 = lVar7;
        if (((lVar8 <= lVar7) &&
            (lVar9 = lVar10 + 1, plVar12 = plVar2, lVar11 = lVar6, lVar13 = lVar8, lVar7 <= lVar8))
           && (lVar9 = lVar10, plVar12 = plVar1, lVar11 = lVar5, lVar13 = lVar7, lVar6 < lVar5)) {
          lVar9 = lVar10 + 1;
          plVar12 = plVar2;
          lVar11 = lVar6;
          lVar13 = lVar8;
        }
      }
      else {
        plVar12 = (long *)(lVar4 + lVar10 * 8);
        lVar11 = *plVar12;
        lVar13 = *(long *)(lVar3 + lVar11 * 8);
      }
      plVar1 = (long *)(lVar4 + param_1 * 8);
      lVar10 = *plVar1;
      lVar5 = *(long *)(lVar3 + lVar10 * 8);
      if (((lVar5 <= lVar13) && ((lVar5 < lVar13 || (lVar10 < lVar11)))) &&
         (plVar12 = plVar1, lVar11 = lVar10, lVar9 != param_1)) break;
      *plVar1 = lVar11;
      *plVar12 = lVar10;
      lVar11 = *(long *)(param_2 + 0x18);
      *(long *)(lVar11 + *plVar1 * 8) = param_1;
      *(long *)(lVar11 + *plVar12 * 8) = lVar9;
      lVar11 = *(long *)(param_2 + 0x10);
      lVar10 = lVar9 * 2;
      if (lVar11 < lVar10) {
        return;
      }
      param_1 = lVar9;
      if (lVar10 == 0) {
        return;
      }
    }
  }
  return;
}


