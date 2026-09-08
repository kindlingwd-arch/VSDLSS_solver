/* driver: zIL_HqyUMVZy_HqLISBlgXSPAIPP addr=009adc20 size=64f */
#include "vsdlss_ref.h"


void zIL_HqyUMVZy_HqLISBlgXSPAIPP
               (long param_1,undefined8 param_2,long param_3,long param_4,long param_5)

{
  ulong *puVar1;
  double *pdVar2;
  double *pdVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  
  uVar8 = DAT_00abd200;
  switch(param_2) {
  default:
    KOBOOLOXSPAIPP(0xf,"triangularsolves.c",0x177,6,"blockSaxpyBackSolveNI");
    return;
  case 1:
    lVar4 = *(long *)(param_3 + 8);
    lVar9 = param_5 + -8 + param_1 * 8;
    lVar10 = 2;
    dVar12 = (double)(*(ulong *)(param_5 + param_1 * 8) ^ DAT_00abd200);
    dVar14 = *(double *)(lVar4 + 8);
    if (1 < param_4) {
      do {
        *(double *)(lVar9 + lVar10 * 8) =
             *(double *)(lVar4 + lVar10 * 8) * (dVar12 / dVar14) + *(double *)(lVar9 + lVar10 * 8);
        lVar10 = lVar10 + 1;
      } while (lVar10 != param_4 + 1);
      return;
    }
    break;
  case 2:
    lVar9 = *(long *)(param_3 + 8);
    lVar4 = *(long *)(param_3 + 0x10);
    puVar1 = (ulong *)(param_5 + param_1 * 8);
    pdVar2 = (double *)(param_5 + 8 + param_1 * 8);
    dVar12 = (double)(*puVar1 ^ DAT_00abd200) / *(double *)(lVar9 + 8);
    dVar14 = *(double *)(lVar9 + 0x10) * dVar12 + *pdVar2;
    dVar13 = (double)(DAT_00abd200 ^ (ulong)dVar14);
    *pdVar2 = dVar14;
    dVar14 = *(double *)(lVar4 + 8);
    if (2 < param_4) {
      lVar10 = 2;
      do {
        puVar1[lVar10] =
             (ulong)(*(double *)(lVar9 + 8 + lVar10 * 8) * dVar12 +
                     *(double *)(lVar4 + lVar10 * 8) * (dVar13 / dVar14) + (double)puVar1[lVar10]);
        lVar10 = lVar10 + 1;
      } while (lVar10 != param_4);
      return;
    }
    break;
  case 3:
    lVar9 = *(long *)(param_3 + 8);
    lVar4 = *(long *)(param_3 + 0x10);
    lVar10 = *(long *)(param_3 + 0x18);
    pdVar2 = (double *)(param_5 + 8 + param_1 * 8);
    pdVar3 = (double *)(param_5 + 0x10 + param_1 * 8);
    dVar13 = (double)(*(ulong *)(param_5 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar9 + 8);
    dVar14 = *(double *)(lVar9 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    dVar15 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar4 + 8);
    dVar12 = *(double *)(lVar9 + 0x18) * dVar13 + *(double *)(lVar4 + 0x10) * dVar15 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar10 + 8);
    if (3 < param_4) {
      lVar11 = 2;
      do {
        pdVar2[lVar11] =
             *(double *)(lVar9 + 0x10 + lVar11 * 8) * dVar13 +
             *(double *)(lVar4 + 8 + lVar11 * 8) * dVar15 +
             *(double *)(lVar10 + lVar11 * 8) * ((double)(uVar8 ^ (ulong)dVar12) / dVar14) +
             pdVar2[lVar11];
        lVar11 = lVar11 + 1;
      } while (lVar11 != param_4 + -1);
      return;
    }
    break;
  case 4:
    lVar4 = *(long *)(param_3 + 8);
    lVar9 = param_1 * 8;
    lVar10 = *(long *)(param_3 + 0x10);
    lVar11 = *(long *)(param_3 + 0x18);
    lVar5 = *(long *)(param_3 + 0x20);
    pdVar2 = (double *)(param_5 + 8 + lVar9);
    dVar13 = (double)(*(ulong *)(param_5 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar4 + 8);
    dVar14 = *(double *)(lVar4 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x10 + lVar9);
    pdVar3 = (double *)(param_5 + 0x18 + lVar9);
    dVar15 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar10 + 8);
    dVar14 = *(double *)(lVar4 + 0x18) * dVar13 + *(double *)(lVar10 + 0x10) * dVar15 + *pdVar2;
    *pdVar2 = dVar14;
    dVar16 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar11 + 8);
    dVar12 = *(double *)(lVar4 + 0x20) * dVar13 + *(double *)(lVar10 + 0x18) * dVar15 +
             *(double *)(lVar11 + 0x10) * dVar16 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar5 + 8);
    if (4 < param_4) {
      lVar9 = 2;
      do {
        pdVar2[lVar9] =
             *(double *)(lVar4 + 0x18 + lVar9 * 8) * dVar13 +
             *(double *)(lVar10 + 0x10 + lVar9 * 8) * dVar15 +
             *(double *)(lVar11 + 8 + lVar9 * 8) * dVar16 +
             *(double *)(lVar5 + lVar9 * 8) * ((double)(uVar8 ^ (ulong)dVar12) / dVar14) +
             pdVar2[lVar9];
        lVar9 = lVar9 + 1;
      } while (lVar9 != param_4 + -2);
      return;
    }
    break;
  case 5:
    lVar4 = *(long *)(param_3 + 8);
    lVar9 = param_1 * 8;
    lVar10 = *(long *)(param_3 + 0x10);
    lVar11 = *(long *)(param_3 + 0x18);
    lVar5 = *(long *)(param_3 + 0x20);
    lVar6 = *(long *)(param_3 + 0x28);
    pdVar2 = (double *)(param_5 + 8 + lVar9);
    dVar13 = (double)(*(ulong *)(param_5 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar4 + 8);
    dVar14 = *(double *)(lVar4 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x10 + lVar9);
    dVar15 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar10 + 8);
    dVar14 = *(double *)(lVar4 + 0x18) * dVar13 + *(double *)(lVar10 + 0x10) * dVar15 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x18 + lVar9);
    pdVar3 = (double *)(param_5 + 0x20 + lVar9);
    dVar16 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar11 + 8);
    dVar14 = *(double *)(lVar4 + 0x20) * dVar13 + *(double *)(lVar10 + 0x18) * dVar15 +
             *(double *)(lVar11 + 0x10) * dVar16 + *pdVar2;
    *pdVar2 = dVar14;
    dVar17 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar5 + 8);
    dVar12 = *(double *)(lVar4 + 0x28) * dVar13 + *(double *)(lVar10 + 0x20) * dVar15 +
             *(double *)(lVar11 + 0x18) * dVar16 + *(double *)(lVar5 + 0x10) * dVar17 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar6 + 8);
    if (5 < param_4) {
      lVar9 = 2;
      do {
        pdVar2[lVar9] =
             *(double *)(lVar4 + 0x20 + lVar9 * 8) * dVar13 +
             *(double *)(lVar10 + 0x18 + lVar9 * 8) * dVar15 +
             *(double *)(lVar11 + 0x10 + lVar9 * 8) * dVar16 +
             *(double *)(lVar5 + 8 + lVar9 * 8) * dVar17 +
             *(double *)(lVar6 + lVar9 * 8) * ((double)(uVar8 ^ (ulong)dVar12) / dVar14) +
             pdVar2[lVar9];
        lVar9 = lVar9 + 1;
      } while (lVar9 != param_4 + -3);
      return;
    }
    break;
  case 6:
    lVar4 = *(long *)(param_3 + 8);
    lVar9 = param_1 * 8;
    lVar10 = *(long *)(param_3 + 0x10);
    lVar11 = *(long *)(param_3 + 0x18);
    lVar5 = *(long *)(param_3 + 0x20);
    lVar6 = *(long *)(param_3 + 0x28);
    lVar7 = *(long *)(param_3 + 0x30);
    pdVar2 = (double *)(param_5 + 8 + lVar9);
    dVar13 = (double)(*(ulong *)(param_5 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar4 + 8);
    dVar14 = *(double *)(lVar4 + 0x10) * dVar13 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x10 + lVar9);
    dVar15 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar10 + 8);
    dVar14 = *(double *)(lVar4 + 0x18) * dVar13 + *(double *)(lVar10 + 0x10) * dVar15 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x18 + lVar9);
    dVar16 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar11 + 8);
    dVar14 = *(double *)(lVar4 + 0x20) * dVar13 + *(double *)(lVar10 + 0x18) * dVar15 +
             *(double *)(lVar11 + 0x10) * dVar16 + *pdVar2;
    *pdVar2 = dVar14;
    pdVar2 = (double *)(param_5 + 0x20 + lVar9);
    pdVar3 = (double *)(param_5 + 0x28 + lVar9);
    dVar17 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar5 + 8);
    dVar14 = *(double *)(lVar4 + 0x28) * dVar13 + *(double *)(lVar10 + 0x20) * dVar15 +
             *(double *)(lVar11 + 0x18) * dVar16 + *(double *)(lVar5 + 0x10) * dVar17 + *pdVar2;
    *pdVar2 = dVar14;
    dVar18 = (double)((ulong)dVar14 ^ uVar8) / *(double *)(lVar6 + 8);
    dVar12 = *(double *)(lVar4 + 0x30) * dVar13 + *(double *)(lVar10 + 0x28) * dVar15 +
             *(double *)(lVar11 + 0x20) * dVar16 + *(double *)(lVar5 + 0x18) * dVar17 +
             *(double *)(lVar6 + 0x10) * dVar18 + *pdVar3;
    *pdVar3 = dVar12;
    dVar14 = *(double *)(lVar7 + 8);
    if (6 < param_4) {
      lVar9 = 2;
      do {
        pdVar2[lVar9] =
             *(double *)(lVar4 + 0x28 + lVar9 * 8) * dVar13 +
             *(double *)(lVar10 + 0x20 + lVar9 * 8) * dVar15 +
             *(double *)(lVar11 + 0x18 + lVar9 * 8) * dVar16 +
             *(double *)(lVar5 + 0x10 + lVar9 * 8) * dVar17 +
             *(double *)(lVar6 + 8 + lVar9 * 8) * dVar18 +
             *(double *)(lVar7 + lVar9 * 8) * ((double)(uVar8 ^ (ulong)dVar12) / dVar14) +
             pdVar2[lVar9];
        lVar9 = lVar9 + 1;
      } while (lVar9 != param_4 + -4);
    }
  }
  return;
}


