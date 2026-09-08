/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDAdjustCompStarts_vsdlss  addr: 009fd5c0  size: 25 */
#include "vsdlss_ref.h"

/* signature: void MLDAdjustCompStarts_vsdlss(int param_1,int param_2,long param_3); */

void MLDAdjustCompStarts_vsdlss(int param_1,int param_2,long param_3)

{
  int *piVar1;
  
  if (0 < param_1) {
    piVar1 = (int *)(param_3 + 4);
    do {
      *piVar1 = *piVar1 + param_2 + -1;
      piVar1 = piVar1 + 1;
    } while (piVar1 != (int *)(param_3 + 8 + (ulong)(param_1 - 1) * 4));
  }
  return;
}


