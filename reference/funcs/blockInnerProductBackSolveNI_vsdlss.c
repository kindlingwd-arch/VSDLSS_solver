/* VSDLSS function (decompiled by Ghidra) */
/* name: blockInnerProductBackSolveNI_vsdlss  addr: 00a2eb20  size: 6fe */
#include "vsdlss_ref.h"

/* signature: void blockInnerProductBackSolveNI_vsdlss(int param_1,undefined4 param_2,long param_3,int param_4,long param_5); */

void blockInnerProductBackSolveNI_vsdlss
               (int param_1,undefined4 param_2,long param_3,int param_4,long param_5)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  double *pdVar4;
  double *pdVar5;
  double dVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  double *pdVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  
  switch(param_2) {
  default:
    nrerror_vsdlss(0xf,"triangularsolves.c",0x38f,6,"blockInnerProductBackSolveNI");
    return;
  case 1:
    dVar16 = 0.0;
    pdVar1 = (double *)(param_5 + (long)param_1 * 8);
    if (1 < param_4) {
      dVar16 = 0.0;
      lVar8 = 0;
      do {
        pdVar2 = (double *)(*(long *)(param_3 + 8) + 0x10 + lVar8);
        lVar9 = lVar8 + 8;
        lVar8 = lVar8 + 8;
        dVar16 = dVar16 + *pdVar2 * *(double *)((long)pdVar1 + lVar9);
      } while (lVar8 != (ulong)(param_4 - 2) * 8 + 8);
    }
    *pdVar1 = (*pdVar1 - dVar16) / *(double *)(*(long *)(param_3 + 8) + 8);
    return;
  case 2:
    lVar8 = *(long *)(param_3 + 8);
    pdVar1 = (double *)(param_5 + (long)param_1 * 8);
    if (param_4 < 3) {
      dVar17 = 0.0;
      dVar16 = dVar17;
    }
    else {
      dVar17 = 0.0;
      lVar9 = 0;
      dVar16 = 0.0;
      do {
        dVar18 = *(double *)((long)pdVar1 + lVar9 + 8);
        pdVar2 = (double *)(lVar8 + 0x18 + lVar9);
        pdVar3 = (double *)(*(long *)(param_3 + 0x10) + 0x10 + lVar9);
        lVar9 = lVar9 + 8;
        dVar16 = dVar16 + *pdVar2 * dVar18;
        dVar17 = dVar17 + dVar18 * *pdVar3;
      } while (lVar9 != (ulong)(param_4 - 3) * 8 + 8);
    }
    pdVar2 = (double *)(param_5 + -8 + (long)param_1 * 8);
    dVar17 = (*pdVar1 - dVar17) / *(double *)(*(long *)(param_3 + 0x10) + 8);
    *pdVar1 = dVar17;
    *pdVar2 = ((*pdVar2 - dVar16) - dVar17 * *(double *)(lVar8 + 0x10)) / *(double *)(lVar8 + 8);
    return;
  case 3:
    lVar8 = *(long *)(param_3 + 8);
    lVar9 = *(long *)(param_3 + 0x10);
    lVar12 = (long)param_1 * 8;
    pdVar1 = (double *)(param_5 + lVar12);
    if (param_4 < 4) {
      dVar18 = 0.0;
      dVar16 = dVar18;
      dVar17 = dVar18;
    }
    else {
      dVar18 = 0.0;
      lVar11 = 0;
      dVar17 = 0.0;
      dVar16 = 0.0;
      do {
        dVar19 = *(double *)((long)pdVar1 + lVar11 + 8);
        dVar16 = dVar16 + *(double *)(lVar8 + 0x20 + lVar11) * dVar19;
        pdVar2 = (double *)(lVar9 + 0x18 + lVar11);
        pdVar3 = (double *)(*(long *)(param_3 + 0x18) + 0x10 + lVar11);
        lVar11 = lVar11 + 8;
        dVar17 = dVar17 + *pdVar2 * dVar19;
        dVar18 = dVar18 + dVar19 * *pdVar3;
      } while (lVar11 != (ulong)(param_4 - 4) * 8 + 8);
    }
    pdVar2 = (double *)(param_5 + -8 + lVar12);
    dVar18 = (*pdVar1 - dVar18) / *(double *)(*(long *)(param_3 + 0x18) + 8);
    *pdVar1 = dVar18;
    dVar17 = ((*pdVar2 - dVar17) - dVar18 * *(double *)(lVar9 + 0x10)) / *(double *)(lVar9 + 8);
    *pdVar2 = dVar17;
    pdVar2 = (double *)(param_5 + -0x10 + lVar12);
    *pdVar2 = (((*pdVar2 - dVar16) - *pdVar1 * *(double *)(lVar8 + 0x18)) -
              dVar17 * *(double *)(lVar8 + 0x10)) / *(double *)(lVar8 + 8);
    return;
  case 4:
    lVar8 = *(long *)(param_3 + 8);
    lVar9 = *(long *)(param_3 + 0x10);
    lVar11 = (long)param_1 * 8;
    lVar12 = *(long *)(param_3 + 0x18);
    pdVar1 = (double *)(param_5 + lVar11);
    if (param_4 < 5) {
      dVar19 = 0.0;
      dVar16 = dVar19;
      dVar17 = dVar19;
      dVar18 = dVar19;
    }
    else {
      dVar19 = 0.0;
      lVar13 = 0;
      dVar18 = 0.0;
      dVar17 = 0.0;
      dVar16 = 0.0;
      do {
        dVar20 = *(double *)((long)pdVar1 + lVar13 + 8);
        dVar16 = dVar16 + *(double *)(lVar8 + 0x28 + lVar13) * dVar20;
        dVar17 = dVar17 + *(double *)(lVar9 + 0x20 + lVar13) * dVar20;
        pdVar2 = (double *)(lVar12 + 0x18 + lVar13);
        pdVar3 = (double *)(*(long *)(param_3 + 0x20) + 0x10 + lVar13);
        lVar13 = lVar13 + 8;
        dVar18 = dVar18 + *pdVar2 * dVar20;
        dVar19 = dVar19 + dVar20 * *pdVar3;
      } while (lVar13 != (ulong)(param_4 - 5) * 8 + 8);
    }
    pdVar2 = (double *)(param_5 + -8 + lVar11);
    pdVar3 = (double *)(param_5 + -0x10 + lVar11);
    dVar19 = (*pdVar1 - dVar19) / *(double *)(*(long *)(param_3 + 0x20) + 8);
    *pdVar1 = dVar19;
    dVar18 = ((*pdVar2 - dVar18) - dVar19 * *(double *)(lVar12 + 0x10)) / *(double *)(lVar12 + 8);
    *pdVar2 = dVar18;
    dVar17 = (((*pdVar3 - dVar17) - *pdVar1 * *(double *)(lVar9 + 0x18)) -
             dVar18 * *(double *)(lVar9 + 0x10)) / *(double *)(lVar9 + 8);
    *pdVar3 = dVar17;
    pdVar3 = (double *)(param_5 + -0x18 + lVar11);
    *pdVar3 = ((((*pdVar3 - dVar16) - *pdVar1 * *(double *)(lVar8 + 0x20)) -
               *pdVar2 * *(double *)(lVar8 + 0x18)) - dVar17 * *(double *)(lVar8 + 0x10)) /
              *(double *)(lVar8 + 8);
    return;
  case 5:
    lVar8 = *(long *)(param_3 + 8);
    lVar9 = *(long *)(param_3 + 0x10);
    lVar13 = (long)param_1 * 8;
    lVar12 = *(long *)(param_3 + 0x18);
    lVar11 = *(long *)(param_3 + 0x20);
    pdVar1 = (double *)(param_5 + lVar13);
    if (param_4 < 6) {
      dVar20 = 0.0;
      dVar16 = dVar20;
      dVar17 = dVar20;
      dVar18 = dVar20;
      dVar19 = dVar20;
    }
    else {
      dVar20 = 0.0;
      lVar10 = 0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      dVar17 = 0.0;
      dVar16 = 0.0;
      do {
        dVar15 = *(double *)((long)pdVar1 + lVar10 + 8);
        dVar16 = dVar16 + *(double *)(lVar8 + 0x30 + lVar10) * dVar15;
        dVar17 = dVar17 + *(double *)(lVar9 + 0x28 + lVar10) * dVar15;
        dVar18 = dVar18 + *(double *)(lVar12 + 0x20 + lVar10) * dVar15;
        pdVar2 = (double *)(lVar11 + 0x18 + lVar10);
        pdVar3 = (double *)(*(long *)(param_3 + 0x28) + 0x10 + lVar10);
        lVar10 = lVar10 + 8;
        dVar19 = dVar19 + *pdVar2 * dVar15;
        dVar20 = dVar20 + dVar15 * *pdVar3;
      } while (lVar10 != (ulong)(param_4 - 6) * 8 + 8);
    }
    pdVar2 = (double *)(param_5 + -8 + lVar13);
    pdVar3 = (double *)(param_5 + -0x10 + lVar13);
    dVar20 = (*pdVar1 - dVar20) / *(double *)(*(long *)(param_3 + 0x28) + 8);
    *pdVar1 = dVar20;
    dVar19 = ((*pdVar2 - dVar19) - dVar20 * *(double *)(lVar11 + 0x10)) / *(double *)(lVar11 + 8);
    *pdVar2 = dVar19;
    dVar18 = (((*pdVar3 - dVar18) - *pdVar1 * *(double *)(lVar12 + 0x18)) -
             dVar19 * *(double *)(lVar12 + 0x10)) / *(double *)(lVar12 + 8);
    pdVar4 = (double *)(param_5 + -0x18 + lVar13);
    pdVar14 = (double *)(param_5 + -0x20 + lVar13);
    *pdVar3 = dVar18;
    dVar18 = ((((*pdVar4 - dVar17) - *pdVar1 * *(double *)(lVar9 + 0x20)) -
              *pdVar2 * *(double *)(lVar9 + 0x18)) - dVar18 * *(double *)(lVar9 + 0x10)) /
             *(double *)(lVar9 + 8);
    *pdVar4 = dVar18;
    dVar17 = ((*pdVar14 - dVar16) - *pdVar1 * *(double *)(lVar8 + 0x28)) -
             *pdVar2 * *(double *)(lVar8 + 0x20);
    dVar16 = *pdVar3;
    break;
  case 6:
    lVar8 = *(long *)(param_3 + 8);
    lVar9 = *(long *)(param_3 + 0x10);
    lVar10 = (long)param_1 * 8;
    lVar12 = *(long *)(param_3 + 0x18);
    lVar11 = *(long *)(param_3 + 0x20);
    lVar13 = *(long *)(param_3 + 0x28);
    pdVar1 = (double *)(param_5 + lVar10);
    if (param_4 < 7) {
      dVar15 = 0.0;
      dVar16 = dVar15;
      dVar17 = dVar15;
      dVar18 = dVar15;
      dVar19 = dVar15;
      dVar20 = dVar15;
    }
    else {
      dVar15 = 0.0;
      lVar7 = 0;
      dVar20 = 0.0;
      dVar19 = 0.0;
      dVar18 = 0.0;
      dVar17 = 0.0;
      dVar16 = 0.0;
      do {
        dVar6 = *(double *)((long)pdVar1 + lVar7 + 8);
        dVar16 = dVar16 + *(double *)(lVar8 + 0x38 + lVar7) * dVar6;
        dVar17 = dVar17 + *(double *)(lVar9 + 0x30 + lVar7) * dVar6;
        dVar18 = dVar18 + *(double *)(lVar12 + 0x28 + lVar7) * dVar6;
        dVar19 = dVar19 + *(double *)(lVar11 + 0x20 + lVar7) * dVar6;
        pdVar2 = (double *)(lVar13 + 0x18 + lVar7);
        pdVar3 = (double *)(*(long *)(param_3 + 0x30) + 0x10 + lVar7);
        lVar7 = lVar7 + 8;
        dVar20 = dVar20 + *pdVar2 * dVar6;
        dVar15 = dVar15 + dVar6 * *pdVar3;
      } while (lVar7 != (ulong)(param_4 - 7) * 8 + 8);
    }
    pdVar2 = (double *)(param_5 + -8 + lVar10);
    pdVar3 = (double *)(param_5 + -0x10 + lVar10);
    dVar15 = (*pdVar1 - dVar15) / *(double *)(*(long *)(param_3 + 0x30) + 8);
    *pdVar1 = dVar15;
    dVar20 = ((*pdVar2 - dVar20) - dVar15 * *(double *)(lVar13 + 0x10)) / *(double *)(lVar13 + 8);
    *pdVar2 = dVar20;
    dVar19 = (((*pdVar3 - dVar19) - *pdVar1 * *(double *)(lVar11 + 0x18)) -
             dVar20 * *(double *)(lVar11 + 0x10)) / *(double *)(lVar11 + 8);
    pdVar4 = (double *)(param_5 + -0x18 + lVar10);
    *pdVar3 = dVar19;
    dVar18 = ((((*pdVar4 - dVar18) - *pdVar1 * *(double *)(lVar12 + 0x20)) -
              *pdVar2 * *(double *)(lVar12 + 0x18)) - dVar19 * *(double *)(lVar12 + 0x10)) /
             *(double *)(lVar12 + 8);
    pdVar5 = (double *)(param_5 + -0x20 + lVar10);
    pdVar14 = (double *)(param_5 + -0x28 + lVar10);
    *pdVar4 = dVar18;
    dVar18 = (((((*pdVar5 - dVar17) - *pdVar1 * *(double *)(lVar9 + 0x28)) -
               *pdVar2 * *(double *)(lVar9 + 0x20)) - *pdVar3 * *(double *)(lVar9 + 0x18)) -
             dVar18 * *(double *)(lVar9 + 0x10)) / *(double *)(lVar9 + 8);
    *pdVar5 = dVar18;
    dVar17 = (((*pdVar14 - dVar16) - *pdVar1 * *(double *)(lVar8 + 0x30)) -
             *pdVar2 * *(double *)(lVar8 + 0x28)) - *pdVar3 * *(double *)(lVar8 + 0x20);
    dVar16 = *pdVar4;
  }
  *pdVar14 = ((dVar17 - dVar16 * *(double *)(lVar8 + 0x18)) - dVar18 * *(double *)(lVar8 + 0x10)) /
             *(double *)(lVar8 + 8);
  return;
}


