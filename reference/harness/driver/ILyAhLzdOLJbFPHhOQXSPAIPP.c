/* driver: ILyAhLzdOLJbFPHhOQXSPAIPP addr=0098b390 size=20b */
#include "vsdlss_ref.h"


void ILyAhLzdOLJbFPHhOQXSPAIPP(void)

{
  long *plVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  undefined8 *puVar6;
  long lVar7;
  long lVar8;
  
  lVar3 = CFKAhLzgKhOQpBQROK();
  if (lVar3 == -1) {
    FUN_0098ac50();
  }
  lVar8 = lVar3 * 0x4a0;
  if (*(long *)(&DAT_00e79528 + lVar8) == 0) {
    KOBOOLOXSPAIPP(0x45,"jobreloadmanager.c",0x151);
    if (*(long *)(&DAT_00e79530 + lVar8) == 0) {
      KOBOOLOXSPAIPP(1,"jobreloadmanager.c",0x154);
      return;
    }
    uVar4 = _OByQBqBJFnBOJryDXSPAIPP();
    *(undefined8 *)(&DAT_00e79538 + lVar8) = uVar4;
    uVar4 = QyDpByAXbce123pcmpbcptcaXSPAIPP(uVar4);
    *(undefined8 *)(&DAT_00e79540 + lVar8) = uVar4;
    uVar4 = OByAbBD1bBD2nCXSPAIPP(*(undefined8 *)(&DAT_00e79538 + lVar8));
    *(undefined8 *)(&DAT_00e79548 + lVar8) = uVar4;
    lVar5 = OByAbBD3nCXSPAIPP(*(undefined8 *)(&DAT_00e79538 + lVar8));
    plVar1 = *(long **)(&DAT_00e79548 + lVar8);
    *(long *)(&DAT_00e79550 + lVar8) = lVar5;
    lVar7 = plVar1[1] + plVar1[2] + *(long *)(lVar5 + 8);
    lVar5 = *plVar1;
    if (lVar5 == lVar7 || lVar5 - lVar7 < 0) {
      *(undefined8 *)(&DAT_00e79560 + lVar8) = 0;
      *(undefined8 *)(&DAT_00e79558 + lVar8) = 0xffffffffffffffff;
      *(undefined8 *)(&DAT_00e79568 + lVar8) = 0;
      *(undefined8 *)(&DAT_00e79570 + lVar8) = 0;
    }
    else {
      puVar6 = (undefined8 *)OByAdy_QLOmKBaERKHXSPAIPP();
      uVar4 = puVar6[5];
      uVar2 = *puVar6;
      *(undefined8 **)(&DAT_00e79560 + lVar8) = puVar6;
      lVar8 = puVar6[8];
      uVar4 = QyDlBTgtB_QLOXSPAIPP(uVar2,uVar4,0,0);
      puVar6[8] = uVar4;
      _LMVgtB_QLOXSPAIPP(lVar8,puVar6[5],uVar4);
      if (lVar8 != 0) {
        _yPFdOBBXSPAIPP(lVar8);
      }
      lVar8 = lVar3 * 0x4a0;
      *(undefined8 *)(&DAT_00e79558 + lVar8) = **(undefined8 **)(&DAT_00e79560 + lVar8);
      PBQqBJFnBOJdXSPAIPP();
      uVar4 = QyDpByApBLOABOtB_XSPAIPP(*(undefined8 *)(&DAT_00e79558 + lVar8));
      *(undefined8 *)(&DAT_00e79568 + lVar8) = uVar4;
      uVar4 = _EB_HgABKQFQVnBOJ(*(undefined8 *)(*(long *)(&DAT_00e79560 + lVar8) + 0x10),uVar4);
      *(undefined8 *)(&DAT_00e79570 + lVar8) = uVar4;
    }
    *(undefined8 *)(&DAT_00e79528 + lVar3 * 0x4a0) = 1;
    lVar8 = BKNqLISBgKkBJXSPAIPP();
    if (lVar8 != 0) {
      uVar4 = QyDlBTbtB_QLOXSPAIPP(0,*(undefined8 *)(&DAT_00e79538 + lVar3 * 0x4a0),lVar5,0);
      PBQnCYKAd_gKCLnQOPXSPAIPP();
      PBQnCpBPYKApEPtB_P(uVar4);
      return;
    }
  }
  return;
}


