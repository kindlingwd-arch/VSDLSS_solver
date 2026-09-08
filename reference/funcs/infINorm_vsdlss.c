/* VSDLSS function (decompiled by Ghidra) */
/* name: infINorm_vsdlss  addr: 009d7f50  size: 45 */
#include "vsdlss_ref.h"

/* signature: int infINorm_vsdlss(long param_1,int param_2); */

int infINorm_vsdlss(long param_1,int param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  ulong uVar4;
  
  if (param_2 < 1) {
    iVar1 = 0;
  }
  else {
    uVar3 = *(int *)(param_1 + 4) >> 0x1f;
    iVar1 = (*(uint *)(param_1 + 4) ^ uVar3) - uVar3;
    if ((long)param_2 != 1) {
      uVar4 = 2;
      do {
        uVar3 = *(int *)(param_1 + uVar4 * 4) >> 0x1f;
        iVar2 = (*(uint *)(param_1 + uVar4 * 4) ^ uVar3) - uVar3;
        if (iVar1 < iVar2) {
          iVar1 = iVar2;
        }
        uVar4 = uVar4 + 1;
      } while (uVar4 <= (ulong)(long)param_2);
      return iVar1;
    }
  }
  return iVar1;
}


