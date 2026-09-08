/* driver: OBLOABOcIBJYOOXSPAIPP addr=00979300 size=51 */
#include "vsdlss_ref.h"


void OBLOABOcIBJYOOXSPAIPP(long param_1,long *param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  
  lVar1 = *param_2;
  lVar2 = param_2[5];
  lVar5 = 1;
  lVar3 = param_2[7];
  if (0 < lVar1) {
    do {
      lVar7 = *(long *)(param_1 + *(long *)(lVar2 + lVar5 * 8) * 8);
      lVar4 = *(long *)(param_1 + *(long *)(lVar3 + lVar5 * 8) * 8);
      lVar6 = lVar4;
      if (lVar7 <= lVar4) {
        lVar6 = lVar7;
        lVar7 = lVar4;
      }
      *(long *)(lVar2 + lVar5 * 8) = lVar6;
      *(long *)(lVar3 + lVar5 * 8) = lVar7;
      lVar5 = lVar5 + 1;
    } while (lVar5 != lVar1 + 1);
  }
  return;
}


