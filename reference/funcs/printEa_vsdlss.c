/* VSDLSS function (decompiled by Ghidra) */
/* name: printEa_vsdlss  addr: 009e6790  size: 56 */
#include "vsdlss_ref.h"

/* signature: void printEa_vsdlss(uint *param_1); */

void printEa_vsdlss(uint *param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = *param_1;
  puts("Element Array Info:\n");
  printf("Num Elems: %d;\n",(ulong)uVar1);
  putchar(10);
  puts("Num\tfromNode\ttoNode\tOffDiag");
  iVar2 = 1;
  if (0 < (int)uVar1) {
    do {
      iVar3 = iVar2 + 1;
      printElem_vsdlss(param_1,iVar2);
      iVar2 = iVar3;
    } while (iVar3 <= (int)uVar1);
  }
  return;
}


