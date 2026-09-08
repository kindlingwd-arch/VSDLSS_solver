/* driver: PRJgtB_QLOcIBJPXSPAIPP addr=00961ba0 size=22 */
#include "vsdlss_ref.h"


long PRJgtB_QLOcIBJPXSPAIPP(long param_1,ulong param_2)

{
  long lVar1;
  ulong uVar2;
  
  if (param_2 != 0) {
    uVar2 = 1;
    lVar1 = 0;
    do {
      lVar1 = lVar1 + *(long *)(param_1 + uVar2 * 8);
      uVar2 = uVar2 + 1;
    } while (uVar2 <= param_2);
    return lVar1;
  }
  return 0;
}


