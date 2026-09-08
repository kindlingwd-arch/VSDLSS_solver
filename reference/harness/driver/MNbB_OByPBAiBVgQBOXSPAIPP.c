/* driver: MNbB_OByPBAiBVgQBOXSPAIPP addr=009a6b40 size=58 */
#include "vsdlss_ref.h"


void MNbB_OByPBAiBVgQBOXSPAIPP(long param_1,long param_2)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  
  lVar3 = *(long *)(param_2 + 0x28);
  lVar4 = *(long *)(param_2 + 0x20);
  lVar5 = *(long *)(param_2 + 0x18);
  while( true ) {
    if (param_1 < 2) {
      return;
    }
    plVar1 = (long *)(lVar4 + param_1 * 8);
    lVar7 = param_1 >> 1;
    plVar2 = (long *)(lVar4 + lVar7 * 8);
    lVar6 = *plVar1;
    if ((*(long *)(lVar3 + *plVar2 * 8) < *(long *)(lVar3 + lVar6 * 8)) && (lVar7 != param_1))
    break;
    *plVar1 = *plVar2;
    *plVar2 = lVar6;
    *(long *)(lVar5 + *plVar1 * 8) = param_1;
    *(long *)(lVar5 + *plVar2 * 8) = lVar7;
    param_1 = lVar7;
  }
  return;
}


