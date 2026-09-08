/* driver: MLPQqLISBXSPAIPP addr=009a57d0 size=1f5 */
#include "vsdlss_ref.h"


void MLPQqLISBXSPAIPP(undefined8 param_1,long param_2,long param_3,long *param_4,long *param_5,
                     long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  
  lVar5 = *param_4;
  lVar2 = *param_5;
  lVar1 = BKNqLISBgKkBJXSPAIPP();
  param_6 = param_6 + (lVar5 + lVar2) * 8;
  lVar5 = param_3 - (lVar5 + lVar2);
  if (lVar5 < 1) {
    if (lVar1 == 0) goto LAB_009a58f9;
  }
  else if (lVar1 == 0) {
    OByAtB_dOLJdFIBXSPAIPP(param_1,lVar5,param_6);
LAB_009a58f9:
    lVar2 = OByAbBD3nCXSPAIPP(0xffffffffffffffff);
    lVar5 = *(long *)(lVar2 + 8);
    param_6 = param_6 + lVar5 * -8;
    lVar3 = OByAXbce3nYprpcqlLlBTtB_XSPAIPP(param_6);
    if (lVar3 != lVar5) {
      KOBOOLOXSPAIPP(1,"prepostsolve.c",0x285);
    }
    PLISBuFQEbBD3nCsMMBOXSPAIPP(lVar2,param_6,param_6);
    ABIBQBbBD3nCXSPAIPP(lVar2);
    lVar2 = OByAbBD1bBD2nCXSPAIPP(0xffffffffffffffff);
    lVar4 = *(long *)(lVar2 + 8) + *(long *)(lVar2 + 0x10);
    param_6 = param_6 + lVar4 * -8;
    lVar3 = OByAXbce1bce2nYprpcqlLlBTtB_XSPAIPP(param_6);
    if (lVar3 != lVar4) {
      KOBOOLOXSPAIPP(1,"prepostsolve.c",0x2a3);
    }
    PLISBuFQEbBD1bBD2nCsMMBOXSPAIPP(lVar2,param_6,param_6);
    ABIBQBbBD1bBD2nCXSPAIPP(lVar2);
    lVar2 = OByAXbce123pcmpbcptcaXSPAIPP();
    goto LAB_009a5870;
  }
  lVar2 = DBQnC3qySBXSPAIPP();
  lVar5 = *(long *)(lVar2 + 8);
  param_6 = param_6 + lVar5 * -8;
  PLISBuFQEbBD3nCsMMBOXSPAIPP(lVar2,param_6,param_6);
  lVar2 = DBQnCqySBXSPAIPP();
  lVar4 = *(long *)(lVar2 + 8) + *(long *)(lVar2 + 0x10);
  param_6 = param_6 + lVar4 * -8;
  PLISBuFQEbBD1bBD2nCsMMBOXSPAIPP(lVar2,param_6,param_6);
  lVar2 = DBQnCpBLOABOtB_qySBXSPAIPP();
LAB_009a5870:
  if ((lVar1 == 0) || (param_2 == 0)) {
    FKSnBOJRQBbtB_QLOgKnIy_BXSPAIPP(lVar2,param_3,param_6);
    TOFQBtB_rLdFIBXSPAIPP(param_1,param_3,param_6);
    if ((lVar1 == 0) && (lVar2 != 0)) {
      _yPFdOBBXSPAIPP(lVar2);
    }
  }
  else {
    FKSnBOJRQBbtB_QLOXSPAIPP(lVar2,param_3,param_6,param_2);
  }
  *param_4 = lVar4;
  *param_5 = lVar5;
  return;
}


