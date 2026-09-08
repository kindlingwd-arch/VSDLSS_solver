/* VSDLSS function (decompiled by Ghidra) */
/* name: snipDispCharPtrListElem_vsdlss  addr: 009dd430  size: 3d */
#include "vsdlss_ref.h"

/* signature: undefined8 snipDispCharPtrListElem_vsdlss(undefined8 *param_1,long param_2,long param_3); */

undefined8 snipDispCharPtrListElem_vsdlss(undefined8 *param_1,long param_2,long param_3)

{
  long lVar1;
  
  if (param_3 != 0) {
    *(undefined8 *)(param_3 + 8) = *(undefined8 *)(param_2 + 8);
    lVar1 = param_2;
    *(long *)(param_2 + 8) = DAT_00e965b0;
    DAT_00e965b0 = lVar1;
    return *(undefined8 *)(param_3 + 8);
  }
  *param_1 = *(undefined8 *)(param_2 + 8);
  lVar1 = param_2;
  *(long *)(param_2 + 8) = DAT_00e965b0;
  DAT_00e965b0 = lVar1;
  return *param_1;
}


