/* driver: JRIQFMIBqMyOPBbBKPBqyUMV1zV6XSPAIPP addr=009b06d0 size=31b */
#include "vsdlss_ref.h"


void JRIQFMIBqMyOPBbBKPBqyUMV1zV6XSPAIPP
               (undefined8 param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  double *pdVar6;
  long lVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  
  lVar14 = *(long *)(param_6 + 8);
  switch(param_1) {
  default:
    KOBOOLOXSPAIPP(0xf,"arithkernels.c",0x28b,6,"multipleSparseDenseSaxpy1by6");
    return;
  case 1:
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = 1;
    dVar8 = *(double *)(param_3 + 0x40);
    if (0 < param_2) {
      do {
        lVar18 = lVar17 * 8;
        lVar19 = lVar17 * 8;
        lVar17 = lVar17 + 1;
        pdVar6 = (double *)(lVar14 + *(long *)(param_5 + lVar19) * 8);
        *pdVar6 = *(double *)(lVar15 + lVar18) * dVar8 + *pdVar6;
      } while (lVar17 != param_2 + 1);
      return;
    }
    break;
  case 2:
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = *(long *)(param_4 + 0x10);
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    if (0 < param_2) {
      lVar18 = 1;
      do {
        lVar19 = lVar18 * 8;
        lVar20 = lVar18 * 8;
        lVar21 = lVar18 * 8;
        lVar18 = lVar18 + 1;
        pdVar6 = (double *)(lVar14 + *(long *)(param_5 + lVar20) * 8);
        *pdVar6 = *(double *)(lVar15 + lVar19) * dVar8 + *(double *)(lVar17 + lVar21) * dVar9 +
                  *pdVar6;
      } while (lVar18 != param_2 + 1);
      return;
    }
    break;
  case 3:
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = *(long *)(param_4 + 0x10);
    dVar8 = *(double *)(param_3 + 0x40);
    lVar18 = *(long *)(param_4 + 0x18);
    dVar9 = *(double *)(param_3 + 0x48);
    dVar10 = *(double *)(param_3 + 0x50);
    if (0 < param_2) {
      lVar19 = 1;
      do {
        lVar20 = lVar19 * 8;
        lVar21 = lVar19 * 8;
        pdVar6 = (double *)(lVar14 + *(long *)(param_5 + lVar19 * 8) * 8);
        lVar16 = lVar19 * 8;
        lVar19 = lVar19 + 1;
        *pdVar6 = *(double *)(lVar15 + lVar20) * dVar8 + *(double *)(lVar17 + lVar21) * dVar9 +
                  *(double *)(lVar18 + lVar16) * dVar10 + *pdVar6;
      } while (lVar19 != param_2 + 1);
      return;
    }
    break;
  case 4:
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = *(long *)(param_4 + 0x10);
    lVar18 = *(long *)(param_4 + 0x18);
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    lVar19 = *(long *)(param_4 + 0x20);
    dVar10 = *(double *)(param_3 + 0x50);
    dVar11 = *(double *)(param_3 + 0x58);
    if (0 < param_2) {
      lVar20 = 1;
      do {
        lVar21 = lVar20 * 8;
        lVar16 = lVar20 * 8;
        pdVar6 = (double *)(lVar14 + *(long *)(param_5 + lVar20 * 8) * 8);
        lVar1 = lVar20 * 8;
        lVar2 = lVar20 * 8;
        lVar20 = lVar20 + 1;
        *pdVar6 = *(double *)(lVar15 + lVar21) * dVar8 + *(double *)(lVar17 + lVar16) * dVar9 +
                  *(double *)(lVar18 + lVar1) * dVar10 + *(double *)(lVar19 + lVar2) * dVar11 +
                  *pdVar6;
      } while (lVar20 != param_2 + 1);
      return;
    }
    break;
  case 5:
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = *(long *)(param_4 + 0x10);
    lVar18 = *(long *)(param_4 + 0x18);
    lVar19 = *(long *)(param_4 + 0x20);
    dVar8 = *(double *)(param_3 + 0x40);
    lVar20 = *(long *)(param_4 + 0x28);
    dVar9 = *(double *)(param_3 + 0x48);
    dVar10 = *(double *)(param_3 + 0x50);
    dVar11 = *(double *)(param_3 + 0x58);
    dVar12 = *(double *)(param_3 + 0x60);
    if (0 < param_2) {
      lVar21 = 1;
      do {
        lVar16 = lVar21 * 8;
        lVar1 = lVar21 * 8;
        pdVar6 = (double *)(lVar14 + *(long *)(param_5 + lVar21 * 8) * 8);
        lVar2 = lVar21 * 8;
        lVar3 = lVar21 * 8;
        lVar4 = lVar21 * 8;
        lVar21 = lVar21 + 1;
        *pdVar6 = *(double *)(lVar15 + lVar16) * dVar8 + *(double *)(lVar17 + lVar1) * dVar9 +
                  *(double *)(lVar18 + lVar2) * dVar10 + *(double *)(lVar19 + lVar3) * dVar11 +
                  *(double *)(lVar20 + lVar4) * dVar12 + *pdVar6;
      } while (lVar21 != param_2 + 1);
      return;
    }
    break;
  case 6:
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = *(long *)(param_4 + 0x10);
    lVar18 = *(long *)(param_4 + 0x18);
    lVar19 = *(long *)(param_4 + 0x20);
    lVar20 = *(long *)(param_4 + 0x28);
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    lVar21 = *(long *)(param_4 + 0x30);
    dVar10 = *(double *)(param_3 + 0x50);
    dVar11 = *(double *)(param_3 + 0x58);
    dVar12 = *(double *)(param_3 + 0x60);
    dVar13 = *(double *)(param_3 + 0x68);
    if (0 < param_2) {
      lVar16 = 1;
      do {
        lVar1 = lVar16 * 8;
        lVar2 = lVar16 * 8;
        pdVar6 = (double *)(lVar14 + *(long *)(param_5 + lVar16 * 8) * 8);
        lVar3 = lVar16 * 8;
        lVar4 = lVar16 * 8;
        lVar7 = lVar16 * 8;
        lVar5 = lVar16 * 8;
        lVar16 = lVar16 + 1;
        *pdVar6 = *(double *)(lVar15 + lVar1) * dVar8 + *(double *)(lVar17 + lVar2) * dVar9 +
                  *(double *)(lVar18 + lVar3) * dVar10 + *(double *)(lVar19 + lVar4) * dVar11 +
                  *(double *)(lVar20 + lVar7) * dVar12 + *(double *)(lVar21 + lVar5) * dVar13 +
                  *pdVar6;
      } while (lVar16 != param_2 + 1);
    }
  }
  return;
}


