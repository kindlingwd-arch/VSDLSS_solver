/* VSDLSS function (decompiled by Ghidra) */
/* name: markCountingMark_vsdlss  addr: 00a1f320  size: 44 */
#include "vsdlss_ref.h"

/* signature: void markCountingMark_vsdlss(int *param_1,int param_2); */

void markCountingMark_vsdlss(int *param_1,int param_2)

{
  int *piVar1;
  
  if ((param_2 < 1) || (param_1[1] < param_2)) {
    nrerror_vsdlss(1,"countingmarkutils.c",0x8b);
  }
  piVar1 = (int *)(*(long *)(param_1 + 2) + (long)param_2 * 4);
  if (*piVar1 < *param_1) {
    *piVar1 = *param_1;
  }
  return;
}


