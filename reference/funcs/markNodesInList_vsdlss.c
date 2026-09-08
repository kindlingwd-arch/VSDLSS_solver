/* VSDLSS function (decompiled by Ghidra) */
/* name: markNodesInList_vsdlss  addr: 00a0ffa0  size: 22 */
#include "vsdlss_ref.h"

/* signature: void markNodesInList_vsdlss(int param_1,long param_2,long param_3); */

void markNodesInList_vsdlss(int param_1,long param_2,long param_3)

{
  int iVar1;
  int *piVar2;
  
  if (0 < param_1) {
    piVar2 = (int *)(param_2 + 4);
    do {
      iVar1 = *piVar2;
      piVar2 = piVar2 + 1;
      *(undefined1 *)(param_3 + iVar1) = 1;
    } while (piVar2 != (int *)(param_2 + 8 + (ulong)(param_1 - 1) * 4));
  }
  return;
}


