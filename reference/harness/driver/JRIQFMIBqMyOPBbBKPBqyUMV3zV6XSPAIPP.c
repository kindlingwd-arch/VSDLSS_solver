/* driver: JRIQFMIBqMyOPBbBKPBqyUMV3zV6XSPAIPP addr=009b0fc0 size=801 */
#include "vsdlss_ref.h"


void JRIQFMIBqMyOPBbBKPBqyUMV3zV6XSPAIPP
               (undefined8 param_1,long param_2,long param_3,long param_4,long param_5,long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar7;
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
  double dVar20;
  double dVar21;
  long lVar22;
  long lVar23;
  long lVar24;
  long lVar25;
  long lVar26;
  long lVar27;
  long lVar28;
  long lVar29;
  long lVar30;
  long lVar31;
  long lVar32;
  long lVar33;
  long lVar34;
  long lVar35;
  
  lVar22 = *(long *)(param_6 + 8);
  lVar23 = *(long *)(param_6 + 0x10);
  lVar24 = *(long *)(param_6 + 0x18);
  switch(param_1) {
  default:
    KOBOOLOXSPAIPP(0xf,"arithkernels.c",0x40a,6,"multipleSparseDenseSaxpy3by6");
    return;
  case 1:
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x78);
    lVar25 = *(long *)(param_4 + 8);
    dVar6 = *(double *)(param_3 + 0xb0);
    lVar27 = 1;
    if (0 < param_2) {
      do {
        lVar29 = *(long *)(param_5 + lVar27 * 8) * 8;
        *(double *)(lVar22 + lVar29) =
             *(double *)(lVar25 + lVar27 * 8) * dVar4 + *(double *)(lVar22 + lVar29);
        *(double *)(lVar23 + lVar29) =
             *(double *)(lVar25 + lVar27 * 8) * dVar5 + *(double *)(lVar23 + lVar29);
        lVar28 = lVar27 * 8;
        lVar27 = lVar27 + 1;
        *(double *)(lVar29 + lVar24) =
             *(double *)(lVar25 + lVar28) * dVar6 + *(double *)(lVar29 + lVar24);
      } while (lVar27 != param_2 + 1);
      return;
    }
    break;
  case 2:
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x48);
    dVar6 = *(double *)(param_3 + 0x78);
    dVar7 = *(double *)(param_3 + 0x80);
    dVar8 = *(double *)(param_3 + 0xb0);
    dVar9 = *(double *)(param_3 + 0xb8);
    if (0 < param_2) {
      lVar28 = 1;
      do {
        lVar31 = *(long *)(param_5 + lVar28 * 8) * 8;
        *(double *)(lVar22 + lVar31) =
             *(double *)(lVar25 + lVar28 * 8) * dVar4 + *(double *)(lVar27 + lVar28 * 8) * dVar5 +
             *(double *)(lVar22 + lVar31);
        *(double *)(lVar23 + lVar31) =
             *(double *)(lVar25 + lVar28 * 8) * dVar6 + *(double *)(lVar27 + lVar28 * 8) * dVar7 +
             *(double *)(lVar23 + lVar31);
        lVar29 = lVar28 * 8;
        lVar30 = lVar28 * 8;
        lVar28 = lVar28 + 1;
        *(double *)(lVar31 + lVar24) =
             *(double *)(lVar25 + lVar29) * dVar8 + *(double *)(lVar27 + lVar30) * dVar9 +
             *(double *)(lVar31 + lVar24);
      } while (lVar28 != param_2 + 1);
      return;
    }
    break;
  case 3:
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    dVar4 = *(double *)(param_3 + 0x40);
    lVar28 = *(long *)(param_4 + 0x18);
    dVar5 = *(double *)(param_3 + 0x48);
    dVar6 = *(double *)(param_3 + 0x50);
    dVar7 = *(double *)(param_3 + 0x78);
    dVar8 = *(double *)(param_3 + 0x80);
    dVar9 = *(double *)(param_3 + 0x88);
    dVar10 = *(double *)(param_3 + 0xb0);
    dVar11 = *(double *)(param_3 + 0xb8);
    dVar12 = *(double *)(param_3 + 0xc0);
    if (0 < param_2) {
      lVar29 = 1;
      do {
        lVar33 = *(long *)(param_5 + lVar29 * 8) * 8;
        *(double *)(lVar22 + lVar33) =
             *(double *)(lVar25 + lVar29 * 8) * dVar4 + *(double *)(lVar27 + lVar29 * 8) * dVar5 +
             *(double *)(lVar28 + lVar29 * 8) * dVar6 + *(double *)(lVar22 + lVar33);
        *(double *)(lVar23 + lVar33) =
             *(double *)(lVar25 + lVar29 * 8) * dVar7 + *(double *)(lVar27 + lVar29 * 8) * dVar8 +
             *(double *)(lVar28 + lVar29 * 8) * dVar9 + *(double *)(lVar23 + lVar33);
        lVar30 = lVar29 * 8;
        lVar31 = lVar29 * 8;
        lVar26 = lVar29 * 8;
        lVar29 = lVar29 + 1;
        *(double *)(lVar33 + lVar24) =
             *(double *)(lVar25 + lVar30) * dVar10 + *(double *)(lVar27 + lVar31) * dVar11 +
             *(double *)(lVar28 + lVar26) * dVar12 + *(double *)(lVar33 + lVar24);
      } while (lVar29 != param_2 + 1);
      return;
    }
    break;
  case 4:
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    lVar28 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x48);
    lVar29 = *(long *)(param_4 + 0x20);
    dVar6 = *(double *)(param_3 + 0x50);
    dVar7 = *(double *)(param_3 + 0x58);
    dVar8 = *(double *)(param_3 + 0x78);
    dVar9 = *(double *)(param_3 + 0x80);
    dVar10 = *(double *)(param_3 + 0x88);
    dVar11 = *(double *)(param_3 + 0x90);
    dVar12 = *(double *)(param_3 + 0xb0);
    dVar13 = *(double *)(param_3 + 0xb8);
    dVar14 = *(double *)(param_3 + 0xc0);
    dVar15 = *(double *)(param_3 + 200);
    if (0 < param_2) {
      lVar30 = 1;
      do {
        lVar34 = *(long *)(param_5 + lVar30 * 8) * 8;
        *(double *)(lVar22 + lVar34) =
             *(double *)(lVar25 + lVar30 * 8) * dVar4 + *(double *)(lVar27 + lVar30 * 8) * dVar5 +
             *(double *)(lVar28 + lVar30 * 8) * dVar6 + *(double *)(lVar29 + lVar30 * 8) * dVar7 +
             *(double *)(lVar22 + lVar34);
        *(double *)(lVar23 + lVar34) =
             *(double *)(lVar25 + lVar30 * 8) * dVar8 + *(double *)(lVar27 + lVar30 * 8) * dVar9 +
             *(double *)(lVar28 + lVar30 * 8) * dVar10 + *(double *)(lVar29 + lVar30 * 8) * dVar11 +
             *(double *)(lVar23 + lVar34);
        lVar31 = lVar30 * 8;
        lVar26 = lVar30 * 8;
        lVar1 = lVar30 * 8;
        lVar33 = lVar30 * 8;
        lVar30 = lVar30 + 1;
        *(double *)(lVar34 + lVar24) =
             *(double *)(lVar25 + lVar31) * dVar12 + *(double *)(lVar27 + lVar26) * dVar13 +
             *(double *)(lVar28 + lVar1) * dVar14 + *(double *)(lVar29 + lVar33) * dVar15 +
             *(double *)(lVar34 + lVar24);
      } while (lVar30 != param_2 + 1);
      return;
    }
    break;
  case 5:
    dVar4 = *(double *)(param_3 + 0x40);
    lVar25 = *(long *)(param_4 + 8);
    lVar27 = *(long *)(param_4 + 0x10);
    lVar28 = *(long *)(param_4 + 0x18);
    lVar29 = *(long *)(param_4 + 0x20);
    dVar5 = *(double *)(param_3 + 0x48);
    lVar30 = *(long *)(param_4 + 0x28);
    dVar6 = *(double *)(param_3 + 0x50);
    dVar7 = *(double *)(param_3 + 0x58);
    dVar8 = *(double *)(param_3 + 0x60);
    dVar9 = *(double *)(param_3 + 0x78);
    dVar10 = *(double *)(param_3 + 0x80);
    dVar11 = *(double *)(param_3 + 0x88);
    dVar12 = *(double *)(param_3 + 0x90);
    dVar13 = *(double *)(param_3 + 0x98);
    dVar14 = *(double *)(param_3 + 0xb0);
    dVar15 = *(double *)(param_3 + 0xb8);
    dVar16 = *(double *)(param_3 + 0xc0);
    dVar17 = *(double *)(param_3 + 200);
    dVar18 = *(double *)(param_3 + 0xd0);
    if (0 < param_2) {
      lVar31 = 1;
      do {
        lVar35 = *(long *)(param_5 + lVar31 * 8) * 8;
        *(double *)(lVar22 + lVar35) =
             dVar4 * *(double *)(lVar25 + lVar31 * 8) + *(double *)(lVar27 + lVar31 * 8) * dVar5 +
             *(double *)(lVar28 + lVar31 * 8) * dVar6 + *(double *)(lVar29 + lVar31 * 8) * dVar7 +
             *(double *)(lVar30 + lVar31 * 8) * dVar8 + *(double *)(lVar22 + lVar35);
        *(double *)(lVar23 + lVar35) =
             *(double *)(lVar25 + lVar31 * 8) * dVar9 + *(double *)(lVar27 + lVar31 * 8) * dVar10 +
             *(double *)(lVar28 + lVar31 * 8) * dVar11 + *(double *)(lVar29 + lVar31 * 8) * dVar12 +
             *(double *)(lVar30 + lVar31 * 8) * dVar13 + *(double *)(lVar23 + lVar35);
        lVar26 = lVar31 * 8;
        lVar33 = lVar31 * 8;
        lVar2 = lVar31 * 8;
        lVar1 = lVar31 * 8;
        lVar34 = lVar31 * 8;
        lVar31 = lVar31 + 1;
        *(double *)(lVar35 + lVar24) =
             *(double *)(lVar25 + lVar26) * dVar14 + *(double *)(lVar27 + lVar33) * dVar15 +
             *(double *)(lVar28 + lVar2) * dVar16 + *(double *)(lVar29 + lVar1) * dVar17 +
             *(double *)(lVar30 + lVar34) * dVar18 + *(double *)(lVar35 + lVar24);
      } while (lVar31 != param_2 + 1);
      return;
    }
    break;
  case 6:
    dVar4 = *(double *)(param_3 + 0x40);
    dVar5 = *(double *)(param_3 + 0x48);
    lVar25 = *(long *)(param_4 + 8);
    dVar6 = *(double *)(param_3 + 0x50);
    lVar27 = *(long *)(param_4 + 0x10);
    dVar7 = *(double *)(param_3 + 0x58);
    lVar28 = *(long *)(param_4 + 0x18);
    lVar29 = *(long *)(param_4 + 0x20);
    lVar30 = *(long *)(param_4 + 0x28);
    lVar31 = *(long *)(param_4 + 0x30);
    dVar8 = *(double *)(param_3 + 0x60);
    dVar9 = *(double *)(param_3 + 0x68);
    dVar10 = *(double *)(param_3 + 0x78);
    dVar11 = *(double *)(param_3 + 0x80);
    dVar12 = *(double *)(param_3 + 0x88);
    dVar13 = *(double *)(param_3 + 0x90);
    dVar14 = *(double *)(param_3 + 0x98);
    dVar15 = *(double *)(param_3 + 0xa0);
    dVar16 = *(double *)(param_3 + 0xb0);
    dVar17 = *(double *)(param_3 + 0xb8);
    dVar18 = *(double *)(param_3 + 0xc0);
    dVar19 = *(double *)(param_3 + 200);
    dVar20 = *(double *)(param_3 + 0xd0);
    dVar21 = *(double *)(param_3 + 0xd8);
    if (0 < param_2) {
      lVar26 = 1;
      do {
        lVar32 = *(long *)(param_5 + lVar26 * 8) * 8;
        *(double *)(lVar22 + lVar32) =
             dVar4 * *(double *)(lVar25 + lVar26 * 8) + dVar5 * *(double *)(lVar27 + lVar26 * 8) +
             dVar6 * *(double *)(lVar28 + lVar26 * 8) + dVar7 * *(double *)(lVar29 + lVar26 * 8) +
             *(double *)(lVar30 + lVar26 * 8) * dVar8 + *(double *)(lVar31 + lVar26 * 8) * dVar9 +
             *(double *)(lVar22 + lVar32);
        *(double *)(lVar23 + lVar32) =
             *(double *)(lVar25 + lVar26 * 8) * dVar10 + *(double *)(lVar27 + lVar26 * 8) * dVar11 +
             *(double *)(lVar28 + lVar26 * 8) * dVar12 + *(double *)(lVar29 + lVar26 * 8) * dVar13 +
             *(double *)(lVar30 + lVar26 * 8) * dVar14 + *(double *)(lVar31 + lVar26 * 8) * dVar15 +
             *(double *)(lVar23 + lVar32);
        lVar33 = lVar26 * 8;
        lVar1 = lVar26 * 8;
        lVar35 = lVar26 * 8;
        lVar34 = lVar26 * 8;
        lVar3 = lVar26 * 8;
        lVar2 = lVar26 * 8;
        lVar26 = lVar26 + 1;
        *(double *)(lVar32 + lVar24) =
             *(double *)(lVar25 + lVar33) * dVar16 + *(double *)(lVar27 + lVar1) * dVar17 +
             *(double *)(lVar28 + lVar35) * dVar18 + *(double *)(lVar29 + lVar34) * dVar19 +
             *(double *)(lVar30 + lVar3) * dVar20 + *(double *)(lVar31 + lVar2) * dVar21 +
             *(double *)(lVar32 + lVar24);
      } while (lVar26 != param_2 + 1);
    }
  }
  return;
}


