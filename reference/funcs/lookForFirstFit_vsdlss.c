/* VSDLSS function (decompiled by Ghidra) */
/* name: lookForFirstFit_vsdlss  addr: 009fb3b0  size: 6a */
#include "vsdlss_ref.h"

/* signature: int lookForFirstFit_vsdlss(int param_1,ulong param_2,ulong param_3); */

int lookForFirstFit_vsdlss(int param_1,ulong param_2,ulong param_3)

{
  ulong *puVar1;
  ulong uVar2;
  ulong *puVar3;
  int iVar4;
  
  iVar4 = -1;
  if (-1 < (int)(&DAT_00ec4980)[param_1]) {
    puVar3 = (ulong *)(&DAT_00ec4180)[param_1];
    iVar4 = 0;
    uVar2 = *puVar3 - (long)(int)(*puVar3 % param_3);
    while (uVar2 < param_2) {
      iVar4 = iVar4 + 1;
      if ((int)(&DAT_00ec4980)[param_1] < iVar4) {
        return -1;
      }
      puVar1 = puVar3 + 2;
      puVar3 = puVar3 + 2;
      uVar2 = *puVar1 - (long)(int)(*puVar1 % param_3);
    }
  }
  return iVar4;
}


