/* VSDLSS function (decompiled by Ghidra) */
/* name: getSortedOrder_vsdlss  addr: 009e6a00  size: 11f */
#include "vsdlss_ref.h"

/* signature: undefined8 getSortedOrder_vsdlss(int *param_1,undefined8 param_2); */

undefined8 getSortedOrder_vsdlss(int *param_1,undefined8 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  
  iVar1 = param_1[1];
  iVar2 = *param_1;
  lVar6 = *(long *)(param_1 + 6);
  lVar7 = *(long *)(param_1 + 10);
  uVar5 = newIVector_vsdlss((long)iVar2,0,0);
  if (0 < iVar2) {
    lVar8 = 0;
    do {
      iVar3 = *(int *)(lVar6 + 4 + lVar8);
      iVar4 = *(int *)(lVar7 + 4 + lVar8);
      if (iVar4 < iVar3) {
        *(int *)(lVar6 + 4 + lVar8) = iVar4;
        *(int *)(lVar7 + 4 + lVar8) = iVar3;
      }
      lVar8 = lVar8 + 4;
    } while (lVar8 != (ulong)(iVar2 - 1) * 4 + 4);
  }
  initLabelVec_vsdlss(uVar5,iVar2);
  lVar6 = newIVector_vsdlss((long)iVar1,0,0);
  lVar7 = newIVector_vsdlss((long)iVar1,0,0);
  sortOnDigit_vsdlss(*(undefined8 *)(param_1 + 10),iVar2,iVar1,uVar5,lVar6,lVar7,param_2);
  sortOnDigit_vsdlss(*(undefined8 *)(param_1 + 6),iVar2,iVar1,uVar5,lVar6,lVar7,param_2);
  if (lVar7 != 0) {
    casiFree_vsdlss(lVar7);
  }
  if (lVar6 != 0) {
    casiFree_vsdlss(lVar6);
  }
  getInverseMap_vsdlss(uVar5,iVar2,param_2);
  copyIVector_vsdlss(param_2,(long)iVar2,uVar5);
  return uVar5;
}


