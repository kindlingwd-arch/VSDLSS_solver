/* VSDLSS function (decompiled by Ghidra) */
/* name: postSolve_vsdlss  addr: 00a0b920  size: 1e8 */
#include "vsdlss_ref.h"

/* signature: void postSolve_vsdlss(undefined8 param_1,long param_2,int param_3,int *param_4,int *param_5,long param_6); */

void postSolve_vsdlss(undefined8 param_1,long param_2,int param_3,int *param_4,int *param_5,
                     long param_6)

{
  int iVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = *param_4;
  iVar2 = *param_5;
  iVar1 = enqSolveInMem_vsdlss();
  iVar4 = param_3 - (iVar5 + iVar2);
  param_6 = param_6 + (long)(iVar5 + iVar2) * 8;
  if (iVar4 < 1) {
    if (iVar1 == 0) goto LAB_00a0ba49;
  }
  else if (iVar1 == 0) {
    readVecFromFile_vsdlss(param_1,iVar4,param_6);
LAB_00a0ba49:
    lVar3 = readDeg3Pf_vsdlss(0xffffffff);
    iVar5 = *(int *)(lVar3 + 4);
    param_6 = param_6 + (long)iVar5 * -8;
    iVar2 = read_DEG3PARTRESNoNewVec_vsdlss(param_6);
    if (iVar2 != iVar5) {
      nrerror_vsdlss(1,"prepostsolve.c",0x285);
    }
    solveWithDeg3PfUpper_vsdlss(lVar3,param_6,param_6);
    deleteDeg3Pf_vsdlss(lVar3);
    lVar3 = readDeg1Deg2Pf_vsdlss(0xffffffff);
    iVar4 = *(int *)(lVar3 + 4) + *(int *)(lVar3 + 8);
    param_6 = param_6 + (long)iVar4 * -8;
    iVar2 = read_DEG1DEG2PARTRESNoNewVec_vsdlss(param_6);
    if (iVar2 != iVar4) {
      nrerror_vsdlss(1,"prepostsolve.c",0x2a3);
    }
    solveWithDeg1Deg2PfUpper_vsdlss(lVar3,param_6,param_6);
    deleteDeg1Deg2Pf_vsdlss(lVar3);
    lVar3 = read_DEG123REORDERVEC_vsdlss();
    goto LAB_00a0b9bc;
  }
  lVar3 = getPf3Save_vsdlss();
  iVar5 = *(int *)(lVar3 + 4);
  param_6 = param_6 + (long)iVar5 * -8;
  solveWithDeg3PfUpper_vsdlss(lVar3,param_6,param_6);
  lVar3 = getPfSave_vsdlss();
  iVar4 = *(int *)(lVar3 + 4) + *(int *)(lVar3 + 8);
  param_6 = param_6 + (long)iVar4 * -8;
  solveWithDeg1Deg2PfUpper_vsdlss(lVar3,param_6,param_6);
  lVar3 = getPfReorderVecSave_vsdlss();
LAB_00a0b9bc:
  if ((iVar1 == 0) || (param_2 == 0)) {
    invPermuteDVectorInPlace_vsdlss(lVar3,param_3,param_6);
    writeVecToFile_vsdlss(param_1,param_3,param_6);
    if ((iVar1 == 0) && (lVar3 != 0)) {
      casiFree_vsdlss(lVar3);
    }
  }
  else {
    invPermuteDVector_vsdlss(lVar3,param_3,param_6,param_2);
  }
  *param_4 = iVar4;
  *param_5 = iVar5;
  return;
}


