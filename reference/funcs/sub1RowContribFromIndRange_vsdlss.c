/* VSDLSS function (decompiled by Ghidra) */
/* name: sub1RowContribFromIndRange_vsdlss  addr: 009efad0  size: 248 */
#include "vsdlss_ref.h"

/* signature: void sub1RowContribFromIndRange_vsdlss(int param_1,int param_2,int param_3,long param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub1RowContribFromIndRange_vsdlss(int param_1,int param_2,int param_3,long param_4)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  int iVar11;
  long lVar12;
  int iVar13;
  long lVar14;
  double dVar15;
  double dVar16;
  
  uVar6 = DAT_00abd200;
  lVar14 = (long)param_1;
  lVar3 = *(long *)(param_4 + 0x28);
  lVar4 = *(long *)(param_4 + 0x30);
  if (param_2 < 2) {
    param_2 = 2;
  }
  lVar9 = *(long *)(*(long *)(param_4 + 0x38) + lVar14 * 8);
  iVar13 = *(int *)(lVar3 + lVar14 * 4);
  if (iVar13 <= param_3) {
    param_3 = iVar13;
  }
  if (param_3 < param_2) {
    return;
  }
  dVar16 = _DAT_00ab67f8 / *(double *)(lVar9 + 8);
  lVar9 = lVar9 + -0x10 + (long)param_2 * 8;
  lVar14 = *(long *)(lVar4 + lVar14 * 8) + -8 + (long)param_2 * 4;
  iVar13 = (2 - param_2) + iVar13;
  do {
    iVar11 = iVar13 + -1;
    iVar1 = *(int *)(lVar14 + 8);
    lVar7 = (long)iVar1;
    dVar15 = (double)(*(ulong *)(lVar9 + 0x10) ^ uVar6) * dVar16;
    if ((*(long *)(param_4 + 0x60) == 0) || (iVar1 <= *(int *)(param_4 + 0x68))) {
      iVar2 = *(int *)(lVar3 + lVar7 * 4);
      iVar5 = iVar11 - iVar2;
      lVar12 = *(long *)(*(long *)(param_4 + 0x38) + lVar7 * 8);
      if (iVar5 == 0) goto LAB_009efc25;
LAB_009efb99:
      if (SBORROW4(iVar11,iVar2) == iVar5 < 0) {
        nrerror_vsdlss(1,"factorapar_ls.c",0xf6);
      }
      else {
        lVar7 = *(long *)(lVar4 + lVar7 * 8);
        if ((iVar2 < 1) || (iVar11 < 1)) {
          iVar13 = 1;
        }
        else {
          lVar8 = 0;
          iVar13 = 1;
          do {
            lVar10 = (long)iVar13;
            if (*(int *)(lVar14 + 4 + lVar10 * 4) == *(int *)(lVar7 + 4 + lVar8 * 4)) {
              iVar13 = iVar13 + 1;
              *(double *)(lVar12 + 8 + lVar8 * 8) =
                   *(double *)(lVar9 + 8 + lVar10 * 8) * dVar15 +
                   *(double *)(lVar12 + 8 + lVar8 * 8);
            }
            iVar1 = (int)lVar8;
            lVar8 = lVar8 + 1;
          } while (iVar13 <= iVar11 && iVar1 + 2 <= iVar2);
        }
        if (iVar13 <= iVar11) {
          nrerror_vsdlss(0x25,"factorapar_ls.c",0xf1);
        }
      }
    }
    else {
      iVar2 = *(int *)(lVar3 + lVar7 * 4);
      iVar5 = iVar11 - iVar2;
      lVar12 = *(long *)(*(long *)(param_4 + 0x60) + (long)(iVar1 - *(int *)(param_4 + 0x68)) * 8);
      if (iVar11 != iVar2) goto LAB_009efb99;
LAB_009efc25:
      if (0 < iVar11) {
        lVar7 = 0;
        do {
          *(double *)(lVar12 + 8 + lVar7) =
               *(double *)(lVar9 + 0x10 + lVar7) * dVar15 + *(double *)(lVar12 + 8 + lVar7);
          lVar7 = lVar7 + 8;
        } while (lVar7 != (ulong)(iVar13 - 2) * 8 + 8);
      }
    }
    if (param_3 < param_2 + 1) {
      return;
    }
    param_2 = param_2 + 1;
    lVar9 = lVar9 + 8;
    lVar14 = lVar14 + 4;
    iVar13 = iVar11;
  } while( true );
}


