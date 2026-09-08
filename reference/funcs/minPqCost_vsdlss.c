/* VSDLSS function (decompiled by Ghidra) */
/* name: minPqCost_vsdlss  addr: 00a2c6f0  size: 22 */
#include "vsdlss_ref.h"

/* signature: int minPqCost_vsdlss(int param_1,int param_2,long param_3); */

int minPqCost_vsdlss(int param_1,int param_2,long param_3)

{
  if (*(int *)(*(long *)(param_3 + 0x20) +
              (long)*(int *)(*(long *)(param_3 + 0x18) + (long)param_2 * 4) * 4) <
      *(int *)(*(long *)(param_3 + 0x20) +
              (long)*(int *)(*(long *)(param_3 + 0x18) + (long)param_1 * 4) * 4)) {
    param_1 = param_2;
  }
  return param_1;
}


