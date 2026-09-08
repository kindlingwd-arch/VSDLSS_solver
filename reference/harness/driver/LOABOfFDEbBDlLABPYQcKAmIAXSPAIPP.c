/* driver: LOABOfFDEbBDlLABPYQcKAmIAXSPAIPP addr=0099d060 size=135 */
#include "vsdlss_ref.h"


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long LOABOfFDEbBDlLABPYQcKAmIAXSPAIPP
               (long param_1,long param_2,long param_3,undefined8 param_4,undefined8 param_5)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  double dVar7;
  double dVar8;
  
  lVar3 = *(long *)(param_1 + 0x10);
  lVar1 = *(long *)(param_1 + 0x50);
  if (999 < lVar3) {
    dVar7 = pow((double)lVar3,_DAT_00b1d070);
    dVar7 = floor(dVar7);
    lVar5 = ((*(long *)(param_1 + 0x18) * 2) / lVar3) * 0x1e;
    dVar8 = pow((double)lVar3,DAT_00ab7228);
    dVar8 = floor(dVar8);
    lVar4 = 10;
    if (9 < lVar5) {
      lVar4 = lVar5;
    }
    lVar6 = 1;
    lVar5 = (long)dVar8;
    if ((long)dVar8 <= lVar4) {
      lVar5 = lVar4;
    }
    lVar4 = 0;
    do {
      lVar2 = *(long *)(lVar1 + lVar6 * 8);
      if (lVar5 < lVar2) {
        lVar4 = lVar4 + 1;
        *(long *)(param_3 + lVar4 * 8) = lVar2;
        *(long *)(param_2 + lVar4 * 8) = lVar6;
      }
      lVar6 = lVar6 + 1;
    } while (lVar6 != lVar3 + 1);
    if (lVar4 != 0) {
      _yPFpyAFUqLOQnBOJXSPAIPP(lVar4,param_4,param_3,param_2,param_5);
      lVar3 = 10;
      if (9 < (long)dVar7) {
        lVar3 = (long)dVar7;
      }
      if (lVar4 <= lVar3) {
        lVar3 = lVar4;
      }
      return lVar3;
    }
  }
  return 0;
}


