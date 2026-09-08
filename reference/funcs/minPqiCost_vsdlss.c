/* VSDLSS function (decompiled by Ghidra) */
/* name: minPqiCost_vsdlss  addr: 00a0cce0  size: 33 */
#include "vsdlss_ref.h"

/* signature: int minPqiCost_vsdlss(int param_1,int param_2,long param_3); */

int minPqiCost_vsdlss(int param_1,int param_2,long param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = *(int *)(*(long *)(param_3 + 0x18) + (long)param_1 * 4);
  iVar2 = *(int *)(*(long *)(param_3 + 0x18) + (long)param_2 * 4);
  iVar3 = *(int *)(*(long *)(param_3 + 0x20) + (long)iVar2 * 4);
  iVar4 = *(int *)(*(long *)(param_3 + 0x20) + (long)iVar1 * 4);
  iVar5 = param_1;
  if ((iVar3 <= iVar4) && (iVar5 = param_2, iVar4 <= iVar3)) {
    if (iVar2 < iVar1) {
      param_1 = param_2;
    }
    return param_1;
  }
  return iVar5;
}


