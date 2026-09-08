/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteEntireCEdgelist_vsdlss  addr: 009e2110  size: 2b */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteEntireCEdgelist_vsdlss(long param_1); */

undefined8 deleteEntireCEdgelist_vsdlss(long param_1)

{
  long lVar1;
  
  while (lVar1 = param_1, lVar1 != 0) {
    param_1 = *(long *)(lVar1 + 8);
    *(long *)(lVar1 + 8) = DAT_00ead898;
    DAT_00ead898 = lVar1;
  }
  return 0;
}


