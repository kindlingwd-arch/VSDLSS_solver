/* VSDLSS function (decompiled by Ghidra) */
/* name: convertGUT2G_vsdlss  addr: 009f4b90  size: 69 */
#include "vsdlss_ref.h"

/* signature: void convertGUT2G_vsdlss(int *param_1); */

void convertGUT2G_vsdlss(int *param_1)

{
  undefined8 *puVar1;
  long lVar2;
  undefined8 uVar3;
  int *piVar4;
  int iVar5;
  undefined8 *puVar6;
  
  iVar5 = *param_1;
  lVar2 = *(long *)(param_1 + 6);
  puVar6 = (undefined8 *)(lVar2 + (long)iVar5 * 8);
  if (0 < iVar5) {
    do {
      for (piVar4 = (int *)*puVar6; piVar4 != (int *)0x0; piVar4 = *(int **)(piVar4 + 2)) {
        while (*piVar4 <= iVar5) {
          piVar4 = *(int **)(piVar4 + 2);
          if (piVar4 == (int *)0x0) goto LAB_009f4bed;
        }
        puVar1 = (undefined8 *)(lVar2 + (long)*piVar4 * 8);
        uVar3 = newCEdgelist_vsdlss(iVar5,*puVar1);
        *puVar1 = uVar3;
      }
LAB_009f4bed:
      puVar6 = puVar6 + -1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
  }
  return;
}


