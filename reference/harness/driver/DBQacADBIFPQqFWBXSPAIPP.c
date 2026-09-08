/* driver: DBQacADBIFPQqFWBXSPAIPP addr=009742f0 size=21 */
#include "vsdlss_ref.h"


long DBQacADBIFPQqFWBXSPAIPP(long param_1)

{
  long lVar1;
  
  lVar1 = 0;
  if (param_1 == 0) {
    return 0;
  }
  do {
    param_1 = *(long *)(param_1 + 8);
    lVar1 = lVar1 + 1;
  } while (param_1 != 0);
  return lVar1;
}


