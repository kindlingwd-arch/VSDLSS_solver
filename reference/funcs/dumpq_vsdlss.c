/* VSDLSS function (decompiled by Ghidra) */
/* name: dumpq_vsdlss  addr: 00a0d7e0  size: da */
#include "vsdlss_ref.h"

/* signature: void dumpq_vsdlss(int *param_1); */

void dumpq_vsdlss(int *param_1)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  
  iVar3 = param_1[1];
  iVar2 = *param_1;
  if (iVar3 == iVar2) {
    puts("Queue is empty.");
    return;
  }
  if (iVar3 < iVar2) {
    if (iVar2 <= param_1[2]) {
      lVar4 = (long)iVar2 << 2;
      do {
        iVar2 = iVar2 + 1;
        puVar1 = (uint *)(*(long *)(param_1 + 4) + lVar4);
        lVar4 = lVar4 + 4;
        printf(" %d",(ulong)*puVar1);
      } while (iVar2 <= param_1[2]);
      iVar3 = param_1[1];
    }
    if (1 < iVar3) {
      lVar4 = 4;
      iVar3 = 1;
      do {
        iVar3 = iVar3 + 1;
        puVar1 = (uint *)(*(long *)(param_1 + 4) + lVar4);
        lVar4 = lVar4 + 4;
        printf(" %d",(ulong)*puVar1);
      } while (iVar3 < param_1[1]);
    }
  }
  else {
    lVar4 = (long)iVar2 << 2;
    if (iVar2 < iVar3) {
      do {
        iVar2 = iVar2 + 1;
        puVar1 = (uint *)(*(long *)(param_1 + 4) + lVar4);
        lVar4 = lVar4 + 4;
        printf(" %d",(ulong)*puVar1);
      } while (iVar2 < param_1[1]);
      putchar(10);
      return;
    }
  }
  putchar(10);
  return;
}


