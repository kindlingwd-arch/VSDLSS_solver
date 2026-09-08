/* VSDLSS function (decompiled by Ghidra) */
/* name: remFromBucketMarkInBpq_vsdlss  addr: 00a1ed40  size: 10d */
#include "vsdlss_ref.h"

/* signature: void remFromBucketMarkInBpq_vsdlss(int param_1,uint *param_2); */

void remFromBucketMarkInBpq_vsdlss(int param_1,uint *param_2)

{
  uint *puVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  long lVar7;
  int iVar8;
  
  lVar7 = *(long *)(param_2 + 8);
  uVar3 = *param_2;
  puVar2 = (uint *)(lVar7 + (long)param_1 * 4);
  uVar4 = *puVar2;
  if ((0 < (int)uVar4) && ((int)uVar4 <= (int)(uVar3 * 2))) {
    uVar5 = param_2[4];
    puVar1 = (uint *)(*(long *)(param_2 + 10) + (long)param_1 * 4);
    uVar6 = *puVar1;
    if ((int)uVar3 < (int)uVar4) {
      *(uint *)(*(long *)(param_2 + 6) + (long)(int)(uVar4 - uVar3) * 4) = uVar6;
    }
    else {
      *(uint *)(*(long *)(param_2 + 10) + (long)(int)uVar4 * 4) = uVar6;
    }
    if (uVar6 != uVar5) {
      if (((int)uVar3 < (int)uVar6) || ((int)uVar6 < 1)) {
        nrerror_vsdlss(1,"bpqutils.c",0xa8);
      }
      *(uint *)(lVar7 + (long)(int)uVar6 * 4) = uVar4;
    }
    param_2[2] = param_2[2] - 1;
    *puVar1 = param_2[5];
    *puVar2 = ~uVar3;
    param_2[5] = -param_1;
    if (*puVar1 != uVar5) {
      iVar8 = -*puVar1;
      if (((int)uVar3 < iVar8) || (iVar8 < 1)) {
        nrerror_vsdlss(1,"bpqutils.c",0xb7);
      }
      *(int *)(lVar7 + (long)iVar8 * 4) = -param_1;
    }
    param_2[3] = param_2[3] + 1;
  }
  return;
}


