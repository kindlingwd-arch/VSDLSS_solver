/* VSDLSS function (decompiled by Ghidra) */
/* name: denseSolve_vsdlss  addr: 009e62e0  size: 7d */
#include "vsdlss_ref.h"

/* signature: undefined8 denseSolve_vsdlss(long param_1,undefined8 param_2); */

undefined8 denseSolve_vsdlss(long param_1,undefined8 param_2)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  
  iVar1 = *(int *)(param_1 + 4);
  uVar2 = newMatrix(iVar1,iVar1);
  convertElemArrToDense(param_1,uVar2);
  denseCholesky(uVar2,iVar1);
  solveDenseLower(uVar2,iVar1,param_2,param_2);
  uVar3 = newVector_vsdlss(0,(long)iVar1,0);
  solveDenseUpper(uVar2,iVar1,param_2,uVar3);
  deleteMatrix_vsdlss(uVar2,iVar1);
  return uVar3;
}


