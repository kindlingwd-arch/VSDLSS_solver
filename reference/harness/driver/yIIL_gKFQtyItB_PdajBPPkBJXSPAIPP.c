/* driver: yIIL_gKFQtyItB_PdajBPPkBJXSPAIPP addr=0096a810 size=473 */
#include "vsdlss_ref.h"


void yIIL_gKFQtyItB_PdajBPPkBJXSPAIPP(long *param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long local_a8;
  long local_a0;
  long local_80;
  long local_68;
  long local_50;
  long local_40;
  
  uVar1 = *param_2;
  lVar2 = param_2[2];
  lVar3 = param_2[10];
  lVar4 = param_2[9];
  if (lVar2 < 0x400) {
    local_a8 = param_1[9];
    lVar16 = param_1[8];
    lVar14 = param_1[7];
    local_a0 = param_1[6];
    local_80 = *param_1;
    local_40 = DBQZOByHnLFKQgKA(param_1,param_2);
    local_68 = QyDlBTdntB_QLOXSPAIPP(uVar1,lVar2,1,0);
    param_2[0xb] = local_68;
    if (lVar2 < 1) goto LAB_0096ab65;
    local_50 = 1;
  }
  else {
    local_a8 = param_1[9];
    lVar16 = param_1[8];
    local_50 = lVar2 >> 10;
    lVar14 = param_1[7];
    local_a0 = param_1[6];
    local_80 = *param_1;
    local_40 = DBQZOByHnLFKQgKA(param_1,param_2);
    local_68 = QyDlBTdntB_QLOXSPAIPP(uVar1,lVar2,1,0);
    param_2[0xb] = local_68;
  }
  lVar13 = 0;
  lVar12 = 0;
  lVar15 = 1;
  while( true ) {
    lVar7 = *(long *)(lVar4 + lVar15 * 8);
    lVar5 = lVar12;
    if (lVar13 < lVar7) {
      lVar12 = lVar7 + 1;
      if (lVar7 < local_50) {
        lVar12 = local_50;
      }
      lVar13 = lVar12 + -1;
      lVar5 = KBTZIL_HXSPAIPP(uVar1,lVar12 * 8);
    }
    lVar12 = lVar5 + lVar7 * 8;
    lVar13 = lVar13 - lVar7;
    lVar10 = 2;
    *(long *)(local_68 + lVar15 * 8) = lVar5;
    lVar8 = *(long *)(lVar3 + lVar15 * 8);
    lVar9 = *(long *)(local_a0 + 8);
    lVar6 = 1;
    *(undefined8 *)(lVar5 + 8) = *(undefined8 *)(local_a8 + 8);
    if ((1 < lVar7) && (lVar11 = lVar10, 0 < lVar9)) {
      do {
        if (*(long *)(lVar14 + lVar6 * 8) == *(long *)(lVar8 + lVar11 * 8)) {
          lVar10 = lVar6 * 8;
          lVar6 = lVar6 + 1;
          *(undefined8 *)(lVar5 + lVar11 * 8) = *(undefined8 *)(lVar16 + lVar10);
        }
        else {
          *(undefined8 *)(lVar5 + lVar11 * 8) = 0;
        }
        lVar10 = lVar11 + 1;
      } while ((lVar6 <= lVar9) && (lVar10 = lVar11 + 1, lVar11 = lVar10, lVar10 <= lVar7));
    }
    if (lVar6 <= lVar9) {
      KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x42d);
    }
    if (lVar10 <= lVar7) {
      do {
        *(undefined8 *)(lVar5 + lVar10 * 8) = 0;
        lVar10 = lVar10 + 1;
      } while (lVar10 != lVar7 + 1);
    }
    local_80 = local_80 - lVar9;
    local_a0 = local_a0 + 8;
    lVar14 = lVar14 + lVar9 * 8;
    lVar16 = lVar16 + lVar9 * 8;
    local_a8 = local_a8 + 8;
    if ((lVar15 < lVar2) && (lVar15 == local_40)) {
      lVar7 = KBTgtB_QLOXSPAIPP(local_80,0,0);
      _LMVgtB_QLOXSPAIPP(lVar14,local_80,lVar7);
      if (param_1[7] != 0) {
        _yPFdOBBXSPAIPP();
      }
      param_1[7] = lVar7;
      lVar5 = KBTdtB_QLOXSPAIPP(0,local_80,0);
      _LMVdtB_QLOXSPAIPP(lVar16,local_80,lVar5);
      if (param_1[8] != 0) {
        _yPFdOBBXSPAIPP();
      }
      lVar16 = lVar2 - lVar15;
      param_1[8] = lVar5;
      lVar8 = KBTgtB_QLOXSPAIPP(lVar16,0,0);
      _LMVgtB_QLOXSPAIPP(local_a0,lVar16,lVar8);
      if (param_1[6] != 0) {
        _yPFdOBBXSPAIPP();
      }
      param_1[6] = lVar8;
      lVar9 = KBTdtB_QLOXSPAIPP(0,lVar16,0);
      _LMVdtB_QLOXSPAIPP(local_a8,lVar16,lVar9);
      if (param_1[9] != 0) {
        _yPFdOBBXSPAIPP();
      }
      param_1[9] = lVar9;
      lVar14 = lVar7;
      lVar16 = lVar5;
      local_a0 = lVar8;
      local_a8 = lVar9;
    }
    if (lVar2 < lVar15 + 1) break;
    lVar15 = lVar15 + 1;
  }
LAB_0096ab65:
  if (local_80 != 0) {
    KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x454);
  }
  if (param_1[7] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[7] = 0;
  if (param_1[8] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[8] = 0;
  if (param_1[6] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[6] = 0;
  if (param_1[9] != 0) {
    _yPFdOBBXSPAIPP();
  }
  param_1[9] = 0;
  return;
}


