/* VSDLSS function (decompiled by Ghidra) */
/* name: printcel_vsdlss  addr: 009e2170  size: 4d */
#include "vsdlss_ref.h"

/* signature: int printcel_vsdlss(uint *param_1); */

int printcel_vsdlss(uint *param_1)

{
  int iVar1;
  
  iVar1 = 0;
  putchar(10);
  for (; param_1 != (uint *)0x0; param_1 = *(uint **)(param_1 + 2)) {
    iVar1 = iVar1 + 1;
    printf("%d ",(ulong)*param_1);
  }
  putchar(10);
  return iVar1;
}


