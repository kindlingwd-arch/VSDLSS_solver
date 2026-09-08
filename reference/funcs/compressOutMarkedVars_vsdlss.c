/* VSDLSS function (decompiled by Ghidra) */
/* name: compressOutMarkedVars_vsdlss  addr: 009d97c0  size: 3c */
#include "vsdlss_ref.h"

/* signature: int compressOutMarkedVars_vsdlss(long param_1,int param_2,long param_3); */

int compressOutMarkedVars_vsdlss(long param_1,int param_2,long param_3)

{
  int iVar1;
  int *piVar2;
  
  if (0 < param_2) {
    piVar2 = (int *)(param_3 + 4);
    iVar1 = 0;
    do {
      if (*(char *)(param_1 + *piVar2) == '\0') {
        iVar1 = iVar1 + 1;
        *(int *)(param_3 + (long)iVar1 * 4) = *piVar2;
      }
      piVar2 = piVar2 + 1;
    } while (piVar2 != (int *)(param_3 + 8 + (ulong)(param_2 - 1) * 4));
    return iVar1;
  }
  return 0;
}


