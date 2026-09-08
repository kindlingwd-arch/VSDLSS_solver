/* VSDLSS function (decompiled by Ghidra) */
/* name: orderHighDegNodesAtEndOld_vsdlss  addr: 00a02f50  size: 126 */
#include "vsdlss_ref.h"

/* signature: int orderHighDegNodesAtEndOld_vsdlss(long param_1,long param_2,long param_3,undefined8 param_4,undefined8 param_5); */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int orderHighDegNodesAtEndOld_vsdlss
              (long param_1,long param_2,long param_3,undefined8 param_4,undefined8 param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  double dVar6;
  double dVar7;
  
  iVar1 = *(int *)(param_1 + 8);
  piVar5 = *(int **)(param_1 + 0x28);
  if (999 < iVar1) {
    dVar6 = pow((double)iVar1,_DAT_00b1d070);
    dVar6 = floor(dVar6);
    iVar3 = ((*(int *)(param_1 + 0xc) * 2) / iVar1) * 0x1e;
    dVar7 = pow((double)iVar1,DAT_00ab7228);
    dVar7 = floor(dVar7);
    iVar2 = 10;
    if (9 < iVar3) {
      iVar2 = iVar3;
    }
    iVar4 = 1;
    iVar3 = (int)dVar7;
    if ((int)dVar7 <= iVar2) {
      iVar3 = iVar2;
    }
    iVar2 = 0;
    do {
      piVar5 = piVar5 + 1;
      if (iVar3 < *piVar5) {
        iVar2 = iVar2 + 1;
        *(int *)(param_3 + (long)iVar2 * 4) = *piVar5;
        *(int *)(param_2 + (long)iVar2 * 4) = iVar4;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 != iVar1 + 1);
    if (iVar2 != 0) {
      casiRadixSortPerm_vsdlss(iVar2,param_4,param_3,param_2,param_5);
      iVar1 = 10;
      if (9 < (int)dVar6) {
        iVar1 = (int)dVar6;
      }
      if (iVar2 <= iVar1) {
        iVar1 = iVar2;
      }
      return iVar1;
    }
  }
  return 0;
}


