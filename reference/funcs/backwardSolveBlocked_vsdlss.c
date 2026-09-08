/* VSDLSS function (decompiled by Ghidra) */
/* name: backwardSolveBlocked_vsdlss  addr: 00a19ce0  size: 52a */
#include "vsdlss_ref.h"

/* signature: void backwardSolveBlocked_vsdlss(long param_1,long param_2); */

void backwardSolveBlocked_vsdlss(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  double *pdVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  int iVar13;
  long lVar14;
  long lVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  long lVar20;
  long lVar21;
  long lVar22;
  long lVar23;
  double *pdVar24;
  int iVar25;
  double dVar26;
  double dVar27;
  double dVar28;
  double dVar29;
  int *local_60;
  
  lVar9 = *(long *)(param_1 + 0x30);
  iVar5 = *(int *)(param_1 + 0x14);
  iVar6 = *(int *)(param_1 + 8);
  lVar10 = *(long *)(param_1 + 0x38);
  lVar11 = *(long *)(param_1 + 0x20);
  lVar12 = *(long *)(param_1 + 0x28);
  if (0 < iVar5) {
    local_60 = (int *)(lVar11 + (long)iVar5 * 4);
    iVar16 = iVar6;
    do {
      iVar7 = *local_60;
      switch(iVar7) {
      default:
        if (0 < iVar7) {
          iVar19 = iVar7 + -1;
          iVar25 = 1;
          do {
            iVar18 = iVar19 + 1;
            if (5 < iVar19) {
              iVar18 = 6;
            }
            iVar17 = iVar16 - iVar18;
            lVar15 = (long)(iVar17 + 1);
            iVar8 = *(int *)(lVar12 + lVar15 * 4);
            iVar13 = (iVar6 - (iVar17 + 1)) + 1;
            if (iVar13 == iVar8) {
              blockInnerProductBackSolveNI_vsdlss
                        (iVar16,iVar18,lVar10 + -8 + lVar15 * 8,iVar13,param_2);
            }
            else {
              blockInnerProductBackSolve_vsdlss
                        (iVar16,iVar18,*(undefined8 *)(lVar9 + lVar15 * 8),lVar10 + -8 + lVar15 * 8,
                         iVar8,param_2);
            }
            iVar25 = iVar25 + 6;
            iVar19 = iVar19 + -6;
            iVar16 = iVar17;
          } while (iVar25 <= iVar7);
        }
        break;
      case 1:
        lVar20 = (long)iVar16;
        lVar15 = *(long *)(lVar10 + lVar20 * 8);
        iVar19 = *(int *)(lVar12 + lVar20 * 4);
        iVar7 = (iVar6 - iVar16) + 1;
        if (iVar7 == iVar19) {
          if (iVar7 < 2) goto LAB_00a1a22c;
          dVar28 = 0.0;
          lVar14 = 0;
          do {
            pdVar24 = (double *)(param_2 + lVar20 * 8 + 8 + lVar14);
            pdVar3 = (double *)(lVar15 + 0x10 + lVar14);
            lVar14 = lVar14 + 8;
            dVar28 = dVar28 + *pdVar24 * *pdVar3;
          } while (lVar14 != (ulong)((iVar6 - iVar16) - 1) * 8 + 8);
        }
        else if (iVar19 < 2) {
LAB_00a1a22c:
          dVar28 = 0.0;
        }
        else {
          dVar28 = 0.0;
          lVar14 = 0;
          do {
            piVar4 = (int *)(*(long *)(lVar9 + lVar20 * 8) + 8 + lVar14);
            lVar21 = lVar14 * 2;
            lVar14 = lVar14 + 4;
            dVar28 = dVar28 + *(double *)(param_2 + (long)*piVar4 * 8) *
                              *(double *)(lVar15 + 0x10 + lVar21);
          } while (lVar14 != (ulong)(iVar19 - 2) * 4 + 4);
        }
        pdVar24 = (double *)(lVar20 * 8 + param_2);
        iVar16 = iVar16 + -1;
        *pdVar24 = (*pdVar24 - dVar28) / *(double *)(lVar15 + 8);
        break;
      case 2:
        lVar21 = (long)iVar16;
        lVar15 = lVar21 * 8;
        lVar20 = *(long *)(lVar10 + lVar21 * 8);
        iVar19 = *(int *)(lVar12 + lVar21 * 4);
        iVar7 = (iVar6 - iVar16) + 1;
        lVar14 = *(long *)(lVar10 + -8 + lVar15);
        if (iVar7 == iVar19) {
          if (iVar7 < 2) goto LAB_00a1a21f;
          dVar29 = 0.0;
          lVar21 = 0;
          dVar28 = 0.0;
          do {
            dVar27 = *(double *)(lVar15 + param_2 + 8 + lVar21);
            pdVar24 = (double *)(lVar14 + 0x18 + lVar21);
            pdVar3 = (double *)(lVar20 + 0x10 + lVar21);
            lVar21 = lVar21 + 8;
            dVar28 = dVar28 + *pdVar24 * dVar27;
            dVar29 = dVar29 + dVar27 * *pdVar3;
          } while (lVar21 != (ulong)((iVar6 - iVar16) - 1) * 8 + 8);
        }
        else if (iVar19 < 2) {
LAB_00a1a21f:
          dVar29 = 0.0;
          dVar28 = dVar29;
        }
        else {
          dVar29 = 0.0;
          lVar22 = 0;
          dVar28 = 0.0;
          do {
            lVar23 = lVar22 * 2;
            dVar27 = *(double *)
                      (param_2 + (long)*(int *)(*(long *)(lVar9 + lVar21 * 8) + 8 + lVar22) * 8);
            lVar1 = lVar22 * 2;
            lVar22 = lVar22 + 4;
            dVar28 = dVar28 + *(double *)(lVar14 + 0x18 + lVar23) * dVar27;
            dVar29 = dVar29 + dVar27 * *(double *)(lVar20 + 0x10 + lVar1);
          } while (lVar22 != (ulong)(iVar19 - 2) * 4 + 4);
        }
        dVar29 = (*(double *)(lVar15 + param_2) - dVar29) / *(double *)(lVar20 + 8);
        iVar7 = iVar16 + -1;
        iVar16 = iVar16 + -2;
        pdVar24 = (double *)(param_2 + (long)iVar7 * 8);
        *(double *)(lVar15 + param_2) = dVar29;
        *pdVar24 = (*pdVar24 - (dVar29 * *(double *)(lVar14 + 0x10) + dVar28)) /
                   *(double *)(lVar14 + 8);
        break;
      case 3:
        lVar22 = (long)iVar16;
        lVar15 = lVar22 * 8;
        lVar20 = *(long *)(lVar10 + lVar22 * 8);
        iVar19 = *(int *)(lVar12 + lVar22 * 4);
        lVar14 = *(long *)(lVar10 + -0x10 + lVar15);
        iVar7 = (iVar6 - iVar16) + 1;
        lVar21 = *(long *)(lVar10 + -8 + lVar15);
        if (iVar7 == iVar19) {
          if (iVar7 < 2) goto LAB_00a1a20e;
          dVar27 = 0.0;
          lVar22 = 0;
          dVar28 = 0.0;
          dVar29 = 0.0;
          do {
            dVar26 = *(double *)(lVar15 + param_2 + 8 + lVar22);
            dVar29 = dVar29 + *(double *)(lVar14 + 0x20 + lVar22) * dVar26;
            pdVar24 = (double *)(lVar21 + 0x18 + lVar22);
            pdVar3 = (double *)(lVar20 + 0x10 + lVar22);
            lVar22 = lVar22 + 8;
            dVar28 = dVar28 + *pdVar24 * dVar26;
            dVar27 = dVar27 + dVar26 * *pdVar3;
          } while (lVar22 != (ulong)((iVar6 - iVar16) - 1) * 8 + 8);
        }
        else if (iVar19 < 2) {
LAB_00a1a20e:
          dVar27 = 0.0;
          dVar28 = dVar27;
          dVar29 = dVar27;
        }
        else {
          dVar27 = 0.0;
          lVar23 = 0;
          dVar28 = 0.0;
          dVar29 = 0.0;
          do {
            dVar26 = *(double *)
                      (param_2 + (long)*(int *)(*(long *)(lVar9 + lVar22 * 8) + 8 + lVar23) * 8);
            dVar29 = dVar29 + *(double *)(lVar14 + 0x20 + lVar23 * 2) * dVar26;
            lVar1 = lVar23 * 2;
            lVar2 = lVar23 * 2;
            lVar23 = lVar23 + 4;
            dVar28 = dVar28 + *(double *)(lVar21 + 0x18 + lVar1) * dVar26;
            dVar27 = dVar27 + dVar26 * *(double *)(lVar20 + 0x10 + lVar2);
          } while (lVar23 != (ulong)(iVar19 - 2) * 4 + 4);
        }
        dVar26 = (*(double *)(lVar15 + param_2) - dVar27) / *(double *)(lVar20 + 8);
        iVar7 = iVar16 + -1;
        iVar16 = iVar16 + -3;
        lVar20 = (long)iVar7 * 8;
        *(double *)(lVar15 + param_2) = dVar26;
        pdVar24 = (double *)(param_2 + lVar20);
        dVar27 = *(double *)(lVar14 + 0x18);
        dVar28 = (*pdVar24 - (dVar26 * *(double *)(lVar21 + 0x10) + dVar28)) /
                 *(double *)(lVar21 + 8);
        pdVar3 = (double *)(param_2 + -8 + lVar20);
        *pdVar24 = dVar28;
        *pdVar3 = (*pdVar3 - (dVar28 * *(double *)(lVar14 + 0x10) + dVar27 * dVar26 + dVar29)) /
                  *(double *)(lVar14 + 8);
        break;
      case 4:
      case 5:
      case 6:
        iVar18 = iVar16 - iVar7;
        lVar15 = (long)(iVar18 + 1);
        iVar19 = *(int *)(lVar12 + lVar15 * 4);
        iVar25 = (iVar6 - (iVar18 + 1)) + 1;
        if (iVar25 == iVar19) {
          blockInnerProductBackSolveNI_vsdlss(iVar16,iVar7,lVar10 + -8 + lVar15 * 8,iVar25,param_2);
          iVar16 = iVar18;
        }
        else {
          blockInnerProductBackSolve_vsdlss
                    (iVar16,iVar7,*(undefined8 *)(lVar9 + lVar15 * 8),lVar10 + -8 + lVar15 * 8,
                     iVar19,param_2);
          iVar16 = iVar18;
        }
      }
      local_60 = local_60 + -1;
    } while (local_60 != (int *)(lVar11 + -4 + (long)iVar5 * 4 + (ulong)(iVar5 - 1) * -4));
  }
  return;
}


