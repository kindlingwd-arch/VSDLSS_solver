/* VSDLSS function (decompiled by Ghidra) */
/* name: sub3RowContribFromRowRange_vsdlss  addr: 009f0430  size: 5bb */
#include "vsdlss_ref.h"

/* signature: void sub3RowContribFromRowRange_vsdlss(int param_1,int param_2,int param_3,long param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub3RowContribFromRowRange_vsdlss(int param_1,int param_2,int param_3,long param_4)

{
  long lVar1;
  long lVar2;
  int iVar3;
  double dVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  int iVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  int iVar14;
  int iVar15;
  long lVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  
  lVar1 = *(long *)(param_4 + 0x30);
  lVar11 = *(long *)(param_4 + 0x38);
  lVar2 = *(long *)(param_4 + 0x28);
  lVar6 = (long)param_1;
  lVar16 = *(long *)(lVar1 + lVar6 * 8);
  lVar8 = *(long *)(lVar11 + lVar6 * 8);
  iVar10 = *(int *)(lVar2 + lVar6 * 4);
  lVar12 = *(long *)(lVar11 + 8 + lVar6 * 8);
  lVar11 = *(long *)(lVar11 + 0x10 + lVar6 * 8);
  if (param_2 <= *(int *)(lVar16 + (long)iVar10 * 4)) {
    dVar21 = *(double *)(lVar8 + 8);
    if (dVar21 <= 0.0) {
      nrerror_vsdlss(0x23,"factorapar_ls.c",0x1d4,param_1);
      dVar21 = *(double *)(lVar8 + 8);
    }
    dVar4 = _DAT_00ab67f8;
    dVar21 = _DAT_00ab67f8 / dVar21;
    if ((param_2 <= param_1 + 1) &&
       (dVar22 = (double)(*(ulong *)(lVar8 + 0x10) ^ DAT_00abd200), 1 < iVar10)) {
      lVar6 = 0;
      do {
        *(double *)(lVar12 + 8 + lVar6) =
             *(double *)(lVar8 + 0x10 + lVar6) * dVar22 * dVar21 + *(double *)(lVar12 + 8 + lVar6);
        lVar6 = lVar6 + 8;
      } while (lVar6 != (ulong)(iVar10 - 2) * 8 + 8);
    }
    iVar15 = iVar10 + -2;
    dVar22 = *(double *)(lVar12 + 8);
    if (dVar22 <= 0.0) {
      nrerror_vsdlss(0x23,"factorapar_ls.c",0x1e3);
      dVar22 = *(double *)(lVar12 + 8);
    }
    lVar6 = lVar8 + 0x10;
    dVar22 = dVar4 / dVar22;
    if (param_2 <= param_1 + 2) {
      dVar17 = (double)(*(ulong *)(lVar8 + 0x18) ^ DAT_00abd200);
      dVar19 = (double)(*(ulong *)(lVar12 + 0x10) ^ DAT_00abd200);
      if (0 < iVar15) {
        lVar7 = 0;
        do {
          *(double *)(lVar11 + 8 + lVar7) =
               *(double *)(lVar8 + 0x18 + lVar7) * dVar17 * dVar21 +
               *(double *)(lVar12 + 0x10 + lVar7) * dVar19 * dVar22 +
               *(double *)(lVar11 + 8 + lVar7);
          lVar7 = lVar7 + 8;
        } while (lVar7 != (ulong)(iVar10 - 3) * 8 + 8);
      }
    }
    dVar17 = *(double *)(lVar11 + 8);
    if (dVar17 <= 0.0) {
      nrerror_vsdlss(0x23,"factorapar_ls.c",500);
      dVar17 = *(double *)(lVar11 + 8);
    }
    uVar5 = DAT_00abd200;
    if (1 < iVar15) {
      iVar10 = *(int *)(lVar16 + 0x10);
      lVar16 = lVar16 + 8;
      while (lVar12 = lVar12 + 8, iVar10 < param_2) {
        iVar15 = iVar15 + -1;
        lVar6 = lVar6 + 8;
        lVar11 = lVar11 + 8;
        if (iVar15 == 1) {
          return;
        }
        iVar10 = *(int *)(lVar16 + 0xc);
        lVar16 = lVar16 + 4;
      }
      if (iVar10 <= param_3) {
        do {
          iVar15 = iVar15 + -1;
          lVar8 = (long)iVar10;
          dVar19 = (double)(*(ulong *)(lVar6 + 0x10) ^ uVar5) * dVar21;
          dVar18 = (double)(*(ulong *)(lVar12 + 0x10) ^ uVar5) * dVar22;
          dVar20 = (double)(*(ulong *)(lVar11 + 0x10) ^ uVar5) * (dVar4 / dVar17);
          if ((*(long *)(param_4 + 0x60) == 0) || (iVar10 <= *(int *)(param_4 + 0x68))) {
            iVar14 = *(int *)(lVar2 + lVar8 * 4);
            lVar7 = *(long *)(*(long *)(param_4 + 0x38) + lVar8 * 8);
            if (iVar15 != iVar14) goto LAB_009f06e0;
LAB_009f0785:
            lVar8 = 0;
            do {
              *(double *)(lVar7 + 8 + lVar8 * 8) =
                   *(double *)(lVar6 + 0x10 + lVar8 * 8) * dVar19 +
                   *(double *)(lVar12 + 0x10 + lVar8 * 8) * dVar18 +
                   *(double *)(lVar11 + 0x10 + lVar8 * 8) * dVar20 +
                   *(double *)(lVar7 + 8 + lVar8 * 8);
              iVar10 = (int)lVar8;
              lVar8 = lVar8 + 1;
            } while (iVar10 + 2 <= iVar15);
          }
          else {
            iVar14 = *(int *)(lVar2 + lVar8 * 4);
            lVar7 = *(long *)(*(long *)(param_4 + 0x60) +
                             (long)(iVar10 - *(int *)(param_4 + 0x68)) * 8);
            if (iVar15 == iVar14) goto LAB_009f0785;
LAB_009f06e0:
            if (iVar15 < iVar14) {
              lVar8 = *(long *)(lVar1 + lVar8 * 8);
              if (0 < iVar14) {
                lVar9 = 0;
                iVar10 = 1;
                do {
                  lVar13 = (long)iVar10;
                  if (*(int *)(lVar16 + 4 + lVar13 * 4) == *(int *)(lVar8 + 4 + lVar9 * 4)) {
                    iVar10 = iVar10 + 1;
                    *(double *)(lVar7 + 8 + lVar9 * 8) =
                         *(double *)(lVar6 + 8 + lVar13 * 8) * dVar19 +
                         *(double *)(lVar12 + 8 + lVar13 * 8) * dVar18 +
                         *(double *)(lVar11 + 8 + lVar13 * 8) * dVar20 +
                         *(double *)(lVar7 + 8 + lVar9 * 8);
                  }
                  iVar3 = (int)lVar9;
                  lVar9 = lVar9 + 1;
                } while (iVar10 <= iVar15 && iVar3 + 2 <= iVar14);
                if (iVar15 < iVar10) goto LAB_009f07e1;
              }
              nrerror_vsdlss(0x25,"factorapar_ls.c",0x225);
            }
            else {
              nrerror_vsdlss(1,"factorapar_ls.c",0x22a);
            }
          }
LAB_009f07e1:
        } while ((iVar15 != 1) &&
                (iVar10 = *(int *)(lVar16 + 0xc), lVar6 = lVar6 + 8, lVar12 = lVar12 + 8,
                lVar11 = lVar11 + 8, lVar16 = lVar16 + 4, iVar10 <= param_3));
      }
    }
  }
  return;
}


