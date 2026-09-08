/* VSDLSS function (decompiled by Ghidra) */
/* name: convertChildToParentToGraph_vsdlss  addr: 009f4f90  size: 92 */
#include "vsdlss_ref.h"

/* signature: long convertChildToParentToGraph_vsdlss(int *param_1,int param_2); */

long convertChildToParentToGraph_vsdlss(int *param_1,int param_2)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 uVar3;
  int iVar4;
  int iVar5;
  
  lVar2 = newGraph_vsdlss(param_2);
  uVar3 = newCEdgeVector_vsdlss(param_2,1,0);
  *(undefined8 *)(lVar2 + 0x18) = uVar3;
  if (param_2 < 1) {
    iVar5 = 0;
  }
  else {
    iVar4 = 1;
    iVar5 = 0;
    do {
      param_1 = param_1 + 1;
      if (0 < *param_1) {
        iVar5 = iVar5 + 1;
        puVar1 = (undefined8 *)(*(long *)(lVar2 + 0x18) + (long)*param_1 * 8);
        uVar3 = newCEdgelist_vsdlss(iVar4,*puVar1);
        *puVar1 = uVar3;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 <= param_2);
  }
  *(int *)(lVar2 + 4) = iVar5;
  return lVar2;
}


