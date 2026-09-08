/* VSDLSS function (decompiled by Ghidra) */
/* name: solveLoadCaseBlocked_vsdlss  addr: 00a1a240  size: bd */
#include "vsdlss_ref.h"

/* signature: void solveLoadCaseBlocked_vsdlss(long param_1,long param_2); */

void solveLoadCaseBlocked_vsdlss(long param_1,long param_2)

{
  int iVar1;
  undefined4 uVar2;
  long lVar3;
  long lVar4;
  
  if (*(int *)(param_1 + 0x58) != 0) {
    iVar1 = *(int *)(param_1 + 8);
    uVar2 = *(undefined4 *)(param_1 + 0x14);
    forwardSolveBlocked_LRange(param_1,param_2,1,uVar2,1,iVar1,iVar1,param_2 + (long)iVar1 * 8);
    lVar3 = *(long *)(param_1 + 0x50);
    if (0 < iVar1) {
      lVar4 = 0;
      do {
        *(double *)(param_2 + 8 + lVar4) =
             *(double *)(param_2 + 8 + lVar4) * *(double *)(lVar3 + 8 + lVar4);
        lVar4 = lVar4 + 8;
      } while (lVar4 != (ulong)(iVar1 - 1) * 8 + 8);
    }
    backwardSolveBlocked_LRange(param_1,param_2,1,uVar2,iVar1);
    return;
  }
  forwardSolveBlocked_vsdlss();
  backwardSolveBlocked_vsdlss(param_1,param_2);
  return;
}


