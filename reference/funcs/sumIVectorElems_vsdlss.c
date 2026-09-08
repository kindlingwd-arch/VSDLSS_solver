/* VSDLSS function (decompiled by Ghidra) */
/* name: sumIVectorElems_vsdlss  addr: 009d9360  size: 21 */
#include "vsdlss_ref.h"

/* signature: int sumIVectorElems_vsdlss(long param_1,int param_2); */

int sumIVectorElems_vsdlss(long param_1,int param_2)

{
  int iVar1;
  ulong uVar2;
  
  if ((long)param_2 != 0) {
    uVar2 = 1;
    iVar1 = 0;
    do {
      iVar1 = iVar1 + *(int *)(param_1 + uVar2 * 4);
      uVar2 = uVar2 + 1;
    } while (uVar2 <= (ulong)(long)param_2);
    return iVar1;
  }
  return 0;
}


