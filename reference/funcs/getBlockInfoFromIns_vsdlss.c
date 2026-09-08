/* VSDLSS function (decompiled by Ghidra) */
/* name: getBlockInfoFromIns_vsdlss  addr: 00a22060  size: 14c */
#include "vsdlss_ref.h"

/* signature: long getBlockInfoFromIns_vsdlss(undefined4 param_1,long param_2,undefined8 *param_3); */

long getBlockInfoFromIns_vsdlss(undefined4 param_1,long param_2,undefined8 *param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  ulong uVar11;
  int iVar12;
  ulong uVar13;
  
  iVar8 = *(int *)*param_3;
  lVar6 = renameInsAndGetNodeToBlock_vsdlss(param_2,param_3);
  iVar1 = *(int *)(lVar6 + (long)iVar8 * 4);
  uVar13 = (ulong)iVar1;
  lVar7 = newBlockInfo_vsdlss(param_1,iVar1);
  piVar3 = *(int **)(lVar7 + 0x10);
  lVar4 = *(long *)(lVar7 + 8);
  lVar5 = *(long *)(lVar7 + 0x30);
  initIVector_vsdlss(piVar3,uVar13,0);
  if (0 < iVar8) {
    piVar9 = (int *)(param_2 + 4);
    do {
      iVar12 = *piVar9;
      piVar9 = piVar9 + 1;
      piVar3[*(int *)(lVar6 + (long)iVar12 * 4)] = piVar3[*(int *)(lVar6 + (long)iVar12 * 4)] + 1;
    } while (piVar9 != (int *)(param_2 + 8 + (ulong)(iVar8 - 1) * 4));
  }
  if (0 < iVar1) {
    iVar12 = 0;
    iVar8 = 1;
    piVar9 = piVar3;
    do {
      piVar9 = piVar9 + 1;
      iVar2 = *piVar9;
      if (0 < iVar2) {
        piVar10 = (int *)(lVar5 + 4 + (long)iVar12 * 4);
        do {
          *piVar10 = iVar8;
          piVar10 = piVar10 + 1;
        } while (piVar10 != (int *)(lVar5 + 8 + ((long)iVar12 + (ulong)(iVar2 - 1)) * 4));
        iVar12 = iVar12 + iVar2;
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 != iVar1 + 1);
  }
  if (lVar6 != 0) {
    casiFree_vsdlss(lVar6);
  }
  if ((0 < iVar1) && (*(undefined4 *)(lVar4 + 4) = 1, 1 < uVar13)) {
    uVar11 = 2;
    do {
      *(int *)(lVar4 + uVar11 * 4) = *(int *)(lVar4 + -4 + uVar11 * 4) + piVar3[uVar11 - 1];
      uVar11 = uVar11 + 1;
    } while (uVar11 <= uVar13);
  }
  return lVar7;
}


