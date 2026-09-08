/* VSDLSS function (decompiled by Ghidra) */
/* name: pqiIncreasedKeyIter_vsdlss  addr: 00a0cd20  size: f7 */
#include "vsdlss_ref.h"

/* signature: void pqiIncreasedKeyIter_vsdlss(int param_1,long param_2); */

void pqiIncreasedKeyIter_vsdlss(int param_1,long param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int *piVar13;
  int iVar14;
  
  iVar12 = *(int *)(param_2 + 8);
  iVar11 = param_1 * 2;
  if ((iVar11 <= iVar12) && (iVar11 != 0)) {
    lVar7 = *(long *)(param_2 + 0x20);
    lVar8 = *(long *)(param_2 + 0x18);
    while( true ) {
      iVar10 = iVar11;
      if (iVar11 < iVar12) {
        piVar1 = (int *)(lVar8 + (long)iVar11 * 4);
        piVar2 = (int *)(lVar8 + 4 + (long)iVar11 * 4);
        iVar3 = *piVar1;
        iVar4 = *piVar2;
        iVar5 = *(int *)(lVar7 + (long)iVar3 * 4);
        iVar6 = *(int *)(lVar7 + (long)iVar4 * 4);
        piVar13 = piVar1;
        iVar12 = iVar3;
        iVar14 = iVar5;
        if (((iVar6 <= iVar5) &&
            (piVar13 = piVar2, iVar10 = iVar11 + 1, iVar12 = iVar4, iVar14 = iVar6, iVar5 <= iVar6))
           && (piVar13 = piVar1, iVar10 = iVar11, iVar12 = iVar3, iVar14 = iVar5, iVar4 < iVar3)) {
          piVar13 = piVar2;
          iVar10 = iVar11 + 1;
          iVar12 = iVar4;
          iVar14 = iVar6;
        }
      }
      else {
        piVar13 = (int *)(lVar8 + (long)iVar11 * 4);
        iVar12 = *piVar13;
        iVar14 = *(int *)(lVar7 + (long)iVar12 * 4);
      }
      piVar1 = (int *)(lVar8 + (long)param_1 * 4);
      iVar11 = *piVar1;
      iVar3 = *(int *)(lVar7 + (long)iVar11 * 4);
      if (((iVar3 <= iVar14) && ((iVar3 < iVar14 || (iVar11 < iVar12)))) &&
         (piVar13 = piVar1, iVar12 = iVar11, iVar10 != param_1)) break;
      *piVar1 = iVar12;
      *piVar13 = iVar11;
      lVar9 = *(long *)(param_2 + 0x10);
      *(int *)(lVar9 + (long)*piVar1 * 4) = param_1;
      *(int *)(lVar9 + (long)*piVar13 * 4) = iVar10;
      iVar12 = *(int *)(param_2 + 8);
      iVar11 = iVar10 * 2;
      if (iVar12 < iVar11) {
        return;
      }
      param_1 = iVar10;
      if (iVar11 == 0) {
        return;
      }
    }
  }
  return;
}


