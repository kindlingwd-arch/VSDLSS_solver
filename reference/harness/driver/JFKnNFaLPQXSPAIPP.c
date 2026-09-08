/* driver: JFKnNFaLPQXSPAIPP addr=009a7370 size=2c */
#include "vsdlss_ref.h"


long JFKnNFaLPQXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar1 = *(long *)(*(long *)(param_3 + 0x20) + param_1 * 8);
  lVar2 = *(long *)(*(long *)(param_3 + 0x20) + param_2 * 8);
  lVar3 = *(long *)(*(long *)(param_3 + 0x28) + lVar2 * 8);
  lVar4 = *(long *)(*(long *)(param_3 + 0x28) + lVar1 * 8);
  lVar5 = param_1;
  if ((lVar3 <= lVar4) && (lVar5 = param_2, lVar4 <= lVar3)) {
    if (lVar2 < lVar1) {
      param_1 = param_2;
    }
    return param_1;
  }
  return lVar5;
}


