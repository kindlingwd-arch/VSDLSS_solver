/* driver: DBQpLLQPgKcIFJrOBBXSPAIPP addr=00992490 size=53 */
#include "vsdlss_ref.h"


undefined8 DBQpLLQPgKcIFJrOBBXSPAIPP(long param_1,long param_2)

{
  undefined8 uVar1;
  long lVar2;
  long lVar3;
  
  if (param_2 < 1) {
    uVar1 = 0;
  }
  else {
    uVar1 = 0;
    lVar2 = 1;
    do {
      while ((lVar3 = *(long *)(param_1 + lVar2 * 8), lVar3 != -0x7ffffff5 && (lVar3 < 0))) {
        lVar3 = lVar2 + 1;
        uVar1 = KBTacADBIFPQXSPAIPP(lVar2,uVar1);
        lVar2 = lVar3;
        if (param_2 < lVar3) {
          return uVar1;
        }
      }
      lVar2 = lVar2 + 1;
    } while (lVar2 <= param_2);
  }
  return uVar1;
}


