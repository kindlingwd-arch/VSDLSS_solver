/* VSDLSS function (decompiled by Ghidra) */
/* name: getCEdgelistSize_vsdlss  addr: 009e2240  size: 20 */
#include "vsdlss_ref.h"

/* signature: int getCEdgelistSize_vsdlss(long param_1); */

int getCEdgelistSize_vsdlss(long param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if (param_1 == 0) {
    return 0;
  }
  do {
    param_1 = *(long *)(param_1 + 8);
    iVar1 = iVar1 + 1;
  } while (param_1 != 0);
  return iVar1;
}


