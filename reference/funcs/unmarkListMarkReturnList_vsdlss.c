/* VSDLSS function (decompiled by Ghidra) */
/* name: unmarkListMarkReturnList_vsdlss  addr: 009f69d0  size: 31 */
#include "vsdlss_ref.h"

/* signature: void unmarkListMarkReturnList_vsdlss(long param_1); */

void unmarkListMarkReturnList_vsdlss(long param_1)

{
  int *piVar1;
  long lVar2;
  
  lVar2 = *(long *)(param_1 + 8);
  for (piVar1 = *(int **)(param_1 + 0x10); piVar1 != (int *)0x0; piVar1 = *(int **)(piVar1 + 2)) {
    *(undefined1 *)(lVar2 + *piVar1) = 0;
  }
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  return;
}


