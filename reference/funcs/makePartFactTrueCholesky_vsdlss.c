/* VSDLSS function (decompiled by Ghidra) */
/* name: makePartFactTrueCholesky_vsdlss  addr: 00a1a960  size: aa */
#include "vsdlss_ref.h"

/* signature: void makePartFactTrueCholesky_vsdlss(int param_1,long param_2); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void makePartFactTrueCholesky_vsdlss(int param_1,long param_2)

{
  int iVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  double dVar5;
  double *pdVar6;
  long lVar8;
  double dVar9;
  double *pdVar7;
  
  dVar5 = _DAT_00ab67f8;
  lVar2 = *(long *)(param_2 + 0x38);
  lVar3 = *(long *)(param_2 + 0x28);
  param_1 = *(int *)(param_2 + 8) - param_1;
  lVar8 = 0;
  if (0 < param_1) {
    do {
      lVar4 = *(long *)(lVar2 + 8 + lVar8 * 8);
      iVar1 = *(int *)(lVar3 + 4 + lVar8 * 4);
      dVar9 = SQRT(*(double *)(lVar4 + 8));
      if (NAN(dVar9)) {
        dVar9 = sqrt(*(double *)(lVar4 + 8));
      }
      if (0 < iVar1) {
        pdVar6 = (double *)(lVar4 + 8);
        do {
          pdVar7 = pdVar6 + 1;
          *pdVar6 = *pdVar6 * (dVar5 / dVar9);
          pdVar6 = pdVar7;
        } while (pdVar7 != (double *)(lVar4 + 0x10 + (ulong)(iVar1 - 1) * 8));
      }
      iVar1 = (int)lVar8;
      lVar8 = lVar8 + 1;
    } while (iVar1 + 2 <= param_1);
  }
  return;
}


