/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDFindPartition_vsdlss  addr: 00a00d50  size: 114 */
#include "vsdlss_ref.h"

/* signature: void MLDFindPartition_vsdlss(undefined8 param_1,undefined4 param_2,long param_3,long param_4,undefined8 param_5); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void MLDFindPartition_vsdlss
               (undefined8 param_1,undefined4 param_2,long param_3,long param_4,undefined8 param_5)

{
  int *piVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  undefined1 auStack_48 [4];
  int local_44;
  int local_40;
  
  lVar2 = *(long *)(param_3 + 0x58);
  if (*(int *)(param_3 + 8) < 1) {
    iVar6 = 0;
    iVar7 = 0;
    iVar3 = 0;
  }
  else {
    piVar5 = (int *)(lVar2 + 4);
    piVar1 = (int *)(lVar2 + 8 + (ulong)(*(int *)(param_3 + 8) - 1) * 4);
    iVar3 = 0;
    do {
      while (lVar2 != 0) {
        iVar6 = *piVar5;
        piVar5 = piVar5 + 1;
        iVar3 = iVar3 + iVar6;
        if (piVar5 == piVar1) goto LAB_00a00db0;
      }
      piVar5 = piVar5 + 1;
      iVar3 = iVar3 + 1;
    } while (piVar5 != piVar1);
LAB_00a00db0:
    iVar7 = iVar3 / 2;
    iVar6 = iVar3 - iVar7;
  }
  *(undefined4 *)(param_3 + 4) = 1;
  *(long *)(param_4 + 8) = param_3;
  iVar4 = (int)((double)(iVar3 / DAT_00ec5d90) * _DAT_00ec5da8);
  iVar3 = 2;
  if (1 < iVar4) {
    iVar3 = iVar4;
  }
  iVar4 = MLDCoarsen_vsdlss(param_1,param_2,iVar3,param_4,param_5);
  local_44 = iVar7;
  local_40 = iVar6;
  MLDInitPartition_vsdlss(param_2,auStack_48,*(undefined8 *)(param_4 + (long)iVar4 * 8),param_5);
  iVar3 = iVar4;
  if (1 < iVar4) {
    do {
      iVar6 = iVar3 + -1;
      MLDUncoarsenRefineOneLevel_vsdlss(auStack_48,iVar3,iVar4,param_4,param_5);
      iVar3 = iVar6;
    } while (iVar6 != 1);
  }
  return;
}


