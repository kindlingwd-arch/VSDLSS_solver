/* driver: LOABOfFDEbBDlLABPYQcKAXSPAIPP addr=0099f130 size=26e */
#include "vsdlss_ref.h"


long LOABOfFDEbBDlLABPYQcKAXSPAIPP
               (long param_1,long param_2,long param_3,long *param_4,long param_5)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  double dVar15;
  
  lVar5 = 0;
  lVar2 = *(long *)(param_1 + 0x10);
  lVar11 = *(long *)(param_1 + 0x50);
  if (999 < lVar2) {
    lVar4 = KBTatB_QLOXSPAIPP(lVar2,1,0);
    plVar10 = (long *)(param_3 + 8);
    plVar1 = plVar10 + param_2;
    if (0 < param_2) {
      do {
        lVar5 = *plVar10;
        plVar10 = plVar10 + 1;
        *(undefined1 *)(lVar4 + lVar5) = 1;
      } while (plVar10 != plVar1);
    }
    lVar5 = ((*(long *)(param_1 + 0x18) * 2) / lVar2) * 0x1e;
    dVar15 = pow((double)lVar2,DAT_00ab7228);
    dVar15 = floor(dVar15);
    lVar12 = 10;
    if (9 < lVar5) {
      lVar12 = lVar5;
    }
    lVar14 = 0;
    lVar5 = 1;
    do {
      if (lVar12 < *(long *)(lVar11 + lVar5 * 8)) {
        lVar14 = lVar14 + (ulong)(*(char *)(lVar4 + lVar5) == '\0');
      }
      lVar5 = lVar5 + 1;
    } while (lVar5 != lVar2 + 1);
    lVar6 = KBTgtB_QLOXSPAIPP(lVar14,0,0);
    lVar7 = KBTgtB_QLOXSPAIPP(lVar14,0,0);
    lVar8 = KBTgtB_QLOXSPAIPP(lVar14,0,0);
    lVar9 = KBTgtB_QLOXSPAIPP(lVar14,0,0);
    lVar13 = 0;
    lVar5 = 1;
    do {
      lVar3 = *(long *)(lVar11 + lVar5 * 8);
      if ((lVar12 < lVar3) && (*(char *)(lVar4 + lVar5) == '\0')) {
        lVar13 = lVar13 + 1;
        *(long *)(lVar9 + lVar13 * 8) = lVar3;
        *(long *)(lVar8 + lVar13 * 8) = lVar5;
      }
      lVar5 = lVar5 + 1;
    } while (lVar2 + 1 != lVar5);
    if (lVar14 != lVar13) {
      KOBOOLOXSPAIPP(1,"mldorder.c",0x13e3);
    }
    lVar5 = 0;
    if (lVar14 != 0) {
      _yPFpyAFUqLOQnBOJXSPAIPP(lVar14,lVar6,lVar9,lVar8,lVar7);
      lVar11 = 1;
      lVar5 = 10;
      if (9 < (long)dVar15) {
        lVar5 = (long)dVar15;
      }
      if (lVar14 <= lVar5) {
        lVar5 = lVar14;
      }
      do {
        *(undefined8 *)(param_5 + (lVar2 - (param_2 + lVar5)) * 8 + lVar11 * 8) =
             *(undefined8 *)(lVar8 + (lVar14 - lVar5) * 8 + lVar11 * 8);
        lVar11 = lVar11 + 1;
      } while (lVar11 <= lVar5);
      *param_4 = *param_4 + lVar5;
    }
    if (lVar4 != 0) {
      _yPFdOBBXSPAIPP(lVar4);
    }
    if (lVar6 != 0) {
      _yPFdOBBXSPAIPP();
    }
    if (lVar7 != 0) {
      _yPFdOBBXSPAIPP();
    }
    if (lVar8 != 0) {
      _yPFdOBBXSPAIPP(lVar8);
    }
    if (lVar9 != 0) {
      _yPFdOBBXSPAIPP();
    }
  }
  return lVar5;
}


