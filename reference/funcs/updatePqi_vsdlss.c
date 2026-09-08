/* VSDLSS function (decompiled by Ghidra) */
/* name: updatePqi_vsdlss  addr: 00a0ceb0  size: 16f */
#include "vsdlss_ref.h"

/* signature: void updatePqi_vsdlss(int param_1,int param_2,long param_3); */

void updatePqi_vsdlss(int param_1,int param_2,long param_3)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int iVar13;
  long lVar14;
  int iVar15;
  
  lVar14 = (long)param_1;
  lVar7 = *(long *)(param_3 + 0x20);
  piVar12 = (int *)(lVar7 + lVar14 * 4);
  if (param_2 < *piVar12) {
    lVar8 = *(long *)(param_3 + 0x10);
    iVar11 = *(int *)(lVar8 + lVar14 * 4);
    *piVar12 = param_2;
    if (1 < iVar11) {
      lVar14 = *(long *)(param_3 + 0x18);
      do {
        iVar9 = iVar11 >> 1;
        piVar12 = (int *)(lVar14 + (long)iVar11 * 4);
        piVar1 = (int *)(lVar14 + (long)iVar9 * 4);
        iVar13 = *piVar12;
        iVar3 = *piVar1;
        iVar10 = *(int *)(lVar7 + (long)iVar3 * 4);
        iVar15 = *(int *)(lVar7 + (long)iVar13 * 4);
        if (((iVar10 <= iVar15) && ((iVar10 < iVar15 || (iVar3 < iVar13)))) && (iVar9 != iVar11)) {
          return;
        }
        *piVar12 = iVar3;
        *piVar1 = iVar13;
        *(int *)(lVar8 + (long)*piVar12 * 4) = iVar11;
        *(int *)(lVar8 + (long)*piVar1 * 4) = iVar9;
        iVar11 = iVar9;
      } while (1 < iVar9);
    }
  }
  else if (*piVar12 < param_2) {
    lVar8 = *(long *)(param_3 + 0x10);
    iVar11 = *(int *)(lVar8 + lVar14 * 4);
    *piVar12 = param_2;
    iVar13 = *(int *)(param_3 + 8);
    iVar3 = iVar11 * 2;
    if ((iVar3 <= iVar13) && (iVar3 != 0)) {
      lVar14 = *(long *)(param_3 + 0x18);
      while( true ) {
        iVar10 = iVar3;
        if (iVar3 < iVar13) {
          piVar1 = (int *)(lVar14 + (long)iVar3 * 4);
          piVar2 = (int *)(lVar14 + 4 + (long)iVar3 * 4);
          iVar9 = *piVar1;
          iVar4 = *piVar2;
          iVar5 = *(int *)(lVar7 + (long)iVar9 * 4);
          iVar6 = *(int *)(lVar7 + (long)iVar4 * 4);
          piVar12 = piVar1;
          iVar13 = iVar9;
          iVar15 = iVar5;
          if (((iVar6 <= iVar5) &&
              (piVar12 = piVar2, iVar13 = iVar4, iVar15 = iVar6, iVar10 = iVar3 + 1, iVar5 <= iVar6)
              ) && (piVar12 = piVar1, iVar13 = iVar9, iVar15 = iVar5, iVar10 = iVar3, iVar4 < iVar9)
             ) {
            piVar12 = piVar2;
            iVar13 = iVar4;
            iVar15 = iVar6;
            iVar10 = iVar3 + 1;
          }
        }
        else {
          piVar12 = (int *)(lVar14 + (long)iVar3 * 4);
          iVar13 = *piVar12;
          iVar15 = *(int *)(lVar7 + (long)iVar13 * 4);
        }
        piVar1 = (int *)(lVar14 + (long)iVar11 * 4);
        iVar3 = *piVar1;
        iVar9 = *(int *)(lVar7 + (long)iVar3 * 4);
        if (((iVar9 <= iVar15) && ((iVar9 < iVar15 || (iVar3 < iVar13)))) &&
           (piVar12 = piVar1, iVar13 = iVar3, iVar10 != iVar11)) break;
        *piVar1 = iVar13;
        *piVar12 = iVar3;
        *(int *)(lVar8 + (long)*piVar1 * 4) = iVar11;
        *(int *)(lVar8 + (long)*piVar12 * 4) = iVar10;
        iVar13 = *(int *)(param_3 + 8);
        iVar3 = iVar10 * 2;
        if (iVar13 < iVar3) {
          return;
        }
        iVar11 = iVar10;
        if (iVar3 == 0) {
          return;
        }
      }
    }
  }
  return;
}


