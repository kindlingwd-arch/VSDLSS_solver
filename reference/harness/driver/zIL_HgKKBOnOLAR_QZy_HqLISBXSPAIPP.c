/* driver: zIL_HgKKBOnOLAR_QZy_HqLISBXSPAIPP addr=009ae520 size=6f7 */
#include "vsdlss_ref.h"


void zIL_HgKKBOnOLAR_QZy_HqLISBXSPAIPP
               (long param_1,undefined8 param_2,long param_3,long param_4,long param_5,long param_6)

{
  double *pdVar1;
  long lVar2;
  long lVar3;
  double *pdVar4;
  double *pdVar5;
  double *pdVar6;
  double *pdVar7;
  double dVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  double *pdVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  
  switch(param_2) {
  default:
    KOBOOLOXSPAIPP(0xf,"triangularsolves.c",0x2c7,6,"blockInnerProductBackSolve");
    return;
  case 1:
    dVar17 = 0.0;
    if (1 < param_5) {
      dVar17 = 0.0;
      lVar10 = 1;
      do {
        lVar11 = lVar10 * 8;
        lVar13 = lVar10 * 8;
        lVar10 = lVar10 + 1;
        dVar17 = dVar17 + *(double *)(*(long *)(param_4 + 8) + 8 + lVar13) *
                          *(double *)(param_6 + *(long *)(param_3 + 8 + lVar11) * 8);
      } while (lVar10 != param_5);
    }
    pdVar1 = (double *)(param_6 + param_1 * 8);
    *pdVar1 = (*pdVar1 - dVar17) / *(double *)(*(long *)(param_4 + 8) + 8);
    return;
  case 2:
    lVar10 = *(long *)(param_4 + 8);
    if (param_5 < 3) {
      dVar18 = 0.0;
      dVar17 = dVar18;
    }
    else {
      dVar18 = 0.0;
      lVar11 = 1;
      dVar17 = 0.0;
      do {
        lVar13 = lVar11 * 8;
        dVar19 = *(double *)(param_6 + *(long *)(param_3 + 0x10 + lVar11 * 8) * 8);
        lVar12 = lVar11 * 8;
        lVar11 = lVar11 + 1;
        dVar17 = dVar17 + *(double *)(lVar10 + 0x10 + lVar13) * dVar19;
        dVar18 = dVar18 + dVar19 * *(double *)(*(long *)(param_4 + 0x10) + 8 + lVar12);
      } while (lVar11 != param_5 + -1);
    }
    pdVar1 = (double *)(param_6 + param_1 * 8);
    dVar18 = (*pdVar1 - dVar18) / *(double *)(*(long *)(param_4 + 0x10) + 8);
    *pdVar1 = dVar18;
    pdVar1 = (double *)(param_6 + -8 + param_1 * 8);
    *pdVar1 = ((*pdVar1 - dVar17) - dVar18 * *(double *)(lVar10 + 0x10)) / *(double *)(lVar10 + 8);
    return;
  case 3:
    lVar10 = *(long *)(param_4 + 8);
    lVar11 = *(long *)(param_4 + 0x10);
    if (param_5 < 4) {
      dVar19 = 0.0;
      dVar17 = dVar19;
      dVar18 = dVar19;
    }
    else {
      dVar19 = 0.0;
      lVar13 = 1;
      dVar18 = 0.0;
      dVar17 = 0.0;
      do {
        dVar20 = *(double *)(param_6 + *(long *)(param_3 + 0x18 + lVar13 * 8) * 8);
        dVar17 = dVar17 + *(double *)(lVar10 + 0x18 + lVar13 * 8) * dVar20;
        lVar12 = lVar13 * 8;
        lVar14 = lVar13 * 8;
        lVar13 = lVar13 + 1;
        dVar18 = dVar18 + *(double *)(lVar11 + 0x10 + lVar12) * dVar20;
        dVar19 = dVar19 + dVar20 * *(double *)(*(long *)(param_4 + 0x18) + 8 + lVar14);
      } while (lVar13 != param_5 + -2);
    }
    param_1 = param_1 * 8;
    pdVar1 = (double *)(param_6 + param_1);
    pdVar4 = (double *)(param_6 + -8 + param_1);
    dVar19 = (*pdVar1 - dVar19) / *(double *)(*(long *)(param_4 + 0x18) + 8);
    *pdVar1 = dVar19;
    dVar18 = ((*pdVar4 - dVar18) - dVar19 * *(double *)(lVar11 + 0x10)) / *(double *)(lVar11 + 8);
    *pdVar4 = dVar18;
    pdVar4 = (double *)(param_6 + -0x10 + param_1);
    *pdVar4 = (((*pdVar4 - dVar17) - *pdVar1 * *(double *)(lVar10 + 0x18)) -
              dVar18 * *(double *)(lVar10 + 0x10)) / *(double *)(lVar10 + 8);
    return;
  case 4:
    lVar10 = *(long *)(param_4 + 8);
    lVar11 = *(long *)(param_4 + 0x10);
    lVar13 = *(long *)(param_4 + 0x18);
    if (param_5 < 5) {
      dVar20 = 0.0;
      dVar17 = dVar20;
      dVar18 = dVar20;
      dVar19 = dVar20;
    }
    else {
      dVar20 = 0.0;
      lVar12 = 1;
      dVar19 = 0.0;
      dVar18 = 0.0;
      dVar17 = 0.0;
      do {
        dVar21 = *(double *)(param_6 + *(long *)(param_3 + 0x20 + lVar12 * 8) * 8);
        dVar17 = dVar17 + *(double *)(lVar10 + 0x20 + lVar12 * 8) * dVar21;
        dVar18 = dVar18 + *(double *)(lVar11 + 0x18 + lVar12 * 8) * dVar21;
        lVar14 = lVar12 * 8;
        lVar9 = lVar12 * 8;
        lVar12 = lVar12 + 1;
        dVar19 = dVar19 + *(double *)(lVar13 + 0x10 + lVar14) * dVar21;
        dVar20 = dVar20 + dVar21 * *(double *)(*(long *)(param_4 + 0x20) + 8 + lVar9);
      } while (lVar12 != param_5 + -3);
    }
    param_1 = param_1 * 8;
    pdVar1 = (double *)(param_6 + param_1);
    pdVar4 = (double *)(param_6 + -8 + param_1);
    pdVar5 = (double *)(param_6 + -0x10 + param_1);
    dVar20 = (*pdVar1 - dVar20) / *(double *)(*(long *)(param_4 + 0x20) + 8);
    *pdVar1 = dVar20;
    dVar19 = ((*pdVar4 - dVar19) - dVar20 * *(double *)(lVar13 + 0x10)) / *(double *)(lVar13 + 8);
    *pdVar4 = dVar19;
    dVar18 = (((*pdVar5 - dVar18) - *pdVar1 * *(double *)(lVar11 + 0x18)) -
             dVar19 * *(double *)(lVar11 + 0x10)) / *(double *)(lVar11 + 8);
    *pdVar5 = dVar18;
    pdVar5 = (double *)(param_6 + -0x18 + param_1);
    *pdVar5 = ((((*pdVar5 - dVar17) - *pdVar1 * *(double *)(lVar10 + 0x20)) -
               *pdVar4 * *(double *)(lVar10 + 0x18)) - dVar18 * *(double *)(lVar10 + 0x10)) /
              *(double *)(lVar10 + 8);
    return;
  case 5:
    lVar10 = *(long *)(param_4 + 8);
    lVar11 = *(long *)(param_4 + 0x10);
    lVar13 = *(long *)(param_4 + 0x18);
    lVar12 = *(long *)(param_4 + 0x20);
    if (param_5 < 6) {
      dVar21 = 0.0;
      dVar17 = dVar21;
      dVar18 = dVar21;
      dVar19 = dVar21;
      dVar20 = dVar21;
    }
    else {
      dVar21 = 0.0;
      lVar14 = 1;
      dVar20 = 0.0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      dVar17 = 0.0;
      do {
        dVar16 = *(double *)(param_6 + *(long *)(param_3 + 0x28 + lVar14 * 8) * 8);
        dVar17 = dVar17 + *(double *)(lVar10 + 0x28 + lVar14 * 8) * dVar16;
        dVar18 = dVar18 + *(double *)(lVar11 + 0x20 + lVar14 * 8) * dVar16;
        dVar19 = dVar19 + *(double *)(lVar13 + 0x18 + lVar14 * 8) * dVar16;
        lVar9 = lVar14 * 8;
        lVar2 = lVar14 * 8;
        lVar14 = lVar14 + 1;
        dVar20 = dVar20 + *(double *)(lVar12 + 0x10 + lVar9) * dVar16;
        dVar21 = dVar21 + dVar16 * *(double *)(*(long *)(param_4 + 0x28) + 8 + lVar2);
      } while (lVar14 != param_5 + -4);
    }
    param_1 = param_1 * 8;
    pdVar1 = (double *)(param_6 + param_1);
    pdVar4 = (double *)(param_6 + -8 + param_1);
    pdVar5 = (double *)(param_6 + -0x10 + param_1);
    pdVar6 = (double *)(param_6 + -0x18 + param_1);
    pdVar15 = (double *)(param_6 + -0x20 + param_1);
    dVar21 = (*pdVar1 - dVar21) / *(double *)(*(long *)(param_4 + 0x28) + 8);
    *pdVar1 = dVar21;
    dVar20 = ((*pdVar4 - dVar20) - dVar21 * *(double *)(lVar12 + 0x10)) / *(double *)(lVar12 + 8);
    *pdVar4 = dVar20;
    dVar19 = (((*pdVar5 - dVar19) - *pdVar1 * *(double *)(lVar13 + 0x18)) -
             dVar20 * *(double *)(lVar13 + 0x10)) / *(double *)(lVar13 + 8);
    *pdVar5 = dVar19;
    dVar19 = ((((*pdVar6 - dVar18) - *pdVar1 * *(double *)(lVar11 + 0x20)) -
              *pdVar4 * *(double *)(lVar11 + 0x18)) - dVar19 * *(double *)(lVar11 + 0x10)) /
             *(double *)(lVar11 + 8);
    *pdVar6 = dVar19;
    dVar18 = ((*pdVar15 - dVar17) - *pdVar1 * *(double *)(lVar10 + 0x28)) -
             *pdVar4 * *(double *)(lVar10 + 0x20);
    dVar17 = *pdVar5;
    break;
  case 6:
    lVar10 = *(long *)(param_4 + 8);
    lVar11 = *(long *)(param_4 + 0x10);
    lVar13 = *(long *)(param_4 + 0x18);
    lVar12 = *(long *)(param_4 + 0x20);
    lVar14 = *(long *)(param_4 + 0x28);
    if (param_5 < 7) {
      dVar16 = 0.0;
      dVar17 = dVar16;
      dVar18 = dVar16;
      dVar19 = dVar16;
      dVar20 = dVar16;
      dVar21 = dVar16;
    }
    else {
      dVar16 = 0.0;
      lVar9 = 1;
      dVar21 = 0.0;
      dVar20 = 0.0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      dVar17 = 0.0;
      do {
        dVar8 = *(double *)(param_6 + *(long *)(param_3 + 0x30 + lVar9 * 8) * 8);
        dVar17 = dVar17 + *(double *)(lVar10 + 0x30 + lVar9 * 8) * dVar8;
        dVar18 = dVar18 + *(double *)(lVar11 + 0x28 + lVar9 * 8) * dVar8;
        dVar19 = dVar19 + *(double *)(lVar13 + 0x20 + lVar9 * 8) * dVar8;
        dVar20 = dVar20 + *(double *)(lVar12 + 0x18 + lVar9 * 8) * dVar8;
        lVar2 = lVar9 * 8;
        lVar3 = lVar9 * 8;
        lVar9 = lVar9 + 1;
        dVar21 = dVar21 + *(double *)(lVar14 + 0x10 + lVar2) * dVar8;
        dVar16 = dVar16 + dVar8 * *(double *)(*(long *)(param_4 + 0x30) + 8 + lVar3);
      } while (lVar9 != param_5 + -5);
    }
    param_1 = param_1 * 8;
    pdVar1 = (double *)(param_6 + param_1);
    pdVar4 = (double *)(param_6 + -8 + param_1);
    pdVar5 = (double *)(param_6 + -0x10 + param_1);
    pdVar6 = (double *)(param_6 + -0x18 + param_1);
    dVar16 = (*pdVar1 - dVar16) / *(double *)(*(long *)(param_4 + 0x30) + 8);
    *pdVar1 = dVar16;
    dVar21 = ((*pdVar4 - dVar21) - dVar16 * *(double *)(lVar14 + 0x10)) / *(double *)(lVar14 + 8);
    *pdVar4 = dVar21;
    dVar20 = (((*pdVar5 - dVar20) - *pdVar1 * *(double *)(lVar12 + 0x18)) -
             dVar21 * *(double *)(lVar12 + 0x10)) / *(double *)(lVar12 + 8);
    *pdVar5 = dVar20;
    dVar19 = ((((*pdVar6 - dVar19) - *pdVar1 * *(double *)(lVar13 + 0x20)) -
              *pdVar4 * *(double *)(lVar13 + 0x18)) - dVar20 * *(double *)(lVar13 + 0x10)) /
             *(double *)(lVar13 + 8);
    pdVar7 = (double *)(param_6 + -0x20 + param_1);
    pdVar15 = (double *)(param_6 + -0x28 + param_1);
    *pdVar6 = dVar19;
    dVar19 = (((((*pdVar7 - dVar18) - *pdVar1 * *(double *)(lVar11 + 0x28)) -
               *pdVar4 * *(double *)(lVar11 + 0x20)) - *pdVar5 * *(double *)(lVar11 + 0x18)) -
             dVar19 * *(double *)(lVar11 + 0x10)) / *(double *)(lVar11 + 8);
    *pdVar7 = dVar19;
    dVar18 = (((*pdVar15 - dVar17) - *pdVar1 * *(double *)(lVar10 + 0x30)) -
             *pdVar4 * *(double *)(lVar10 + 0x28)) - *pdVar5 * *(double *)(lVar10 + 0x20);
    dVar17 = *pdVar6;
  }
  *pdVar15 = ((dVar18 - dVar17 * *(double *)(lVar10 + 0x18)) - dVar19 * *(double *)(lVar10 + 0x10))
             / *(double *)(lVar10 + 8);
  return;
}


