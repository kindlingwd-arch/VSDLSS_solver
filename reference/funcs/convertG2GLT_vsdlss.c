/* VSDLSS function (decompiled by Ghidra) */
/* name: convertG2GLT_vsdlss  addr: 009f4d20  size: 68 */
#include "vsdlss_ref.h"

/* signature: void convertG2GLT_vsdlss(int *param_1); */

void convertG2GLT_vsdlss(int *param_1)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  long lVar6;
  
  lVar6 = 8;
  iVar1 = *param_1;
  iVar4 = 1;
  if (0 < iVar1) {
    do {
      piVar5 = (int *)0x0;
      piVar3 = *(int **)(*(long *)(param_1 + 6) + lVar6);
      while (piVar3 != (int *)0x0) {
        while (piVar2 = piVar3, *piVar2 <= iVar4) {
          piVar3 = *(int **)(piVar2 + 2);
          piVar5 = piVar2;
          if (*(int **)(piVar2 + 2) == (int *)0x0) goto LAB_009f4d5f;
        }
        piVar3 = (int *)snipDispCEdgelistElem_vsdlss(lVar6 + *(long *)(param_1 + 6),piVar2,piVar5);
      }
LAB_009f4d5f:
      iVar4 = iVar4 + 1;
      lVar6 = lVar6 + 8;
    } while (iVar4 <= iVar1);
  }
  return;
}


