/* VSDLSS function (decompiled by Ghidra) */
/* name: solveLowerTriangForPartFact_vsdlss  addr: 00a194c0  size: ff */
#include "vsdlss_ref.h"

/* signature: void solveLowerTriangForPartFact_vsdlss(int param_1,long param_2,long param_3); */

void solveLowerTriangForPartFact_vsdlss(int param_1,long param_2,long param_3)

{
  long lVar1;
  int *piVar2;
  double dVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  ulong uVar10;
  long lVar11;
  double *pdVar12;
  double *pdVar13;
  int iVar14;
  double *pdVar15;
  long lVar16;
  int iVar17;
  double *pdVar18;
  double dVar19;
  
  uVar10 = DAT_00abd200;
  iVar17 = *(int *)(param_2 + 8);
  lVar5 = *(long *)(param_2 + 0x30);
  lVar6 = *(long *)(param_2 + 0x38);
  lVar7 = *(long *)(param_2 + 0x28);
  if (0 < iVar17 - param_1) {
    lVar16 = 1;
    iVar14 = iVar17 - (iVar17 - param_1);
    pdVar18 = (double *)(param_3 + 0x10);
    do {
      lVar8 = *(long *)(lVar6 + lVar16 * 8);
      iVar4 = *(int *)(lVar7 + lVar16 * 4);
      dVar19 = pdVar18[-1] / *(double *)(lVar8 + 8);
      pdVar18[-1] = dVar19;
      dVar19 = (double)((ulong)dVar19 ^ uVar10);
      if (iVar17 == iVar4) {
        if (1 < iVar17) {
          pdVar15 = (double *)(lVar8 + 0x10);
          pdVar12 = pdVar18;
          do {
            dVar3 = *pdVar15;
            pdVar13 = pdVar12 + 1;
            pdVar15 = pdVar15 + 1;
            *pdVar12 = dVar3 * dVar19 + *pdVar12;
            pdVar12 = pdVar13;
          } while (pdVar13 != (double *)(param_3 + 0x10) + (ulong)(iVar17 - 2) + lVar16);
        }
      }
      else {
        lVar9 = *(long *)(lVar5 + lVar16 * 8);
        if (1 < iVar4) {
          lVar11 = 0;
          do {
            piVar2 = (int *)(lVar9 + 8 + lVar11);
            lVar1 = lVar11 * 2;
            lVar11 = lVar11 + 4;
            pdVar12 = (double *)(param_3 + (long)*piVar2 * 8);
            *pdVar12 = *(double *)(lVar8 + 0x10 + lVar1) * dVar19 + *pdVar12;
          } while (lVar11 != (ulong)(iVar4 - 2) * 4 + 4);
        }
      }
      iVar17 = iVar17 + -1;
      pdVar18 = pdVar18 + 1;
      lVar16 = lVar16 + 1;
    } while (iVar17 != iVar14);
  }
  return;
}


