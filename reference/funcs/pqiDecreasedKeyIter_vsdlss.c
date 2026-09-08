/* VSDLSS function (decompiled by Ghidra) */
/* name: pqiDecreasedKeyIter_vsdlss  addr: 00a0ce30  size: 6b */
#include "vsdlss_ref.h"

/* signature: void pqiDecreasedKeyIter_vsdlss(int param_1,long param_2); */

void pqiDecreasedKeyIter_vsdlss(int param_1,long param_2)

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
  
  if (1 < param_1) {
    lVar7 = *(long *)(param_2 + 0x18);
    lVar8 = *(long *)(param_2 + 0x20);
    do {
      iVar10 = param_1 >> 1;
      piVar1 = (int *)(lVar7 + (long)param_1 * 4);
      piVar2 = (int *)(lVar7 + (long)iVar10 * 4);
      iVar3 = *piVar1;
      iVar4 = *piVar2;
      iVar5 = *(int *)(lVar8 + (long)iVar4 * 4);
      iVar6 = *(int *)(lVar8 + (long)iVar3 * 4);
      if ((iVar5 <= iVar6) && (((iVar5 < iVar6 || (iVar4 < iVar3)) && (iVar10 != param_1)))) {
        return;
      }
      *piVar1 = iVar4;
      *piVar2 = iVar3;
      lVar9 = *(long *)(param_2 + 0x10);
      *(int *)(lVar9 + (long)*piVar1 * 4) = param_1;
      *(int *)(lVar9 + (long)*piVar2 * 4) = iVar10;
      param_1 = iVar10;
    } while (1 < iVar10);
  }
  return;
}


