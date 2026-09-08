/* VSDLSS function (decompiled by Ghidra) */
/* name: nq_vsdlss  addr: 00a0d6f0  size: 85 */
#include "vsdlss_ref.h"

/* signature: void nq_vsdlss(int *param_1,undefined4 param_2); */

void nq_vsdlss(int *param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  long lVar3;
  
  iVar1 = param_1[1];
  iVar2 = *param_1;
  if (iVar2 == iVar1 + 1) {
    nrerror_vsdlss(0x35,"queueutils.c",0xa3,"queue");
    iVar2 = *param_1;
    iVar1 = param_1[1];
  }
  lVar3 = (long)iVar1;
  if ((iVar2 == 0) && (param_1[2] == iVar1)) {
    nrerror_vsdlss(0x35,"queueutils.c",0xa8,"queue");
    lVar3 = (long)param_1[1];
  }
  *(undefined4 *)(*(long *)(param_1 + 4) + lVar3 * 4) = param_2;
  iVar1 = param_1[1];
  param_1[1] = iVar1 + 1;
  if (param_1[2] < iVar1 + 1) {
    param_1[1] = 0;
  }
  return;
}


