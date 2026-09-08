/* VSDLSS function (decompiled by Ghidra) */
/* name: emptyDeq_vsdlss  addr: 00a2f700  size: f */
#include "vsdlss_ref.h"

/* signature: bool emptyDeq_vsdlss(long param_1); */

bool emptyDeq_vsdlss(long param_1)

{
  return *(long *)(param_1 + 8) == *(long *)(param_1 + 0x10);
}


