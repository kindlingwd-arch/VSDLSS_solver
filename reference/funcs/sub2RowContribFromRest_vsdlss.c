/* VSDLSS function (decompiled by Ghidra) */
/* name: sub2RowContribFromRest_vsdlss  addr: 00a175f0  size: 35e */
#include "vsdlss_ref.h"

/* signature: void sub2RowContribFromRest_vsdlss(int param_1,long param_2); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub2RowContribFromRest_vsdlss(int param_1,long param_2)

{
  long lVar1;
  long lVar2;
  int iVar3;
  double dVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  int iVar10;
  long lVar11;
  int iVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  
  lVar6 = (long)param_1;
  lVar1 = *(long *)(param_2 + 0x30);
  lVar2 = *(long *)(param_2 + 0x28);
  lVar14 = *(long *)(*(long *)(param_2 + 0x38) + lVar6 * 8);
  lVar15 = *(long *)(lVar1 + lVar6 * 8);
  lVar13 = *(long *)(*(long *)(param_2 + 0x38) + 8 + lVar6 * 8);
  iVar9 = *(int *)(lVar2 + lVar6 * 4);
  dVar19 = *(double *)(lVar14 + 8);
  if (dVar19 <= 0.0) {
    nrerror_vsdlss(0x23,"vsdlss1memutils.c",0x4fb,param_1);
    dVar19 = *(double *)(lVar14 + 8);
  }
  uVar5 = DAT_00abd200;
  dVar4 = _DAT_00ab67f8;
  iVar10 = iVar9 + -1;
  dVar19 = _DAT_00ab67f8 / dVar19;
  dVar16 = (double)(*(ulong *)(lVar14 + 0x10) ^ DAT_00abd200);
  if (0 < iVar10) {
    lVar6 = 0;
    do {
      *(double *)(lVar13 + 8 + lVar6) =
           *(double *)(lVar14 + 0x10 + lVar6) * dVar16 * dVar19 + *(double *)(lVar13 + 8 + lVar6);
      lVar6 = lVar6 + 8;
    } while (lVar6 != (ulong)(iVar9 - 2) * 8 + 8);
  }
  dVar16 = *(double *)(lVar13 + 8);
  if (dVar16 <= 0.0) {
    nrerror_vsdlss(0x23,"vsdlss1memutils.c",0x508,param_1 + 1);
    dVar16 = *(double *)(lVar13 + 8);
  }
  lVar14 = lVar14 + 8;
  lVar15 = lVar15 + 4;
  if (1 < iVar10) {
    do {
      iVar10 = iVar10 + -1;
      iVar9 = *(int *)(lVar15 + 8);
      lVar6 = (long)iVar9;
      dVar17 = (double)(*(ulong *)(lVar14 + 0x10) ^ uVar5) * dVar19;
      dVar18 = (double)(*(ulong *)(lVar13 + 0x10) ^ uVar5) * (dVar4 / dVar16);
      if ((*(long *)(param_2 + 0x60) == 0) || (iVar9 <= *(int *)(param_2 + 0x68))) {
        iVar12 = *(int *)(lVar2 + lVar6 * 4);
        iVar9 = iVar10 - iVar12;
        lVar11 = *(long *)(*(long *)(param_2 + 0x38) + lVar6 * 8);
        if (iVar9 == 0) goto LAB_00a177b4;
LAB_00a17720:
        if (SBORROW4(iVar10,iVar12) == iVar9 < 0) {
          nrerror_vsdlss(1,"vsdlss1memutils.c",0x52f);
        }
        else {
          lVar6 = *(long *)(lVar1 + lVar6 * 8);
          if ((iVar12 < 1) || (iVar10 < 1)) {
            iVar9 = 1;
          }
          else {
            lVar7 = 0;
            iVar9 = 1;
            do {
              lVar8 = (long)iVar9;
              if (*(int *)(lVar15 + 4 + lVar8 * 4) == *(int *)(lVar6 + 4 + lVar7 * 4)) {
                iVar9 = iVar9 + 1;
                *(double *)(lVar11 + 8 + lVar7 * 8) =
                     *(double *)(lVar14 + 8 + lVar8 * 8) * dVar17 +
                     *(double *)(lVar13 + 8 + lVar8 * 8) * dVar18 +
                     *(double *)(lVar11 + 8 + lVar7 * 8);
              }
              iVar3 = (int)lVar7;
              lVar7 = lVar7 + 1;
            } while (iVar9 <= iVar10 && iVar3 + 2 <= iVar12);
          }
          if (iVar9 <= iVar10) {
            nrerror_vsdlss(0x25,"vsdlss1memutils.c",0x52a);
          }
        }
      }
      else {
        iVar12 = *(int *)(lVar2 + lVar6 * 4);
        lVar11 = *(long *)(*(long *)(param_2 + 0x60) + (long)(iVar9 - *(int *)(param_2 + 0x68)) * 8)
        ;
        iVar9 = iVar10 - iVar12;
        if (iVar10 != iVar12) goto LAB_00a17720;
LAB_00a177b4:
        lVar6 = 0;
        do {
          *(double *)(lVar11 + 8 + lVar6 * 8) =
               *(double *)(lVar14 + 0x10 + lVar6 * 8) * dVar17 +
               *(double *)(lVar13 + 0x10 + lVar6 * 8) * dVar18 + *(double *)(lVar11 + 8 + lVar6 * 8)
          ;
          iVar9 = (int)lVar6;
          lVar6 = lVar6 + 1;
        } while (iVar9 + 2 <= iVar10);
      }
      lVar13 = lVar13 + 8;
      lVar14 = lVar14 + 8;
      lVar15 = lVar15 + 4;
    } while (iVar10 != 1);
  }
  return;
}


