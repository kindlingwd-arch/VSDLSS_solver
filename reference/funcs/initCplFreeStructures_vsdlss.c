/* VSDLSS function (decompiled by Ghidra) */
/* name: initCplFreeStructures_vsdlss  addr: 009dd2a0  size: 37 */
#include "vsdlss_ref.h"

/* signature: void initCplFreeStructures_vsdlss(ulong param_1); */

void initCplFreeStructures_vsdlss(ulong param_1)

{
  DAT_00e3be20 = createTag_vsdlss();
  DAT_00e965b0 = 0;
  DAT_00e965a8 = 1;
  if (0x1f < param_1) {
    DAT_00e965a8 = param_1 >> 4;
  }
  return;
}


