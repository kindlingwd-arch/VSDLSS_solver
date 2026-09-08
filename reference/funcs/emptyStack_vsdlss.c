/* VSDLSS function (decompiled by Ghidra) */
/* name: emptyStack_vsdlss  addr: 00a2f7a0  size: f */
#include "vsdlss_ref.h"

/* signature: bool emptyStack_vsdlss(long param_1); */

bool emptyStack_vsdlss(long param_1)

{
  return *(long *)(param_1 + 8) == *(long *)(param_1 + 0x10);
}


