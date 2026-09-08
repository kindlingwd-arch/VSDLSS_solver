/* VSDLSS function (decompiled by Ghidra) */
/* name: getMaxSupNodeNZ_vsdlss  addr: 00a18fd0  size: 61 */
#include "vsdlss_ref.h"

/* signature: int getMaxSupNodeNZ_vsdlss(long param_1); */

int getMaxSupNodeNZ_vsdlss(long param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  if (0 < *(int *)(param_1 + 0x14)) {
    piVar4 = (int *)(*(long *)(param_1 + 0x20) + 4);
    iVar5 = 1;
    iVar2 = 0;
    do {
      iVar1 = *piVar4;
      iVar3 = (*(int *)(*(long *)(param_1 + 0x28) + (long)iVar5 * 4) + 1) * iVar1 -
              ((iVar1 + -1) * iVar1) / 2;
      if (iVar2 < iVar3) {
        iVar2 = iVar3;
      }
      piVar4 = piVar4 + 1;
      iVar5 = iVar5 + iVar1;
    } while (piVar4 != (int *)(*(long *)(param_1 + 0x20) + 8 +
                              (ulong)(*(int *)(param_1 + 0x14) - 1) * 4));
    return iVar2;
  }
  return 0;
}


