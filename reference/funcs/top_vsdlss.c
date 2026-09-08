/* VSDLSS function (decompiled by Ghidra) */
/* name: top_vsdlss  addr: 00a2f780  size: 10 */
#include "vsdlss_ref.h"

/* signature: undefined4 top_vsdlss(undefined4 *param_1); */

undefined4 top_vsdlss(undefined4 *param_1)

{
  if (*(undefined4 **)(param_1 + 2) != *(undefined4 **)(param_1 + 4)) {
    return **(undefined4 **)(param_1 + 2);
  }
  return *param_1;
}


