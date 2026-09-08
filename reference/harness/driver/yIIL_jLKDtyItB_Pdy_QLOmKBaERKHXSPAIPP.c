/* driver: yIIL_jLKDtyItB_Pdy_QLOmKBaERKHXSPAIPP addr=009699f0 size=1cc */
#include "vsdlss_ref.h"


void yIIL_jLKDtyItB_Pdy_QLOmKBaERKHXSPAIPP(undefined8 *param_1)

{
  long *plVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  ulong uVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  
  uVar2 = *param_1;
  lVar8 = param_1[2];
  lVar3 = param_1[9];
  lVar4 = param_1[8];
  lVar5 = param_1[5];
  lVar7 = lVar8 >> 10;
  if (lVar8 < 0x400) {
    lVar7 = 1;
  }
  lVar8 = QyDlBTdntB_QLOXSPAIPP(uVar2,lVar8,1,0);
  param_1[0xb] = lVar8;
  if (0 < lVar5) {
    lVar16 = 1;
    lVar12 = 0;
    lVar9 = 0;
    lVar15 = 1;
    while( true ) {
      lVar6 = *(long *)(lVar4 + lVar16 * 8);
      plVar1 = (long *)(lVar3 + lVar15 * 8);
      lVar14 = *plVar1;
      uVar13 = lVar6 * lVar14 - ((lVar6 + -1) * lVar6) / 2;
      lVar10 = lVar9;
      if (lVar12 < (long)uVar13) {
        lVar12 = uVar13 + 1;
        if ((long)uVar13 < lVar7) {
          lVar12 = lVar7;
        }
        lVar10 = KBTZIL_HXSPAIPP(uVar2,lVar12 * 8);
        lVar14 = *plVar1;
        lVar12 = lVar12 + -1;
      }
      lVar12 = lVar12 - uVar13;
      lVar9 = lVar10 + uVar13 * 8;
      uVar11 = 1;
      if (uVar13 != 0) {
        do {
          *(undefined8 *)(lVar10 + uVar11 * 8) = 0;
          uVar11 = uVar11 + 1;
        } while (uVar11 <= uVar13);
      }
      *(long *)(lVar8 + lVar15 * 8) = lVar10;
      lVar10 = lVar15 + 1;
      lVar15 = lVar15 + lVar6;
      lVar14 = uVar13 - lVar14;
      if (lVar10 < lVar15) {
        do {
          lVar14 = lVar14 - *(long *)(lVar3 + lVar10 * 8);
          *(long *)(lVar8 + lVar10 * 8) =
               *(long *)(lVar8 + -8 + lVar10 * 8) + *(long *)(lVar3 + -8 + lVar10 * 8) * 8;
          lVar10 = lVar10 + 1;
        } while (lVar10 != lVar15);
      }
      if (lVar14 != 0) {
        KOBOOLOXSPAIPP(1,"vsdlss1memutils.c",0x249);
      }
      if (lVar5 < lVar16 + 1) break;
      lVar16 = lVar16 + 1;
    }
  }
  return;
}


