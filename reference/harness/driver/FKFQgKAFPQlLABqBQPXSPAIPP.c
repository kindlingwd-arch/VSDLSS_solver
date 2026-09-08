/* driver: FKFQgKAFPQlLABqBQPXSPAIPP addr=00989900 size=4a */
#include "vsdlss_ref.h"


void FKFQgKAFPQlLABqBQPXSPAIPP(long *param_1)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  
  lVar4 = 1;
  lVar1 = *param_1;
  if (0 < lVar1) {
    do {
      lVar2 = param_1[1];
      lVar5 = lVar4 + 1;
      uVar3 = KBTacADBIFPQXSPAIPP(lVar4,0);
      *(undefined8 *)(lVar2 + lVar4 * 8) = uVar3;
      lVar4 = lVar5;
    } while (lVar5 <= lVar1);
  }
  return;
}


