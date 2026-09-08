/* VSDLSS function (decompiled by Ghidra) */
/* name: initIndistNodeSets_vsdlss  addr: 00a221f0  size: 49 */
#include "vsdlss_ref.h"

/* signature: void initIndistNodeSets_vsdlss(int *param_1); */

void initIndistNodeSets_vsdlss(int *param_1)

{
  int iVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  undefined8 *puVar6;
  
  lVar5 = 8;
  iVar1 = *param_1;
  iVar3 = 1;
  if (0 < iVar1) {
    do {
      puVar6 = (undefined8 *)(lVar5 + *(long *)(param_1 + 2));
      iVar4 = iVar3 + 1;
      lVar5 = lVar5 + 8;
      uVar2 = newCEdgelist_vsdlss(iVar3,0);
      *puVar6 = uVar2;
      iVar3 = iVar4;
    } while (iVar4 <= iVar1);
  }
  return;
}


