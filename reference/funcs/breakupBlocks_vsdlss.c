/* VSDLSS function (decompiled by Ghidra) */
/* name: breakupBlocks_vsdlss  addr: 00a1aa10  size: 82 */
#include "vsdlss_ref.h"

/* signature: int breakupBlocks_vsdlss(long param_1,int param_2,int param_3,long param_4); */

int breakupBlocks_vsdlss(long param_1,int param_2,int param_3,long param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  
  if (param_2 < 1) {
    return 0;
  }
  piVar5 = (int *)(param_1 + 4);
  piVar1 = (int *)(param_1 + 8 + (ulong)(param_2 - 1) * 4);
  iVar4 = 0;
  do {
    while (iVar2 = *piVar5, iVar2 <= param_3) {
      iVar4 = iVar4 + 1;
      piVar5 = piVar5 + 1;
      *(int *)(param_4 + (long)iVar4 * 4) = iVar2;
      if (piVar5 == piVar1) {
        return iVar4;
      }
    }
    if (0 < iVar2) {
      iVar6 = iVar2 + -1;
      piVar8 = (int *)(param_4 + 4 + (long)iVar4 * 4);
      iVar7 = 1;
      do {
        iVar3 = iVar6 + 1;
        if (param_3 <= iVar6) {
          iVar3 = param_3;
        }
        iVar7 = iVar7 + param_3;
        iVar4 = iVar4 + 1;
        *piVar8 = iVar3;
        iVar6 = iVar6 - param_3;
        piVar8 = piVar8 + 1;
      } while (iVar7 <= iVar2);
    }
    piVar5 = piVar5 + 1;
  } while (piVar5 != piVar1);
  return iVar4;
}


