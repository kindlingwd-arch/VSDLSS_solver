/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteMLDGraph_vsdlss  addr: 009fc590  size: b3 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteMLDGraph_vsdlss(long param_1); */

undefined8 deleteMLDGraph_vsdlss(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 0x28) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x28) = 0;
    if (*(long *)(param_1 + 0x38) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    if (*(long *)(param_1 + 0x40) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x40) = 0;
    if (*(long *)(param_1 + 0x48) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x48) = 0;
    if (*(long *)(param_1 + 0x50) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x50) = 0;
    if (*(long *)(param_1 + 0x58) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x58) = 0;
    if (*(long *)(param_1 + 0x60) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x60) = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


