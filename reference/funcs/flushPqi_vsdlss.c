/* VSDLSS function (decompiled by Ghidra) */
/* name: flushPqi_vsdlss  addr: 00a0d2d0  size: 3e */
#include "vsdlss_ref.h"

/* signature: void flushPqi_vsdlss(long param_1); */

void flushPqi_vsdlss(long param_1)

{
  long lVar1;
  int iVar2;
  int *piVar3;
  
  if (0 < *(int *)(param_1 + 8)) {
    piVar3 = *(int **)(param_1 + 0x18);
    lVar1 = *(long *)(param_1 + 0x10);
    iVar2 = 1;
    do {
      piVar3 = piVar3 + 1;
      iVar2 = iVar2 + 1;
      *(undefined4 *)(lVar1 + (long)*piVar3 * 4) = 0xffffffff;
    } while (iVar2 <= *(int *)(param_1 + 8));
  }
  *(undefined4 *)(param_1 + 8) = 0;
  return;
}


