/* VSDLSS function (decompiled by Ghidra) */
/* name: remFromBucketBpq_vsdlss  addr: 00a1f110  size: 61 */
#include "vsdlss_ref.h"

/* signature: void remFromBucketBpq_vsdlss(int param_1,int *param_2); */

void remFromBucketBpq_vsdlss(int param_1,int *param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  int *piVar7;
  
  lVar6 = *(long *)(param_2 + 8);
  iVar2 = *param_2;
  iVar3 = param_2[4];
  piVar1 = (int *)(lVar6 + (long)param_1 * 4);
  iVar4 = *piVar1;
  if ((0 < iVar4) && (iVar4 <= iVar2 * 2)) {
    piVar7 = (int *)((long)param_1 * 4 + *(long *)(param_2 + 10));
    iVar5 = *piVar7;
    if (iVar2 < iVar4) {
      *(int *)(*(long *)(param_2 + 6) + (long)(iVar4 - iVar2) * 4) = iVar5;
    }
    else {
      *(int *)(*(long *)(param_2 + 10) + (long)iVar4 * 4) = iVar5;
    }
    if (iVar5 != iVar3) {
      *(int *)(lVar6 + (long)iVar5 * 4) = iVar4;
    }
    param_2[2] = param_2[2] + -1;
    *piVar1 = iVar3;
    *piVar7 = iVar3;
  }
  return;
}


