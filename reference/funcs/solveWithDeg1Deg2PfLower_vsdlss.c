/* VSDLSS function (decompiled by Ghidra) */
/* name: solveWithDeg1Deg2PfLower_vsdlss  addr: 00a0ae60  size: 136 */
#include "vsdlss_ref.h"

/* signature: void solveWithDeg1Deg2PfLower_vsdlss(long param_1,long param_2,long param_3); */

void solveWithDeg1Deg2PfLower_vsdlss(long param_1,long param_2,long param_3)

{
  double *pdVar1;
  long lVar2;
  double dVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  double *pdVar14;
  int iVar15;
  double dVar16;
  double dVar17;
  
  iVar4 = *(int *)(param_1 + 4);
  lVar7 = *(long *)(param_1 + 0x10);
  lVar8 = *(long *)(param_1 + 0x28);
  lVar9 = *(long *)(param_1 + 0x30);
  lVar10 = *(long *)(param_1 + 0x38);
  lVar11 = *(long *)(param_1 + 0x40);
  iVar15 = *(int *)(param_1 + 8) + iVar4;
  if (0 < iVar4) {
    lVar12 = 0;
    do {
      iVar5 = *(int *)(lVar8 + 4 + lVar12);
      dVar3 = *(double *)(lVar9 + 8 + lVar12 * 2);
      dVar16 = *(double *)(param_2 + 8 + lVar12 * 2) / *(double *)(lVar7 + 8 + lVar12 * 2);
      *(double *)(param_3 + 8 + lVar12 * 2) = dVar16;
      if (0 < iVar5) {
        pdVar14 = (double *)(param_2 + (long)iVar5 * 8);
        *pdVar14 = *pdVar14 - dVar16 * dVar3;
      }
      lVar12 = lVar12 + 4;
    } while (lVar12 != (ulong)(iVar4 - 1) * 4 + 4);
  }
  iVar5 = iVar4 + 1;
  if (iVar5 <= iVar15) {
    lVar12 = (long)iVar5 * 8;
    lVar2 = (long)iVar5 * 4;
    pdVar14 = (double *)(param_3 + lVar12);
    lVar13 = 0;
    do {
      iVar5 = *(int *)(lVar8 + lVar2 + lVar13);
      dVar3 = *(double *)(lVar9 + lVar12 + lVar13 * 2);
      iVar6 = *(int *)(lVar2 + lVar10 + lVar13);
      dVar17 = *(double *)(param_2 + lVar12 + lVar13 * 2) / *(double *)(lVar7 + lVar12 + lVar13 * 2)
      ;
      dVar16 = *(double *)(lVar11 + lVar12 + lVar13 * 2);
      *pdVar14 = dVar17;
      if (0 < iVar5) {
        pdVar1 = (double *)(param_2 + (long)iVar5 * 8);
        *pdVar1 = *pdVar1 - dVar17 * dVar3;
      }
      if (0 < iVar6) {
        pdVar1 = (double *)(param_2 + (long)iVar6 * 8);
        *pdVar1 = *pdVar1 - dVar16 * *pdVar14;
      }
      lVar13 = lVar13 + 4;
      pdVar14 = pdVar14 + 1;
    } while (lVar13 != (ulong)((iVar15 - iVar4) - 1) * 4 + 4);
  }
  return;
}


