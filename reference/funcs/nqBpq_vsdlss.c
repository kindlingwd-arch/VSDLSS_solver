/* VSDLSS function (decompiled by Ghidra) */
/* name: nqBpq_vsdlss  addr: 00a1ef60  size: a0 */
#include "vsdlss_ref.h"

/* signature: void nqBpq_vsdlss(int param_1,int param_2,int *param_3); */

void nqBpq_vsdlss(int param_1,int param_2,int *param_3)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  
  iVar3 = *param_3;
  if ((((param_1 < 1) || (iVar3 < param_1)) || (param_2 < 1)) || (iVar3 < param_2)) {
    nrerror_vsdlss(1,"bpqutils.c",0x104);
  }
  lVar5 = *(long *)(param_3 + 8);
  piVar1 = (int *)(*(long *)(param_3 + 6) + (long)param_2 * 4);
  piVar2 = (int *)(*(long *)(param_3 + 10) + (long)param_1 * 4);
  *piVar2 = *piVar1;
  *(int *)(lVar5 + (long)param_1 * 4) = param_2 + iVar3;
  *piVar1 = param_1;
  iVar4 = *piVar2;
  if (param_3[4] != iVar4) {
    if ((iVar3 < iVar4) || (iVar4 < 1)) {
      nrerror_vsdlss(1,"bpqutils.c",0x111);
    }
    *(int *)(lVar5 + (long)iVar4 * 4) = param_1;
  }
  param_3[2] = param_3[2] + 1;
  return;
}


