/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteStack_vsdlss  addr: 00a2f720  size: 5 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteStack_vsdlss(long param_1); */

undefined8 deleteStack_vsdlss(long param_1)

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


