/* VSDLSS function (decompiled by Ghidra) */
/* name: sub3RowContribFromRest_vsdlss  addr: 00a17970  size: 54c */
#include "vsdlss_ref.h"

/* signature: void sub3RowContribFromRest_vsdlss(int param_1,long param_2); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub3RowContribFromRest_vsdlss(int param_1,long param_2)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  double dVar6;
  ulong uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  long lVar12;
  long lVar13;
  int iVar14;
  int iVar15;
  long lVar16;
  long lVar17;
  long lVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  double dVar24;
  
  lVar3 = *(long *)(param_2 + 0x30);
  lVar16 = *(long *)(param_2 + 0x38);
  lVar4 = *(long *)(param_2 + 0x28);
  lVar8 = (long)param_1;
  lVar17 = *(long *)(lVar16 + lVar8 * 8);
  lVar12 = *(long *)(lVar3 + lVar8 * 8);
  iVar15 = *(int *)(lVar4 + lVar8 * 4);
  lVar18 = *(long *)(lVar16 + 8 + lVar8 * 8);
  lVar16 = *(long *)(lVar16 + 0x10 + lVar8 * 8);
  dVar24 = *(double *)(lVar17 + 8);
  if (dVar24 <= 0.0) {
    nrerror_vsdlss(0x23,"vsdlss1memutils.c",0x558,param_1);
    dVar24 = *(double *)(lVar17 + 8);
  }
  uVar7 = DAT_00abd200;
  dVar6 = _DAT_00ab67f8;
  dVar24 = _DAT_00ab67f8 / dVar24;
  dVar20 = (double)(*(ulong *)(lVar17 + 0x10) ^ DAT_00abd200);
  if (1 < iVar15) {
    lVar8 = 0;
    do {
      *(double *)(lVar18 + 8 + lVar8) =
           *(double *)(lVar17 + 0x10 + lVar8) * dVar20 * dVar24 + *(double *)(lVar18 + 8 + lVar8);
      lVar8 = lVar8 + 8;
    } while (lVar8 != (ulong)(iVar15 - 2) * 8 + 8);
  }
  iVar11 = iVar15 + -2;
  dVar20 = *(double *)(lVar18 + 8);
  if (dVar20 <= 0.0) {
    nrerror_vsdlss(0x23,"vsdlss1memutils.c",0x565,param_1 + 1);
    dVar20 = *(double *)(lVar18 + 8);
  }
  uVar1 = *(ulong *)(lVar17 + 0x18);
  uVar2 = *(ulong *)(lVar18 + 0x10);
  dVar20 = dVar6 / dVar20;
  if (0 < iVar11) {
    lVar8 = 0;
    do {
      *(double *)(lVar16 + 8 + lVar8) =
           *(double *)(lVar17 + 0x18 + lVar8) * (double)(uVar1 ^ uVar7) * dVar24 +
           *(double *)(lVar18 + 0x10 + lVar8) * (double)(uVar2 ^ uVar7) * dVar20 +
           *(double *)(lVar16 + 8 + lVar8);
      lVar8 = lVar8 + 8;
    } while (lVar8 != (ulong)(iVar15 - 3) * 8 + 8);
  }
  dVar19 = *(double *)(lVar16 + 8);
  if (dVar19 <= 0.0) {
    nrerror_vsdlss(0x23,"vsdlss1memutils.c",0x574,param_1 + 2);
    dVar19 = *(double *)(lVar16 + 8);
  }
  lVar12 = lVar12 + 8;
  lVar17 = lVar17 + 0x10;
  lVar18 = lVar18 + 8;
  if (1 < iVar11) {
    do {
      iVar11 = iVar11 + -1;
      iVar15 = *(int *)(lVar12 + 8);
      lVar8 = (long)iVar15;
      dVar21 = (double)(*(ulong *)(lVar17 + 0x10) ^ uVar7) * dVar24;
      dVar22 = (double)(*(ulong *)(lVar18 + 0x10) ^ uVar7) * dVar20;
      dVar23 = (double)(*(ulong *)(lVar16 + 0x10) ^ uVar7) * (dVar6 / dVar19);
      if ((*(long *)(param_2 + 0x60) == 0) || (iVar15 <= *(int *)(param_2 + 0x68))) {
        iVar14 = *(int *)(lVar4 + lVar8 * 4);
        iVar15 = iVar11 - iVar14;
        lVar13 = *(long *)(*(long *)(param_2 + 0x38) + lVar8 * 8);
        if (iVar15 == 0) goto LAB_00a17c0c;
LAB_00a17b62:
        if (SBORROW4(iVar11,iVar14) == iVar15 < 0) {
          nrerror_vsdlss(1,"vsdlss1memutils.c",0x59f);
        }
        else {
          lVar8 = *(long *)(lVar3 + lVar8 * 8);
          if ((iVar14 < 1) || (iVar11 < 1)) {
            iVar15 = 1;
          }
          else {
            lVar9 = 0;
            iVar15 = 1;
            do {
              lVar10 = (long)iVar15;
              if (*(int *)(lVar12 + 4 + lVar10 * 4) == *(int *)(lVar8 + 4 + lVar9 * 4)) {
                iVar15 = iVar15 + 1;
                *(double *)(lVar13 + 8 + lVar9 * 8) =
                     *(double *)(lVar17 + 8 + lVar10 * 8) * dVar21 +
                     *(double *)(lVar18 + 8 + lVar10 * 8) * dVar22 +
                     *(double *)(lVar16 + 8 + lVar10 * 8) * dVar23 +
                     *(double *)(lVar13 + 8 + lVar9 * 8);
              }
              iVar5 = (int)lVar9;
              lVar9 = lVar9 + 1;
            } while (iVar15 <= iVar11 && iVar5 + 2 <= iVar14);
          }
          if (iVar15 <= iVar11) {
            nrerror_vsdlss(0x25,"vsdlss1memutils.c",0x59a);
          }
        }
      }
      else {
        iVar14 = *(int *)(lVar4 + lVar8 * 4);
        lVar13 = *(long *)(*(long *)(param_2 + 0x60) + (long)(iVar15 - *(int *)(param_2 + 0x68)) * 8
                          );
        iVar15 = iVar11 - iVar14;
        if (iVar11 != iVar14) goto LAB_00a17b62;
LAB_00a17c0c:
        lVar8 = 0;
        do {
          *(double *)(lVar13 + 8 + lVar8 * 8) =
               *(double *)(lVar17 + 0x10 + lVar8 * 8) * dVar21 +
               *(double *)(lVar18 + 0x10 + lVar8 * 8) * dVar22 +
               *(double *)(lVar16 + 0x10 + lVar8 * 8) * dVar23 + *(double *)(lVar13 + 8 + lVar8 * 8)
          ;
          iVar15 = (int)lVar8;
          lVar8 = lVar8 + 1;
        } while (iVar15 + 2 <= iVar11);
      }
      lVar12 = lVar12 + 4;
      lVar16 = lVar16 + 8;
      lVar17 = lVar17 + 8;
      lVar18 = lVar18 + 8;
    } while (iVar11 != 1);
  }
  return;
}


