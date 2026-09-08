/* VSDLSS function (decompiled by Ghidra) */
/* name: MLDMarkInCMNodesInArr_vsdlss  addr: 00a28da0  size: 32 */
#include "vsdlss_ref.h"

/* signature: void MLDMarkInCMNodesInArr_vsdlss(int param_1,long param_2,undefined4 *param_3); */

void MLDMarkInCMNodesInArr_vsdlss(int param_1,long param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  
  lVar3 = *(long *)(param_3 + 2);
  uVar1 = *param_3;
  if (0 < param_1) {
    piVar4 = (int *)(param_2 + 4);
    do {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *(undefined4 *)(lVar3 + (long)iVar2 * 4) = uVar1;
    } while (piVar4 != (int *)(param_2 + 8 + (ulong)(param_1 - 1) * 4));
  }
  return;
}


