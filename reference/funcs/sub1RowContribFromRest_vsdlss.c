/* VSDLSS function (decompiled by Ghidra) */
/* name: sub1RowContribFromRest_vsdlss  addr: 00a173c0  size: 218 */
#include "vsdlss_ref.h"

/* signature: void sub1RowContribFromRest_vsdlss(int param_1,long param_2); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub1RowContribFromRest_vsdlss(int param_1,long param_2)

{
  int iVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  ulong uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  int iVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  double dVar14;
  double dVar15;
  
  lVar6 = (long)param_1;
  lVar2 = *(long *)(param_2 + 0x30);
  lVar3 = *(long *)(param_2 + 0x28);
  lVar12 = *(long *)(*(long *)(param_2 + 0x38) + lVar6 * 8);
  lVar13 = *(long *)(lVar2 + lVar6 * 8);
  iVar10 = *(int *)(lVar3 + lVar6 * 4);
  dVar15 = *(double *)(lVar12 + 8);
  if (dVar15 <= 0.0) {
    nrerror_vsdlss(0x23,"vsdlss1memutils.c",0x4b0,param_1);
    dVar15 = *(double *)(lVar12 + 8);
  }
  uVar5 = DAT_00abd200;
  dVar15 = _DAT_00ab67f8 / dVar15;
  if (1 < iVar10) {
    do {
      iVar10 = iVar10 + -1;
      iVar9 = *(int *)(lVar13 + 8);
      lVar6 = (long)iVar9;
      dVar14 = (double)(*(ulong *)(lVar12 + 0x10) ^ uVar5) * dVar15;
      if ((*(long *)(param_2 + 0x60) == 0) || (iVar9 <= *(int *)(param_2 + 0x68))) {
        iVar1 = *(int *)(lVar3 + lVar6 * 4);
        iVar4 = iVar10 - iVar1;
        lVar11 = *(long *)(*(long *)(param_2 + 0x38) + lVar6 * 8);
        if (iVar4 == 0) goto LAB_00a174f5;
LAB_00a1746f:
        if (SBORROW4(iVar10,iVar1) == iVar4 < 0) {
          nrerror_vsdlss(1,"vsdlss1memutils.c",0x4d6);
        }
        else {
          lVar6 = *(long *)(lVar2 + lVar6 * 8);
          if ((iVar1 < 1) || (iVar10 < 1)) {
            iVar9 = 1;
          }
          else {
            lVar7 = 0;
            iVar9 = 1;
            do {
              lVar8 = (long)iVar9;
              if (*(int *)(lVar13 + 4 + lVar8 * 4) == *(int *)(lVar6 + 4 + lVar7 * 4)) {
                iVar9 = iVar9 + 1;
                *(double *)(lVar11 + 8 + lVar7 * 8) =
                     *(double *)(lVar12 + 8 + lVar8 * 8) * dVar14 +
                     *(double *)(lVar11 + 8 + lVar7 * 8);
              }
              iVar4 = (int)lVar7;
              lVar7 = lVar7 + 1;
            } while (iVar9 <= iVar10 && iVar4 + 2 <= iVar1);
          }
          if (iVar9 <= iVar10) {
            nrerror_vsdlss(0x25,"vsdlss1memutils.c",0x4d1);
          }
        }
      }
      else {
        iVar1 = *(int *)(lVar3 + lVar6 * 4);
        iVar4 = iVar10 - iVar1;
        lVar11 = *(long *)(*(long *)(param_2 + 0x60) + (long)(iVar9 - *(int *)(param_2 + 0x68)) * 8)
        ;
        if (iVar10 != iVar1) goto LAB_00a1746f;
LAB_00a174f5:
        lVar6 = 0;
        do {
          *(double *)(lVar11 + 8 + lVar6 * 8) =
               *(double *)(lVar12 + 0x10 + lVar6 * 8) * dVar14 + *(double *)(lVar11 + 8 + lVar6 * 8)
          ;
          iVar9 = (int)lVar6;
          lVar6 = lVar6 + 1;
        } while (iVar9 + 2 <= iVar10);
      }
      lVar12 = lVar12 + 8;
      lVar13 = lVar13 + 4;
    } while (iVar10 != 1);
  }
  return;
}


