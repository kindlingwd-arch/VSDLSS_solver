/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteQ_vsdlss  addr: 00a0d680  size: 2b */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteQ_vsdlss(long param_1); */

undefined8 deleteQ_vsdlss(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 0x10) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x10) = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


