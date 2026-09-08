/* VSDLSS function (decompiled by Ghidra) */
/* name: unmarkListMark_vsdlss  addr: 009f6a10  size: 43 */
#include "vsdlss_ref.h"

/* signature: void unmarkListMark_vsdlss(long param_1); */

void unmarkListMark_vsdlss(long param_1)

{
  long lVar1;
  int *piVar2;
  undefined8 uVar3;
  
  piVar2 = *(int **)(param_1 + 0x10);
  lVar1 = *(long *)(param_1 + 8);
  if (piVar2 == (int *)0x0) {
    uVar3 = 0;
  }
  else {
    do {
      *(undefined1 *)(lVar1 + *piVar2) = 0;
      piVar2 = *(int **)(piVar2 + 2);
    } while (piVar2 != (int *)0x0);
    uVar3 = *(undefined8 *)(param_1 + 0x10);
  }
  uVar3 = deleteEntireCEdgelist_vsdlss(uVar3);
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = uVar3;
  return;
}


