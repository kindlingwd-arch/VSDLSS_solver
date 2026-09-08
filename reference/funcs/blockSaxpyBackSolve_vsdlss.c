/* VSDLSS function (decompiled by Ghidra) */
/* name: blockSaxpyBackSolve_vsdlss  addr: 00a2d3f0  size: 6b5 */
#include "vsdlss_ref.h"

/* signature: void blockSaxpyBackSolve_vsdlss(int param_1,undefined4 param_2,long param_3,long param_4,int param_5,long param_6); */

void blockSaxpyBackSolve_vsdlss
               (int param_1,undefined4 param_2,long param_3,long param_4,int param_5,long param_6)

{
  long lVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  double *pdVar7;
  double *pdVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  
  uVar13 = DAT_00abd200;
  switch(param_2) {
  default:
    nrerror_vsdlss(0xf,"triangularsolves.c",0xcf,6,"blockSaxpyBackSolve");
    return;
  case 1:
    lVar14 = *(long *)(param_4 + 8);
    dVar17 = (double)(*(ulong *)(param_6 + (long)param_1 * 8) ^ DAT_00abd200);
    dVar19 = *(double *)(lVar14 + 8);
    if (1 < param_5) {
      lVar15 = 0;
      do {
        pdVar7 = (double *)(param_6 + (long)*(int *)(param_3 + 8 + lVar15) * 8);
        lVar16 = lVar15 * 2;
        lVar15 = lVar15 + 4;
        *pdVar7 = *(double *)(lVar14 + 0x10 + lVar16) * (dVar17 / dVar19) + *pdVar7;
      } while (lVar15 != (ulong)(param_5 - 2) * 4 + 4);
      return;
    }
    break;
  case 2:
    lVar14 = *(long *)(param_4 + 8);
    pdVar7 = (double *)(param_6 + 8 + (long)param_1 * 8);
    lVar15 = *(long *)(param_4 + 0x10);
    dVar17 = (double)(*(ulong *)(param_6 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar14 + 8);
    dVar19 = *(double *)(lVar14 + 0x10) * dVar17 + *pdVar7;
    dVar18 = (double)(DAT_00abd200 ^ (ulong)dVar19);
    *pdVar7 = dVar19;
    dVar19 = *(double *)(lVar15 + 8);
    if (2 < param_5) {
      lVar16 = 0;
      do {
        pdVar7 = (double *)(param_6 + (long)*(int *)(param_3 + 0xc + lVar16) * 8);
        lVar9 = lVar16 * 2;
        lVar10 = lVar16 * 2;
        lVar16 = lVar16 + 4;
        *pdVar7 = *(double *)(lVar14 + 0x18 + lVar9) * dVar17 +
                  *(double *)(lVar15 + 0x10 + lVar10) * (dVar18 / dVar19) + *pdVar7;
      } while (lVar16 != (ulong)(param_5 - 3) * 4 + 4);
      return;
    }
    break;
  case 3:
    lVar15 = *(long *)(param_4 + 8);
    lVar14 = (long)param_1 * 8;
    lVar16 = *(long *)(param_4 + 0x10);
    lVar9 = *(long *)(param_4 + 0x18);
    pdVar7 = (double *)(param_6 + 8 + lVar14);
    pdVar8 = (double *)(param_6 + 0x10 + lVar14);
    dVar18 = (double)(*(ulong *)(param_6 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar15 + 8);
    dVar19 = *(double *)(lVar15 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    dVar20 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar16 + 8);
    dVar17 = *(double *)(lVar15 + 0x18) * dVar18 + *(double *)(lVar16 + 0x10) * dVar20 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar9 + 8);
    if (3 < param_5) {
      lVar14 = 0;
      do {
        pdVar7 = (double *)(param_6 + (long)*(int *)(param_3 + 0x10 + lVar14) * 8);
        lVar10 = lVar14 * 2;
        lVar11 = lVar14 * 2;
        lVar12 = lVar14 * 2;
        lVar14 = lVar14 + 4;
        *pdVar7 = *(double *)(lVar15 + 0x20 + lVar10) * dVar18 +
                  *(double *)(lVar16 + 0x18 + lVar11) * dVar20 +
                  *(double *)(lVar9 + 0x10 + lVar12) * ((double)(uVar13 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar14 != (ulong)(param_5 - 4) * 4 + 4);
      return;
    }
    break;
  case 4:
    lVar15 = *(long *)(param_4 + 8);
    lVar14 = (long)param_1 * 8;
    lVar16 = *(long *)(param_4 + 0x10);
    lVar9 = *(long *)(param_4 + 0x18);
    lVar10 = *(long *)(param_4 + 0x20);
    pdVar7 = (double *)(param_6 + 8 + lVar14);
    dVar18 = (double)(*(ulong *)(param_6 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar15 + 8);
    dVar19 = *(double *)(lVar15 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x10 + lVar14);
    pdVar8 = (double *)(param_6 + 0x18 + lVar14);
    dVar20 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar16 + 8);
    dVar19 = *(double *)(lVar15 + 0x18) * dVar18 + *(double *)(lVar16 + 0x10) * dVar20 + *pdVar7;
    *pdVar7 = dVar19;
    dVar21 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar9 + 8);
    dVar17 = *(double *)(lVar15 + 0x20) * dVar18 + *(double *)(lVar16 + 0x18) * dVar20 +
             *(double *)(lVar9 + 0x10) * dVar21 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar10 + 8);
    if (4 < param_5) {
      lVar14 = 0;
      do {
        pdVar7 = (double *)(param_6 + (long)*(int *)(param_3 + 0x14 + lVar14) * 8);
        lVar11 = lVar14 * 2;
        lVar12 = lVar14 * 2;
        lVar1 = lVar14 * 2;
        lVar2 = lVar14 * 2;
        lVar14 = lVar14 + 4;
        *pdVar7 = *(double *)(lVar15 + 0x28 + lVar11) * dVar18 +
                  *(double *)(lVar16 + 0x20 + lVar12) * dVar20 +
                  *(double *)(lVar9 + 0x18 + lVar1) * dVar21 +
                  *(double *)(lVar10 + 0x10 + lVar2) * ((double)(uVar13 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar14 != (ulong)(param_5 - 5) * 4 + 4);
      return;
    }
    break;
  case 5:
    lVar15 = *(long *)(param_4 + 8);
    lVar14 = (long)param_1 * 8;
    lVar16 = *(long *)(param_4 + 0x10);
    lVar9 = *(long *)(param_4 + 0x18);
    lVar10 = *(long *)(param_4 + 0x20);
    lVar11 = *(long *)(param_4 + 0x28);
    pdVar7 = (double *)(param_6 + 8 + lVar14);
    dVar18 = (double)(*(ulong *)(param_6 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar15 + 8);
    dVar19 = *(double *)(lVar15 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x10 + lVar14);
    dVar20 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar16 + 8);
    dVar19 = *(double *)(lVar15 + 0x18) * dVar18 + *(double *)(lVar16 + 0x10) * dVar20 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x18 + lVar14);
    pdVar8 = (double *)(param_6 + 0x20 + lVar14);
    dVar21 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar9 + 8);
    dVar19 = *(double *)(lVar15 + 0x20) * dVar18 + *(double *)(lVar16 + 0x18) * dVar20 +
             *(double *)(lVar9 + 0x10) * dVar21 + *pdVar7;
    *pdVar7 = dVar19;
    dVar22 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar10 + 8);
    dVar17 = *(double *)(lVar15 + 0x28) * dVar18 + *(double *)(lVar16 + 0x20) * dVar20 +
             *(double *)(lVar9 + 0x18) * dVar21 + *(double *)(lVar10 + 0x10) * dVar22 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar11 + 8);
    if (5 < param_5) {
      lVar14 = 0;
      do {
        pdVar7 = (double *)(param_6 + (long)*(int *)(param_3 + 0x18 + lVar14) * 8);
        lVar12 = lVar14 * 2;
        lVar1 = lVar14 * 2;
        lVar2 = lVar14 * 2;
        lVar3 = lVar14 * 2;
        lVar4 = lVar14 * 2;
        lVar14 = lVar14 + 4;
        *pdVar7 = *(double *)(lVar15 + 0x30 + lVar12) * dVar18 +
                  *(double *)(lVar16 + 0x28 + lVar1) * dVar20 +
                  *(double *)(lVar9 + 0x20 + lVar2) * dVar21 +
                  *(double *)(lVar10 + 0x18 + lVar3) * dVar22 +
                  *(double *)(lVar11 + 0x10 + lVar4) * ((double)(uVar13 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar14 != (ulong)(param_5 - 6) * 4 + 4);
      return;
    }
    break;
  case 6:
    lVar15 = *(long *)(param_4 + 8);
    lVar14 = (long)param_1 * 8;
    lVar16 = *(long *)(param_4 + 0x10);
    lVar9 = *(long *)(param_4 + 0x18);
    lVar10 = *(long *)(param_4 + 0x20);
    lVar11 = *(long *)(param_4 + 0x28);
    lVar12 = *(long *)(param_4 + 0x30);
    pdVar7 = (double *)(param_6 + 8 + lVar14);
    dVar18 = (double)(*(ulong *)(param_6 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar15 + 8);
    dVar19 = *(double *)(lVar15 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x10 + lVar14);
    dVar20 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar16 + 8);
    dVar19 = *(double *)(lVar15 + 0x18) * dVar18 + *(double *)(lVar16 + 0x10) * dVar20 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x18 + lVar14);
    dVar21 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar9 + 8);
    dVar19 = *(double *)(lVar15 + 0x20) * dVar18 + *(double *)(lVar16 + 0x18) * dVar20 +
             *(double *)(lVar9 + 0x10) * dVar21 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x20 + lVar14);
    pdVar8 = (double *)(param_6 + 0x28 + lVar14);
    dVar22 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar10 + 8);
    dVar19 = *(double *)(lVar15 + 0x28) * dVar18 + *(double *)(lVar16 + 0x20) * dVar20 +
             *(double *)(lVar9 + 0x18) * dVar21 + *(double *)(lVar10 + 0x10) * dVar22 + *pdVar7;
    *pdVar7 = dVar19;
    dVar23 = (double)((ulong)dVar19 ^ uVar13) / *(double *)(lVar11 + 8);
    dVar17 = *(double *)(lVar15 + 0x30) * dVar18 + *(double *)(lVar16 + 0x28) * dVar20 +
             *(double *)(lVar9 + 0x20) * dVar21 + *(double *)(lVar10 + 0x18) * dVar22 +
             *(double *)(lVar11 + 0x10) * dVar23 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar12 + 8);
    if (6 < param_5) {
      lVar14 = 0;
      do {
        pdVar7 = (double *)(param_6 + (long)*(int *)(param_3 + 0x1c + lVar14) * 8);
        lVar1 = lVar14 * 2;
        lVar2 = lVar14 * 2;
        lVar3 = lVar14 * 2;
        lVar4 = lVar14 * 2;
        lVar5 = lVar14 * 2;
        lVar6 = lVar14 * 2;
        lVar14 = lVar14 + 4;
        *pdVar7 = *(double *)(lVar15 + 0x38 + lVar1) * dVar18 +
                  *(double *)(lVar16 + 0x30 + lVar2) * dVar20 +
                  *(double *)(lVar9 + 0x28 + lVar3) * dVar21 +
                  *(double *)(lVar10 + 0x20 + lVar4) * dVar22 +
                  *(double *)(lVar11 + 0x18 + lVar5) * dVar23 +
                  *(double *)(lVar12 + 0x10 + lVar6) * ((double)(uVar13 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar14 != (ulong)(param_5 - 7) * 4 + 4);
    }
  }
  return;
}


