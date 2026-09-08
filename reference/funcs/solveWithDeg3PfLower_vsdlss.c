/* VSDLSS function (decompiled by Ghidra) */
/* name: solveWithDeg3PfLower_vsdlss  addr: 00a0b0d0  size: db */
#include "vsdlss_ref.h"

/* signature: void solveWithDeg3PfLower_vsdlss(long param_1,long param_2,long param_3); */

void solveWithDeg3PfLower_vsdlss(long param_1,long param_2,long param_3)

{
  double *pdVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  double dVar17;
  
  iVar5 = *(int *)(param_1 + 4);
  lVar9 = *(long *)(param_1 + 8);
  lVar10 = *(long *)(param_1 + 0x10);
  lVar11 = *(long *)(param_1 + 0x18);
  lVar12 = *(long *)(param_1 + 0x20);
  lVar13 = *(long *)(param_1 + 0x28);
  lVar14 = *(long *)(param_1 + 0x30);
  lVar15 = *(long *)(param_1 + 0x38);
  if (0 < iVar5) {
    lVar16 = 0;
    do {
      iVar6 = *(int *)(lVar10 + 4 + lVar16);
      dVar2 = *(double *)(lVar11 + 8 + lVar16 * 2);
      iVar7 = *(int *)(lVar12 + 4 + lVar16);
      dVar17 = *(double *)(param_2 + 8 + lVar16 * 2) / *(double *)(lVar9 + 8 + lVar16 * 2);
      iVar8 = *(int *)(lVar14 + 4 + lVar16);
      dVar3 = *(double *)(lVar13 + 8 + lVar16 * 2);
      dVar4 = *(double *)(lVar15 + 8 + lVar16 * 2);
      *(double *)(param_3 + 8 + lVar16 * 2) = dVar17;
      if (0 < iVar6) {
        pdVar1 = (double *)(param_2 + (long)iVar6 * 8);
        *pdVar1 = *pdVar1 - dVar17 * dVar2;
      }
      if (0 < iVar7) {
        pdVar1 = (double *)(param_2 + (long)iVar7 * 8);
        *pdVar1 = *pdVar1 - dVar3 * *(double *)(param_3 + 8 + lVar16 * 2);
      }
      if (0 < iVar8) {
        pdVar1 = (double *)(param_2 + (long)iVar8 * 8);
        *pdVar1 = *pdVar1 - dVar4 * *(double *)(param_3 + 8 + lVar16 * 2);
      }
      lVar16 = lVar16 + 4;
    } while (lVar16 != (ulong)(iVar5 - 1) * 4 + 4);
  }
  return;
}


