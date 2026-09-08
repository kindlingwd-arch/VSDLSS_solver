/* driver: kkbdOLJkjbqQRzXSPAIPP addr=009b69a0 size=4c5 */
#include "vsdlss_ref.h"


void kkbdOLJkjbqQRzXSPAIPP
               (long param_1,long param_2,undefined8 param_3,long param_4,undefined8 *param_5)

{
  long lVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  ulong uVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  long lVar13;
  long local_b8;
  long local_b0;
  long local_a8;
  long local_88;
  long local_80;
  long local_60;
  undefined8 local_48;
  undefined8 local_40 [2];
  
  lVar2 = *(long *)(param_1 + 0x10);
  uVar6 = KBTaLRKQFKDkyOHXSPAIPP(lVar2);
  lVar3 = param_5[8];
  uVar4 = *param_5;
  lVar7 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
  if (lVar2 < 0x400) {
    lVar8 = PRJgtB_QLOcIBJPXSPAIPP(*(undefined8 *)(param_1 + 0x50),lVar2);
    local_88 = (lVar8 * 3) / lVar2;
    if (local_88 < 9) {
      local_88 = 9;
    }
    local_b0 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
    _LMVgtB_QLOXSPAIPP(param_4,lVar2,local_b0);
    local_80 = KBTatB_QLOXSPAIPP(lVar2,0,0);
    JyOHYKAkLSBrLcKA(lVar2,param_2,param_3,local_b0,local_80);
    local_a8 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
    FKFQgtB_QLOXSPAIPP(local_a8,lVar2,0);
    FKFQgtB_QLOXSPAIPP(param_4,lVar2,0xffffffff8000000c);
    local_48 = 0;
    if (lVar2 < 1) {
      local_b8 = 0;
      goto LAB_009b6c8b;
    }
    local_60 = 1;
  }
  else {
    local_60 = lVar2 >> 10;
    lVar8 = PRJgtB_QLOcIBJPXSPAIPP(*(undefined8 *)(param_1 + 0x50),lVar2);
    local_88 = (lVar8 * 3) / lVar2;
    if (local_88 < 9) {
      local_88 = 9;
    }
    local_b0 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
    _LMVgtB_QLOXSPAIPP(param_4,lVar2,local_b0);
    local_80 = KBTatB_QLOXSPAIPP(lVar2,0,0);
    JyOHYKAkLSBrLcKA(lVar2,param_2,param_3,local_b0,local_80);
    local_a8 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
    FKFQgtB_QLOXSPAIPP(local_a8,lVar2,0);
    FKFQgtB_QLOXSPAIPP(param_4,lVar2,0xffffffff8000000c);
  }
  local_48 = 0;
  lVar8 = 0;
  lVar13 = 0;
  lVar12 = 1;
  local_b8 = 0;
  do {
    if ((lVar2 - param_2) + 1 == lVar12) {
      FKFQatB_QLOXSPAIPP(local_80,lVar2,0);
    }
    lVar5 = *(long *)(local_b0 + lVar12 * 8);
    if (*(long *)(param_4 + lVar5 * 8) < -0x3ffffff9) {
      if (0 < *(long *)(local_a8 + lVar5 * 8)) {
        kjbsMAyQBlLABYAGYOOyVXSPAIPP(lVar5,param_4,param_1,local_a8,uVar6);
      }
      uVar9 = kjbdFKApBy_EYOOpBQqFWB(lVar5,param_4,param_1,lVar7,uVar6);
      lVar1 = uVar9 + 1;
      lVar10 = lVar8;
      if (lVar13 < lVar1) {
        lVar8 = uVar9 + 2;
        if (lVar1 < local_60) {
          lVar8 = local_60;
        }
        lVar13 = lVar8 + -1;
        lVar10 = KBTZIL_HXSPAIPP(uVar4,lVar8 * 8);
      }
      lVar8 = lVar10 + lVar1 * 8;
      lVar13 = lVar13 - lVar1;
      *(long *)(lVar10 + 8) = lVar5;
      uVar11 = 1;
      if (uVar9 != 0) {
        do {
          *(undefined8 *)(lVar10 + 8 + uVar11 * 8) = *(undefined8 *)(lVar7 + uVar11 * 8);
          uVar11 = uVar11 + 1;
        } while (uVar11 <= uVar9);
      }
      if (local_88 < (long)uVar9) {
        kjbcIFJgKAFPQlLABPYOOqQRzaLJzL
                  (lVar5,local_40,uVar9,lVar10 + 8,param_4,param_1,local_a8,&local_48,uVar6,local_80
                  );
      }
      else {
        local_40[0] = 1;
        kjbcIFJgKAFPQlLABPYOOqQRzX1
                  (lVar5,1,uVar9,lVar10 + 8,param_4,param_1,local_a8,&local_48,uVar6);
      }
      local_b8 = local_b8 + 1;
      *(undefined8 *)(lVar3 + local_b8 * 8) = local_40[0];
    }
    lVar12 = lVar12 + 1;
  } while (lVar12 <= lVar2);
LAB_009b6c8b:
  kjbdFKFPEpBLOABOFKDXSPAIPP(lVar2,local_48,param_4,uVar6);
  _EB_HmOABOBAYQcKA(lVar2,param_2,param_3,param_4);
  if (local_a8 != 0) {
    _yPFdOBBXSPAIPP();
  }
  if (local_80 != 0) {
    _yPFdOBBXSPAIPP();
  }
  if (local_b0 != 0) {
    _yPFdOBBXSPAIPP();
  }
  ABIBQBaLRKQFKDkyOHXSPAIPP(uVar6);
  if (lVar7 != 0) {
    _yPFdOBBXSPAIPP(lVar7);
  }
  param_5[6] = 1;
  param_5[5] = local_b8;
  param_5[7] = local_b8;
  return;
}


