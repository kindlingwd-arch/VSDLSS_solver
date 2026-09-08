/* driver: Cy_QLOYmKBaERKHlBTXSPAIPP addr=0096e530 size=288 */
#include "vsdlss_ref.h"


void Cy_QLOYmKBaERKHlBTXSPAIPP(undefined8 param_1,long param_2,undefined8 param_3,long param_4)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  
  lVar11 = *(long *)(param_4 + 0x28);
  lVar2 = *(long *)(param_4 + 0x40);
  uVar3 = DBQkyUgtB_QLOXSPAIPP(*(undefined8 *)(param_4 + 0x48),*(undefined8 *)(param_4 + 0x10));
  lVar4 = DBQkyUqRMlLABlxXSPAIPP(param_4);
  lVar5 = KBTgtB_QLOXSPAIPP(uVar3,0,0);
  uVar6 = KBTtB_QLOXSPAIPP(0,0x4011,0);
  uVar7 = KBTdtB_QLOXSPAIPP(0,lVar4 + 0x10,0);
  uVar8 = KBTdtB_QLOXSPAIPP(0,lVar4 + 0x10,0);
  uVar3 = _yPFnyOqLISBOd;
  lVar4 = (uVar6 & 0xfffffffffffffff0) + 0x10;
  lVar1 = (uVar7 & 0xfffffffffffffff0) + 0x10;
  lVar9 = (uVar8 & 0xfffffffffffffff0) + 0x10;
  if (param_2 != 0) {
    lVar10 = DBQlZIL_HPnyOQdy_QXSPAIPP(lVar11,lVar2,param_3);
    lVar11 = lVar11 - lVar10;
  }
  lVar13 = 1;
  lVar10 = 1;
  if (0 < lVar11) {
    do {
      while (lVar12 = *(long *)(lVar2 + lVar13 * 8), lVar12 != 2) {
        if (lVar12 == 3) {
          lVar12 = lVar10 + 3;
          PRz3pLTaLKQOFzdOLJpBPQXSPAIPP(lVar10,param_4);
        }
        else if (lVar12 == 1) {
          lVar12 = lVar10 + 1;
          PRz1pLTaLKQOFzdOLJpBPQXSPAIPP(lVar10,param_4);
        }
        else {
          lVar12 = lVar12 + lVar10;
          Cy_QLOuFQEFKZIL_HXO(lVar10,lVar12 + -1,param_1,param_4,uVar3,lVar5,lVar4,lVar1,lVar9);
          PRzZIL_HaLKQOFzdOLJpLTpyKDBXSPAIPP
                    (lVar10,lVar12 + -1,lVar12,*(undefined8 *)(param_4 + 0x10),param_1,param_4,uVar3
                     ,lVar5,lVar4,lVar1,lVar9);
        }
        lVar13 = lVar13 + 1;
        lVar10 = lVar12;
        if (lVar11 < lVar13) goto LAB_0096e71e;
      }
      lVar13 = lVar13 + 1;
      PRz2pLTaLKQOFzdOLJpBPQXSPAIPP(lVar10,param_4);
      lVar10 = lVar10 + 2;
    } while (lVar13 <= lVar11);
  }
LAB_0096e71e:
  if (lVar5 != 0) {
    _yPFdOBBXSPAIPP(lVar5);
  }
  if (uVar6 != 0) {
    _yPFdOBBXSPAIPP();
  }
  if (uVar7 != 0) {
    _yPFdOBBXSPAIPP();
  }
  if (uVar8 == 0) {
    return;
  }
  _yPFdOBBXSPAIPP();
  return;
}


