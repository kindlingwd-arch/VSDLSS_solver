/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDCoarsenOneLevel_vsdlss  addr: 009feca0  size: 18d */
#include "vsdlss_ref.h"

/* signature: void MLDCoarsenOneLevel_vsdlss(undefined4 param_1,undefined4 param_2,int param_3,long param_4,long param_5); */

void MLDCoarsenOneLevel_vsdlss
               (undefined4 param_1,undefined4 param_2,int param_3,long param_4,long param_5)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  
  lVar2 = *(long *)(param_4 + (long)param_3 * 8);
  uVar3 = *(undefined8 *)(param_5 + 0x58);
  uVar4 = *(undefined8 *)(param_5 + 0x48);
  uVar5 = *(undefined8 *)(param_5 + 0x50);
  iVar1 = *(int *)(lVar2 + 8);
  uVar6 = *(undefined8 *)(param_5 + 0x60);
  uVar7 = *(undefined8 *)(param_5 + 0x90);
  uVar8 = *(undefined8 *)(param_5 + 0x98);
  *(undefined8 *)(param_5 + 0x50) = 0;
  *(undefined8 *)(param_5 + 0x58) = 0;
  *(undefined8 *)(param_5 + 0x90) = 0;
  *(undefined8 *)(param_5 + 0x98) = 0;
  *(undefined8 *)(param_5 + 0x48) = 0;
  *(undefined8 *)(param_5 + 0x60) = 0;
  initLabelVec_vsdlss(uVar3,iVar1);
  if (4 < iVar1) {
    FUN_009fbc10(uVar3,iVar1);
  }
  if (param_3 == 1) {
    uVar9 = MLDSHEMMatching_1(param_2,lVar2,uVar5,uVar3,uVar7,uVar8);
    uVar10 = MLDCreateCoarseGraph_1(param_1,uVar9,lVar2,uVar5,uVar7,uVar4,uVar6,uVar8);
  }
  else {
    uVar9 = MLDSHEMMatching_vsdlss();
    uVar10 = MLDCreateCoarseGraph_vsdlss(param_1,uVar9,lVar2,uVar5,uVar7,uVar4,uVar6,uVar8);
  }
  *(undefined8 *)(param_5 + 0x58) = uVar3;
  *(undefined8 *)(param_5 + 0x50) = uVar5;
  *(undefined8 *)(param_5 + 0x90) = uVar7;
  *(undefined8 *)(param_5 + 0x48) = uVar4;
  *(undefined8 *)(param_5 + 0x98) = uVar8;
  *(undefined8 *)(param_5 + 0x60) = uVar6;
  *(undefined8 *)(param_4 + 8 + (long)param_3 * 8) = uVar10;
  return;
}


