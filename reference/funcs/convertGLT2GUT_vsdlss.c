/* VSDLSS function (decompiled by Ghidra) */
/* name: convertGLT2GUT_vsdlss  addr: 009f4c00  size: 80 */
#include "vsdlss_ref.h"

/* signature: void convertGLT2GUT_vsdlss(int *param_1); */

void convertGLT2GUT_vsdlss(int *param_1)

{
  undefined8 *puVar1;
  int iVar2;
  long lVar3;
  undefined8 uVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  undefined8 *puVar8;
  int *piVar9;
  int *local_30;
  
  iVar7 = *param_1;
  lVar3 = *(long *)(param_1 + 6);
  puVar8 = (undefined8 *)(lVar3 + (long)iVar7 * 8);
  if (0 < iVar7) {
    do {
      piVar6 = (int *)*puVar8;
      piVar9 = (int *)0x0;
      while (piVar6 != (int *)0x0) {
        while (piVar5 = piVar6, iVar2 = *piVar5, iVar7 <= iVar2) {
          piVar6 = *(int **)(piVar5 + 2);
          piVar9 = piVar5;
          if (*(int **)(piVar5 + 2) == (int *)0x0) goto LAB_009f4c3f;
        }
        piVar6 = (int *)snipAndReturnCEdgelistElem_vsdlss(puVar8,piVar5,piVar9,&local_30);
        puVar1 = (undefined8 *)(lVar3 + (long)iVar2 * 8);
        uVar4 = *puVar1;
        *local_30 = iVar7;
        *(undefined8 *)(local_30 + 2) = uVar4;
        *puVar1 = local_30;
      }
LAB_009f4c3f:
      puVar8 = puVar8 + -1;
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
  }
  return;
}


