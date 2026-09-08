/* VSDLSS function (decompiled by Ghidra) */
/* name: dumpAStack_vsdlss  addr: 00a1e930  size: 75 */
#include "vsdlss_ref.h"

/* signature: void dumpAStack_vsdlss(long param_1); */

void dumpAStack_vsdlss(long param_1)

{
  uint *puVar1;
  int iVar2;
  long lVar3;
  
  printf("AStack size %d\n",(ulong)*(uint *)(param_1 + 8));
  if (*(int *)(param_1 + 4) != 0) {
    lVar3 = 4;
    iVar2 = 1;
    puts("AStack is printed base to top.");
    if (0 < *(int *)(param_1 + 4)) {
      do {
        iVar2 = iVar2 + 1;
        puVar1 = (uint *)(*(long *)(param_1 + 0x10) + lVar3);
        lVar3 = lVar3 + 4;
        printf(" %d\n",(ulong)*puVar1);
      } while (iVar2 <= *(int *)(param_1 + 4));
    }
    return;
  }
  puts("AStack is empty.");
  return;
}


