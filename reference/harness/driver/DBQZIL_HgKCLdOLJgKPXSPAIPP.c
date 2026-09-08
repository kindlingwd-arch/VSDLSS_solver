/* driver: DBQZIL_HgKCLdOLJgKPXSPAIPP addr=00989760 size=159 */
#include "vsdlss_ref.h"


long DBQZIL_HgKCLdOLJgKPXSPAIPP(undefined8 param_1,long param_2,undefined8 *param_3)

{
  long *plVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  long lVar11;
  long lVar12;
  
  lVar9 = *(long *)*param_3;
  lVar7 = OBKyJBgKPYKAeBQlLABrLZIL_HXSPAIPP(param_2,param_3);
  uVar2 = *(ulong *)(lVar7 + lVar9 * 8);
  lVar8 = KBTZIL_HgKCLXSPAIPP(param_1,uVar2);
  lVar3 = *(long *)(lVar8 + 0x18);
  lVar4 = *(long *)(lVar8 + 0x10);
  lVar5 = *(long *)(lVar8 + 0x38);
  FKFQgtB_QLOXSPAIPP(lVar3,uVar2,0);
  plVar10 = (long *)(param_2 + 8);
  if (0 < lVar9) {
    do {
      lVar11 = *plVar10;
      plVar10 = plVar10 + 1;
      plVar1 = (long *)(lVar3 + *(long *)(lVar7 + lVar11 * 8) * 8);
      *plVar1 = *plVar1 + 1;
    } while (plVar10 != (long *)(param_2 + 8 + lVar9 * 8));
  }
  lVar11 = 0;
  lVar9 = 1;
  if (0 < (long)uVar2) {
    lVar6 = *(long *)(lVar3 + 8);
    while( true ) {
      lVar12 = lVar11;
      if (0 < lVar6) {
        lVar12 = lVar6 + lVar11;
        do {
          lVar11 = lVar11 + 1;
          *(long *)(lVar5 + lVar11 * 8) = lVar9;
        } while (lVar11 != lVar12);
      }
      lVar11 = lVar12;
      lVar9 = lVar9 + 1;
      if (lVar9 == uVar2 + 1) break;
      lVar6 = *(long *)(lVar3 + lVar9 * 8);
    }
  }
  if (lVar7 != 0) {
    _yPFdOBBXSPAIPP(lVar7);
  }
  if ((0 < (long)uVar2) && (*(undefined8 *)(lVar4 + 8) = 1, 1 < uVar2)) {
    lVar9 = 2;
    do {
      *(long *)(lVar4 + lVar9 * 8) =
           *(long *)(lVar4 + -8 + lVar9 * 8) + *(long *)(lVar3 + -8 + lVar9 * 8);
      lVar9 = lVar9 + 1;
    } while (lVar9 != uVar2 + 1);
  }
  return lVar8;
}


