/* VSDLSS function (decompiled by Ghidra) */
/* name: flushCelFreeStructures_vsdlss  addr: 009e1fb0  size: 2e */
#include "vsdlss_ref.h"

/* signature: void flushCelFreeStructures_vsdlss(void); */

void flushCelFreeStructures_vsdlss(void)

{
  if (-1 < DAT_00e3be28) {
    deleteTag_vsdlss();
  }
  DAT_00e3be28 = createTag_vsdlss();
  DAT_00ead898 = 0;
  return;
}


