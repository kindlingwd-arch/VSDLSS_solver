/* VSDLSS function (decompiled by Ghidra) */
/* name: deleteIndistNodeSets_vsdlss  addr: 00a21d10  size: 71 */
#include "vsdlss_ref.h"

/* signature: undefined8 deleteIndistNodeSets_vsdlss(int *param_1); */

undefined8 deleteIndistNodeSets_vsdlss(int *param_1)

{
  int iVar1;
  undefined8 uVar2;
  int iVar3;
  long lVar4;
  undefined8 *puVar5;
  
  if (param_1 != (int *)0x0) {
    iVar1 = *param_1;
    lVar4 = 8;
    iVar3 = 1;
    if (0 < iVar1) {
      do {
        puVar5 = (undefined8 *)(lVar4 + *(long *)(param_1 + 2));
        iVar3 = iVar3 + 1;
        lVar4 = lVar4 + 8;
        uVar2 = deleteEntireCEdgelist_vsdlss(*puVar5);
        *puVar5 = uVar2;
      } while (iVar3 <= iVar1);
    }
    if (*(long *)(param_1 + 2) != 0) {
      casiFree_vsdlss();
    }
    param_1[2] = 0;
    param_1[3] = 0;
    casiFree_vsdlss(param_1);
  }
  return 0;
}


