/* VSDLSS function (decompiled by Ghidra) */
/* name: getMaxIVector_vsdlss  addr: 009d8a00  size: 36 */
#include "vsdlss_ref.h"

/* signature: int getMaxIVector_vsdlss(long param_1,int param_2); */

int getMaxIVector_vsdlss(long param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  
  if (param_2 < 1) {
    iVar1 = 0;
  }
  else {
    iVar1 = *(int *)(param_1 + 4);
    if (param_2 != 1) {
      piVar2 = (int *)(param_1 + 8);
      do {
        if (iVar1 < *piVar2) {
          iVar1 = *piVar2;
        }
        piVar2 = piVar2 + 1;
      } while (piVar2 != (int *)(param_1 + 0xc + (ulong)(param_2 - 2) * 4));
      return iVar1;
    }
  }
  return iVar1;
}


