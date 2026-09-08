/* VSDLSS function (decompiled by Ghidra) */
/* name: getMaxChunkRows_vsdlss  addr: 009e2c40  size: 3f */
#include "vsdlss_ref.h"

/* signature: int getMaxChunkRows_vsdlss(long param_1); */

int getMaxChunkRows_vsdlss(long param_1)

{
  int iVar1;
  int iVar2;
  long lVar3;
  
  if (0 < *(int *)(param_1 + 8)) {
    lVar3 = 0;
    iVar1 = 0;
    do {
      iVar2 = (*(int *)(*(long *)(param_1 + 0x60) + 4 + lVar3) -
              *(int *)(*(long *)(param_1 + 0x50) + 4 + lVar3)) + 1;
      if (iVar1 < iVar2) {
        iVar1 = iVar2;
      }
      lVar3 = lVar3 + 4;
    } while (lVar3 != (ulong)(*(int *)(param_1 + 8) - 1) * 4 + 4);
    return iVar1;
  }
  return 0;
}


