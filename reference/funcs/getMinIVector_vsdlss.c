/* VSDLSS function (decompiled by Ghidra) */
/* name: getMinIVector_vsdlss  addr: 009d8a40  size: 4d */
#include "vsdlss_ref.h"

/* signature: int getMinIVector_vsdlss(long param_1,int param_2,undefined4 *param_3); */

int getMinIVector_vsdlss(long param_1,int param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  ulong uVar3;
  undefined4 uVar4;
  ulong uVar5;
  
  if (0 < param_2) {
    iVar2 = *(int *)(param_1 + 4);
    if ((long)param_2 == 1) {
      uVar4 = 1;
    }
    else {
      uVar3 = 2;
      uVar5 = 1;
      do {
        iVar1 = *(int *)(param_1 + uVar3 * 4);
        if (iVar1 < iVar2) {
          uVar5 = uVar3 & 0xffffffff;
          iVar2 = iVar1;
        }
        uVar4 = (undefined4)uVar5;
        uVar3 = uVar3 + 1;
      } while (uVar3 <= (ulong)(long)param_2);
    }
    *param_3 = uVar4;
    return iVar2;
  }
  *param_3 = 0;
  return 0;
}


