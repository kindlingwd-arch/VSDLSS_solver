/* VSDLSS function (decompiled by Ghidra) */
/* name: countNumberOfBlocks_vsdlss  addr: 00a221b0  size: 35 */
#include "vsdlss_ref.h"

/* signature: int countNumberOfBlocks_vsdlss(int *param_1); */

int countNumberOfBlocks_vsdlss(int *param_1)

{
  int iVar1;
  long *plVar2;
  
  if (0 < *param_1) {
    plVar2 = (long *)(*(long *)(param_1 + 2) + 8);
    iVar1 = 0;
    do {
      iVar1 = (iVar1 + 1) - (uint)(*plVar2 == 0);
      plVar2 = plVar2 + 1;
    } while (plVar2 != (long *)(*(long *)(param_1 + 2) + 0x10 + (ulong)(*param_1 - 1) * 8));
    return iVar1;
  }
  return 0;
}


