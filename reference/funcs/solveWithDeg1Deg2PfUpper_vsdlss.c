/* VSDLSS function (decompiled by Ghidra) */
/* name: solveWithDeg1Deg2PfUpper_vsdlss  addr: 00a0b650  size: 157 */
#include "vsdlss_ref.h"

/* signature: void solveWithDeg1Deg2PfUpper_vsdlss(long param_1,long param_2,long param_3); */

void solveWithDeg1Deg2PfUpper_vsdlss(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  double dVar3;
  double dVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  int iVar12;
  long lVar13;
  long lVar14;
  double *pdVar15;
  long lVar16;
  double dVar17;
  
  lVar8 = *(long *)(param_1 + 0x10);
  iVar5 = *(int *)(param_1 + 4);
  lVar9 = *(long *)(param_1 + 0x28);
  lVar10 = *(long *)(param_1 + 0x38);
  lVar11 = *(long *)(param_1 + 0x30);
  iVar12 = *(int *)(param_1 + 8) + iVar5;
  lVar14 = *(long *)(param_1 + 0x40);
  if (iVar5 < iVar12) {
    lVar2 = (long)iVar12 * 8;
    lVar16 = (long)iVar12 * 4;
    lVar13 = 0;
    pdVar15 = (double *)(param_3 + lVar2);
    do {
      iVar6 = *(int *)(lVar9 + lVar16 + lVar13);
      dVar17 = *(double *)(param_2 + lVar2 + lVar13 * 2);
      dVar3 = *(double *)(lVar11 + lVar2 + lVar13 * 2);
      iVar7 = *(int *)(lVar16 + lVar10 + lVar13);
      dVar4 = *(double *)(lVar14 + lVar2 + lVar13 * 2);
      *pdVar15 = dVar17;
      if (0 < iVar6) {
        dVar17 = dVar17 - dVar3 * *(double *)(param_3 + (long)iVar6 * 8);
        *pdVar15 = dVar17;
      }
      if (0 < iVar7) {
        dVar17 = dVar17 - dVar4 * *(double *)(param_3 + (long)iVar7 * 8);
        *pdVar15 = dVar17;
      }
      lVar1 = lVar13 * 2;
      lVar13 = lVar13 + -4;
      *pdVar15 = dVar17 / *(double *)(lVar2 + lVar8 + lVar1);
      pdVar15 = pdVar15 + -1;
    } while (lVar13 != ~(ulong)((iVar12 - iVar5) - 1) * 4);
  }
  if (0 < iVar5) {
    lVar10 = (long)iVar5 * 8;
    lVar14 = 0;
    pdVar15 = (double *)(param_3 + lVar10);
    do {
      iVar12 = *(int *)(lVar9 + (long)iVar5 * 4 + lVar14);
      dVar17 = *(double *)(param_2 + lVar10 + lVar14 * 2);
      dVar3 = *(double *)(lVar11 + lVar10 + lVar14 * 2);
      *pdVar15 = dVar17;
      if (0 < iVar12) {
        dVar17 = dVar17 - dVar3 * *(double *)(param_3 + (long)iVar12 * 8);
        *pdVar15 = dVar17;
      }
      lVar2 = lVar14 * 2;
      lVar14 = lVar14 + -4;
      *pdVar15 = dVar17 / *(double *)(lVar10 + lVar8 + lVar2);
      pdVar15 = pdVar15 + -1;
    } while (lVar14 != ~(ulong)(iVar5 - 1) * 4);
  }
  return;
}


