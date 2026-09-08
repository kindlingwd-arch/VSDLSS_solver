/* VSDLSS function (decompiled by Ghidra) */
/* name: newCEdgelist_vsdlss  addr: 009e1fe0  size: 65 */
#include "vsdlss_ref.h"

/* signature: void newCEdgelist_vsdlss(undefined4 param_1,undefined8 param_2); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void newCEdgelist_vsdlss(undefined4 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  if (DAT_00e3be28 < 0) {
    uVar1 = getMemAllocBlockSize_vsdlss();
    initCelFreeStructures_vsdlss(uVar1);
  }
  _DAT_00ead888 = _DAT_00ead888 + 1;
  if (DAT_00ead898 == (undefined4 *)0x0) {
    DAT_00ead898 = (undefined4 *)newCEdgeChunk_vsdlss();
  }
  uVar1 = *(undefined8 *)(DAT_00ead898 + 2);
  *DAT_00ead898 = param_1;
  *(undefined8 *)(DAT_00ead898 + 2) = param_2;
  DAT_00ead898 = (undefined4 *)uVar1;
  return;
}


