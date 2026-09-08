/* VSDLSS function (decompiled by Ghidra) */
/* name: minPq_vsdlss  addr: 00a2c9f0  size: 23 */
#include "vsdlss_ref.h"

/* signature: void minPq_vsdlss(long param_1,uint *param_2); */

void minPq_vsdlss(long param_1,uint *param_2)

{
  if (*(int *)(param_1 + 8) == 0) {
    *param_2 = ~*(uint *)(param_1 + 4);
    return;
  }
  *param_2 = *(uint *)(*(long *)(param_1 + 0x20) + (long)*(int *)(*(long *)(param_1 + 0x18) + 4) * 4
                      );
  return;
}


