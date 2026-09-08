/* VSDLSS function (decompiled by Ghidra) */
/* name: addIVectors_vsdlss  addr: 009d7ce0  size: 27 */
#include "vsdlss_ref.h"

/* signature: void addIVectors_vsdlss(long param_1,long param_2,int param_3,long param_4); */

void addIVectors_vsdlss(long param_1,long param_2,int param_3,long param_4)

{
  ulong uVar1;
  
  uVar1 = 1;
  if ((long)param_3 != 0) {
    do {
      *(int *)(param_4 + uVar1 * 4) = *(int *)(param_1 + uVar1 * 4) + *(int *)(param_2 + uVar1 * 4);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= (ulong)(long)param_3);
  }
  return;
}


