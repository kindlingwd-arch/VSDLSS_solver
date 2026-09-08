/* VSDLSS function (decompiled by Ghidra) */
/* name: renameIVector_vsdlss  addr: 009d8ed0  size: 25 */
#include "vsdlss_ref.h"

/* signature: void renameIVector_vsdlss(long param_1,int param_2,long param_3); */

void renameIVector_vsdlss(long param_1,int param_2,long param_3)

{
  ulong uVar1;
  
  uVar1 = 1;
  if ((long)param_2 != 0) {
    do {
      *(undefined4 *)(param_3 + uVar1 * 4) =
           *(undefined4 *)(param_1 + (long)*(int *)(param_3 + uVar1 * 4) * 4);
      uVar1 = uVar1 + 1;
    } while (uVar1 <= (ulong)(long)param_2);
  }
  return;
}


