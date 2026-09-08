/* VSDLSS function (decompiled by Ghidra) */
/* name: pqSwap_vsdlss  addr: 00a2c6c0  size: 2f */
#include "vsdlss_ref.h"

/* signature: void pqSwap_vsdlss(int param_1,int param_2,long param_3); */

void pqSwap_vsdlss(int param_1,int param_2,long param_3)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  long lVar4;
  
  lVar4 = *(long *)(param_3 + 0x10);
  piVar1 = (int *)(*(long *)(param_3 + 0x18) + (long)param_1 * 4);
  piVar2 = (int *)(*(long *)(param_3 + 0x18) + (long)param_2 * 4);
  iVar3 = *piVar1;
  *piVar1 = *piVar2;
  *piVar2 = iVar3;
  *(int *)(lVar4 + (long)*piVar1 * 4) = param_1;
  *(int *)(lVar4 + (long)*piVar2 * 4) = param_2;
  return;
}


