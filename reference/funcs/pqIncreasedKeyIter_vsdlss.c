/* VSDLSS function (decompiled by Ghidra) */
/* name: pqIncreasedKeyIter_vsdlss  addr: 00a2c760  size: bd */
#include "vsdlss_ref.h"

/* signature: void pqIncreasedKeyIter_vsdlss(int param_1,long param_2); */

void pqIncreasedKeyIter_vsdlss(int param_1,long param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  int iVar11;
  int iVar12;
  
  iVar9 = param_1 * 2;
  iVar2 = *(int *)(param_2 + 8);
  lVar5 = *(long *)(param_2 + 0x18);
  lVar6 = *(long *)(param_2 + 0x20);
  lVar7 = *(long *)(param_2 + 0x10);
  if ((iVar9 <= iVar2) && (iVar9 != 0)) {
    while( true ) {
      iVar8 = iVar9;
      if (iVar9 < iVar2) {
        piVar10 = (int *)(lVar5 + (long)iVar9 * 4);
        piVar1 = (int *)(lVar5 + 4 + (long)iVar9 * 4);
        iVar11 = *piVar10;
        iVar12 = *(int *)(lVar6 + (long)iVar11 * 4);
        iVar3 = *piVar1;
        iVar4 = *(int *)(lVar6 + (long)iVar3 * 4);
        if (iVar4 < iVar12) {
          piVar10 = piVar1;
          iVar8 = iVar9 + 1;
          iVar11 = iVar3;
          iVar12 = iVar4;
        }
      }
      else {
        piVar10 = (int *)(lVar5 + (long)iVar9 * 4);
        iVar11 = *piVar10;
        iVar12 = *(int *)(lVar6 + (long)iVar11 * 4);
      }
      piVar1 = (int *)(lVar5 + (long)param_1 * 4);
      iVar9 = *piVar1;
      if ((*(int *)(lVar6 + (long)iVar9 * 4) < iVar12) &&
         (piVar10 = piVar1, iVar11 = iVar9, iVar8 != param_1)) break;
      *piVar1 = iVar11;
      *piVar10 = iVar9;
      *(int *)(lVar7 + (long)*piVar1 * 4) = param_1;
      *(int *)(lVar7 + (long)*piVar10 * 4) = iVar8;
      iVar9 = iVar8 * 2;
      if (iVar2 < iVar9) {
        return;
      }
      param_1 = iVar8;
      if (iVar9 == 0) {
        return;
      }
    }
  }
  return;
}


