/* driver: zIL_HqyUMVZy_HqLISBXSPAIPP addr=009ad580 size=676 */
#include "vsdlss_ref.h"


void zIL_HqyUMVZy_HqLISBXSPAIPP
               (long param_1,undefined8 param_2,long param_3,long param_4,long param_5,long param_6)

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
  ulong uVar12;
  long lVar13;
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
  
  uVar12 = DAT_00abd200;
  switch(param_2) {
  default:
    KOBOOLOXSPAIPP(0xf,"triangularsolves.c",0xcf,6,"blockSaxpyBackSolve");
    return;
  case 1:
    lVar13 = *(long *)(param_4 + 8);
    lVar14 = 2;
    dVar17 = (double)(*(ulong *)(param_6 + param_1 * 8) ^ DAT_00abd200);
    dVar19 = *(double *)(lVar13 + 8);
    if (1 < param_5) {
      do {
        pdVar7 = (double *)(param_6 + *(long *)(param_3 + lVar14 * 8) * 8);
        lVar15 = lVar14 * 8;
        lVar14 = lVar14 + 1;
        *pdVar7 = *(double *)(lVar13 + lVar15) * (dVar17 / dVar19) + *pdVar7;
      } while (lVar14 != param_5 + 1);
      return;
    }
    break;
  case 2:
    lVar13 = *(long *)(param_4 + 8);
    pdVar7 = (double *)(param_6 + 8 + param_1 * 8);
    lVar14 = *(long *)(param_4 + 0x10);
    dVar17 = (double)(*(ulong *)(param_6 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar13 + 8);
    dVar19 = *(double *)(lVar13 + 0x10) * dVar17 + *pdVar7;
    dVar18 = (double)(DAT_00abd200 ^ (ulong)dVar19);
    *pdVar7 = dVar19;
    dVar19 = *(double *)(lVar14 + 8);
    if (2 < param_5) {
      lVar15 = 2;
      do {
        pdVar7 = (double *)(param_6 + *(long *)(param_3 + 8 + lVar15 * 8) * 8);
        lVar9 = lVar15 * 8;
        lVar16 = lVar15 * 8;
        lVar15 = lVar15 + 1;
        *pdVar7 = *(double *)(lVar13 + 8 + lVar9) * dVar17 +
                  *(double *)(lVar14 + lVar16) * (dVar18 / dVar19) + *pdVar7;
      } while (lVar15 != param_5);
      return;
    }
    break;
  case 3:
    lVar13 = *(long *)(param_4 + 8);
    lVar14 = *(long *)(param_4 + 0x10);
    lVar15 = *(long *)(param_4 + 0x18);
    pdVar7 = (double *)(param_6 + 8 + param_1 * 8);
    pdVar8 = (double *)(param_6 + 0x10 + param_1 * 8);
    dVar18 = (double)(*(ulong *)(param_6 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar13 + 8);
    dVar19 = *(double *)(lVar13 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    dVar20 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar14 + 8);
    dVar17 = *(double *)(lVar13 + 0x18) * dVar18 + *(double *)(lVar14 + 0x10) * dVar20 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar15 + 8);
    if (3 < param_5) {
      lVar16 = 2;
      do {
        pdVar7 = (double *)(param_6 + *(long *)(param_3 + 0x10 + lVar16 * 8) * 8);
        lVar10 = lVar16 * 8;
        lVar11 = lVar16 * 8;
        lVar9 = lVar16 * 8;
        lVar16 = lVar16 + 1;
        *pdVar7 = *(double *)(lVar13 + 0x10 + lVar10) * dVar18 +
                  *(double *)(lVar14 + 8 + lVar11) * dVar20 +
                  *(double *)(lVar15 + lVar9) * ((double)(uVar12 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar16 != param_5 + -1);
      return;
    }
    break;
  case 4:
    lVar14 = *(long *)(param_4 + 8);
    lVar13 = param_1 * 8;
    lVar15 = *(long *)(param_4 + 0x10);
    lVar16 = *(long *)(param_4 + 0x18);
    lVar9 = *(long *)(param_4 + 0x20);
    pdVar7 = (double *)(param_6 + 8 + lVar13);
    dVar18 = (double)(*(ulong *)(param_6 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar14 + 8);
    dVar19 = *(double *)(lVar14 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x10 + lVar13);
    pdVar8 = (double *)(param_6 + 0x18 + lVar13);
    dVar20 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar15 + 8);
    dVar19 = *(double *)(lVar14 + 0x18) * dVar18 + *(double *)(lVar15 + 0x10) * dVar20 + *pdVar7;
    *pdVar7 = dVar19;
    dVar21 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar16 + 8);
    dVar17 = *(double *)(lVar14 + 0x20) * dVar18 + *(double *)(lVar15 + 0x18) * dVar20 +
             *(double *)(lVar16 + 0x10) * dVar21 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar9 + 8);
    if (4 < param_5) {
      lVar13 = 2;
      do {
        pdVar7 = (double *)(param_6 + *(long *)(param_3 + 0x18 + lVar13 * 8) * 8);
        lVar11 = lVar13 * 8;
        lVar1 = lVar13 * 8;
        lVar2 = lVar13 * 8;
        lVar10 = lVar13 * 8;
        lVar13 = lVar13 + 1;
        *pdVar7 = *(double *)(lVar14 + 0x18 + lVar11) * dVar18 +
                  *(double *)(lVar15 + 0x10 + lVar1) * dVar20 +
                  *(double *)(lVar16 + 8 + lVar2) * dVar21 +
                  *(double *)(lVar9 + lVar10) * ((double)(uVar12 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar13 != param_5 + -2);
      return;
    }
    break;
  case 5:
    lVar14 = *(long *)(param_4 + 8);
    lVar13 = param_1 * 8;
    lVar15 = *(long *)(param_4 + 0x10);
    lVar16 = *(long *)(param_4 + 0x18);
    lVar9 = *(long *)(param_4 + 0x20);
    lVar10 = *(long *)(param_4 + 0x28);
    pdVar7 = (double *)(param_6 + 8 + lVar13);
    dVar18 = (double)(*(ulong *)(param_6 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar14 + 8);
    dVar19 = *(double *)(lVar14 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x10 + lVar13);
    dVar20 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar15 + 8);
    dVar19 = *(double *)(lVar14 + 0x18) * dVar18 + *(double *)(lVar15 + 0x10) * dVar20 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x18 + lVar13);
    pdVar8 = (double *)(param_6 + 0x20 + lVar13);
    dVar21 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar16 + 8);
    dVar19 = *(double *)(lVar14 + 0x20) * dVar18 + *(double *)(lVar15 + 0x18) * dVar20 +
             *(double *)(lVar16 + 0x10) * dVar21 + *pdVar7;
    *pdVar7 = dVar19;
    dVar22 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar9 + 8);
    dVar17 = *(double *)(lVar14 + 0x28) * dVar18 + *(double *)(lVar15 + 0x20) * dVar20 +
             *(double *)(lVar16 + 0x18) * dVar21 + *(double *)(lVar9 + 0x10) * dVar22 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar10 + 8);
    if (5 < param_5) {
      lVar13 = 2;
      do {
        pdVar7 = (double *)(param_6 + *(long *)(param_3 + 0x20 + lVar13 * 8) * 8);
        lVar11 = lVar13 * 8;
        lVar1 = lVar13 * 8;
        lVar2 = lVar13 * 8;
        lVar3 = lVar13 * 8;
        lVar4 = lVar13 * 8;
        lVar13 = lVar13 + 1;
        *pdVar7 = *(double *)(lVar14 + 0x20 + lVar11) * dVar18 +
                  *(double *)(lVar15 + 0x18 + lVar1) * dVar20 +
                  *(double *)(lVar16 + 0x10 + lVar2) * dVar21 +
                  *(double *)(lVar9 + 8 + lVar3) * dVar22 +
                  *(double *)(lVar10 + lVar4) * ((double)(uVar12 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar13 != param_5 + -3);
      return;
    }
    break;
  case 6:
    lVar14 = *(long *)(param_4 + 8);
    lVar13 = param_1 * 8;
    lVar15 = *(long *)(param_4 + 0x10);
    lVar16 = *(long *)(param_4 + 0x18);
    lVar9 = *(long *)(param_4 + 0x20);
    lVar10 = *(long *)(param_4 + 0x28);
    lVar11 = *(long *)(param_4 + 0x30);
    pdVar7 = (double *)(param_6 + 8 + lVar13);
    dVar18 = (double)(*(ulong *)(param_6 + param_1 * 8) ^ DAT_00abd200) / *(double *)(lVar14 + 8);
    dVar19 = *(double *)(lVar14 + 0x10) * dVar18 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x10 + lVar13);
    dVar20 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar15 + 8);
    dVar19 = *(double *)(lVar14 + 0x18) * dVar18 + *(double *)(lVar15 + 0x10) * dVar20 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x18 + lVar13);
    dVar21 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar16 + 8);
    dVar19 = *(double *)(lVar14 + 0x20) * dVar18 + *(double *)(lVar15 + 0x18) * dVar20 +
             *(double *)(lVar16 + 0x10) * dVar21 + *pdVar7;
    *pdVar7 = dVar19;
    pdVar7 = (double *)(param_6 + 0x20 + lVar13);
    pdVar8 = (double *)(param_6 + 0x28 + lVar13);
    dVar22 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar9 + 8);
    dVar19 = *(double *)(lVar14 + 0x28) * dVar18 + *(double *)(lVar15 + 0x20) * dVar20 +
             *(double *)(lVar16 + 0x18) * dVar21 + *(double *)(lVar9 + 0x10) * dVar22 + *pdVar7;
    *pdVar7 = dVar19;
    dVar23 = (double)((ulong)dVar19 ^ uVar12) / *(double *)(lVar10 + 8);
    dVar17 = *(double *)(lVar14 + 0x30) * dVar18 + *(double *)(lVar15 + 0x28) * dVar20 +
             *(double *)(lVar16 + 0x20) * dVar21 + *(double *)(lVar9 + 0x18) * dVar22 +
             *(double *)(lVar10 + 0x10) * dVar23 + *pdVar8;
    *pdVar8 = dVar17;
    dVar19 = *(double *)(lVar11 + 8);
    if (6 < param_5) {
      lVar13 = 2;
      do {
        pdVar7 = (double *)(param_6 + *(long *)(param_3 + 0x28 + lVar13 * 8) * 8);
        lVar2 = lVar13 * 8;
        lVar3 = lVar13 * 8;
        lVar4 = lVar13 * 8;
        lVar5 = lVar13 * 8;
        lVar6 = lVar13 * 8;
        lVar1 = lVar13 * 8;
        lVar13 = lVar13 + 1;
        *pdVar7 = *(double *)(lVar14 + 0x28 + lVar2) * dVar18 +
                  *(double *)(lVar15 + 0x20 + lVar3) * dVar20 +
                  *(double *)(lVar16 + 0x18 + lVar4) * dVar21 +
                  *(double *)(lVar9 + 0x10 + lVar5) * dVar22 +
                  *(double *)(lVar10 + 8 + lVar6) * dVar23 +
                  *(double *)(lVar11 + lVar1) * ((double)(uVar12 ^ (ulong)dVar17) / dVar19) +
                  *pdVar7;
      } while (lVar13 != param_5 + -4);
    }
  }
  return;
}


