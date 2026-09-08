/* VSDLSS function (decompiled by Ghidra) */
/* name: eliminateDuplicateEdgesInGraph_vsdlss  addr: 009f5030  size: de */
#include "vsdlss_ref.h"

/* signature: void eliminateDuplicateEdgesInGraph_vsdlss(int *param_1); */

void eliminateDuplicateEdgesInGraph_vsdlss(int *param_1)

{
  int iVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  int *piVar6;
  int iVar7;
  long lVar8;
  int iVar9;
  
  iVar1 = *param_1;
  lVar3 = newListMark_vsdlss(iVar1);
  if (iVar1 < 1) {
    iVar7 = 0;
  }
  else {
    iVar7 = 0;
    iVar9 = 1;
    lVar8 = 8;
    do {
      puVar5 = (undefined8 *)(lVar8 + *(long *)(param_1 + 6));
      piVar6 = (int *)*puVar5;
      if (piVar6 == (int *)0x0) {
        uVar4 = 0;
      }
      else {
        do {
          while( true ) {
            if (*(char *)(*(long *)(lVar3 + 8) + (long)*piVar6) != '\0') break;
            iVar7 = iVar7 + 1;
            markListMark_vsdlss(lVar3);
            piVar6 = *(int **)(piVar6 + 2);
            if (piVar6 == (int *)0x0) goto LAB_009f50b4;
          }
          piVar6 = *(int **)(piVar6 + 2);
        } while (piVar6 != (int *)0x0);
LAB_009f50b4:
        puVar5 = (undefined8 *)(lVar8 + *(long *)(param_1 + 6));
        uVar4 = *puVar5;
      }
      iVar9 = iVar9 + 1;
      uVar4 = deleteEntireCEdgelist_vsdlss(uVar4);
      *puVar5 = uVar4;
      lVar2 = *(long *)(param_1 + 6);
      uVar4 = unmarkListMarkReturnList_vsdlss();
      *(undefined8 *)(lVar8 + lVar2) = uVar4;
      lVar8 = lVar8 + 8;
    } while (iVar9 <= iVar1);
  }
  deleteListMark_vsdlss(lVar3);
  param_1[1] = iVar7;
  return;
}


