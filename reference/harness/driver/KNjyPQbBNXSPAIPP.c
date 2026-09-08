/* driver: KNjyPQbBNXSPAIPP addr=009b2730 size=2d */
#include "vsdlss_ref.h"


void KNjyPQbBNXSPAIPP(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  
  lVar1 = *(long *)(param_2 + 0x10);
  uVar2 = KBTacADBIFPQXSPAIPP(param_1,*(undefined8 *)(lVar1 + 8));
  *(undefined8 *)(lVar1 + 8) = uVar2;
  *(undefined8 *)(param_2 + 0x10) = *(undefined8 *)(*(long *)(param_2 + 0x10) + 8);
  return;
}


