/* VSDLSS function (decompiled by Ghidra) */
/* name: getMinAndMaxIVector_vsdlss  addr: 009d8ae0  size: 8b */
#include "vsdlss_ref.h"

/* signature: void getMinAndMaxIVector_vsdlss(long param_1,int param_2,int *param_3,int *param_4,undefined4 *param_5,undefined4 *param_6); */

void getMinAndMaxIVector_vsdlss
               (long param_1,int param_2,int *param_3,int *param_4,undefined4 *param_5,
               undefined4 *param_6)

{
  int iVar1;
  ulong uVar2;
  int iVar3;
  undefined4 uVar4;
  ulong uVar5;
  ulong uVar6;
  int iVar7;
  undefined4 uVar8;
  ulong uVar9;
  
  if (param_2 < 1) {
    *param_3 = 0;
    *param_4 = 0;
    *param_5 = 0;
    *param_6 = 0;
    return;
  }
  iVar3 = *(int *)(param_1 + 4);
  uVar6 = (ulong)param_2;
  iVar7 = iVar3;
  if (uVar6 == 1) {
    uVar8 = 1;
    uVar4 = 1;
  }
  else {
    uVar2 = 2;
    uVar9 = 1;
    uVar5 = 1;
LAB_009d8b1e:
    do {
      uVar8 = (undefined4)uVar9;
      uVar4 = (undefined4)uVar5;
      iVar1 = *(int *)(param_1 + uVar2 * 4);
      if (iVar1 < iVar7) {
        uVar5 = uVar2 & 0xffffffff;
        iVar7 = iVar1;
      }
      else if (iVar3 < iVar1) {
        uVar9 = uVar2 & 0xffffffff;
        uVar8 = (undefined4)uVar2;
        uVar2 = uVar2 + 1;
        iVar3 = iVar1;
        if (uVar6 < uVar2) break;
        goto LAB_009d8b1e;
      }
      uVar4 = (undefined4)uVar5;
      uVar2 = uVar2 + 1;
    } while (uVar2 <= uVar6);
  }
  *param_3 = iVar7;
  *param_4 = iVar3;
  *param_5 = uVar4;
  *param_6 = uVar8;
  return;
}


