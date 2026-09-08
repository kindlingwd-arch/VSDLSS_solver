/* driver: OBAR_BnOLzIBJqFWBXSPAIPP addr=009aabf0 size=625 */
#include "vsdlss_ref.h"


long OBAR_BnOLzIBJqFWBXSPAIPP
               (long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,long *param_5,
               long *param_6,long *param_7)

{
  long lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  long lVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long local_b8;
  long local_b0;
  long local_a8;
  long local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined1 local_88 [40];
  undefined8 local_60;
  undefined8 local_50;
  
  lVar1 = BKNqLISBgKkBJXSPAIPP();
  if (lVar1 == 0) {
    DAT_00e3b560 = 0xffffffffffffffff;
  }
  else {
    DAT_00e3b560 = _OByQBqBJFnBOJryDXSPAIPP();
  }
  FKFQcIBJYOOfByABO(local_88,param_2,param_1);
  local_60 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  local_50 = KBTgtB_QLOXSPAIPP(param_2,0,0);
  OByAgtB_QLOdlRJXSPAIPP(0x1b,param_2,local_60);
  OByAgtB_QLOdlRJXSPAIPP(0x1c,param_2,local_50);
  _EB_HdOLJYKArLlLABPXSPAIPP(param_1,param_2,local_60,local_50);
  local_90 = zR_HBQlBFDEzLOPgKQLlLABZR_HBQPXSPAIPP(local_88);
  ABIBQBcIBJYOOlLfByABO(local_88);
  if (lVar1 == 0) {
    puVar2 = (undefined8 *)KBTgtB_QLOXSPAIPP(param_1,0,0);
  }
  else {
    puVar2 = (undefined8 *)QyDlBTgtB_QLOXSPAIPP(DAT_00e3b560,param_1,0,0);
  }
  *puVar2 = 0;
  FQBOpBLOABObBD123lLABP
            (param_3,param_4,&local_90,puVar2,&DAT_00e8e720,&local_b0,&local_a8,&local_a0);
  local_90 = ABIBQBYOOZR_HBQYOOXSPAIPP(local_90);
  if (mljwXkYrpgvXtYjscqXafYlecb != 0) {
    lVar6 = DBQbBD123pBLOABOtB_nOBS();
    if (lVar1 != 0) {
      lVar7 = DBQpBLOABOtB_nOBS();
      lVar12 = 0;
      lVar9 = local_b0 + local_a8 + local_a0;
      lVar4 = param_1 - lVar9;
      if (0 < param_1) {
        lVar11 = 1;
        do {
          lVar10 = puVar2[lVar11] - lVar9;
          if (0 < lVar10) {
            lVar12 = lVar12 + 1;
            lVar8 = *(long *)(lVar6 + lVar11 * 8) - lVar9;
            if (((lVar4 < lVar10) || (lVar8 < 1)) || (lVar4 < lVar8)) {
              KOBOOLOXSPAIPP(1,"reduceproblem.c",0x2ec);
            }
            *(long *)(lVar7 + lVar8 * 8) = lVar10;
          }
          lVar11 = lVar11 + 1;
        } while (lVar11 <= param_1);
      }
      if (lVar12 != lVar4) {
        KOBOOLOXSPAIPP(1,"reduceproblem.c",0x2f3);
      }
    }
    if (lVar6 != 0) {
      _yPFdOBBXSPAIPP(lVar6);
    }
    PBQbBD123pBLOABOtB_nOBS(0);
  }
  lVar6 = local_b0 + local_a8;
  ABD2lLABPgKnOLzmOFDXSPAIPP = local_a8;
  ABD1lLABPgKnOLzmOFDXSPAIPP = local_b0;
  ABD3lLABPgKnOLzmOFDXSPAIPP = local_a0;
  lVar7 = lVar6 + local_a0;
  *param_5 = local_b0;
  *param_6 = lVar6;
  *param_7 = local_a0;
  puVar3 = puVar2;
  if (lVar1 == 0) {
    TOFQBXbce123pcmpbcptcaXSPAIPP(puVar2,param_1);
    puVar3 = DAT_00e8e758;
  }
  DAT_00e8e758 = puVar3;
  OBKRJzBOYQcKAjFPQ(param_1,puVar2,lVar7,param_3,param_4);
  puVar3 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,param_1,0);
  *puVar3 = 0;
  OByAdtB_QLOdlRJXSPAIPP(0x1a,param_1,puVar3);
  _EB_HdLOlBDyQFSBbFyDPXSPAIPP(param_1,puVar3);
  EyKAIBgPLIyQBAtyOPgKbFyDtB_XSPAIPP(param_1,puVar3,0);
  MBOJRQBdtB_QLOgKnIy_BXSPAIPP(puVar2,param_1,puVar3);
  FKFQuOFQBdOLJrLmCCbFyDXSPAIPP(0x15,0x16,0x17);
  FKFQaERKHuOFQBXSPAIPP(0xd);
  local_98 = 0;
  local_b8 = 0;
  lVar4 = CLOJkyQdLObBD1bBD2YKAqMIFQXSPAIPP
                    (DAT_00e3b560,param_1,param_2,local_b0,local_a8,local_a0,puVar2,&local_98,
                     &local_b8);
  if (lVar1 == 0) {
    _yPFdOBBXSPAIPP(puVar2);
    *(undefined8 **)(lVar4 + 0x18) = puVar3;
    Cy_QLObBD1bBD2nCXSPAIPP(local_a0,lVar4,puVar3,&local_98,&local_b8);
    CFKFPEaERKHuOFQBXSPAIPP();
    TOFQBbBD1bBD2nCXSPAIPP(lVar4);
    *(undefined8 *)(lVar4 + 0x18) = 0;
    ABIBQBbBD1bBD2nCXSPAIPP(lVar4);
  }
  else {
    *(undefined8 **)(lVar4 + 0x18) = puVar3;
    Cy_QLObBD1bBD2nCXSPAIPP(local_a0,lVar4,puVar3,&local_98,&local_b8);
    CFKFPEaERKHuOFQBXSPAIPP();
    uVar5 = QyDlBTdtB_QLOXSPAIPP(0,DAT_00e3b560,lVar6,0);
    *(undefined8 *)(lVar4 + 0x18) = uVar5;
    _LMVdtB_QLOXSPAIPP(puVar3,lVar6,uVar5);
    DAT_00e8e750 = lVar4;
  }
  lVar4 = param_1 - lVar6;
  TOFQBdtB_QLOdlRJXSPAIPP(3,lVar4,puVar3 + lVar6);
  _yPFdOBBXSPAIPP(puVar3);
  uVar5 = CLOJkyQdLObBD3XSPAIPP(DAT_00e3b560,lVar4,local_a0,local_98);
  _yPFpBJLSBXSPAIPP(0xd);
  puVar2 = (undefined8 *)KBTdtB_QLOXSPAIPP(0,lVar4,0);
  *puVar2 = 0;
  OByAdtB_QLOdlRJXSPAIPP(3,lVar4,puVar2);
  _yPFpBJLSBXSPAIPP(3);
  Cy_QLObBD3nCXSPAIPP(uVar5,puVar2,&local_b8);
  lVar6 = CFKFPEuOFQBdOLJrLmCCbFyDXSPAIPP();
  if (lVar6 != local_b8) {
    KOBOOLOXSPAIPP(1,"reduceproblem.c",0x36a);
  }
  if (lVar1 == 0) {
    TOFQBbBD3nCXSPAIPP(uVar5);
    ABIBQBbBD3nCXSPAIPP(uVar5);
    TOFQBdtB_QLOdlRJXSPAIPP(0x14,param_1 - lVar7,puVar2 + local_a0);
    _yPFdOBBXSPAIPP(puVar2);
  }
  else {
    DAT_00e8e748 = uVar5;
    TOFQBdtB_QLOdlRJXSPAIPP(0x14,param_1 - lVar7,puVar2 + local_a0);
    _yPFdOBBXSPAIPP(puVar2);
    PBQnCgKCLhOQXSPAIPP(DAT_00e3b560,DAT_00e8e758,DAT_00e8e750,DAT_00e8e748);
  }
  return local_b8;
}


