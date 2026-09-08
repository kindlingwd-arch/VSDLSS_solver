/* VSDLSS function (decompiled by Ghidra) */
/* name: sub1RowContribFromRowRange_vsdlss  addr: 009ef830  size: 274 */
#include "vsdlss_ref.h"

/* signature: void sub1RowContribFromRowRange_vsdlss(int param_1,int param_2,int param_3,long param_4); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub1RowContribFromRowRange_vsdlss(int param_1,int param_2,int param_3,long param_4)

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
  long lVar10;
  int iVar11;
  long lVar12;
  long lVar13;
  double dVar14;
  double dVar15;
  
  lVar6 = (long)param_1;
  lVar2 = *(long *)(param_4 + 0x28);
  lVar3 = *(long *)(param_4 + 0x30);
  iVar9 = *(int *)(lVar2 + lVar6 * 4);
  lVar10 = *(long *)(lVar3 + lVar6 * 8);
  lVar6 = *(long *)(*(long *)(param_4 + 0x38) + lVar6 * 8);
  if (*(int *)(lVar10 + (long)iVar9 * 4) < param_2) {
    return;
  }
  dVar15 = *(double *)(lVar6 + 8);
  if (dVar15 <= 0.0) {
    nrerror_vsdlss(0x23,"factorapar_ls.c",0x84,param_1);
    dVar15 = *(double *)(lVar6 + 8);
  }
  uVar5 = DAT_00abd200;
  if (iVar9 < 2) {
    return;
  }
  while (iVar11 = *(int *)(lVar10 + 8), iVar11 < param_2) {
    iVar9 = iVar9 + -1;
    lVar6 = lVar6 + 8;
    lVar10 = lVar10 + 4;
    if (iVar9 == 1) {
      return;
    }
  }
  if (param_3 < iVar11) {
    return;
  }
  dVar15 = _DAT_00ab67f8 / dVar15;
  do {
    iVar9 = iVar9 + -1;
    lVar7 = (long)iVar11;
    dVar14 = (double)(*(ulong *)(lVar6 + 0x10) ^ uVar5) * dVar15;
    if ((*(long *)(param_4 + 0x60) == 0) || (iVar11 <= *(int *)(param_4 + 0x68))) {
      iVar1 = *(int *)(lVar2 + lVar7 * 4);
      lVar13 = *(long *)(*(long *)(param_4 + 0x38) + lVar7 * 8);
      if (iVar9 == iVar1) goto LAB_009ef9b7;
LAB_009ef91a:
      if (iVar9 < iVar1) {
        lVar7 = *(long *)(lVar3 + lVar7 * 8);
        if (0 < iVar1) {
          lVar8 = 0;
          iVar11 = 1;
          do {
            lVar12 = (long)iVar11;
            if (*(int *)(lVar10 + 4 + lVar12 * 4) == *(int *)(lVar7 + 4 + lVar8 * 4)) {
              iVar11 = iVar11 + 1;
              *(double *)(lVar13 + 8 + lVar8 * 8) =
                   *(double *)(lVar6 + 8 + lVar12 * 8) * dVar14 +
                   *(double *)(lVar13 + 8 + lVar8 * 8);
            }
            iVar4 = (int)lVar8;
            lVar8 = lVar8 + 1;
          } while (iVar11 <= iVar9 && iVar4 + 2 <= iVar1);
          if (iVar9 < iVar11) goto LAB_009ef9ec;
        }
        nrerror_vsdlss(0x25,"factorapar_ls.c",0xad);
      }
      else {
        nrerror_vsdlss(1,"factorapar_ls.c",0xb2);
      }
    }
    else {
      iVar1 = *(int *)(lVar2 + lVar7 * 4);
      lVar13 = *(long *)(*(long *)(param_4 + 0x60) + (long)(iVar11 - *(int *)(param_4 + 0x68)) * 8);
      if (iVar9 != iVar1) goto LAB_009ef91a;
LAB_009ef9b7:
      lVar7 = 0;
      do {
        *(double *)(lVar13 + 8 + lVar7 * 8) =
             *(double *)(lVar6 + 0x10 + lVar7 * 8) * dVar14 + *(double *)(lVar13 + 8 + lVar7 * 8);
        iVar11 = (int)lVar7;
        lVar7 = lVar7 + 1;
      } while (iVar11 + 2 <= iVar9);
    }
LAB_009ef9ec:
    if ((iVar9 == 1) ||
       (iVar11 = *(int *)(lVar10 + 0xc), lVar6 = lVar6 + 8, lVar10 = lVar10 + 4, param_3 < iVar11))
    {
      return;
    }
  } while( true );
}


