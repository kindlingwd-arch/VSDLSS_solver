/* VSDLSS function (decompiled by Ghidra) */
/* name: sub3RowContribFromIndRange_vsdlss  addr: 009f0a00  size: 32c */
#include "vsdlss_ref.h"

/* signature: void sub3RowContribFromIndRange_vsdlss(int param_1,int param_2,int param_3,long param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub3RowContribFromIndRange_vsdlss(int param_1,int param_2,int param_3,long param_4)

{
  long lVar1;
  long lVar2;
  int iVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  int iVar7;
  long lVar8;
  int iVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  int iVar15;
  int iVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  
  uVar4 = DAT_00abd200;
  lVar12 = (long)param_1;
  lVar1 = *(long *)(param_4 + 0x30);
  lVar2 = *(long *)(param_4 + 0x28);
  iVar16 = 4;
  if (3 < param_2) {
    iVar16 = param_2;
  }
  lVar8 = *(long *)(param_4 + 0x38);
  lVar10 = *(long *)(lVar8 + 8 + lVar12 * 8);
  lVar13 = *(long *)(lVar8 + lVar12 * 8);
  lVar8 = *(long *)(lVar8 + 0x10 + lVar12 * 8);
  iVar7 = *(int *)(lVar2 + lVar12 * 4);
  if (iVar7 <= param_3) {
    param_3 = iVar7;
  }
  if (param_3 < iVar16) {
    return;
  }
  lVar5 = (long)iVar16 * 8;
  dVar20 = _DAT_00ab67f8 / *(double *)(lVar8 + 8);
  dVar21 = _DAT_00ab67f8 / *(double *)(lVar10 + 8);
  dVar22 = _DAT_00ab67f8 / *(double *)(lVar13 + 8);
  lVar8 = lVar8 + -0x20 + lVar5;
  lVar12 = *(long *)(lVar1 + lVar12 * 8) + -8 + (long)iVar16 * 4;
  lVar10 = lVar10 + -0x18 + lVar5;
  lVar13 = lVar13 + -0x10 + lVar5;
  iVar7 = (iVar7 - iVar16) + 2;
  do {
    iVar9 = iVar7 + -1;
    iVar15 = *(int *)(lVar12 + 8);
    lVar5 = (long)iVar15;
    dVar17 = (double)(*(ulong *)(lVar13 + 0x10) ^ uVar4) * dVar22;
    dVar18 = (double)(*(ulong *)(lVar10 + 0x10) ^ uVar4) * dVar21;
    dVar19 = (double)(*(ulong *)(lVar8 + 0x10) ^ uVar4) * dVar20;
    if ((*(long *)(param_4 + 0x60) == 0) || (iVar15 <= *(int *)(param_4 + 0x68))) {
      iVar15 = *(int *)(lVar2 + lVar5 * 4);
      iVar3 = iVar9 - iVar15;
      lVar14 = *(long *)(*(long *)(param_4 + 0x38) + lVar5 * 8);
      if (iVar3 == 0) goto LAB_009f0bdd;
LAB_009f0b2f:
      if (SBORROW4(iVar9,iVar15) == iVar3 < 0) {
        nrerror_vsdlss(1,"factorapar_ls.c",0x27f);
      }
      else {
        lVar5 = *(long *)(lVar1 + lVar5 * 8);
        if ((iVar15 < 1) || (iVar9 < 1)) {
          iVar7 = 1;
        }
        else {
          lVar6 = 0;
          iVar7 = 1;
          do {
            lVar11 = (long)iVar7;
            if (*(int *)(lVar12 + 4 + lVar11 * 4) == *(int *)(lVar5 + 4 + lVar6 * 4)) {
              iVar7 = iVar7 + 1;
              *(double *)(lVar14 + 8 + lVar6 * 8) =
                   *(double *)(lVar13 + 8 + lVar11 * 8) * dVar17 +
                   *(double *)(lVar10 + 8 + lVar11 * 8) * dVar18 +
                   *(double *)(lVar8 + 8 + lVar11 * 8) * dVar19 +
                   *(double *)(lVar14 + 8 + lVar6 * 8);
            }
            iVar3 = (int)lVar6;
            lVar6 = lVar6 + 1;
          } while (iVar7 <= iVar9 && iVar3 + 2 <= iVar15);
        }
        if (iVar7 <= iVar9) {
          nrerror_vsdlss(0x25,"factorapar_ls.c",0x27a);
        }
      }
    }
    else {
      lVar14 = *(long *)(*(long *)(param_4 + 0x60) + (long)(iVar15 - *(int *)(param_4 + 0x68)) * 8);
      iVar15 = *(int *)(lVar2 + lVar5 * 4);
      iVar3 = iVar9 - iVar15;
      if (iVar9 != iVar15) goto LAB_009f0b2f;
LAB_009f0bdd:
      if (0 < iVar9) {
        lVar5 = 0;
        do {
          *(double *)(lVar14 + 8 + lVar5) =
               *(double *)(lVar13 + 0x10 + lVar5) * dVar17 +
               *(double *)(lVar10 + 0x10 + lVar5) * dVar18 +
               *(double *)(lVar8 + 0x10 + lVar5) * dVar19 + *(double *)(lVar14 + 8 + lVar5);
          lVar5 = lVar5 + 8;
        } while (lVar5 != (ulong)(iVar7 - 2) * 8 + 8);
      }
    }
    if (param_3 < iVar16 + 1) {
      return;
    }
    iVar16 = iVar16 + 1;
    lVar8 = lVar8 + 8;
    lVar12 = lVar12 + 4;
    lVar10 = lVar10 + 8;
    lVar13 = lVar13 + 8;
    iVar7 = iVar9;
  } while( true );
}


