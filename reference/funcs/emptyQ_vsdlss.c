/* VSDLSS function (decompiled by Ghidra) */
/* name: emptyQ_vsdlss  addr: 00a0d790  size: c */
#include "vsdlss_ref.h"

/* signature: bool emptyQ_vsdlss(int *param_1); */

bool emptyQ_vsdlss(int *param_1)

{
  return param_1[1] == *param_1;
}


