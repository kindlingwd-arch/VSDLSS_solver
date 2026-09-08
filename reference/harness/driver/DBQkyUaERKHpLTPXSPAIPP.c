/* driver: DBQkyUaERKHpLTPXSPAIPP addr=00974d30 size=41 */
#include "vsdlss_ref.h"


long DBQkyUaERKHpLTPXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  if (0 < *(long *)(param_1 + 0x10)) {
    lVar3 = 1;
    lVar1 = 0;
    do {
      lVar2 = (*(long *)(*(long *)(param_1 + 0x70) + lVar3 * 8) -
              *(long *)(*(long *)(param_1 + 0x60) + lVar3 * 8)) + 1;
      if (lVar1 < lVar2) {
        lVar1 = lVar2;
      }
      lVar3 = lVar3 + 1;
    } while (lVar3 != *(long *)(param_1 + 0x10) + 1);
    return lVar1;
  }
  return 0;
}


