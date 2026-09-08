/* driver: DBQqLOQBAmOABOXSPAIPP addr=009791d0 size=123 */
#include "vsdlss_ref.h"


undefined8 DBQqLOQBAmOABOXSPAIPP(long *param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  
  lVar1 = *param_1;
  lVar6 = param_1[5];
  lVar2 = param_1[1];
  lVar7 = param_1[7];
  uVar5 = KBTgtB_QLOXSPAIPP(lVar1,0,0);
  lVar8 = 1;
  if (0 < lVar1) {
    do {
      lVar3 = *(long *)(lVar6 + lVar8 * 8);
      lVar4 = *(long *)(lVar7 + lVar8 * 8);
      if (lVar4 < lVar3) {
        *(long *)(lVar6 + lVar8 * 8) = lVar4;
        *(long *)(lVar7 + lVar8 * 8) = lVar3;
      }
      lVar8 = lVar8 + 1;
    } while (lVar8 != lVar1 + 1);
  }
  FKFQjyzBItB_XSPAIPP(uVar5,lVar1);
  lVar6 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
  lVar7 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
  PLOQmKbFDFQXSPAIPP(param_1[7],lVar1,lVar2,uVar5,lVar6,lVar7,param_2);
  PLOQmKbFDFQXSPAIPP(param_1[5],lVar1,lVar2,uVar5,lVar6,lVar7,param_2);
  if (lVar7 != 0) {
    _yPFdOBBXSPAIPP(lVar7);
  }
  if (lVar6 != 0) {
    _yPFdOBBXSPAIPP(lVar6);
  }
  DBQgKSBOPBkyMXSPAIPP(uVar5,lVar1,param_2);
  _LMVgtB_QLOXSPAIPP(param_2,lVar1,uVar5);
  return uVar5;
}


