/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteListMark_vsdlss  addr: 009f6ab0  size: 30 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteListMark_vsdlss(long param_1); */

undefined8 deleteListMark_vsdlss(long param_1)

{
  if (param_1 != 0) {
    unmarkListMark_vsdlss();
    if (*(long *)(param_1 + 8) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 8) = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


