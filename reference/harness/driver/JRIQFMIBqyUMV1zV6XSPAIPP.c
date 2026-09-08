/* driver: JRIQFMIBqyUMV1zV6XSPAIPP addr=009af6c0 size=2f1 */
#include "vsdlss_ref.h"


void JRIQFMIBqyUMV1zV6XSPAIPP
               (undefined8 param_1,long param_2,long param_3,long param_4,long param_5)

{
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  
  lVar7 = *(long *)(param_5 + 8);
  switch(param_1) {
  default:
    KOBOOLOXSPAIPP(0xf,"arithkernels.c",0x89,6,"multipleSaxpy1by6");
    return;
  case 1:
    dVar1 = *(double *)(param_3 + 0x40);
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = 1;
    if (0 < param_2) {
      do {
        *(double *)(lVar7 + lVar10 * 8) =
             *(double *)(lVar8 + lVar10 * 8) * dVar1 + *(double *)(lVar7 + lVar10 * 8);
        lVar10 = lVar10 + 1;
      } while (lVar10 != param_2 + 1);
      return;
    }
    break;
  case 2:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    if (0 < param_2) {
      lVar11 = 1;
      do {
        *(double *)(lVar7 + lVar11 * 8) =
             *(double *)(lVar8 + lVar11 * 8) * dVar1 + *(double *)(lVar10 + lVar11 * 8) * dVar2 +
             *(double *)(lVar7 + lVar11 * 8);
        lVar11 = lVar11 + 1;
      } while (lVar11 != param_2 + 1);
      return;
    }
    break;
  case 3:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    dVar1 = *(double *)(param_3 + 0x40);
    lVar11 = *(long *)(param_4 + 0x18);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x50);
    if (0 < param_2) {
      lVar13 = 1;
      do {
        *(double *)(lVar7 + lVar13 * 8) =
             *(double *)(lVar8 + lVar13 * 8) * dVar1 + *(double *)(lVar10 + lVar13 * 8) * dVar2 +
             *(double *)(lVar11 + lVar13 * 8) * dVar3 + *(double *)(lVar7 + lVar13 * 8);
        lVar13 = lVar13 + 1;
      } while (lVar13 != param_2 + 1);
      return;
    }
    break;
  case 4:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    lVar11 = *(long *)(param_4 + 0x18);
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar13 = *(long *)(param_4 + 0x20);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    if (0 < param_2) {
      lVar12 = 1;
      do {
        *(double *)(lVar7 + lVar12 * 8) =
             *(double *)(lVar8 + lVar12 * 8) * dVar1 + *(double *)(lVar10 + lVar12 * 8) * dVar2 +
             *(double *)(lVar11 + lVar12 * 8) * dVar3 + *(double *)(lVar13 + lVar12 * 8) * dVar4 +
             *(double *)(lVar7 + lVar12 * 8);
        lVar12 = lVar12 + 1;
      } while (lVar12 != param_2 + 1);
      return;
    }
    break;
  case 5:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    lVar11 = *(long *)(param_4 + 0x18);
    lVar13 = *(long *)(param_4 + 0x20);
    dVar1 = *(double *)(param_3 + 0x40);
    lVar12 = *(long *)(param_4 + 0x28);
    dVar2 = *(double *)(param_3 + 0x48);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x60);
    if (0 < param_2) {
      lVar14 = 1;
      do {
        *(double *)(lVar7 + lVar14 * 8) =
             *(double *)(lVar8 + lVar14 * 8) * dVar1 + *(double *)(lVar10 + lVar14 * 8) * dVar2 +
             *(double *)(lVar11 + lVar14 * 8) * dVar3 + *(double *)(lVar13 + lVar14 * 8) * dVar4 +
             *(double *)(lVar12 + lVar14 * 8) * dVar5 + *(double *)(lVar7 + lVar14 * 8);
        lVar14 = lVar14 + 1;
      } while (lVar14 != param_2 + 1);
      return;
    }
    break;
  case 6:
    lVar8 = *(long *)(param_4 + 8);
    lVar10 = *(long *)(param_4 + 0x10);
    lVar11 = *(long *)(param_4 + 0x18);
    lVar13 = *(long *)(param_4 + 0x20);
    lVar12 = *(long *)(param_4 + 0x28);
    dVar1 = *(double *)(param_3 + 0x40);
    dVar2 = *(double *)(param_3 + 0x48);
    lVar14 = *(long *)(param_4 + 0x30);
    dVar3 = *(double *)(param_3 + 0x50);
    dVar4 = *(double *)(param_3 + 0x58);
    dVar5 = *(double *)(param_3 + 0x60);
    dVar6 = *(double *)(param_3 + 0x68);
    if (0 < param_2) {
      lVar9 = 1;
      do {
        *(double *)(lVar7 + lVar9 * 8) =
             *(double *)(lVar8 + lVar9 * 8) * dVar1 + *(double *)(lVar10 + lVar9 * 8) * dVar2 +
             *(double *)(lVar11 + lVar9 * 8) * dVar3 + *(double *)(lVar13 + lVar9 * 8) * dVar4 +
             *(double *)(lVar12 + lVar9 * 8) * dVar5 + *(double *)(lVar14 + lVar9 * 8) * dVar6 +
             *(double *)(lVar7 + lVar9 * 8);
        lVar9 = lVar9 + 1;
      } while (lVar9 != param_2 + 1);
      return;
    }
  }
  return;
}


