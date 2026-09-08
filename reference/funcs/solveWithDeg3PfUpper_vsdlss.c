/* VSDLSS function (decompiled by Ghidra) */
/* name: solveWithDeg3PfUpper_vsdlss  addr: 00a0b450  size: f2 */
#include "vsdlss_ref.h"

/* signature: void solveWithDeg3PfUpper_vsdlss(long param_1,long param_2,long param_3); */

void solveWithDeg3PfUpper_vsdlss(long param_1,long param_2,long param_3)

{
  long lVar1;
  long lVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  double *pdVar18;
  long lVar19;
  double dVar20;
  
  iVar6 = *(int *)(param_1 + 4);
  lVar10 = *(long *)(param_1 + 8);
  lVar11 = *(long *)(param_1 + 0x10);
  lVar12 = *(long *)(param_1 + 0x18);
  lVar13 = *(long *)(param_1 + 0x20);
  lVar14 = *(long *)(param_1 + 0x28);
  lVar15 = *(long *)(param_1 + 0x30);
  lVar16 = *(long *)(param_1 + 0x38);
  if (0 < iVar6) {
    lVar2 = (long)iVar6 * 8;
    lVar19 = (long)iVar6 * 4;
    lVar17 = 0;
    pdVar18 = (double *)(param_3 + lVar2);
    do {
      iVar7 = *(int *)(lVar11 + lVar19 + lVar17);
      dVar20 = *(double *)(param_2 + lVar2 + lVar17 * 2);
      dVar3 = *(double *)(lVar12 + lVar2 + lVar17 * 2);
      iVar8 = *(int *)(lVar13 + lVar19 + lVar17);
      dVar4 = *(double *)(lVar14 + lVar2 + lVar17 * 2);
      iVar9 = *(int *)(lVar19 + lVar15 + lVar17);
      dVar5 = *(double *)(lVar16 + lVar2 + lVar17 * 2);
      *pdVar18 = dVar20;
      if (0 < iVar7) {
        dVar20 = dVar20 - dVar3 * *(double *)(param_3 + (long)iVar7 * 8);
        *pdVar18 = dVar20;
      }
      if (0 < iVar8) {
        dVar20 = dVar20 - dVar4 * *(double *)(param_3 + (long)iVar8 * 8);
        *pdVar18 = dVar20;
      }
      if (0 < iVar9) {
        dVar20 = dVar20 - dVar5 * *(double *)(param_3 + (long)iVar9 * 8);
        *pdVar18 = dVar20;
      }
      lVar1 = lVar17 * 2;
      lVar17 = lVar17 + -4;
      *pdVar18 = dVar20 / *(double *)(lVar10 + lVar2 + lVar1);
      pdVar18 = pdVar18 + -1;
    } while (lVar17 != ~(ulong)(iVar6 - 1) * 4);
  }
  return;
}


