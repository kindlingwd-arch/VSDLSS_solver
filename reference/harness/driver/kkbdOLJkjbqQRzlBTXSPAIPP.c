/* driver: kkbdOLJkjbqQRzlBTXSPAIPP addr=009b7410 size=43a */
#include "vsdlss_ref.h"


void kkbdOLJkjbqQRzlBTXSPAIPP
               (long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
               undefined8 *param_5)

{
  long *plVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long *plVar5;
  bool bVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  long *plVar11;
  ulong uVar12;
  long *plVar13;
  long lVar14;
  long *plVar15;
  long lVar16;
  long lVar17;
  long *plVar18;
  long *local_c0;
  long local_b0;
  long local_98;
  undefined1 local_58 [40];
  
  lVar2 = *(long *)(param_1 + 0x10);
  lVar14 = param_5[8];
  uVar3 = *param_5;
  lVar7 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
  lVar8 = lVar2 >> 10;
  if (lVar2 < 0x400) {
    lVar8 = 1;
  }
  lVar4 = *(long *)(param_1 + 0x50);
  plVar5 = *(long **)(param_1 + 0x60);
  KBTaLRKQFKDkyOHlLfByABO(local_58,lVar2);
  _EB_HYQcKAjFPQmOABOBAYQcKA(lVar2,param_4,param_2,param_3);
  lVar9 = KBTgtB_QLOXSPAIPP(lVar2,0,0);
  _LMVgtB_QLOXSPAIPP(param_4,lVar2,lVar9);
  DBQgKSBOPBkyMXSPAIPP(lVar9,lVar2,param_4);
  if (lVar9 != 0) {
    _yPFdOBBXSPAIPP(lVar9);
  }
  MBOJRQBkjbeOyME(param_4,param_1);
  lVar9 = KBTgtB_QLOXSPAIPP(lVar2,1,0);
  if (lVar2 < 1) {
    local_98 = 0;
  }
  else {
    plVar18 = (long *)0x0;
    lVar17 = 0;
    local_b0 = 0;
    local_98 = 0;
    lVar16 = 1;
    plVar11 = plVar5;
    do {
      local_c0 = plVar11 + 1;
      RMAyQBoyIcIFJbRM(param_1,lVar16,lVar17,lVar9,local_58);
      if ((*(long *)(lVar4 + lVar16 * 8) == 1) && (lVar16 + -1 == *(long *)(*local_c0 + 8))) {
        *(long *)(lVar9 + -8 + lVar16 * 8) = lVar16;
        *(undefined8 *)(lVar9 + lVar16 * 8) = 0;
        plVar1 = (long *)(lVar14 + local_98 * 8);
        *plVar1 = *plVar1 + 1;
        *(long *)(lVar4 + lVar16 * 8) = *(long *)(lVar4 + -8 + lVar16 * 8) + -1;
        *local_c0 = *plVar11 + 8;
      }
      else {
        uVar10 = kjbdFKApBy_EYOOpBQqFWBlBT(lVar16,param_1,lVar7,lVar9,local_58);
        lVar17 = uVar10 + 1;
        plVar11 = plVar18;
        if (local_b0 < lVar17) {
          local_b0 = uVar10 + 2;
          if (lVar17 < lVar8) {
            local_b0 = lVar8;
          }
          plVar11 = (long *)KBTZIL_HXSPAIPP(uVar3,local_b0 * 8);
          local_b0 = local_b0 + -1;
        }
        plVar18 = plVar11 + lVar17;
        local_b0 = local_b0 - lVar17;
        plVar11[1] = lVar16;
        plVar1 = plVar11 + 1;
        uVar12 = 1;
        if (uVar10 != 0) {
          do {
            plVar11[uVar12 + 1] = *(long *)(lVar7 + uVar12 * 8);
            uVar12 = uVar12 + 1;
          } while (uVar12 <= uVar10);
          if ((long)uVar10 < 0x41) {
            plVar15 = plVar18;
            if (0 < (long)uVar10) {
              while (plVar15 != plVar11 + 2) {
                bVar6 = true;
                plVar13 = plVar11 + 2;
                do {
                  lVar17 = *plVar13;
                  if (plVar13[1] < lVar17) {
                    *plVar13 = plVar13[1];
                    plVar13[1] = lVar17;
                    bVar6 = false;
                  }
                  plVar13 = plVar13 + 1;
                } while (plVar13 != plVar15);
                if ((bVar6) || (plVar15 = plVar15 + -1, plVar15 == plVar1)) break;
              }
            }
            if ((long)uVar10 < 2) goto LAB_009b76c3;
          }
          else {
            _yPFpyAFUqLOQZyPB8(uVar10,lVar7,plVar1);
          }
          lVar17 = 2;
          do {
            while (plVar11[lVar17] < plVar11[lVar17 + 1]) {
              lVar17 = lVar17 + 1;
              if ((long)uVar10 < lVar17) goto LAB_009b76c3;
            }
            lVar17 = lVar17 + 1;
            KOBOOLOXSPAIPP(1,"mldmmd.c",0x9c6);
          } while (lVar17 <= (long)uVar10);
        }
LAB_009b76c3:
        local_98 = local_98 + 1;
        *(undefined8 *)(lVar14 + local_98 * 8) = 1;
        *(ulong *)(lVar4 + lVar16 * 8) = uVar10;
        *local_c0 = (long)plVar1;
        lVar17 = lVar16;
      }
      lVar16 = lVar16 + 1;
      plVar11 = local_c0;
    } while (lVar16 <= lVar2);
  }
  if (lVar9 != 0) {
    _yPFdOBBXSPAIPP();
  }
  ABIBQBaLRKQFKDkyOHlLfByABO(local_58);
  if (lVar7 != 0) {
    _yPFdOBBXSPAIPP(lVar7);
  }
  param_5[6] = 1;
  param_5[5] = local_98;
  param_5[7] = local_98;
  lVar14 = QyDlBTgtB_QLOXSPAIPP(uVar3,lVar2,0,0);
  param_5[9] = lVar14;
  lVar7 = QyDlBTgntB_QLOXSPAIPP(uVar3,lVar2,1,0);
  param_5[10] = lVar7;
  lVar8 = 1;
  if (0 < lVar2) {
    do {
      lVar9 = plVar5[lVar8];
      *(long *)(lVar14 + lVar8 * 8) = *(long *)(lVar4 + lVar8 * 8) + 1;
      *(undefined8 *)(lVar4 + lVar8 * 8) = 0;
      *(long *)(lVar7 + lVar8 * 8) = lVar9 + -8;
      plVar5[lVar8] = 0;
      lVar8 = lVar8 + 1;
    } while (lVar8 != lVar2 + 1);
  }
  return;
}


