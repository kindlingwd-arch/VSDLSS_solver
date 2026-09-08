/* VSDLSS function (decompiled by Ghidra) */
/* name: printIntArray_vsdlss  addr: 009d7840  size: 41 */
#include "vsdlss_ref.h"

/* signature: void printIntArray_vsdlss(uint *param_1,int param_2); */

void printIntArray_vsdlss(uint *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = 1;
  if (0 < param_2) {
    do {
      param_1 = param_1 + 1;
      iVar1 = iVar1 + 1;
      printf("%d\t",(ulong)*param_1);
    } while (iVar1 <= param_2);
  }
  putchar(10);
  return;
}


