/* VSDLSS function (decompiled by Ghidra) */
/* name: getRootsInElimTree_vsdlss  addr: 00a27e80  size: 55 */
#include "vsdlss_ref.h"

/* signature: undefined8 getRootsInElimTree_vsdlss(long param_1,int param_2); */

undefined8 getRootsInElimTree_vsdlss(long param_1,int param_2)

{
  undefined8 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  if (param_2 < 1) {
    uVar1 = 0;
  }
  else {
    piVar4 = (int *)(param_1 + 4);
    uVar1 = 0;
    iVar2 = 1;
    do {
      while ((*piVar4 != -0x7ffffff5 && (*piVar4 < 0))) {
        iVar3 = iVar2 + 1;
        uVar1 = newCEdgelist_vsdlss(iVar2,uVar1);
        piVar4 = piVar4 + 1;
        iVar2 = iVar3;
        if (param_2 < iVar3) {
          return uVar1;
        }
      }
      iVar2 = iVar2 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar2 <= param_2);
  }
  return uVar1;
}


