/* VSDLSS function (decompiled by Ghidra) */
/* name: checkOffDiagOrder_vsdlss  addr: 009e67f0  size: 77 */
#include "vsdlss_ref.h"

/* signature: void checkOffDiagOrder_vsdlss(int *param_1); */

void checkOffDiagOrder_vsdlss(int *param_1)

{
  int iVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  int iVar6;
  int iVar7;
  
  iVar1 = *param_1;
  lVar5 = 0;
  lVar3 = *(long *)(param_1 + 6);
  lVar4 = *(long *)(param_1 + 10);
  iVar6 = 2;
  if (1 < iVar1) {
    do {
      while ((iVar7 = *(int *)(lVar3 + 4 + lVar5), iVar2 = *(int *)(lVar3 + 8 + lVar5),
             iVar7 <= iVar2 &&
             ((iVar2 != iVar7 || (*(int *)(lVar4 + 4 + lVar5) <= *(int *)(lVar4 + 8 + lVar5)))))) {
        iVar6 = iVar6 + 1;
        lVar5 = lVar5 + 4;
        if (iVar1 < iVar6) {
          return;
        }
      }
      iVar7 = iVar6 + 1;
      nrerror_vsdlss(0x1e,"elemarrutils.c",0xd0,iVar6);
      lVar5 = lVar5 + 4;
      iVar6 = iVar7;
    } while (iVar7 <= iVar1);
  }
  return;
}


