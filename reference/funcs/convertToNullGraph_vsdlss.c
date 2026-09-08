/* VSDLSS function (decompiled by Ghidra) */
/* name: convertToNullGraph_vsdlss  addr: 00a08100  size: d6 */
#include "vsdlss_ref.h"

/* signature: void convertToNullGraph_vsdlss(int *param_1); */

void convertToNullGraph_vsdlss(int *param_1)

{
  undefined8 *puVar1;
  int iVar2;
  undefined8 uVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  
  if (param_1 == (int *)0x0) {
    return;
  }
  if (*(long *)(param_1 + 2) != 0) {
    casiFree_vsdlss();
  }
  param_1[2] = 0;
  param_1[3] = 0;
  if (*(long *)(param_1 + 4) != 0) {
    casiFree_vsdlss();
  }
  lVar5 = *(long *)(param_1 + 6);
  param_1[4] = 0;
  param_1[5] = 0;
  if (lVar5 != 0) {
    if (0 < *param_1) {
      lVar6 = 8;
      iVar4 = 1;
      do {
        puVar1 = (undefined8 *)(lVar5 + lVar6);
        iVar4 = iVar4 + 1;
        lVar6 = lVar6 + 8;
        uVar3 = deleteEntireCEdgelist_vsdlss(*puVar1);
        iVar2 = *param_1;
        *puVar1 = uVar3;
        lVar5 = *(long *)(param_1 + 6);
      } while (iVar4 <= iVar2);
      if (lVar5 == 0) goto LAB_00a0818a;
    }
    casiFree_vsdlss();
  }
LAB_00a0818a:
  param_1[6] = 0;
  param_1[7] = 0;
  if (*(long *)(param_1 + 8) != 0) {
    casiFree_vsdlss();
  }
  param_1[8] = 0;
  param_1[9] = 0;
  if (*(long *)(param_1 + 10) != 0) {
    casiFree_vsdlss();
  }
  param_1[10] = 0;
  param_1[0xb] = 0;
  *param_1 = 0;
  param_1[1] = 0;
  return;
}


