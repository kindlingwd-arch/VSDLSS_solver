/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteCharPtrList_vsdlss  addr: 009dd3e0  size: 15 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteCharPtrList_vsdlss(long param_1); */

undefined8 deleteCharPtrList_vsdlss(long param_1)

{
  long lVar1;
  
  lVar1 = param_1;
  *(long *)(param_1 + 8) = DAT_00e965b0;
  DAT_00e965b0 = lVar1;
  return 0;
}


