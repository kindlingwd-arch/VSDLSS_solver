/* driver: DBQkFKZR_HBQZMNXSPAIPP addr=009b2600 size=35 */
#include "vsdlss_ref.h"


long DBQkFKZR_HBQZMNXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  
  lVar1 = *(long *)(param_1 + 0x20);
  if (0 < *(long *)(param_1 + 8)) {
    lVar2 = 1;
    while( true ) {
      if (*(long *)(*(long *)(param_1 + 0x30) + 8) != lVar1) {
        return lVar2;
      }
      lVar2 = lVar2 + 1;
      if (*(long *)(param_1 + 8) < lVar2) break;
      lVar1 = *(long *)(*(long *)(param_1 + 0x30) + lVar2 * 8);
    }
  }
  return 0;
}


