/* VSDLSS function (decompiled by Ghidra) */
/* name: initCelFreeStructures_vsdlss  addr: 009e1f70  size: 37 */
#include "vsdlss_ref.h"

/* signature: void initCelFreeStructures_vsdlss(ulong param_1); */

void initCelFreeStructures_vsdlss(ulong param_1)

{
  DAT_00e3be28 = createTag_vsdlss();
  DAT_00ead898 = 0;
  DAT_00ead890 = 1;
  if (0x1f < param_1) {
    DAT_00ead890 = param_1 >> 4;
  }
  return;
}


