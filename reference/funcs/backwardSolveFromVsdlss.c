/* VSDLSS function (decompiled by Ghidra) */
/* name: backwardSolveFromVsdlss  addr: 00a12e80  size: ae */
#include "vsdlss_ref.h"

/* signature: void backwardSolveFromVsdlss(undefined8 param_1); */

void backwardSolveFromVsdlss(undefined8 param_1)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  int iVar5;
  long lVar6;
  
  lVar4 = DAT_00ecb058;
  uVar3 = DAT_00ecb050;
  iVar1 = *(int *)(DAT_00ecb058 + 8);
  lVar6 = getFbspi();
  lVar2 = DAT_00ecb040;
  iVar5 = enqThreadsInPool();
  setFbsParWorkVecsAndSize(lVar2 + (long)iVar1 * 8,(iVar1 - *(int *)(lVar6 + 8)) * iVar5);
  if (DAT_00ecb048 == 0) {
    backwardSolveBlocked_mt(lVar4,lVar6,lVar2);
    if (casiParSolverF == 0) {
      invPermuteDVector_vsdlss();
    }
    else {
      invPermuteDVectorPar(uVar3,iVar1,lVar2,param_1);
    }
  }
  else {
    backwardSolveBlocked_mt(lVar4,lVar6,param_1);
  }
  setFbsParWorkVecsAndSize(0,0);
  return;
}


