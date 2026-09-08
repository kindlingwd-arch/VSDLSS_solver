/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteDeg1Deg2Pf_vsdlss  addr: 009e3520  size: 57 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteDeg1Deg2Pf_vsdlss(long param_1); */

undefined8 deleteDeg1Deg2Pf_vsdlss(long param_1)

{
  if (param_1 != 0) {
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
    casiFree_vsdlss(param_1);
  }
  return 0;
}


