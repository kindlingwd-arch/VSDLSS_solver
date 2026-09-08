/* VSDLSS function (decompiled by Ghidra) */
/* name: renameNodesInAdjListOfGraph_vsdlss  addr: 009f4e00  size: 52 */
#include "vsdlss_ref.h"

/* signature: void renameNodesInAdjListOfGraph_vsdlss(int *param_1,long param_2); */

void renameNodesInAdjListOfGraph_vsdlss(int *param_1,long param_2)

{
  int iVar1;
  long lVar2;
  int *piVar3;
  undefined8 *puVar4;
  
  iVar1 = *param_1;
  if (0 < iVar1) {
    lVar2 = *(long *)(param_1 + 6);
    puVar4 = (undefined8 *)(lVar2 + 8);
    do {
      for (piVar3 = (int *)*puVar4; piVar3 != (int *)0x0; piVar3 = *(int **)(piVar3 + 2)) {
        *piVar3 = *(int *)(param_2 + (long)*piVar3 * 4);
      }
      puVar4 = puVar4 + 1;
    } while (puVar4 != (undefined8 *)(lVar2 + 0x10 + (ulong)(iVar1 - 1) * 8));
    return;
  }
  return;
}


