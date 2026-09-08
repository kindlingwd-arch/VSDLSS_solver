/* VSDLSS function (decompiled by Ghidra) */
/* name: copyCEdgelistToArray_vsdlss  addr: 009e2140  size: 2a */
#include "vsdlss_ref.h"

/* signature: int copyCEdgelistToArray_vsdlss(undefined4 *param_1,undefined4 *param_2); */

int copyCEdgelistToArray_vsdlss(undefined4 *param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  int iVar2;
  
  if (param_1 != (undefined4 *)0x0) {
    iVar2 = 0;
    do {
      param_2 = param_2 + 1;
      uVar1 = *param_1;
      param_1 = *(undefined4 **)(param_1 + 2);
      iVar2 = iVar2 + 1;
      *param_2 = uVar1;
    } while (param_1 != (undefined4 *)0x0);
    return iVar2;
  }
  return 0;
}


