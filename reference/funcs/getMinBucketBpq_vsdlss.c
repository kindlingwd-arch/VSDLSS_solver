/* VSDLSS function (decompiled by Ghidra) */
/* name: getMinBucketBpq_vsdlss  addr: 00a1f0c0  size: 42 */
#include "vsdlss_ref.h"

/* signature: int getMinBucketBpq_vsdlss(long param_1); */

int getMinBucketBpq_vsdlss(long param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  if (*(int *)(param_1 + 4) < 1) {
    return 0;
  }
  iVar1 = *(int *)(*(long *)(param_1 + 0x18) + 4);
  if (iVar1 == *(int *)(param_1 + 0x10)) {
    iVar3 = 1;
    piVar4 = (int *)(*(long *)(param_1 + 0x18) + 8);
    do {
      iVar3 = iVar3 + 1;
      if (*(int *)(param_1 + 4) < iVar3) {
        return 0;
      }
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
    } while (iVar2 == iVar1);
  }
  else {
    iVar3 = 1;
  }
  return iVar3;
}


