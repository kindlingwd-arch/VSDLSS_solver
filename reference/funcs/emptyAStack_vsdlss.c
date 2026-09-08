/* VSDLSS function (decompiled by Ghidra) */
/* name: emptyAStack_vsdlss  addr: 00a1e8f0  size: b */
#include "vsdlss_ref.h"

/* signature: bool emptyAStack_vsdlss(long param_1); */

bool emptyAStack_vsdlss(long param_1)

{
  return *(int *)(param_1 + 4) == 0;
}


