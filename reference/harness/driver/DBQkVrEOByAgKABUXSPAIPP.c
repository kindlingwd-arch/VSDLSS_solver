/* driver: DBQkVrEOByAgKABUXSPAIPP addr=009730c0 size=43 */
#include "vsdlss_ref.h"


long DBQkVrEOByAgKABUXSPAIPP(long param_1)

{
  long lVar1;
  long lVar2;
  
  if (0 < DAT_00e74528) {
    lVar2 = 1;
    do {
      if ((*(long *)(&DAT_00e74320 + lVar2 * 8) != 0) &&
         (*(long *)(lVar2 * 8 + 0xe73440) == param_1)) {
        lVar1 = 0;
        if (lVar2 <= DAT_00e74528) {
          lVar1 = lVar2;
        }
        return lVar1;
      }
      lVar2 = lVar2 + 1;
    } while (lVar2 <= DAT_00e74528);
  }
  return 0;
}


