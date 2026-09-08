/* driver: JRIQFMIBqMyOPBbBKPBqyUMV2zV6XSPAIPP addr=009b0a20 size=57a */
#include "vsdlss_ref.h"


void JRIQFMIBqMyOPBbBKPBqyUMV2zV6XSPAIPP
               (undefined8 param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  double *pdVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  long lVar24;
  long lVar25;
  long lVar26;
  long lVar27;
  long lVar28;
  double *pdVar29;
  
  lVar20 = *(long *)(param_6 + 8);
  lVar21 = *(long *)(param_6 + 0x10);
  switch(param_1) {
  default:
    KOBOOLOXSPAIPP(0xf,"arithkernels.c",0x33a,6,"multipleSparseDenseSaxpy2by6");
    return;
  case 1:
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x78);
    lVar22 = *(long *)(param_4 + 8);
    lVar24 = 1;
    if (0 < param_2) {
      do {
        lVar25 = *(long *)(param_5 + lVar24 * 8) * 8;
        pdVar4 = (double *)(lVar20 + lVar25);
        pdVar29 = (double *)(lVar25 + lVar21);
        *pdVar4 = *(double *)(lVar22 + lVar24 * 8) * dVar8 + *pdVar4;
        lVar25 = lVar24 * 8;
        lVar24 = lVar24 + 1;
        *pdVar29 = *(double *)(lVar22 + lVar25) * dVar9 + *pdVar29;
      } while (lVar24 != param_2 + 1);
      return;
    }
    break;
  case 2:
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 8);
    dVar10 = *(double *)(param_3 + 0x78);
    lVar24 = *(long *)(param_4 + 0x10);
    dVar11 = *(double *)(param_3 + 0x80);
    if (0 < param_2) {
      lVar25 = 1;
      do {
        lVar26 = *(long *)(param_5 + lVar25 * 8) * 8;
        pdVar4 = (double *)(lVar20 + lVar26);
        pdVar29 = (double *)(lVar26 + lVar21);
        *pdVar4 = *(double *)(lVar22 + lVar25 * 8) * dVar8 +
                  *(double *)(lVar24 + lVar25 * 8) * dVar9 + *pdVar4;
        lVar26 = lVar25 * 8;
        lVar27 = lVar25 * 8;
        lVar25 = lVar25 + 1;
        *pdVar29 = *(double *)(lVar22 + lVar26) * dVar10 + *(double *)(lVar24 + lVar27) * dVar11 +
                   *pdVar29;
      } while (lVar25 != param_2 + 1);
      return;
    }
    break;
  case 3:
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 8);
    dVar10 = *(double *)(param_3 + 0x50);
    lVar24 = *(long *)(param_4 + 0x10);
    dVar11 = *(double *)(param_3 + 0x78);
    lVar25 = *(long *)(param_4 + 0x18);
    dVar12 = *(double *)(param_3 + 0x80);
    dVar13 = *(double *)(param_3 + 0x88);
    if (0 < param_2) {
      lVar26 = 1;
      do {
        lVar27 = *(long *)(param_5 + lVar26 * 8) * 8;
        pdVar4 = (double *)(lVar20 + lVar27);
        pdVar29 = (double *)(lVar27 + lVar21);
        *pdVar4 = *(double *)(lVar22 + lVar26 * 8) * dVar8 +
                  *(double *)(lVar24 + lVar26 * 8) * dVar9 +
                  *(double *)(lVar25 + lVar26 * 8) * dVar10 + *pdVar4;
        lVar27 = lVar26 * 8;
        lVar28 = lVar26 * 8;
        lVar23 = lVar26 * 8;
        lVar26 = lVar26 + 1;
        *pdVar29 = *(double *)(lVar22 + lVar27) * dVar11 + *(double *)(lVar24 + lVar28) * dVar12 +
                   *(double *)(lVar25 + lVar23) * dVar13 + *pdVar29;
      } while (lVar26 != param_2 + 1);
      return;
    }
    break;
  case 4:
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 8);
    dVar10 = *(double *)(param_3 + 0x50);
    lVar24 = *(long *)(param_4 + 0x10);
    dVar11 = *(double *)(param_3 + 0x58);
    lVar25 = *(long *)(param_4 + 0x18);
    dVar12 = *(double *)(param_3 + 0x78);
    lVar26 = *(long *)(param_4 + 0x20);
    dVar13 = *(double *)(param_3 + 0x80);
    dVar14 = *(double *)(param_3 + 0x88);
    dVar15 = *(double *)(param_3 + 0x90);
    if (0 < param_2) {
      lVar27 = 1;
      do {
        lVar28 = *(long *)(param_5 + lVar27 * 8) * 8;
        pdVar4 = (double *)(lVar20 + lVar28);
        pdVar29 = (double *)(lVar28 + lVar21);
        *pdVar4 = *(double *)(lVar22 + lVar27 * 8) * dVar8 +
                  *(double *)(lVar24 + lVar27 * 8) * dVar9 +
                  *(double *)(lVar25 + lVar27 * 8) * dVar10 +
                  *(double *)(lVar26 + lVar27 * 8) * dVar11 + *pdVar4;
        lVar28 = lVar27 * 8;
        lVar23 = lVar27 * 8;
        lVar5 = lVar27 * 8;
        lVar1 = lVar27 * 8;
        lVar27 = lVar27 + 1;
        *pdVar29 = *(double *)(lVar22 + lVar28) * dVar12 + *(double *)(lVar24 + lVar23) * dVar13 +
                   *(double *)(lVar25 + lVar5) * dVar14 + *(double *)(lVar26 + lVar1) * dVar15 +
                   *pdVar29;
      } while (lVar27 != param_2 + 1);
      return;
    }
    break;
  case 5:
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 8);
    dVar10 = *(double *)(param_3 + 0x50);
    lVar24 = *(long *)(param_4 + 0x10);
    dVar11 = *(double *)(param_3 + 0x58);
    lVar25 = *(long *)(param_4 + 0x18);
    dVar12 = *(double *)(param_3 + 0x60);
    lVar26 = *(long *)(param_4 + 0x20);
    dVar13 = *(double *)(param_3 + 0x78);
    lVar27 = *(long *)(param_4 + 0x28);
    dVar14 = *(double *)(param_3 + 0x80);
    dVar15 = *(double *)(param_3 + 0x88);
    dVar16 = *(double *)(param_3 + 0x90);
    dVar17 = *(double *)(param_3 + 0x98);
    if (0 < param_2) {
      lVar28 = 1;
      do {
        lVar23 = *(long *)(param_5 + lVar28 * 8) * 8;
        pdVar4 = (double *)(lVar20 + lVar23);
        pdVar29 = (double *)(lVar23 + lVar21);
        *pdVar4 = *(double *)(lVar22 + lVar28 * 8) * dVar8 +
                  *(double *)(lVar24 + lVar28 * 8) * dVar9 +
                  *(double *)(lVar25 + lVar28 * 8) * dVar10 +
                  *(double *)(lVar26 + lVar28 * 8) * dVar11 +
                  *(double *)(lVar27 + lVar28 * 8) * dVar12 + *pdVar4;
        lVar23 = lVar28 * 8;
        lVar5 = lVar28 * 8;
        lVar1 = lVar28 * 8;
        lVar3 = lVar28 * 8;
        lVar2 = lVar28 * 8;
        lVar28 = lVar28 + 1;
        *pdVar29 = *(double *)(lVar22 + lVar23) * dVar13 + *(double *)(lVar24 + lVar5) * dVar14 +
                   *(double *)(lVar25 + lVar1) * dVar15 + *(double *)(lVar26 + lVar3) * dVar16 +
                   *(double *)(lVar27 + lVar2) * dVar17 + *pdVar29;
      } while (lVar28 != param_2 + 1);
      return;
    }
    break;
  case 6:
    dVar8 = *(double *)(param_3 + 0x40);
    dVar9 = *(double *)(param_3 + 0x48);
    lVar22 = *(long *)(param_4 + 8);
    dVar10 = *(double *)(param_3 + 0x50);
    lVar24 = *(long *)(param_4 + 0x10);
    dVar11 = *(double *)(param_3 + 0x58);
    lVar25 = *(long *)(param_4 + 0x18);
    dVar12 = *(double *)(param_3 + 0x60);
    lVar26 = *(long *)(param_4 + 0x20);
    dVar13 = *(double *)(param_3 + 0x68);
    lVar27 = *(long *)(param_4 + 0x28);
    dVar14 = *(double *)(param_3 + 0x78);
    lVar28 = *(long *)(param_4 + 0x30);
    dVar15 = *(double *)(param_3 + 0x80);
    dVar16 = *(double *)(param_3 + 0x88);
    dVar17 = *(double *)(param_3 + 0x90);
    dVar18 = *(double *)(param_3 + 0x98);
    dVar19 = *(double *)(param_3 + 0xa0);
    if (0 < param_2) {
      lVar23 = 1;
      do {
        lVar5 = *(long *)(param_5 + lVar23 * 8) * 8;
        pdVar4 = (double *)(lVar20 + lVar5);
        pdVar29 = (double *)(lVar5 + lVar21);
        *pdVar4 = *(double *)(lVar22 + lVar23 * 8) * dVar8 +
                  *(double *)(lVar24 + lVar23 * 8) * dVar9 +
                  *(double *)(lVar25 + lVar23 * 8) * dVar10 +
                  *(double *)(lVar26 + lVar23 * 8) * dVar11 +
                  *(double *)(lVar27 + lVar23 * 8) * dVar12 +
                  *(double *)(lVar28 + lVar23 * 8) * dVar13 + *pdVar4;
        lVar5 = lVar23 * 8;
        lVar1 = lVar23 * 8;
        lVar2 = lVar23 * 8;
        lVar6 = lVar23 * 8;
        lVar3 = lVar23 * 8;
        lVar7 = lVar23 * 8;
        lVar23 = lVar23 + 1;
        *pdVar29 = *(double *)(lVar22 + lVar5) * dVar14 + *(double *)(lVar24 + lVar1) * dVar15 +
                   *(double *)(lVar25 + lVar2) * dVar16 + *(double *)(lVar26 + lVar6) * dVar17 +
                   *(double *)(lVar27 + lVar3) * dVar18 + *(double *)(lVar28 + lVar7) * dVar19 +
                   *pdVar29;
      } while (lVar23 != param_2 + 1);
    }
  }
  return;
}


