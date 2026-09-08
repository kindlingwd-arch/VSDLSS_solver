/* driver: FKFQjyzBItB_XSPAIPP addr=00961450 size=1f */
#include "vsdlss_ref.h"


void FKFQjyzBItB_XSPAIPP(long param_1,long param_2)

{
  long lVar1;
  
  lVar1 = 1;
  if (0 < param_2) {
    do {
      *(long *)(param_1 + lVar1 * 8) = lVar1;
      lVar1 = lVar1 + 1;
    } while (lVar1 != param_2 + 1);
  }
  return;
}


