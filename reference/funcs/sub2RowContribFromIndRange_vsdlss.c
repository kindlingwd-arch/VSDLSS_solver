/* VSDLSS function (decompiled by Ghidra) */
/* name: sub2RowContribFromIndRange_vsdlss  addr: 009f0150  size: 2c6 */
#include "vsdlss_ref.h"

/* signature: void sub2RowContribFromIndRange_vsdlss(int param_1,int param_2,int param_3,long param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub2RowContribFromIndRange_vsdlss(int param_1,int param_2,int param_3,long param_4)

{
  int iVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  long lVar9;
  int iVar10;
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
  
  uVar5 = DAT_00abd200;
  lVar13 = (long)param_1;
  lVar2 = *(long *)(param_4 + 0x30);
  iVar16 = 3;
  if (2 < param_2) {
    iVar16 = param_2;
  }
  lVar3 = *(long *)(param_4 + 0x28);
  lVar11 = *(long *)(*(long *)(param_4 + 0x38) + lVar13 * 8);
  lVar9 = *(long *)(*(long *)(param_4 + 0x38) + 8 + lVar13 * 8);
  iVar8 = *(int *)(lVar3 + lVar13 * 4);
  if (iVar8 <= param_3) {
    param_3 = iVar8;
  }
  if (param_3 < iVar16) {
    return;
  }
  lVar6 = (long)iVar16 * 8;
  dVar19 = _DAT_00ab67f8 / *(double *)(lVar9 + 8);
  dVar20 = _DAT_00ab67f8 / *(double *)(lVar11 + 8);
  lVar9 = lVar9 + -0x18 + lVar6;
  lVar13 = *(long *)(lVar2 + lVar13 * 8) + -8 + (long)iVar16 * 4;
  lVar11 = lVar11 + -0x10 + lVar6;
  iVar8 = (iVar8 - iVar16) + 2;
  do {
    iVar10 = iVar8 + -1;
    iVar1 = *(int *)(lVar13 + 8);
    lVar6 = (long)iVar1;
    dVar17 = (double)(*(ulong *)(lVar11 + 0x10) ^ uVar5) * dVar20;
    dVar18 = (double)(*(ulong *)(lVar9 + 0x10) ^ uVar5) * dVar19;
    if ((*(long *)(param_4 + 0x60) == 0) || (iVar1 <= *(int *)(param_4 + 0x68))) {
      iVar15 = *(int *)(lVar3 + lVar6 * 4);
      iVar4 = iVar10 - iVar15;
      lVar14 = *(long *)(*(long *)(param_4 + 0x38) + lVar6 * 8);
      if (iVar4 == 0) goto LAB_009f02f5;
LAB_009f0254:
      if (SBORROW4(iVar10,iVar15) == iVar4 < 0) {
        nrerror_vsdlss(1,"factorapar_ls.c",0x1a9);
      }
      else {
        lVar6 = *(long *)(lVar2 + lVar6 * 8);
        if ((iVar15 < 1) || (iVar10 < 1)) {
          iVar8 = 1;
        }
        else {
          lVar7 = 0;
          iVar8 = 1;
          do {
            lVar12 = (long)iVar8;
            if (*(int *)(lVar13 + 4 + lVar12 * 4) == *(int *)(lVar6 + 4 + lVar7 * 4)) {
              iVar8 = iVar8 + 1;
              *(double *)(lVar14 + 8 + lVar7 * 8) =
                   *(double *)(lVar11 + 8 + lVar12 * 8) * dVar17 +
                   *(double *)(lVar9 + 8 + lVar12 * 8) * dVar18 +
                   *(double *)(lVar14 + 8 + lVar7 * 8);
            }
            iVar1 = (int)lVar7;
            lVar7 = lVar7 + 1;
          } while (iVar8 <= iVar10 && iVar1 + 2 <= iVar15);
        }
        if (iVar8 <= iVar10) {
          nrerror_vsdlss(0x25,"factorapar_ls.c",0x1a4);
        }
      }
    }
    else {
      iVar15 = *(int *)(lVar3 + lVar6 * 4);
      iVar4 = iVar10 - iVar15;
      lVar14 = *(long *)(*(long *)(param_4 + 0x60) + (long)(iVar1 - *(int *)(param_4 + 0x68)) * 8);
      if (iVar10 != iVar15) goto LAB_009f0254;
LAB_009f02f5:
      if (0 < iVar10) {
        lVar6 = 0;
        do {
          *(double *)(lVar14 + 8 + lVar6) =
               *(double *)(lVar11 + 0x10 + lVar6) * dVar17 +
               *(double *)(lVar9 + 0x10 + lVar6) * dVar18 + *(double *)(lVar14 + 8 + lVar6);
          lVar6 = lVar6 + 8;
        } while (lVar6 != (ulong)(iVar8 - 2) * 8 + 8);
      }
    }
    if (param_3 < iVar16 + 1) {
      return;
    }
    iVar16 = iVar16 + 1;
    lVar9 = lVar9 + 8;
    lVar13 = lVar13 + 4;
    lVar11 = lVar11 + 8;
    iVar8 = iVar10;
  } while( true );
}


