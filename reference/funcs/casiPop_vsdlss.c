/* VSDLSS function (decompiled by Ghidra) */
/* name: casiPop_vsdlss  addr: 00a2f750  size: 27 */
#include "vsdlss_ref.h"

/* signature: undefined4 casiPop_vsdlss(undefined4 *param_1); */

undefined4 casiPop_vsdlss(undefined4 *param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  
  puVar2 = *(undefined4 **)(param_1 + 2);
  if (puVar2 != *(undefined4 **)(param_1 + 4)) {
    uVar1 = *puVar2;
    *(undefined8 *)(param_1 + 2) = *(undefined8 *)(puVar2 + 2);
    deleteCEdgelist_vsdlss(puVar2);
    return uVar1;
  }
  return *param_1;
}


