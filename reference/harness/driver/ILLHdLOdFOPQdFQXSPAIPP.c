/* driver: ILLHdLOdFOPQdFQXSPAIPP addr=0098fc60 size=68 */
#include "vsdlss_ref.h"


long ILLHdLOdFOPQdFQXSPAIPP(long param_1,ulong param_2,ulong param_3)

{
  long lVar1;
  ulong uVar2;
  
  lVar1 = -1;
  if (-1 < (long)(&DAT_00e8c600)[param_1]) {
    uVar2 = *(ulong *)(&DAT_00e8be00)[param_1];
    lVar1 = 0;
    for (uVar2 = uVar2 - uVar2 % param_3; uVar2 < param_2; uVar2 = uVar2 - uVar2 % param_3) {
      lVar1 = lVar1 + 1;
      if ((long)(&DAT_00e8c600)[param_1] < lVar1) {
        return -1;
      }
      uVar2 = ((ulong *)(&DAT_00e8be00)[param_1])[lVar1 * 2];
    }
  }
  return lVar1;
}


