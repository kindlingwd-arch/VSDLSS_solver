/* VSDLSS function (decompiled by Ghidra) */
/* name: getMldovsCountingMark_vsdlss  addr: 009fce70  size: 1d */
#include "vsdlss_ref.h"

/* signature: long getMldovsCountingMark_vsdlss(undefined8 param_1,long param_2); */

long getMldovsCountingMark_vsdlss(undefined8 param_1,long param_2)

{
  long lVar1;
  
  lVar1 = *(long *)(param_2 + 0x40);
  *(undefined8 *)(lVar1 + 8) = *(undefined8 *)(param_2 + 0x98);
  initCountingMark_vsdlss(param_1,lVar1);
  return lVar1;
}


