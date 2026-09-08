/* VSDLSS function (decompiled by Ghidra) */
/* name: blockSaxpyBackSolveNI_vsdlss  addr: 00a2dad0  size: 681 */
#include "vsdlss_ref.h"

/* signature: void blockSaxpyBackSolveNI_vsdlss(int param_1,undefined4 param_2,long param_3,int param_4,long param_5); */

void blockSaxpyBackSolveNI_vsdlss
               (int param_1,undefined4 param_2,long param_3,int param_4,long param_5)

{
  ulong *puVar1;
  double *pdVar2;
  double *pdVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  ulong uVar9;
  long lVar10;
  long lVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  
  uVar9 = DAT_00abd200;
  switch(param_2) {
  default:
    nrerror_vsdlss(0xf,"triangularsolves.c",0x177,6,"blockSaxpyBackSolveNI");
    return;
  case 1:
    lVar4 = *(long *)(param_3 + 8);
    lVar10 = param_5 + -8 + (long)param_1 * 8;
    dVar12 = (double)(*(ulong *)(param_5 + (long)param_1 * 8) ^ DAT_00abd200);
    dVar14 = *(double *)(lVar4 + 8);
    if (1 < param_4) {
      lVar11 = 0;
      do {
        *(double *)(lVar10 + 0x10 + lVar11) =
             *(double *)(lVar4 + 0x10 + lVar11) * (dVar12 / dVar14) +
             *(double *)(lVar10 + 0x10 + lVar11);
        lVar11 = lVar11 + 8;
      } while (lVar11 != (ulong)(param_4 - 2) * 8 + 8);
      return;
    }
    break;
  case 2:
    lVar10 = *(long *)(param_3 + 8);
    lVar4 = *(long *)(param_3 + 0x10);
    puVar1 = (ulong *)(param_5 + (long)param_1 * 8);
    pdVar2 = (double *)(param_5 + 8 + (long)param_1 * 8);
    dVar12 = (double)(*puVar1 ^ DAT_00abd200) / *(double *)(lVar10 + 8);
    dVar14 = *(double *)(lVar10 + 0x10) * dVar12 + *pdVar2;
    dVar13 = (double)(DAT_00abd200 ^ (ulong)dVar14);
    *pdVar2 = dVar14;
    dVar14 = *(double *)(lVar4 + 8);
    if (2 < param_4) {
      lVar11 = 0;
      do {
        *(double *)((long)puVar1 + lVar11 + 0x10) =
             *(double *)(lVar10 + 0x18 + lVar11) * dVar12 +
             *(double *)(lVar4 + 0x10 + lVar11) * (dVar13 / dVar14) +
             *(double *)((long)puVar1 + lVar11 + 0x10);
        lVar11 = lVar11 + 8;
      } while (lVar11 != (ulong)(param_4 - 3) * 8 + 8);
      return;
    }
    break;
  case 3:
    lVar4 = *(long *)(param_3 + 8);
    lVar10 = (long)param_1 * 8;
    lVar11 = *(long *)(param_3 + 0x10);
    lVar5 = *(long *)(param_3 + 0x18);
    pdVar2 = (double *)(param_5 + 8 + lVar10);
    pdVar3 = (double *)(param_5 + 0x10 + lVar10);
    dVar13 = (double)(*(ulong *)(param_5 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar4 + 8);
    dVar14 = *(double *)(lVar4 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    dVar15 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar11 + 8);
    dVar12 = *(double *)(lVar4 + 0x18) * dVar13 + *(double *)(lVar11 + 0x10) * dVar15 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar5 + 8);
    if (3 < param_4) {
      lVar10 = 0;
      do {
        *(double *)((long)pdVar2 + lVar10 + 0x10) =
             *(double *)(lVar4 + 0x20 + lVar10) * dVar13 +
             *(double *)(lVar11 + 0x18 + lVar10) * dVar15 +
             *(double *)(lVar5 + 0x10 + lVar10) * ((double)(uVar9 ^ (ulong)dVar12) / dVar14) +
             *(double *)((long)pdVar2 + lVar10 + 0x10);
        lVar10 = lVar10 + 8;
      } while (lVar10 != (ulong)(param_4 - 4) * 8 + 8);
      return;
    }
    break;
  case 4:
    lVar4 = *(long *)(param_3 + 8);
    lVar10 = (long)param_1 * 8;
    lVar11 = *(long *)(param_3 + 0x10);
    lVar5 = *(long *)(param_3 + 0x18);
    lVar6 = *(long *)(param_3 + 0x20);
    pdVar2 = (double *)(param_5 + 8 + lVar10);
    dVar13 = (double)(*(ulong *)(param_5 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar4 + 8);
    dVar14 = *(double *)(lVar4 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x10 + lVar10);
    pdVar3 = (double *)(param_5 + 0x18 + lVar10);
    dVar15 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar11 + 8);
    dVar14 = *(double *)(lVar4 + 0x18) * dVar13 + *(double *)(lVar11 + 0x10) * dVar15 + *pdVar2;
    *pdVar2 = dVar14;
    dVar16 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar5 + 8);
    dVar12 = *(double *)(lVar4 + 0x20) * dVar13 + *(double *)(lVar11 + 0x18) * dVar15 +
             *(double *)(lVar5 + 0x10) * dVar16 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar6 + 8);
    if (4 < param_4) {
      lVar10 = 0;
      do {
        *(double *)((long)pdVar2 + lVar10 + 0x10) =
             *(double *)(lVar4 + 0x28 + lVar10) * dVar13 +
             *(double *)(lVar11 + 0x20 + lVar10) * dVar15 +
             *(double *)(lVar5 + 0x18 + lVar10) * dVar16 +
             *(double *)(lVar6 + 0x10 + lVar10) * ((double)(uVar9 ^ (ulong)dVar12) / dVar14) +
             *(double *)((long)pdVar2 + lVar10 + 0x10);
        lVar10 = lVar10 + 8;
      } while (lVar10 != (ulong)(param_4 - 5) * 8 + 8);
      return;
    }
    break;
  case 5:
    lVar4 = *(long *)(param_3 + 8);
    lVar10 = (long)param_1 * 8;
    lVar11 = *(long *)(param_3 + 0x10);
    lVar5 = *(long *)(param_3 + 0x18);
    lVar6 = *(long *)(param_3 + 0x20);
    lVar7 = *(long *)(param_3 + 0x28);
    pdVar2 = (double *)(param_5 + 8 + lVar10);
    dVar13 = (double)(*(ulong *)(param_5 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar4 + 8);
    dVar14 = *(double *)(lVar4 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x10 + lVar10);
    dVar15 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar11 + 8);
    dVar14 = *(double *)(lVar4 + 0x18) * dVar13 + *(double *)(lVar11 + 0x10) * dVar15 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x18 + lVar10);
    pdVar3 = (double *)(param_5 + 0x20 + lVar10);
    dVar16 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar5 + 8);
    dVar14 = *(double *)(lVar4 + 0x20) * dVar13 + *(double *)(lVar11 + 0x18) * dVar15 +
             *(double *)(lVar5 + 0x10) * dVar16 + *pdVar2;
    *pdVar2 = dVar14;
    dVar17 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar6 + 8);
    dVar12 = *(double *)(lVar4 + 0x28) * dVar13 + *(double *)(lVar11 + 0x20) * dVar15 +
             *(double *)(lVar5 + 0x18) * dVar16 + *(double *)(lVar6 + 0x10) * dVar17 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar7 + 8);
    if (5 < param_4) {
      lVar10 = 0;
      do {
        *(double *)((long)pdVar2 + lVar10 + 0x10) =
             *(double *)(lVar4 + 0x30 + lVar10) * dVar13 +
             *(double *)(lVar11 + 0x28 + lVar10) * dVar15 +
             *(double *)(lVar5 + 0x20 + lVar10) * dVar16 +
             *(double *)(lVar6 + 0x18 + lVar10) * dVar17 +
             *(double *)(lVar7 + 0x10 + lVar10) * ((double)(uVar9 ^ (ulong)dVar12) / dVar14) +
             *(double *)((long)pdVar2 + lVar10 + 0x10);
        lVar10 = lVar10 + 8;
      } while (lVar10 != (ulong)(param_4 - 6) * 8 + 8);
      return;
    }
    break;
  case 6:
    lVar4 = *(long *)(param_3 + 8);
    lVar10 = (long)param_1 * 8;
    lVar11 = *(long *)(param_3 + 0x10);
    lVar5 = *(long *)(param_3 + 0x18);
    lVar6 = *(long *)(param_3 + 0x20);
    lVar7 = *(long *)(param_3 + 0x28);
    lVar8 = *(long *)(param_3 + 0x30);
    pdVar2 = (double *)(param_5 + 8 + lVar10);
    dVar13 = (double)(*(ulong *)(param_5 + (long)param_1 * 8) ^ DAT_00abd200) /
             *(double *)(lVar4 + 8);
    dVar14 = *(double *)(lVar4 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x10 + lVar10);
    dVar15 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar11 + 8);
    dVar14 = *(double *)(lVar4 + 0x18) * dVar13 + *(double *)(lVar11 + 0x10) * dVar15 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x18 + lVar10);
    dVar16 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar5 + 8);
    dVar14 = *(double *)(lVar4 + 0x20) * dVar13 + *(double *)(lVar11 + 0x18) * dVar15 +
             *(double *)(lVar5 + 0x10) * dVar16 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x20 + lVar10);
    pdVar3 = (double *)(param_5 + 0x28 + lVar10);
    dVar17 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar6 + 8);
    dVar14 = *(double *)(lVar4 + 0x28) * dVar13 + *(double *)(lVar11 + 0x20) * dVar15 +
             *(double *)(lVar5 + 0x18) * dVar16 + *(double *)(lVar6 + 0x10) * dVar17 + *pdVar2;
    *pdVar2 = dVar14;
    dVar18 = (double)((ulong)dVar14 ^ uVar9) / *(double *)(lVar7 + 8);
    dVar12 = *(double *)(lVar4 + 0x30) * dVar13 + *(double *)(lVar11 + 0x28) * dVar15 +
             *(double *)(lVar5 + 0x20) * dVar16 + *(double *)(lVar6 + 0x18) * dVar17 +
             *(double *)(lVar7 + 0x10) * dVar18 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar8 + 8);
    if (6 < param_4) {
      lVar10 = 0;
      do {
        *(double *)((long)pdVar2 + lVar10 + 0x10) =
             *(double *)(lVar4 + 0x38 + lVar10) * dVar13 +
             *(double *)(lVar11 + 0x30 + lVar10) * dVar15 +
             *(double *)(lVar5 + 0x28 + lVar10) * dVar16 +
             *(double *)(lVar6 + 0x20 + lVar10) * dVar17 +
             *(double *)(lVar7 + 0x18 + lVar10) * dVar18 +
             *(double *)(lVar8 + 0x10 + lVar10) * ((double)(uVar9 ^ (ulong)dVar12) / dVar14) +
             *(double *)((long)pdVar2 + lVar10 + 0x10);
        lVar10 = lVar10 + 8;
      } while (lVar10 != (ulong)(param_4 - 7) * 8 + 8);
    }
  }
  return;
}


