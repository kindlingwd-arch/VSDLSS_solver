/* driver: DBQpLTrLZIL_HtB_XSPAIPP addr=0096ea00 size=60 */
#include "vsdlss_ref.h"


void DBQpLTrLZIL_HtB_XSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar2 = 0;
  lVar3 = 1;
  if (0 < param_1) {
    lVar1 = *(long *)(param_2 + 8);
    while( true ) {
      lVar4 = lVar2;
      if (0 < lVar1) {
        lVar4 = lVar1 + lVar2;
        do {
          lVar2 = lVar2 + 1;
          *(long *)(param_3 + lVar2 * 8) = lVar3;
        } while (lVar2 != lVar4);
      }
      lVar2 = lVar4;
      lVar3 = lVar3 + 1;
      if (lVar3 == param_1 + 1) break;
      lVar1 = *(long *)(param_2 + lVar3 * 8);
    }
  }
  return;
}


