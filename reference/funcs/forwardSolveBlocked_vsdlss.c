/* VSDLSS function (decompiled by Ghidra) */
/* name: forwardSolveBlocked_vsdlss  addr: 00a197c0  size: 4e4 */
#include "vsdlss_ref.h"

/* signature: void forwardSolveBlocked_vsdlss(long param_1,long param_2); */

void forwardSolveBlocked_vsdlss(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  double *pdVar4;
  double *pdVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  int iVar16;
  long lVar17;
  int iVar18;
  int iVar19;
  long lVar20;
  int iVar21;
  int iVar22;
  double dVar23;
  double dVar24;
  double dVar25;
  int *local_48;
  int local_40;
  
  lVar10 = *(long *)(param_1 + 0x30);
  iVar6 = *(int *)(param_1 + 8);
  local_48 = *(int **)(param_1 + 0x20);
  local_40 = 1;
  lVar11 = *(long *)(param_1 + 0x38);
  iVar7 = *(int *)(param_1 + 0x14);
  lVar12 = *(long *)(param_1 + 0x28);
  iVar18 = 1;
  if (0 < iVar7) {
    do {
      local_48 = local_48 + 1;
      iVar8 = *local_48;
      switch(iVar8) {
      default:
        iVar21 = iVar8 + -1;
        iVar22 = 1;
        if (0 < iVar8) {
          do {
            lVar17 = (long)iVar18;
            iVar19 = 6;
            if (iVar21 < 6) {
              iVar19 = iVar21 + 1;
            }
            iVar9 = *(int *)(lVar12 + lVar17 * 4);
            iVar16 = (iVar6 - iVar18) + 1;
            if (iVar16 == iVar9) {
              blockSaxpyBackSolveNI_vsdlss(iVar18,iVar19,lVar11 + -8 + lVar17 * 8,iVar16,param_2);
            }
            else {
              blockSaxpyBackSolve_vsdlss
                        (iVar18,iVar19,*(undefined8 *)(lVar10 + lVar17 * 8),lVar11 + -8 + lVar17 * 8
                         ,iVar9,param_2);
            }
            iVar22 = iVar22 + 6;
            iVar18 = iVar18 + iVar19;
            iVar21 = iVar21 + -6;
          } while (iVar22 <= iVar8);
        }
        break;
      case 1:
        lVar14 = (long)iVar18;
        lVar17 = *(long *)(lVar11 + lVar14 * 8);
        iVar21 = *(int *)(lVar12 + lVar14 * 4);
        iVar8 = (iVar6 - iVar18) + 1;
        dVar23 = (double)(*(ulong *)(param_2 + lVar14 * 8) ^ DAT_00abd200) / *(double *)(lVar17 + 8)
        ;
        if (iVar8 == iVar21) {
          lVar14 = param_2 + -8 + lVar14 * 8;
          if (1 < iVar8) {
            lVar15 = 0;
            do {
              *(double *)(lVar14 + 0x10 + lVar15) =
                   *(double *)(lVar17 + 0x10 + lVar15) * dVar23 +
                   *(double *)(lVar14 + 0x10 + lVar15);
              lVar15 = lVar15 + 8;
            } while (lVar15 != (ulong)((iVar6 - iVar18) - 1) * 8 + 8);
            iVar18 = iVar18 + 1;
            break;
          }
        }
        else {
          lVar14 = *(long *)(lVar10 + lVar14 * 8);
          if (1 < iVar21) {
            lVar15 = 0;
            do {
              pdVar4 = (double *)(param_2 + (long)*(int *)(lVar14 + 8 + lVar15) * 8);
              lVar13 = lVar15 * 2;
              lVar15 = lVar15 + 4;
              *pdVar4 = *(double *)(lVar17 + 0x10 + lVar13) * dVar23 + *pdVar4;
            } while (lVar15 != (ulong)(iVar21 - 2) * 4 + 4);
          }
        }
        iVar18 = iVar18 + 1;
        break;
      case 2:
        lVar13 = (long)iVar18;
        lVar17 = lVar13 * 8;
        lVar14 = *(long *)(lVar11 + lVar13 * 8);
        pdVar4 = (double *)(param_2 + 8 + lVar17);
        lVar15 = *(long *)(lVar11 + 8 + lVar17);
        iVar22 = iVar6 - (iVar18 + 1);
        dVar23 = (double)(*(ulong *)(param_2 + lVar13 * 8) ^ DAT_00abd200) / *(double *)(lVar14 + 8)
        ;
        dVar24 = *(double *)(lVar14 + 0x10) * dVar23 + *pdVar4;
        *pdVar4 = dVar24;
        iVar8 = iVar22 + 1;
        iVar21 = *(int *)(lVar12 + (long)(iVar18 + 1) * 4);
        dVar24 = (double)((ulong)dVar24 ^ DAT_00abd200) / *(double *)(lVar15 + 8);
        if (iVar8 == iVar21) {
          if (1 < iVar8) {
            lVar13 = 0;
            do {
              *(double *)(lVar17 + param_2 + 0x10 + lVar13) =
                   *(double *)(lVar14 + 0x18 + lVar13) * dVar23 +
                   *(double *)(lVar15 + 0x10 + lVar13) * dVar24 +
                   *(double *)(lVar17 + param_2 + 0x10 + lVar13);
              lVar13 = lVar13 + 8;
            } while (lVar13 != (ulong)(iVar22 - 1) * 8 + 8);
            iVar18 = iVar18 + 2;
            break;
          }
        }
        else {
          lVar17 = *(long *)(lVar10 + 8 + lVar17);
          if (1 < iVar21) {
            lVar13 = 0;
            do {
              pdVar4 = (double *)(param_2 + (long)*(int *)(lVar17 + 8 + lVar13) * 8);
              lVar20 = lVar13 * 2;
              lVar1 = lVar13 * 2;
              lVar13 = lVar13 + 4;
              *pdVar4 = *(double *)(lVar14 + 0x18 + lVar20) * dVar23 +
                        *(double *)(lVar15 + 0x10 + lVar1) * dVar24 + *pdVar4;
            } while (lVar13 != (ulong)(iVar21 - 2) * 4 + 4);
          }
        }
        iVar18 = iVar18 + 2;
        break;
      case 3:
        lVar13 = (long)iVar18;
        lVar17 = lVar13 * 8;
        pdVar4 = (double *)(param_2 + 8 + lVar17);
        lVar14 = *(long *)(lVar11 + lVar13 * 8);
        lVar15 = *(long *)(lVar11 + 8 + lVar17);
        pdVar5 = (double *)(param_2 + 0x10 + lVar17);
        dVar23 = (double)(*(ulong *)(param_2 + lVar13 * 8) ^ DAT_00abd200) / *(double *)(lVar14 + 8)
        ;
        dVar24 = *(double *)(lVar14 + 0x10) * dVar23 + *pdVar4;
        *pdVar4 = dVar24;
        lVar13 = *(long *)(lVar11 + 0x10 + lVar17);
        dVar24 = (double)((ulong)dVar24 ^ DAT_00abd200) / *(double *)(lVar15 + 8);
        dVar25 = *(double *)(lVar14 + 0x18) * dVar23 + *(double *)(lVar15 + 0x10) * dVar24 + *pdVar5
        ;
        *pdVar5 = dVar25;
        iVar21 = *(int *)(lVar12 + (long)(iVar18 + 2) * 4);
        iVar22 = iVar6 - (iVar18 + 2);
        iVar8 = iVar22 + 1;
        dVar25 = (double)((ulong)dVar25 ^ DAT_00abd200) / *(double *)(lVar13 + 8);
        if (iVar8 == iVar21) {
          if (1 < iVar8) {
            lVar17 = 0;
            do {
              *(double *)((long)pdVar4 + lVar17 + 0x10) =
                   *(double *)(lVar14 + 0x20 + lVar17) * dVar23 +
                   *(double *)(lVar15 + 0x18 + lVar17) * dVar24 +
                   *(double *)(lVar13 + 0x10 + lVar17) * dVar25 +
                   *(double *)((long)pdVar4 + lVar17 + 0x10);
              lVar17 = lVar17 + 8;
            } while (lVar17 != (ulong)(iVar22 - 1) * 8 + 8);
            iVar18 = iVar18 + 3;
            break;
          }
        }
        else {
          lVar17 = *(long *)(lVar10 + 0x10 + lVar17);
          if (1 < iVar21) {
            lVar20 = 0;
            do {
              pdVar4 = (double *)(param_2 + (long)*(int *)(lVar17 + 8 + lVar20) * 8);
              lVar1 = lVar20 * 2;
              lVar2 = lVar20 * 2;
              lVar3 = lVar20 * 2;
              lVar20 = lVar20 + 4;
              *pdVar4 = *(double *)(lVar14 + 0x20 + lVar1) * dVar23 +
                        *(double *)(lVar15 + 0x18 + lVar2) * dVar24 +
                        *(double *)(lVar13 + 0x10 + lVar3) * dVar25 + *pdVar4;
            } while (lVar20 != (ulong)(iVar21 - 2) * 4 + 4);
          }
        }
        iVar18 = iVar18 + 3;
        break;
      case 4:
      case 5:
      case 6:
        lVar17 = (long)iVar18;
        iVar21 = *(int *)(lVar12 + lVar17 * 4);
        iVar22 = (iVar6 - iVar18) + 1;
        if (iVar22 == iVar21) {
          blockSaxpyBackSolveNI_vsdlss(iVar18,iVar8,lVar11 + -8 + lVar17 * 8,iVar22,param_2);
        }
        else {
          blockSaxpyBackSolve_vsdlss
                    (iVar18,iVar8,*(undefined8 *)(lVar10 + lVar17 * 8),lVar11 + -8 + lVar17 * 8,
                     iVar21,param_2);
        }
        iVar18 = iVar18 + iVar8;
      }
      local_40 = local_40 + 1;
    } while (local_40 <= iVar7);
  }
  return;
}


