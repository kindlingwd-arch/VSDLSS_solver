/* driver: MNqTyMXSPAIPP addr=009a69e0 size=2b */
#include "vsdlss_ref.h"


void MNqTyMXSPAIPP(long param_1,long param_2,long param_3)

{
  long *plVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  
  lVar3 = *(long *)(param_3 + 0x18);
  plVar1 = (long *)(*(long *)(param_3 + 0x20) + param_1 * 8);
  plVar2 = (long *)(*(long *)(param_3 + 0x20) + param_2 * 8);
  lVar4 = *plVar1;
  *plVar1 = *plVar2;
  *plVar2 = lVar4;
  *(long *)(lVar3 + *plVar1 * 8) = param_1;
  *(long *)(lVar3 + *plVar2 * 8) = param_2;
  return;
}


