/* driver: zRFIAZMNXSPAIPP addr=009b25b0 size=4d */
#include "vsdlss_ref.h"


void zRFIAZMNXSPAIPP(long *param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar1 = *param_1;
  lVar2 = param_1[4];
  lVar4 = 1;
  if (0 < lVar1) {
    do {
      lVar3 = DBQacADBIFPQqFWBXSPAIPP(*(undefined8 *)(lVar2 + lVar4 * 8));
      lVar5 = lVar4 + 1;
      KNZMNXSPAIPP(lVar4,lVar3 + 1,param_2);
      lVar4 = lVar5;
    } while (lVar5 <= lVar1);
  }
  return;
}


