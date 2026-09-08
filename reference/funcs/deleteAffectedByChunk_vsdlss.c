/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteAffectedByChunk_vsdlss  addr: 00a1bef0  size: 41 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteAffectedByChunk_vsdlss(long param_1); */

undefined8 deleteAffectedByChunk_vsdlss(long param_1)

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
    casiFree_vsdlss(param_1);
  }
  return 0;
}


