/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteDoubleEndedQ_vsdlss  addr: 00a2f610  size: 36 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteDoubleEndedQ_vsdlss(long param_1); */

undefined8 deleteDoubleEndedQ_vsdlss(long param_1)

{
  if (param_1 != 0) {
    deleteEntireCEdgelist_vsdlss(*(undefined8 *)(param_1 + 8));
    *(undefined8 *)(param_1 + 8) = 0;
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


