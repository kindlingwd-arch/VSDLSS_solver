/* driver: DBQkyUgtB_QLOXSPAIPP addr=00961210 size=39 */
#include "vsdlss_ref.h"


long DBQkyUgtB_QLOXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long *plVar2;
  
  if (param_2 < 1) {
    lVar1 = 0;
  }
  else {
    lVar1 = *(long *)(param_1 + 8);
    if (param_2 != 1) {
      plVar2 = (long *)(param_1 + 0x10);
      do {
        if (lVar1 < *plVar2) {
          lVar1 = *plVar2;
        }
        plVar2 = plVar2 + 1;
      } while (plVar2 != (long *)(param_1 + 8 + param_2 * 8));
      return lVar1;
    }
  }
  return lVar1;
}


