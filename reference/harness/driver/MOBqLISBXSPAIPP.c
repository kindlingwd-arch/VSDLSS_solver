/* driver: MOBqLISBXSPAIPP addr=009a52f0 size=1cd */
#include "vsdlss_ref.h"


void MOBqLISBXSPAIPP(undefined8 param_1,long param_2,long param_3,long *param_4,long *param_5)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  undefined8 *puVar6;
  long lVar7;
  
  lVar2 = BKNqLISBgKkBJXSPAIPP();
  if (lVar2 == 0) {
    lVar2 = OByAXbce123pcmpbcptcaXSPAIPP();
    puVar6 = (undefined8 *)KBTtB_QLOXSPAIPP(0,param_3,1);
    *puVar6 = 0;
    OByAtB_dOLJdFIBXSPAIPP(param_1,param_3,puVar6);
    MBOJRQBbtB_QLOgKnIy_BXSPAIPP(lVar2,param_3,puVar6);
    if (lVar2 != 0) {
      _yPFdOBBXSPAIPP(lVar2);
    }
    lVar2 = OByAbBD1bBD2nCXSPAIPP(0xffffffffffffffff);
    lVar7 = *(long *)(lVar2 + 8) + *(long *)(lVar2 + 0x10);
    PLISBuFQEbBD1bBD2nCjLTBOXSPAIPP(lVar2,puVar6,puVar6);
    ABIBQBbBD1bBD2nCXSPAIPP(lVar2);
    puVar1 = puVar6 + lVar7;
    TOFQBXbce1bce2nYprpcqXSPAIPP(puVar6,lVar7);
    lVar4 = OByAbBD3nCXSPAIPP(0xffffffffffffffff);
    lVar2 = *(long *)(lVar4 + 8);
    PLISBuFQEbBD3nCjLTBOXSPAIPP(lVar4,puVar1,puVar1);
    ABIBQBbBD3nCXSPAIPP(lVar4);
    TOFQBXbce3nYprpcqXSPAIPP(puVar1,lVar2);
    TOFQBbtB_QLOdlRJXSPAIPP(5,param_3 - (lVar7 + lVar2),puVar1 + lVar2);
    if (puVar1 + lVar2 + -(lVar7 + lVar2) != (undefined8 *)0x0) {
      _yPFdOBBXSPAIPP();
    }
  }
  else {
    uVar3 = DBQnCpBLOABOtB_qySBXSPAIPP();
    lVar4 = DBQnCbBD1bBD2nyOQpBPXSPAIPP();
    if (param_2 == 0) {
      OByAtB_dOLJdFIBXSPAIPP(param_1,param_3,lVar4);
      MBOJRQBbtB_QLOgKnIy_BXSPAIPP(uVar3,param_3,lVar4);
    }
    else {
      MBOJRQBbtB_QLOXSPAIPP(uVar3,param_3,param_2,lVar4);
    }
    lVar2 = DBQnCqySBXSPAIPP();
    lVar7 = *(long *)(lVar2 + 8) + *(long *)(lVar2 + 0x10);
    PLISBuFQEbBD1bBD2nCjLTBOXSPAIPP(lVar2,lVar4,lVar4);
    lVar5 = DBQnC3qySBXSPAIPP();
    lVar4 = lVar4 + lVar7 * 8;
    lVar2 = *(long *)(lVar5 + 8);
    PLISBuFQEbBD3nCjLTBOXSPAIPP(lVar5,lVar4,lVar4);
  }
  *param_4 = lVar7;
  *param_5 = lVar2;
  return;
}


