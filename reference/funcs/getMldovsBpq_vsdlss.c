/* VSDLSS function (decompiled by Ghidra) */
/* name: getMldovsBpq_vsdlss  addr: 00a2ae40  size: 2a */
#include "vsdlss_ref.h"

/* signature: long getMldovsBpq_vsdlss(undefined8 param_1,long param_2); */

long getMldovsBpq_vsdlss(undefined8 param_1,long param_2)

{
  long lVar1;
  
  lVar1 = *(long *)(param_2 + 0x38);
  *(undefined8 *)(lVar1 + 0x18) = *(undefined8 *)(param_2 + 0x60);
  *(undefined8 *)(lVar1 + 0x20) = *(undefined8 *)(param_2 + 0x68);
  *(undefined8 *)(lVar1 + 0x28) = *(undefined8 *)(param_2 + 0x70);
  initBpq_vsdlss(param_1,lVar1);
  return lVar1;
}


