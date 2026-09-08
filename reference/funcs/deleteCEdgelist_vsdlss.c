/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteCEdgelist_vsdlss  addr: 009e2060  size: 15 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteCEdgelist_vsdlss(long param_1); */

undefined8 deleteCEdgelist_vsdlss(long param_1)

{
  long lVar1;
  
  lVar1 = param_1;
  *(long *)(param_1 + 8) = DAT_00ead898;
  DAT_00ead898 = lVar1;
  return 0;
}


