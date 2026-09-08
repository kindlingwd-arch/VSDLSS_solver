/* VSDLSS function (decompiled by Ghidra) */
/* name: lookForFitInLastStdBlock_vsdlss  addr: 009fb420  size: 3f */
#include "vsdlss_ref.h"

/* signature: int lookForFitInLastStdBlock_vsdlss(int param_1,ulong param_2,ulong param_3); */

int lookForFitInLastStdBlock_vsdlss(int param_1,ulong param_2,ulong param_3)

{
  ulong uVar1;
  int iVar2;
  
  iVar2 = (&DAT_00ec4d80)[param_1];
  if ((-1 < iVar2) &&
     (uVar1 = *(ulong *)((long)iVar2 * 0x10 + (&DAT_00ec4180)[param_1]),
     uVar1 - (long)(int)(uVar1 % param_3) < param_2)) {
    iVar2 = -1;
  }
  return iVar2;
}


