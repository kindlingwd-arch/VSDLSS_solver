/* VSDLSS function (decompiled by Ghidra) */
/* name: preSolve_vsdlss  addr: 00a0b280  size: 1c5 */
#include "vsdlss_ref.h"

/* signature: void preSolve_vsdlss(undefined8 param_1,long param_2,int param_3,int *param_4,int *param_5); */

void preSolve_vsdlss(undefined8 param_1,long param_2,int param_3,int *param_4,int *param_5)

{
  int iVar1;
  undefined8 uVar2;
  long lVar3;
  long lVar4;
  undefined8 *puVar5;
  int iVar6;
  
  iVar1 = enqSolveInMem_vsdlss();
  if (iVar1 == 0) {
    lVar3 = read_DEG123REORDERVEC_vsdlss();
    puVar5 = (undefined8 *)newVector_vsdlss(0,(long)param_3,1);
    *puVar5 = 0;
    readVecFromFile_vsdlss(param_1,param_3,puVar5);
    permuteDVectorInPlace_vsdlss(lVar3,param_3,puVar5);
    if (lVar3 != 0) {
      casiFree_vsdlss(lVar3);
    }
    lVar3 = readDeg1Deg2Pf_vsdlss(0xffffffff);
    iVar6 = *(int *)(lVar3 + 4) + *(int *)(lVar3 + 8);
    solveWithDeg1Deg2PfLower_vsdlss(lVar3,puVar5,puVar5);
    deleteDeg1Deg2Pf_vsdlss(lVar3);
    write_DEG1DEG2PARTRES_vsdlss(puVar5,iVar6);
    puVar5 = puVar5 + iVar6;
    lVar3 = readDeg3Pf_vsdlss(0xffffffff);
    iVar1 = *(int *)(lVar3 + 4);
    solveWithDeg3PfLower_vsdlss(lVar3,puVar5,puVar5);
    deleteDeg3Pf_vsdlss(lVar3);
    write_DEG3PARTRES_vsdlss(puVar5,iVar1);
    writeDVectorFNum_vsdlss(5,param_3 - (iVar6 + iVar1),puVar5 + iVar1);
    if (puVar5 + iVar1 + -(long)(iVar6 + iVar1) != (undefined8 *)0x0) {
      casiFree_vsdlss();
    }
  }
  else {
    uVar2 = getPfReorderVecSave_vsdlss();
    lVar3 = getPfDeg1Deg2PartRes_vsdlss();
    if (param_2 == 0) {
      readVecFromFile_vsdlss(param_1,param_3,lVar3);
      permuteDVectorInPlace_vsdlss(uVar2,param_3,lVar3);
    }
    else {
      permuteDVector_vsdlss(uVar2,param_3,param_2,lVar3);
    }
    lVar4 = getPfSave_vsdlss();
    iVar6 = *(int *)(lVar4 + 4) + *(int *)(lVar4 + 8);
    solveWithDeg1Deg2PfLower_vsdlss(lVar4,lVar3,lVar3);
    lVar3 = lVar3 + (long)iVar6 * 8;
    lVar4 = getPf3Save_vsdlss();
    iVar1 = *(int *)(lVar4 + 4);
    solveWithDeg3PfLower_vsdlss(lVar4,lVar3,lVar3);
  }
  *param_4 = iVar6;
  *param_5 = iVar1;
  return;
}


