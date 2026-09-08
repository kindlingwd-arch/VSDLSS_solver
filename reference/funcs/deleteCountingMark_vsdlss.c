/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteCountingMark_vsdlss  addr: 00a1f2a0  size: 2b */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteCountingMark_vsdlss(long param_1); */

undefined8 deleteCountingMark_vsdlss(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 8) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 8) = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


