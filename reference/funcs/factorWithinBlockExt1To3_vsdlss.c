/* VSDLSS function (decompiled by Ghidra) */
/* name: factorWithinBlockExt1To3_vsdlss  addr: 009ef5e0  size: 235 */
#include "vsdlss_ref.h"

/* signature: void factorWithinBlockExt1To3_vsdlss(int param_1,int param_2,long param_3); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void factorWithinBlockExt1To3_vsdlss(int param_1,int param_2,long param_3)

{
  ulong uVar1;
  ulong uVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  double dVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  double dVar11;
  double dVar12;
  
  lVar10 = (long)param_1;
  lVar4 = *(long *)(param_3 + 0x38);
  iVar3 = *(int *)(*(long *)(param_3 + 0x28) + lVar10 * 4);
  if (2 < param_2 - 1U) {
    nrerror_vsdlss(1,"factorapar_ls.c",0x39);
  }
  lVar5 = *(long *)(lVar4 + lVar10 * 8);
  if (*(double *)(lVar5 + 8) <= 0.0) {
    nrerror_vsdlss(0x23,"factorapar_ls.c",0x3f,param_1);
  }
  uVar8 = DAT_00abd200;
  dVar7 = _DAT_00ab67f8;
  if (1 < param_2) {
    lVar6 = *(long *)(lVar4 + 8 + lVar10 * 8);
    dVar12 = _DAT_00ab67f8 / *(double *)(lVar5 + 8);
    dVar11 = (double)(*(ulong *)(lVar5 + 0x10) ^ DAT_00abd200);
    if (1 < iVar3) {
      lVar9 = 0;
      do {
        *(double *)(lVar6 + 8 + lVar9) =
             *(double *)(lVar5 + 0x10 + lVar9) * dVar11 * dVar12 + *(double *)(lVar6 + 8 + lVar9);
        lVar9 = lVar9 + 8;
      } while (lVar9 != (ulong)(iVar3 - 2) * 8 + 8);
    }
    if (*(double *)(lVar6 + 8) <= 0.0) {
      nrerror_vsdlss(0x23,"factorapar_ls.c",0x4e,param_1 + 1);
    }
    if (param_2 != 2) {
      dVar11 = *(double *)(lVar6 + 8);
      lVar4 = *(long *)(lVar4 + 0x10 + lVar10 * 8);
      uVar1 = *(ulong *)(lVar5 + 0x18);
      uVar2 = *(ulong *)(lVar6 + 0x10);
      if (2 < iVar3) {
        lVar10 = 0;
        do {
          *(double *)(lVar4 + 8 + lVar10) =
               *(double *)(lVar5 + 0x18 + lVar10) * (double)(uVar1 ^ uVar8) * dVar12 +
               *(double *)(lVar6 + 0x10 + lVar10) * (double)(uVar2 ^ uVar8) * (dVar7 / dVar11) +
               *(double *)(lVar4 + 8 + lVar10);
          lVar10 = lVar10 + 8;
        } while (lVar10 != (ulong)(iVar3 - 3) * 8 + 8);
      }
      if (*(double *)(lVar4 + 8) <= 0.0) {
        nrerror_vsdlss(0x23,"factorapar_ls.c",0x5f,param_1 + 2);
        return;
      }
    }
  }
  return;
}


