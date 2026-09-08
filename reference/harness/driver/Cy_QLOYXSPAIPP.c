/* driver: Cy_QLOYXSPAIPP addr=00981920 size=275 */
#include "vsdlss_ref.h"


void Cy_QLOYXSPAIPP(long param_1,long param_2,long param_3)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  
  uVar4 = *(undefined8 *)(param_2 + 8);
  lVar1 = *(long *)(param_1 + 0x10);
  uVar2 = DBQkyUgtB_QLOXSPAIPP(*(undefined8 *)(param_2 + 0x20),uVar4);
  lVar3 = KBTgtB_QLOXSPAIPP(uVar2,0,0);
  uVar2 = _OByQBdy_QLOZIL_H(param_2);
  uVar4 = DBQkyUgtB_QLOXSPAIPP(*(undefined8 *)(param_2 + 0x18),uVar4);
  uVar4 = KBTdy_QLOZIL_HfByABOXSPAIPP(uVar4);
  if (0 < lVar1) {
    lVar9 = 1;
    _yPFrFJBPXSPAIPP(QXTLOH1XSPAIPP);
    uVar5 = _OByQBdy_QLOaERKH(param_1,param_2,1);
    FKFQdy_QLOaERKHdOLJY(param_1,uVar5,uVar2,lVar3);
    _yPFrFJBPXSPAIPP(QXTLOH2XSPAIPP);
    y__RJbFCCrFJBPXSPAIPP(QXTLOH1XSPAIPP,QXTLOH2XSPAIPP,QXCy_QLOgmXSPAIPP);
    OBJLSBYaERKHdFIBXSPAIPP(param_1,1);
LAB_00981a08:
    lVar9 = lVar9 + 1;
    BCCB_QuFQEFKaERKH(param_1,param_2,uVar5,uVar4,lVar3);
    _yPFrFJBPXSPAIPP(QXTLOH1XSPAIPP);
    TOFQBdy_QLOaERKHXSPAIPP(uVar5,param_1,param_2);
    _yPFrFJBPXSPAIPP(QXTLOH2XSPAIPP);
    y__RJbFCCrFJBPXSPAIPP(QXTLOH1XSPAIPP,QXTLOH2XSPAIPP,QXCy_QLOgmXSPAIPP);
    ABIBQBdy_QLOaERKHXSPAIPP(uVar5);
    if (lVar9 <= lVar1) {
      _yPFrFJBPXSPAIPP(QXTLOH1XSPAIPP);
      uVar5 = _OByQBdy_QLOaERKH(param_1,param_2,lVar9);
      FKFQdy_QLOaERKHdOLJY(param_1,uVar5,uVar2,lVar3);
      _yPFrFJBPXSPAIPP(QXTLOH2XSPAIPP);
      y__RJbFCCrFJBPXSPAIPP(QXTLOH1XSPAIPP,QXTLOH2XSPAIPP,QXCy_QLOgmXSPAIPP);
      OBJLSBYaERKHdFIBXSPAIPP(param_1,lVar9);
      lVar7 = 1;
      do {
        while( true ) {
          lVar8 = lVar9;
          if (lVar9 <= lVar7) {
            lVar8 = lVar7;
          }
          lVar6 = lVar9;
          if (lVar7 <= lVar9) {
            lVar6 = lVar7;
          }
          if (*(char *)(*(long *)(*(long *)(param_3 + 0x10) + lVar8 * 8) + lVar6) == '\0') break;
          lVar8 = lVar7 + 1;
          BCCB_QmCdOLJaERKHmKrLaERKH(param_1,param_2,lVar7,lVar9,uVar5,uVar2,lVar3);
          lVar7 = lVar8;
          if (lVar8 == lVar9) goto LAB_00981a08;
        }
        lVar7 = lVar7 + 1;
      } while (lVar7 != lVar9);
      goto LAB_00981a08;
    }
  }
  ABIBQBdy_QLOZIL_HXSPAIPP(uVar2);
  ABIBQBdy_QLOZIL_HXSPAIPP(uVar4);
  if (lVar3 == 0) {
    return;
  }
  _yPFdOBBXSPAIPP(lVar3);
  return;
}


