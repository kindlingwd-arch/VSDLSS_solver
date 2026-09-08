/* VSDLSS function (decompiled by Ghidra) */
/* name: printElem_vsdlss  addr: 009e6730  size: 29 */
#include "vsdlss_ref.h"

/* signature: void printElem_vsdlss(long param_1,int param_2); */

void printElem_vsdlss(long param_1,int param_2)

{
  long lVar1;
  
  lVar1 = (long)param_2;
  printf("%d\t%d\t%d\t%f\n",*(undefined8 *)(*(long *)(param_1 + 0x30) + lVar1 * 8),param_2,
         (ulong)*(uint *)(*(long *)(param_1 + 0x18) + lVar1 * 4),
         (ulong)*(uint *)(*(long *)(param_1 + 0x28) + lVar1 * 4));
  return;
}


