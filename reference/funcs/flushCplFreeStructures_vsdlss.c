/* VSDLSS function (decompiled by Ghidra) */
/* name: flushCplFreeStructures_vsdlss  addr: 009dd2e0  size: 2e */
#include "vsdlss_ref.h"

/* signature: void flushCplFreeStructures_vsdlss(void); */

void flushCplFreeStructures_vsdlss(void)

{
  if (-1 < DAT_00e3be20) {
    deleteTag_vsdlss();
  }
  DAT_00e3be20 = createTag_vsdlss();
  DAT_00e965b0 = 0;
  return;
}


