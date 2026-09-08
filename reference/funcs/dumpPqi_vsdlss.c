/* VSDLSS function (decompiled by Ghidra) */
/* name: dumpPqi_vsdlss  addr: 00a0cbf0  size: 86 */
#include "vsdlss_ref.h"

/* signature: void dumpPqi_vsdlss(long param_1); */

void dumpPqi_vsdlss(long param_1)

{
  uint uVar1;
  int iVar2;
  long lVar3;
  
  lVar3 = 4;
  iVar2 = 1;
  printf("Number of elements in Pqi is %d\n",(ulong)*(uint *)(param_1 + 8));
  puts("-------------------------------------");
  puts("\tEntry\tCost\tPosition");
  puts("\t-----\t----\t--------");
  if (0 < *(int *)(param_1 + 8)) {
    do {
      iVar2 = iVar2 + 1;
      uVar1 = *(uint *)(*(long *)(param_1 + 0x18) + lVar3);
      lVar3 = lVar3 + 4;
      printf("\t%d\t%d\t%d\n",(ulong)uVar1,
             (ulong)*(uint *)(*(long *)(param_1 + 0x20) + (long)(int)uVar1 * 4),
             (ulong)*(uint *)(*(long *)(param_1 + 0x10) + (long)(int)uVar1 * 4));
    } while (iVar2 <= *(int *)(param_1 + 8));
  }
  return;
}


