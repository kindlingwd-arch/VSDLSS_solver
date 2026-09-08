/* driver: zRFIAnNXSPAIPP addr=009a6e90 size=64 */
#include "vsdlss_ref.h"


void zRFIAnNXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  *(long *)(param_1 + 0x10) = param_2;
  if (0 < param_2) {
    lVar1 = *(long *)(param_1 + 0x18);
    lVar2 = *(long *)(param_1 + 0x20);
    lVar3 = 1;
    do {
      *(long *)(lVar1 + *(long *)(lVar2 + lVar3 * 8) * 8) = lVar3;
      lVar3 = lVar3 + 1;
    } while (lVar3 != param_2 + 1);
    param_2 = *(long *)(param_1 + 0x10);
  }
  param_2 = param_2 / 2;
  if (0 < param_2) {
    do {
      MNgK_OByPBAiBVgQBOXSPAIPP(param_2,param_1);
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}


