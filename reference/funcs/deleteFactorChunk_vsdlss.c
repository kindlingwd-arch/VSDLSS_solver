/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteFactorChunk_vsdlss  addr: 009f24b0  size: 2a */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteFactorChunk_vsdlss(undefined4 *param_1); */

undefined8 deleteFactorChunk_vsdlss(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    if (*(long *)(param_1 + 8) != 0) {
      casiFree_vsdlss();
    }
    *(undefined8 *)(param_1 + 8) = 0;
    deleteTag_vsdlss(*param_1);
  }
  return 0;
}


