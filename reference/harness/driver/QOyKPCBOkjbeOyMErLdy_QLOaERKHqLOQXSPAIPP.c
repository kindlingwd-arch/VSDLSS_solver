/* driver: QOyKPCBOkjbeOyMErLdy_QLOaERKHqLOQXSPAIPP addr=009a0690 size=262 */
#include "vsdlss_ref.h"


void QOyKPCBOkjbeOyMErLdy_QLOaERKHqLOQXSPAIPP(long param_1,long param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  ulong uVar4;
  long *plVar5;
  bool bVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  ulong uVar11;
  long *plVar12;
  long lVar13;
  long lVar14;
  long *plVar15;
  long lVar16;
  long local_48;
  
  lVar1 = param_3[5];
  lVar10 = param_3[2];
  uVar2 = *param_3;
  lVar13 = *(long *)(param_2 + 0x60);
  lVar14 = *(long *)(param_2 + 0x50);
  lVar3 = param_3[8];
  lVar7 = QyDlBTgtB_QLOXSPAIPP(uVar2,lVar10,0,0);
  param_3[9] = lVar7;
  lVar8 = QyDlBTgntB_QLOXSPAIPP(uVar2,lVar10,1,0);
  param_3[10] = lVar8;
  lVar9 = 1;
  if (0 < lVar10) {
    do {
      *(long *)(lVar7 + *(long *)(param_1 + lVar9 * 8) * 8) = *(long *)(lVar14 + lVar9 * 8) + 1;
      lVar16 = *(long *)(lVar13 + lVar9 * 8);
      *(undefined8 *)(lVar14 + lVar9 * 8) = 0;
      *(long *)(lVar8 + *(long *)(param_1 + lVar9 * 8) * 8) = lVar16 + -8;
      *(undefined8 *)(lVar13 + lVar9 * 8) = 0;
      lVar9 = lVar9 + 1;
    } while (lVar9 != lVar10 + 1);
  }
  lVar10 = DBQkyUgtB_QLOXSPAIPP(lVar7,lVar10);
  if (lVar10 < 0x41) {
    local_48 = 0;
  }
  else {
    local_48 = KBTgtB_QLOXSPAIPP(lVar10,0,0);
  }
  lVar14 = 1;
  lVar13 = 1;
  if (0 < lVar1) {
    do {
      lVar9 = *(long *)(lVar3 + lVar14 * 8);
      uVar4 = *(ulong *)(lVar7 + lVar13 * 8);
      plVar5 = *(long **)(lVar8 + lVar13 * 8);
      uVar11 = 1;
      if (uVar4 != 0) {
        do {
          plVar5[uVar11] = *(long *)(param_1 + plVar5[uVar11] * 8);
          uVar11 = uVar11 + 1;
        } while (uVar11 <= uVar4);
        if ((long)uVar4 < 0x41) {
          plVar15 = plVar5 + uVar4;
          if (0 < (long)uVar4) {
            while (plVar15 != plVar5 + 1) {
              bVar6 = true;
              plVar12 = plVar5 + 1;
              do {
                lVar16 = *plVar12;
                if (plVar12[1] < lVar16) {
                  *plVar12 = plVar12[1];
                  plVar12[1] = lVar16;
                  bVar6 = false;
                }
                plVar12 = plVar12 + 1;
              } while (plVar12 != plVar15);
              if ((bVar6) || (plVar15 = plVar15 + -1, plVar15 == plVar5)) break;
            }
          }
          if ((long)uVar4 < 2) goto LAB_009a0883;
        }
        else {
          _yPFpyAFUqLOQZyPB8(uVar4,local_48,plVar5);
        }
        lVar16 = 2;
        do {
          while (plVar5[lVar16 + -1] < plVar5[lVar16]) {
            lVar16 = lVar16 + 1;
            if ((long)uVar4 < lVar16) goto LAB_009a0883;
          }
          lVar16 = lVar16 + 1;
          KOBOOLOXSPAIPP(1,"mldorder.c",0x1694);
        } while (lVar16 <= (long)uVar4);
      }
LAB_009a0883:
      lVar13 = lVar13 + lVar9;
      lVar14 = lVar14 + 1;
    } while (lVar14 <= lVar1);
  }
  if ((0x40 < lVar10) && (local_48 != 0)) {
    _yPFdOBBXSPAIPP();
    return;
  }
  return;
}


