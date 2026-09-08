/* VSDLSS function (decompiled by Ghidra) */
/* name: blockInnerProductBackSolve_vsdlss  addr: 00a2e3d0  size: 72a */
#include "vsdlss_ref.h"

/* signature: void blockInnerProductBackSolve_vsdlss(int param_1,undefined4 param_2,long param_3,long param_4,int param_5,long param_6); */

void blockInnerProductBackSolve_vsdlss
               (int param_1,undefined4 param_2,long param_3,long param_4,int param_5,long param_6)

{
  double *pdVar1;
  long lVar2;
  long lVar3;
  double *pdVar4;
  double *pdVar5;
  double *pdVar6;
  double *pdVar7;
  int *piVar8;
  double dVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  double *pdVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  
  switch(param_2) {
  default:
    nrerror_vsdlss(0xf,"triangularsolves.c",0x2c7,6,"blockInnerProductBackSolve");
    return;
  case 1:
    dVar18 = 0.0;
    if (1 < param_5) {
      dVar18 = 0.0;
      lVar11 = 0;
      do {
        piVar8 = (int *)(param_3 + 8 + lVar11);
        lVar12 = lVar11 * 2;
        lVar11 = lVar11 + 4;
        dVar18 = dVar18 + *(double *)(*(long *)(param_4 + 8) + 0x10 + lVar12) *
                          *(double *)(param_6 + (long)*piVar8 * 8);
      } while (lVar11 != (ulong)(param_5 - 2) * 4 + 4);
    }
    pdVar1 = (double *)(param_6 + (long)param_1 * 8);
    *pdVar1 = (*pdVar1 - dVar18) / *(double *)(*(long *)(param_4 + 8) + 8);
    return;
  case 2:
    lVar11 = *(long *)(param_4 + 8);
    if (param_5 < 3) {
      dVar19 = 0.0;
      dVar18 = dVar19;
    }
    else {
      dVar19 = 0.0;
      lVar12 = 0;
      dVar18 = 0.0;
      do {
        lVar14 = lVar12 * 2;
        dVar20 = *(double *)(param_6 + (long)*(int *)(param_3 + 0xc + lVar12) * 8);
        lVar13 = lVar12 * 2;
        lVar12 = lVar12 + 4;
        dVar18 = dVar18 + *(double *)(lVar11 + 0x18 + lVar14) * dVar20;
        dVar19 = dVar19 + dVar20 * *(double *)(*(long *)(param_4 + 0x10) + 0x10 + lVar13);
      } while (lVar12 != (ulong)(param_5 - 3) * 4 + 4);
    }
    pdVar1 = (double *)(param_6 + (long)param_1 * 8);
    dVar19 = (*pdVar1 - dVar19) / *(double *)(*(long *)(param_4 + 0x10) + 8);
    *pdVar1 = dVar19;
    pdVar1 = (double *)(param_6 + -8 + (long)param_1 * 8);
    *pdVar1 = ((*pdVar1 - dVar18) - dVar19 * *(double *)(lVar11 + 0x10)) / *(double *)(lVar11 + 8);
    return;
  case 3:
    lVar11 = *(long *)(param_4 + 8);
    lVar12 = *(long *)(param_4 + 0x10);
    if (param_5 < 4) {
      dVar20 = 0.0;
      dVar18 = dVar20;
      dVar19 = dVar20;
    }
    else {
      dVar20 = 0.0;
      lVar14 = 0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      do {
        dVar21 = *(double *)(param_6 + (long)*(int *)(param_3 + 0x10 + lVar14) * 8);
        dVar18 = dVar18 + *(double *)(lVar11 + 0x20 + lVar14 * 2) * dVar21;
        lVar13 = lVar14 * 2;
        lVar15 = lVar14 * 2;
        lVar14 = lVar14 + 4;
        dVar19 = dVar19 + *(double *)(lVar12 + 0x18 + lVar13) * dVar21;
        dVar20 = dVar20 + dVar21 * *(double *)(*(long *)(param_4 + 0x18) + 0x10 + lVar15);
      } while (lVar14 != (ulong)(param_5 - 4) * 4 + 4);
    }
    lVar14 = (long)param_1 * 8;
    pdVar1 = (double *)(param_6 + lVar14);
    pdVar4 = (double *)(param_6 + -8 + lVar14);
    dVar20 = (*pdVar1 - dVar20) / *(double *)(*(long *)(param_4 + 0x18) + 8);
    *pdVar1 = dVar20;
    dVar19 = ((*pdVar4 - dVar19) - dVar20 * *(double *)(lVar12 + 0x10)) / *(double *)(lVar12 + 8);
    *pdVar4 = dVar19;
    pdVar4 = (double *)(param_6 + -0x10 + lVar14);
    *pdVar4 = (((*pdVar4 - dVar18) - *pdVar1 * *(double *)(lVar11 + 0x18)) -
              dVar19 * *(double *)(lVar11 + 0x10)) / *(double *)(lVar11 + 8);
    return;
  case 4:
    lVar11 = *(long *)(param_4 + 8);
    lVar12 = *(long *)(param_4 + 0x10);
    lVar14 = *(long *)(param_4 + 0x18);
    if (param_5 < 5) {
      dVar21 = 0.0;
      dVar18 = dVar21;
      dVar19 = dVar21;
      dVar20 = dVar21;
    }
    else {
      dVar21 = 0.0;
      lVar13 = 0;
      dVar20 = 0.0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      do {
        dVar22 = *(double *)(param_6 + (long)*(int *)(param_3 + 0x14 + lVar13) * 8);
        dVar18 = dVar18 + *(double *)(lVar11 + 0x28 + lVar13 * 2) * dVar22;
        dVar19 = dVar19 + *(double *)(lVar12 + 0x20 + lVar13 * 2) * dVar22;
        lVar15 = lVar13 * 2;
        lVar10 = lVar13 * 2;
        lVar13 = lVar13 + 4;
        dVar20 = dVar20 + *(double *)(lVar14 + 0x18 + lVar15) * dVar22;
        dVar21 = dVar21 + dVar22 * *(double *)(*(long *)(param_4 + 0x20) + 0x10 + lVar10);
      } while (lVar13 != (ulong)(param_5 - 5) * 4 + 4);
    }
    lVar13 = (long)param_1 * 8;
    pdVar1 = (double *)(param_6 + lVar13);
    pdVar4 = (double *)(param_6 + -8 + lVar13);
    pdVar5 = (double *)(param_6 + -0x10 + lVar13);
    dVar21 = (*pdVar1 - dVar21) / *(double *)(*(long *)(param_4 + 0x20) + 8);
    *pdVar1 = dVar21;
    dVar20 = ((*pdVar4 - dVar20) - dVar21 * *(double *)(lVar14 + 0x10)) / *(double *)(lVar14 + 8);
    *pdVar4 = dVar20;
    dVar19 = (((*pdVar5 - dVar19) - *pdVar1 * *(double *)(lVar12 + 0x18)) -
             dVar20 * *(double *)(lVar12 + 0x10)) / *(double *)(lVar12 + 8);
    *pdVar5 = dVar19;
    pdVar5 = (double *)(param_6 + -0x18 + lVar13);
    *pdVar5 = ((((*pdVar5 - dVar18) - *pdVar1 * *(double *)(lVar11 + 0x20)) -
               *pdVar4 * *(double *)(lVar11 + 0x18)) - dVar19 * *(double *)(lVar11 + 0x10)) /
              *(double *)(lVar11 + 8);
    return;
  case 5:
    lVar11 = *(long *)(param_4 + 8);
    lVar12 = *(long *)(param_4 + 0x10);
    lVar14 = *(long *)(param_4 + 0x18);
    lVar13 = *(long *)(param_4 + 0x20);
    if (param_5 < 6) {
      dVar22 = 0.0;
      dVar18 = dVar22;
      dVar19 = dVar22;
      dVar20 = dVar22;
      dVar21 = dVar22;
    }
    else {
      dVar22 = 0.0;
      lVar15 = 0;
      dVar21 = 0.0;
      dVar20 = 0.0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      do {
        dVar17 = *(double *)(param_6 + (long)*(int *)(param_3 + 0x18 + lVar15) * 8);
        dVar18 = dVar18 + *(double *)(lVar11 + 0x30 + lVar15 * 2) * dVar17;
        dVar19 = dVar19 + *(double *)(lVar12 + 0x28 + lVar15 * 2) * dVar17;
        dVar20 = dVar20 + *(double *)(lVar14 + 0x20 + lVar15 * 2) * dVar17;
        lVar10 = lVar15 * 2;
        lVar2 = lVar15 * 2;
        lVar15 = lVar15 + 4;
        dVar21 = dVar21 + *(double *)(lVar13 + 0x18 + lVar10) * dVar17;
        dVar22 = dVar22 + dVar17 * *(double *)(*(long *)(param_4 + 0x28) + 0x10 + lVar2);
      } while (lVar15 != (ulong)(param_5 - 6) * 4 + 4);
    }
    lVar15 = (long)param_1 * 8;
    pdVar1 = (double *)(param_6 + lVar15);
    pdVar4 = (double *)(param_6 + -8 + lVar15);
    pdVar5 = (double *)(param_6 + -0x10 + lVar15);
    pdVar6 = (double *)(param_6 + -0x18 + lVar15);
    pdVar16 = (double *)(param_6 + -0x20 + lVar15);
    dVar22 = (*pdVar1 - dVar22) / *(double *)(*(long *)(param_4 + 0x28) + 8);
    *pdVar1 = dVar22;
    dVar21 = ((*pdVar4 - dVar21) - dVar22 * *(double *)(lVar13 + 0x10)) / *(double *)(lVar13 + 8);
    *pdVar4 = dVar21;
    dVar20 = (((*pdVar5 - dVar20) - *pdVar1 * *(double *)(lVar14 + 0x18)) -
             dVar21 * *(double *)(lVar14 + 0x10)) / *(double *)(lVar14 + 8);
    *pdVar5 = dVar20;
    dVar20 = ((((*pdVar6 - dVar19) - *pdVar1 * *(double *)(lVar12 + 0x20)) -
              *pdVar4 * *(double *)(lVar12 + 0x18)) - dVar20 * *(double *)(lVar12 + 0x10)) /
             *(double *)(lVar12 + 8);
    *pdVar6 = dVar20;
    dVar19 = ((*pdVar16 - dVar18) - *pdVar1 * *(double *)(lVar11 + 0x28)) -
             *pdVar4 * *(double *)(lVar11 + 0x20);
    dVar18 = *pdVar5;
    break;
  case 6:
    lVar11 = *(long *)(param_4 + 8);
    lVar12 = *(long *)(param_4 + 0x10);
    lVar14 = *(long *)(param_4 + 0x18);
    lVar13 = *(long *)(param_4 + 0x20);
    lVar15 = *(long *)(param_4 + 0x28);
    if (param_5 < 7) {
      dVar17 = 0.0;
      dVar18 = dVar17;
      dVar19 = dVar17;
      dVar20 = dVar17;
      dVar21 = dVar17;
      dVar22 = dVar17;
    }
    else {
      dVar17 = 0.0;
      lVar10 = 0;
      dVar22 = 0.0;
      dVar21 = 0.0;
      dVar20 = 0.0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      do {
        dVar9 = *(double *)(param_6 + (long)*(int *)(param_3 + 0x1c + lVar10) * 8);
        dVar18 = dVar18 + *(double *)(lVar11 + 0x38 + lVar10 * 2) * dVar9;
        dVar19 = dVar19 + *(double *)(lVar12 + 0x30 + lVar10 * 2) * dVar9;
        dVar20 = dVar20 + *(double *)(lVar14 + 0x28 + lVar10 * 2) * dVar9;
        dVar21 = dVar21 + *(double *)(lVar13 + 0x20 + lVar10 * 2) * dVar9;
        lVar2 = lVar10 * 2;
        lVar3 = lVar10 * 2;
        lVar10 = lVar10 + 4;
        dVar22 = dVar22 + *(double *)(lVar15 + 0x18 + lVar2) * dVar9;
        dVar17 = dVar17 + dVar9 * *(double *)(*(long *)(param_4 + 0x30) + 0x10 + lVar3);
      } while (lVar10 != (ulong)(param_5 - 7) * 4 + 4);
    }
    lVar10 = (long)param_1 * 8;
    pdVar1 = (double *)(param_6 + lVar10);
    pdVar4 = (double *)(param_6 + -8 + lVar10);
    pdVar5 = (double *)(param_6 + -0x10 + lVar10);
    pdVar6 = (double *)(param_6 + -0x18 + lVar10);
    dVar17 = (*pdVar1 - dVar17) / *(double *)(*(long *)(param_4 + 0x30) + 8);
    *pdVar1 = dVar17;
    dVar22 = ((*pdVar4 - dVar22) - dVar17 * *(double *)(lVar15 + 0x10)) / *(double *)(lVar15 + 8);
    *pdVar4 = dVar22;
    dVar21 = (((*pdVar5 - dVar21) - *pdVar1 * *(double *)(lVar13 + 0x18)) -
             dVar22 * *(double *)(lVar13 + 0x10)) / *(double *)(lVar13 + 8);
    *pdVar5 = dVar21;
    dVar20 = ((((*pdVar6 - dVar20) - *pdVar1 * *(double *)(lVar14 + 0x20)) -
              *pdVar4 * *(double *)(lVar14 + 0x18)) - dVar21 * *(double *)(lVar14 + 0x10)) /
             *(double *)(lVar14 + 8);
    pdVar7 = (double *)(param_6 + -0x20 + lVar10);
    pdVar16 = (double *)(param_6 + -0x28 + lVar10);
    *pdVar6 = dVar20;
    dVar20 = (((((*pdVar7 - dVar19) - *pdVar1 * *(double *)(lVar12 + 0x28)) -
               *pdVar4 * *(double *)(lVar12 + 0x20)) - *pdVar5 * *(double *)(lVar12 + 0x18)) -
             dVar20 * *(double *)(lVar12 + 0x10)) / *(double *)(lVar12 + 8);
    *pdVar7 = dVar20;
    dVar19 = (((*pdVar16 - dVar18) - *pdVar1 * *(double *)(lVar11 + 0x30)) -
             *pdVar4 * *(double *)(lVar11 + 0x28)) - *pdVar5 * *(double *)(lVar11 + 0x20);
    dVar18 = *pdVar6;
  }
  *pdVar16 = ((dVar19 - dVar18 * *(double *)(lVar11 + 0x18)) - dVar20 * *(double *)(lVar11 + 0x10))
             / *(double *)(lVar11 + 8);
  return;
}


