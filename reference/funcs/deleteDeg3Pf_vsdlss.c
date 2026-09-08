/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteDeg3Pf_vsdlss  addr: 009e3850  size: b3 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteDeg3Pf_vsdlss(long param_1); */

undefined8 deleteDeg3Pf_vsdlss(long param_1)

{
  if (param_1 != 0) {
    if (*(long *)(param_1 + 8) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 8) = 0;
    if (*(long *)(param_1 + 0x10) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 0x10) = 0;
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


