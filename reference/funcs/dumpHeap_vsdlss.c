/* VSDLSS function (decompiled by Ghidra) */
/* name: dumpHeap_vsdlss  addr: 00a21680  size: 83 */
#include "vsdlss_ref.h"

/* signature: void dumpHeap_vsdlss(long param_1); */

void dumpHeap_vsdlss(long param_1)

{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  long lVar4;
  
  lVar4 = 8;
  iVar3 = 1;
  printf("Number of elements in Heap is %d\n",(ulong)*(uint *)(param_1 + 4));
  puts("-------------------------------------");
  puts("\tEntry\tCost\tPosition");
  puts("\t-----\t----\t--------");
  if (0 < *(int *)(param_1 + 4)) {
    do {
      puVar2 = (uint *)(lVar4 + *(long *)(param_1 + 0x10));
      iVar3 = iVar3 + 1;
      lVar4 = lVar4 + 8;
      uVar1 = *puVar2;
      printf("\t%d\t%d\t%d\n",(ulong)uVar1,(ulong)puVar2[1],
             (ulong)*(uint *)(*(long *)(param_1 + 8) + (long)(int)uVar1 * 4));
    } while (iVar3 <= *(int *)(param_1 + 4));
  }
  return;
}


