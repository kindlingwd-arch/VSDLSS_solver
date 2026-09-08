/* driver: JRIQFMIBqyUMV2zV6XSPAIPP addr=009af9f0 size=509 */
#include "vsdlss_ref.h"


void JRIQFMIBqyUMV2zV6XSPAIPP
               (undefined8 param_1,long param_2,long param_3,long param_4,long param_5)

{
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  long lVar19;
  long lVar20;
  long lVar21;
  
  lVar13 = *(long *)(param_5 + 8);
  lVar14 = *(long *)(param_5 + 0x10);
  switch(param_1) {
  default:
    KOBOOLOXSPAIPP(0xf,"arithkernels.c",0x133,6,"multipleSaxpy2by6");
    return;
  case 1:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x78);
    lVar17 = 1;
    lVar15 = *(long *)(param_4 + 8);
    if (0 < param_2) {
      do {
        *(double *)(lVar13 + lVar17 * 8) =
             *(double *)(lVar15 + lVar17 * 8) * dVar1 + *(double *)(lVar13 + lVar17 * 8);
        *(double *)(lVar14 + lVar17 * 8) =
             *(double *)(lVar15 + lVar17 * 8) * dVar2 + *(double *)(lVar14 + lVar17 * 8);
        lVar17 = lVar17 + 1;
      } while (lVar17 != param_2 + 1);
      return;
    }
    break;
  case 2:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x78);
    dVar4 = *(double *)(param_3 + 0x80);
    lVar15 = *(long *)(param_4 + 8);
    lVar17 = *(long *)(param_4 + 0x10);
    if (0 < param_2) {
      lVar18 = 1;
      do {
        *(double *)(lVar13 + lVar18 * 8) =
             *(double *)(lVar15 + lVar18 * 8) * dVar1 + *(double *)(lVar17 + lVar18 * 8) * dVar2 +
             *(double *)(lVar13 + lVar18 * 8);
        *(double *)(lVar14 + lVar18 * 8) =
             *(double *)(lVar15 + lVar18 * 8) * dVar3 + *(double *)(lVar17 + lVar18 * 8) * dVar4 +
             *(double *)(lVar14 + lVar18 * 8);
        lVar18 = lVar18 + 1;
      } while (lVar18 != param_2 + 1);
      return;
    }
    break;
  case 3:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar15 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x78);
    dVar5 = *(double *)(param_3 + 0x80);
    dVar6 = *(double *)(param_3 + 0x88);
    lVar17 = *(long *)(param_4 + 8);
    lVar18 = *(long *)(param_4 + 0x18);
    if (0 < param_2) {
      lVar19 = 1;
      do {
        *(double *)(lVar13 + lVar19 * 8) =
             *(double *)(lVar17 + lVar19 * 8) * dVar1 + *(double *)(lVar15 + lVar19 * 8) * dVar2 +
             *(double *)(lVar18 + lVar19 * 8) * dVar3 + *(double *)(lVar13 + lVar19 * 8);
        *(double *)(lVar14 + lVar19 * 8) =
             *(double *)(lVar17 + lVar19 * 8) * dVar4 + *(double *)(lVar15 + lVar19 * 8) * dVar5 +
             *(double *)(lVar18 + lVar19 * 8) * dVar6 + *(double *)(lVar14 + lVar19 * 8);
        lVar19 = lVar19 + 1;
      } while (lVar19 != param_2 + 1);
      return;
    }
    break;
  case 4:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar15 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    lVar17 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x78);
    dVar6 = *(double *)(param_3 + 0x80);
    dVar7 = *(double *)(param_3 + 0x88);
    dVar8 = *(double *)(param_3 + 0x90);
    lVar18 = *(long *)(param_4 + 8);
    lVar19 = *(long *)(param_4 + 0x20);
    if (0 < param_2) {
      lVar20 = 1;
      do {
        *(double *)(lVar13 + lVar20 * 8) =
             *(double *)(lVar18 + lVar20 * 8) * dVar1 + *(double *)(lVar15 + lVar20 * 8) * dVar2 +
             *(double *)(lVar17 + lVar20 * 8) * dVar3 + *(double *)(lVar19 + lVar20 * 8) * dVar4 +
             *(double *)(lVar13 + lVar20 * 8);
        *(double *)(lVar14 + lVar20 * 8) =
             *(double *)(lVar18 + lVar20 * 8) * dVar5 + *(double *)(lVar15 + lVar20 * 8) * dVar6 +
             *(double *)(lVar17 + lVar20 * 8) * dVar7 + *(double *)(lVar19 + lVar20 * 8) * dVar8 +
             *(double *)(lVar14 + lVar20 * 8);
        lVar20 = lVar20 + 1;
      } while (lVar20 != param_2 + 1);
      return;
    }
    break;
  case 5:
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar15 = *(long *)(param_4 + 0x10);
    dVar3 = *(double *)(param_3 + 0x50);
    lVar17 = *(long *)(param_4 + 0x18);
    dVar4 = *(double *)(param_3 + 0x58);
    lVar18 = *(long *)(param_4 + 0x20);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x78);
    dVar7 = *(double *)(param_3 + 0x80);
    dVar8 = *(double *)(param_3 + 0x88);
    dVar9 = *(double *)(param_3 + 0x90);
    dVar10 = *(double *)(param_3 + 0x98);
    lVar19 = *(long *)(param_4 + 8);
    lVar20 = *(long *)(param_4 + 0x28);
    if (0 < param_2) {
      lVar21 = 1;
      do {
        *(double *)(lVar13 + lVar21 * 8) =
             *(double *)(lVar19 + lVar21 * 8) * dVar1 + *(double *)(lVar15 + lVar21 * 8) * dVar2 +
             *(double *)(lVar17 + lVar21 * 8) * dVar3 + *(double *)(lVar18 + lVar21 * 8) * dVar4 +
             *(double *)(lVar20 + lVar21 * 8) * dVar5 + *(double *)(lVar13 + lVar21 * 8);
        *(double *)(lVar14 + lVar21 * 8) =
             *(double *)(lVar19 + lVar21 * 8) * dVar6 + *(double *)(lVar15 + lVar21 * 8) * dVar7 +
             *(double *)(lVar17 + lVar21 * 8) * dVar8 + *(double *)(lVar18 + lVar21 * 8) * dVar9 +
             *(double *)(lVar20 + lVar21 * 8) * dVar10 + *(double *)(lVar14 + lVar21 * 8);
        lVar21 = lVar21 + 1;
      } while (lVar21 != param_2 + 1);
      return;
    }
    break;
  case 6:
    lVar15 = *(long *)(param_4 + 0x10);
    lVar17 = *(long *)(param_4 + 0x18);
    lVar18 = *(long *)(param_4 + 0x20);
    dVar1 = *(double *)(param_3 + 0x40);
    lVar19 = *(long *)(param_4 + 0x28);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x68);
    dVar7 = *(double *)(param_3 + 0x78);
    dVar8 = *(double *)(param_3 + 0x80);
    dVar9 = *(double *)(param_3 + 0x88);
    dVar10 = *(double *)(param_3 + 0x90);
    dVar11 = *(double *)(param_3 + 0x98);
    dVar12 = *(double *)(param_3 + 0xa0);
    lVar20 = *(long *)(param_4 + 8);
    lVar21 = *(long *)(param_4 + 0x30);
    if (0 < param_2) {
      lVar16 = 1;
      do {
        *(double *)(lVar13 + lVar16 * 8) =
             *(double *)(lVar20 + lVar16 * 8) * dVar1 + *(double *)(lVar15 + lVar16 * 8) * dVar2 +
             *(double *)(lVar17 + lVar16 * 8) * dVar3 + *(double *)(lVar18 + lVar16 * 8) * dVar4 +
             *(double *)(lVar19 + lVar16 * 8) * dVar5 + *(double *)(lVar21 + lVar16 * 8) * dVar6 +
             *(double *)(lVar13 + lVar16 * 8);
        *(double *)(lVar14 + lVar16 * 8) =
             *(double *)(lVar20 + lVar16 * 8) * dVar7 + *(double *)(lVar15 + lVar16 * 8) * dVar8 +
             *(double *)(lVar17 + lVar16 * 8) * dVar9 + *(double *)(lVar18 + lVar16 * 8) * dVar10 +
             *(double *)(lVar19 + lVar16 * 8) * dVar11 + *(double *)(lVar21 + lVar16 * 8) * dVar12 +
             *(double *)(lVar14 + lVar16 * 8);
        lVar16 = lVar16 + 1;
      } while (lVar16 != param_2 + 1);
    }
  }
  return;
}


