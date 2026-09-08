/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteElemArr_vsdlss  addr: 009e6610  size: 83 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteElemArr_vsdlss(long param_1); */

undefined8 deleteElemArr_vsdlss(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 0x18) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x18) = 0;
    if (*(long *)(param_1 + 0x20) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x20) = 0;
    if (*(long *)(param_1 + 0x28) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x28) = 0;
    if (*(long *)(param_1 + 0x30) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x30) = 0;
    if (*(long *)(param_1 + 0x38) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


