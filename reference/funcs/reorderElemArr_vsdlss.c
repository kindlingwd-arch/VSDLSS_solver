/* VSDLSS function (decompiled by Ghidra) */
/* name: reorderElemArr_vsdlss  addr: 009e6b20  size: 50 */
#include "vsdlss_ref.h"

/* signature: void reorderElemArr_vsdlss(long param_1,int *param_2); */

void reorderElemArr_vsdlss(long param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  int iVar6;
  int iVar7;
  
  iVar1 = *param_2;
  lVar3 = *(long *)(param_2 + 6);
  lVar4 = *(long *)(param_2 + 10);
  if (0 < iVar1) {
    lVar5 = 0;
    do {
      iVar2 = *(int *)(param_1 + (long)*(int *)(lVar3 + 4 + lVar5) * 4);
      iVar6 = *(int *)(param_1 + (long)*(int *)(lVar4 + 4 + lVar5) * 4);
      iVar7 = iVar2;
      if (iVar2 <= iVar6) {
        iVar7 = iVar6;
        iVar6 = iVar2;
      }
      *(int *)(lVar3 + 4 + lVar5) = iVar6;
      *(int *)(lVar4 + 4 + lVar5) = iVar7;
      lVar5 = lVar5 + 4;
    } while (lVar5 != (ulong)(iVar1 - 1) * 4 + 4);
  }
  return;
}


