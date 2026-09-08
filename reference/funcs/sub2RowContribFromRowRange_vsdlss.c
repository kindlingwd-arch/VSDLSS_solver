/* VSDLSS function (decompiled by Ghidra) */
/* name: sub2RowContribFromRowRange_vsdlss  addr: 009efd30  size: 400 */
#include "vsdlss_ref.h"

/* signature: void sub2RowContribFromRowRange_vsdlss(int param_1,int param_2,int param_3,long param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub2RowContribFromRowRange_vsdlss(int param_1,int param_2,int param_3,long param_4)

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
  long lVar10;
  long lVar11;
  long lVar12;
  int iVar13;
  int iVar14;
  long lVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  
  lVar1 = *(long *)(param_4 + 0x30);
  lVar2 = *(long *)(param_4 + 0x28);
  lVar6 = (long)param_1;
  iVar9 = *(int *)(lVar2 + lVar6 * 4);
  lVar15 = *(long *)(lVar1 + lVar6 * 8);
  lVar10 = *(long *)(*(long *)(param_4 + 0x38) + lVar6 * 8);
  lVar6 = *(long *)(*(long *)(param_4 + 0x38) + 8 + lVar6 * 8);
  if (param_2 <= *(int *)(lVar15 + (long)iVar9 * 4)) {
    dVar19 = *(double *)(lVar10 + 8);
    if (dVar19 <= 0.0) {
      nrerror_vsdlss(0x23,"factorapar_ls.c",0x11d,param_1);
      dVar19 = *(double *)(lVar10 + 8);
    }
    dVar4 = _DAT_00ab67f8;
    iVar14 = iVar9 + -1;
    dVar19 = _DAT_00ab67f8 / dVar19;
    if ((param_2 <= param_1 + 1) &&
       (dVar16 = (double)(*(ulong *)(lVar10 + 0x10) ^ DAT_00abd200), 0 < iVar14)) {
      lVar7 = 0;
      do {
        *(double *)(lVar6 + 8 + lVar7) =
             *(double *)(lVar10 + 0x10 + lVar7) * dVar16 * dVar19 + *(double *)(lVar6 + 8 + lVar7);
        lVar7 = lVar7 + 8;
      } while (lVar7 != (ulong)(iVar9 - 2) * 8 + 8);
    }
    dVar16 = *(double *)(lVar6 + 8);
    if (dVar16 <= 0.0) {
      nrerror_vsdlss(0x23,"factorapar_ls.c",300);
      dVar16 = *(double *)(lVar6 + 8);
    }
    uVar5 = DAT_00abd200;
    if (1 < iVar14) {
      iVar9 = *(int *)(lVar15 + 0xc);
      while( true ) {
        lVar10 = lVar10 + 8;
        if (param_2 <= iVar9) break;
        iVar14 = iVar14 + -1;
        lVar6 = lVar6 + 8;
        if (iVar14 == 1) {
          return;
        }
        iVar9 = *(int *)(lVar15 + 0x10);
        lVar15 = lVar15 + 4;
      }
      if (iVar9 <= param_3) {
        lVar15 = lVar15 + 4;
        do {
          iVar14 = iVar14 + -1;
          lVar7 = (long)iVar9;
          dVar17 = (double)(*(ulong *)(lVar10 + 0x10) ^ uVar5) * dVar19;
          dVar18 = (double)(*(ulong *)(lVar6 + 0x10) ^ uVar5) * (dVar4 / dVar16);
          if ((*(long *)(param_4 + 0x60) == 0) || (iVar9 <= *(int *)(param_4 + 0x68))) {
            iVar13 = *(int *)(lVar2 + lVar7 * 4);
            lVar12 = *(long *)(*(long *)(param_4 + 0x38) + lVar7 * 8);
            if (iVar14 != iVar13) goto LAB_009efedb;
LAB_009eff6d:
            lVar7 = 0;
            do {
              *(double *)(lVar12 + 8 + lVar7 * 8) =
                   *(double *)(lVar10 + 0x10 + lVar7 * 8) * dVar17 +
                   *(double *)(lVar6 + 0x10 + lVar7 * 8) * dVar18 +
                   *(double *)(lVar12 + 8 + lVar7 * 8);
              iVar9 = (int)lVar7;
              lVar7 = lVar7 + 1;
            } while (iVar9 + 2 <= iVar14);
          }
          else {
            iVar13 = *(int *)(lVar2 + lVar7 * 4);
            lVar12 = *(long *)(*(long *)(param_4 + 0x60) +
                              (long)(iVar9 - *(int *)(param_4 + 0x68)) * 8);
            if (iVar14 == iVar13) goto LAB_009eff6d;
LAB_009efedb:
            if (iVar14 < iVar13) {
              lVar7 = *(long *)(lVar1 + lVar7 * 8);
              if (0 < iVar13) {
                lVar8 = 0;
                iVar9 = 1;
                do {
                  lVar11 = (long)iVar9;
                  if (*(int *)(lVar15 + 4 + lVar11 * 4) == *(int *)(lVar7 + 4 + lVar8 * 4)) {
                    iVar9 = iVar9 + 1;
                    *(double *)(lVar12 + 8 + lVar8 * 8) =
                         *(double *)(lVar10 + 8 + lVar11 * 8) * dVar17 +
                         *(double *)(lVar6 + 8 + lVar11 * 8) * dVar18 +
                         *(double *)(lVar12 + 8 + lVar8 * 8);
                  }
                  iVar3 = (int)lVar8;
                  lVar8 = lVar8 + 1;
                } while (iVar9 <= iVar14 && iVar3 + 2 <= iVar13);
                if (iVar14 < iVar9) goto LAB_009effb1;
              }
              nrerror_vsdlss(0x25,"factorapar_ls.c",0x158);
            }
            else {
              nrerror_vsdlss(1,"factorapar_ls.c",0x15d);
            }
          }
LAB_009effb1:
        } while ((iVar14 != 1) &&
                (iVar9 = *(int *)(lVar15 + 0xc), lVar6 = lVar6 + 8, lVar10 = lVar10 + 8,
                lVar15 = lVar15 + 4, iVar9 <= param_3));
      }
    }
  }
  return;
}


