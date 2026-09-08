/* driver: FKCglLOJXSPAIPP addr=00960750 size=58 */
#include "vsdlss_ref.h"


long FKCglLOJXSPAIPP(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  
  if (param_2 < 1) {
    lVar1 = 0;
  }
  else {
    uVar3 = *(long *)(param_1 + 8) >> 0x3f;
    lVar1 = (uVar3 ^ *(ulong *)(param_1 + 8)) - uVar3;
    if (param_2 != 1) {
      lVar4 = 2;
      do {
        uVar3 = *(long *)(param_1 + lVar4 * 8) >> 0x3f;
        lVar2 = (uVar3 ^ *(ulong *)(param_1 + lVar4 * 8)) - uVar3;
        if (lVar1 < lVar2) {
          lVar1 = lVar2;
        }
        lVar4 = lVar4 + 1;
      } while (lVar4 != param_2 + 1);
      return lVar1;
    }
  }
  return lVar1;
}


