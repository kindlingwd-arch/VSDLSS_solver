/* VSDLSS function (decompiled by Ghidra) */
/* name: solveLoadCase_vsdlss  addr: 00a195d0  size: 1d5 */
#include "vsdlss_ref.h"

/* signature: void solveLoadCase_vsdlss(long param_1,long param_2); */

void solveLoadCase_vsdlss(long param_1,long param_2)

{
  double *pdVar1;
  long lVar2;
  int *piVar3;
  double dVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  long lVar13;
  double *pdVar14;
  double *pdVar15;
  double *pdVar16;
  int iVar17;
  double *pdVar18;
  long lVar19;
  double dVar20;
  
  uVar11 = DAT_00abd200;
  iVar5 = *(int *)(param_1 + 8);
  lVar7 = *(long *)(param_1 + 0x30);
  lVar8 = *(long *)(param_1 + 0x38);
  lVar9 = *(long *)(param_1 + 0x28);
  if (0 < iVar5) {
    pdVar1 = (double *)(param_2 + 0x10);
    lVar19 = 1;
    pdVar18 = pdVar1;
    iVar17 = iVar5;
    do {
      while( true ) {
        iVar6 = *(int *)(lVar9 + lVar19 * 4);
        lVar10 = *(long *)(lVar8 + lVar19 * 8);
        dVar20 = (double)((ulong)pdVar18[-1] ^ uVar11) / *(double *)(lVar10 + 8);
        if (iVar17 == iVar6) break;
        lVar13 = *(long *)(lVar7 + lVar19 * 8);
        if (1 < iVar6) {
          lVar12 = 0;
          do {
            pdVar14 = (double *)(param_2 + (long)*(int *)(lVar13 + 8 + lVar12) * 8);
            lVar2 = lVar12 * 2;
            lVar12 = lVar12 + 4;
            *pdVar14 = *(double *)(lVar10 + 0x10 + lVar2) * dVar20 + *pdVar14;
          } while (lVar12 != (ulong)(iVar6 - 2) * 4 + 4);
        }
LAB_00a1966c:
        pdVar18 = pdVar18 + 1;
        lVar19 = lVar19 + 1;
        iVar17 = iVar17 + -1;
        if (iVar17 == 0) goto LAB_00a19679;
      }
      if (iVar17 < 2) goto LAB_00a1966c;
      pdVar16 = (double *)(lVar10 + 0x10);
      pdVar14 = pdVar18;
      do {
        dVar4 = *pdVar16;
        pdVar15 = pdVar14 + 1;
        pdVar16 = pdVar16 + 1;
        *pdVar14 = dVar4 * dVar20 + *pdVar14;
        pdVar14 = pdVar15;
      } while (pdVar15 != pdVar1 + (ulong)(iVar17 - 2) + lVar19);
      pdVar18 = pdVar18 + 1;
      lVar19 = lVar19 + 1;
      iVar17 = iVar17 + -1;
    } while (iVar17 != 0);
LAB_00a19679:
    lVar19 = (long)iVar5;
    pdVar18 = (double *)(param_2 + 8 + lVar19 * 8);
    iVar17 = 1;
    do {
      iVar6 = *(int *)(lVar9 + lVar19 * 4);
      lVar10 = *(long *)(lVar8 + lVar19 * 8);
      dVar20 = 0.0;
      if (iVar17 == iVar6) {
        if (iVar17 != 1) {
          dVar20 = 0.0;
          pdVar14 = pdVar18;
          pdVar16 = (double *)(lVar10 + 0x10);
          do {
            dVar4 = *pdVar14;
            pdVar14 = pdVar14 + 1;
            dVar20 = dVar20 + dVar4 * *pdVar16;
            pdVar16 = pdVar16 + 1;
          } while (pdVar14 != pdVar1 + (ulong)(iVar17 - 2) + lVar19);
        }
      }
      else if (1 < iVar6) {
        dVar20 = 0.0;
        lVar13 = 0;
        do {
          piVar3 = (int *)(*(long *)(lVar7 + lVar19 * 8) + 8 + lVar13);
          lVar12 = lVar13 * 2;
          lVar13 = lVar13 + 4;
          dVar20 = dVar20 + *(double *)(param_2 + (long)*piVar3 * 8) *
                            *(double *)(lVar10 + 0x10 + lVar12);
        } while (lVar13 != (ulong)(iVar6 - 2) * 4 + 4);
      }
      pdVar14 = pdVar18 + -1;
      iVar17 = iVar17 + 1;
      pdVar18 = pdVar18 + -1;
      lVar19 = lVar19 + -1;
      *pdVar18 = (*pdVar14 - dVar20) / *(double *)(lVar10 + 8);
    } while (iVar17 != iVar5 + 1);
  }
  return;
}


