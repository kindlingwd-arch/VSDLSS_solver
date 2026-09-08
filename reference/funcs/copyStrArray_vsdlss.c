/* VSDLSS function (decompiled by Ghidra) */
/* name: copyStrArray_vsdlss  addr: 009d9410  size: 4c */
#include "vsdlss_ref.h"

/* signature: void copyStrArray_vsdlss(long param_1,ulong param_2,long param_3); */

void copyStrArray_vsdlss(long param_1,ulong param_2,long param_3)

{
  ulong uVar1;
  ulong uVar2;
  bool bVar3;
  
  uVar1 = 1;
  uVar2 = 2;
  if (param_2 != 0) {
    do {
      strcpy(*(char **)(param_3 + uVar1 * 8),*(char **)(param_1 + uVar1 * 8));
      bVar3 = uVar2 <= param_2;
      uVar1 = uVar2;
      uVar2 = uVar2 + 1;
    } while (bVar3);
  }
  return;
}


