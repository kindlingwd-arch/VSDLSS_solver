/* driver: zRFIAZMNYOOXSPAIPP addr=009b2560 size=48 */
#include "vsdlss_ref.h"


void zRFIAZMNYOOXSPAIPP(long *param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  lVar1 = *param_1;
  lVar2 = param_1[2];
  lVar3 = 1;
  if (0 < lVar1) {
    do {
      lVar4 = lVar3 + 1;
      KNZMNXSPAIPP(lVar3,*(long *)(lVar2 + lVar3 * 8) + 1,param_2);
      lVar3 = lVar4;
    } while (lVar4 <= lVar1);
  }
  return;
}


