/* VSDLSS function (decompiled by Ghidra) */
/* name: convertFromNodeToNonZeros_vsdlss  addr: 009e6d90  size: 79 */
#include "vsdlss_ref.h"

/* signature: void convertFromNodeToNonZeros_vsdlss(int *param_1); */

void convertFromNodeToNonZeros_vsdlss(int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  int *piVar6;
  
  iVar2 = *param_1;
  lVar4 = *(long *)(param_1 + 6);
  lVar5 = newIVector_vsdlss((long)param_1[1],1,0);
  if (0 < iVar2) {
    piVar6 = (int *)(lVar4 + 4);
    do {
      iVar3 = *piVar6;
      piVar6 = piVar6 + 1;
      piVar1 = (int *)(lVar5 + (long)iVar3 * 4);
      *piVar1 = *piVar1 + 1;
    } while (piVar6 != (int *)(lVar4 + 8 + (ulong)(iVar2 - 1) * 4));
  }
  if (*(long *)(param_1 + 6) != 0) {
    casiFree_vsdlss();
  }
  param_1[6] = 0;
  param_1[7] = 0;
  *(long *)(param_1 + 8) = lVar5;
  *(undefined1 *)(param_1 + 4) = 1;
  return;
}


