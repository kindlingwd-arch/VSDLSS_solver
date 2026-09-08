/* driver: MNFbB_OByPBAiBVgQBOXSPAIPP addr=009a74b0 size=5f */
#include "vsdlss_ref.h"


void MNFbB_OByPBAiBVgQBOXSPAIPP(long param_1,long param_2)

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
  
  if (1 < param_1) {
    lVar3 = *(long *)(param_2 + 0x20);
    lVar4 = *(long *)(param_2 + 0x28);
    do {
      plVar1 = (long *)(lVar3 + param_1 * 8);
      lVar9 = param_1 >> 1;
      plVar2 = (long *)(lVar3 + lVar9 * 8);
      lVar5 = *plVar1;
      lVar6 = *plVar2;
      lVar7 = *(long *)(lVar4 + lVar6 * 8);
      lVar8 = *(long *)(lVar4 + lVar5 * 8);
      if ((lVar7 <= lVar8) && (((lVar7 < lVar8 || (lVar6 < lVar5)) && (lVar9 != param_1)))) {
        return;
      }
      *plVar1 = lVar6;
      *plVar2 = lVar5;
      lVar5 = *(long *)(param_2 + 0x18);
      *(long *)(lVar5 + *plVar1 * 8) = param_1;
      *(long *)(lVar5 + *plVar2 * 8) = lVar9;
      param_1 = lVar9;
    } while (1 < lVar9);
  }
  return;
}


