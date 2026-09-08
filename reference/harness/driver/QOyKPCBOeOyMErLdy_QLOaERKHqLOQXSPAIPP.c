/* driver: QOyKPCBOeOyMErLdy_QLOaERKHqLOQXSPAIPP addr=00969660 size=27b */
#include "vsdlss_ref.h"


void QOyKPCBOeOyMErLdy_QLOaERKHqLOQXSPAIPP(long param_1,long param_2,undefined8 *param_3)

{
  long *plVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  bool bVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  ulong uVar12;
  long *plVar13;
  long *plVar14;
  long lVar15;
  long lVar16;
  long local_70;
  long local_48;
  
  lVar2 = param_3[5];
  lVar11 = param_3[2];
  uVar3 = *param_3;
  lVar15 = *(long *)(param_2 + 0x30);
  lVar4 = *(long *)(param_2 + 0x10);
  lVar5 = param_3[8];
  lVar8 = QyDlBTgtB_QLOXSPAIPP(uVar3,lVar11,0,0);
  param_3[9] = lVar8;
  lVar9 = QyDlBTgntB_QLOXSPAIPP(uVar3,lVar11,1,0);
  param_3[10] = lVar9;
  lVar10 = 1;
  if (0 < lVar11) {
    do {
      *(long *)(lVar8 + *(long *)(param_1 + lVar10 * 8) * 8) = *(long *)(lVar4 + lVar10 * 8) + 1;
      lVar16 = *(long *)(lVar15 + lVar10 * 8);
      *(undefined8 *)(lVar4 + lVar10 * 8) = 0;
      *(long *)(lVar9 + *(long *)(param_1 + lVar10 * 8) * 8) = lVar16 + -8;
      *(undefined8 *)(lVar15 + lVar10 * 8) = 0;
      lVar10 = lVar10 + 1;
    } while (lVar10 != lVar11 + 1);
  }
  lVar11 = DBQkyUgtB_QLOXSPAIPP(lVar8,lVar11);
  if (lVar11 < 0x41) {
    local_48 = 0;
  }
  else {
    local_48 = KBTgtB_QLOXSPAIPP(lVar11,0,0);
  }
  local_70 = 1;
  lVar15 = 1;
  if (0 < lVar2) {
    do {
      lVar4 = *(long *)(lVar5 + local_70 * 8);
      uVar6 = *(ulong *)(lVar8 + lVar15 * 8);
      lVar10 = *(long *)(lVar9 + lVar15 * 8);
      uVar12 = 1;
      if (uVar6 != 0) {
        do {
          *(undefined8 *)(lVar10 + uVar12 * 8) =
               *(undefined8 *)(param_1 + *(long *)(lVar10 + uVar12 * 8) * 8);
          uVar12 = uVar12 + 1;
        } while (uVar12 <= uVar6);
      }
      lVar16 = uVar6 - lVar4;
      plVar1 = (long *)(lVar10 + lVar4 * 8);
      if (lVar16 < 0x41) {
        plVar14 = plVar1 + lVar16;
        if (0 < lVar16) {
          while (plVar14 != plVar1 + 1) {
            bVar7 = true;
            plVar13 = plVar1 + 1;
            do {
              lVar10 = *plVar13;
              if (plVar13[1] < lVar10) {
                *plVar13 = plVar13[1];
                plVar13[1] = lVar10;
                bVar7 = false;
              }
              plVar13 = plVar13 + 1;
            } while (plVar13 != plVar14);
            if ((bVar7) || (plVar14 = plVar14 + -1, plVar14 == plVar1)) break;
          }
        }
      }
      else {
        _yPFpyAFUqLOQZyPB8(lVar16,local_48,plVar1);
      }
      lVar10 = 2;
      if (1 < (long)uVar6) {
        do {
          while (plVar1[(lVar10 + -1) - lVar4] < plVar1[lVar10 - lVar4]) {
            lVar10 = lVar10 + 1;
            if ((long)uVar6 < lVar10) goto LAB_00969864;
          }
          lVar10 = lVar10 + 1;
          KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x1e9);
        } while (lVar10 <= (long)uVar6);
      }
LAB_00969864:
      local_70 = local_70 + 1;
      lVar15 = lVar15 + lVar4;
    } while (local_70 <= lVar2);
  }
  if ((0x40 < lVar11) && (local_48 != 0)) {
    _yPFdOBBXSPAIPP();
    return;
  }
  return;
}


