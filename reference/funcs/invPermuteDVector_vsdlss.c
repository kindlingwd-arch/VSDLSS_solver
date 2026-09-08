/* VSDLSS function (decompiled by Ghidra) */
/* name: invPermuteDVector_vsdlss  addr: 009d92f0  size: 2a */
#include "vsdlss_ref.h"

/* signature: void invPermuteDVector_vsdlss(long param_1,int param_2,long param_3,long param_4); */

void invPermuteDVector_vsdlss(long param_1,int param_2,long param_3,long param_4)

{
  ulong uVar1;
  
  uVar1 = 1;
  if ((long)param_2 != 0) {
    do {
      *(undefined8 *)(param_4 + uVar1 * 8) =
           *(undefined8 *)(param_3 + (long)*(int *)(param_1 + uVar1 * 4) * 8);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= (ulong)(long)param_2);
  }
  return;
}


