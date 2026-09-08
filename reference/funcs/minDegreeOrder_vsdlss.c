/* VSDLSS function (decompiled by Ghidra) */
/* name: minDegreeOrder_vsdlss  addr: 00a27da0  size: db */
#include "vsdlss_ref.h"

/* signature: void minDegreeOrder_vsdlss(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,undefined8 *param_4); */

void minDegreeOrder_vsdlss
               (undefined8 *param_1,undefined8 *param_2,undefined8 *param_3,undefined8 *param_4)

{
  int iVar1;
  int *piVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  piVar2 = (int *)*param_1;
  iVar1 = *piVar2;
  uVar3 = newIVector_vsdlss((long)iVar1,1,0x8000000b);
  uVar4 = newIVector_vsdlss((long)iVar1,1,0);
  uVar5 = newIndistNodeSets_vsdlss(iVar1);
  if (999 < iVar1) {
    iVar1 = iVar1 >> 2;
  }
  uVar6 = newHashTable_vsdlss(iVar1);
  initIndistNodeSets_vsdlss(uVar5);
  minDegreeOrderBpq(piVar2,uVar3,uVar4,uVar5,uVar6);
  uVar7 = deleteGraph_vsdlss(piVar2);
  initOrderSelectedInProb_vsdlss = 0;
  deleteHashTable_vsdlss(uVar6);
  *param_1 = uVar7;
  *param_2 = uVar3;
  *param_3 = uVar4;
  *param_4 = uVar5;
  return;
}


